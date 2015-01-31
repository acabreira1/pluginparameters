/*
  ==============================================================================

   This file is part of the PluginParameters module.
   Copyright 2012-13 by MarC

  ------------------------------------------------------------------------------

   PluginParameters is provided WITHOUT ANY WARRANTY; without even the implied 
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

   To release a closed-source product which uses PluginParameters, commercial 
   licenses are available. For more information, please send me a PM (Personal 
   Message) or reply to this thread at the JUCE forum: 
   http://www.juce.com/forum/topic/juce-module-automatically-handle-plugin-parameters

  ==============================================================================
*/

#ifndef __PLUGINPARAMETERS_PARAMGROUPS_HEADER__
#define __PLUGINPARAMETERS_PARAMGROUPS_HEADER__

#include "Params.h"

class StringParamArray;
class FloatParamArray;
class LogParamArray;
class LogWith0ParamArray;
class LogWithSignParamArray;
class IntParamArray;
class BoolParamArray;

class StringParamMatrix;
class FloatParamMatrix;
class LogParamMatrix;
class LogWith0ParamMatrix;
class LogWithSignParamMatrix;
class IntParamMatrix;
class BoolParamMatrix;

/** Base class for all groups of parameters. Distinctions by type are made below. */
class ParamGroup{  
private:       
  bool nonSavedChanges;

  ParamGroup* parentParamGroup;
  
  void setParentParamGroup(ParamGroup *paramGroup){
    parentParamGroup=paramGroup;        
  }

  /** Counter of automated and non automated parameters. When a new parameter is added, 
      they are incremented accordingly. This is used to compute the parameter indexes in 
      the global list of automated or non automated parameters. */
  int numAutomatedParams;
  int numNonAutomatedParams;
  
  /* index with which it is added to its parent ParamGroup 
     (this information is needed to make runAfterParamGroupChange(...)
      more efficient by allowing a switch to tell between ParamGroups) */
  int localIndex;  

  const String name;      

  String xmlName;

  bool xmlNameInGetParameterName;

  String xmlFileName;

  /** List of all parameters included in this group */
  OwnedArray<Param > paramList;
  
  /** List of all subgroups of parameters included in this group */
  OwnedArray<ParamGroup > paramGroupList;

  /** List of all Params that were allocated internally with a new command */
  OwnedArray<Param > paramsToUnallocateAtDestructor;

  /** List of all ParamGroups that were allocated internally with a new command */
  OwnedArray<ParamGroup > paramGroupsToUnallocateAtDestructor;    

  /** Pointer to the AudioProcessor class to be able notify parameter changes to the host 
      with PluginProcessor::updateHostAndUi(...) */
  PluginProcessor* pluginProcessor;

  /** Pointer to the PluginProcessor's UndoManager instance by default. It can also
      be set up to point to independent UndoManagers. */
  UndoManager *undoManager;

protected:
  /** Returns the pointer to the (Extended)AudioProcessor instance */
  PluginProcessor* getProcessor() const{
    return pluginProcessor;
  }  

  void setPluginProcessor(PluginProcessor* pluginProcessorArg){
    pluginProcessor=pluginProcessorArg;    
  }  
  
  bool *saveXmlOptionCopy;  
  
public:
  /** Sets the pointer to the UndoManager instance 
      (by default the same that its parent ParamGroup) */
  void setUndoManager(UndoManager *undoManagerArg){
    undoManager=undoManagerArg;
  }

  /** Returns the pointer to the UndoManager instance */
  UndoManager* getUndoManager() const{
    return undoManager;
  }  
  
  /** Set if there are changes in the parameters that have not been saved in Xml */
  void setNonSavedChanges(bool nonSavedChangesArg){
    bool oldNonSavedChanges=nonSavedChanges;
    nonSavedChanges=nonSavedChangesArg;
    if (oldNonSavedChanges!=nonSavedChangesArg){
      runAfterNonSavedChangesChange();
      //propagate nonSavedChanges to parent ParamGroups
      if (nonSavedChangesArg){
        if (getParentParamGroup()!=nullptr)
          getParentParamGroup()->setNonSavedChanges(true);
      } else { //if !nonSavedChangesArg
        // set nonSavedChanges in its children ParamGroups too
        for (int g=0;g<getNumParamGroups();g++)
          getParamGroup(g)->setNonSavedChanges(false);
      }
    }
  }

  /** Returns true if there are changes in the parameters that have not been saved in Xml */
  const bool getNonSavedChanges() const{
    return nonSavedChanges;
  }

  /** Gets the ParamGroup to which this ParamGroup was added */
  ParamGroup* getParentParamGroup() const{
    return parentParamGroup;
  }

  /** Returns the number of automated parameters added so far */
  const int getNumAutomatedParams() const {
    return numAutomatedParams;
  }
  
  /** Returns the number of non automated parameters added so far */
  const int getNumNonAutomatedParams() const {
    return numNonAutomatedParams;
  }

  /** Returns the index with which it was added to its parent ParamGroup */
  const int getIndex() const {
    return localIndex;
  }  

  /** Returns a String label for this ParamGroup */
  const String getName() const { return name; }  
 
  /** Returns the tag name under which this ParamGroup is added to its parent ParamGroup 
      See readXml(...) and saveXml(...) for more details. */ 
  String getXmlName() const { return xmlName; }

  /** Sets the tag name under which this ParamGroup is added to its parent ParamGroup 
      See readXml(...) and saveXml(...) for more details. */ 
  void setXmlName(const String xmlNameArg){
    xmlName=xmlNameArg;
  }  
  
  /** Show in the parameter name displayed by the host the name of this 
      paramGroup (enabled by default). By default all nested paramGroups 
      are included.*/
  void setXmlNameInGetParameterName(const bool show){
    xmlNameInGetParameterName=show;
  }

  bool getXmlNameInGetParameterName() const{
    return xmlNameInGetParameterName;
  }


  /** Returns the number of parameters included in this group (subgroups are ignored) */
  const int getNumParams() const { 
    return paramList.size(); 
  }
  
  /** Returns the number of subgroups of parameters included in this group 
      (subgroups of subgroups are ignored) */
  const int getNumParamGroups() const { 
    return paramGroupList.size(); 
  }  

  /** Set settings recursively for all parameters in this ParamGroup */
  void setOption(const Param::Options settingId,const bool newValue, const bool applyRecursively){
    for (int i=0;i<paramList.size();i++)
      paramList[i]->setOption(settingId,newValue);
      
    if (applyRecursively){
      for (int i=0;i<paramGroupList.size();i++)
        paramGroupList[i]->setOption(settingId,newValue,true);
    }
  }
     
  ParamGroup *getParamGroup(const int groupIndex) const{
    jassert(groupIndex>=0 && groupIndex<paramGroupList.size());
    return paramGroupList[groupIndex];
  }
  
  /** Adds a subgroup of parameters to this group. If there is another
      ParamGroup child with the same tag name (xmlName), a numeric suffix 
      will be added automatically to it to make it unique.
   */
  void addParamGroup(const int paramGroupIndex, ParamGroup *paramGroup){
    //Oh oh! You are not adding the parameter groups in the same order that you enumerated
    //their indexes. Please go and fix it.
    if (paramGroupIndex!=paramGroupList.size()) {jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are not adding the parameter groups in the same order that you enumerated their indexes."); return;}
    
    paramGroup->pluginProcessor=getProcessor();
    paramGroup->undoManager=getUndoManager();
    paramGroup->parentParamGroup=this;
    paramGroup->numAutomatedParams=numAutomatedParams;
    paramGroup->numNonAutomatedParams=numNonAutomatedParams;
    paramGroup->localIndex=paramGroupIndex;

    //produce a unique tag name:
    //if there was already a child with the same xmlName tag
    //add a numeric suffix to make this tag name unique
    int counter=2;
    int g=0;
    String baseName=paramGroup->getXmlName();
    while (g<paramGroupList.size()){
      if (paramGroupList[g]->getXmlName()==paramGroup->getXmlName()){
        paramGroup->setXmlName(baseName+":"+(String)(counter++));
        g=0;
      } else g++;
    }

    paramGroup->initParameters();
    paramGroupList.add(paramGroup);    
        
    numAutomatedParams=paramGroup->getNumAutomatedParams();
    numNonAutomatedParams=paramGroup->getNumNonAutomatedParams();      
  }
    
  Param *getParam(const int index) const{
    if (index<0 || index>=paramList.size()) {jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"Param index out of bounds: "+String(index)+" in ParamGroup with XML name: "+this->getXmlName()); return nullptr;}
    return paramList[index];
  }

