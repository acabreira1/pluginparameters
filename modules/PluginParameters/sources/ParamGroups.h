/*
  ==============================================================================

   This file is part of the PluginParameters module
   Copyright 2012-13 by MarC

  ------------------------------------------------------------------------------

   PluginParameters can be redistributed and/or modified under the terms of the GNU 
   General Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   PluginParameters is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE. See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses PluginParameters, commercial licenses 
   are available: visit http://www.rawmaterialsoftware.com/viewtopic.php?f=6&t=11122 
   for more information.

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
  ParamGroup* parentParamGroup;
  
  void setParentParamGroup(ParamGroup *paramGroup){
    parentParamGroup=paramGroup;        
  }

  /** Counter of automated and non automated parameters. When a new paramter is added, 
      they are incremented accordingly. This is used to compute the parameter indexes in 
      the global list of automated or non automated parameters. */
  int numAutomatedParams;
  int numNonAutomatedParams;
  
  int localIndex;

  const String name;      

  String xmlName;

  String xmlFileName;
  
  bool updateUiFlag;  

  /** List of all parameters included in this group */
  OwnedArray<Param > paramList;
  
  /** List of all subgroups of parameters included in this group */
  OwnedArray<ParamGroup > paramGroupList;

  /** List of all Params that were allocated internally with a new command */
  OwnedArray<Param > paramsToUnallocateAtDestructor;

  /** List of all ParamGroups that were allocated internally with a new command */
  OwnedArray<ParamGroup > paramGroupsToUnallocateAtDestructor;    

  /** Pointer to the AudioProcessor class to be able notify parameter changes to the host 
      with update */
  PluginProcessor* pluginProcessor;

protected:    
  /** Returns the pointer to the (Extended)AudioProcessor instance */
  PluginProcessor* getProcessor() const{
    return pluginProcessor;
  }    
  
  /** Sets the number of automated parameters added so far */
  void setNumAutomatedParams(int numAutomatedParamsArg){
    numAutomatedParams=numAutomatedParamsArg;
  }  
  
  /** Sets the number of non automated parameters added so far */
  void setNumNonAutomatedParams(int numNonAutomatedParamsArg){
    numNonAutomatedParams=numNonAutomatedParamsArg;
  }
  
  /** Sets the index with which it is added to its parent ParamGroup 
      (this information is needed to make runAfterParamGroupChange(...)
       more efficient by allowing a switch to tell between ParamGroups) */
  void setLocalIndex(int paramGroupIndex){
    localIndex=paramGroupIndex;
  }
  
  bool *saveXmlFlagCopy;  
  
