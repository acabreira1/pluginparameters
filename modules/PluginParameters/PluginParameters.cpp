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

#include "PluginParameters.h"

using namespace PluginParameters;

/*
Only methods who implicate a circular reference and therefore
can't be written in the header file are placed here.
*/

//protected

void Param::updateHostFromUi(PluginParameters_HostFloatType newHostValue){
  if (!pluginProcessor) return;  
  updateFromFlag=UPDATE_FROM_UI;
  pluginProcessor->updateHostAndUi(this,newHostValue,true,false); 
  resetUpdateFromFlag();
}

//public

void Param::updateUi(const bool requestFlag){
  if (!pluginProcessor) return;
  updateUiFlag=requestFlag;
}

void Param::updateProcessorHostAndUiFromXml(bool forceRunAfterParamChange,bool forceUpdateUi){      
  if (!pluginProcessor) return;
  if (loadXmlFlag){
    if (updateFromLoadedXml()){
      updateFromFlag=UPDATE_FROM_XML;
      pluginProcessor->updateHostAndUi(this,xmlHostValue);    
      resetUpdateFromFlag();
    } else if (forceRunAfterParamChange){
      updateFromFlag=UPDATE_FROM_XML;
      pluginProcessor->updateHostAndUi(this,xmlHostValue,true,forceUpdateUi);    
      resetUpdateFromFlag();
    } else if (forceUpdateUi){
      updateUi(true);
    }   
	}
}

void Param::updateProcessorHostAndUi(PluginParameters_HostFloatType newHostValue, UpdateFromFlags updateFromFlagArg){    
  if (hostSet(newHostValue)){  
    if (!pluginProcessor) return;
    updateFromFlag=updateFromFlagArg;     
    pluginProcessor->updateHostAndUi(this,newHostValue);    
    resetUpdateFromFlag();  
  }  
}

void Param::updateHostAndUi(bool runAfterParamChange, UpdateFromFlags updateFromFlagArg){      
  if (!pluginProcessor) return;
  updateFromFlag=updateFromFlagArg;

  pluginProcessor->updateHostAndUi(this,hostGet(),runAfterParamChange);    
  resetUpdateFromFlag();
}

void Param::updateHost(bool runAfterParamChange, UpdateFromFlags updateFromFlagArg){      
  if (!pluginProcessor) return;
  updateFromFlag=updateFromFlagArg;

  pluginProcessor->updateHostAndUi(this,hostGet(),runAfterParamChange,false);    
  resetUpdateFromFlag();
}

void StringParam::updateProcessorAndHostFromUi(const String valueArg) {    
  if (*value!=valueArg){
    *value=valueArg;
    Param::updateHostFromUi((PluginParameters_HostFloatType)(0.f));    
    //this parameter can't be automated (maped as a PluginParameters_HostFloatType)
    //so hostSet doesn't do anything, we must update *value manually
    //and notify the host always with 0    
  }     
}

void FloatParam::updateProcessorAndHostFromUi(const double valueArg){  
  if (maxValue==minValue)
    Param::updateHostFromUi((PluginParameters_HostFloatType)(0.f));
  PluginParameters_HostFloatType newHostValue=(PluginParameters_HostFloatType)(valueArg-minValue)/(maxValue-minValue);
  if (newHostValue<0)
    newHostValue=(PluginParameters_HostFloatType)(0.f);
  else if (newHostValue>1)
    newHostValue=(PluginParameters_HostFloatType)(1.f);
    
  if (hostSet(newHostValue))
    Param::updateHostFromUi(newHostValue);  
}

void LogParam::updateProcessorAndHostFromUi(const double valueArg){
  
  if (maxLogValue==minLogValue)
    Param::updateHostFromUi((PluginParameters_HostFloatType)(0.f));
  PluginParameters_HostFloatType newHostValue=(PluginParameters_HostFloatType)(valueArg-minLogValue)/(maxLogValue-minLogValue);
  if (newHostValue<0)
    newHostValue=(PluginParameters_HostFloatType)(0.f);
  else if (newHostValue>1)
    newHostValue=(PluginParameters_HostFloatType)(1.f);
  
  if (hostSet(newHostValue))
    Param::updateHostFromUi(newHostValue);  
}