  /** Adds a parameter to this group. 
      If forceUniqueXmlName=true and there is another Param child 
      with the same attribute name (xmlName), a numeric suffix will be 
      added automatically to it to make it unique.
      Use forceUniqueXmlName=false only when you are 100% sure that
      you are using different names for all childs and you want to improve
      the initialization performance. Use with care, if you are wrong, it
      will lead to undetectable errors. */
  void addParam(const int paramIndex,Param * const param,bool forceUniqueXmlName=true){ 
    //Oh oh! You are not adding the parameters in the same order that you enumerated
    //their indexes. Please go and fix it.   
    if (paramIndex!=paramList.size()) {jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are not adding the parameters in the same order that you enumerated their indexes."); return;}    

    param->pluginProcessor=getProcessor();

    if (param->registeredAtHost())
      param->globalIndex=numAutomatedParams++;
    else
      param->globalIndex=numNonAutomatedParams++;

    if (forceUniqueXmlName){
      //produce a unique attribute name:
      //if there was already a Param with the same xmlName attribute
      //add a numeric suffix to make this attribute name unique
      int counter=2;
      int p=0;
      String baseName=param->getXmlName();
      while (p<paramList.size()){
        if (paramList[p]->getXmlName()==param->getXmlName()){
          param->setXmlName(baseName+":"+(String)(counter++));
          p=0;
        } else p++;
      }
    }

    paramList.add(param);
  }          
  
  StringParam *getStringParam(const int index) const{
    /* wrong index */
    if (index<0 || index>=paramList.size()) {jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"Param index out of bounds: "+String(index)+" in ParamGroup with XML name: "+this->getXmlName()); return nullptr;}
    /* You are trying to fetch a parameter with another type... 
        You want to use another method from the following list: 
        getStringParam(), getFloatParam(), getLogParam(), 
        getIntParam(), 
        getBoolParam()  */
    StringParam *pointer=dynamic_cast<StringParam *>(paramList[index]);
    if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a parameter with another type..."); return nullptr;}
    return pointer;
  }
  
  void addStringParam(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, String *const value,bool forceUniqueXmlName=true){      
    Param *param;
    addParam(paramIndex,param=new StringParam(name,registerAtHostFlag,loadSaveOptions,value),forceUniqueXmlName);
    paramsToUnallocateAtDestructor.add(param);
  }    

  void addStringParamArray(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, String* const values,int *const size,const int maxSize,bool saveOnlySizedArrayFlag=true);  