public:  
  /** Gets the ParamGroup to which this ParamGroup was added */
  ParamGroup* getParentParamGroup() const{
    return parentParamGroup;
  }    

  /** Sets the pointer to the PluginProcessor instance */
  void setPluginProcessor(PluginProcessor *pluginProcessorArg){
    pluginProcessor=pluginProcessorArg;
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
     See loadXml(...) and saveXml(...) for more details. */ 
 String getXmlName() const { return xmlName; }

 /** Sets the tag name under which this ParamGroup is added to its parent ParamGroup 
     See loadXml(...) and saveXml(...) for more details. */ 
 void setXmlName(const String xmlNameArg){
   xmlName=xmlNameArg;
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

  /** Import all its parameters from xml (set to true by default) when the user requests it */
  void setLoadXml(const bool enable, const bool applyRecursively){
    for (int i=0;i<paramList.size();i++)
      paramList[i]->setLoadXml(enable);
      
    if (applyRecursively){
      for (int i=0;i<paramGroupList.size();i++)
        paramGroupList[i]->setLoadXml(enable,true);
    }
  } 
  
  /** Export all its parameters to xml (set to true by default) when the user requests it */
  void setSaveXml(const bool enable, const bool applyRecursively){
    for (int i=0;i<paramList.size();i++)
      paramList[i]->setSaveXml(enable);
    
    if (applyRecursively){  
      for (int i=0;i<paramGroupList.size();i++)
        paramGroupList[i]->setSaveXml(enable,true);
    }
  }
 
  /** Set the updateUi flags of all its parameters to true only in this ParamGroup
      or applyRecursively. */
  virtual void updateUi(const bool enable,const bool applyRecursively){
    for (int i=0;i<paramList.size();i++)
      paramList[i]->updateUi(enable); 
    
    if (applyRecursively){     
      for (int g=0;g<paramGroupList.size();g++)
        paramGroupList[g]->updateUi(enable,true);
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
    if (paramGroupIndex!=paramGroupList.size()) {jassertfalse; return;}
    
    paramGroup->setPluginProcessor(getProcessor());
    paramGroup->setParentParamGroup(this);
    paramGroup->setNumAutomatedParams(numAutomatedParams);
    paramGroup->setNumNonAutomatedParams(numNonAutomatedParams);
    paramGroup->setLocalIndex(paramGroupIndex);            

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
    if (index<0 || index>=paramList.size()) {jassertfalse; return nullptr;}
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
    if (paramIndex!=paramList.size()) {jassertfalse; return;}    

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
    if (index<0 || index>=paramList.size()) {jassertfalse; return nullptr;}
    /* You are trying to fetch a parameter with another type... 
        You want to use another method from the following list: 
        getStringParam(), getFloatParam(), getFloatParam(), getLogParam(), 
        getLogParam(), getIntParam(), getIntParam(), 
        getBoolParam()  */
    jassert(paramList[index]->getType()=="String");   
    return dynamic_cast<StringParam *>(paramList[index]);
  }
  
  void addStringParam(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, String *const value,bool forceUniqueXmlName=true){      
    Param *param;
    if (automationFlag)
      addParam(paramIndex,param=new StringParam(pluginProcessor,name,numAutomatedParams++,automationFlag,loadSaveXmlFlag,value),forceUniqueXmlName);
    else
      addParam(paramIndex,param=new StringParam(pluginProcessor,name,numNonAutomatedParams++,automationFlag,loadSaveXmlFlag,value),forceUniqueXmlName);
    paramsToUnallocateAtDestructor.add(param);
  }    

  void addStringParamArray(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, String* const values,int *const size,const int maxSize,bool saveOnlySizedArrayFlag=true,bool saveOnlyNonDefaultValuesFlag=true);  

  void addStringParamMatrix(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, String** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true);   

  StringParamArray *getStringParamArray(const int index) const;
  StringParamMatrix *getStringParamMatrix(const int index) const;
  
  FloatParam *getFloatParam(const int index) const{
    /* wrong index */
    if (index<0 || index>=paramList.size()) {jassertfalse; return nullptr;}
    /* You are trying to fetch a parameter with another type... 
        You want to use another method from the following list: 
        getStringParam(), getFloatParam(), getFloatParam(), getLogParam(), 
        getLogParam(), getIntParam(), getIntParam(), 
        getBoolParam()  */
    jassert(paramList[index]->getType()=="Float");   
    return dynamic_cast<FloatParam *>(paramList[index]);
  }
  
  void addFloatParam(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType *const value, const PluginParameters_PluginFloatType minValue=(PluginParameters_PluginFloatType)(0),const PluginParameters_PluginFloatType maxValue=(PluginParameters_PluginFloatType)(0),bool forceUniqueXmlName=true){
    Param *param;
    if (automationFlag)
      addParam(paramIndex,param=new FloatParam(pluginProcessor,name,numAutomatedParams++,automationFlag,loadSaveXmlFlag,value,minValue,maxValue),forceUniqueXmlName);
    else
      addParam(paramIndex,param=new FloatParam(pluginProcessor,name,numNonAutomatedParams++,automationFlag,loadSaveXmlFlag,value,minValue,maxValue),forceUniqueXmlName);        
    paramsToUnallocateAtDestructor.add(param);
  }   

  void addFloatParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType* const values,int *const size,const int maxSize,const PluginParameters_PluginFloatType minValue=(PluginParameters_PluginFloatType)(0),const PluginParameters_PluginFloatType maxValue=(PluginParameters_PluginFloatType)(1), bool saveOnlySizedArrayFlag=true, bool saveOnlyNonDefaultValuesFlag=true);

  void addFloatParamMatrix(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols,const PluginParameters_PluginFloatType minValue=(PluginParameters_PluginFloatType)(0),const PluginParameters_PluginFloatType maxValue=(PluginParameters_PluginFloatType)(1), const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true);

  FloatParamArray *getFloatParamArray(const int index) const;
  FloatParamMatrix *getFloatParamMatrix(const int index) const;
  
  LogParam *getLogParam(const int index) const{
    /* wrong index */
    if (index<0 || index>=paramList.size()) {jassertfalse; return nullptr;}
    /* You are trying to fetch a parameter with another type... 
        You want to use another method from the following list: 
        getStringParam(), getFloatParam(), getFloatParam(), getLogParam(), 
        getLogParam(), getIntParam(), getIntParam(), 
        getBoolParam()  */
    jassert(paramList[index]->getType()=="Log");    
    return dynamic_cast<LogParam *>(paramList[index]);
  }
   
   void addLogParam(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType *const value, const PluginParameters_PluginFloatType minValue=(PluginParameters_PluginFloatType)(0.001),const PluginParameters_PluginFloatType maxValue=(PluginParameters_PluginFloatType)(1),const PluginParameters_PluginFloatType factor = (PluginParameters_PluginFloatType)(1),bool forceUniqueXmlName=true){
    Param *param;
    if (automationFlag)
      addParam(paramIndex,param=new LogParam(pluginProcessor,name,numAutomatedParams++,automationFlag,loadSaveXmlFlag,value,minValue,maxValue,factor),forceUniqueXmlName);
    else
      addParam(paramIndex,param=new LogParam(pluginProcessor,name,numNonAutomatedParams++,automationFlag,loadSaveXmlFlag,value,minValue,maxValue,factor),forceUniqueXmlName);        
    paramsToUnallocateAtDestructor.add(param);
  }   

  void addLogParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType* const values,int *const size,const int maxSize,const PluginParameters_PluginFloatType minValue=(PluginParameters_PluginFloatType)(0),const PluginParameters_PluginFloatType maxValue=(PluginParameters_PluginFloatType)(1), const PluginParameters_PluginFloatType factor=(PluginParameters_PluginFloatType)(1), bool saveOnlySizedArrayFlag=true, bool saveOnlyNonDefaultValuesFlag=true);  

  void addLogParamMatrix(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginParameters_PluginFloatType minValue=(PluginParameters_PluginFloatType)(0),const PluginParameters_PluginFloatType maxValue=(PluginParameters_PluginFloatType)(1), const PluginParameters_PluginFloatType factor=(PluginParameters_PluginFloatType)(1), const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true);  
  
  LogParamArray *getLogParamArray(const int index) const;
  LogParamMatrix *getLogParamMatrix(const int index) const;

  LogWith0Param *getLogWith0Param(const int index) const{
    /* wrong index */
    if (index<0 || index>=paramList.size()) {jassertfalse; return nullptr;}
    /* You are trying to fetch a parameter with another type... 
        You want to use another method from the following list: 
        getStringParam(), getFloatParam(), getFloatParam(), getLogParam(), 
        getLogParam(), getIntParam(), getIntParam(), 
        getBoolParam()  */
    jassert(paramList[index]->getType()=="LogWith0");    
    return dynamic_cast<LogWith0Param *>(paramList[index]);
  }  
   
   void addLogWith0Param(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType *const value, const PluginParameters_PluginFloatType minValue=(PluginParameters_PluginFloatType)(0.001),const PluginParameters_PluginFloatType maxValue=(PluginParameters_PluginFloatType)(1),const PluginParameters_PluginFloatType factor = (PluginParameters_PluginFloatType)(1),bool forceUniqueXmlName=true){
    Param *param;
    if (automationFlag)
      addParam(paramIndex,param=new LogWith0Param(pluginProcessor,name,numAutomatedParams++,automationFlag,loadSaveXmlFlag,value,minValue,maxValue,factor),forceUniqueXmlName);
    else
      addParam(paramIndex,param=new LogWith0Param(pluginProcessor,name,numNonAutomatedParams++,automationFlag,loadSaveXmlFlag,value,minValue,maxValue,factor),forceUniqueXmlName);        
    paramsToUnallocateAtDestructor.add(param);
  }

  void addLogWith0ParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType* const values,int *const size,const int maxSize,const PluginParameters_PluginFloatType minValue=(PluginParameters_PluginFloatType)(0),const PluginParameters_PluginFloatType maxValue=(PluginParameters_PluginFloatType)(1), const PluginParameters_PluginFloatType factor=(PluginParameters_PluginFloatType)(1), bool saveOnlySizedArrayFlag=true, bool saveOnlyNonDefaultValuesFlag=true);

  void addLogWith0ParamMatrix(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginParameters_PluginFloatType minValue=(PluginParameters_PluginFloatType)(0),const PluginParameters_PluginFloatType maxValue=(PluginParameters_PluginFloatType)(1), const PluginParameters_PluginFloatType factor=(PluginParameters_PluginFloatType)(1), const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true);
  
  LogWith0ParamArray *getLogWith0ParamArray(const int index) const;
  LogWith0ParamMatrix *getLogWith0ParamMatrix(const int index) const;

  LogWithSignParam *getLogWithSignParam(const int index) const{
    /* wrong index */
    if (index<0 || index>=paramList.size()) {jassertfalse; return nullptr;}
    /* You are trying to fetch a parameter with another type... 
        You want to use another method from the following list: 
        getStringParam(), getFloatParam(), getFloatParam(), getLogParam(), 
        getLogParam(), getIntParam(), getIntParam(), 
        getBoolParam()  */
    jassert(paramList[index]->getType()=="LogWithSign");    
    return dynamic_cast<LogWithSignParam *>(paramList[index]);
  }  
   
   void addLogWithSignParam(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType *const value, const PluginParameters_PluginFloatType minNegativeValue=(PluginParameters_PluginFloatType)(-1),const PluginParameters_PluginFloatType maxPositiveValue=(PluginParameters_PluginFloatType)(1),const PluginParameters_PluginFloatType minAbsValue=(PluginParameters_PluginFloatType)(0.001),const PluginParameters_PluginFloatType factor = (PluginParameters_PluginFloatType)(1),bool forceUniqueXmlName=true){
   Param *param;
   if (automationFlag)
    addParam(paramIndex,param=new LogWithSignParam(pluginProcessor,name,numAutomatedParams++,automationFlag,loadSaveXmlFlag,value,minNegativeValue,maxPositiveValue,minAbsValue,factor),forceUniqueXmlName);
  else
    addParam(paramIndex,param=new LogWithSignParam(pluginProcessor,name,numNonAutomatedParams++,automationFlag,loadSaveXmlFlag,value,minNegativeValue,maxPositiveValue,minAbsValue,factor),forceUniqueXmlName);
  paramsToUnallocateAtDestructor.add(param);
}  

  void addLogWithSignParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType* const values,int *const size,const int maxSize,const PluginParameters_PluginFloatType minValue=(PluginParameters_PluginFloatType)(0),const PluginParameters_PluginFloatType maxValue=(PluginParameters_PluginFloatType)(1), const PluginParameters_PluginFloatType minAbsValue=(PluginParameters_PluginFloatType)(0.001), const PluginParameters_PluginFloatType factor=(PluginParameters_PluginFloatType)(1), bool saveOnlySizedArrayFlag=true, bool saveOnlyNonDefaultValuesFlag=true);

  void addLogWithSignParamMatrix(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginParameters_PluginFloatType minValue=(PluginParameters_PluginFloatType)(0),const PluginParameters_PluginFloatType maxValue=(PluginParameters_PluginFloatType)(1), const PluginParameters_PluginFloatType minAbsValue=(PluginParameters_PluginFloatType)(0.001), const PluginParameters_PluginFloatType factor=(PluginParameters_PluginFloatType)(1), const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true);

  LogWithSignParamArray *getLogWithSignParamArray(const int index) const;
  LogWithSignParamMatrix *getLogWithSignParamMatrix(const int index) const;
  
  IntParam *getIntParam(const int index) const{
    /* wrong index */ 
    if (index<0 || index>=paramList.size()) {jassertfalse; return nullptr;}
    /* You are trying to fetch a parameter with another type... 
        You want to use another method from the following list: 
        getStringParam(), getFloatParam(), getFloatParam(), getLogParam(), 
        getLogParam(), getIntParam(), getIntParam(), 
        getBoolParam()  */
    jassert(paramList[index]->getType()=="Int");    
    return dynamic_cast<IntParam *>(paramList[index]);
  }  
  
  void addIntParam(const int paramIndex,const String &name,const bool automationFlag, const bool loadSaveXmlFlag,PluginParameters_PluginIntType *const value, const PluginParameters_PluginIntType minValue=0,const PluginParameters_PluginIntType maxValue=1,bool forceUniqueXmlName=true){
    Param *param;
    if (automationFlag)
      addParam(paramIndex,param=new IntParam(pluginProcessor,name,numAutomatedParams++,automationFlag,loadSaveXmlFlag,value,minValue,maxValue),forceUniqueXmlName);
    else
      addParam(paramIndex,param=new IntParam(pluginProcessor,name,numNonAutomatedParams++,automationFlag,loadSaveXmlFlag,value,minValue,maxValue),forceUniqueXmlName);
    paramsToUnallocateAtDestructor.add(param);
  }    

  void addIntParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginIntType* const values,int *const size,const int maxSize,const PluginParameters_PluginIntType minValue=(PluginParameters_PluginIntType)(0),const PluginParameters_PluginIntType maxValue=(PluginParameters_PluginIntType)(1), bool saveOnlySizedArrayFlag=true, bool saveOnlyNonDefaultValuesFlag=true);  

  void addIntParamMatrix(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginIntType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols,const PluginParameters_PluginIntType minValue=(PluginParameters_PluginIntType)(0),const PluginParameters_PluginIntType maxValue=(PluginParameters_PluginIntType)(1), const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true);
  
  IntParamArray *getIntParamArray(const int index) const;
  IntParamMatrix *getIntParamMatrix(const int index) const;

  BoolParam *getBoolParam(const int index) const{
    /* wrong index */
    if (index<0 || index>=paramList.size()) {jassertfalse; return nullptr;}
    /* You are trying to fetch a parameter with another type... 
        You want to use another method from the following list: 
        getStringParam(), getFloatParam(), getFloatParam(), getLogParam(), 
        getLogParam(), getIntParam(), getIntParam(), 
        getBoolParam()  */
    jassert(paramList[index]->getType()=="Bool");
    return dynamic_cast<BoolParam *>(paramList[index]);
  }  
  
  void addBoolParam(const int paramIndex,const String &name,const bool automationFlag, const bool loadSaveXmlFlag, bool *const value,bool forceUniqueXmlName=true){
    Param *param;
    if (automationFlag)
      addParam(paramIndex,param=new BoolParam(pluginProcessor,name,numAutomatedParams++,automationFlag,loadSaveXmlFlag,value),forceUniqueXmlName);
    else
      addParam(paramIndex,param=new BoolParam(pluginProcessor,name,numNonAutomatedParams++,automationFlag,loadSaveXmlFlag,value),forceUniqueXmlName);
    paramsToUnallocateAtDestructor.add(param);
  }
  
  void addBoolParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, bool* const values,int *const size,const int maxSize,bool saveOnlySizedArrayFlag=true,bool saveOnlyNonDefaultValuesFlag=true);

  void addBoolParamMatrix(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, bool** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true);

  BoolParamArray *getBoolParamArray(const int index) const;
  BoolParamMatrix *getBoolParamMatrix(const int index) const;    

  /** Stores the parameter values as an XML attribute.
      If createChild is set to true it will create a child XML node 
      (you want to disable it at the root node).*/
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool applyRecursively){
    //create a child XmlElement for this ParamGroup if you are adding xml
    //contents to an already populated XmlElement.

    if (createChild){
      //create a child      
      xml=xml->createNewChildElement(getXmlName());
    }

    for (int i=0;i<getNumParams();i++)
      paramList[i]->saveXml(xml);
      
    if (applyRecursively){
      for (int g=0;g<getNumParamGroups();g++)
        paramGroupList[g]->saveXml(xml,true,true);
    }
  } 
  
  /** Save this ParamGroup to disk as XML. It creates a 
      root tag with the name of this ParamGroup */
  virtual bool saveXmlFile(const File &file,const String &dtdToUse=""){
    XmlElement xml(getName());
    saveXml(&xml,false,true);
    return xml.writeToFile(file,dtdToUse);
  }
  
  /** Delete info (its XML file by default) about this paramGroup from disk. */
  virtual bool removeXmlFile(const File &file){
    if (!file.exists())
      return false;    
                  
    return file.deleteFile();   
  }
  
  /** Preload Xml values from all parameters into xmlValue. It is not 
      advisable to run this on the Processing thread since it may
      take a little while to parse all this parameters from Xml 
      and normalize them. */
  virtual void loadXml(XmlElement *xml, const bool applyRecursively){
    //this child couldn't be found
    if (xml==nullptr)
      return;
          
    Param *param;
    for (int i=0;i<getNumParams();i++){
      param=getParam(i);
      param->loadXml(xml);      
    }      
    
    if (applyRecursively){
      for (int g=0;g<getNumParamGroups();g++){
        XmlElement *childXml=xml->getChildByName(getParamGroup(g)->getXmlName());
        getParamGroup(g)->loadXml(childXml,true);
      }
    }        
  }   
  
  /** Load this paramGroup from disk. If the file doesn't contain
      XML with a tag that matches the name of this ParamGroup, 
      it won't be loaded and it will return false. */
  virtual bool loadXmlFile(const File &file){  
    XmlDocument myDocument (file);
    ScopedPointer <XmlElement> xml(myDocument.getDocumentElement());
    //this file doesn't contain xml with the same tag name it was saved
    if (xml==nullptr || xml->getTagName()!=getName()) { jassertfalse; return false; }          
    loadXml(xml,true);
    return true;
  }
  
  /* Update all parameters from xmlValue after loadXml(...) has been called. 
     This should be considerably faster than loadXml (which loads everything from
     disk into memory) so you can risk to put it in the processing thread. */
  virtual void updateProcessorFromXml(const bool applyRecursively){
    runBeforeParamBatchChange();
    for (int i=0;i<getNumParams();i++){
      getParam(i)->updateProcessorFromXml();
    }
    
    if (applyRecursively){
      for (int g=0;g<getNumParamGroups();g++){
        getParamGroup(g)->updateProcessorFromXml(true);
      }
    }
    runAfterParamBatchChange();        
  }  

  /* Update all parameters from xmlValue after loadXml(...) has been called. 
     This should be considerably faster than loadXml (which loads everything from
     disk into memory) so you can risk to put it in the processing thread. 
     Update for each parameter the Host and the UI accordingly. */
  virtual void updateProcessorHostAndUiFromXml(bool forceRunAfterParamChange, bool forceUpdateUi,const bool applyRecursively){
    runBeforeParamBatchChange();
    for (int i=0;i<getNumParams();i++){
      getParam(i)->updateProcessorHostAndUiFromXml(forceRunAfterParamChange,forceUpdateUi);
    }
    
    if (applyRecursively){
      for (int g=0;g<getNumParamGroups();g++){
        getParamGroup(g)->updateProcessorHostAndUiFromXml(forceRunAfterParamChange,forceUpdateUi,true);
      }
    }
    runAfterParamBatchChange();        
  }  
  
  /** Reset all parameters in this ParamGroup to their defaultValue.
      If applyRecursively=true do the same for the child ParamGroups. 
      This method should be overrided when the ParamGroup contains 
      variables not initialized as parameters.*/
  virtual void updateProcessorFromDefaultValue(const bool applyRecursively){
    runBeforeParamBatchChange();   
    for (int i=0;i<paramList.size();i++)
      paramList[i]->updateProcessorFromDefaultValue();
      
    if (applyRecursively){
      for (int i=0;i<paramGroupList.size();i++)
        paramGroupList[i]->updateProcessorFromDefaultValue(true);
    }
    runAfterParamBatchChange();
  }

  /** Reset all parameters in this ParamGroup to their defaultValue.
      If applyRecursively=true do the same for the child ParamGroups. 
      This method should be overrided when the ParamGroup contains 
      variables not initialized as parameters.
      Update for each parameter the Host and the UI accordingly. */
  virtual void updateProcessorHostAndUiFromDefaultValue(bool forceRunAfterParamChange, bool forceUpdateUi,const bool applyRecursively){
    runBeforeParamBatchChange();   
    for (int i=0;i<paramList.size();i++)
      paramList[i]->updateProcessorHostAndUiFromDefaultValue(forceRunAfterParamChange,forceUpdateUi);
      
    if (applyRecursively){
      for (int i=0;i<paramGroupList.size();i++)
        paramGroupList[i]->updateProcessorHostAndUiFromDefaultValue(forceRunAfterParamChange,forceUpdateUi,true);
    }
    runAfterParamBatchChange();
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
  
  /** Implements what should be executed BEFORE a new set of parameters is loaded in this 
      ParamGroup using for instance updateProcessorHostAndUiFromXml(...) or updateProcessorFromDefaultValue(...)
      E.g. Place here any initialization of variables that are not defined as Parameters. */
  virtual void runBeforeParamBatchChange() {};

  /** Implements what should be executed AFTER a new set of parameters is loaded in this 
      ParamGroup using for instance updateProcessorHostAndUiFromXml(...) or updateProcessorFromDefaultValue(...)
      E.g. Place here any post-processing of variables that are not defined as Parameters. */
  virtual void runAfterParamBatchChange() {};

  /** All parameters and parameter groups must be added in this method */
  virtual void initParameters() = 0;
  
  /** Notify the host about all the parameters in this paramGroup and update the UI. This is 
      useful when you change the parameters of this paramGroup (maybe several times) without
      notifying the Host and the UI (without using update(...)) and at the end of the process 
      you want to update the Host and the UI from all of them at once */
  void updateHost(bool runAfterParamChange,UpdateFromFlags updateFromFlag,const bool applyRecursively){
    for (int paramIndex=0;paramIndex<getNumParams();paramIndex++)
      getParam(paramIndex)->updateHost(runAfterParamChange,updateFromFlag);

    if (applyRecursively){
      for (int i=0;i<getNumParamGroups();i++)
        getParamGroup(i)->updateHost(runAfterParamChange,updateFromFlag,true);
    }
  }
  
  ParamGroup(const String &name):
    parentParamGroup(nullptr),
    numAutomatedParams(0),
    numNonAutomatedParams(0),
    name(name),
    xmlName(name),
    updateUiFlag(false),
    pluginProcessor(nullptr),
    saveXmlFlagCopy(nullptr)
    {
      #if JUCE_DEBUG
      // we should be able to use this name as an XML attribute name
      // this checks whether the attribute name string contains any illegal characters...
      for (String::CharPointerType t (name.getCharPointer()); ! t.isEmpty(); ++t)
          jassert (t.isLetterOrDigit() || *t == '_' || *t == '-' || *t == ':');
      #endif
    }

  ~ParamGroup(){
    paramGroupList.clear(false);
    paramGroupsToUnallocateAtDestructor.clear(true);
    paramList.clear(false);    
    paramsToUnallocateAtDestructor.clear(true);    

    if (saveXmlFlagCopy){
      delete[] saveXmlFlagCopy;
      saveXmlFlagCopy=nullptr;
    }
  }
};

