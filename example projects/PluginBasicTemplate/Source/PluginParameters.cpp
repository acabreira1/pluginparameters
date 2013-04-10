/*
  ==============================================================================

   This file is part of the PluginParameters library
   Copyright 2012-13 by MarC

  ------------------------------------------------------------------------------

   PluginParameters can be redistributed and/or modified under the terms of the GNU 
   General Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   PluginParameters is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses PluginParameters, commercial licenses 
   are available: visit LINK for more information.

  ==============================================================================
*/

#include "PluginParameters.h"

/*
Only methods who implicate a circular reference and therefore
can't be written in the header file are placed here.
*/

//protected

void Param::updateHostFromUi(HostFloatType newHostValue){
  if (!pluginProcessor) return;  
  updateFromFlag=UPDATE_FROM_UI;
  pluginProcessor->updateHostAndUi(this,newHostValue,true,false); 
  resetUpdateInfoFlags();
}

//public

void Param::requestUpdateUi(const bool enable){
  if (!pluginProcessor) return;
  updateUiFlag=enable;
}

void Param::updateProcessorHostAndUiFromXml(bool forceRunAfterChange,bool forceUpdateUi){      
  if (!pluginProcessor) return;
  if (loadXmlIsOn()){
    if (writeXmlValue()){
      updateFromFlag=UPDATE_FROM_XML;
      pluginProcessor->updateHostAndUi(this,xmlHostValue);    
      resetUpdateInfoFlags();
    } else if (forceRunAfterChange){
      updateFromFlag=UPDATE_FROM_XML;
      pluginProcessor->updateHostAndUi(this,xmlHostValue,true,forceUpdateUi);    
      resetUpdateInfoFlags();
    }else if (forceUpdateUi){
      requestUpdateUi(true);
    }   
	}
}

void Param::updateProcessorHostAndUi(HostFloatType newHostValue, UpdateFromFlags updateFromFlagArg){    
  if (hostSet(newHostValue)){  
    if (!pluginProcessor) return;
    updateFromFlag=updateFromFlagArg;     
    pluginProcessor->updateHostAndUi(this,newHostValue);    
    resetUpdateInfoFlags();  
  }  
}

void Param::updateHostAndUi(bool runAfterParamChange, UpdateFromFlags updateFromFlagArg){      
  if (!pluginProcessor) return;
  updateFromFlag=updateFromFlagArg;

  pluginProcessor->updateHostAndUi(this,hostGet(),runAfterParamChange);    
  resetUpdateInfoFlags();
}

void Param::updateHost(bool runAfterParamChange, UpdateFromFlags updateFromFlagArg){      
  if (!pluginProcessor) return;
  updateFromFlag=updateFromFlagArg;

  pluginProcessor->updateHostAndUi(this,hostGet(),runAfterParamChange,false);    
  resetUpdateInfoFlags();
}

void StringParam::updateProcessorAndHostFromUi(const String valueArg) {    
  if (*value!=valueArg){
    *value=valueArg;
    Param::updateHostFromUi(static_cast<HostFloatType>(0.f));    
    //this parameter can't be automated (maped as a HostFloatType)
    //so hostSet doesn't do anything, we must update *value manually
    //and notify the host always with 0    
  }     
}

void FloatParam::updateProcessorAndHostFromUi(const double valueArg){  
  if (maxValue==minValue)
    Param::updateHostFromUi(static_cast<HostFloatType>(0.f));
  HostFloatType newHostValue=static_cast<HostFloatType>(valueArg-minValue)/(maxValue-minValue);
  if (newHostValue<0)
    newHostValue=static_cast<HostFloatType>(0.f);
  else if (newHostValue>1)
    newHostValue=static_cast<HostFloatType>(1.f);
    
  if (hostSet(newHostValue))
    Param::updateHostFromUi(newHostValue);  
}

void LogParam::updateProcessorAndHostFromUi(const double valueArg){
  
  if (maxLogValue==minLogValue)
    Param::updateHostFromUi(static_cast<HostFloatType>(0.f));
  HostFloatType newHostValue=static_cast<HostFloatType>(valueArg-minLogValue)/(maxLogValue-minLogValue);
  if (newHostValue<0)
    newHostValue=static_cast<HostFloatType>(0.f);
  else if (newHostValue>1)
    newHostValue=static_cast<HostFloatType>(1.f);
  
  if (hostSet(newHostValue))
    Param::updateHostFromUi(newHostValue);  
}