  void addStringParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, String** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag=true);   

  StringParamArray *getStringParamArray(const int index) const;
  StringParamMatrix *getStringParamMatrix(const int index) const;
  
  template<class FloatType,class FloatTypeMap>
  FloatTypeParam<FloatType,FloatTypeMap> *getFloatTypeParam(const int index) const{
    /* wrong index */
    if (index<0 || index>=paramList.size()) {jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"Param index out of bounds: "+String(index)+" in ParamGroup with XML name: "+this->getXmlName()); return nullptr;}
    /* You are trying to fetch a parameter with another type... 
        You want to use another method from the following list: 
        getStringParam(), getFloatParam(), getLogParam(), 
        getIntParam(), 
        getBoolParam()  */
    FloatTypeParam<FloatType,FloatTypeMap> *pointer=dynamic_cast<FloatTypeParam<FloatType,FloatTypeMap> *>(paramList[index]);
    if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a parameter with another type..."); return nullptr;}
    return pointer;
  }

  template<class FloatType>
  FloatTypeParam<FloatType> *getFloatTypeParam(const int index) const{
    /* wrong index */
    if (index<0 || index>=paramList.size()) {jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"Param index out of bounds: "+String(index)+" in ParamGroup with XML name: "+this->getXmlName()); return nullptr;}
    /* You are trying to fetch a parameter with another type... 
        You want to use another method from the following list: 
        getStringParam(), getFloatParam(), getLogParam(), 
        getIntParam(), 
        getBoolParam()  */
    FloatTypeParam<FloatType> *pointer=dynamic_cast<FloatTypeParam<FloatType> *>(paramList[index]);
    if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a parameter with another type..."); return nullptr;}
    return pointer;
  }

  FloatParam* getFloatParam(const int index) const{
    return getFloatTypeParam<float>(index);
  }

  template<class FloatType,class FloatTypeMap>
  void addFloatTypeParam(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, FloatType *const value, const FloatType minValue=(FloatType)(0),const FloatType maxValue=(FloatType)(0),bool forceUniqueXmlName=true){
    Param *param;
    addParam(paramIndex,param=new FloatTypeParam<FloatType,FloatTypeMap>(name,registerAtHostFlag,loadSaveOptions,value,minValue,maxValue),forceUniqueXmlName);
    paramsToUnallocateAtDestructor.add(param);
  }

  template<class FloatType>
  void addFloatTypeParam(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, FloatType *const value, const FloatType minValue=(FloatType)(0),const FloatType maxValue=(FloatType)(0),bool forceUniqueXmlName=true){
    Param *param;
    addParam(paramIndex,param=new FloatTypeParam<FloatType>(name,registerAtHostFlag,loadSaveOptions,value,minValue,maxValue),forceUniqueXmlName);
    paramsToUnallocateAtDestructor.add(param);
  }   

  void addFloatParam(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float *const value, const float minValue=(float)(0),const float maxValue=(float)(1),bool forceUniqueXmlName=true){
    addFloatTypeParam<float>(paramIndex,name,registerAtHostFlag,loadSaveOptions,value,minValue,maxValue,forceUniqueXmlName);
  }   

  void addFloatParamArray(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float* const values,int *const size,const int maxSize,const float minValue=(float)(0),const float maxValue=(float)(1), bool saveOnlySizedArrayFlag=true);

  void addFloatParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols,const float minValue=(float)(0),const float maxValue=(float)(1), const bool saveOnlySizedMatrixFlag=true);

  FloatParamArray *getFloatParamArray(const int index) const;
  FloatParamMatrix *getFloatParamMatrix(const int index) const;
  
  LogParam *getLogParam(const int index) const{
    /* wrong index */
    if (index<0 || index>=paramList.size()) {jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"Param index out of bounds: "+String(index)+" in ParamGroup with XML name: "+this->getXmlName()); return nullptr;}
    /* You are trying to fetch a parameter with another type... 
        You want to use another method from the following list: 
        getStringParam(), getFloatParam(), getLogParam(), 
        getIntParam(), 
        getBoolParam()  */
    LogParam *pointer=dynamic_cast<LogParam *>(paramList[index]);
    if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a parameter with another type..."); return nullptr;}
    return pointer;
  }
   
   void addLogParam(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float *const value, const float minValue=(float)(0.001),const float maxValue=(float)(1),bool forceUniqueXmlName=true){
    Param *param;
    addParam(paramIndex,param=new LogParam(name,registerAtHostFlag,loadSaveOptions,value,minValue,maxValue),forceUniqueXmlName);
    paramsToUnallocateAtDestructor.add(param);
  }   

  void addLogParamArray(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float* const values,int *const size,const int maxSize,const float minValue=(float)(0),const float maxValue=(float)(1), bool saveOnlySizedArrayFlag=true);  

  void addLogParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const float minValue=(float)(0),const float maxValue=(float)(1), const bool saveOnlySizedMatrixFlag=true);  
  
  LogParamArray *getLogParamArray(const int index) const;
  LogParamMatrix *getLogParamMatrix(const int index) const;

  LogWith0Param *getLogWith0Param(const int index) const{
    /* wrong index */
    if (index<0 || index>=paramList.size()) {jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"Param index out of bounds: "+String(index)+" in ParamGroup with XML name: "+this->getXmlName()); return nullptr;}
    /* You are trying to fetch a parameter with another type... 
        You want to use another method from the following list: 
        getStringParam(), getFloatParam(), getLogParam(), 
        getIntParam(), 
        getBoolParam()  */
    LogWith0Param *pointer=dynamic_cast<LogWith0Param *>(paramList[index]);
    if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a parameter with another type..."); return nullptr;}
    return pointer;
  }  
   
   void addLogWith0Param(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float *const value, const float minValue=(float)(0.001),const float maxValue=(float)(1),bool forceUniqueXmlName=true){
    Param *param;
    addParam(paramIndex,param=new LogWith0Param(name,registerAtHostFlag,loadSaveOptions,value,minValue,maxValue),forceUniqueXmlName);    
    paramsToUnallocateAtDestructor.add(param);
  }

  void addLogWith0ParamArray(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float* const values,int *const size,const int maxSize,const float minValue=(float)(0),const float maxValue=(float)(1), bool saveOnlySizedArrayFlag=true);

  void addLogWith0ParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const float minValue=(float)(0),const float maxValue=(float)(1), const bool saveOnlySizedMatrixFlag=true);
  
  LogWith0ParamArray *getLogWith0ParamArray(const int index) const;
  LogWith0ParamMatrix *getLogWith0ParamMatrix(const int index) const;

  LogWithSignParam *getLogWithSignParam(const int index) const{
    /* wrong index */
    if (index<0 || index>=paramList.size()) {jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"Param index out of bounds: "+String(index)+" in ParamGroup with XML name: "+this->getXmlName()); return nullptr;}
    /* You are trying to fetch a parameter with another type... 
        You want to use another method from the following list: 
        getStringParam(), getFloatParam(), getLogParam(), 
        getIntParam(), 
        getBoolParam()  */
    LogWithSignParam *pointer=dynamic_cast<LogWithSignParam *>(paramList[index]);
    if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a parameter with another type..."); return nullptr;}
    return pointer;
  }  
   
   void addLogWithSignParam(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float *const value, const float minNegativeValue=(float)(-1),const float maxPositiveValue=(float)(1),bool forceUniqueXmlName=true){
   Param *param;
   addParam(paramIndex,param=new LogWithSignParam(name,registerAtHostFlag,loadSaveOptions,value,minNegativeValue,maxPositiveValue),forceUniqueXmlName);
  paramsToUnallocateAtDestructor.add(param);
}  

  void addLogWithSignParamArray(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float* const values,int *const size,const int maxSize,const float minValue=(float)(0),const float maxValue=(float)(1), bool saveOnlySizedArrayFlag=true);

  void addLogWithSignParamMatrix(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const float minValue=(float)(0),const float maxValue=(float)(1), const bool saveOnlySizedMatrixFlag=true);

  LogWithSignParamArray *getLogWithSignParamArray(const int index) const;
  LogWithSignParamMatrix *getLogWithSignParamMatrix(const int index) const;
  
  template<class IntType>
  IntTypeParam<IntType> *getIntTypeParam(const int index) const{
    /* wrong index */ 
    if (index<0 || index>=paramList.size()) {jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"Param index out of bounds: "+String(index)+" in ParamGroup with XML name: "+this->getXmlName()); return nullptr;}
    /* You are trying to fetch a parameter with another type... 
        You want to use another method from the following list: 
        getStringParam(), getFloatParam(), getLogParam(), 
        getIntParam(), 
        getBoolParam()  */
    IntTypeParam<IntType> *pointer=dynamic_cast<IntTypeParam<IntType> *>(paramList[index]);
    if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a parameter with another type..."); return nullptr;}
    return pointer;
  }

  IntParam *getIntParam(const int index) const{
    return getIntTypeParam<int>(index);  
  }
  
  template<class IntType>
  void addIntTypeParam(const int paramIndex,const String &name,const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions,IntType *const value, const IntType minValue=0,const IntType maxValue=1,bool forceUniqueXmlName=true){
    Param *param;
    addParam(paramIndex,param=new IntTypeParam<IntType>(name,registerAtHostFlag,loadSaveOptions,value,minValue,maxValue),forceUniqueXmlName);
    paramsToUnallocateAtDestructor.add(param);
  }

  void addIntParam(const int paramIndex,const String &name,const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions,int *const value, const int minValue=0,const int maxValue=1,bool forceUniqueXmlName=true){
    addIntTypeParam<int>(paramIndex,name,registerAtHostFlag,loadSaveOptions,value,minValue,maxValue,forceUniqueXmlName);
  }

  void addIntParamArray(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, int* const values,int *const size,const int maxSize,const int minValue=(int)(0),const int maxValue=(int)(1), bool saveOnlySizedArrayFlag=true);  

  void addIntParamMatrix(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, int** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols,const int minValue=(int)(0),const int maxValue=(int)(1), const bool saveOnlySizedMatrixFlag=true);
  
  IntParamArray *getIntParamArray(const int index) const;
  IntParamMatrix *getIntParamMatrix(const int index) const;

  BoolParam *getBoolParam(const int index) const{
    /* wrong index */
    if (index<0 || index>=paramList.size()) {jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"Param index out of bounds: "+String(index)+" in ParamGroup with XML name: "+this->getXmlName()); return nullptr;}
    /* You are trying to fetch a parameter with another type... 
        You want to use another method from the following list: 
        getStringParam(), getFloatParam(), getLogParam(), 
        getIntParam(), 
        getBoolParam()  */
    BoolParam *pointer=dynamic_cast<BoolParam *>(paramList[index]);
    if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a parameter with another type..."); return nullptr;}
    return pointer;
  }  
  
  void addBoolParam(const int paramIndex,const String &name,const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, bool *const value,bool forceUniqueXmlName=true){
    Param *param;
    addParam(paramIndex,param=new BoolParam(name,registerAtHostFlag,loadSaveOptions,value),forceUniqueXmlName);
    paramsToUnallocateAtDestructor.add(param);
  }
  
  void addBoolParamArray(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, bool* const values,int *const size,const int maxSize,bool saveOnlySizedArrayFlag=true);

  void addBoolParamMatrix(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, bool** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag=true);

  BoolParamArray *getBoolParamArray(const int index) const;
  BoolParamMatrix *getBoolParamMatrix(const int index) const;    

  /** Set the updateUi flags of all its parameters to true only in this ParamGroup
      or applyRecursively. */
  virtual void updateUi(const bool request,const bool applyRecursively){
    for (int i=0;i<paramList.size();i++)
      paramList[i]->updateUi(request); 
    
    if (applyRecursively){     
      for (int g=0;g<paramGroupList.size();g++)
        paramGroupList[g]->updateUi(request,true);
    }
  }

  /** Override to update the relevant preset managers about
      saved changes in this ParamGroup */
  virtual void runAfterNonSavedChangesChange(){}

  /** Stores the parameter values as an XML attribute.
      If createChild is set to true it will create a child XML node 
      (you want to disable it at the root node).*/
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool applyRecursively, XmlType xmlType=SESSION){
    //create a child XmlElement for this ParamGroup if you are adding xml
    //contents to an already populated XmlElement.

    if (createChild){
      //create a child      
      xml=xml->createNewChildElement(getXmlName());
    }

    for (int i=0;i<getNumParams();i++)
      paramList[i]->saveXml(xml,xmlType);
      
    if (applyRecursively){
      for (int g=0;g<getNumParamGroups();g++)
        paramGroupList[g]->saveXml(xml,true,true,xmlType);
    }    
  } 
  
  /** Save this ParamGroup to disk as XML. It creates a 
      root tag with the name of this ParamGroup */
  virtual bool savePreset(const File &file,bool applyRecursively,const String &dtdToUse=""){
    XmlElement xml(getName());
    saveXml(&xml,false,applyRecursively,PRESET);

    if (xml.writeToFile(file,dtdToUse)){
      setNonSavedChanges(false);
      return true;
    }

    return false;
  }
  
  /** Delete an XML file asociated with this paramGroup from disk. 
      You may want to reimplement it when you are coding
      'nested' presets. */
  virtual bool removePreset(const File &file){
    if (!file.exists())
      return false;    
                  
    return file.moveToTrash();   
  }

  /** Rename an XML file asociated with this paramGroup from disk. 
      You may want to reimplement it when you are coding
      'nested' presets. */
  virtual bool renamePreset(const File &file,const File &newFile){
    if (!file.exists())
      return false;

    return file.moveFileTo(newFile);
  }
  
  /** Preload Xml values from all parameters into xmlValue. It is not 
      advisable to run this on the Processing thread since it is accessing 
      the disk and may take a little while to parse all this parameters from Xml 
      and normalize them. */
  virtual void readXml(XmlElement *xml, const bool applyRecursively, XmlType xmlType=SESSION){
    //this child couldn't be found
    if (xml==nullptr)
      return;
          
    Param *param;
    for (int i=0;i<getNumParams();i++){
      param=getParam(i);
      param->readXml(xml,xmlType);      
    }      
    
    if (applyRecursively){
      for (int g=0;g<getNumParamGroups();g++){
        XmlElement *childXml=xml->getChildByName(getParamGroup(g)->getXmlName());
        getParamGroup(g)->readXml(childXml,true,xmlType);
      }
    }        
  }
  
  /** Load this paramGroup from disk. If the file doesn't contain
      XML with a tag that matches the name of this ParamGroup, 
      it won't be loaded and it will return false. */
  virtual bool readPreset(const File &file){  
    XmlDocument myDocument (file);
    ScopedPointer <XmlElement> xml(myDocument.getDocumentElement());
    //this file doesn't contain xml with the same tag name it was saved
    if (xml==nullptr || xml->getTagName()!=getName()) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"Error reading preset file. Tag name doesn't match the name of the ParamGroup"); return false; }          
    readXml(xml,true,PRESET);
    return true;
  }
  
  /* Update all parameters from xmlValue after readXml(...) has been called. 
     This should be considerably faster than readXml (which loads everything from
     disk into memory) so you can risk to put it in the processing thread. */
  virtual void updateProcessorFromXml(const bool applyRecursively){
    runBeforeParamGroupUpdate();
    for (int i=0;i<getNumParams();i++){
      getParam(i)->updateProcessorFromXml();
    }
    
    if (applyRecursively){
      for (int g=0;g<getNumParamGroups();g++){
        getParamGroup(g)->updateProcessorFromXml(true);
      }
    }
    setNonSavedChanges(false);
    runAfterParamGroupUpdate();        
  }  

  /* Update all parameters from xmlValue after readXml(...) has been called. 
     This should be considerably faster than readXml (which loads everything from
     disk into memory) so you can risk to put it in the processing thread. 
     Update for each parameter the Host and the UI accordingly. */
  virtual void updateProcessorHostAndUiFromXml(bool forceUpdateHost, bool forceUpdateUi,const bool applyRecursively){
    runBeforeParamGroupUpdate();
    for (int i=0;i<getNumParams();i++){
      getParam(i)->updateProcessorHostAndUiFromXml(forceUpdateHost,forceUpdateUi);
    }
    
    if (applyRecursively){
      for (int g=0;g<getNumParamGroups();g++){
        getParamGroup(g)->updateProcessorHostAndUiFromXml(forceUpdateHost,forceUpdateUi,true);
      }
    } 
    setNonSavedChanges(false);
    runAfterParamGroupUpdate();        
  }  
  
  /** Reset all parameters in this ParamGroup to their defaultValue.
      If applyRecursively=true do the same for the child ParamGroups.
      */
  virtual void updateProcessorFromDefaultXml(const bool applyRecursively){
    runBeforeParamGroupUpdate();   
    for (int i=0;i<paramList.size();i++)
      paramList[i]->updateProcessorFromDefaultXml();
      
    if (applyRecursively){
      for (int i=0;i<paramGroupList.size();i++)
        paramGroupList[i]->updateProcessorFromDefaultXml(true);
    }
    setNonSavedChanges(true);
    runAfterParamGroupUpdate();
  }

  /** Reset all parameters in this ParamGroup to their defaultValue.
      If applyRecursively=true do the same for the child ParamGroups.
      Update for each parameter the Host and the UI accordingly. 
      Beware that parameters with the option loadFromPresets set to false
      won't be reset. */
  virtual void updateProcessorHostAndUiFromDefaultXml(bool forceRunAfterParamChange, bool forceUpdateUi,const bool applyRecursively){
    runBeforeParamGroupUpdate();   
    for (int i=0;i<paramList.size();i++){
      if (paramList[i]->getOption(Param::loadFromPresets))
        paramList[i]->updateProcessorHostAndUiFromDefaultXml(forceRunAfterParamChange,forceUpdateUi);
    }
      
    if (applyRecursively){
      for (int i=0;i<paramGroupList.size();i++)
        paramGroupList[i]->updateProcessorHostAndUiFromDefaultXml(forceRunAfterParamChange,forceUpdateUi,true);
    } 
    setNonSavedChanges(true);
    runAfterParamGroupUpdate();
  }  

  /** Implements what is executed after any parameter of this ParamGroup is updated 
      with a different value inside of the children paramGroup.
      This method will be called by the host, probably on the audio thread, so
      it's absolutely time-critical. Don't use critical sections or anything
      UI-related, or anything at all that may block in any way! */
  virtual void runAfterParamGroupChange(int /*paramGroupIndex*/, int /*paramIndex*/, UpdateFromFlags /*updateFromFlag*/) {};
  
  /** Implements what is executed after a parameter is updated with a different value.
      This method will be called by the host, probably on the audio thread, so
      it's absolutely time-critical. Don't use critical sections or anything
      UI-related, or anything at all that may block in any way! */
  virtual void runAfterParamChange(int /*paramIndex*/, UpdateFromFlags /*updateFromFlag*/) {};
  
  /** Implements what should be executed *before* a new set of parameters is loaded in this 
      ParamGroup using for instance updateProcessorHostAndUiFromXml(...) or updateProcessorFromDefaultXml(...)
      E.g. Place here any initialization of variables that are not defined as Parameters. */
  virtual void runBeforeParamGroupUpdate() {};

  /** Implements what should be executed *after* a new set of parameters is loaded in this 
      ParamGroup using for instance updateProcessorHostAndUiFromXml(...) or updateProcessorFromDefaultXml(...)
      E.g. Place here any post-processing of variables that are not defined as Parameters. */
  virtual void runAfterParamGroupUpdate() {};

  /** All parameters and parameter groups must be added in this method */
  virtual void initParameters() = 0;
  
  /** Notify the host about all the parameters in this paramGroup and update the UI. This is 
      useful when you change the parameters of this paramGroup (maybe several times) without
      notifying the Host and the UI (without using update(...)) and at the end of the process 
      you want to update the Host and the UI from all of them at once */
  void updateHost(bool runAfterParamChange,const bool applyRecursively,UpdateFromFlags updateFromFlag=UPDATE_FROM_PROCESSOR){
    for (int paramIndex=0;paramIndex<getNumParams();paramIndex++)
      getParam(paramIndex)->updateHost(runAfterParamChange,updateFromFlag);

    if (applyRecursively){
      for (int i=0;i<getNumParamGroups();i++)
        getParamGroup(i)->updateHost(runAfterParamChange,true,updateFromFlag);
    }
  }
  
  ParamGroup(const String &name):    
    nonSavedChanges(true),
    parentParamGroup(nullptr),
    numAutomatedParams(0),
    numNonAutomatedParams(0),    
    name(name),
    xmlName(name),
    xmlNameInGetParameterName(true),
    pluginProcessor(nullptr),
    undoManager(nullptr),   
    saveXmlOptionCopy(nullptr)   
    {
      #if JUCE_DEBUG
      // we should be able to use this name as an XML attribute name
      // this checks whether the attribute name string contains any illegal characters...
      for (String::CharPointerType t (name.getCharPointer()); ! t.isEmpty(); ++t)
          jassert (t.isLetterOrDigit() || *t == '_' || *t == '-' || *t == ':');
      #endif
    }

  virtual ~ParamGroup(){
    paramGroupList.clear(false);
    paramGroupsToUnallocateAtDestructor.clear(true);
    paramList.clear(false);    
    paramsToUnallocateAtDestructor.clear(true);    

    if (saveXmlOptionCopy){
      delete[] saveXmlOptionCopy;
      saveXmlOptionCopy=nullptr;
    }
  }
};

