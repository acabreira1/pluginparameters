/*
==============================================================================

This file is part of the PluginParameters module.
Copyright 2012-15 by 4drX

Permission is granted to use this software under the terms of the
GPL v2 (or any later version).

Details of this license can be found at: www.gnu.org/licenses

------------------------------------------------------------------------------

PluginParameters is provided WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

To release a closed-source product which uses PluginParameters, you will
need to purchase a commercial license. For more information, please send me
a PM (Personal Message) or reply to this thread at the JUCE forum:
http://www.juce.com/forum/topic/juce-module-automatically-handle-plugin-parameters

==============================================================================
*/

#ifndef __PLUGINPARAMETERS_PLUGINPROCESSOR_HEADER__
#define __PLUGINPARAMETERS_PLUGINPROCESSOR_HEADER__

#include "ParamGroups.h"

#ifndef JucePlugin_Name
#define JucePlugin_Name "PluginProcessor"
#endif

#ifdef PluginParameters_ErrorLogger
#define logError(msg) { Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+String(msg)); jassertfalse; }
#else
#define logError(msg) { }
#endif

class runAfterParamChangeThread : public ThreadPoolJob {
  int groupIndex;
  ParamGroup *paramGroup;
  Param *param;
  float newValue;
  UpdateFromFlags updateFromFlag;
  int runAfterParamChange;
  int updateUi;  

  JobStatus runJob() override;

public:
  runAfterParamChangeThread(int groupIndex,ParamGroup *paramGroup,Param *param,float newValue,UpdateFromFlags updateFromFlag,int runAfterParamChange=1,int updateUi=1)
    :ThreadPoolJob(String(paramGroup->getName())+" "+String(groupIndex)),
    groupIndex(groupIndex),
    paramGroup(paramGroup),
    param(param),
    newValue(newValue),   
    updateFromFlag(updateFromFlag),
    runAfterParamChange(runAfterParamChange),
    updateUi(updateUi)    
  {
  }
};

/**  methods to redirect all parameters to the global list referred by the host        
       and handle automatically automated and not automated parameters */
class PluginProcessor : public ParamGroup, public AudioPluginInstance{
protected:  
  #ifdef PluginParameters_ErrorLogger
  FileLogger *fileLogger;
  #endif

  ThreadPool runAfterParamChangeThreads;

private:
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

  const String getName() const {
    return ParamGroup::getName();
  }

  #ifdef JucePlugin_WantsMidiInput  
  virtual bool acceptsMidi() const override{
  #if JucePlugin_WantsMidiInput
    return true;
  #else
    return false;
  #endif
  }
  #endif

  #ifdef JucePlugin_ProducesMidiOutput  
  virtual bool producesMidi() const override{
  #if JucePlugin_ProducesMidiOutput
    return true;
  #else
    return false;
  #endif
  } 
  #endif

  #ifdef JucePlugin_SilenceInProducesSilenceOut  
  virtual bool silenceInProducesSilenceOut() const override{
  #if JucePlugin_SilenceInProducesSilenceOut
    return true;
  #else
    return false;
  #endif
  }
  #endif
  
