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

#ifndef __PLUGINPARAMETERS_PARAMGROUPS_HEADER__
#define __PLUGINPARAMETERS_PARAMGROUPS_HEADER__

#include "Params.h"

class StringParamArray;

template<class FloatType, class FloatTypeMap = IdentityMap<FloatType> >
class FloatTypeParamArray;

template<class IntType>
class IntTypeParamArray;

class BoolParamArray;

class StringParamMatrix;

template<class FloatType, class FloatTypeMap = IdentityMap<FloatType> >
class FloatTypeParamMatrix;

template<class IntType>
class IntTypeParamMatrix;

class BoolParamMatrix;

/** Base class for all groups of parameters. Distinctions by type are made below. */
class ParamGroup{  
  JUCE_DECLARE_NON_COPYABLE(ParamGroup)

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
    /* Wrong index? */
    if (index<0 || index>=paramList.size()) {jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"Param index out of bounds: "+String(index)+" in ParamGroup with XML name: "+this->getXmlName()); return nullptr;}
    /* You are trying to fetch a parameter with another type... 
        You want to use another method such as:
        getStringParam(), getFloatParam(), getLogParam(), etc  */
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
  
  template<class FloatType, class FloatTypeMap = IdentityMap<FloatType> >
  FloatTypeParam<FloatType,FloatTypeMap> *getFloatTypeParam(const int index) const{
    /* Wrong index? */
    if (index<0 || index>=paramList.size()) {jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"Param index out of bounds: "+String(index)+" in ParamGroup with XML name: "+this->getXmlName()); return nullptr;}
    /* You are trying to fetch a parameter with another type... 
        You want to use another method such as:
        getStringParam(), getFloatParam(), getLogParam(), etc  */
    FloatTypeParam<FloatType,FloatTypeMap> *pointer=dynamic_cast<FloatTypeParam<FloatType,FloatTypeMap> *>(paramList[index]);
    if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a parameter with another type..."); return nullptr;}
    return pointer;
  }  

  template<class FloatType, class FloatTypeMap = IdentityMap<FloatType> >
  void addFloatTypeParam(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, FloatType *const value, const FloatType minValue=(FloatType)(0),const FloatType maxValue=(FloatType)(1),bool forceUniqueXmlName=true){
    Param *param;
    addParam(paramIndex,param=new FloatTypeParam<FloatType,FloatTypeMap>(name,registerAtHostFlag,loadSaveOptions,value,minValue,maxValue),forceUniqueXmlName);
    paramsToUnallocateAtDestructor.add(param);
  }

  template<class FloatType, class FloatTypeMap = IdentityMap<FloatType> >
  void addFloatTypeParamArray(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, FloatType* const values, int *const size, const int maxSize, const FloatType minValue = (FloatType)(0), const FloatType maxValue = (FloatType)(1), bool saveOnlySizedArrayFlag = true){
    ParamGroup *paramGroup;
    addParamGroup(paramIndex, paramGroup = new FloatTypeParamArray<FloatType, FloatTypeMap>(name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, minValue, maxValue, saveOnlySizedArrayFlag));
    paramGroupsToUnallocateAtDestructor.add(paramGroup);
  }

  template<class FloatType, class FloatTypeMap = IdentityMap<FloatType> >
  void addFloatTypeParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, FloatType** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const FloatType minValue = (FloatType)(0), const FloatType maxValue = (FloatType)(1), const bool saveOnlySizedMatrixFlag = true){
    ParamGroup *paramGroup;
    addParamGroup(paramIndex, paramGroup = new FloatTypeParamMatrix<FloatType, FloatTypeMap>(name, registerAtHostFlag, loadSaveOptions, values, numRows, numCols, maxRows, maxCols, minValue, maxValue, saveOnlySizedMatrixFlag));
    paramGroupsToUnallocateAtDestructor.add(paramGroup);
  }

  template<class FloatType, class FloatTypeMap = IdentityMap<FloatType> >
  FloatTypeParamArray<FloatType, FloatTypeMap> *getFloatTypeParamArray(const int index) const{
    /* Wrong index? */
    if (index<0 || index >= paramGroupList.size()) { jassertfalse; return nullptr; }
    FloatTypeParamArray<FloatType, FloatTypeMap> *pointer = dynamic_cast<FloatTypeParamArray<FloatType, FloatTypeMap> *>(paramGroupList[index]);
    if (pointer == nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__) + ":" + String(__LINE__) + "::" + "You are trying to fetch a ParamGroup of another type..."); return nullptr; }
    return pointer;
  }

  template<class FloatType, class FloatTypeMap = IdentityMap<FloatType> >
  FloatTypeParamMatrix<FloatType, FloatTypeMap> *getFloatTypeParamMatrix(const int index) const{
    /* Wrong index? */
    if (index<0 || index >= paramGroupList.size()) { jassertfalse; return nullptr; }
    FloatTypeParamMatrix<FloatType, FloatTypeMap> *pointer = dynamic_cast<FloatTypeParamMatrix<FloatType, FloatTypeMap> *>(paramGroupList[index]);
    if (pointer == nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__) + ":" + String(__LINE__) + "::" + "You are trying to fetch a ParamGroup of another type..."); return nullptr; }
    return pointer;
  }

  FloatParam* getFloatParam(const int index) const{
    return getFloatTypeParam<float>(index);
  }

  void addFloatParam(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float *const value, const float minValue=(float)(0),const float maxValue=(float)(1),bool forceUniqueXmlName=true){
    addFloatTypeParam<float>(paramIndex,name,registerAtHostFlag,loadSaveOptions,value,minValue,maxValue,forceUniqueXmlName);
  }   

  void addFloatParamArray(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float* const values, int *const size, const int maxSize, const float minValue = (float)(0), const float maxValue = (float)(1), bool saveOnlySizedArrayFlag = true){
    addFloatTypeParamArray<float>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, minValue, maxValue, saveOnlySizedArrayFlag);
  }

  void addFloatParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const float minValue = (float)(0), const float maxValue = (float)(1), const bool saveOnlySizedMatrixFlag = true){    
    addFloatTypeParamMatrix<float>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, numRows, numCols, maxRows, maxCols, minValue, maxValue, saveOnlySizedMatrixFlag);
  }

  FloatTypeParamArray<float> *getFloatParamArray(const int index) const{
    return getFloatTypeParamArray<float>(index);
  }

  FloatTypeParamMatrix<float> *getFloatParamMatrix(const int index) const{
    return getFloatTypeParamMatrix<float>(index);
  }
  
  LogParam *getLogParam(const int index) const{
    return getFloatTypeParam<float,DefaultLogMap>(index);
  }
   
  void addLogParam(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float *const value, const float minValue=(float)(0.001),const float maxValue=(float)(1),bool forceUniqueXmlName=true){
    addFloatTypeParam<float,DefaultLogMap>(paramIndex,name,registerAtHostFlag,loadSaveOptions,value,minValue,maxValue, forceUniqueXmlName);
  }   

   void addLogParamArray(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float* const values, int *const size, const int maxSize, const float minValue = (float)(0), const float maxValue = (float)(1), bool saveOnlySizedArrayFlag = true){
     addFloatTypeParamArray<float,DefaultLogMap>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, minValue, maxValue, saveOnlySizedArrayFlag);
   }

   void addLogParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const float minValue = (float)(0), const float maxValue = (float)(1), const bool saveOnlySizedMatrixFlag = true){
     addFloatTypeParamMatrix<float,DefaultLogMap>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, numRows, numCols, maxRows, maxCols, minValue, maxValue, saveOnlySizedMatrixFlag);
   }
  
   FloatTypeParamArray<float, DefaultLogMap> *getLogParamArray(const int index) const{
     return getFloatTypeParamArray<float,DefaultLogMap>(index);
   }

   FloatTypeParamMatrix<float, DefaultLogMap> *getLogParamMatrix(const int index) const{
     return getFloatTypeParamMatrix<float,DefaultLogMap>(index);
   }

   FloatTypeParam<float, DefaultLogWith0Map> *getLogWith0Param(const int index) const{
    return getFloatTypeParam<float, DefaultLogWith0Map>(index);
  }  
   
   void addLogWith0Param(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float *const value, const float minValue=(float)(0.001),const float maxValue=(float)(1),bool forceUniqueXmlName=true){
     addFloatTypeParam<float, DefaultLogWith0Map>(paramIndex, name, registerAtHostFlag, loadSaveOptions, value, minValue, maxValue, forceUniqueXmlName);
  }

   void addLogWith0ParamArray(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float* const values, int *const size, const int maxSize, const float minValue = (float)(0), const float maxValue = (float)(1), bool saveOnlySizedArrayFlag = true){
     addFloatTypeParamArray<float, DefaultLogWith0Map>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, minValue, maxValue, saveOnlySizedArrayFlag);
   }

   void addLogWith0ParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const float minValue = (float)(0), const float maxValue = (float)(1), const bool saveOnlySizedMatrixFlag = true){
     addFloatTypeParamMatrix<float, DefaultLogWith0Map>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, numRows, numCols, maxRows, maxCols, minValue, maxValue, saveOnlySizedMatrixFlag);
   }
  
   FloatTypeParamArray<float, DefaultLogWith0Map> *getLogWith0ParamArray(const int index) const{
     return getFloatTypeParamArray<float, DefaultLogWith0Map>(index);
   }

   FloatTypeParamMatrix<float, DefaultLogWith0Map> *getLogWith0ParamMatrix(const int index) const{
     return getFloatTypeParamMatrix<float, DefaultLogWith0Map>(index);
   }

   FloatTypeParam<float, DefaultLogWithSignMap> *getLogWithSignParam(const int index) const{
    return getFloatTypeParam<float, DefaultLogWithSignMap>(index);
  }  
   
   void addLogWithSignParam(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float *const value, const float minNegativeValue=(float)(-1),const float maxPositiveValue=(float)(1),bool forceUniqueXmlName=true){
     addFloatTypeParam<float, DefaultLogWithSignMap>(paramIndex, name, registerAtHostFlag, loadSaveOptions, value, minNegativeValue, maxPositiveValue, forceUniqueXmlName);
    }

   void addLogWithSignParamArray(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float* const values, int *const size, const int maxSize, const float minValue = (float)(0), const float maxValue = (float)(1), bool saveOnlySizedArrayFlag = true){
     addFloatTypeParamArray<float, DefaultLogWithSignMap>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, minValue, maxValue, saveOnlySizedArrayFlag);
   }

   void addLogWithSignParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const float minValue = (float)(0), const float maxValue = (float)(1), const bool saveOnlySizedMatrixFlag = true){
     addFloatTypeParamMatrix<float, DefaultLogWithSignMap>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, numRows, numCols, maxRows, maxCols, minValue, maxValue, saveOnlySizedMatrixFlag);
   }

   FloatTypeParamArray<float, DefaultLogWithSignMap> *getLogWithSignParamArray(const int index) const{
     return getFloatTypeParamArray<float, DefaultLogWithSignMap>(index);
   }

   FloatTypeParamMatrix<float, DefaultLogWithSignMap> *getLogWithSignParamMatrix(const int index) const{
     return getFloatTypeParamMatrix<float, DefaultLogWithSignMap>(index);
   }

  DoubleParam* getDoubleParam(const int index) const{
    return getFloatTypeParam<double>(index);
  }

  void addDoubleParam(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, double *const value, const double minValue = (double)(0), const double maxValue = (double)(1), bool forceUniqueXmlName = true){
    addFloatTypeParam<double>(paramIndex, name, registerAtHostFlag, loadSaveOptions, value, minValue, maxValue, forceUniqueXmlName);
  }

  void addDoubleParamArray(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, double* const values, int *const size, const int maxSize, const double minValue = (double)(0), const double maxValue = (double)(1), bool saveOnlySizedArrayFlag = true){
    addFloatTypeParamArray<double>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, minValue, maxValue, saveOnlySizedArrayFlag);
  }

  void addDoubleParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, double** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const double minValue = (double)(0), const double maxValue = (double)(1), const bool saveOnlySizedMatrixFlag = true){
    addFloatTypeParamMatrix<double>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, numRows, numCols, maxRows, maxCols, minValue, maxValue, saveOnlySizedMatrixFlag);
  }

  FloatTypeParamArray<double> *getDoubleParamArray(const int index) const{
    return getFloatTypeParamArray<double>(index);
  }

  FloatTypeParamMatrix<double> *getDoubleParamMatrix(const int index) const{
    return getFloatTypeParamMatrix<double>(index);
  }

  FloatTypeParam<double, DefaultDoubleLogMap> *getDoubleLogParam(const int index) const{
    return getFloatTypeParam<double, DefaultDoubleLogMap>(index);
  }

  void addDoubleLogParam(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, double *const value, const double minValue = (double)(0.001), const double maxValue = (double)(1), bool forceUniqueXmlName = true){
    addFloatTypeParam<double, DefaultDoubleLogMap>(paramIndex, name, registerAtHostFlag, loadSaveOptions, value, minValue, maxValue, forceUniqueXmlName);
  }

  void addDoubleLogParamArray(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, double* const values, int *const size, const int maxSize, const double minValue = (double)(0), const double maxValue = (double)(1), bool saveOnlySizedArrayFlag = true){
    addFloatTypeParamArray<double, DefaultDoubleLogMap>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, minValue, maxValue, saveOnlySizedArrayFlag);
  }

  void addDoubleLogParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, double** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const double minValue = (double)(0), const double maxValue = (double)(1), const bool saveOnlySizedMatrixFlag = true)
  {
    addFloatTypeParamMatrix<double, DefaultDoubleLogMap>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, numRows, numCols, maxRows, maxCols, minValue, maxValue, saveOnlySizedMatrixFlag);
  }

  FloatTypeParamArray<double, DefaultDoubleLogMap> *getDoubleLogParamArray(const int index) const{
    return getFloatTypeParamArray<double, DefaultDoubleLogMap>(index);
  }

  FloatTypeParamMatrix<double, DefaultDoubleLogMap> *getDoubleLogParamMatrix(const int index) const{
    return getFloatTypeParamMatrix<double, DefaultDoubleLogMap>(index);
  }

  FloatTypeParam<double, DefaultDoubleLogWith0Map> *getDoubleLogWith0Param(const int index) const{
    return getFloatTypeParam<double, DefaultDoubleLogWith0Map>(index);
  }

  void addDoubleLogWith0Param(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, double *const value, const double minValue = (double)(0.001), const double maxValue = (double)(1), bool forceUniqueXmlName = true){
    addFloatTypeParam<double, DefaultDoubleLogWith0Map>(paramIndex, name, registerAtHostFlag, loadSaveOptions, value, minValue, maxValue, forceUniqueXmlName);
  }

  void addDoubleLogWith0ParamArray(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, double* const values, int *const size, const int maxSize, const double minValue = (double)(0), const double maxValue = (double)(1), bool saveOnlySizedArrayFlag = true){
    addFloatTypeParamArray<double, DefaultDoubleLogWith0Map>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, minValue, maxValue, saveOnlySizedArrayFlag);
  }

  void addDoubleLogWith0ParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, double** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const double minValue = (double)(0), const double maxValue = (double)(1), const bool saveOnlySizedMatrixFlag = true){
    addFloatTypeParamMatrix<double, DefaultDoubleLogWith0Map>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, numRows, numCols, maxRows, maxCols, minValue, maxValue, saveOnlySizedMatrixFlag);
  }

  FloatTypeParamArray<double, DefaultDoubleLogWith0Map> *getDoubleLogWith0ParamArray(const int index) const{
    return getFloatTypeParamArray<double, DefaultDoubleLogWith0Map>(index);
  }

  FloatTypeParamMatrix<double, DefaultDoubleLogWith0Map> *getDoubleLogWith0ParamMatrix(const int index) const{
    return getFloatTypeParamMatrix<double, DefaultDoubleLogWith0Map>(index);
  }

  DoubleLogWithSignParam *getDoubleLogWithSignParam(const int index) const{
    return getFloatTypeParam<double, DefaultDoubleLogWithSignMap>(index);
  }

  void addDoubleLogWithSignParam(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, double *const value, const double minNegativeValue = (double)(-1), const double maxPositiveValue = (double)(1), bool forceUniqueXmlName = true){
    addFloatTypeParam<double, DefaultDoubleLogWithSignMap>(paramIndex, name, registerAtHostFlag, loadSaveOptions, value, minNegativeValue, maxPositiveValue, forceUniqueXmlName);
  }

  void addDoubleLogWithSignParamArray(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, double* const values, int *const size, const int maxSize, const double minValue = (double)(0), const double maxValue = (double)(1), bool saveOnlySizedArrayFlag = true){
    addFloatTypeParamArray<double, DefaultDoubleLogWithSignMap>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, minValue, maxValue, saveOnlySizedArrayFlag);
  }

  void addDoubleLogWithSignParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, double** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const double minValue = (double)(0), const double maxValue = (double)(1), const bool saveOnlySizedMatrixFlag = true){
    addFloatTypeParamMatrix<double, DefaultDoubleLogWithSignMap>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, numRows, numCols, maxRows, maxCols, minValue, maxValue, saveOnlySizedMatrixFlag);
  }

  FloatTypeParamArray<double, DefaultDoubleLogWithSignMap> *getDoubleLogWithSignParamArray(const int index) const{
    return getFloatTypeParamArray<double, DefaultDoubleLogWithSignMap>(index);
  }

  FloatTypeParamMatrix<double, DefaultDoubleLogWithSignMap> *getDoubleLogWithSignParamMatrix(const int index) const{
    return getFloatTypeParamMatrix<double, DefaultDoubleLogWithSignMap>(index);
  }

  template<class IntType>
  IntTypeParam<IntType> *getIntTypeParam(const int index) const{
    /* Wrong index? */ 
    if (index<0 || index>=paramList.size()) {jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"Param index out of bounds: "+String(index)+" in ParamGroup with XML name: "+this->getXmlName()); return nullptr;}
    /* You are trying to fetch a parameter with another type... 
        You want to use another method such as:
        getStringParam(), getFloatParam(), getLogParam(), etc */
    IntTypeParam<IntType> *pointer=dynamic_cast<IntTypeParam<IntType> *>(paramList[index]);
    if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a parameter with another type..."); return nullptr;}
    return pointer;
  }

  template<class IntType>
  void addIntTypeParam(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, IntType *const value, const IntType minValue = 0, const IntType maxValue = 1, bool forceUniqueXmlName = true){
    Param *param;
    addParam(paramIndex, param = new IntTypeParam<IntType>(name, registerAtHostFlag, loadSaveOptions, value, minValue, maxValue), forceUniqueXmlName);
    paramsToUnallocateAtDestructor.add(param);
  }  

  template<class IntType>
  void addIntTypeParamArray(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, IntType* const values, int *const size, const int maxSize, const IntType minValue = (IntType)(0), const IntType maxValue = (IntType)(1), bool saveOnlySizedArrayFlag = true){
    ParamGroup *paramGroup;
    addParamGroup(paramIndex, paramGroup = new IntTypeParamArray<IntType>(name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, minValue, maxValue, saveOnlySizedArrayFlag));
    paramGroupsToUnallocateAtDestructor.add(paramGroup);
  }

  template<class IntType>
  void addIntTypeParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, IntType** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const IntType minValue = (IntType)(0), const IntType maxValue = (IntType)(1), const bool saveOnlySizedMatrixFlag = true){
    ParamGroup *paramGroup;
    addParamGroup(paramIndex, paramGroup = new IntTypeParamMatrix<IntType>(name, registerAtHostFlag, loadSaveOptions, values, numRows, numCols, maxRows, maxCols, minValue, maxValue, saveOnlySizedMatrixFlag));
    paramGroupsToUnallocateAtDestructor.add(paramGroup);
  }

  template<class IntType>
  IntTypeParamArray<IntType> *getIntTypeParamArray(const int index) const{
    /* Wrong index? */
    if (index<0 || index >= paramGroupList.size()) { jassertfalse; return nullptr; }
    IntTypeParamArray<IntType> *pointer = dynamic_cast<IntTypeParamArray<IntType> *>(paramGroupList[index]);
    if (pointer == nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__) + ":" + String(__LINE__) + "::" + "You are trying to fetch a ParamGroup of another type..."); return nullptr; }
    return pointer;
  }

  template<class IntType>
  IntTypeParamMatrix<IntType> *getIntTypeParamMatrix(const int index) const{
    /* Wrong index? */
    if (index<0 || index >= paramGroupList.size()) { jassertfalse; return nullptr; }
    IntTypeParamMatrix<IntType> *pointer = dynamic_cast<IntTypeParamMatrix<IntType> *>(paramGroupList[index]);
    if (pointer == nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__) + ":" + String(__LINE__) + "::" + "You are trying to fetch a ParamGroup of another type..."); return nullptr; }
    return pointer;
  }  

  IntParam *getIntParam(const int index) const{
    return getIntTypeParam<int>(index);
  }

  Int8Param *getInt8Param(const int index) const{
    return getIntTypeParam<juce::int8>(index);
  }

  Uint8Param *getUint8Param(const int index) const{
    return getIntTypeParam<juce::uint8>(index);
  }

  Int16Param *getInt16Param(const int index) const{
    return getIntTypeParam<juce::int16>(index);
  }

  Uint16Param *getUint16Param(const int index) const{
    return getIntTypeParam<juce::uint16>(index);
  }

  Int32Param *getInt32Param(const int index) const{
    return getIntTypeParam<juce::int32>(index);
  }

  Uint32Param *getUint32Param(const int index) const{
    return getIntTypeParam<juce::uint32>(index);
  }

  Int64Param *getInt64Param(const int index) const{
    return getIntTypeParam<juce::int64>(index);
  }

  Uint64Param *getUint64Param(const int index) const{
    return getIntTypeParam<juce::uint64>(index);
  }

  void addIntParam(const int paramIndex,const String &name,const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions,int *const value, const int minValue=0,const int maxValue=1,bool forceUniqueXmlName=true){
    addIntTypeParam<int>(paramIndex,name,registerAtHostFlag,loadSaveOptions,value,minValue,maxValue,forceUniqueXmlName);
  }

  void addInt8Param(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::int8 *const value, const juce::int8 minValue = 0, const juce::int8 maxValue = 1, bool forceUniqueXmlName = true){
    addIntTypeParam<juce::int8>(paramIndex, name, registerAtHostFlag, loadSaveOptions, value, minValue, maxValue, forceUniqueXmlName);
  }

  void addUint8Param(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::uint8 *const value, const juce::uint8 minValue = 0, const juce::uint8 maxValue = 1, bool forceUniqueXmlName = true){
    addIntTypeParam<juce::uint8>(paramIndex, name, registerAtHostFlag, loadSaveOptions, value, minValue, maxValue, forceUniqueXmlName);
  }

  void addInt16Param(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::int16 *const value, const juce::int16 minValue = 0, const juce::int16 maxValue = 1, bool forceUniqueXmlName = true){
    addIntTypeParam<juce::int16>(paramIndex, name, registerAtHostFlag, loadSaveOptions, value, minValue, maxValue, forceUniqueXmlName);
  }

  void addUint16Param(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::uint16 *const value, const juce::uint16 minValue = 0, const juce::uint16 maxValue = 1, bool forceUniqueXmlName = true){
    addIntTypeParam<juce::uint16>(paramIndex, name, registerAtHostFlag, loadSaveOptions, value, minValue, maxValue, forceUniqueXmlName);
  }

  void addInt32Param(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::int32 *const value, const juce::int32 minValue = 0, const juce::int32 maxValue = 1, bool forceUniqueXmlName = true){
    addIntTypeParam<juce::int32>(paramIndex, name, registerAtHostFlag, loadSaveOptions, value, minValue, maxValue, forceUniqueXmlName);
  }

  void addUint32Param(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::uint32 *const value, const juce::uint32 minValue = 0, const juce::uint32 maxValue = 1, bool forceUniqueXmlName = true){
    addIntTypeParam<juce::uint32>(paramIndex, name, registerAtHostFlag, loadSaveOptions, value, minValue, maxValue, forceUniqueXmlName);
  }
  
  void addInt64Param(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::int64 *const value, const juce::int64 minValue = 0, const juce::int64 maxValue = 1, bool forceUniqueXmlName = true){
    addIntTypeParam<juce::int64>(paramIndex, name, registerAtHostFlag, loadSaveOptions, value, minValue, maxValue, forceUniqueXmlName);
  }

  void addUint64Param(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::uint64 *const value, const juce::uint64 minValue = 0, const juce::uint64 maxValue = 1, bool forceUniqueXmlName = true){
    addIntTypeParam<juce::uint64>(paramIndex, name, registerAtHostFlag, loadSaveOptions, value, minValue, maxValue, forceUniqueXmlName);
  }

  void addIntParamArray(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, int* const values, int *const size, const int maxSize, const int minValue = (int)(0), const int maxValue = (int)(1), bool saveOnlySizedArrayFlag = true){
    addIntTypeParamArray<int>(paramIndex,name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, minValue, maxValue, saveOnlySizedArrayFlag);
  }

  void addIntParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, int** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const int minValue = (int)(0), const int maxValue = (int)(1), const bool saveOnlySizedMatrixFlag = true){
    addIntTypeParamMatrix<int>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, numRows, numCols, maxRows, maxCols, minValue, maxValue, saveOnlySizedMatrixFlag);
  }

  IntTypeParamArray<int> *getIntParamArray(const int index) const{
    return getIntTypeParamArray<int>(index);
  }

  IntTypeParamMatrix<int> *getIntParamMatrix(const int index) const{
    return getIntTypeParamMatrix<int>(index);
  }

  void addInt8ParamArray(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::int8* const values, int *const size, const int maxSize, const juce::int8 minValue = (juce::int8)(0), const juce::int8 maxValue = (juce::int8)(1), bool saveOnlySizedArrayFlag = true){
    addIntTypeParamArray<juce::int8>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, minValue, maxValue, saveOnlySizedArrayFlag);
  }

  void addInt8ParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::int8** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const juce::int8 minValue = (juce::int8)(0), const juce::int8 maxValue = (juce::int8)(1), const bool saveOnlySizedMatrixFlag = true){
    addIntTypeParamMatrix<juce::int8>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, numRows, numCols, maxRows, maxCols, minValue, maxValue, saveOnlySizedMatrixFlag);
  }
  
  IntTypeParamArray<juce::int8> *getInt8ParamArray(const int index) const{
    return getIntTypeParamArray<juce::int8>(index);
  }

  IntTypeParamMatrix<juce::int8> *getInt8ParamMatrix(const int index) const{
    return getIntTypeParamMatrix<juce::int8>(index);
  }

  void addUint8ParamArray(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::uint8* const values, int *const size, const int maxSize, const juce::uint8 minValue = (juce::uint8)(0), const juce::uint8 maxValue = (juce::uint8)(1), bool saveOnlySizedArrayFlag = true){
    addIntTypeParamArray<juce::uint8>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, minValue, maxValue, saveOnlySizedArrayFlag);
  }

  void addUint8ParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::uint8** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const juce::uint8 minValue = (juce::uint8)(0), const juce::uint8 maxValue = (juce::uint8)(1), const bool saveOnlySizedMatrixFlag = true){
    addIntTypeParamMatrix<juce::uint8>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, numRows, numCols, maxRows, maxCols, minValue, maxValue, saveOnlySizedMatrixFlag);
  }

  IntTypeParamArray<juce::uint8> *getUint8ParamArray(const int index) const{
    return getIntTypeParamArray<juce::uint8>(index);
  }

  IntTypeParamMatrix<juce::uint8> *getUint8ParamMatrix(const int index) const{
    return getIntTypeParamMatrix<juce::uint8>(index);
  }

  void addInt16ParamArray(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::int16* const values, int *const size, const int maxSize, const juce::int16 minValue = (juce::int16)(0), const juce::int16 maxValue = (juce::int16)(1), bool saveOnlySizedArrayFlag = true){
    addIntTypeParamArray<juce::int16>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, minValue, maxValue, saveOnlySizedArrayFlag);
  }

  void addInt16ParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::int16** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const juce::int16 minValue = (juce::int16)(0), const juce::int16 maxValue = (juce::int16)(1), const bool saveOnlySizedMatrixFlag = true){
    addIntTypeParamMatrix<juce::int16>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, numRows, numCols, maxRows, maxCols, minValue, maxValue, saveOnlySizedMatrixFlag);
  }

  IntTypeParamArray<juce::int16> *getInt16ParamArray(const int index) const{
    return getIntTypeParamArray<juce::int16>(index);
  }

  IntTypeParamMatrix<juce::int16> *getInt16ParamMatrix(const int index) const{
    return getIntTypeParamMatrix<juce::int16>(index);
  }

  void addUint16ParamArray(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::uint16* const values, int *const size, const int maxSize, const juce::uint16 minValue = (juce::uint16)(0), const juce::uint16 maxValue = (juce::uint16)(1), bool saveOnlySizedArrayFlag = true){
    addIntTypeParamArray<juce::uint16>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, minValue, maxValue, saveOnlySizedArrayFlag);
  }

  void addUint16ParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::uint16** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const juce::uint16 minValue = (juce::uint16)(0), const juce::uint16 maxValue = (juce::uint16)(1), const bool saveOnlySizedMatrixFlag = true){
    addIntTypeParamMatrix<juce::uint16>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, numRows, numCols, maxRows, maxCols, minValue, maxValue, saveOnlySizedMatrixFlag);
  }

  IntTypeParamArray<juce::uint16> *getUint16ParamArray(const int index) const{
    return getIntTypeParamArray<juce::uint16>(index);
  }

  IntTypeParamMatrix<juce::uint16> *getUint16ParamMatrix(const int index) const{
    return getIntTypeParamMatrix<juce::uint16>(index);
  }

  void addInt32ParamArray(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::int32* const values, int *const size, const int maxSize, const juce::int32 minValue = (juce::int32)(0), const juce::int32 maxValue = (juce::int32)(1), bool saveOnlySizedArrayFlag = true){
    addIntTypeParamArray<juce::int32>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, minValue, maxValue, saveOnlySizedArrayFlag);
  }

  void addInt32ParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::int32** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const juce::int32 minValue = (juce::int32)(0), const juce::int32 maxValue = (juce::int32)(1), const bool saveOnlySizedMatrixFlag = true){
    addIntTypeParamMatrix<juce::int32>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, numRows, numCols, maxRows, maxCols, minValue, maxValue, saveOnlySizedMatrixFlag);
  }

  IntTypeParamArray<juce::int32> *getInt32ParamArray(const int index) const{
    return getIntTypeParamArray<juce::int32>(index);
  }

  IntTypeParamMatrix<juce::int32> *getInt32ParamMatrix(const int index) const{
    return getIntTypeParamMatrix<juce::int32>(index);
  }

  void addUint32ParamArray(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::uint32* const values, int *const size, const int maxSize, const juce::uint32 minValue = (juce::uint32)(0), const juce::uint32 maxValue = (juce::uint32)(1), bool saveOnlySizedArrayFlag = true){
    addIntTypeParamArray<juce::uint32>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, minValue, maxValue, saveOnlySizedArrayFlag);
  }

  void addUint32ParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::uint32** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const juce::uint32 minValue = (juce::uint32)(0), const juce::uint32 maxValue = (juce::uint32)(1), const bool saveOnlySizedMatrixFlag = true){
    addIntTypeParamMatrix<juce::uint32>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, numRows, numCols, maxRows, maxCols, minValue, maxValue, saveOnlySizedMatrixFlag);
  }

  IntTypeParamArray<juce::uint32> *getUint32ParamArray(const int index) const{
    return getIntTypeParamArray<juce::uint32>(index);
  }

  IntTypeParamMatrix<juce::uint32> *getUint32ParamMatrix(const int index) const{
    return getIntTypeParamMatrix<juce::uint32>(index);
  }

  void addInt64ParamArray(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::int64* const values, int *const size, const int maxSize, const juce::int64 minValue = (juce::int64)(0), const juce::int64 maxValue = (juce::int64)(1), bool saveOnlySizedArrayFlag = true){
    addIntTypeParamArray<juce::int64>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, minValue, maxValue, saveOnlySizedArrayFlag);
  }

  void addInt64ParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::int64** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const juce::int64 minValue = (juce::int64)(0), const juce::int64 maxValue = (juce::int64)(1), const bool saveOnlySizedMatrixFlag = true){
    addIntTypeParamMatrix<juce::int64>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, numRows, numCols, maxRows, maxCols, minValue, maxValue, saveOnlySizedMatrixFlag);
  }

  IntTypeParamArray<juce::int64> *getInt64ParamArray(const int index) const{
    return getIntTypeParamArray<juce::int64>(index);
  }

  IntTypeParamMatrix<juce::int64> *getInt64ParamMatrix(const int index) const{
    return getIntTypeParamMatrix<juce::int64>(index);
  }

  void addUint64ParamArray(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::uint64* const values, int *const size, const int maxSize, const juce::uint64 minValue = (juce::uint64)(0), const juce::uint64 maxValue = (juce::uint64)(1), bool saveOnlySizedArrayFlag = true){
    addIntTypeParamArray<juce::uint64>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, minValue, maxValue, saveOnlySizedArrayFlag);
  }

  void addUint64ParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, juce::uint64** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const juce::uint64 minValue = (juce::uint64)(0), const juce::uint64 maxValue = (juce::uint64)(1), const bool saveOnlySizedMatrixFlag = true){
    addIntTypeParamMatrix<juce::uint64>(paramIndex, name, registerAtHostFlag, loadSaveOptions, values, numRows, numCols, maxRows, maxCols, minValue, maxValue, saveOnlySizedMatrixFlag);
  }

  IntTypeParamArray<juce::uint64> *getUint64ParamArray(const int index) const{
    return getIntTypeParamArray<juce::uint64>(index);
  }

  IntTypeParamMatrix<juce::uint64> *getUint64ParamMatrix(const int index) const{
    return getIntTypeParamMatrix<juce::uint64>(index);
  }

  BoolParam *getBoolParam(const int index) const{
    /* Wrong index? */
    if (index<0 || index>=paramList.size()) {jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"Param index out of bounds: "+String(index)+" in ParamGroup with XML name: "+this->getXmlName()); return nullptr;}
    /* You are trying to fetch a parameter with another type... 
        You want to use another method such as:
        getStringParam(), getFloatParam(), getLogParam(), etc */
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
template<class Type>
class TypeParamArray : public ParamGroup{       
  JUCE_DECLARE_NON_COPYABLE(TypeParamArray)

private:  
  int *const size;
  const int maxSize;
  const bool saveOnlySizedArrayFlag;
  const bool saveOnlyNonDefaultValuesFlag;
  const bool updateOnlySizedArrayFlag;

protected:
  Type* const values;  
  const bool registerAtHostFlag;
  const LoadSaveOptions loadSaveOptions;  

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