//-----------------------------------------------------------------------------------

/** ParamGroup base class for array of Params. */
class ParamArray : public ParamGroup{       
  int *const size;

protected:      
  const int maxSize;
  const bool registerAtHostFlag;
  const LoadSaveOptions loadSaveOptions;
  const bool saveOnlySizedArrayFlag;      
  const bool saveOnlyNonDefaultValuesFlag;    
  const bool updateOnlySizedArrayFlag;

public:      
  virtual void initParameters() = 0;
  
  /** Returns the size of the "visible" array */
  int getSize() const{
    if (size==nullptr)
      return maxSize;
    else
      return *size;
  }
  
  /** Returns the maximum (allocated) size of the array */
  int getMaxSize() const{
    return maxSize;
  }
  
  /** Returns the minimum range of each parameter in the array */
  const double getMin() const{
    if ((size!=nullptr && *size>0) || (maxSize>0))
      return getParam(0)->getMin();
    else
      return 0;
  }
  
  /** Returns the maximum range of each parameter in the array */
  const double getMax() const{
    if ((size!=nullptr && *size>0) || (maxSize>0))
      return getParam(0)->getMax();
    else
      return 0;
  }
 
  virtual void updateUi(const bool request,const bool applyRecursively){
    const int arrayLength=(size!=nullptr)?*size:maxSize;

    for (int i=0;i<arrayLength;i++)
      getParam(i)->updateUi(request); 
    
    if (applyRecursively){     
      for (int g=0;g<getNumParamGroups();g++)
        getParamGroup(g)->updateUi(request,true);
    }
  }

  /** Stores the parameter values as an XML attribute.
      If createChild is set to true it will create a child XML node 
      (you want to disable it at the root node). 
      This method expands the default one to save only the 
      parameters with value other than defaultValue and/or 
      the parameters of the sized array. */
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool applyRecursively, XmlType xmlType=SESSION){
    if (saveXmlOptionCopy){
      switch(xmlType){
        case SESSION:
          for (int i=0;i<ParamArray::maxSize;i++)
            saveXmlOptionCopy[i]=getParam(i)->getOption(Param::saveToSession);
          break;
        case PRESET:
          for (int i=0;i<ParamArray::maxSize;i++)
            saveXmlOptionCopy[i]=getParam(i)->getOption(Param::saveToPresets);
          break;
        default:
          break;
      }      
    }
    
    if (ParamArray::saveOnlySizedArrayFlag){
      switch(xmlType){
        case SESSION:
          for (int i=ParamArray::getSize();i<ParamArray::maxSize;i++)
            getParam(i)->setOption(Param::saveToSession,false);
          break;
        case PRESET:
          for (int i=ParamArray::getSize();i<ParamArray::maxSize;i++)
            getParam(i)->setOption(Param::saveToPresets,false);
          break;
        default:
          break;
      }
    }    
    
    ParamGroup::saveXml(xml,createChild,applyRecursively,xmlType);
    
    if (saveXmlOptionCopy){
      switch(xmlType){
        case SESSION:
          for (int i=0;i<ParamArray::maxSize;i++)
            getParam(i)->setOption(Param::saveToSession,saveXmlOptionCopy[i]);
          break;
        case PRESET:
          for (int i=ParamArray::getSize();i<ParamArray::maxSize;i++)
            getParam(i)->setOption(Param::saveToPresets,saveXmlOptionCopy[i]);
          break;
        default:
          break;
      }
    }
  }
    
  virtual void updateProcessorFromDefaultXml(const bool applyRecursively){
    runBeforeParamGroupUpdate();

    int updateSize;
    if (size!=nullptr && updateOnlySizedArrayFlag)
      updateSize=*size;
    else
      updateSize=maxSize;

    for (int i=0;i<updateSize;i++)
      getParam(i)->updateProcessorFromDefaultXml();
      
    if (applyRecursively){
      for (int i=0;i<getNumParamGroups();i++)
        getParamGroup(i)->updateProcessorFromDefaultXml(true);
    }
    setNonSavedChanges(true);
    runAfterParamGroupUpdate();
  }

  virtual void updateProcessorHostAndUiFromDefaultXml(bool forceRunAfterParamChange, bool forceUpdateUi,const bool applyRecursively){
    runBeforeParamGroupUpdate();   

    int updateSize;
    if (size!=nullptr && updateOnlySizedArrayFlag)
      updateSize=*size;
    else
      updateSize=maxSize;

    for (int i=0;i<updateSize;i++)
      getParam(i)->updateProcessorHostAndUiFromDefaultXml(forceRunAfterParamChange,forceUpdateUi);
      
    if (applyRecursively){
      for (int i=0;i<getNumParamGroups();i++)
        getParamGroup(i)->updateProcessorHostAndUiFromDefaultXml(forceRunAfterParamChange,forceUpdateUi,true);
    }
    setNonSavedChanges(true);
    runAfterParamGroupUpdate();
  }  

  virtual void updateProcessorFromXml(const bool applyRecursively){
    runBeforeParamGroupUpdate();

    int updateSize;
    if (size!=nullptr && updateOnlySizedArrayFlag)
      updateSize=*size;
    else
      updateSize=maxSize;

    for (int i=0;i<updateSize;i++)
      getParam(i)->updateProcessorFromXml();
      
    if (applyRecursively){
      for (int i=0;i<getNumParamGroups();i++)
        getParamGroup(i)->updateProcessorFromXml(true);
    }
    setNonSavedChanges(false);
    runAfterParamGroupUpdate();
  }  

 
  virtual void updateProcessorHostAndUiFromXml(bool forceUpdateHost, bool forceUpdateUi,const bool applyRecursively){
    runBeforeParamGroupUpdate();   

    int updateSize;
    if (size!=nullptr && updateOnlySizedArrayFlag)
      updateSize=*size;
    else
      updateSize=maxSize;

    for (int i=0;i<updateSize;i++)
      getParam(i)->updateProcessorHostAndUiFromXml(forceUpdateHost,forceUpdateUi);
      
    if (applyRecursively){
      for (int i=0;i<getNumParamGroups();i++)
        getParamGroup(i)->updateProcessorHostAndUiFromXml(forceUpdateHost,forceUpdateUi,true);
    }
    setNonSavedChanges(false);
    runAfterParamGroupUpdate();
  }

  ParamArray(const String &name,const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, int *const size, const int maxSize, bool saveOnlySizedArrayFlag=true, bool updateOnlySizedArrayFlag=true):
  ParamGroup(name),
  size(size),
  maxSize(maxSize),  
  registerAtHostFlag(registerAtHostFlag), 
  loadSaveOptions(loadSaveOptions), 
  saveOnlySizedArrayFlag(saveOnlySizedArrayFlag),
  saveOnlyNonDefaultValuesFlag(saveOnlyNonDefaultValuesFlag),
  updateOnlySizedArrayFlag(updateOnlySizedArrayFlag)
  {
    saveXmlOptionCopy=new bool[maxSize];
  }

  virtual ~ParamArray() {}
  
};

