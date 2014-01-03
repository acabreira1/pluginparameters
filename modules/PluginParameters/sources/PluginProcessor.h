/*
  ==============================================================================

   This file is part of the PluginParameters module.
   Copyright 2012-13 by MarC

  ------------------------------------------------------------------------------

   PluginParameters can be redistributed and/or modified under the terms of the 
   GNU General Public License (Version 2), as published by the Free Software 
   Foundation. A copy of the license is included in the JUCE distribution, or 
   can be found online at www.gnu.org/licenses.

   PluginParameters is distributed in the hope that it will be useful, but 
   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
   FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more 
   details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses PluginParameters, commercial 
   licenses are available. For more information, please send me a PM (Personal 
   Message) or reply to this thread at the JUCE forum: 
   http://www.juce.com/forum/topic/juce-module-automatically-handle-plugin-parameters

  ==============================================================================
*/

#ifndef __PLUGINPARAMETERS_PLUGINPROCESSOR_HEADER__
#define __PLUGINPARAMETERS_PLUGINPROCESSOR_HEADER__

#include "ParamGroups.h"

class runAfterParamChangeThread : public ThreadPoolJob {
  ParamGroup *paramGroup;
  int groupIndex;
  PluginParameters::UpdateFromFlags updateFromFlag;

  JobStatus runJob() override;

public:
  runAfterParamChangeThread(ParamGroup *paramGroup,int groupIndex, PluginParameters::UpdateFromFlags updateFromFlag)
    :ThreadPoolJob(String(paramGroup->getName())+" "+String(groupIndex)),
    paramGroup(paramGroup),
    groupIndex(groupIndex),
    updateFromFlag(updateFromFlag)
  {
  }
};

/**  methods to redirect all parameters to the global list referred by the host        
       and handle automatically automated and not automated parameters */
class PluginProcessor : public AudioProcessor, public ParamGroup{
private:

  ThreadPool runAfterParamChangeThreads;

  ParamGroup **groupAutomated;
  int *indexInGroupAutomated;
  ParamGroup **groupNonAutomated;
  int *indexInGroupNonAutomated;

  /** computes applyRecursively all the mappings between the global index 
    of parameters and their ParamGroup and index in the ParamGroup */
  void mapGlobalIndex(ParamGroup *root, bool countIfAutomate, ParamGroup **map,int *indexMap)  {
    for (int i=0;i<root->getNumParams();i++){      
      Param* param=root->getParam(i);
      if (param->registeredAtHost()==countIfAutomate){
        jassert(param->getGlobalIndex()<((countIfAutomate)?getNumAutomatedParams():getNumNonAutomatedParams()));
        map[param->getGlobalIndex()]=root;
        indexMap[param->getGlobalIndex()]=i;
      }
    }
    for (int g=0;g<root->getNumParamGroups();g++)
      mapGlobalIndex(root->getParamGroup(g),countIfAutomate,map,indexMap);    
  }
  
public:            
  const String getName() const { return JucePlugin_Name; }
  
  bool acceptsMidi() const{
  #if JucePlugin_WantsMidiInput
    return true;
  #else
    return false;
  #endif
  }

  bool producesMidi() const{
  #if JucePlugin_ProducesMidiOutput
    return true;
  #else
    return false;
  #endif
  } 

  bool silenceInProducesSilenceOut() const{
  #if JucePlugin_SilenceInProducesSilenceOut
    return true;
  #else
    return false;
  #endif
  }
  
  /** generalization of setParameterNotifyingHost to be able to deal transparently
      with automated and non automated parameters */ 
  void updateHostAndUi(Param *const param, float newValue,bool runAfterParamChange=true,bool updateUi=true){             
    const int globalIndex=param->getGlobalIndex();
    ParamGroup *localParamGroup;
    int groupIndex;
    if (param->registeredAtHost()){ //if automated, notify host     
                                  //and get corresponding localParamGroup and groupIndex 
      //notify host      
      sendParamChangeMessageToListeners (globalIndex, newValue);      
      localParamGroup=groupAutomated[globalIndex];
      groupIndex=indexInGroupAutomated[globalIndex];
      
    } else { //if not automated skip host notification
             // but get corresponding localParamGroup and groupIndex
      localParamGroup=groupNonAutomated[globalIndex];
      groupIndex=indexInGroupNonAutomated[globalIndex];
    }
    
    if (runAfterParamChange){
      //register non saved changes in the ParamGroup
      if (param->saveXmlIsOn())
        localParamGroup->setNonSavedChanges(true);
      
      //"runAfterParamChange" defined in its ParamGroup
      localParamGroup->runAfterParamChange(groupIndex,param->getUpdateFromFlag());
      //"runAfterParamGroupChange" defined in its parent ParamGroup
      if (localParamGroup->getParentParamGroup()!=nullptr)
        localParamGroup->getParentParamGroup()->runAfterParamGroupChange(localParamGroup->getIndex(),groupIndex,param->getUpdateFromFlag());
    }
    if (updateUi){
      param->updateUi(true);
    }
    
  }
  