//-----------------------------------------------------------------------------------

/** ParamGroup base class for array of Params. */
class ParamArray : public ParamGroup{       
protected:
  PluginParameters_PluginIntType *const size;    
  const int maxSize;
  const bool automationFlag;
  const bool loadSaveXmlFlag;
  const bool saveOnlySizedArrayFlag;      
  const bool saveOnlyNonDefaultValuesFlag;    
  const bool updateOnlySizedArrayFlag;

public:      
  virtual void initParameters() = 0;
  
  /** Returns the size of the "visible" array */
  PluginParameters_PluginIntType getSize() const{
    return *size;
  }
  
  /** Returns the maximum (allocated) size of the array */
  PluginParameters_PluginIntType getMaxSize() const{
    return maxSize;
  }
  
  /** Returns the minimum range of each parameter in the array */
  const double getMin() const{
    if (*size>0) 
      return getParam(0)->getMin();
    else
      return 0;
  }
  
  /** Returns the maximum range of each parameter in the array */
  const double getMax() const{
    if (*size>0) 
      return getParam(0)->getMax();
    else
      return 0;
  }
 
  virtual void updateUi(const bool enable,const bool applyRecursively){
    for (int i=0;i<*size;i++)
      getParam(i)->updateUi(enable); 
    
    if (applyRecursively){     
      for (int g=0;g<getNumParamGroups();g++)
        getParamGroup(g)->updateUi(enable,true);
    }
  }
    
  virtual void updateProcessorFromDefaultValue(const bool applyRecursively){
    runBeforeParamBatchChange();

    int updateSize;
    if (updateOnlySizedArrayFlag)
      updateSize=*size;
    else
      updateSize=maxSize;

    for (int i=0;i<updateSize;i++)
      getParam(i)->updateProcessorFromDefaultValue();
      
    if (applyRecursively){
      for (int i=0;i<getNumParamGroups();i++)
        getParamGroup(i)->updateProcessorFromDefaultValue(true);
    }

    runAfterParamBatchChange();
  }  

 
  virtual void updateProcessorHostAndUiFromDefaultValue(bool forceRunAfterParamChange, bool forceUpdateUi,const bool applyRecursively){
    runBeforeParamBatchChange();   

    int updateSize;
    if (updateOnlySizedArrayFlag)
      updateSize=*size;
    else
      updateSize=maxSize;

    for (int i=0;i<updateSize;i++)
      getParam(i)->updateProcessorHostAndUiFromDefaultValue(forceRunAfterParamChange,forceUpdateUi);
      
    if (applyRecursively){
      for (int i=0;i<getNumParamGroups();i++)
        getParamGroup(i)->updateProcessorHostAndUiFromDefaultValue(forceRunAfterParamChange,forceUpdateUi,true);
    }
    runAfterParamBatchChange();
  }  

  virtual void updateProcessorFromXml(const bool applyRecursively){
    runBeforeParamBatchChange();

    int updateSize;
    if (updateOnlySizedArrayFlag)
      updateSize=*size;
    else
      updateSize=maxSize;

    for (int i=0;i<updateSize;i++)
      getParam(i)->updateProcessorFromXml();
      
    if (applyRecursively){
      for (int i=0;i<getNumParamGroups();i++)
        getParamGroup(i)->updateProcessorFromXml(true);
    }

    runAfterParamBatchChange();
  }  

 
  virtual void updateProcessorHostAndUiFromXml(bool forceRunAfterParamChange, bool forceUpdateUi,const bool applyRecursively){
    runBeforeParamBatchChange();   

    int updateSize;
    if (updateOnlySizedArrayFlag)
      updateSize=*size;
    else
      updateSize=maxSize;

    for (int i=0;i<updateSize;i++)
      getParam(i)->updateProcessorHostAndUiFromXml(forceRunAfterParamChange,forceUpdateUi);
      
    if (applyRecursively){
      for (int i=0;i<getNumParamGroups();i++)
        getParamGroup(i)->updateProcessorHostAndUiFromXml(forceRunAfterParamChange,forceUpdateUi,true);
    }
    runAfterParamBatchChange();
  }