/** ParamGroup containing an array of FloatParams. */
class FloatParamArray : public ParamArray{
private:
  float* const values;
  float minValue;
  float maxValue;

public: 
  void initParameters() override{
    for (int i=0;i<ParamArray::getMaxSize();i++){
      ParamGroup::addFloatParam(i,(String)(i),ParamArray::registerAtHostFlag,ParamArray::loadSaveOptions,&(values[i]),minValue,maxValue,false);
    }
  }

  /** Returns the value of position i in the array */
  float getValue(int i) const{
    if (i>=0 && i<ParamArray::getSize() && i<ParamArray::getMaxSize())
      return values[i];
    else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(float minValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getFloatParam(i)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(float maxValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getFloatParam(i)->setMax(maxValueArg);
  }
 
  /** Updates the value from its UI value */ 
  void updateProcessorAndHostFromUi(int i,const float valueArg, UndoManager *const undoManager=nullptr, const bool dontCreateNewUndoTransaction=false) const{
    return ParamGroup::getFloatParam(i)->updateProcessorAndHostFromUi(valueArg,undoManager,dontCreateNewUndoTransaction);
  } 
  
  FloatParamArray(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float* const values,int *const size,const int maxSize,const float minValue=(float)(0),const float maxValue=(float)(1), bool saveOnlySizedArrayFlag=true):
  ParamArray(name,registerAtHostFlag,loadSaveOptions,size,maxSize,saveOnlySizedArrayFlag),
  values(values),
  minValue(minValue),
  maxValue(maxValue)
  {} 
};

/** ParamGroup containing an array of LogParams. */
class LogParamArray : public ParamArray{
private:
  float* const values;
  float minValue;
  float maxValue;
  
public: 
  void initParameters() override{
    for (int i=0;i<ParamArray::getMaxSize();i++){
      ParamGroup::addLogParam(i,(String)(i),ParamArray::registerAtHostFlag,ParamArray::loadSaveOptions,&(values[i]),minValue,maxValue,false);
    }
  }

  /** Returns the value of position i in the array */
  float getValue(int i) const{
    if (i>=0 && i<ParamArray::getSize() && i<ParamArray::getMaxSize())
      return values[i];
    else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(float minValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getLogParam(i)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(float maxValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getLogParam(i)->setMax(maxValueArg);
  }
   
  /** Updates the value from its UI value */ 
  void updateProcessorAndHostFromUi(int i,const double valueArg, UndoManager *const undoManager=nullptr, const bool dontCreateNewUndoTransaction=false) const{
    return ParamGroup::getLogParam(i)->updateProcessorAndHostFromUi((float)valueArg,undoManager,dontCreateNewUndoTransaction);
  } 
  
  LogParamArray(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float* const values,int *const size,const int maxSize,const float minValue=(float)(0),const float maxValue=(float)(1), bool saveOnlySizedArrayFlag=true):
  ParamArray(name,registerAtHostFlag,loadSaveOptions,size,maxSize,saveOnlySizedArrayFlag),
  values(values),
  minValue(minValue),
  maxValue(maxValue)
  {} 
};

/** ParamGroup containing an array of LogWith0Params. */
class LogWith0ParamArray : public ParamArray{
private:
  float* const values;
  float minValue;
  float maxValue;

public:   
  void initParameters() override{
    for (int i=0;i<ParamArray::getMaxSize();i++){
      ParamGroup::addLogWith0Param(i,(String)(i),ParamArray::registerAtHostFlag,ParamArray::loadSaveOptions,&(values[i]),minValue,maxValue,false);
    }
  }

  /** Returns the value of position i in the array */
  float getValue(int i) const{
    if (i>=0 && i<ParamArray::getSize() && i<ParamArray::getMaxSize())
      return values[i];
    else return 0;
  }    
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(float minValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getLogWith0Param(i)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(float maxValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getLogWith0Param(i)->setMax(maxValueArg);
  }
 
  /** Updates the value from its UI value */ 
  void updateProcessorAndHostFromUi(int i,const double valueArg, UndoManager *const undoManager=nullptr, const bool dontCreateNewUndoTransaction=false) const{
    return ParamGroup::getLogWith0Param(i)->updateProcessorAndHostFromUi((float)valueArg,undoManager,dontCreateNewUndoTransaction);
  }
  
  LogWith0ParamArray(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float* const values,int *const size,const int maxSize,const float minValue=(float)(0),const float maxValue=(float)(1), bool saveOnlySizedArrayFlag=true):
  ParamArray(name,registerAtHostFlag,loadSaveOptions,size,maxSize,saveOnlySizedArrayFlag),
  values(values),
  minValue(minValue),
  maxValue(maxValue)
  {} 
};

/** ParamGroup containing an array of LogWithSignParams. */
class LogWithSignParamArray : public ParamArray{
private:
  float* const values;
  float minValue;
  float maxValue;

public: 
  void initParameters() override{
    for (int i=0;i<ParamArray::getMaxSize();i++){
      ParamGroup::addLogWithSignParam(i,(String)(i),ParamArray::registerAtHostFlag,ParamArray::loadSaveOptions,&(values[i]),minValue,maxValue,false);
    }
  }

  /** Returns the value of position i in the array */
  float getValue(int i) const{
    if (i>=0 && i<ParamArray::getSize() && i<ParamArray::getMaxSize())
      return values[i];
    else return 0;
  }
 
  /** Sets the minimum range of each parameter in the array */
  void setMin(float minValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getLogWithSignParam(i)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(float maxValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getLogWithSignParam(i)->setMax(maxValueArg);
  }
 
  /** Updates the value from its UI value */ 
  void updateProcessorAndHostFromUi(int i,const float valueArg, UndoManager *const undoManager=nullptr, const bool dontCreateNewUndoTransaction=false) const{
    return ParamGroup::getLogWithSignParam(i)->updateProcessorAndHostFromUi(valueArg,undoManager,dontCreateNewUndoTransaction);
  }
  
  LogWithSignParamArray(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float* const values,int *const size,const int maxSize,const float minValue=(float)(0),const float maxValue=(float)(1), bool saveOnlySizedArrayFlag=true):
  ParamArray(name,registerAtHostFlag,loadSaveOptions,size,maxSize,saveOnlySizedArrayFlag),
  values(values),
  minValue(minValue),
  maxValue(maxValue)
  {} 
};

/** ParamGroup containing an array of IntParams. */
class IntParamArray : public ParamArray{
private:
  int* const values;
  int minValue;
  int maxValue;

public: 
  void initParameters() override{
    for (int i=0;i<ParamArray::getMaxSize();i++){
      ParamGroup::addIntParam(i,(String)(i),ParamArray::registerAtHostFlag,ParamArray::loadSaveOptions,&(values[i]),minValue,maxValue,false);
    }
  } 

  /** Returns the value of position i in the array */
  int getValue(int i) const{
    if (i>=0 && i<ParamArray::getSize() && i<ParamArray::getMaxSize())
      return values[i];
    else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(int minValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getIntParam(i)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(int maxValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getIntParam(i)->setMax(maxValueArg);
  }

  /** Updates the value from its UI value */ 
  void updateProcessorAndHostFromUi(int i,const int valueArg, UndoManager *const undoManager=nullptr, const bool dontCreateNewUndoTransaction=false) const{
    return ParamGroup::getIntParam(i)->updateProcessorAndHostFromUi(valueArg,undoManager,dontCreateNewUndoTransaction);
  }
  
  IntParamArray(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, int* const values,int *const size,const int maxSize,const int minValue=(int)(0),const int maxValue=(int)(1), bool saveOnlySizedArrayFlag=true):
  ParamArray(name,registerAtHostFlag,loadSaveOptions,size,maxSize,saveOnlySizedArrayFlag),
  values(values),
  minValue(minValue),
  maxValue(maxValue)
  {} 
};

/** ParamGroup containing an array of BoolParams. */
class BoolParamArray : public ParamArray{
private:
  bool* const values;

public: 
  void initParameters() override{
    for (int i=0;i<ParamArray::getMaxSize();i++){
      ParamGroup::addBoolParam(i,(String)(i),ParamArray::registerAtHostFlag,ParamArray::loadSaveOptions,&(values[i]),false);
    }
  } 

  /** Returns the value of position i in the array */
  bool getValue(int i) const{
    if (i>=0 && i<ParamArray::getSize() && i<ParamArray::getMaxSize())
      return values[i];
    else return 0;
  }    
 
  /** Updates the value from its UI value */ 
  void updateProcessorAndHostFromUi(int i,const bool valueArg, UndoManager *const undoManager=nullptr, const bool dontCreateNewUndoTransaction=false) const{
    return ParamGroup::getBoolParam(i)->updateProcessorAndHostFromUi(valueArg,undoManager,dontCreateNewUndoTransaction);
  }
  
  BoolParamArray(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, bool* const values,int *const size,const int maxSize,bool saveOnlySizedArrayFlag=true):
  ParamArray(name,registerAtHostFlag,loadSaveOptions,size,maxSize,saveOnlySizedArrayFlag),
  values(values)
  {} 
};


/** ParamGroup containing an array of StringParams. */
class StringParamArray : public ParamArray{
private:
  String* const values;

public: 
  void initParameters() override{
    for (int i=0;i<ParamArray::getMaxSize();i++){
      ParamGroup::addStringParam(i,(String)(i),ParamArray::registerAtHostFlag,ParamArray::loadSaveOptions,&(values[i]),false);
    }
  } 

  /** Returns the value of position i in the array */
  String getValue(int i) const{
    if (i>=0 && i<ParamArray::getSize() && i<ParamArray::getMaxSize())
      return values[i];
    else return String::empty;
  }    
 
  /** Updates the value from its UI value */ 
  void updateProcessorAndHostFromUi(int i,const String valueArg, UndoManager *const undoManager=nullptr, const bool dontCreateNewUndoTransaction=false) const{
    return ParamGroup::getStringParam(i)->updateProcessorAndHostFromUi(valueArg,undoManager,dontCreateNewUndoTransaction);
  }
  
  StringParamArray(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, String* const values,int *const size,const int maxSize,bool saveOnlySizedArrayFlag=true):
  ParamArray(name,registerAtHostFlag,loadSaveOptions,size,maxSize,saveOnlySizedArrayFlag),
  values(values)
  {
    // Strings cannot be automated! 
    // (They aren't supported at least in VST)
    // Try again setting argument registerAtHostFlag=false
    jassert(registerAtHostFlag==false);
  } 
};


//-----------------------------------------------------------------------------------

/** ParamGroup base class for a matrix of Params. */
class ParamMatrix : public ParamGroup{
protected:
  int *const numRows;
  int *const numCols;
  const int maxRows;
  const int maxCols;
  const bool registerAtHostFlag;
  const LoadSaveOptions loadSaveOptions;
  bool saveOnlySizedMatrixFlag;
  bool saveOnlyNonDefaultValuesFlag;
  bool updateOnlySizedMatrixFlag;
  
public:    
  virtual void initParameters() = 0;
  
  /** Returns the number of rows of the "visible" array */
  int getNumRows() const{
    if (numRows!=nullptr)
      return *numRows;
    else
      return maxRows;    
  }
  
  /** Returns the number of columns of the "visible" array */
  int getNumCols() const{
    if (numCols!=nullptr)
      return *numCols;
    else 
      return maxCols;
  }
  
  /** Returns the maximum (allocated) size of the array */
  int getMaxRows() const{
    return maxRows;
  }
  
  int getMaxCols() const{
    return maxCols;
  }  
  
  /** Returns the minimum range of each parameter in the matrix */
  const double getMin() const{
    if (((numCols!=nullptr && *numCols>0)||maxCols>0) && ((numRows!=nullptr && *numRows>0)||maxRows>0))
      return getParam(0,0)->getMin();
    else
      return 0;
  }
  
  /** Returns the maximum range of each parameter in the matrix */
  const double getMax() const{
    if (((numCols!=nullptr && *numCols>0)||maxCols>0) && ((numRows!=nullptr && *numRows>0)||maxRows>0))
      return getParam(0,0)->getMax();
    else
      return 0;
  }	
  
  Param *getParam(const int row,const int col) const{
    const int index=row*getNumCols()+col;
    return ParamGroup::getParam(index);
  }

  /** Update the host and the UI about all parameters in this row */
  void updateHostAndUiRow(int row,bool forceRunAfterParamChange,UpdateFromFlags updateFromFlag=UPDATE_FROM_PROCESSOR){
    for (int col=0;col<getNumCols();col++)
      getParam(row,col)->updateHost(forceRunAfterParamChange,updateFromFlag);
  }
  
  /** Update the host and the UI about all parameters in this column */
  void updateHostAndUiCol(int col,bool forceRunAfterParamChange,UpdateFromFlags updateFromFlag=UPDATE_FROM_PROCESSOR){
    for (int row=0;row<getNumRows();row++)
      getParam(row,col)->updateHost(forceRunAfterParamChange,updateFromFlag);
  }

  virtual void updateUi(const bool request,const bool applyRecursively){
    const int rows=(numRows!=nullptr)?*numRows:maxRows;
    const int cols=(numCols!=nullptr)?*numCols:maxCols;

    for (int i=0;i<rows;i++)
      for (int j=0;j<cols;j++)
        getParam(i,j)->updateUi(request);
    
    if (applyRecursively){     
      for (int g=0;g<getNumParamGroups();g++)
        getParamGroup(g)->updateUi(request,true);
    }
  }

  /** Stores the parameter values as an XML attribute.
      If createChild is set to true it will create a child XML node 
      (you want to disable it at the root node). 
      This method expands the default one to save only the 
      parameters with value other than defaultValue and/or 
      the parameters of the sized matrix. */
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool applyRecursively, XmlType xmlType=SESSION){
    
    if (saveXmlOptionCopy){
      switch(xmlType){
        case SESSION:
          for (int i=0;i<ParamMatrix::maxRows*ParamMatrix::maxCols;i++)
            saveXmlOptionCopy[i]=ParamGroup::getParam(i)->getOption(Param::saveToSession);
          break;
        case PRESET:
          for (int i=0;i<ParamMatrix::maxRows*ParamMatrix::maxCols;i++)
            saveXmlOptionCopy[i]=ParamGroup::getParam(i)->getOption(Param::saveToPresets);
          break;
        default:
          break;
      }
    }
    
    if (saveOnlySizedMatrixFlag){
      switch(xmlType){
        case SESSION:
          for (int i=0;i<ParamMatrix::getNumRows();i++){        
            for (int j=ParamMatrix::getNumCols();j<ParamMatrix::maxCols;j++){
              getParam(i,j)->setOption(Param::saveToSession,false);
            }        
          }
          break;
        case PRESET:
          for (int i=0;i<ParamMatrix::getNumRows();i++){        
            for (int j=ParamMatrix::getNumCols();j<ParamMatrix::maxCols;j++){
              getParam(i,j)->setOption(Param::saveToPresets,false);
            }        
          }
          break;
        default:
          break;
      }      
    }
    
    ParamGroup::saveXml(xml,createChild,applyRecursively);
    
    if (saveXmlOptionCopy){
      switch(xmlType){
        case SESSION:
          for (int i=0;i<maxRows*maxCols;i++)
            ParamGroup::getParam(i)->setOption(Param::saveToSession,saveXmlOptionCopy[i]);
          break;
        case PRESET:
          for (int i=0;i<maxRows*maxCols;i++)
            ParamGroup::getParam(i)->setOption(Param::saveToPresets,saveXmlOptionCopy[i]);
          break;
        default:
          break;
      }
    }
  }

  virtual void updateProcessorFromDefaultXml(const bool applyRecursively){
    runBeforeParamGroupUpdate();

    int updateRowsSize,updateColsSize;
    if (numRows!=nullptr && numCols!=nullptr && updateOnlySizedMatrixFlag){
      updateRowsSize=*numRows;
      updateColsSize=*numCols;
    } else {
      updateRowsSize=maxRows;
      updateColsSize=maxCols;
    }

    for (int i=0;i<updateRowsSize;i++)
      for (int j=0;j<updateColsSize;j++)
        getParam(i,j)->updateProcessorFromDefaultXml();
      
    if (applyRecursively){
      for (int i=0;i<getNumParamGroups();i++)
        getParamGroup(i)->updateProcessorFromDefaultXml(true);
    }
    setNonSavedChanges(true);
    runAfterParamGroupUpdate();
  }

  virtual void updateProcessorHostAndUiFromDefaultXml(bool forceRunAfterParamChange, bool forceUpdateUi,const bool applyRecursively){
    runBeforeParamGroupUpdate();   

    int updateRowsSize,updateColsSize;
    if (numRows!=nullptr && numCols!=nullptr && updateOnlySizedMatrixFlag){
      updateRowsSize=*numRows;
      updateColsSize=*numCols;
    } else {
      updateRowsSize=maxRows;
      updateColsSize=maxCols;
    }

    for (int i=0;i<updateRowsSize;i++)
      for (int j=0;j<updateColsSize;j++)
        getParam(i,j)->updateProcessorHostAndUiFromDefaultXml(forceRunAfterParamChange,forceUpdateUi);
      
    if (applyRecursively){
      for (int i=0;i<getNumParamGroups();i++)
        getParamGroup(i)->updateProcessorHostAndUiFromDefaultXml(forceRunAfterParamChange,forceUpdateUi,true);
    }
    setNonSavedChanges(true);
    runAfterParamGroupUpdate();
  }

  virtual void updateProcessorFromXml(const bool applyRecursively){
    runBeforeParamGroupUpdate();

    int updateRowsSize,updateColsSize;
    if (numRows!=nullptr && numCols!=nullptr && updateOnlySizedMatrixFlag){
      updateRowsSize=*numRows;
      updateColsSize=*numCols;
    } else {
      updateRowsSize=maxRows;
      updateColsSize=maxCols;
    }

    for (int i=0;i<updateRowsSize;i++)
      for (int j=0;j<updateColsSize;j++)
        getParam(i,j)->updateProcessorFromXml();
      
    if (applyRecursively){
      for (int i=0;i<getNumParamGroups();i++)
        getParamGroup(i)->updateProcessorFromXml(true);
    }
    setNonSavedChanges(false);
    runAfterParamGroupUpdate();
  }

  virtual void updateProcessorHostAndUiFromXml(bool forceUpdateHost, bool forceUpdateUi,const bool applyRecursively){
    runBeforeParamGroupUpdate();   

    int updateRowsSize,updateColsSize;
    if (numRows!=nullptr && numCols!=nullptr && updateOnlySizedMatrixFlag){
      updateRowsSize=*numRows;
      updateColsSize=*numCols;
    } else {
      updateRowsSize=maxRows;
      updateColsSize=maxCols;
    }

    for (int i=0;i<updateRowsSize;i++)
      for (int j=0;j<updateColsSize;j++)
        getParam(i,j)->updateProcessorHostAndUiFromXml(forceUpdateHost,forceUpdateUi);
      
    if (applyRecursively){
      for (int i=0;i<getNumParamGroups();i++)
        getParamGroup(i)->updateProcessorHostAndUiFromXml(forceUpdateHost,forceUpdateUi,true);
    }
    setNonSavedChanges(false);
    runAfterParamGroupUpdate();
  }
  
  ParamMatrix(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag=true, bool updateOnlySizedMatrixFlag=true):
  ParamGroup(name),
  numRows(numRows),
  numCols(numCols),
  maxRows(maxRows),  
  maxCols(maxCols),
  registerAtHostFlag(registerAtHostFlag),
  loadSaveOptions(loadSaveOptions),
  saveOnlySizedMatrixFlag(saveOnlySizedMatrixFlag),
  saveOnlyNonDefaultValuesFlag(saveOnlyNonDefaultValuesFlag),
  updateOnlySizedMatrixFlag(updateOnlySizedMatrixFlag)
  {
    saveXmlOptionCopy=new bool[maxCols*maxRows];
  }

  virtual ~ParamMatrix() {}
};

/** ParamGroup containing a matrix of FloatParams. */
class FloatParamMatrix : public ParamMatrix{
private:
  float** const values;
  float minValue;
  float maxValue;

public: 
  void initParameters() override{
    for (int i=0;i<ParamMatrix::getMaxRows();i++){
      for (int j=0;j<ParamMatrix::getMaxCols();j++){
        ParamGroup::addFloatParam(i*ParamMatrix::getMaxCols()+j,(String)(i)+":"+(String)j,ParamMatrix::registerAtHostFlag,ParamMatrix::loadSaveOptions,&(values[i][j]),minValue,maxValue,false);        
      }
    }
  }

  FloatParam* getFloatParam(int i,int j) const{
    return ParamGroup::getFloatParam(i*ParamMatrix::getMaxCols()+j);
  }

  /** Returns the value of position i,j in the array */
  float getValue(int i,int j) const{
    if (i>=0 && i<ParamMatrix::getNumRows() && i<ParamMatrix::getMaxRows()
        && j>=0 && j<ParamMatrix::getNumCols() && j<ParamMatrix::getMaxCols())
      return values[i][j];
    else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(float minValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getFloatParam(i*ParamMatrix::getMaxCols()+j)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(float maxValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getFloatParam(i*ParamMatrix::getMaxCols()+j)->setMax(maxValueArg);;      
  }
 
  /** Updates the value from its UI value */ 
  void updateProcessorAndHostFromUi(int i,int j,const float valueArg, UndoManager *const undoManager=nullptr, const bool dontCreateNewUndoTransaction=false) const{
    return ParamGroup::getFloatParam(i*ParamMatrix::getMaxCols()+j)->updateProcessorAndHostFromUi(valueArg,undoManager,dontCreateNewUndoTransaction);
  } 
  
  FloatParamMatrix(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols,const float minValue=(float)(0),const float maxValue=(float)(1), const bool saveOnlySizedMatrixFlag=true):
  ParamMatrix(name,registerAtHostFlag,loadSaveOptions,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag),
  values(values),
  minValue(minValue),
  maxValue(maxValue)
  {} 
};

/** ParamGroup containing a matrix of LogParams. */
class LogParamMatrix : public ParamMatrix{
private:
  float** const values;
  float minValue;
  float maxValue;
  bool sparseCompressionFlag; 
  float mostProbableValue;

public: 
  void initParameters() override{
    for (int i=0;i<ParamMatrix::getMaxRows();i++){
      for (int j=0;j<ParamMatrix::getMaxCols();j++){
        ParamGroup::addLogParam(i*ParamMatrix::getMaxCols()+j,(String)(i)+":"+(String)j,ParamMatrix::registerAtHostFlag,ParamMatrix::loadSaveOptions,&(values[i][j]),minValue,maxValue,false);        
      }
    }
  }

  LogParam* getLogParam(int i,int j) const{
    return ParamGroup::getLogParam(i*ParamMatrix::getMaxCols()+j);
  }

  /** Returns the value of position i,j in the array */
  float getValue(int i,int j) const{
    if (i>=0 && i<ParamMatrix::getNumRows() && i<ParamMatrix::getMaxRows()
        && j>=0 && j<ParamMatrix::getNumCols() && j<ParamMatrix::getMaxCols())
      return values[i][j];
    else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(float minValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getLogParam(i*ParamMatrix::getMaxCols()+j)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(float maxValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getLogParam(i*ParamMatrix::getMaxCols()+j)->setMax(maxValueArg);     
  }

  /** Updates the value from its UI value */ 
  void updateProcessorAndHostFromUi(int i,int j,const double valueArg, UndoManager *const undoManager=nullptr, const bool dontCreateNewUndoTransaction=false) const{
    return ParamGroup::getLogParam(i*ParamMatrix::getMaxCols()+j)->updateProcessorAndHostFromUi((float)valueArg,undoManager,dontCreateNewUndoTransaction);
  } 

  LogParamMatrix(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const float minValue=(float)(0),const float maxValue=(float)(1), const bool saveOnlySizedMatrixFlag=true):
  ParamMatrix(name,registerAtHostFlag,loadSaveOptions,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag),
  values(values),
  minValue(minValue),
  maxValue(maxValue)
  {} 
};

/** ParamGroup containing a matrix of LogWith0Params. */
class LogWith0ParamMatrix : public ParamMatrix{
private:
  float** const values;
  float minValue;
  float maxValue;
  bool sparseCompressionFlag; 
  float mostProbableValue;

public: 
  void initParameters() override{
    for (int i=0;i<ParamMatrix::getMaxRows();i++){
      for (int j=0;j<ParamMatrix::getMaxCols();j++){
        ParamGroup::addLogWith0Param(i*ParamMatrix::getMaxCols()+j,(String)(i)+":"+(String)j,ParamMatrix::registerAtHostFlag,ParamMatrix::loadSaveOptions,&(values[i][j]),minValue,maxValue,false);        
      }
    }
  }

  LogWith0Param* getLogWith0Param(int i,int j) const{
    return ParamGroup::getLogWith0Param(i*ParamMatrix::getMaxCols()+j);
  }

  /** Returns the value of position i,j in the array */
  float getValue(int i,int j) const{
    if (i>=0 && i<ParamMatrix::getNumRows() && i<ParamMatrix::getMaxRows()
        && j>=0 && j<ParamMatrix::getNumCols() && j<ParamMatrix::getMaxCols())
      return values[i][j];
    else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(float minValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getLogWith0Param(i*ParamMatrix::getMaxCols()+j)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(float maxValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getLogWith0Param(i*ParamMatrix::getMaxCols()+j)->setMax(maxValueArg);;      
  }

  /** Updates the value from its UI value */ 
  void updateProcessorAndHostFromUi(int i,int j,const double valueArg, UndoManager *const undoManager=nullptr, const bool dontCreateNewUndoTransaction=false) const{
    return ParamGroup::getLogWith0Param(i*ParamMatrix::getMaxCols()+j)->updateProcessorAndHostFromUi((float)valueArg,undoManager,dontCreateNewUndoTransaction);
  }
  
  LogWith0ParamMatrix(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const float minValue=(float)(0),const float maxValue=(float)(1), const bool saveOnlySizedMatrixFlag=true):
  ParamMatrix(name,registerAtHostFlag,loadSaveOptions,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag),
  values(values),
  minValue(minValue),
  maxValue(maxValue)
  {} 
};

/** ParamGroup containing a matrix of LogWithSignParams. */
class LogWithSignParamMatrix : public ParamMatrix{
private:
  float** const values;
  float minValue;
  float maxValue;
  bool sparseCompressionFlag; 
  float mostProbableValue;

public: 
  void initParameters() override{
    for (int i=0;i<ParamMatrix::getMaxRows();i++){
      for (int j=0;j<ParamMatrix::getMaxCols();j++){
        ParamGroup::addLogWithSignParam(i*ParamMatrix::getMaxCols()+j,(String)(i)+":"+(String)j,ParamMatrix::registerAtHostFlag,ParamMatrix::loadSaveOptions,&(values[i][j]),minValue,maxValue,false);        
      }
    }
  }

  LogWithSignParam* getLogWithSignParam(int i,int j) const{
    return ParamGroup::getLogWithSignParam(i*ParamMatrix::getMaxCols()+j);
  }

  /** Returns the value of position i,j in the array */
  float getValue(int i,int j) const{
    if (i>=0 && i<ParamMatrix::getNumRows() && i<ParamMatrix::getMaxRows()
        && j>=0 && j<ParamMatrix::getNumCols() && j<ParamMatrix::getMaxCols())
      return values[i][j];
    else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(float minValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getLogWithSignParam(i*ParamMatrix::getMaxCols()+j)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(float maxValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getLogWithSignParam(i*ParamMatrix::getMaxCols()+j)->setMax(maxValueArg);;      
  }

  /** Updates the value from its UI value */ 
  void updateProcessorAndHostFromUi(int i,int j,const float valueArg, UndoManager *const undoManager=nullptr, const bool dontCreateNewUndoTransaction=false) const{
    return ParamGroup::getLogWithSignParam(i*ParamMatrix::getMaxCols()+j)->updateProcessorAndHostFromUi(valueArg,undoManager,dontCreateNewUndoTransaction);
  } 
  
  LogWithSignParamMatrix(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const float minValue=(float)(0),const float maxValue=(float)(1), const bool saveOnlySizedMatrixFlag=true):
  ParamMatrix(name,registerAtHostFlag,loadSaveOptions,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag),
  values(values),
  minValue(minValue),
  maxValue(maxValue)
  {} 
};

/** ParamGroup containing a matrix of IntParams. */
class IntParamMatrix : public ParamMatrix{
private:
  int** const values;  
  int minValue;
  int maxValue;

public: 
  void initParameters() override{
    for (int i=0;i<ParamMatrix::getMaxRows();i++){
      for (int j=0;j<ParamMatrix::getMaxCols();j++){
        ParamGroup::addIntParam(i*ParamMatrix::getMaxCols()+j,(String)(i)+":"+(String)j,ParamMatrix::registerAtHostFlag,ParamMatrix::loadSaveOptions,&(values[i][j]),minValue,maxValue,false);        
      }
    }
  }

  IntParam* getIntParam(int i,int j) const{
    return ParamGroup::getIntParam(i*ParamMatrix::getMaxCols()+j);
  }

  /** Returns the value of position i,j in the array */
  int getValue(int i,int j) const{
    if (i>=0 && i<ParamMatrix::getNumRows() && i<ParamMatrix::getMaxRows()
        && j>=0 && j<ParamMatrix::getNumCols() && j<ParamMatrix::getMaxCols())
      return values[i][j];
    else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(int minValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getIntParam(i*ParamMatrix::getMaxCols()+j)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(int maxValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getIntParam(i*ParamMatrix::getMaxCols()+j)->setMax(maxValueArg);
  }

  /** Updates the value from its UI value */ 
  void updateProcessorAndHostFromUi(int i,int j,const int valueArg, UndoManager *const undoManager=nullptr, const bool dontCreateNewUndoTransaction=false) const{
    return ParamGroup::getIntParam(i*ParamMatrix::getMaxCols()+j)->updateProcessorAndHostFromUi(valueArg,undoManager,dontCreateNewUndoTransaction);
  } 
  
  IntParamMatrix(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, int** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols,const int minValue=(int)(0),const int maxValue=(int)(1), const bool saveOnlySizedMatrixFlag=true):
  ParamMatrix(name,registerAtHostFlag,loadSaveOptions,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag),
  values(values),
  minValue(minValue),
  maxValue(maxValue)
  {} 
};


/** ParamGroup containing a matrix of BoolParams. */
class BoolParamMatrix : public ParamMatrix{
private:
  bool** const values;

public: 
  void initParameters() override{
    for (int i=0;i<ParamMatrix::getMaxRows();i++){
      for (int j=0;j<ParamMatrix::getMaxCols();j++){
        ParamGroup::addBoolParam(i*ParamMatrix::getMaxCols()+j,(String)(i)+":"+(String)j,ParamMatrix::registerAtHostFlag,ParamMatrix::loadSaveOptions,&(values[i][j]),false);
      }
    }
  } 

  BoolParam* getBoolParam(int i,int j) const{
    return ParamGroup::getBoolParam(i*ParamMatrix::getMaxCols()+j);
  }

  /** Returns the value of position i,j in the array */
  bool getValue(int i,int j) const{
    if (i>=0 && i<ParamMatrix::getNumRows() && i<ParamMatrix::getMaxRows()
        && j>=0 && j<ParamMatrix::getNumCols() && j<ParamMatrix::getMaxCols())
      return values[i][j];
    else return 0;
  }   
 
  /** Updates the value from its UI value */ 
  void updateProcessorAndHostFromUi(int i,int j,const bool valueArg, UndoManager *const undoManager=nullptr, const bool dontCreateNewUndoTransaction=false) const{
    return ParamGroup::getBoolParam(i*ParamMatrix::getMaxCols()+j)->updateProcessorAndHostFromUi(valueArg,undoManager,dontCreateNewUndoTransaction);
  } 
  
  BoolParamMatrix(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, bool** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag=true):
  ParamMatrix(name,registerAtHostFlag,loadSaveOptions,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag),  
  values(values)
  {} 
};

/** ParamGroup containing a matrix of StringParams. */
class StringParamMatrix : public ParamMatrix{
private:
  String** const values;

public: 
  void initParameters() override{
    for (int i=0;i<ParamMatrix::getMaxRows();i++){
      for (int j=0;j<ParamMatrix::getMaxCols();j++){
        ParamGroup::addStringParam(i*ParamMatrix::getMaxCols()+j,(String)(i)+":"+(String)j,ParamMatrix::registerAtHostFlag,ParamMatrix::loadSaveOptions,&(values[i][j]),false);
      }
    }
  } 

  StringParam* getStringParam(int i,int j) const{
    return ParamGroup::getStringParam(i*ParamMatrix::getMaxCols()+j);
  }

  /** Returns the value of position i,j in the array */
  String getValue(int i,int j) const{
    if (i>=0 && i<ParamMatrix::getNumRows() && i<ParamMatrix::getMaxRows()
        && j>=0 && j<ParamMatrix::getNumCols() && j<ParamMatrix::getMaxCols())
      return values[i][j];
    else return String::empty;
  }   
 
  /** Updates the value from its UI value */ 
  void updateProcessorAndHostFromUi(int i,int j,const String valueArg, UndoManager *const undoManager=nullptr, const bool dontCreateNewUndoTransaction=false) const{
    return ParamGroup::getStringParam(i*ParamMatrix::getMaxCols()+j)->updateProcessorAndHostFromUi(valueArg,undoManager,dontCreateNewUndoTransaction);
  } 
  
  StringParamMatrix(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, String** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag=true):
  ParamMatrix(name,registerAtHostFlag,loadSaveOptions,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag),  
  values(values)
  {
    // Strings cannot be automated! 
    // (They aren't supported at least in VST)
    // Try again setting argument registerAtHostFlag=false
    jassert(registerAtHostFlag==false);
  } 
};
#endif