  int getNumParameters(){
    return getNumAutomatedParams();
  }
  
  const String getParameterName (int index){
    if (index>=0 && index<getNumAutomatedParams()){
      String name=groupAutomated[index]->getParam(indexInGroupAutomated[index])->getXmlName();
      ParamGroup *paramGroup=groupAutomated[index];
      while (paramGroup->getParentParamGroup()!=nullptr){
        name=paramGroup->getXmlName()+"_"+name;
        paramGroup=paramGroup->getParentParamGroup();
      }
      return name; 
    } else   
      return String::empty;
  }  

  float getParameter (int index){
    if (index>=0 && index<getNumAutomatedParams())  
      return groupAutomated[index]->getParam(indexInGroupAutomated[index])->hostGet();
    else
      return 0.f;
  }

  void setParameter (int index, float newValue){     
    if (index>=0 && index<getNumAutomatedParams()){
      ParamGroup * const localParamGroup=groupAutomated[index];
      const int groupIndex=indexInGroupAutomated[index];
      Param * const param=localParamGroup->getParam(groupIndex);
      if (param->hostSet(newValue)){
        param->updateUi(true);
        //setParameter is called from the Host to the send new values of automated parameters 
        //and will be usually be running on the processing thread so we need to create a different 
        //thread that doesn't block processing
        runAfterParamChangeThreads.addJob(new runAfterParamChangeThread(localParamGroup,groupIndex,param->getUpdateFromFlag()),true);
      } else if (param->forceRunAfterParamChangeInHostIsOn()){
        //same applies here
        runAfterParamChangeThreads.addJob(new runAfterParamChangeThread(localParamGroup,groupIndex,param->getUpdateFromFlag()),true);
      }
    } 
  }  
   
  /** generates the mappings between all parameters in ParamGroups and 
    the global list of parameters, by looking at paramGroup and
    all its children */
  void initAllParameters(){
    setPluginProcessor(this);
    initParameters();

    //automated parameters:
    //groupAutomated > associated ParamGroup
    //indexInGroupAutomated > associated index inside the ParamGroup
    groupAutomated=new ParamGroup*[getNumAutomatedParams()];    
    for (int i=0;i<getNumAutomatedParams();i++) groupAutomated[i]=nullptr;
    indexInGroupAutomated=new int[getNumAutomatedParams()];
    mapGlobalIndex(this,true,groupAutomated,indexInGroupAutomated);
    
    //non automated parameters:
    //groupNonAutomated > associated ParamGroup
    //indexInGroupNonAutomated > associated index inside the ParamGroup    
    groupNonAutomated=new ParamGroup*[getNumNonAutomatedParams()];
    for (int i=0;i<getNumNonAutomatedParams();i++) groupNonAutomated[i]=nullptr;
    indexInGroupNonAutomated=new int[getNumNonAutomatedParams()];
    mapGlobalIndex(this,false,groupNonAutomated,indexInGroupNonAutomated);  
  }

  PluginProcessor():
    ParamGroup(JucePlugin_Name),
    groupAutomated(nullptr),
    indexInGroupAutomated(nullptr),
    groupNonAutomated(nullptr),
    indexInGroupNonAutomated(nullptr){            
  }
  
  ~PluginProcessor(){
    if (groupAutomated){
      delete[] groupAutomated;
      groupAutomated=nullptr;
    }

    if (groupNonAutomated){
      delete[] groupNonAutomated;
      groupNonAutomated=nullptr;
    }
    
    if (indexInGroupAutomated){
      delete[] indexInGroupAutomated;
      indexInGroupAutomated=nullptr;
    }

    if (indexInGroupNonAutomated){
      delete[] indexInGroupNonAutomated;
      indexInGroupNonAutomated=nullptr;
    }    

  }
};

#endif