  ParamArray(const String &name,const bool automationFlag, const bool loadSaveXmlFlag, int *const size, const int maxSize, bool saveOnlySizedArrayFlag=true, bool saveOnlyNonDefaultValuesFlag=true, bool updateOnlySizedArrayFlag=true):
  ParamGroup(name),
  size(size),
  maxSize(maxSize),  
  automationFlag(automationFlag), 
  loadSaveXmlFlag(loadSaveXmlFlag), 
  saveOnlySizedArrayFlag(saveOnlySizedArrayFlag),
  saveOnlyNonDefaultValuesFlag(saveOnlyNonDefaultValuesFlag),
  updateOnlySizedArrayFlag(updateOnlySizedArrayFlag)
  {
    saveXmlFlagCopy=new bool[maxSize];
  }    
  
};

/** ParamGroup containing an array of FloatParams. */
class FloatParamArray : public ParamArray{
private:
  PluginParameters_PluginFloatType* const values;
  PluginParameters_PluginFloatType minValue;
  PluginParameters_PluginFloatType maxValue;

public: 
  void initParameters(){
    for (int i=0;i<ParamArray::getMaxSize();i++){
      ParamGroup::addFloatParam(i,(String)(i),ParamArray::automationFlag,ParamArray::loadSaveXmlFlag,&(values[i]),minValue,maxValue,false);
    }
  }

  /** Returns the value of position i in the array */
  PluginParameters_PluginFloatType getValue(int i) const{
    if (i>=0 && i<*ParamArray::size && i<ParamArray::getMaxSize())
      return values[i];
    else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(PluginParameters_PluginFloatType minValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getFloatParam(i)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(PluginParameters_PluginFloatType maxValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getFloatParam(i)->setMax(maxValueArg);
  }
 
  /** Updates the value from its UI denormalized value */ 
  void updateProcessorAndHostFromUi(int i,const double valueArg) const{
    return ParamGroup::getFloatParam(i)->updateProcessorAndHostFromUi(valueArg);
  } 
  
 /** Stores the parameter values as an XML attribute.
      If createChild is set to true it will create a child XML node 
      (you want to disable it at the root node). 
      This method expands the default one to save only the 
      parameters with value other than defaultValue and/or 
      the parameters of the sized array. */
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool applyRecursively){
    FloatParam *param;
    
    jassert(saveXmlFlagCopy);
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamArray::maxSize;i++){
        saveXmlFlagCopy[i]=getParam(i)->saveXmlIsOn();
      }
    }
    
    if (ParamArray::saveOnlySizedArrayFlag){
      for (int i=*ParamArray::size;i<ParamArray::maxSize;i++){        
        getParam(i)->setSaveXml(false);
      }
    }
    
    if (ParamArray::saveOnlyNonDefaultValuesFlag){
      for (int i=0;i<ParamArray::maxSize;i++){        
        param=getFloatParam(i);
        if (param->getValue()==param->getDefaultValue())
          param->setSaveXml(false);
      }
    }
    
    ParamGroup::saveXml(xml,createChild,applyRecursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamArray::maxSize;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  }
  
  FloatParamArray(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType* const values,int *const size,const int maxSize,const PluginParameters_PluginFloatType minValue=(PluginParameters_PluginFloatType)(0),const PluginParameters_PluginFloatType maxValue=(PluginParameters_PluginFloatType)(1), bool saveOnlySizedArrayFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
  ParamArray(name,automationFlag,loadSaveXmlFlag,size,maxSize,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag),
  values(values),
  minValue(minValue),
  maxValue(maxValue)
  {} 
};

/** ParamGroup containing an array of LogParams. */
class LogParamArray : public ParamArray{
private:
  PluginParameters_PluginFloatType* const values;
  PluginParameters_PluginFloatType minValue;
  PluginParameters_PluginFloatType maxValue;
  const PluginParameters_PluginFloatType factor;
  
public: 
  void initParameters(){
    for (int i=0;i<ParamArray::getMaxSize();i++){
      ParamGroup::addLogParam(i,(String)(i),ParamArray::automationFlag,ParamArray::loadSaveXmlFlag,&(values[i]),minValue,maxValue,factor,false);
    }
  }

  /** Returns the value of position i in the array */
  PluginParameters_PluginFloatType getValue(int i) const{
    if (i>=0 && i<*ParamArray::size && i<ParamArray::getMaxSize())
      return values[i];
    else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(PluginParameters_PluginFloatType minValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getLogParam(i)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(PluginParameters_PluginFloatType maxValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getLogParam(i)->setMax(maxValueArg);
  }
   
  /** Updates the value from its UI denormalized value */ 
  void updateProcessorAndHostFromUi(int i,const double valueArg) const{
    return ParamGroup::getLogParam(i)->updateProcessorAndHostFromUi(valueArg);
  } 
  
  /** Stores the parameter values as an XML attribute.
      If createChild is set to true it will create a child XML node 
      (you want to disable it at the root node). 
      This method expands the default one to save only the 
      parameters with value other than defaultValue and/or 
      the parameters of the sized array. */
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool applyRecursively){
    LogParam *param;
    
    jassert(saveXmlFlagCopy);
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamArray::maxSize;i++){
        saveXmlFlagCopy[i]=getParam(i)->saveXmlIsOn();
      }
    }
    
    if (ParamArray::saveOnlySizedArrayFlag){
      for (int i=*ParamArray::size;i<ParamArray::maxSize;i++){        
        getParam(i)->setSaveXml(false);
      }
    }
    
    if (ParamArray::saveOnlyNonDefaultValuesFlag){
      for (int i=0;i<ParamArray::maxSize;i++){        
        param=getLogParam(i);
        if (param->getValue()==param->getDefaultValue())
          param->setSaveXml(false);
      }
    }
    
    ParamGroup::saveXml(xml,createChild,applyRecursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamArray::maxSize;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  }
  
  LogParamArray(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType* const values,int *const size,const int maxSize,const PluginParameters_PluginFloatType minValue=(PluginParameters_PluginFloatType)(0),const PluginParameters_PluginFloatType maxValue=(PluginParameters_PluginFloatType)(1), const PluginParameters_PluginFloatType factor=(PluginParameters_PluginFloatType)(1), bool saveOnlySizedArrayFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
  ParamArray(name,automationFlag,loadSaveXmlFlag,size,maxSize,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag),
  values(values),
  minValue(minValue),
  maxValue(maxValue),
  factor(factor)
  {} 
};

/** ParamGroup containing an array of LogWith0Params. */
class LogWith0ParamArray : public ParamArray{
private:
  PluginParameters_PluginFloatType* const values;
  const PluginParameters_PluginFloatType factor;
  PluginParameters_PluginFloatType minValue;
  PluginParameters_PluginFloatType maxValue;

public:   
  void initParameters(){
    for (int i=0;i<ParamArray::getMaxSize();i++){
      ParamGroup::addLogWith0Param(i,(String)(i),ParamArray::automationFlag,ParamArray::loadSaveXmlFlag,&(values[i]),minValue,maxValue,factor,false);
    }
  }

  /** Returns the value of position i in the array */
  PluginParameters_PluginFloatType getValue(int i) const{
    if (i>=0 && i<*ParamArray::size && i<ParamArray::getMaxSize())
      return values[i];
    else return 0;
  }    
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(PluginParameters_PluginFloatType minValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getLogWith0Param(i)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(PluginParameters_PluginFloatType maxValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getLogWith0Param(i)->setMax(maxValueArg);
  }
 
  /** Updates the value from its UI denormalized value */ 
  void updateProcessorAndHostFromUi(int i,const double valueArg) const{
    return ParamGroup::getLogWith0Param(i)->updateProcessorAndHostFromUi(valueArg);
  } 
  
  /** Stores the parameter values as an XML attribute.
      If createChild is set to true it will create a child XML node 
      (you want to disable it at the root node). 
      This method expands the default one to save only the 
      parameters with value other than defaultValue and/or 
      the parameters of the sized array. */
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool applyRecursively){
    LogWith0Param *param;
    
    jassert(saveXmlFlagCopy);
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamArray::maxSize;i++){
        saveXmlFlagCopy[i]=getParam(i)->saveXmlIsOn();
      }
    }
    
    if (ParamArray::saveOnlySizedArrayFlag){
      for (int i=*ParamArray::size;i<ParamArray::maxSize;i++){        
        getParam(i)->setSaveXml(false);
      }
    }
    
    if (ParamArray::saveOnlyNonDefaultValuesFlag){
      for (int i=0;i<ParamArray::maxSize;i++){        
        param=getLogWith0Param(i);
        if (param->getValue()==param->getDefaultValue())
          param->setSaveXml(false);
      }
    }
    
    ParamGroup::saveXml(xml,createChild,applyRecursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamArray::maxSize;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  }
  
  LogWith0ParamArray(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType* const values,int *const size,const int maxSize,const PluginParameters_PluginFloatType minValue=(PluginParameters_PluginFloatType)(0),const PluginParameters_PluginFloatType maxValue=(PluginParameters_PluginFloatType)(1), const PluginParameters_PluginFloatType factor=(PluginParameters_PluginFloatType)(1), bool saveOnlySizedArrayFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
  ParamArray(name,automationFlag,loadSaveXmlFlag,size,maxSize,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag),
  values(values),
  minValue(minValue),
  maxValue(maxValue),
  factor(factor)
  {} 
};

/** ParamGroup containing an array of LogWithSignParams. */
class LogWithSignParamArray : public ParamArray{
private:
  PluginParameters_PluginFloatType* const values;
  const PluginParameters_PluginFloatType factor;
  PluginParameters_PluginFloatType minValue;
  PluginParameters_PluginFloatType maxValue;
  PluginParameters_PluginFloatType minAbsValue;

public: 
  void initParameters(){
    for (int i=0;i<ParamArray::getMaxSize();i++){
      ParamGroup::addLogWithSignParam(i,(String)(i),ParamArray::automationFlag,ParamArray::loadSaveXmlFlag,&(values[i]),minValue,maxValue,minAbsValue,factor,false);
    }
  }