  /** Returns the value of position i in the array */
  Type getValue(int i) const{
    if (i >= 0 && i<getSize() && i<maxSize)
      return values[i];
    else return 0;
  }
 
  virtual void updateUi(const bool request,const bool applyRecursively){
    for (int i = 0; i<getSize(); i++)
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
          for (int i=0;i<maxSize;i++)
            saveXmlOptionCopy[i]=getParam(i)->getOption(Param::saveToSession);
          break;
        case PRESET:
          for (int i=0;i<maxSize;i++)
            saveXmlOptionCopy[i]=getParam(i)->getOption(Param::saveToPresets);
          break;
        default:
          break;
      }      
    }
    
    if (saveOnlySizedArrayFlag){
      switch(xmlType){
        case SESSION:
          for (int i = getSize(); i<maxSize; i++)
            getParam(i)->setOption(Param::saveToSession,false);
          break;
        case PRESET:
          for (int i = getSize(); i<maxSize; i++)
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
          for (int i=0;i<maxSize;i++)
            getParam(i)->setOption(Param::saveToSession,saveXmlOptionCopy[i]);
          break;
        case PRESET:
          for (int i = getSize(); i<maxSize; i++)
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

  TypeParamArray(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, Type* const values, int *const size, const int maxSize, bool saveOnlySizedArrayFlag = true, bool updateOnlySizedArrayFlag = true) :
  ParamGroup(name),  
  size(size),
  maxSize(maxSize),  
  saveOnlySizedArrayFlag(saveOnlySizedArrayFlag),
  saveOnlyNonDefaultValuesFlag(saveOnlyNonDefaultValuesFlag),
  updateOnlySizedArrayFlag(updateOnlySizedArrayFlag),
  values(values),
  registerAtHostFlag(registerAtHostFlag), 
  loadSaveOptions(loadSaveOptions)  
  {
    //deleted at ~ParamGroup()
    ParamGroup::saveXmlOptionCopy=new bool[maxSize];
  }

  virtual ~TypeParamArray() {
  }
  
};

/** ParamGroup containing an array of StringParams. */
class StringParamArray : public TypeParamArray<String>{
  JUCE_DECLARE_NON_COPYABLE(StringParamArray)

public:
  void initParameters() override{
    for (int i = 0; i<TypeParamArray<String>::getMaxSize(); i++){
      ParamGroup::addStringParam(i, (String)(i), TypeParamArray<String>::registerAtHostFlag, TypeParamArray<String>::loadSaveOptions, &(TypeParamArray<String>::values[i]), false);
    }
  }

  /** Updates the value from its UI value */
  void updateProcessorAndHostFromUi(int i, const String valueArg, UndoManager *const undoManager = nullptr, const bool dontCreateNewUndoTransaction = false) const{
    return ParamGroup::getStringParam(i)->updateProcessorAndHostFromUi(valueArg, undoManager, dontCreateNewUndoTransaction);
  }

  StringParamArray(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, String* const values, int *const size, const int maxSize, bool saveOnlySizedArrayFlag = true) :
    TypeParamArray<String>(name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, saveOnlySizedArrayFlag)
  {
    // Strings cannot be automated! 
    // (They aren't supported at least in VST)
    // Try again setting argument registerAtHostFlag=false
    jassert(TypeParamArray<String>::registerAtHostFlag == false);
  }
};

/** ParamGroup containing an array of FloatTypeParams. */
template<class FloatType, class FloatTypeMap>
class FloatTypeParamArray : public TypeParamArray<FloatType>{
  JUCE_DECLARE_NON_COPYABLE(FloatTypeParamArray)

private:
  const FloatType minValue;
  const FloatType maxValue;

public: 
  void initParameters() override{
    for (int i=0;i<TypeParamArray<FloatType>::getMaxSize();i++){
      ParamGroup::addFloatTypeParam<FloatType, FloatTypeMap>(i, (String)(i), TypeParamArray<FloatType>::registerAtHostFlag, TypeParamArray<FloatType>::loadSaveOptions, &(TypeParamArray<FloatType>::values[i]), minValue, maxValue, false);
    }
  }
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(FloatType minValueArg){
    for (int i = 0; i<TypeParamArray<FloatType>::getMaxSize(); i++)
      ParamGroup::getFloatTypeParam<FloatType,FloatTypeMap>(i)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(FloatType maxValueArg){
    for (int i = 0; i<TypeParamArray<FloatType>::getMaxSize(); i++)
      ParamGroup::getFloatTypeParam<FloatType, FloatTypeMap>(i)->setMax(maxValueArg);
  }
 
  /** Updates the value from its UI value */ 
  void updateProcessorAndHostFromUi(int i, const FloatType valueArg, UndoManager *const undoManager = nullptr, const bool dontCreateNewUndoTransaction = false) const{
    return ParamGroup::getFloatTypeParam<FloatType, FloatTypeMap>(i)->updateProcessorAndHostFromUi(valueArg, undoManager, dontCreateNewUndoTransaction);
  } 
  
  FloatTypeParamArray(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, FloatType* const values, int *const size, const int maxSize, const FloatType minValue = (FloatType)(0), const FloatType maxValue = (FloatType)(1), bool saveOnlySizedArrayFlag = true) :
    TypeParamArray<FloatType>(name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, saveOnlySizedArrayFlag),
    minValue(minValue),
    maxValue(maxValue)
  {} 
};

/** ParamGroup containing an array of IntTypeParams. */
template<class IntType>
class IntTypeParamArray : public TypeParamArray<IntType>{
  JUCE_DECLARE_NON_COPYABLE(IntTypeParamArray)

private:
  const IntType minValue;
  const IntType maxValue;

public:
  void initParameters() override{
    for (int i = 0; i<TypeParamArray<IntType>::getMaxSize(); i++){
      ParamGroup::addIntTypeParam<IntType>(i, (String)(i), TypeParamArray<IntType>::registerAtHostFlag, TypeParamArray<IntType>::loadSaveOptions, &(TypeParamArray<IntType>::values[i]), minValue, maxValue, false);
    }
  }

  /** Sets the minimum range of each parameter in the array */
  void setMin(IntType minValueArg){
    for (int i = 0; i<TypeParamArray<IntType>::getMaxSize(); i++)
      ParamGroup::getIntTypeParam<IntType, IntTypeMap>(i)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(IntType maxValueArg){
    for (int i = 0; i<TypeParamArray<IntType>::getMaxSize(); i++)
      ParamGroup::getIntTypeParam<IntType, IntTypeMap>(i)->setMax(maxValueArg);
  }

  /** Updates the value from its UI value */
  void updateProcessorAndHostFromUi(int i, const IntType valueArg, UndoManager *const undoManager = nullptr, const bool dontCreateNewUndoTransaction = false) const{
    return ParamGroup::getIntTypeParam<IntType, IntTypeMap>(i)->updateProcessorAndHostFromUi(valueArg, undoManager, dontCreateNewUndoTransaction);
  }

  IntTypeParamArray(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, IntType* const values, int *const size, const int maxSize, const IntType minValue = (IntType)(0), const IntType maxValue = (IntType)(1), bool saveOnlySizedArrayFlag = true) :
    TypeParamArray<IntType>(name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, saveOnlySizedArrayFlag),
    minValue(minValue),
    maxValue(maxValue)
  {}
};

/** ParamGroup containing an array of BoolParams. */
class BoolParamArray : public TypeParamArray<bool>{
  JUCE_DECLARE_NON_COPYABLE(BoolParamArray)

public: 
  void initParameters() override{
    for (int i = 0; i<TypeParamArray<bool>::getMaxSize(); i++){
      ParamGroup::addBoolParam(i, (String)(i), TypeParamArray<bool>::registerAtHostFlag, TypeParamArray<bool>::loadSaveOptions, &(TypeParamArray<bool>::values[i]), false);
    }
  }
 
  /** Updates the value from its UI value */ 
  void updateProcessorAndHostFromUi(int i,const bool valueArg, UndoManager *const undoManager=nullptr, const bool dontCreateNewUndoTransaction=false) const{
    return ParamGroup::getBoolParam(i)->updateProcessorAndHostFromUi(valueArg,undoManager,dontCreateNewUndoTransaction);
  }
  
  BoolParamArray(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, bool* const values,int *const size,const int maxSize,bool saveOnlySizedArrayFlag=true):
    TypeParamArray<bool>(name, registerAtHostFlag, loadSaveOptions, values, size, maxSize, saveOnlySizedArrayFlag)
  {} 
};

//-----------------------------------------------------------------------------------

/** ParamGroup base class for a matrix of Params. */
template<class Type>
class TypeParamMatrix : public ParamGroup{
  JUCE_DECLARE_NON_COPYABLE(TypeParamMatrix)

private:  
  int *const numRows;
  int *const numCols;
  const int maxRows;
  const int maxCols;
  bool saveOnlySizedMatrixFlag;
  bool saveOnlyNonDefaultValuesFlag;
  bool updateOnlySizedMatrixFlag;

protected:
  Type** const values;  
  const bool registerAtHostFlag;
  const LoadSaveOptions loadSaveOptions;  
  
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

  /** Returns the value of position i,j in the array */
  Type getValue(int i, int j) const{
    if (i >= 0 && i<getNumRows() && i<getMaxRows()
      && j >= 0 && j<getNumCols() && j<getMaxCols())
      return values[i][j];
    else return 0;
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
          for (int i=0;i<maxRows*maxCols;i++)
            saveXmlOptionCopy[i]=ParamGroup::getParam(i)->getOption(Param::saveToSession);
          break;
        case PRESET:
          for (int i=0;i<maxRows*maxCols;i++)
            saveXmlOptionCopy[i]=ParamGroup::getParam(i)->getOption(Param::saveToPresets);
          break;
        default:
          break;
      }
    }
    
    if (saveOnlySizedMatrixFlag){
      switch(xmlType){
        case SESSION:
          for (int i=0;i<getNumRows();i++){        
            for (int j=getNumCols();j<maxCols;j++){
              getParam(i,j)->setOption(Param::saveToSession,false);
            }        
          }
          break;
        case PRESET:
          for (int i=0;i<getNumRows();i++){        
            for (int j=getNumCols();j<maxCols;j++){
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
  
  TypeParamMatrix(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, Type** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag = true, bool updateOnlySizedMatrixFlag = true) :
  ParamGroup(name),  
  numRows(numRows),
  numCols(numCols),
  maxRows(maxRows),
  maxCols(maxCols),
  saveOnlySizedMatrixFlag(saveOnlySizedMatrixFlag),
  saveOnlyNonDefaultValuesFlag(saveOnlyNonDefaultValuesFlag),
  updateOnlySizedMatrixFlag(updateOnlySizedMatrixFlag),
  values(values),  
  registerAtHostFlag(registerAtHostFlag),
  loadSaveOptions(loadSaveOptions)
  {
    //deleted at ~ParamGroup()
    ParamGroup::saveXmlOptionCopy=new bool[maxCols*maxRows];
  }

  virtual ~TypeParamMatrix() {}
};

/** ParamGroup containing a matrix of StringParams. */
class StringParamMatrix : public TypeParamMatrix<String>{
  JUCE_DECLARE_NON_COPYABLE(StringParamMatrix)

public:
  void initParameters() override{
    for (int i = 0; i<TypeParamMatrix<String>::getMaxRows(); i++){
      for (int j = 0; j<TypeParamMatrix<String>::getMaxCols(); j++){
        ParamGroup::addStringParam(i*TypeParamMatrix<String>::getMaxCols() + j, (String)(i)+":" + (String)j, TypeParamMatrix<String>::registerAtHostFlag, TypeParamMatrix<String>::loadSaveOptions, &(TypeParamMatrix<String>::values[i][j]), false);
      }
    }
  }

  StringParam* getStringParam(int i, int j) const{
    return ParamGroup::getStringParam(i*TypeParamMatrix<String>::getMaxCols() + j);
  }

  /** Updates the value from its UI value */
  void updateProcessorAndHostFromUi(int i, int j, const String valueArg, UndoManager *const undoManager = nullptr, const bool dontCreateNewUndoTransaction = false) const{
    return ParamGroup::getStringParam(i*TypeParamMatrix<String>::getMaxCols() + j)->updateProcessorAndHostFromUi(valueArg, undoManager, dontCreateNewUndoTransaction);
  }

  StringParamMatrix(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, String** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag = true) :
    TypeParamMatrix<String>(name, registerAtHostFlag, loadSaveOptions, values, numRows, numCols, maxRows, maxCols, saveOnlySizedMatrixFlag)
  {
    // Strings cannot be automated! 
    // (They aren't supported at least in VST)
    // Try again setting argument registerAtHostFlag=false
    jassert(TypeParamMatrix<String>::registerAtHostFlag == false);
  }
};

/** ParamGroup containing a matrix of FloatTypeParams. */
template<class FloatType, class FloatTypeMap>
class FloatTypeParamMatrix : public TypeParamMatrix<FloatType>{
  JUCE_DECLARE_NON_COPYABLE(FloatTypeParamMatrix)

private:  
  const FloatType minValue;
  const FloatType maxValue;

public: 
  void initParameters() override{
    for (int i = 0; i<TypeParamMatrix<FloatType>::getMaxRows(); i++){
      for (int j = 0; j<TypeParamMatrix<FloatType>::getMaxCols(); j++){
        ParamGroup::addFloatTypeParam<FloatType,FloatTypeMap>(i*TypeParamMatrix<FloatType>::getMaxCols() + j, (String)(i)+":" + (String)j, TypeParamMatrix<FloatType>::registerAtHostFlag, TypeParamMatrix<FloatType>::loadSaveOptions, &(TypeParamMatrix<FloatType>::values[i][j]), minValue, maxValue, false);
      }
    }
  }

  FloatTypeParam<FloatType, FloatTypeMap>* getFloatTypeParam(int i, int j) const{
    return ParamGroup::getFloatTypeParam<FloatType,FloatTypeMap>(i*TypeParamMatrix<FloatType>::getMaxCols() + j);
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(float minValueArg){
    for (int i = 0; i<TypeParamMatrix<FloatType>::getMaxRows(); i++)
      for (int j = 0; j<TypeParamMatrix<FloatType>::getMaxCols(); j++)
        ParamGroup::getFloatTypeParam<FloatType,FloatTypeMap>(i*TypeParamMatrix<FloatType>::getMaxCols() + j)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(float maxValueArg){
    for (int i = 0; i<TypeParamMatrix<FloatType>::getMaxRows(); i++)
      for (int j = 0; j<TypeParamMatrix<FloatType>::getMaxCols(); j++)
        ParamGroup::getFloatTypeParam<FloatType, FloatTypeMap>(i*TypeParamMatrix<FloatType>::getMaxCols() + j)->setMax(maxValueArg);;
  }
 
  /** Updates the value from its UI value */ 
  void updateProcessorAndHostFromUi(int i,int j,const float valueArg, UndoManager *const undoManager=nullptr, const bool dontCreateNewUndoTransaction=false) const{
    return ParamGroup::getFloatTypeParam<FloatType, FloatTypeMap>(i*TypeParamMatrix<FloatType>::getMaxCols() + j)->updateProcessorAndHostFromUi(valueArg, undoManager, dontCreateNewUndoTransaction);
  } 
  
  FloatTypeParamMatrix(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, FloatType** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const FloatType minValue = (FloatType)(0), const FloatType maxValue = (FloatType)(1), const bool saveOnlySizedMatrixFlag = true) :
  TypeParamMatrix<FloatType>(name,registerAtHostFlag,loadSaveOptions,values,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag),
  minValue(minValue),
  maxValue(maxValue)
  {} 
};

/** ParamGroup containing a matrix of FloatTypeParams. */
template<class IntType>
class IntTypeParamMatrix : public TypeParamMatrix<IntType>{
  JUCE_DECLARE_NON_COPYABLE(IntTypeParamMatrix)

private:
  const IntType minValue;
  const IntType maxValue;

public:
  void initParameters() override{
    for (int i = 0; i<TypeParamMatrix<IntType>::getMaxRows(); i++){
      for (int j = 0; j<TypeParamMatrix<IntType>::getMaxCols(); j++){
        ParamGroup::addIntTypeParam<IntType>(i*TypeParamMatrix<IntType>::getMaxCols() + j, (String)(i)+":" + (String)j, TypeParamMatrix<IntType>::registerAtHostFlag, TypeParamMatrix<IntType>::loadSaveOptions, &(TypeParamMatrix<IntType>::values[i][j]), minValue, maxValue, false);
      }
    }
  }

  IntTypeParam<IntType>* getIntTypeParam(int i, int j) const{
    return ParamGroup::getIntTypeParam<IntType>(i*TypeParamMatrix<IntType>::getMaxCols() + j);
  }  

  /** Sets the minimum range of each parameter in the array */
  void setMin(float minValueArg){
    for (int i = 0; i<TypeParamMatrix<IntType>::getMaxRows(); i++)
      for (int j = 0; j<TypeParamMatrix<IntType>::getMaxCols(); j++)
        ParamGroup::getIntTypeParam<IntType>(i*TypeParamMatrix<IntType>::getMaxCols() + j)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(float maxValueArg){
    for (int i = 0; i<TypeParamMatrix<IntType>::getMaxRows(); i++)
      for (int j = 0; j<TypeParamMatrix<IntType>::getMaxCols(); j++)
        ParamGroup::getIntTypeParam<IntType>(i*TypeParamMatrix<IntType>::getMaxCols() + j)->setMax(maxValueArg);;
  }

  /** Updates the value from its UI value */
  void updateProcessorAndHostFromUi(int i, int j, const float valueArg, UndoManager *const undoManager = nullptr, const bool dontCreateNewUndoTransaction = false) const{
    return ParamGroup::getIntTypeParam<IntType>(i*TypeParamMatrix<IntType>::getMaxCols() + j)->updateProcessorAndHostFromUi(valueArg, undoManager, dontCreateNewUndoTransaction);
  }

  IntTypeParamMatrix(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, IntType** const values, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const IntType minValue = (IntType)(0), const IntType maxValue = (IntType)(1), const bool saveOnlySizedMatrixFlag = true) :
    TypeParamMatrix<IntType>(name, registerAtHostFlag, loadSaveOptions, values, numRows, numCols, maxRows, maxCols, saveOnlySizedMatrixFlag),
    minValue(minValue),
    maxValue(maxValue)
  {}
};

/** ParamGroup containing a matrix of BoolParams. */
class BoolParamMatrix : public TypeParamMatrix<bool>{
  JUCE_DECLARE_NON_COPYABLE(BoolParamMatrix)

public: 
  void initParameters() override{
    for (int i = 0; i<TypeParamMatrix<bool>::getMaxRows(); i++){
      for (int j = 0; j<TypeParamMatrix<bool>::getMaxCols(); j++){
        ParamGroup::addBoolParam(i*TypeParamMatrix<bool>::getMaxCols() + j, (String)(i)+":" + (String)j, TypeParamMatrix<bool>::registerAtHostFlag, TypeParamMatrix<bool>::loadSaveOptions, &(TypeParamMatrix<bool>::values[i][j]), false);
      }
    }
  } 

  BoolParam* getBoolParam(int i,int j) const{
    return ParamGroup::getBoolParam(i*TypeParamMatrix<bool>::getMaxCols() + j);
  }
 
  /** Updates the value from its UI value */ 
  void updateProcessorAndHostFromUi(int i,int j,const bool valueArg, UndoManager *const undoManager=nullptr, const bool dontCreateNewUndoTransaction=false) const{
    return ParamGroup::getBoolParam(i*TypeParamMatrix<bool>::getMaxCols() + j)->updateProcessorAndHostFromUi(valueArg, undoManager, dontCreateNewUndoTransaction);
  } 
  
  BoolParamMatrix(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, bool** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag=true):
    TypeParamMatrix<bool>(name, registerAtHostFlag, loadSaveOptions, values, numRows, numCols, maxRows, maxCols, saveOnlySizedMatrixFlag)
  {} 
};

typedef FloatTypeParamArray<float> FloatParamArray;
typedef FloatTypeParamArray<float, DefaultLogMap > LogParamArray;
typedef FloatTypeParamArray<float, DefaultLogWith0Map > LogWith0ParamArray;
typedef FloatTypeParamArray<float, DefaultLogWithSignMap > LogWithSignParamArray;

typedef FloatTypeParamArray<double> DoubleParamArray;
typedef FloatTypeParamArray<double, DefaultDoubleLogMap > DoubleLogParamArray;
typedef FloatTypeParamArray<double, DefaultDoubleLogWith0Map > DoubleLogWith0ParamArray;
typedef FloatTypeParamArray<double, DefaultDoubleLogWithSignMap > DoubleLogWithSignParamArray;

typedef FloatTypeParamMatrix<float> FloatParamMatrix;
typedef FloatTypeParamMatrix<float, DefaultLogMap > LogParamMatrix;
typedef FloatTypeParamMatrix<float, DefaultLogWith0Map > LogWith0ParamMatrix;
typedef FloatTypeParamMatrix<float, DefaultLogWithSignMap > LogWithSignParamMatrix;

typedef FloatTypeParamMatrix<double> DoubleParamMatrix;
typedef FloatTypeParamMatrix<double, DefaultDoubleLogMap > DoubleLogParamMatrix;
typedef FloatTypeParamMatrix<double, DefaultDoubleLogWith0Map > DoubleLogWith0ParamMatrix;
typedef FloatTypeParamMatrix<double, DefaultDoubleLogWithSignMap > DoubleLogWithSignParamMatrix;

typedef IntTypeParamArray<int> IntParamArray;
typedef IntTypeParamArray<juce::int8> Int8ParamArray;
typedef IntTypeParamArray<juce::uint8> Uint8ParamArray;
typedef IntTypeParamArray<juce::int16> Int16ParamArray;
typedef IntTypeParamArray<juce::uint16> Uint16ParamArray;
typedef IntTypeParamArray<juce::int32> Int32ParamArray;
typedef IntTypeParamArray<juce::uint32> Uint32ParamArray;
typedef IntTypeParamArray<juce::int64> Int64ParamArray;
typedef IntTypeParamArray<juce::uint64> Uint64ParamArray;

typedef IntTypeParamMatrix<int> IntParamMatrix;
typedef IntTypeParamMatrix<juce::int8> Int8ParamMatrix;
typedef IntTypeParamMatrix<juce::uint8> Uint8ParamMatrix;
typedef IntTypeParamMatrix<juce::int16> Int16ParamMatrix;
typedef IntTypeParamMatrix<juce::uint16> Uint16ParamMatrix;
typedef IntTypeParamMatrix<juce::int32> Int32ParamMatrix;
typedef IntTypeParamMatrix<juce::uint32> Uint32ParamMatrix;
typedef IntTypeParamMatrix<juce::int64> Int64ParamMatrix;
typedef IntTypeParamMatrix<juce::uint64> Uint64ParamMatrix;

#endif