void LogWith0Param::updateProcessorAndHostFromUi(const double valueArg){  
  if (maxLogValue==minLogValue){ //do not let idiots make this crash
    if (valueArg>0)
      Param::updateHostFromUi(static_cast<HostFloatType>(1.f)); //stupid question, stupid answer
    else
      Param::updateHostFromUi(static_cast<HostFloatType>(0.f)); //stupid question, stupid answer
  }
  HostFloatType newHostValue; 
  //using the host parameter scale of [0,1]
  //store positive log value above 0.05 
  //all values in the range of [0,minLogValue] will be stored as 0
  //at 0 (a margin of 0.05 should be safe to avoid confusing 0 with the former)
  
  //[minLogValue-0.05,minLogValue] represent -inf in the UI
  if (valueArg>=minLogValue){      
    newHostValue=static_cast<HostFloatType>(0.05+(valueArg-minLogValue)*0.95/(maxLogValue-minLogValue));
  } else {
    newHostValue=static_cast<HostFloatType>(0);
  }           
  if (newHostValue<0)
    newHostValue=static_cast<HostFloatType>(0.f);
  else if (newHostValue>1)
    newHostValue=static_cast<HostFloatType>(1.f);
  
  if (hostSet(newHostValue))
    Param::updateHostFromUi(newHostValue);  
}

void LogWithSignParam::updateProcessorAndHostFromUi(const double valueArg){
  if (maxPosLogValue==minAbsLogValue || maxNegLogValue==minAbsLogValue){ //do not let idiots make this crash
      if (valueArg>0)
        Param::updateHostFromUi(static_cast<HostFloatType>(1.f)); //stupid question, stupid answer
      else
        Param::updateHostFromUi(static_cast<HostFloatType>(0.f)); //stupid question, stupid answer
    }  
  HostFloatType newHostValue; 
  //using the host parameter scale of [0,1]
  //store positive log value above 0.55 and negative log values below 0.45
  //all values in the range of [-minLogValue,minLogValue] will be stored as 0
  //at 0.5 (a margin of 0.05 should be safe to avoid confusing 0 with the former)  
  
  //in the UI we show a positive range of [0.05,0.05+maxPosLogValue-minAbsLogValue]
  //and a negative range of [-(0.05+maxNegLogValue-minAbsLogValue),-0.05]
  //(-0.05,0.05) represents -inf     
  if (valueArg>=0.05){        
    newHostValue=(HostFloatType)(centerValue+0.05+(valueArg-0.05)*(1-centerValue-0.05)/(maxPosLogValue-minAbsLogValue));
  } else if (valueArg<=-0.05){
    newHostValue=(HostFloatType)(centerValue-0.05+(valueArg+0.05)*(centerValue-0.05)/(maxNegLogValue-minAbsLogValue));
  } else {
    newHostValue=centerValue;
  }       
  if (newHostValue<0)
    newHostValue=static_cast<HostFloatType>(0.f);
  else if (newHostValue>1)
    newHostValue=static_cast<HostFloatType>(1.f);
  
  if (hostSet(newHostValue))
    Param::updateHostFromUi(newHostValue);  
}

void IntParam::updateProcessorAndHostFromUi(const int valueArg){  
  if (maxValue==minValue)
    Param::updateHostFromUi(static_cast<HostFloatType>(0.f));
  HostFloatType newHostValue=static_cast<HostFloatType>(valueArg-minValue)/(maxValue-minValue);
  if (newHostValue<0)
    newHostValue=static_cast<HostFloatType>(0.f);
  else if (newHostValue>1)
    newHostValue=static_cast<HostFloatType>(1.f);
  
  if (hostSet(newHostValue))
    Param::updateHostFromUi(newHostValue);  
}

void BoolParam::updateProcessorAndHostFromUi(const bool valueArg){  
  HostFloatType newHostValue=(valueArg)?static_cast<HostFloatType>(1.f):static_cast<HostFloatType>(0.f);
  
  if (hostSet(newHostValue))
    Param::updateHostFromUi(newHostValue);  
}

void ParamGroup::addStringParamArray(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, String* const values,int *const size,const int maxSize,bool saveOnlySizedArrayFlag,bool saveOnlyNonDefaultValuesFlag){
  addParamGroup(paramIndex,new StringParamArray(name,automationFlag,loadSaveXmlFlag,values,size,maxSize,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag));
}

void ParamGroup::addStringParamMatrix(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, String** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag, bool saveOnlyNonDefaultValuesFlag){
  addParamGroup(paramIndex,new StringParamMatrix(name,automationFlag,loadSaveXmlFlag,values,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag));
}

void ParamGroup::addFloatParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType* const values,int *const size,const int maxSize,const PluginFloatType minValue,const PluginFloatType maxValue, bool saveOnlySizedArrayFlag, bool saveOnlyNonDefaultValuesFlag){
  addParamGroup(paramIndex,new FloatParamArray(name,automationFlag,loadSaveXmlFlag,values,size,maxSize,minValue,maxValue,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag));
}