  /** Returns the value of position i in the array */
  PluginParameters_PluginFloatType getValue(int i) const{
    if (i>=0 && i<*ParamArray::size && i<ParamArray::getMaxSize())
      return values[i];
    else return 0;
  }
 
  /** Sets the minimum range of each parameter in the array */
  void setMin(PluginParameters_PluginFloatType minValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getLogWithSignParam(i)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(PluginParameters_PluginFloatType maxValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getLogWithSignParam(i)->setMax(maxValueArg);
  }
 
  /** Updates the value from its UI denormalized value */ 
  void updateProcessorAndHostFromUi(int i,const double valueArg) const{
    return ParamGroup::getLogWithSignParam(i)->updateProcessorAndHostFromUi(valueArg);
  } 
  
  /** Stores the parameter values as an XML attribute.
      If createChild is set to true it will create a child XML node 
      (you want to disable it at the root node). 
      This method expands the default one to save only the 
      parameters with value other than defaultValue and/or 
      the parameters of the sized array. */
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool applyRecursively){
    LogWithSignParam *param;
    
    jassert(saveXmlFlagCopy);
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamArray::maxSize;i++){
        saveXmlFlagCopy[i]=getParam(i)->saveXmlIsOn();
      }
    }
    
    if (ParamArray::saveOnlySizedArrayFlag){
      for (int i=*ParamArray::size;i<ParamArray::maxSize;i++){        
        getParam(i)->setSaveXml(false);
      }
    }
    
    if (ParamArray::saveOnlyNonDefaultValuesFlag){
      for (int i=0;i<ParamArray::maxSize;i++){        
        param=getLogWithSignParam(i);
        if (param->getValue()==param->getDefaultValue())
          param->setSaveXml(false);
      }
    }
    
    ParamGroup::saveXml(xml,createChild,applyRecursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamArray::maxSize;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  }
  
  LogWithSignParamArray(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType* const values,int *const size,const int maxSize,const PluginParameters_PluginFloatType minValue=(PluginParameters_PluginFloatType)(0),const PluginParameters_PluginFloatType maxValue=(PluginParameters_PluginFloatType)(1), const PluginParameters_PluginFloatType minAbsValue=(PluginParameters_PluginFloatType)(0.001), const PluginParameters_PluginFloatType factor=(PluginParameters_PluginFloatType)(1), bool saveOnlySizedArrayFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
  ParamArray(name,automationFlag,loadSaveXmlFlag,size,maxSize,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag),
  values(values),
  minValue(minValue),
  maxValue(maxValue),
  minAbsValue(minAbsValue),
  factor(factor)
  {} 
};

/** ParamGroup containing an array of IntParams. */
class IntParamArray : public ParamArray{
private:
  PluginParameters_PluginIntType* const values;
  PluginParameters_PluginIntType minValue;
  PluginParameters_PluginIntType maxValue;

public: 
  void initParameters(){
    for (int i=0;i<ParamArray::getMaxSize();i++){
      ParamGroup::addIntParam(i,(String)(i),ParamArray::automationFlag,ParamArray::loadSaveXmlFlag,&(values[i]),minValue,maxValue,false);
    }
  } 

  /** Returns the value of position i in the array */
  PluginParameters_PluginIntType getValue(int i) const{
    if (i>=0 && i<*ParamArray::size && i<ParamArray::getMaxSize())
      return values[i];
    else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(PluginParameters_PluginIntType minValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getIntParam(i)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(PluginParameters_PluginIntType maxValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getIntParam(i)->setMax(maxValueArg);
  }

  /** Updates the value from its UI denormalized value */ 
  void updateProcessorAndHostFromUi(int i,const int valueArg) const{
    return ParamGroup::getIntParam(i)->updateProcessorAndHostFromUi(valueArg);
  } 
  
  /** Stores the parameter values as an XML attribute.
      If createChild is set to true it will create a child XML node 
      (you want to disable it at the root node). 
      This method expands the default one to save only the 
      parameters with value other than defaultValue. */
  /** Stores the parameter values as an XML attribute.
      If createChild is set to true it will create a child XML node 
      (you want to disable it at the root node). 
      This method expands the default one to save only the 
      parameters with value other than defaultValue and/or 
      the parameters of the sized array. */
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool applyRecursively){
    IntParam *param;
    
    jassert(saveXmlFlagCopy);
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamArray::maxSize;i++){
        saveXmlFlagCopy[i]=getParam(i)->saveXmlIsOn();
      }
    }
    
    if (ParamArray::saveOnlySizedArrayFlag){
      for (int i=*ParamArray::size;i<ParamArray::maxSize;i++){        
        getParam(i)->setSaveXml(false);
      }
    }
    
    if (ParamArray::saveOnlyNonDefaultValuesFlag){
      for (int i=0;i<ParamArray::maxSize;i++){        
        param=getIntParam(i);
        if (param->getValue()==param->getDefaultValue())
          param->setSaveXml(false);
      }
    }
    
    ParamGroup::saveXml(xml,createChild,applyRecursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamArray::maxSize;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  }
  
  IntParamArray(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginIntType* const values,int *const size,const int maxSize,const PluginParameters_PluginIntType minValue=(PluginParameters_PluginIntType)(0),const PluginParameters_PluginIntType maxValue=(PluginParameters_PluginIntType)(1), bool saveOnlySizedArrayFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
  ParamArray(name,automationFlag,loadSaveXmlFlag,size,maxSize,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag),
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
  void initParameters(){
    for (int i=0;i<ParamArray::getMaxSize();i++){
      ParamGroup::addBoolParam(i,(String)(i),ParamArray::automationFlag,ParamArray::loadSaveXmlFlag,&(values[i]),false);
    }
  } 

  /** Returns the value of position i in the array */
  bool getValue(int i) const{
    if (i>=0 && i<*ParamArray::size && i<ParamArray::getMaxSize())
      return values[i];
    else return 0;
  }    
 
  /** Updates the value from its UI denormalized value */ 
  void updateProcessorAndHostFromUi(int i,const bool valueArg) const{
    return ParamGroup::getBoolParam(i)->updateProcessorAndHostFromUi(valueArg);
  } 
  
  /** Stores the parameter values as an XML attribute.
      If createChild is set to true it will create a child XML node 
      (you want to disable it at the root node). 
      This method expands the default one to save only the 
      parameters with value other than defaultValue and/or 
      the parameters of the sized array. */
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool applyRecursively){
    BoolParam *param;
    
    jassert(saveXmlFlagCopy);
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamArray::maxSize;i++){
        saveXmlFlagCopy[i]=getParam(i)->saveXmlIsOn();
      }
    }
    
    if (ParamArray::saveOnlySizedArrayFlag){
      for (int i=*ParamArray::size;i<ParamArray::maxSize;i++){        
        getParam(i)->setSaveXml(false);
      }
    }
    
    if (ParamArray::saveOnlyNonDefaultValuesFlag){
      for (int i=0;i<ParamArray::maxSize;i++){        
        param=getBoolParam(i);
        if (param->getValue()==param->getDefaultValue())
          param->setSaveXml(false);
      }
    }
    
    ParamGroup::saveXml(xml,createChild,applyRecursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamArray::maxSize;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  }
  
  BoolParamArray(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, bool* const values,int *const size,const int maxSize,bool saveOnlySizedArrayFlag=true,bool saveOnlyNonDefaultValuesFlag=true):
  ParamArray(name,automationFlag,loadSaveXmlFlag,size,maxSize,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag),
  values(values)
  {} 
};


/** ParamGroup containing an array of StringParams. */
class StringParamArray : public ParamArray{
private:
  String* const values;

public: 
  void initParameters(){
    for (int i=0;i<ParamArray::getMaxSize();i++){
      ParamGroup::addStringParam(i,(String)(i),ParamArray::automationFlag,ParamArray::loadSaveXmlFlag,&(values[i]),false);
    }
  } 

  /** Returns the value of position i in the array */
  String getValue(int i) const{
    if (i>=0 && i<*ParamArray::size && i<ParamArray::getMaxSize())
      return values[i];
    else return String::empty;
  }    
 
  /** Updates the value from its UI denormalized value */ 
  void updateProcessorAndHostFromUi(int i,const String valueArg) const{
    return ParamGroup::getStringParam(i)->updateProcessorAndHostFromUi(valueArg);
  } 
  
  /** Stores the parameter values as an XML attribute.
      If createChild is set to true it will create a child XML node 
      (you want to disable it at the root node). 
      This method expands the default one to save only the 
      parameters with value other than defaultValue and/or 
      the parameters of the sized array. */
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool applyRecursively){
    StringParam *param;
    
    jassert(saveXmlFlagCopy);
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamArray::maxSize;i++){
        saveXmlFlagCopy[i]=getParam(i)->saveXmlIsOn();
      }
    }
    
    if (ParamArray::saveOnlySizedArrayFlag){
      for (int i=*ParamArray::size;i<ParamArray::maxSize;i++){        
        getParam(i)->setSaveXml(false);
      }
    }
    
    if (ParamArray::saveOnlyNonDefaultValuesFlag){
      for (int i=0;i<ParamArray::maxSize;i++){        
        param=getStringParam(i);
        if (param->getValue()==param->getDefaultValue())
          param->setSaveXml(false);
      }
    }
    
    ParamGroup::saveXml(xml,createChild,applyRecursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamArray::maxSize;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  }
  
  StringParamArray(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, String* const values,int *const size,const int maxSize,bool saveOnlySizedArrayFlag=true,bool saveOnlyNonDefaultValuesFlag=true):
  ParamArray(name,automationFlag,loadSaveXmlFlag,size,maxSize,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag),
  values(values)
  {
    // Strings cannot be automated! 
    // (They aren't supported at least in VST)
    // Try again setting argument automationFlag=false
    jassert(automationFlag==false);
  } 
};


//-----------------------------------------------------------------------------------

/** ParamGroup base class for a matrix of Params. */
class ParamMatrix : public ParamGroup{
protected:
  PluginParameters_PluginIntType *const numRows;
  PluginParameters_PluginIntType *const numCols;
  const int maxRows;
  const int maxCols;      
  const bool automationFlag;  
  const bool loadSaveXmlFlag;  
  bool saveOnlySizedMatrixFlag;
  bool saveOnlyNonDefaultValuesFlag;
  bool updateOnlySizedMatrixFlag;
  
public:    
  virtual void initParameters() = 0;
  
