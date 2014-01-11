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

#include "PluginProcessor.h"
#include "Params.h"

/*
Only methods who implicate a circular reference and therefore
can't be written in the header file are placed here.
*/

//protected

void Param::updateHostFromUi(PluginParameters_HostFloatType newHostValue){
  if (pluginProcessor==nullptr) return;  
  updateFromFlag=UPDATE_FROM_UI;
  pluginProcessor->updateHostAndUi(this,newHostValue,true,false); 
  resetUpdateFromFlag();
}

//public

void Param::updateProcessorHostAndUiFromXml(bool forceUpdateHost,bool forceUpdateUi){      
  if (pluginProcessor==nullptr) return;
  if (settings[loadFromSession] || settings[loadFromPresets]){
    if (updateProcessorFromXml()){
      pluginProcessor->updateHostAndUi(this,xmlHostValue,false);    
      resetUpdateFromFlag();
    } else if (forceUpdateHost){
      pluginProcessor->updateHostAndUi(this,xmlHostValue,false,forceUpdateUi);    
      resetUpdateFromFlag();
    } else if (forceUpdateUi){
      updateUi(true);
    }   
	}
}

void Param::updateProcessorHostAndUiFromDefaultXml(bool forceUpdateHost,bool forceUpdateUi){
  if (pluginProcessor==nullptr) return;
  if (settings[loadFromSession] || settings[loadFromPresets]){
    if (updateProcessorFromDefaultXml()){
      pluginProcessor->updateHostAndUi(this,xmlHostValue,false);
      resetUpdateFromFlag();
    } else if (forceUpdateHost){
      pluginProcessor->updateHostAndUi(this,xmlHostValue,false,forceUpdateUi);    
      resetUpdateFromFlag();
    } else if (forceUpdateUi){
      updateUi(true);
    }   
  }
}

void Param::updateProcessorHostAndUi(PluginParameters_HostFloatType newHostValue, UpdateFromFlags updateFromFlagArg){    
  if (hostSet(newHostValue)){  
    if (pluginProcessor==nullptr) return;
    updateFromFlag=updateFromFlagArg;     
    pluginProcessor->updateHostAndUi(this,newHostValue);    
    resetUpdateFromFlag();  
  }  
}

void Param::updateHostAndUi(bool runAfterParamChange, UpdateFromFlags updateFromFlagArg){      
  if (pluginProcessor==nullptr) return;
  updateFromFlag=updateFromFlagArg;

  pluginProcessor->updateHostAndUi(this,hostGet(),runAfterParamChange);    
  resetUpdateFromFlag();
}

void Param::updateHost(bool runAfterParamChange, UpdateFromFlags updateFromFlagArg){      
  if (pluginProcessor==nullptr) return;
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