void LogWith0Param::updateProcessorAndHostFromUi(const double valueArg){  
  if (maxLogValue==minLogValue){ //do not let idiots make this crash
    if (valueArg>0)
      Param::updateHostFromUi((PluginParameters_HostFloatType)(1.f)); //stupid question, stupid answer
    else
      Param::updateHostFromUi((PluginParameters_HostFloatType)(0.f)); //stupid question, stupid answer
  }
  PluginParameters_HostFloatType newHostValue; 
  //using the host parameter scale of [0,1]
  //store positive log value above 0.05 
  //all values in the range of [0,minLogValue] will be stored as 0
  //at 0 (a margin of 0.05 should be safe to avoid confusing 0 with the former)
  
  //[minLogValue-0.05,minLogValue] represent -inf in the UI
  if (valueArg>=minLogValue){      
    newHostValue=(PluginParameters_HostFloatType)(0.05+(valueArg-minLogValue)*0.95/(maxLogValue-minLogValue));
  } else {
    newHostValue=(PluginParameters_HostFloatType)(0);
  }           
  if (newHostValue<0)
    newHostValue=(PluginParameters_HostFloatType)(0.f);
  else if (newHostValue>1)
    newHostValue=(PluginParameters_HostFloatType)(1.f);
  
  if (hostSet(newHostValue))
    Param::updateHostFromUi(newHostValue);  
}

void LogWithSignParam::updateProcessorAndHostFromUi(const double valueArg){
  if (maxPosLogValue==minAbsLogValue || maxNegLogValue==minAbsLogValue){ //do not let idiots make this crash
      if (valueArg>0)
        Param::updateHostFromUi((PluginParameters_HostFloatType)(1.f)); //stupid question, stupid answer
      else
        Param::updateHostFromUi((PluginParameters_HostFloatType)(0.f)); //stupid question, stupid answer
    }  
  PluginParameters_HostFloatType newHostValue; 
  //using the host parameter scale of [0,1]
  //store positive log value above 0.55 and negative log values below 0.45
  //all values in the range of [-minLogValue,minLogValue] will be stored as 0
  //at 0.5 (a margin of 0.05 should be safe to avoid confusing 0 with the former)  
  
  //in the UI we show a positive range of [0.05,0.05+maxPosLogValue-minAbsLogValue]
  //and a negative range of [-(0.05+maxNegLogValue-minAbsLogValue),-0.05]
  //(-0.05,0.05) represents -inf     
  if (valueArg>=0.05){        
    newHostValue=(PluginParameters_HostFloatType)(centerValue+0.05+(valueArg-0.05)*(1-centerValue-0.05)/(maxPosLogValue-minAbsLogValue));
  } else if (valueArg<=-0.05){
    newHostValue=(PluginParameters_HostFloatType)(centerValue-0.05+(valueArg+0.05)*(centerValue-0.05)/(maxNegLogValue-minAbsLogValue));
  } else {
    newHostValue=centerValue;
  }       
  if (newHostValue<0)
    newHostValue=(PluginParameters_HostFloatType)(0.f);
  else if (newHostValue>1)
    newHostValue=(PluginParameters_HostFloatType)(1.f);
  
  if (hostSet(newHostValue))
    Param::updateHostFromUi(newHostValue);  
}