  /** Returns the number of rows of the "visible" array */
  PluginParameters_PluginIntType getNumRows() const{
    return *numRows;
  }
  
  /** Returns the number of columns of the "visible" array */
  PluginParameters_PluginIntType getNumCols() const{
    return *numCols;
  }
  
  /** Returns the maximum (allocated) size of the array */
  PluginParameters_PluginIntType getMaxRows() const{
    return maxRows;
  }
  
  PluginParameters_PluginIntType getMaxCols() const{
    return maxCols;
  }  
  
  /** Returns the minimum range of each parameter in the matrix */
  const double getMin() const{
    if (*numCols>0 && *numRows>0)
      return getParam(0)->getMin();
    else
      return 0;
  }
  
  /** Returns the maximum range of each parameter in the matrix */
  const double getMax() const{
    if (*numCols>0 && *numRows>0)
      return getParam(0)->getMax();
    else
      return 0;
  }	
  
  /** Update the host and the UI about all parameters in this row */
  void updateHostAndUiRow(int row,bool forceRunAfterParamChange,UpdateFromFlags updateFromFlag){
    for (int col=0;col<getNumCols();col++)
      getParam(row*getNumCols()+col)->updateHost(forceRunAfterParamChange,updateFromFlag);
  }
  
  /** Update the host and the UI about all parameters in this column */
  void updateHostAndUiCol(int col,bool forceRunAfterParamChange,UpdateFromFlags updateFromFlag){
    for (int row=0;row<getNumRows();row++)
      getParam(row*getNumCols()+col)->updateHost(forceRunAfterParamChange,updateFromFlag);
  }

  virtual void updateUi(const bool enable,const bool applyRecursively){
    for (int i=0;i<*numRows;i++)
      for (int j=0;j<*numCols;j++)
        getParam(i*maxCols+j)->updateUi(enable);
    
    if (applyRecursively){     
      for (int g=0;g<getNumParamGroups();g++)
        getParamGroup(g)->updateUi(enable,true);
    }
  }

  virtual void updateProcessorFromDefaultValue(const bool applyRecursively){
    runBeforeParamBatchChange();

    int updateRowsSize,updateColsSize;
    if (updateOnlySizedMatrixFlag){
      updateRowsSize=*numRows;
      updateColsSize=*numCols;
    } else {
      updateRowsSize=maxRows;
      updateColsSize=maxCols;
    }

    for (int i=0;i<updateRowsSize;i++)
      for (int j=0;j<updateColsSize;j++)
        getParam(i*maxCols+j)->updateProcessorFromDefaultValue();
      
    if (applyRecursively){
      for (int i=0;i<getNumParamGroups();i++)
        getParamGroup(i)->updateProcessorFromDefaultValue(true);
    }

    runAfterParamBatchChange();
  }

  virtual void updateProcessorHostAndUiFromDefaultValue(bool forceRunAfterParamChange, bool forceUpdateUi,const bool applyRecursively){
    runBeforeParamBatchChange();   

    int updateRowsSize,updateColsSize;
    if (updateOnlySizedMatrixFlag){
      updateRowsSize=*numRows;
      updateColsSize=*numCols;
    } else {
      updateRowsSize=maxRows;
      updateColsSize=maxCols;
    }

    for (int i=0;i<updateRowsSize;i++)
      for (int j=0;j<updateColsSize;j++)
        getParam(i*maxCols+j)->updateProcessorHostAndUiFromDefaultValue(forceRunAfterParamChange,forceUpdateUi);
      
    if (applyRecursively){
      for (int i=0;i<getNumParamGroups();i++)
        getParamGroup(i)->updateProcessorHostAndUiFromDefaultValue(forceRunAfterParamChange,forceUpdateUi,true);
    }

    runAfterParamBatchChange();
  }

  virtual void updateProcessorFromXml(const bool applyRecursively){
    runBeforeParamBatchChange();

    int updateRowsSize,updateColsSize;
    if (updateOnlySizedMatrixFlag){
      updateRowsSize=*numRows;
      updateColsSize=*numCols;
    } else {
      updateRowsSize=maxRows;
      updateColsSize=maxCols;
    }

    for (int i=0;i<updateRowsSize;i++)
      for (int j=0;j<updateColsSize;j++)
        getParam(i*maxCols+j)->updateProcessorFromXml();
      
    if (applyRecursively){
      for (int i=0;i<getNumParamGroups();i++)
        getParamGroup(i)->updateProcessorFromXml(true);
    }

    runAfterParamBatchChange();
  }

  virtual void updateProcessorHostAndUiFromXml(bool forceRunAfterParamChange, bool forceUpdateUi,const bool applyRecursively){
    runBeforeParamBatchChange();   

    int updateRowsSize,updateColsSize;
    if (updateOnlySizedMatrixFlag){
      updateRowsSize=*numRows;
      updateColsSize=*numCols;
    } else {
      updateRowsSize=maxRows;
      updateColsSize=maxCols;
    }

    for (int i=0;i<updateRowsSize;i++)
      for (int j=0;j<updateColsSize;j++)
        getParam(i*maxCols+j)->updateProcessorHostAndUiFromXml(forceRunAfterParamChange,forceUpdateUi);
      
    if (applyRecursively){
      for (int i=0;i<getNumParamGroups();i++)
        getParamGroup(i)->updateProcessorHostAndUiFromXml(forceRunAfterParamChange,forceUpdateUi,true);
    }

    runAfterParamBatchChange();
  }
  
  ParamMatrix(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true, bool updateOnlySizedMatrixFlag=true):
  ParamGroup(name),
  numRows(numRows),
  numCols(numCols),
  maxRows(maxRows),  
  maxCols(maxCols),
  automationFlag(automationFlag),
  loadSaveXmlFlag(loadSaveXmlFlag),
  saveOnlySizedMatrixFlag(saveOnlySizedMatrixFlag),
  saveOnlyNonDefaultValuesFlag(saveOnlyNonDefaultValuesFlag),
  updateOnlySizedMatrixFlag(updateOnlySizedMatrixFlag)
  {
    saveXmlFlagCopy=new bool[maxCols*maxRows];
  } 
};

/** ParamGroup containing a matrix of FloatParams. */
class FloatParamMatrix : public ParamMatrix{
private:
  PluginParameters_PluginFloatType** const values;
  PluginParameters_PluginFloatType minValue;
  PluginParameters_PluginFloatType maxValue;

public: 
  void initParameters(){
    for (int i=0;i<ParamMatrix::getMaxRows();i++){
      for (int j=0;j<ParamMatrix::getMaxCols();j++){
        ParamGroup::addFloatParam(i*ParamMatrix::getMaxCols()+j,(String)(i)+":"+(String)j,ParamMatrix::automationFlag,ParamMatrix::loadSaveXmlFlag,&(values[i][j]),minValue,maxValue,false);        
      }
    }
  }

  /** Returns the value of position i,j in the array */
  PluginParameters_PluginFloatType getValue(int i,int j) const{
    if (i>=0 && i<ParamMatrix::getNumRows() && i<ParamMatrix::getMaxRows()
        && j>=0 && j<ParamMatrix::getNumCols() && j<ParamMatrix::getMaxCols())
      return values[i][j];
    else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(PluginParameters_PluginFloatType minValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getFloatParam(i*ParamMatrix::getMaxCols()+j)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(PluginParameters_PluginFloatType maxValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getFloatParam(i*ParamMatrix::getMaxCols()+j)->setMax(maxValueArg);;      
  }
 
  /** Updates the value from its UI denormalized value */ 
  void updateProcessorAndHostFromUi(int i,int j,const double valueArg) const{
    return ParamGroup::getFloatParam(i*ParamMatrix::getMaxCols()+j)->updateProcessorAndHostFromUi(valueArg);
  } 
  
  /** Stores the parameter values as an XML attribute.
      If createChild is set to true it will create a child XML node 
      (you want to disable it at the root node). 
      This method expands the default one to save only the 
      parameters with value other than defaultValue and/or 
      the parameters of the sized matrix. */
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool applyRecursively){
    FloatParam *param;
    
    jassert(saveXmlFlagCopy);
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamMatrix::maxRows*ParamMatrix::maxCols;i++){
        saveXmlFlagCopy[i]=getParam(i)->saveXmlIsOn();
      }
    }
    
    if (saveOnlySizedMatrixFlag){
      for (int i=0;i<*ParamMatrix::numRows;i++){        
        for (int j=*ParamMatrix::numCols;j<ParamMatrix::maxCols;j++){
          getParam(ParamMatrix::maxCols*i+j)->setSaveXml(false);
        }        
      }
      
      for (int i=*ParamMatrix::numRows;i<ParamMatrix::maxRows;i++){
        for (int j=0;j<maxCols;j++){
          getParam(ParamMatrix::maxCols*i+j)->setSaveXml(false);
        }
      }
    }          
    
    if (ParamMatrix::saveOnlyNonDefaultValuesFlag){
      for (int i=0;i<ParamMatrix::maxRows;i++){
        for (int j=0;j<ParamMatrix::maxCols;j++){    
          param=getFloatParam(ParamMatrix::maxCols*i+j);
          if (param->getValue()==param->getDefaultValue())
            param->setSaveXml(false);
        }
      }
    }
    
    ParamGroup::saveXml(xml,createChild,applyRecursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<maxRows*maxCols;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  }
  
  FloatParamMatrix(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols,const PluginParameters_PluginFloatType minValue=(PluginParameters_PluginFloatType)(0),const PluginParameters_PluginFloatType maxValue=(PluginParameters_PluginFloatType)(1), const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
  ParamMatrix(name,automationFlag,loadSaveXmlFlag,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag),
  values(values),
  minValue(minValue),
  maxValue(maxValue)
  {} 
};

/** ParamGroup containing a matrix of LogParams. */
class LogParamMatrix : public ParamMatrix{
private:
  PluginParameters_PluginFloatType** const values;
  const PluginParameters_PluginFloatType factor;
  PluginParameters_PluginFloatType minValue;
  PluginParameters_PluginFloatType maxValue;
  bool sparseCompressionFlag; 
  PluginParameters_PluginFloatType mostProbableValue;

public: 
  void initParameters(){
    for (int i=0;i<ParamMatrix::getMaxRows();i++){
      for (int j=0;j<ParamMatrix::getMaxCols();j++){
        ParamGroup::addLogParam(i*ParamMatrix::getMaxCols()+j,(String)(i)+":"+(String)j,ParamMatrix::automationFlag,ParamMatrix::loadSaveXmlFlag,&(values[i][j]),minValue,maxValue,factor,false);        
      }
    }
  }