  /** Coordinates all updates, generalization of setParameterNotifyingHost to be able 
      to deal transparently with automated and non automated parameters. 

      if runAfterParamChange==0: runAfterParamChange will never be called.
      if runAfterParamChange==1: runAfterParamChange will be called if there's a value change.
      if runAfterParamChange==2: runAfterParamChange will be called in any case.
      
      if updateUi==0: no updateUi is requested.
      if updateUi==1: an updateUi is requested if there's a value change and runAfterParamChange==1.
      if updateUi==2: an updateUi is requested in any case.
  */ 
  void updateProcessorHostAndUi(Param *const param, float newValue,UpdateFromFlags updateFromFlag,int runAfterParamChange=1,int updateUi=1){
    const int globalIndex=param->getGlobalIndex();
    ParamGroup *paramGroup;
    int groupIndex;
    if (param->registeredAtHost()){ //if automated, notify host     
                                    //and get corresponding localParamGroup and groupIndex 
      
      if (param->getOption(Param::autoChangeGestures)){
        //hosts like Logic require that you indicate when a parameter starts a gesture (changes)
        beginParameterChangeGesture(globalIndex);
        //notify host      
        sendParamChangeMessageToListeners (globalIndex, newValue);
        //hosts like Logic require that you indicate when a parameter ends a gesture (changes)
        endParameterChangeGesture(globalIndex);
      } else {
        //notify host      
        sendParamChangeMessageToListeners (globalIndex, newValue);
      }
      
      paramGroup=groupAutomated[globalIndex];
      groupIndex=indexInGroupAutomated[globalIndex];
      
    } else { //if not automated skip host notification
             // but get corresponding localParamGroup and groupIndex
      paramGroup=groupNonAutomated[globalIndex];
      groupIndex=indexInGroupNonAutomated[globalIndex];
    }

    if (param->getOption(Param::createThreadForParamChange)){
      //create a different thread to run runAfterParamChange(..) and runAfterParamGroupChange(...)
      runAfterParamChangeThreads.addJob(new runAfterParamChangeThread(groupIndex,paramGroup,param,newValue,updateFromFlag,runAfterParamChange,updateUi),true);
    } else {
      switch (runAfterParamChange){
        case 2:
          paramGroup->runAfterParamChange(groupIndex,updateFromFlag);
          if (paramGroup->getParentParamGroup()!=nullptr)
            paramGroup->getParentParamGroup()->runAfterParamGroupChange(paramGroup->getIndex(),groupIndex,updateFromFlag);  
          if (updateUi>=1){
            param->updateUi(true);
          }
          break;
        case 1:
          if (param->hostSet(newValue)){
            if (param->getOption(Param::saveToPresets))
              paramGroup->setNonSavedChanges(true);
            paramGroup->runAfterParamChange(groupIndex,updateFromFlag);
            if (paramGroup->getParentParamGroup()!=nullptr)
              paramGroup->getParentParamGroup()->runAfterParamGroupChange(paramGroup->getIndex(),groupIndex,updateFromFlag);
            if (updateUi>=1)
              param->updateUi(true);
          } else if (updateUi==2){
            param->updateUi(true);
          }
          break;
        case 0:
          if (updateUi>=1){
            param->updateUi(true);
          }
          break;
        default:
          break;
      }
    }    
  }
  
  virtual int getNumParameters() override{
    return getNumAutomatedParams();
  }
  
  virtual const String getParameterName (int index) override{
    if (index>=0 && index<getNumAutomatedParams()){
      String name=groupAutomated[index]->getParam(indexInGroupAutomated[index])->getXmlName();
      ParamGroup *paramGroup=groupAutomated[index];
      while (paramGroup->getParentParamGroup()!=nullptr){
        if (paramGroup->getXmlNameInGetParameterName()) 
          name=paramGroup->getXmlName()+"_"+name;
        paramGroup=paramGroup->getParentParamGroup();
      }
      return name; 
    } else   
      return String::empty;
  }  

  virtual float getParameter (int index) override{
    if (index>=0 && index<getNumAutomatedParams())  
      return groupAutomated[index]->getParam(indexInGroupAutomated[index])->hostGet();
    else
      return 0.f;
  }

  virtual void setParameter (int index, float newValue) override;  
   
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

  // method of AudioPluginInstance
  virtual void fillInPluginDescription (PluginDescription&) const {}

  PluginProcessor(const String name=JucePlugin_Name):
    ParamGroup(name),
    #ifdef PluginParameters_ErrorLogger
    fileLogger(nullptr),    
    #endif
    runAfterParamChangeThreads(1),
    groupAutomated(nullptr),
    indexInGroupAutomated(nullptr),
    groupNonAutomated(nullptr),
    indexInGroupNonAutomated(nullptr)    
    {    
      #ifdef PluginParameters_ErrorLogger
      fileLogger=new FileLogger(File::getSpecialLocation(File::userDesktopDirectory).getChildFile(String("errors.txt")),"error log:\n"),
      Logger::setCurrentLogger(fileLogger);        
      #endif
      //Create main UndoManager
      setUndoManager(new UndoManager());
  }
  
  ~PluginProcessor(){
    #ifdef PluginParameters_ErrorLogger
    Logger::setCurrentLogger(nullptr);
    if (fileLogger){
      delete fileLogger;    
      fileLogger=nullptr;
    }
    #endif

    if (getUndoManager()){
      delete getUndoManager();
      setUndoManager(nullptr);
    }

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