void ParamGroup::addFloatParamMatrix(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols,const PluginFloatType minValue,const PluginFloatType maxValue, const bool saveOnlySizedMatrixFlag, bool saveOnlyNonDefaultValuesFlag){
  addParamGroup(paramIndex,new FloatParamMatrix(name,automationFlag,loadSaveXmlFlag,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag));
}

void ParamGroup::addLogParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType* const values,int *const size,const int maxSize,const PluginFloatType minValue,const PluginFloatType maxValue, const PluginFloatType factor, bool saveOnlySizedArrayFlag, bool saveOnlyNonDefaultValuesFlag){
  addParamGroup(paramIndex,new LogParamArray(name,automationFlag,loadSaveXmlFlag,values,size,maxSize,minValue,maxValue,factor,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag));
}

void ParamGroup::addLogParamMatrix(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginFloatType minValue,const PluginFloatType maxValue, const PluginFloatType factor, const bool saveOnlySizedMatrixFlag, bool saveOnlyNonDefaultValuesFlag){
  addParamGroup(paramIndex,new LogParamMatrix(name,automationFlag,loadSaveXmlFlag,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,factor,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag));
}

void ParamGroup::addLogWith0ParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType* const values,int *const size,const int maxSize,const PluginFloatType minValue,const PluginFloatType maxValue, const PluginFloatType factor, bool saveOnlySizedArrayFlag, bool saveOnlyNonDefaultValuesFlag){
  addParamGroup(paramIndex,new LogWith0ParamArray(name,automationFlag,loadSaveXmlFlag,values,size,maxSize,minValue,maxValue,factor,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag));
}

void ParamGroup::addLogWith0ParamMatrix(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginFloatType minValue,const PluginFloatType maxValue, const PluginFloatType factor, const bool saveOnlySizedMatrixFlag, bool saveOnlyNonDefaultValuesFlag){
  addParamGroup(paramIndex,new LogWith0ParamMatrix(name,automationFlag,loadSaveXmlFlag,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,factor,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag));
}

void ParamGroup::addLogWithSignParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType* const values,int *const size,const int maxSize,const PluginFloatType minValue,const PluginFloatType maxValue, const PluginFloatType minAbsValue, const PluginFloatType factor, bool saveOnlySizedArrayFlag, bool saveOnlyNonDefaultValuesFlag){
  addParamGroup(paramIndex,new LogWithSignParamArray(name,automationFlag,loadSaveXmlFlag,values,size,maxSize,minValue,maxValue,minAbsValue,factor,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag));
}

void ParamGroup::addLogWithSignParamMatrix(const int paramIndex,const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginFloatType minValue,const PluginFloatType maxValue, const PluginFloatType minAbsValue, const PluginFloatType factor, const bool saveOnlySizedMatrixFlag, bool saveOnlyNonDefaultValuesFlag){
  addParamGroup(paramIndex,new LogWithSignParamMatrix(name,automationFlag,loadSaveXmlFlag,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,minAbsValue,factor,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag));
}

void ParamGroup::addIntParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginIntType* const values,int *const size,const int maxSize,const PluginIntType minValue,const PluginIntType maxValue, bool saveOnlySizedArrayFlag, bool saveOnlyNonDefaultValuesFlag){
  addParamGroup(paramIndex,new IntParamArray(name,automationFlag,loadSaveXmlFlag,values,size,maxSize,minValue,maxValue,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag));
}

void ParamGroup::addIntParamMatrix(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginIntType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols,const PluginIntType minValue,const PluginIntType maxValue, const bool saveOnlySizedMatrixFlag, bool saveOnlyNonDefaultValuesFlag){
  addParamGroup(paramIndex,new IntParamMatrix(name,automationFlag,loadSaveXmlFlag,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag));
}

void ParamGroup::addBoolParamArray(const int paramIndex,const bool automationFlag, const bool loadSaveXmlFlag, bool* const values,int *const size,const int maxSize,bool saveOnlySizedArrayFlag,bool saveOnlyNonDefaultValuesFlag){
  addParamGroup(paramIndex,new BoolParamArray(name,automationFlag,loadSaveXmlFlag,values,size,maxSize,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag));
}

void ParamGroup::addBoolParamMatrix(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, bool** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag, bool saveOnlyNonDefaultValuesFlag){
  addParamGroup(paramIndex,new BoolParamMatrix(name,automationFlag,loadSaveXmlFlag,values,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag));
}