  /** Returns the value of position i,j in the array */
  PluginParameters_PluginFloatType getValue(int i,int j) const{
    if (i>=0 && i<ParamMatrix::getNumRows() && i<ParamMatrix::getMaxRows()
        && j>=0 && j<ParamMatrix::getNumCols() && j<ParamMatrix::getMaxCols())
      return values[i][j];
    else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(PluginParameters_PluginFloatType minValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getLogParam(i*ParamMatrix::getMaxCols()+j)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(PluginParameters_PluginFloatType maxValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getLogParam(i*ParamMatrix::getMaxCols()+j)->setMax(maxValueArg);;      
  }

  /** Updates the value from its UI denormalized value */ 
  void updateProcessorAndHostFromUi(int i,int j,const double valueArg) const{
    return ParamGroup::getLogParam(i*ParamMatrix::getMaxCols()+j)->updateProcessorAndHostFromUi(valueArg);
  } 
  
  /** Stores the parameter values as an XML attribute.
      If createChild is set to true it will create a child XML node 
      (you want to disable it at the root node). 
      This method expands the default one to save only the 
      parameters with value other than defaultValue and/or 
      the parameters of the sized matrix. */
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool applyRecursively){
    LogParam *param;
    
    jassert(saveXmlFlagCopy);
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamMatrix::maxRows*ParamMatrix::maxCols;i++){
        saveXmlFlagCopy[i]=getParam(i)->saveXmlIsOn();
      }
    }
    
    if (saveOnlySizedMatrixFlag){
      for (int i=0;i<*ParamMatrix::numRows;i++){        
        for (int j=*ParamMatrix::numCols;j<ParamMatrix::maxCols;j++){
          getParam(ParamMatrix::maxCols*i+j)->setSaveXml(false);
        }        
      }
      
      for (int i=*ParamMatrix::numRows;i<ParamMatrix::maxRows;i++){
        for (int j=0;j<maxCols;j++){
          getParam(ParamMatrix::maxCols*i+j)->setSaveXml(false);
        }
      }
    }          
    
    if (ParamMatrix::saveOnlyNonDefaultValuesFlag){
      for (int i=0;i<ParamMatrix::maxRows;i++){
        for (int j=0;j<ParamMatrix::maxCols;j++){    
          param=getLogParam(ParamMatrix::maxCols*i+j);
          if (param->getValue()==param->getDefaultValue())
            param->setSaveXml(false);
        }
      }
    }
    
    ParamGroup::saveXml(xml,createChild,applyRecursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<maxRows*maxCols;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  }
  
  LogParamMatrix(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginParameters_PluginFloatType minValue=(PluginParameters_PluginFloatType)(0),const PluginParameters_PluginFloatType maxValue=(PluginParameters_PluginFloatType)(1), const PluginParameters_PluginFloatType factor=(PluginParameters_PluginFloatType)(1), const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
  ParamMatrix(name,automationFlag,loadSaveXmlFlag,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag),
  values(values),
  minValue(minValue),
  maxValue(maxValue),
  factor(factor)
  {} 
};

/** ParamGroup containing a matrix of LogWith0Params. */
class LogWith0ParamMatrix : public ParamMatrix{
private:
  PluginParameters_PluginFloatType** const values;
  const PluginParameters_PluginFloatType factor;
  PluginParameters_PluginFloatType minValue;
  PluginParameters_PluginFloatType maxValue;
  bool sparseCompressionFlag; 
  PluginParameters_PluginFloatType mostProbableValue;

public: 
  void initParameters(){
    for (int i=0;i<ParamMatrix::getMaxRows();i++){
      for (int j=0;j<ParamMatrix::getMaxCols();j++){
        ParamGroup::addLogWith0Param(i*ParamMatrix::getMaxCols()+j,(String)(i)+":"+(String)j,ParamMatrix::automationFlag,ParamMatrix::loadSaveXmlFlag,&(values[i][j]),minValue,maxValue,factor,false);        
      }
    }
  }

  /** Returns the value of position i,j in the array */
  PluginParameters_PluginFloatType getValue(int i,int j) const{
    if (i>=0 && i<ParamMatrix::getNumRows() && i<ParamMatrix::getMaxRows()
        && j>=0 && j<ParamMatrix::getNumCols() && j<ParamMatrix::getMaxCols())
      return values[i][j];
    else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(PluginParameters_PluginFloatType minValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getLogWith0Param(i*ParamMatrix::getMaxCols()+j)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(PluginParameters_PluginFloatType maxValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getLogWith0Param(i*ParamMatrix::getMaxCols()+j)->setMax(maxValueArg);;      
  }

  /** Updates the value from its UI denormalized value */ 
  void updateProcessorAndHostFromUi(int i,int j,const double valueArg) const{
    return ParamGroup::getLogWith0Param(i*ParamMatrix::getMaxCols()+j)->updateProcessorAndHostFromUi(valueArg);
  } 
  
  /** Stores the parameter values as an XML attribute.
      If createChild is set to true it will create a child XML node 
      (you want to disable it at the root node). 
      This method expands the default one to save only the 
      parameters with value other than defaultValue and/or 
      the parameters of the sized matrix. */
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool applyRecursively){
    LogWith0Param *param;
    
    jassert(saveXmlFlagCopy);
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamMatrix::maxRows*ParamMatrix::maxCols;i++){
        saveXmlFlagCopy[i]=getParam(i)->saveXmlIsOn();
      }
    }
    
    if (saveOnlySizedMatrixFlag){
      for (int i=0;i<*ParamMatrix::numRows;i++){        
        for (int j=*ParamMatrix::numCols;j<ParamMatrix::maxCols;j++){
          getParam(ParamMatrix::maxCols*i+j)->setSaveXml(false);
        }        
      }
      
      for (int i=*ParamMatrix::numRows;i<ParamMatrix::maxRows;i++){
        for (int j=0;j<maxCols;j++){
          getParam(ParamMatrix::maxCols*i+j)->setSaveXml(false);
        }
      }
    }          
    
    if (ParamMatrix::saveOnlyNonDefaultValuesFlag){
      for (int i=0;i<ParamMatrix::maxRows;i++){
        for (int j=0;j<ParamMatrix::maxCols;j++){    
          param=getLogWith0Param(ParamMatrix::maxCols*i+j);
          if (param->getValue()==param->getDefaultValue())
            param->setSaveXml(false);
        }
      }
    }
    
    ParamGroup::saveXml(xml,createChild,applyRecursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<maxRows*maxCols;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  }
  
  LogWith0ParamMatrix(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginParameters_PluginFloatType minValue=(PluginParameters_PluginFloatType)(0),const PluginParameters_PluginFloatType maxValue=(PluginParameters_PluginFloatType)(1), const PluginParameters_PluginFloatType factor=(PluginParameters_PluginFloatType)(1), const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
  ParamMatrix(name,automationFlag,loadSaveXmlFlag,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag),
  values(values),
  minValue(minValue),
  maxValue(maxValue),
  factor(factor)
  {} 
};

/** ParamGroup containing a matrix of LogWithSignParams. */
class LogWithSignParamMatrix : public ParamMatrix{
private:
  PluginParameters_PluginFloatType** const values;
  const PluginParameters_PluginFloatType factor;
  PluginParameters_PluginFloatType minValue;
  PluginParameters_PluginFloatType maxValue;
  PluginParameters_PluginFloatType minAbsValue;
  bool sparseCompressionFlag; 
  PluginParameters_PluginFloatType mostProbableValue;

public: 
  void initParameters(){
    for (int i=0;i<ParamMatrix::getMaxRows();i++){
      for (int j=0;j<ParamMatrix::getMaxCols();j++){
        ParamGroup::addLogWithSignParam(i*ParamMatrix::getMaxCols()+j,(String)(i)+":"+(String)j,ParamMatrix::automationFlag,ParamMatrix::loadSaveXmlFlag,&(values[i][j]),minValue,maxValue,minAbsValue,factor,false);        
      }
    }
  }

  /** Returns the value of position i,j in the array */
  PluginParameters_PluginFloatType getValue(int i,int j) const{
    if (i>=0 && i<ParamMatrix::getNumRows() && i<ParamMatrix::getMaxRows()
        && j>=0 && j<ParamMatrix::getNumCols() && j<ParamMatrix::getMaxCols())
      return values[i][j];
    else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(PluginParameters_PluginFloatType minValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getLogWithSignParam(i*ParamMatrix::getMaxCols()+j)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(PluginParameters_PluginFloatType maxValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getLogWithSignParam(i*ParamMatrix::getMaxCols()+j)->setMax(maxValueArg);;      
  }

  /** Updates the value from its UI denormalized value */ 
  void updateProcessorAndHostFromUi(int i,int j,const double valueArg) const{
    return ParamGroup::getLogWithSignParam(i*ParamMatrix::getMaxCols()+j)->updateProcessorAndHostFromUi(valueArg);
  } 
  
  /** Stores the parameter values as an XML attribute.
      If createChild is set to true it will create a child XML node 
      (you want to disable it at the root node). 
      This method expands the default one to save only the 
      parameters with value other than defaultValue and/or 
      the parameters of the sized matrix. */
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool applyRecursively){
    LogWithSignParam *param;
    
    jassert(saveXmlFlagCopy);
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamMatrix::maxRows*ParamMatrix::maxCols;i++){
        saveXmlFlagCopy[i]=getParam(i)->saveXmlIsOn();
      }
    }
    
    if (saveOnlySizedMatrixFlag){
      for (int i=0;i<*ParamMatrix::numRows;i++){        
        for (int j=*ParamMatrix::numCols;j<ParamMatrix::maxCols;j++){
          getParam(ParamMatrix::maxCols*i+j)->setSaveXml(false);
        }        
      }
      
      for (int i=*ParamMatrix::numRows;i<ParamMatrix::maxRows;i++){
        for (int j=0;j<maxCols;j++){
          getParam(ParamMatrix::maxCols*i+j)->setSaveXml(false);
        }
      }
    }          
    
    if (ParamMatrix::saveOnlyNonDefaultValuesFlag){
      for (int i=0;i<ParamMatrix::maxRows;i++){
        for (int j=0;j<ParamMatrix::maxCols;j++){    
          param=getLogWithSignParam(ParamMatrix::maxCols*i+j);
          if (param->getValue()==param->getDefaultValue())
            param->setSaveXml(false);
        }
      }
    }
    