void IntParam::updateProcessorAndHostFromUi(const int valueArg){  
  if (maxValue==minValue)
    Param::updateHostFromUi((PluginParameters_HostFloatType)(0.f));
  PluginParameters_HostFloatType newHostValue=(PluginParameters_HostFloatType)(valueArg-minValue)/(maxValue-minValue);
  if (newHostValue<0)
    newHostValue=(PluginParameters_HostFloatType)(0.f);
  else if (newHostValue>1)
    newHostValue=(PluginParameters_HostFloatType)(1.f);
  
  if (hostSet(newHostValue))
    Param::updateHostFromUi(newHostValue);  
}

void BoolParam::updateProcessorAndHostFromUi(const bool valueArg){  
  PluginParameters_HostFloatType newHostValue=(valueArg)?(PluginParameters_HostFloatType)(1.f):(PluginParameters_HostFloatType)(0.f);
  
  if (hostSet(newHostValue))
    Param::updateHostFromUi(newHostValue);  
}

void ParamGroup::addStringParamArray(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, String* const values,int *const size,const int maxSize,bool saveOnlySizedArrayFlag,bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new StringParamArray(name,automationFlag,loadSaveXmlFlag,values,size,maxSize,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addStringParamMatrix(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, String** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new StringParamMatrix(name,automationFlag,loadSaveXmlFlag,values,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

StringParamArray *ParamGroup::getStringParamArray(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch an array with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="String");   
  /* Double check this ParamGroup. This might not be a ParamArray */
  return dynamic_cast<StringParamArray *>(paramGroupList[index]);
}

StringParamMatrix *ParamGroup::getStringParamMatrix(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch a matrix with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="String");
  /* Double check this ParamGroup. This might not be a ParamMatrix */   
  return dynamic_cast<StringParamMatrix *>(paramGroupList[index]);
}

void ParamGroup::addFloatParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType* const values,int *const size,const int maxSize,const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, bool saveOnlySizedArrayFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new FloatParamArray(name,automationFlag,loadSaveXmlFlag,values,size,maxSize,minValue,maxValue,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addFloatParamMatrix(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols,const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, const bool saveOnlySizedMatrixFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new FloatParamMatrix(name,automationFlag,loadSaveXmlFlag,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

FloatParamArray *ParamGroup::getFloatParamArray(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch an array with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="Float");   
  /* Double check this ParamGroup. This might not be a ParamArray */
  return dynamic_cast<FloatParamArray *>(paramGroupList[index]);
}

FloatParamMatrix *ParamGroup::getFloatParamMatrix(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch a matrix with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="Float");   
  /* Double check this ParamGroup. This might not be a ParamMatrix */
  return dynamic_cast<FloatParamMatrix *>(paramGroupList[index]);
}


void ParamGroup::addLogParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType* const values,int *const size,const int maxSize,const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, const PluginParameters_PluginFloatType factor, bool saveOnlySizedArrayFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogParamArray(name,automationFlag,loadSaveXmlFlag,values,size,maxSize,minValue,maxValue,factor,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addLogParamMatrix(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, const PluginParameters_PluginFloatType factor, const bool saveOnlySizedMatrixFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogParamMatrix(name,automationFlag,loadSaveXmlFlag,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,factor,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

LogParamArray *ParamGroup::getLogParamArray(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch an array with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="Log");   
  /* Double check this ParamGroup. This might not be a ParamArray */
  return dynamic_cast<LogParamArray *>(paramGroupList[index]);
}

LogParamMatrix *ParamGroup::getLogParamMatrix(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch a matrix with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="Log");   
  /* Double check this ParamGroup. This might not be a ParamMatrix */
  return dynamic_cast<LogParamMatrix *>(paramGroupList[index]);
}

void ParamGroup::addLogWith0ParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType* const values,int *const size,const int maxSize,const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, const PluginParameters_PluginFloatType factor, bool saveOnlySizedArrayFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogWith0ParamArray(name,automationFlag,loadSaveXmlFlag,values,size,maxSize,minValue,maxValue,factor,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addLogWith0ParamMatrix(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, const PluginParameters_PluginFloatType factor, const bool saveOnlySizedMatrixFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogWith0ParamMatrix(name,automationFlag,loadSaveXmlFlag,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,factor,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

LogWith0ParamArray *ParamGroup::getLogWith0ParamArray(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch an array with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="LogWith0");   
  /* Double check this ParamGroup. This might not be a ParamArray */
  return dynamic_cast<LogWith0ParamArray *>(paramGroupList[index]);
}

LogWith0ParamMatrix *ParamGroup::getLogWith0ParamMatrix(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch a matrix with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="LogWith0");   
  /* Double check this ParamGroup. This might not be a ParamMatrix */
  return dynamic_cast<LogWith0ParamMatrix *>(paramGroupList[index]);
}


void ParamGroup::addLogWithSignParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType* const values,int *const size,const int maxSize,const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, const PluginParameters_PluginFloatType minAbsValue, const PluginParameters_PluginFloatType factor, bool saveOnlySizedArrayFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogWithSignParamArray(name,automationFlag,loadSaveXmlFlag,values,size,maxSize,minValue,maxValue,minAbsValue,factor,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addLogWithSignParamMatrix(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, const PluginParameters_PluginFloatType minAbsValue, const PluginParameters_PluginFloatType factor, const bool saveOnlySizedMatrixFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogWithSignParamMatrix(name,automationFlag,loadSaveXmlFlag,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,minAbsValue,factor,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

LogWithSignParamArray *ParamGroup::getLogWithSignParamArray(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch an array with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="LogWithSign");   
  /* Double check this ParamGroup. This might not be a ParamArray */
  return dynamic_cast<LogWithSignParamArray *>(paramGroupList[index]);
}

LogWithSignParamMatrix *ParamGroup::getLogWithSignParamMatrix(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch a matrix with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="LogWithSign");   
  /* Double check this ParamGroup. This might not be a ParamMatrix */
  return dynamic_cast<LogWithSignParamMatrix *>(paramGroupList[index]);
}

void ParamGroup::addIntParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginIntType* const values,int *const size,const int maxSize,const PluginParameters_PluginIntType minValue,const PluginParameters_PluginIntType maxValue, bool saveOnlySizedArrayFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new IntParamArray(name,automationFlag,loadSaveXmlFlag,values,size,maxSize,minValue,maxValue,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addIntParamMatrix(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginIntType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols,const PluginParameters_PluginIntType minValue,const PluginParameters_PluginIntType maxValue, const bool saveOnlySizedMatrixFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new IntParamMatrix(name,automationFlag,loadSaveXmlFlag,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

IntParamArray *ParamGroup::getIntParamArray(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch an array with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="Int");   
  /* Double check this ParamGroup. This might not be a ParamArray */
  return dynamic_cast<IntParamArray *>(paramGroupList[index]);
}

IntParamMatrix *ParamGroup::getIntParamMatrix(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch a matrix with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="Int");
  /* Double check this ParamGroup. This might not be a ParamMatrix */
  return dynamic_cast<IntParamMatrix *>(paramGroupList[index]);
}

void ParamGroup::addBoolParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, bool* const values,int *const size,const int maxSize,bool saveOnlySizedArrayFlag,bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new BoolParamArray(name,automationFlag,loadSaveXmlFlag,values,size,maxSize,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addBoolParamMatrix(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, bool** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new BoolParamMatrix(name,automationFlag,loadSaveXmlFlag,values,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

BoolParamArray *ParamGroup::getBoolParamArray(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch an array with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="Bool");   
  /* Double check this ParamGroup. This might not be a ParamArray */
  return dynamic_cast<BoolParamArray *>(paramGroupList[index]);
}

BoolParamMatrix *ParamGroup::getBoolParamMatrix(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch a matrix with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="Bool");   
  /* Double check this ParamGroup. This might not be a ParamMatrix */
  return dynamic_cast<BoolParamMatrix *>(paramGroupList[index]);
}