    ParamGroup::saveXml(xml,createChild,applyRecursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<maxRows*maxCols;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  }
  
  LogWithSignParamMatrix(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginParameters_PluginFloatType minValue=(PluginParameters_PluginFloatType)(0),const PluginParameters_PluginFloatType maxValue=(PluginParameters_PluginFloatType)(1), const PluginParameters_PluginFloatType minAbsValue=(PluginParameters_PluginFloatType)(0.001), const PluginParameters_PluginFloatType factor=(PluginParameters_PluginFloatType)(1), const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
  ParamMatrix(name,automationFlag,loadSaveXmlFlag,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag),
  values(values),
  minValue(minValue),
  maxValue(maxValue),
  minAbsValue(minAbsValue),
  factor(factor)
  {} 
};

/** ParamGroup containing a matrix of IntParams. */
class IntParamMatrix : public ParamMatrix{
private:
  PluginParameters_PluginIntType** const values;  
  PluginParameters_PluginIntType minValue;
  PluginParameters_PluginIntType maxValue;

public: 
  void initParameters(){
    for (int i=0;i<ParamMatrix::getMaxRows();i++){
      for (int j=0;j<ParamMatrix::getMaxCols();j++){
        ParamGroup::addIntParam(i*ParamMatrix::getMaxCols()+j,(String)(i)+":"+(String)j,ParamMatrix::automationFlag,ParamMatrix::loadSaveXmlFlag,&(values[i][j]),minValue,maxValue,false);        
      }
    }
  }

  /** Returns the value of position i,j in the array */
  PluginParameters_PluginIntType getValue(int i,int j) const{
    if (i>=0 && i<ParamMatrix::getNumRows() && i<ParamMatrix::getMaxRows()
        && j>=0 && j<ParamMatrix::getNumCols() && j<ParamMatrix::getMaxCols())
      return values[i][j];
    else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(PluginParameters_PluginIntType minValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getIntParam(i*ParamMatrix::getMaxCols()+j)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(PluginParameters_PluginIntType maxValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getIntParam(i*ParamMatrix::getMaxCols()+j)->setMax(maxValueArg);
  }

  /** Updates the value from its UI denormalized value */ 
  void updateProcessorAndHostFromUi(int i,int j,const int valueArg) const{
    return ParamGroup::getIntParam(i*ParamMatrix::getMaxCols()+j)->updateProcessorAndHostFromUi(valueArg);
  } 
  
  /** Stores the parameter values as an XML attribute.
      If createChild is set to true it will create a child XML node 
      (you want to disable it at the root node). 
      This method expands the default one to save only the 
      parameters with value other than defaultValue and/or 
      the parameters of the sized matrix. */
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool applyRecursively){
    IntParam *param;
    
    jassert(saveXmlFlagCopy);
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamMatrix::maxRows*ParamMatrix::maxCols;i++){
        saveXmlFlagCopy[i]=getParam(i)->saveXmlIsOn();
      }
    }
    
    if (saveOnlySizedMatrixFlag){
      for (int i=0;i<*ParamMatrix::numRows;i++){        
        for (int j=*ParamMatrix::numCols;j<ParamMatrix::maxCols;j++){
          getParam(ParamMatrix::maxCols*i+j)->setSaveXml(false);
        }        
      }
      
      for (int i=*ParamMatrix::numRows;i<ParamMatrix::maxRows;i++){
        for (int j=0;j<maxCols;j++){
          getParam(ParamMatrix::maxCols*i+j)->setSaveXml(false);
        }
      }
    }          
    
    if (ParamMatrix::saveOnlyNonDefaultValuesFlag){
      for (int i=0;i<ParamMatrix::maxRows;i++){
        for (int j=0;j<ParamMatrix::maxCols;j++){    
          param=getIntParam(ParamMatrix::maxCols*i+j);
          if (param->getValue()==param->getDefaultValue())
            param->setSaveXml(false);
        }
      }
    }
    
    ParamGroup::saveXml(xml,createChild,applyRecursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<maxRows*maxCols;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  } 
  
  IntParamMatrix(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginIntType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols,const PluginParameters_PluginIntType minValue=(PluginParameters_PluginIntType)(0),const PluginParameters_PluginIntType maxValue=(PluginParameters_PluginIntType)(1), const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
  ParamMatrix(name,automationFlag,loadSaveXmlFlag,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag),
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
  void initParameters(){
    for (int i=0;i<ParamMatrix::getMaxRows();i++){
      for (int j=0;j<ParamMatrix::getMaxCols();j++){
        ParamGroup::addBoolParam(i*ParamMatrix::getMaxCols()+j,(String)(i)+":"+(String)j,ParamMatrix::automationFlag,ParamMatrix::loadSaveXmlFlag,&(values[i][j]),false);
      }
    }
  } 

  /** Returns the value of position i,j in the array */
  bool getValue(int i,int j) const{
    if (i>=0 && i<ParamMatrix::getNumRows() && i<ParamMatrix::getMaxRows()
        && j>=0 && j<ParamMatrix::getNumCols() && j<ParamMatrix::getMaxCols())
      return values[i][j];
    else return 0;
  }   
 
  /** Updates the value from its UI denormalized value */ 
  void updateProcessorAndHostFromUi(int i,int j,const bool valueArg) const{
    return ParamGroup::getBoolParam(i*ParamMatrix::getMaxCols()+j)->updateProcessorAndHostFromUi(valueArg);
  } 
  
  /** Stores the parameter values as an XML attribute.
      If createChild is set to true it will create a child XML node 
      (you want to disable it at the root node). 
      This method expands the default one to save only the 
      parameters with value other than defaultValue and/or 
      the parameters of the sized matrix. */
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool applyRecursively){
    BoolParam *param;
    
    jassert(saveXmlFlagCopy);
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamMatrix::maxRows*ParamMatrix::maxCols;i++){
        saveXmlFlagCopy[i]=getParam(i)->saveXmlIsOn();
      }
    }
    
    if (saveOnlySizedMatrixFlag){
      for (int i=0;i<*ParamMatrix::numRows;i++){        
        for (int j=*ParamMatrix::numCols;j<ParamMatrix::maxCols;j++){
          getParam(ParamMatrix::maxCols*i+j)->setSaveXml(false);
        }        
      }
      
      for (int i=*ParamMatrix::numRows;i<ParamMatrix::maxRows;i++){
        for (int j=0;j<maxCols;j++){
          getParam(ParamMatrix::maxCols*i+j)->setSaveXml(false);
        }
      }
    }          
    
    if (ParamMatrix::saveOnlyNonDefaultValuesFlag){
      for (int i=0;i<ParamMatrix::maxRows;i++){
        for (int j=0;j<ParamMatrix::maxCols;j++){    
          param=getBoolParam(ParamMatrix::maxCols*i+j);
          if (param->getValue()==param->getDefaultValue())
            param->setSaveXml(false);
        }
      }
    }
    
    ParamGroup::saveXml(xml,createChild,applyRecursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<maxRows*maxCols;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  } 
  
  BoolParamMatrix(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, bool** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
  ParamMatrix(name,automationFlag,loadSaveXmlFlag,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag),  
  values(values)
  {} 
};

/** ParamGroup containing a matrix of StringParams. */
class StringParamMatrix : public ParamMatrix{
private:
  String** const values;

public: 
  void initParameters(){
    for (int i=0;i<ParamMatrix::getMaxRows();i++){
      for (int j=0;j<ParamMatrix::getMaxCols();j++){
        ParamGroup::addStringParam(i*ParamMatrix::getMaxCols()+j,(String)(i)+":"+(String)j,ParamMatrix::automationFlag,ParamMatrix::loadSaveXmlFlag,&(values[i][j]),false);
      }
    }
  } 

  /** Returns the value of position i,j in the array */
  String getValue(int i,int j) const{
    if (i>=0 && i<ParamMatrix::getNumRows() && i<ParamMatrix::getMaxRows()
        && j>=0 && j<ParamMatrix::getNumCols() && j<ParamMatrix::getMaxCols())
      return values[i][j];
    else return String::empty;
  }   
 
  /** Updates the value from its UI denormalized value */ 
  void updateProcessorAndHostFromUi(int i,int j,const String valueArg) const{
    return ParamGroup::getStringParam(i*ParamMatrix::getMaxCols()+j)->updateProcessorAndHostFromUi(valueArg);
  } 
  
  /** Stores the parameter values as an XML attribute.
      If createChild is set to true it will create a child XML node 
      (you want to disable it at the root node). 
      This method expands the default one to save only the 
      parameters with value other than defaultValue and/or 
      the parameters of the sized matrix. */
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool applyRecursively){
    StringParam *param;
    
    jassert(saveXmlFlagCopy);
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamMatrix::maxRows*ParamMatrix::maxCols;i++){
        saveXmlFlagCopy[i]=getParam(i)->saveXmlIsOn();
      }
    }
    
    if (saveOnlySizedMatrixFlag){
      for (int i=0;i<*ParamMatrix::numRows;i++){        
        for (int j=*ParamMatrix::numCols;j<ParamMatrix::maxCols;j++){
          getParam(ParamMatrix::maxCols*i+j)->setSaveXml(false);
        }        
      }
      
      for (int i=*ParamMatrix::numRows;i<ParamMatrix::maxRows;i++){
        for (int j=0;j<maxCols;j++){
          getParam(ParamMatrix::maxCols*i+j)->setSaveXml(false);
        }
      }
    }          
    
    if (ParamMatrix::saveOnlyNonDefaultValuesFlag){
      for (int i=0;i<ParamMatrix::maxRows;i++){
        for (int j=0;j<ParamMatrix::maxCols;j++){    
          param=getStringParam(ParamMatrix::maxCols*i+j);
          if (param->getValue()==param->getDefaultValue())
            param->setSaveXml(false);
        }
      }
    }
    
    ParamGroup::saveXml(xml,createChild,applyRecursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<maxRows*maxCols;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  } 
  
  StringParamMatrix(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, String** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
  ParamMatrix(name,automationFlag,loadSaveXmlFlag,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag),  
  values(values)
  {
    // Strings cannot be automated! 
    // (They aren't supported at least in VST)
    // Try again setting argument automationFlag=false
    jassert(automationFlag==false);
  } 
};
#endif