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

#ifndef __JUCER_HEADER_PARAMETERTOOLS
#define __JUCER_HEADER_PARAMETERTOOLS

#include <cmath>

#include "../JuceLibraryCode/JuceHeader.h"

typedef float HostFloatType;
typedef float PluginFloatType;
typedef int PluginIntType;

namespace PluginParameters{
  const PluginFloatType epsilon=(PluginFloatType)1e-6; //used to avoid rounding errors
};

class PluginProcessor;

//UPDATE_FROM_HOST will usally be associated to Automation
enum UpdateFromFlags{
  UPDATE_FROM_HOST=0x01,
  UPDATE_FROM_UI=0x02,
  UPDATE_FROM_PROCESSOR=0x04,
  UPDATE_FROM_XML=0x08
}; 

/** Base class for all parameters. Distinctions by type are made below. */
class Param{
private:     
  const int globalIndex;
  
  /** This flag determines if a parameter will be automated in the host.  */  
  const bool automationFlag;

  const String name;
  
  const String type; 
    
  /** Pointer to the AudioProcessor class to be able notify parameter changes to the host 
      with load */
  PluginProcessor* pluginProcessor;
  
  bool updateUiFlag;     
  
  UpdateFromFlags updateFromFlag;    
  
protected: 
  void updateHostFromUi(HostFloatType newValue);  
  
  bool loadXmlFlag,saveXmlFlag;
  
  HostFloatType xmlHostValue;
    
  // (prevent copy constructor and operator= being generated..)
  // avoids warning C4512: "assignment operator could not be generated"
  Param (const Param&);
  Param& operator=(const Param &other);      
    
public:      
  /** returns true if this parameter is automated or not */
  bool automationIsOn() const { return automationFlag; }

  /** Index in the global list of automated parameters or the list of non automated parameters
      (if automationFlag==true or automationFlag==false respectively) */
  int getGlobalIndex() const { return globalIndex; } 
      
  /** Returns a string label for this parameter */
  const String getName() const { return name; }
  
   /** Returns a string label for the type of this parameter */
  const String getType() const { return type; }

  /** Submit a request for an update in the UI. updateUiRequested() must be called then from
      the UI to query its state. */
  void requestUpdateUi(const bool enable);  
  
  /** Called from the UI timer to determine if the widget associated to this parameter must be 
  updated or not. It automatically sets the updateUi flag to "false" after it.*/
  bool updateUiRequested(){
    if (updateUiFlag){
      updateUiFlag=false;
      return true;
    }
    return false;
  }  
  
  /* Returns whether there is a pending request or not to update the UI without changing it */
  bool updateUiIsOn() const{
    return updateUiFlag;
  }
  
  /**  Returns the parameter value to set the host.
  */
  virtual HostFloatType hostGet() const = 0;  

  /** Preloads an XML attribute into undoRedo and returns its normalized parameter value.
      This value is stored in an auxiliary internal variable but it doesn't change the
      parameter value.
      If the parameter value can't be read from the XML it is set to its default value.
      Call updateProcessorHostAndUiFromXml(...) to set the parameter value to this.
  */
  virtual HostFloatType preLoadXml(XmlElement *xml) = 0;

  /** returns true if this parameter will be imported from xml */
  bool loadXmlIsOn(){
    return loadXmlFlag;
  }

  /** Import this parameter from xml (set to true by default) when the user requests it */
  void setLoadXml(const bool enable){
    loadXmlFlag=enable;
  }
  
  /** returns true if this parameter will be exported to xml */
  bool saveXmlIsOn(){
    return saveXmlFlag;
  }
  
  /** Export this parameter to xml (set to true by default) when the user requests it */
  void setSaveXml(const bool enable){
    saveXmlFlag=enable;
  }
   
  UpdateFromFlags getUpdateFromFlag(){
    return updateFromFlag;
  }  
  
  /** Reset flags used in runAfterParamChange(...) to determine the origin and reason of the update */    
  void resetUpdateInfoFlags(){
    updateFromFlag=UPDATE_FROM_HOST;    
  }

  /** Returns the minimum range of this parameter */  
  virtual const double getMin() const = 0;
  
  /** Returns the maximum range of this parameter */  
  virtual const double getMax() const = 0;

  /** Stores the parameter value as an XML attribute */
  virtual void saveXml(XmlElement *xml) const = 0;      
  
  /**  Sets the parameter from a given value from the host.
       returns true: if it is set to a new value or forceLoad,skipLoad is enabled.
              false: if it set to the same value.
  */
  virtual bool hostSet(const HostFloatType hostValue) = 0;  
  
  /** Set a parameter from the Processor to a new value and notify the host and the UI (when it has changed)
  */  
  void updateProcessorHostAndUi(HostFloatType newValue, UpdateFromFlags updateFromFlag);  
  
  /** Notify the host about the current value of a parameter and update the UI.
      This is useful when you change the value of this parameter (maybe several times) 
      and you don't want to notify the host and the UI until the end.
      */  
  void updateHostAndUi(bool runAfterParamChange, UpdateFromFlags updateFromFlag);
    
  /** Notify the host about the current value of a parameter without updating the UI. 
      This is useful when you change the value of this parameter (maybe several times) 
      and you don't want to notify the host until the end.
      */  
  void updateHost(bool runAfterParamChange, UpdateFromFlags updateFromFlag);
  
  /** Write the xmlValue read with preLoadXml and return true if it was different */
  virtual bool writeXmlValue() = 0;
  
  /** Set a parameter to a new value from the preloaded values from Xml and notify the host and the UI 
      (when it has changed).*/ 
  void updateProcessorHostAndUiFromXml(bool forceRunAfterChange=false,bool forceUpdateUi=false);
  
  Param(PluginProcessor *pluginProcessor, const String &name, const int globalIndex, const bool automationFlag, const bool loadSaveXmlFlag, const String &type):
  pluginProcessor(pluginProcessor),
  globalIndex(globalIndex),
  name(name),
  type(type),  
  automationFlag(automationFlag),
  updateUiFlag(false),  
  loadXmlFlag(loadSaveXmlFlag),
  saveXmlFlag(loadSaveXmlFlag),
  updateFromFlag(UPDATE_FROM_HOST)
  {
    #if JUCE_DEBUG
    // we should be able to use this name as an XML attribute name
    // this checks whether the attribute name string contains any illegal characters...
    for (String::CharPointerType t (name.getCharPointer()); ! t.isEmpty(); ++t)
        jassert (t.isLetterOrDigit() || *t == '_' || *t == '-' || *t == ':');
    #endif            
  }
};

//-----------------------------------------------------------------------------------


/* String parameters can not be automated so their
   automation is merely simulated so that their
   interface presents no difference with the 
   other parameters. */
class StringParam : public Param{
private:
  String virtualHostValue;
  
  // (prevent copy constructor and operator= being generated..)
  // avoids warning C4512: "assignment operator could not be generated"
  StringParam (const StringParam&);
  StringParam& operator=(const StringParam &other);
  
protected:
  String * const value;
  const String defaultValue;
  String xmlValue;  
  
public:
  bool writeXmlValue(){
    if (*value!=xmlValue){
      *value=xmlValue;
      return true;
    }
    return false;
  }
  
  bool hostSet(const HostFloatType ){       
    return false;
  }
   
  /** Updates the value from its UI denormalized value (it doesn't call load since Strings
      can't be automated */ 
  void updateProcessorAndHostFromUi(const String valueArg);  

  /** Returns the parameter value to set the UI */
  String uiGet() const{    
    return (*value);
  }

  HostFloatType hostGet() const{
    return (HostFloatType)(0.f);
  }  
  
  const String getDefaultValue() const{
    return defaultValue;
  }
  
  String getValue() const{
    return *value;
  }
  
  const double getMin() const{
    return 0;
  }
  
  const double getMax() const{
    return 1;
  }
  
  HostFloatType preLoadXml(XmlElement *xml) {
    if (xml==nullptr)
      xmlValue=defaultValue;
    else
      xmlValue=xml->getStringAttribute(Param::getName(),defaultValue);
    return xmlHostValue=(HostFloatType)(0.f);
  }  

  void saveXml(XmlElement *xml) const{
    if (Param::saveXmlFlag)
      xml->setAttribute(Param::getName(),(*value));
  }


 StringParam(PluginProcessor *pluginProcessor, const String &name, const int globalIndex, const bool automationFlag, const bool loadSaveXmlFlag, String * const value):
  Param(pluginProcessor,name,globalIndex,automationFlag,loadSaveXmlFlag,"String"),
  value(value),
  defaultValue(*value),
  xmlValue(*value){
    // Strings cannot be automated! 
    // (They aren't supported at least in VST)
    // Try again setting argument automationFlag=false
    jassert(automationFlag==false);
  }
};

class FloatParam : public Param{
private:
  PluginFloatType minValue;
  PluginFloatType maxValue;

  // (prevent copy constructor and operator= being generated..)
  // avoids warning C4512: "assignment operator could not be generated"
  FloatParam (const FloatParam&);
  FloatParam& operator=(const FloatParam &other);
  
protected:
  PluginFloatType * const value;
  const PluginFloatType defaultValue;
  PluginFloatType xmlValue;  
  
public:
  bool writeXmlValue(){
    if (*value!=xmlValue){
      *value=xmlValue;
      return true;
    }
    return false;
  }
  
  bool hostSet(const HostFloatType hostValue){    
    PluginFloatType oldValue=*value;
    if (hostValue>1)
      *value=maxValue;
    else if (hostValue<0)
      *value=minValue;
    else
      *value=minValue+(PluginFloatType)(hostValue)*(maxValue-minValue);

    if (fabs(*value-oldValue)>PluginParameters::epsilon)
		  return true;
		
		return false;
  }
  
  /** Updates the value from its UI denormalized value */ 
  void updateProcessorAndHostFromUi(const double valueArg);
  
  /** Returns the parameter value to set the UI */
  double uiGet() const{
    if (*value<minValue)
      return (double)(minValue);
    else if (*value>maxValue)
      return (double)(maxValue);
    else
      return (double)(*value);
  }

  HostFloatType hostGet() const{    
    if (maxValue==minValue)
      return (HostFloatType)(0.f);
    HostFloatType newHostValue=(HostFloatType)(*value-minValue)/(maxValue-minValue);
    if (newHostValue<0){
      *value=minValue;
      return (HostFloatType)(0.f);
    } else if (newHostValue>1){
      *value=maxValue;
      return (HostFloatType)(1.f);
    } else
      return newHostValue;
  }  

  void setMin(PluginFloatType minValueArg){        
    if (defaultValue>=minValueArg){
      minValue=minValueArg;
      if (*value<minValueArg)
        *value=minValueArg;
      if (xmlValue<minValueArg)
        xmlValue=minValueArg;
    } else //minValue can't be greater than the (constant) default value
      jassertfalse;
  }

  void setMax(PluginFloatType maxValueArg){    
    if (defaultValue<=maxValueArg){
      maxValue=maxValueArg;
      if (*value>maxValueArg)
        *value=maxValueArg;
      if (xmlValue>maxValueArg)
        xmlValue=maxValueArg;
    } else //maxValue can't be less than the (constant) default value
      jassertfalse;
  }

  const PluginFloatType getDefaultValue() const{
    return defaultValue;    
  }
  
  PluginFloatType getPreloadvalue() const{
    return xmlValue;
  }
  
  PluginFloatType getValue() const{
    return *value;
  }

  const double getMin() const{
    return minValue;
  }

  const double getMax() const{
    return maxValue;
  }

  HostFloatType preLoadXml(XmlElement *xml){    
    if (xml==nullptr)
      xmlValue=defaultValue;
    else
      xmlValue=(PluginFloatType)(xml->getDoubleAttribute(Param::getName(),defaultValue));
    if (maxValue==minValue)
      return xmlHostValue=(HostFloatType)(0.f);
    xmlHostValue=(HostFloatType)(xmlValue-minValue)/(maxValue-minValue);
    if (xmlHostValue<0){
      xmlValue=minValue;
      return xmlHostValue=(HostFloatType)(0.f);
    }
    else if (xmlHostValue>1){
      xmlValue=maxValue;
      return xmlHostValue=(HostFloatType)(1.f);
    }
    else
      return xmlHostValue;
  }

  void saveXml(XmlElement *xml) const{
    if (Param::saveXmlFlag)
      xml->setAttribute(Param::getName(),(double)(*value));
  }
  
  FloatParam(PluginProcessor *pluginProcessor, const String &name, const int globalIndex, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType * const value, const PluginFloatType minValue=(PluginFloatType)(0),const PluginFloatType maxValue=(PluginFloatType)(0)):
  Param(pluginProcessor,name,globalIndex,automationFlag,loadSaveXmlFlag,"Float"),
  defaultValue(jmax<PluginFloatType>(minValue,jmin<PluginFloatType>(*value,maxValue))),
  minValue(minValue),
  maxValue(maxValue),
  value(value){    
    //force *value to the [minValue,maxValue] range
    xmlValue=*value=defaultValue;
  }

};

class LogParam : public Param{
private:
  PluginFloatType minLogValue;
  PluginFloatType maxLogValue;
  const PluginFloatType factor;
  
  // (prevent copy constructor and operator= being generated..)
  // avoids warning C4512: "assignment operator could not be generated"
  LogParam (const LogParam&);
  LogParam& operator=(const LogParam &other);

protected:
  PluginFloatType * const value;
  const PluginFloatType defaultValue; 
  PluginFloatType xmlValue;  
  
public:
  bool writeXmlValue(){
    if (*value!=xmlValue){
      *value=xmlValue;
      return true;
    }
    return false;
  }
  
  bool hostSet(const HostFloatType hostValue){
    PluginFloatType oldValue=*value;
    if (hostValue>1)
      *value=maxLogValue;
    else if (hostValue<0)
      *value=minLogValue;
    else
      *value=(PluginFloatType)pow(10,(double)((minLogValue+hostValue*(maxLogValue-minLogValue))/factor));    

    if (fabs(*value-oldValue)>PluginParameters::epsilon)
		  return true;
		
		return false;
  }
  
  /** Updates the value from its UI denormalized value */ 
  void updateProcessorAndHostFromUi(const double valueArg);
  
  /** Returns the parameter value to set the UI */
  double uiGet() const{
    if (*value<=0) 
      return (double)(minLogValue);
    else{
      double uiValue=factor*log10(fabs((double)*value));
      if (uiValue<minLogValue)
        return (double)(minLogValue);
      else if (uiValue>maxLogValue)        
        return (double)(maxLogValue);
      else
        return (double)(uiValue);
    }
  }

  HostFloatType hostGet() const{        
    if (maxLogValue==minLogValue)
      return (HostFloatType)(0.f);
    if (*value<=0){
      *value=(PluginFloatType)(pow(10,(double)(minLogValue/factor)));
      return (HostFloatType)(0.f);
    }
    HostFloatType newHostValue=(HostFloatType)(factor*log10(fabs((double)*value))-minLogValue)/(maxLogValue-minLogValue);
    if (newHostValue<0){
      *value=(PluginFloatType)(pow(10,(double)(minLogValue/factor)));
      return (HostFloatType)(0.f);
    } else if (newHostValue>1){
      *value=(PluginFloatType)(pow(10,(double)(maxLogValue/factor)));
      return (HostFloatType)(1.f);
    } else
      return newHostValue;
  }  

  void setMin(PluginFloatType minValueArg){
    if (defaultValue>=minValueArg && minValueArg>0){
      minLogValue=(PluginFloatType)(factor*log10((double)(minValueArg)));
      if (*value<minValueArg)
        *value=minValueArg;
      if (xmlValue<minValueArg)
        xmlValue=minValueArg;
    } else //minValue can't be <=0 or greater than the (constant) default value
      jassertfalse;
  }

  void setMax(PluginFloatType maxValueArg){    
    if (defaultValue<=maxValueArg && maxValueArg>0){
      maxLogValue=(PluginFloatType)(factor*log10((double)(maxValueArg)));
      if (*value>maxValueArg)
        *value=maxValueArg;
      if (xmlValue>maxValueArg)
        xmlValue=maxValueArg;
    } else //maxValue can't be <=0 or less than the (constant) default value
      jassertfalse;
  }
  
  const PluginFloatType getDefaultValue() const{
    return defaultValue;
  }
  
  PluginFloatType getValue() const{
    return *value;
  }
  
  const PluginFloatType getFactor() const{
    return factor;
  }  
  
  const double getMin() const{
    return minLogValue;
  }

  const double getMax() const{
    return maxLogValue;
  }

  HostFloatType preLoadXml(XmlElement *xml){                  
    if (xml==nullptr)
      xmlValue=defaultValue;
    else
      xmlValue=(PluginFloatType)(xml->getDoubleAttribute(Param::getName(),defaultValue));      
    
    if (maxLogValue==minLogValue)
      return (HostFloatType)(0.f);
    
    if (xmlValue<=0){
      xmlValue=(PluginFloatType)(pow(10,(double)(minLogValue/factor))); //minValue
      return (HostFloatType)(0.f);
    }    
    xmlHostValue=(HostFloatType)(factor*log10(fabs((double)xmlValue))-minLogValue)/(maxLogValue-minLogValue);
    if (xmlHostValue<0){
      xmlValue=(PluginFloatType)(pow(10,(double)(minLogValue/factor)));
      return xmlHostValue=(HostFloatType)(0.f);
    }else if (xmlHostValue>1){
      xmlValue=(PluginFloatType)(pow(10,(double)(maxLogValue/factor)));
      return xmlHostValue=(HostFloatType)(1.f);
    } else
      return xmlHostValue;
  }

  void saveXml(XmlElement *xml) const{
    if (Param::saveXmlFlag)
      xml->setAttribute(Param::getName(),(double)(*value));
  }
  
  LogParam(PluginProcessor *pluginProcessor, const String &name, const int globalIndex, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType * const value, const PluginFloatType minValue=(PluginFloatType)(0),const PluginFloatType maxValue=(PluginFloatType)(0),const PluginFloatType factor=(PluginFloatType)(1)):
  Param(pluginProcessor,name,globalIndex,automationFlag,loadSaveXmlFlag,"Log"),
  defaultValue(jmax<PluginFloatType>(minValue,jmin<PluginFloatType>(*value,maxValue))),
  minLogValue((PluginFloatType)(factor*log10((double)(minValue)))),
  maxLogValue((PluginFloatType)(factor*log10((double)(maxValue)))),
  factor(factor),
  value(value){
    //log values are stricly positive, please define a strictly positive range
    jassert(minValue>0);
    jassert(maxValue>0);

    //force *value to the [minValue,maxValue] range
    xmlValue=*value=defaultValue;
  }
};

class LogWith0Param : public Param{
private:
  const PluginFloatType factor;
  PluginFloatType minLogValue;
  PluginFloatType maxLogValue;  
  PluginFloatType minValue;
  PluginFloatType maxValue;  
  
  // (prevent copy constructor and operator= being generated..)
  // avoids warning C4512: "assignment operator could not be generated"
  LogWith0Param (const LogWith0Param&);
  LogWith0Param& operator=(const LogWith0Param &other);
  
protected:
  PluginFloatType * const value;
  const PluginFloatType defaultValue;  
  PluginFloatType xmlValue;   

public: 
  bool writeXmlValue(){
    if (*value!=xmlValue){
      *value=xmlValue;
      return true;
    }
    return false;
  }
  
  bool hostSet(const HostFloatType hostValue){
    PluginFloatType oldValue=*value;
    if (hostValue>1)
      *value=maxLogValue;
    else if (hostValue<0.03){
      //leave a margin of 0.02 to avoid precision errors to confuse +/-minLog with 0
      *value=(PluginFloatType)(0);
    } else {
      *value=(PluginFloatType)pow(10,(double)((minLogValue+(jmax<double>((HostFloatType)0.05,hostValue)-(HostFloatType)0.05)*(maxLogValue-minLogValue)/0.95)/factor));
    }
    if (fabs(*value-oldValue)>PluginParameters::epsilon)
		  return true;
		
		return false;
  }
  
  /** Updates the value from its UI denormalized value */ 
  void updateProcessorAndHostFromUi(const double valueArg);
  
  /** Returns the parameter value to set the UI */
  double uiGet() const{
    //[minLogValue-0.05,minLogValue] represent -inf in the UI
    
    if (*value<=0)
      return (double)(minLogValue-0.05);
    
    double uiValue=factor*log10(fabs((double)*value));
    
    if (uiValue<minLogValue)
      return (double)(minLogValue-0.05);
    
    return jmin<double>(maxLogValue,uiValue);
  }
  
  HostFloatType hostGet() const{    
    if (maxLogValue==minLogValue){ //do not let idiots make this crash
      return (HostFloatType)(0.f); //stupid question, stupid answer
    }
    
    //using the host parameter scale of [0,1]
    //store positive log value above 0.05 
    //all values in the range of [0,minLogValue] will be stored as 0
    //at 0 (a margin of 0.05 should be safe to avoid confusing 0 with the former)    
    if (*value<=0){
      *value=(PluginFloatType)0;
      return (HostFloatType)(0.f);
    }
            
    HostFloatType newHostValue=(HostFloatType)(0.05+(factor*log10(fabs((double)*value))-minLogValue)*0.95/(maxLogValue-minLogValue));
    
    if (newHostValue<0){
      *value=(PluginFloatType)0;
      return (HostFloatType)(0.f);
    }else if (newHostValue>1){
      *value=(PluginFloatType)(pow(10,(double)(maxLogValue/factor)));
      return (HostFloatType)(1.f);
    } else
      return newHostValue;    
  }
  
  const PluginFloatType getDefaultValue() const{
    return defaultValue;
  }
  
  PluginFloatType getValue() const{
    return *value;
  }
  
  const PluginFloatType getFactor() const{
    return factor;
  }
    
  void setMin(PluginFloatType minValueArg){
    if (defaultValue>=minValueArg && minValueArg>0){
      minLogValue=(PluginFloatType)(factor*log10((double)(minValueArg)));
      if (*value<minValueArg)
        *value=0;
      if (xmlValue<minValueArg)
        xmlValue=0;
    } else //minValue can't be <=0 or greater than the (constant) default value
      jassertfalse;
  }

  void setMax(PluginFloatType maxValueArg){    
    if (defaultValue<=maxValueArg && maxValueArg>0){
      maxLogValue=(PluginFloatType)(factor*log10((double)(maxValueArg)));
      if (*value>maxValueArg)
        *value=maxValueArg;
      if (xmlValue>maxValueArg)
        xmlValue=maxValueArg;
    } else //maxValue can't be <=0 or less than the (constant) default value
      jassertfalse;
  }  
  
  const double getMin() const{
    return minLogValue-0.05;
  }

  const double getMax() const{
    return maxLogValue;
  }

  HostFloatType preLoadXml(XmlElement *xml){   
    if (xml==nullptr)
      xmlValue=defaultValue;
    else
      xmlValue=(PluginFloatType)(xml->getDoubleAttribute(Param::getName(),defaultValue));     
                     
    if (maxLogValue==minLogValue){ //do not let idiots make this crash
      return xmlHostValue=(HostFloatType)(0.f); //stupid question, stupid answer      
    }
    //using the host parameter scale of [0,1]
    //store positive log value above 0.05 
    //all values in the range of [0,minLogValue] will be stored as 0
    //at 0 (a margin of 0.05 should be safe to avoid confusing 0 with the former)     
    if (xmlValue<=0){
      xmlValue=0;
      return xmlHostValue=(HostFloatType)(0);
    } else {
      double xmlLogValue=factor*log10(fabs((double)xmlValue));
      if (xmlLogValue<minLogValue){
        xmlValue=0;
        xmlHostValue=(HostFloatType)(0);
      }else{
        xmlHostValue=(HostFloatType)(0.05+(xmlLogValue-minLogValue)*0.95/(maxLogValue-minLogValue));
      }     
    }
    if (xmlHostValue>1){
      xmlValue=(PluginFloatType)(pow(10,(double)(maxLogValue/factor)));
      return xmlHostValue=(HostFloatType)(1.f);      
    }
    return xmlHostValue;        
  }

  void saveXml(XmlElement *xml) const{
    if (Param::saveXmlFlag)
      xml->setAttribute(Param::getName(),(double)(*value));
  }

  LogWith0Param(PluginProcessor *pluginProcessor, const String &name, const int globalIndex, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType * const value, const PluginFloatType minValue=(PluginFloatType)(0.001),const PluginFloatType maxValue=(PluginFloatType)(1),const PluginFloatType factor=(PluginFloatType)(1)):
  Param(pluginProcessor,name,globalIndex,automationFlag,loadSaveXmlFlag,"LogWith0"),
  defaultValue(jmax<PluginFloatType>(0,jmin<PluginFloatType>(*value,maxValue))),
  minLogValue((PluginFloatType)(factor*log10((double)(minValue)))),
  maxLogValue((PluginFloatType)(factor*log10((double)(maxValue)))),
  value(value),
  factor(factor){
    //log values are stricly positive, please define a strictly positive range
    jassert(minValue>0);
    jassert(maxValue>0);

    //force *value to the [0,maxValue] range
    xmlValue=*value=defaultValue;
  }
};

class LogWithSignParam : public Param{
private:  
  PluginFloatType maxNegLogValue;
  PluginFloatType maxPosLogValue;  
  PluginFloatType minAbsLogValue;
  const PluginFloatType factor;
  const HostFloatType centerValue;
  
  // (prevent copy constructor and operator= being generated..)
  // avoids warning C4512: "assignment operator could not be generated"
  LogWithSignParam (const LogWithSignParam&);
  LogWithSignParam& operator=(const LogWithSignParam &other);
  
protected:
  PluginFloatType * const value;
  const PluginFloatType defaultValue;  
  PluginFloatType xmlValue;   

public:
  bool writeXmlValue(){
    if (*value!=xmlValue){
      *value=xmlValue;
      return true;
    }
    return false;
  }
  
  bool hostSet(const HostFloatType hostValue){  
    PluginFloatType oldValue=*value;    
    if (hostValue>1)
      *value=maxPosLogValue;
    else if (hostValue<0)
      *value=maxNegLogValue;
    else{
      //leave a margin of 0.02 to avoid precision errors to confuse +/-minLog with 0
      if (hostValue>centerValue+0.03){
        *value=(PluginFloatType)pow(10,((double)(minAbsLogValue+(jmax<double>(0,hostValue-(centerValue+0.05)))*(maxPosLogValue-minAbsLogValue)/(1-centerValue-0.05))/(factor)));
      } else if (hostValue<centerValue-0.03) {
        *value=-(PluginFloatType)pow(10,((double)(minAbsLogValue+((centerValue-0.05)-jmin<double>((centerValue-0.05),hostValue))*(maxNegLogValue-minAbsLogValue)/(centerValue-0.05))/(factor)));
      } else
        *value=(PluginFloatType)(0);
    }      

    if (fabs(*value-oldValue)>PluginParameters::epsilon)
		  return true;
		
		return false;
  }
  
  /** Updates the value from its UI denormalized value */ 
  void updateProcessorAndHostFromUi(const double valueArg);
  
  /** Returns the parameter value to set the UI */
  double uiGet() const{
    //in the UI we show a positive range of [0.05,0.05+maxPosLogValue-minAbsLogValue]
    //and a negative range of [-(0.05+maxNegLogValue-minAbsLogValue),-0.05]
    //[-0.05,0.05] represents -inf    
    if (*value==0){      
      return 0;
    }
    
    double newValue=factor*log10(fabs((double)*value));
    
    if (newValue<minAbsLogValue)
      return 0;
    else if (*value>0){      
      if (newValue>maxPosLogValue)
        return (double)(0.05+maxPosLogValue-minAbsLogValue);
      else
        return (double)(0.05+newValue-minAbsLogValue);
    } else { // *value<0
      if (newValue>maxNegLogValue)
        return -(double)(0.05+maxNegLogValue-minAbsLogValue);
      else
        return -(double)(0.05+newValue-minAbsLogValue);
    }
  }

  HostFloatType hostGet() const{    
    if (maxPosLogValue==minAbsLogValue || maxNegLogValue==minAbsLogValue){ //do not let idiots make this crash
      if (*value>0)
        return (HostFloatType)(1.f); //stupid question, stupid answer
      else
        return (HostFloatType)(0.f); //stupid question, stupid answer
    }
    HostFloatType newHostValue; 
    
    //using the host parameter scale of [0,1]
    //store positive log value above 0.55 and negative log values below 0.45
    //all values in the range of [-minLogValue,minLogValue] will be stored as 0
    //at 0.5 (a margin of 0.05 should be safe to avoid confusing 0 with the former)    
    if (*value==0){      
      return centerValue;
    }
    
    double logValue=factor*log10(fabs((double)*value));
    
    if (logValue<minAbsLogValue){
      *value=0;
      return centerValue;
    }
    
    if (*value>0){
      newHostValue=(HostFloatType)(centerValue+0.05+(logValue-minAbsLogValue)/(maxPosLogValue-minAbsLogValue)*(1-centerValue-0.05));
    } else {
      newHostValue=(HostFloatType)(centerValue-0.05-(logValue-minAbsLogValue)/(maxNegLogValue-minAbsLogValue)*(centerValue-0.05));
    }
    
    if (newHostValue<0){
      *value=-(PluginFloatType)(pow(10,(double)(maxNegLogValue/factor)));
      return (HostFloatType)(0.f);
    }else if (newHostValue>1){
      *value=(PluginFloatType)(pow(10,(double)(maxPosLogValue/factor)));
      return (HostFloatType)(1.f);
    } else
      return newHostValue;   
  }  
  
  const PluginFloatType getDefaultValue() const{
    return defaultValue;
  }
  
  PluginFloatType getValue() const{
    return *value;
  }
  
  const PluginFloatType getFactor() const{
    return factor;
  }
  
  void setMin(PluginFloatType minValueArg){
    if (defaultValue>=minValueArg && minValueArg<0){
      maxNegLogValue=(PluginFloatType)(factor*log10(fabs((double)(minValueArg))));
      if (*value<minValueArg)
        *value=minValueArg;
      if (xmlValue<minValueArg)
        xmlValue=minValueArg;
    } else //minValue can't be >=0 or greater than the (constant) default value
      jassertfalse;
  }
  
  void setMinAbs(PluginFloatType minAbsValueArg){    
    if (fabs(defaultValue)<=minAbsValueArg && minAbsValueArg>0){
      minAbsLogValue=(PluginFloatType)(factor*log10((double)(minAbsValueArg)));
      if (fabs(*value)>minAbsValueArg){
        if (*value<0)
          *value=-minAbsValueArg;
        else
          *value=minAbsValueArg;
      }
      if (fabs(xmlValue)>minAbsValueArg){
        if (xmlValue<0)
          xmlValue=-minAbsValueArg;
        else
          xmlValue=minAbsValueArg;
      }
    } else //minAbsValue can't be <=0 or greater in abs value than the (constant) default value
      jassertfalse;
  }  

  void setMax(PluginFloatType maxValueArg){    
    if (defaultValue<=maxValueArg && maxValueArg>0){
      maxPosLogValue=(PluginFloatType)(factor*log10((double)(maxValueArg)));
      if (*value>maxValueArg)
        *value=maxValueArg;
      if (xmlValue>maxValueArg)
        xmlValue=maxValueArg;
    } else //maxValue can't be <=0 or less than the (constant) default value
      jassertfalse;
  }  
  
  const double getMinAbs() const{
    return pow(10,(double)(minAbsLogValue/factor));
  }  
  
  const double getMin() const{
    return -(maxNegLogValue-minAbsLogValue+0.05);
  }

  const double getMax() const{
    return maxPosLogValue-minAbsLogValue+0.05;
  }

  HostFloatType preLoadXml(XmlElement *xml){        
    if (xml==nullptr)
      xmlValue=defaultValue;
    else
      xmlValue=(PluginFloatType)(xml->getDoubleAttribute(Param::getName(),defaultValue));              
    if (maxPosLogValue==minAbsLogValue || maxNegLogValue==minAbsLogValue){ //do not let idiots make this crash
      if (xmlValue>0)
        return xmlHostValue=(HostFloatType)(1.f); //stupid question, stupid answer
      else
        return xmlHostValue=(HostFloatType)(0.f); //stupid question, stupid answer
    }        
    
    //using the host parameter scale of [0,1]
    //store positive log value above 0.55 and negative log values below 0.45
    //all values in the range of [-minLogValue,minLogValue] will be stored as 0
    //at 0.5 (a margin of 0.05 should be safe to avoid confusing 0 with the former)    
    if (xmlValue==0){
      xmlValue=0;
      return xmlHostValue=centerValue;
    }    
    
    double logValue=factor*log10(fabs((double)(xmlValue)));
    
    if (logValue<minAbsLogValue){
      xmlValue=0;
      return xmlHostValue=centerValue;
    }
    
    if (xmlValue>0){
      xmlHostValue=(HostFloatType)(centerValue+0.05+(logValue-minAbsLogValue)/(maxPosLogValue-minAbsLogValue)*(1-centerValue-0.05));
    } else { // (xmlValue<0)
      xmlHostValue=(HostFloatType)(centerValue-0.05-(logValue-minAbsLogValue)/(maxNegLogValue-minAbsLogValue)*(centerValue-0.05));
    }
    
    if (xmlHostValue<0){
      xmlValue=-(PluginFloatType)(pow(10,(double)(maxNegLogValue/factor)));
      return xmlHostValue=(HostFloatType)(0.f);
    } else if (xmlHostValue>1){
      xmlValue=(PluginFloatType)(pow(10,(double)(maxPosLogValue/factor)));
      return xmlHostValue=(HostFloatType)(1.f);
    }
    return xmlHostValue;     
  }

  void saveXml(XmlElement *xml) const{
    if (Param::saveXmlFlag)
      xml->setAttribute(Param::getName(),(double)(*value));
  }

  LogWithSignParam(PluginProcessor *pluginProcessor, const String &name, const int globalIndex, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType * const value, const PluginFloatType minNegativeValue=(PluginFloatType)(-1),const PluginFloatType maxPositiveValue=(PluginFloatType)(1), const PluginFloatType minAbsValue=(PluginFloatType)(0.001),const PluginFloatType factor=(PluginFloatType)(1)):
  Param(pluginProcessor,name,globalIndex,automationFlag,loadSaveXmlFlag,"LogWithSign"),
  defaultValue(jmax<PluginFloatType>(minNegativeValue,jmin<PluginFloatType>(*value,maxPositiveValue))),
  maxNegLogValue((PluginFloatType)(factor*log10(-(double)minNegativeValue))),
  maxPosLogValue((PluginFloatType)(factor*log10((double)maxPositiveValue))),
  minAbsLogValue((PluginFloatType)(factor*log10((double)minAbsValue))),
  centerValue((HostFloatType)((factor*log10(-(double)minNegativeValue)+factor*log10((double)minAbsValue))/(factor*log10(-(double)minNegativeValue)+factor*log10((double)maxPositiveValue)+2*factor*log10((double)minAbsValue)))),
  value(value),
  factor(factor){   
    //minValue should be negative and maxValue positive, otherwise use LogWith0.
    jassert(minNegativeValue<0);
    jassert(maxPositiveValue>0);
    
    //log values are stricly positive, please define a strictly positive range
    jassert(minAbsValue>0);

    //force *value to the [minNegativeValue,maxPositiveValue] range
    xmlValue=*value=defaultValue;
  }
};

class IntParam : public Param{
private:
  PluginIntType minValue;
  PluginIntType maxValue;
  
  // (prevent copy constructor and operator= being generated..)
  // avoids warning C4512: "assignment operator could not be generated"
  IntParam (const IntParam&);
  IntParam& operator=(const IntParam &other);

protected:
  PluginIntType * const value;
  const PluginIntType defaultValue;
  PluginIntType xmlValue;   

public:
  bool writeXmlValue(){
    if (*value!=xmlValue){
      *value=xmlValue;
      return true;
    }
    return false;
  }
  
  bool hostSet(const HostFloatType hostValue){    
    PluginIntType oldValue=*value;
    if (hostValue>1)
      *value=maxValue;
    else if (hostValue<0)
      *value=minValue;
    else{
      *value=roundToInt<HostFloatType>(minValue+hostValue*(maxValue-minValue));
    }

    if (abs(*value-oldValue)>PluginParameters::epsilon)
		  return true;
		
		return false;
  }
  
  /** Updates the value from its UI denormalized value */ 
  void updateProcessorAndHostFromUi(const int valueArg);
  
  /** Returns the parameter value to set the UI */
  int uiGet() const{    
    if (*value<minValue)
      return (int)(minValue);
    else if (*value>maxValue)        
      return (int)(maxValue);
    else
      return (int)(*value);
  }

  HostFloatType hostGet() const{
    if (maxValue==minValue)
      return (HostFloatType)(0.f);
    HostFloatType newHostValue=(HostFloatType)(*value-minValue)/(maxValue-minValue);
    if (newHostValue<0){
      *value=minValue;
      return (HostFloatType)(0.f);
    } else if (newHostValue>1){
      *value=maxValue;
      return (HostFloatType)(1.f);
    } else{
      return newHostValue;
    }
  }
  
 void setMin(PluginIntType minValueArg){        
    if (defaultValue>=minValueArg){
      minValue=minValueArg;
      if (*value<minValueArg)
        *value=minValueArg;
      if (xmlValue<minValueArg)
        xmlValue=minValueArg;
    } else //minValue can't be greater than the (constant) default value
      jassertfalse;
  }

  void setMax(PluginIntType maxValueArg){    
    if (defaultValue<=maxValueArg){
      maxValue=maxValueArg;
      if (*value>maxValueArg)
        *value=maxValueArg;
      if (xmlValue>maxValueArg)
        xmlValue=maxValueArg;
    } else //maxValue can't be less than the (constant) default value
      jassertfalse;
  }

  const PluginIntType getDefaultValue() const{
    return defaultValue;
  }

  PluginIntType getValue() const{
    return *value;
  }
  
  const double getMin() const{
    return minValue;
  }

  const double getMax() const{
    return maxValue;
  }

  HostFloatType preLoadXml(XmlElement *xml){    
    if (xml==nullptr)
      xmlValue=defaultValue;
    else
      xmlValue=static_cast<PluginIntType>(xml->getIntAttribute(Param::getName(),defaultValue));    
    xmlHostValue=(HostFloatType)(xmlValue-minValue)/(maxValue-minValue);
    if (maxValue==minValue)
      return (HostFloatType)(0.f);
    if (xmlHostValue<0){
      xmlValue=minValue;
      return xmlHostValue=(HostFloatType)(0.f);
    }else if (xmlHostValue>1){
      xmlValue=maxValue;
      return xmlHostValue=(HostFloatType)(1.f);
    }else
      return xmlHostValue;
  }

  void saveXml(XmlElement *xml) const{
    if (Param::saveXmlFlag)
      xml->setAttribute(Param::getName(),(int)(*value));
  }
  
  IntParam(PluginProcessor *pluginProcessor, const String &name, const int globalIndex, const bool automationFlag, const bool loadSaveXmlFlag, PluginIntType * const value, const PluginIntType minValue=0,const PluginIntType maxValue=1):
  Param(pluginProcessor,name,globalIndex,automationFlag,loadSaveXmlFlag,"Int"),
  value(value),
  defaultValue(jmax<PluginIntType>(minValue,jmin<PluginIntType>(*value,maxValue))),
  minValue(minValue),
  maxValue(maxValue){
    //force *value to the [minValue,maxValue] range
    xmlValue=*value=defaultValue;
  }
};

class BoolParam : public Param{
  bool * const value;
  const bool defaultValue;
  bool xmlValue;
  
  // (prevent copy constructor and operator= being generated..)
  // avoids warning C4512: "assignment operator could not be generated"
  BoolParam (const BoolParam&);
  BoolParam& operator=(const BoolParam &other);  

public:
  bool writeXmlValue(){
    if (*value!=xmlValue){
      *value=xmlValue;
      return true;
    }
    return false;
  }
  
  bool hostSet(const HostFloatType hostValue){    
    bool oldValue=*value;
    *value=(hostValue>0.5)?true:false;

    if (*value!=oldValue)
		  return true;
		
		return false;
  }
  
  /** Updates the value from its UI denormalized value */ 
  void updateProcessorAndHostFromUi(const bool valueArg);
  
  /** Returns the parameter value to set the UI */
  bool uiGet() const{  
    return *value;
  }

  HostFloatType hostGet() const{
    return (*value)?(HostFloatType)(1.f):(HostFloatType)(0.f);
  }

  const bool getDefaultValue() const{
    return defaultValue;
  }
  
  bool getValue() const{
    return *value;
  }
  
  const double getMin() const{
    return 0;
  }

  const double getMax() const{
    return 1;
  }

  HostFloatType preLoadXml(XmlElement *xml){    
    if (xml==nullptr)
      xmlValue=defaultValue;
    else
      xmlValue=xml->getBoolAttribute(Param::getName(),defaultValue);
    xmlHostValue=(xmlValue)?(HostFloatType)(1.f):(HostFloatType)(0.f);
    return xmlHostValue;
  }

  void saveXml(XmlElement *xml) const{
    if (Param::saveXmlFlag)
      xml->setAttribute(Param::getName(),(*value)?String("true"):String("false"));
  }
  
  BoolParam(PluginProcessor *pluginProcessor, const String &name, const int globalIndex, const bool automationFlag, const bool loadSaveXmlFlag, bool * const value):
  Param(pluginProcessor,name,globalIndex,automationFlag,loadSaveXmlFlag,"Bool"),
  defaultValue(*value),
  xmlValue(*value),
  value(value){
  }

};

//-----------------------------------------------------------------------------------

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
       
  /** Offset of the paramList' index in the plugin's global list of 
      NON automated parameters */
  
  /** Counter of automated and non automated parameters. When a new paramter is added, 
      they are incremented accordingly. This is used to compute the parameter indexes in 
      the global list of automated or non automated parameters. */
  int numAutomatedParams;
  int numNonAutomatedParams;
  
  const String name;      
  
  bool updateUiFlag;  

  /** List of all parameters included in this group */
  OwnedArray<Param > paramList;
  
  /** List of all subgroups of parameters included in this group */
  OwnedArray<ParamGroup > paramGroupList;

  /** List of all ParamGroups that were allocated internally with a new command */
  OwnedArray<ParamGroup > unallocateAtDestructor;
  
  /** Adds a parameter to this group and checks its index in paramList. */
  void addParam(const int paramIndex,Param * const param){ 
    //Oh oh! You are not adding the parameters in the same order that you enumerated
    //their indexes. Please go and fix it.   
    if (paramIndex!=paramList.size()) {jassertfalse; return;}    
    paramList.add(param);    
  }  
  
   void addParamGroup(const int paramGroupIndex, ParamGroup *paramGroup){
    //Oh oh! You are not adding the parameter groups in the same order that you enumerated
    //their indexes. Please go and fix it.
    if (paramGroupIndex!=paramGroupList.size()) {jassertfalse; return;}
    
    paramGroup->setPluginProcessor(getProcessor());
    paramGroup->setParentParamGroup(this);
    paramGroup->setNumAutomatedParams(numAutomatedParams);
    paramGroup->setNumNonAutomatedParams(numNonAutomatedParams);
    
    paramGroup->init();
    paramGroupList.add(paramGroup);
    unallocateAtDestructor.add(paramGroup);
        
    numAutomatedParams=paramGroup->getNumAutomatedParams();
    numNonAutomatedParams=paramGroup->getNumNonAutomatedParams();      
  }   

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
  
  bool *saveXmlFlagCopy;  
  
public:
  /* Name of the file (without path) where saveXmlToDisk is recommended to write.
     It may read from its const String name (default) or from a StringParam 
     of this paramGroup (you will have to reimplement it). */
  virtual const String getDefaultXmlFileName(){
    return name;
  }

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

 /** Returns a string label for this ParamGroup */
 const String getName() const { return name; }  
  
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
  void setLoadXml(const bool enable, const bool recursively){
    for (int i=0;i<paramList.size();i++)
      paramList[i]->setLoadXml(enable);
      
    if (recursively){
      for (int i=0;i<paramGroupList.size();i++)
        paramGroupList[i]->setLoadXml(enable,recursively);
    }
  } 
  
  /** Export all its parameters to xml (set to true by default) when the user requests it */
  void setSaveXml(const bool enable, const bool recursively){
    for (int i=0;i<paramList.size();i++)
      paramList[i]->setSaveXml(enable);
    
    if (recursively){  
      for (int i=0;i<paramGroupList.size();i++)
        paramGroupList[i]->setSaveXml(enable,recursively);
    }
  }

  /** Used to submit a request for an update in the UI. updateUiRequested() must be called then from
      the UI to query its state.*/
  void requestUpdateUi(const bool enable){
    updateUiFlag=enable;
  }  
  
  /** Set the updateUi flags of all its parameters and parameter subgroups to true 
      recursively. */
  void requestUpdateUiRecursively(const bool enable){
    requestUpdateUi(enable);
    for (int i=0;i<paramList.size();i++)
      paramList[i]->requestUpdateUi(enable);      
    for (int g=0;g<paramGroupList.size();g++)
      paramGroupList[g]->requestUpdateUiRecursively(enable);
  }
  
  /** Called from the UI timer to determine if the general UI of this parameter group 
      (NOT the widgets of each parameter and all of the subgroups Uis contained in it) 
      must be updated  */
  bool updateUiRequested(){
    if (updateUiFlag){
      updateUiFlag=false;
      return true;
    }
    return false;
  }    
  
  bool updateUiIsOn(){
    return updateUiFlag;
  }    
     
  ParamGroup *getParamGroup(const int groupIndex) const{
    jassert(groupIndex>=0 && groupIndex<paramGroupList.size());
    return paramGroupList[groupIndex];
  }
    
  Param *getParam(const int index) const{
    jassert(index>=0 && index<paramList.size());
    return paramList[index];
  }
  
  StringParam *getStringParam(const int index) const{
    /** wrong index */
    jassert(index>=0 && index<paramList.size());
    /** You are trying to fetch a parameter with another type... 
        You want to use another method from the following list: 
        getStringParam(), getFloatParam(), getFloatParam(), getLogParam(), 
        getLogParam(), getIntParam(), getIntParam(), 
        getBoolParam()  */
    jassert(paramList[index]->getType()=="String");   
    return static_cast<StringParam *>(paramList[index]);
  }
  
  void addStringParam(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, String *const value){      
    if (automationFlag)
      addParam(paramIndex,new StringParam(pluginProcessor,name,numAutomatedParams++,automationFlag,loadSaveXmlFlag,value));
    else
      addParam(paramIndex,new StringParam(pluginProcessor,name,numNonAutomatedParams++,automationFlag,loadSaveXmlFlag,value));       
  }  

  void addStringParamArray(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, String* const values,int *const size,const int maxSize,bool saveOnlySizedArrayFlag=true,bool saveOnlyNonDefaultValuesFlag=true);

  void addStringParamMatrix(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, String** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true);   
  
  FloatParam *getFloatParam(const int index) const{
    /** wrong index */
    jassert(index>=0 && index<paramList.size());
    /** You are trying to fetch a parameter with another type... 
        You want to use another method from the following list: 
        getStringParam(), getFloatParam(), getFloatParam(), getLogParam(), 
        getLogParam(), getIntParam(), getIntParam(), 
        getBoolParam()  */
    jassert(paramList[index]->getType()=="Float");   
    return static_cast<FloatParam *>(paramList[index]);
  }
  
  void addFloatParam(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType *const value, const PluginFloatType minValue=(PluginFloatType)(0),const PluginFloatType maxValue=(PluginFloatType)(0)){
    if (automationFlag)
      addParam(paramIndex,new FloatParam(pluginProcessor,name,numAutomatedParams++,automationFlag,loadSaveXmlFlag,value,minValue,maxValue));
    else
      addParam(paramIndex,new FloatParam(pluginProcessor,name,numNonAutomatedParams++,automationFlag,loadSaveXmlFlag,value,minValue,maxValue));        
  }   

  void addFloatParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType* const values,int *const size,const int maxSize,const PluginFloatType minValue=(PluginFloatType)(0),const PluginFloatType maxValue=(PluginFloatType)(1), bool saveOnlySizedArrayFlag=true, bool saveOnlyNonDefaultValuesFlag=true);

  void addFloatParamMatrix(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols,const PluginFloatType minValue=(PluginFloatType)(0),const PluginFloatType maxValue=(PluginFloatType)(1), const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true);
  
  LogParam *getLogParam(const int index) const{
    /** wrong index */
    jassert(index>=0 && index<paramList.size());
    /** You are trying to fetch a parameter with another type... 
        You want to use another method from the following list: 
        getStringParam(), getFloatParam(), getFloatParam(), getLogParam(), 
        getLogParam(), getIntParam(), getIntParam(), 
        getBoolParam()  */
    jassert(paramList[index]->getType()=="Log");    
    return static_cast<LogParam *>(paramList[index]);
  }
   
   void addLogParam(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType *const value, const PluginFloatType minValue=(PluginFloatType)(0.001),const PluginFloatType maxValue=(PluginFloatType)(1),const PluginFloatType factor = (PluginFloatType)(1)){
    if (automationFlag)
      addParam(paramIndex,new LogParam(pluginProcessor,name,numAutomatedParams++,automationFlag,loadSaveXmlFlag,value,minValue,maxValue,factor));
    else
      addParam(paramIndex,new LogParam(pluginProcessor,name,numNonAutomatedParams++,automationFlag,loadSaveXmlFlag,value,minValue,maxValue,factor));        
  }   

  void addLogParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType* const values,int *const size,const int maxSize,const PluginFloatType minValue=(PluginFloatType)(0),const PluginFloatType maxValue=(PluginFloatType)(1), const PluginFloatType factor=(PluginFloatType)(1), bool saveOnlySizedArrayFlag=true, bool saveOnlyNonDefaultValuesFlag=true);  

  void addLogParamMatrix(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginFloatType minValue=(PluginFloatType)(0),const PluginFloatType maxValue=(PluginFloatType)(1), const PluginFloatType factor=(PluginFloatType)(1), const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true);  
  
  LogWith0Param *getLogWith0Param(const int index) const{
    /** wrong index */
    jassert(index>=0 && index<paramList.size());
    /** You are trying to fetch a parameter with another type... 
        You want to use another method from the following list: 
        getStringParam(), getFloatParam(), getFloatParam(), getLogParam(), 
        getLogParam(), getIntParam(), getIntParam(), 
        getBoolParam()  */
    jassert(paramList[index]->getType()=="LogWith0");    
    return static_cast<LogWith0Param *>(paramList[index]);
  }  
   
   void addLogWith0Param(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType *const value, const PluginFloatType minValue=(PluginFloatType)(0.001),const PluginFloatType maxValue=(PluginFloatType)(1),const PluginFloatType factor = (PluginFloatType)(1)){
    if (automationFlag)
      addParam(paramIndex,new LogWith0Param(pluginProcessor,name,numAutomatedParams++,automationFlag,loadSaveXmlFlag,value,minValue,maxValue,factor));
    else
      addParam(paramIndex,new LogWith0Param(pluginProcessor,name,numNonAutomatedParams++,automationFlag,loadSaveXmlFlag,value,minValue,maxValue,factor));        
  }

  void addLogWith0ParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType* const values,int *const size,const int maxSize,const PluginFloatType minValue=(PluginFloatType)(0),const PluginFloatType maxValue=(PluginFloatType)(1), const PluginFloatType factor=(PluginFloatType)(1), bool saveOnlySizedArrayFlag=true, bool saveOnlyNonDefaultValuesFlag=true);

  void addLogWith0ParamMatrix(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginFloatType minValue=(PluginFloatType)(0),const PluginFloatType maxValue=(PluginFloatType)(1), const PluginFloatType factor=(PluginFloatType)(1), const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true);
  
  LogWithSignParam *getLogWithSignParam(const int index) const{
    /** wrong index */
    jassert(index>=0 && index<paramList.size());
    /** You are trying to fetch a parameter with another type... 
        You want to use another method from the following list: 
        getStringParam(), getFloatParam(), getFloatParam(), getLogParam(), 
        getLogParam(), getIntParam(), getIntParam(), 
        getBoolParam()  */
    jassert(paramList[index]->getType()=="LogWithSign");    
    return static_cast<LogWithSignParam *>(paramList[index]);
  }  
   
   void addLogWithSignParam(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType *const value, const PluginFloatType minNegativeValue=(PluginFloatType)(-1),const PluginFloatType maxPositiveValue=(PluginFloatType)(1),const PluginFloatType minAbsValue=(PluginFloatType)(0.001),const PluginFloatType factor = (PluginFloatType)(1)){
   if (automationFlag)
    addParam(paramIndex,new LogWithSignParam(pluginProcessor,name,numAutomatedParams++,automationFlag,loadSaveXmlFlag,value,minNegativeValue,maxPositiveValue,minAbsValue,factor));
  else
    addParam(paramIndex,new LogWithSignParam(pluginProcessor,name,numNonAutomatedParams++,automationFlag,loadSaveXmlFlag,value,minNegativeValue,maxPositiveValue,minAbsValue,factor));
}  

  void addLogWithSignParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType* const values,int *const size,const int maxSize,const PluginFloatType minValue=(PluginFloatType)(0),const PluginFloatType maxValue=(PluginFloatType)(1), const PluginFloatType minAbsValue=(PluginFloatType)(0.001), const PluginFloatType factor=(PluginFloatType)(1), bool saveOnlySizedArrayFlag=true, bool saveOnlyNonDefaultValuesFlag=true);

  void addLogWithSignParamMatrix(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginFloatType minValue=(PluginFloatType)(0),const PluginFloatType maxValue=(PluginFloatType)(1), const PluginFloatType minAbsValue=(PluginFloatType)(0.001), const PluginFloatType factor=(PluginFloatType)(1), const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true);
  
  IntParam *getIntParam(const int index) const{
    /** wrong index */ 
    jassert(index>=0 && index<paramList.size());
    /** You are trying to fetch a parameter with another type... 
        You want to use another method from the following list: 
        getStringParam(), getFloatParam(), getFloatParam(), getLogParam(), 
        getLogParam(), getIntParam(), getIntParam(), 
        getBoolParam()  */
    jassert(paramList[index]->getType()=="Int");    
    return static_cast<IntParam *>(paramList[index]);
  }  
  
  void addIntParam(const int paramIndex,const String &name,const bool automationFlag, const bool loadSaveXmlFlag,PluginIntType *const value, const PluginIntType minValue=0,const PluginIntType maxValue=1){
    if (automationFlag)
      addParam(paramIndex,new IntParam(pluginProcessor,name,numAutomatedParams++,automationFlag,loadSaveXmlFlag,value,minValue,maxValue));
    else
      addParam(paramIndex,new IntParam(pluginProcessor,name,numNonAutomatedParams++,automationFlag,loadSaveXmlFlag,value,minValue,maxValue));
  }    

  void addIntParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginIntType* const values,int *const size,const int maxSize,const PluginIntType minValue=static_cast<PluginIntType>(0),const PluginIntType maxValue=static_cast<PluginIntType>(1), bool saveOnlySizedArrayFlag=true, bool saveOnlyNonDefaultValuesFlag=true);  

  void addIntParamMatrix(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginIntType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols,const PluginIntType minValue=static_cast<PluginIntType>(0),const PluginIntType maxValue=static_cast<PluginIntType>(1), const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true);
  
  BoolParam *getBoolParam(const int index) const{
    /** wrong index */
    jassert(index>=0 && index<paramList.size());
    /** You are trying to fetch a parameter with another type... 
        You want to use another method from the following list: 
        getStringParam(), getFloatParam(), getFloatParam(), getLogParam(), 
        getLogParam(), getIntParam(), getIntParam(), 
        getBoolParam()  */
    jassert(paramList[index]->getType()=="Bool");
    return static_cast<BoolParam *>(paramList[index]);
  }  
  
  void addBoolParam(const int paramIndex,const String &name,const bool automationFlag, const bool loadSaveXmlFlag, bool *const value){
    if (automationFlag)
      addParam(paramIndex,new BoolParam(pluginProcessor,name,numAutomatedParams++,automationFlag,loadSaveXmlFlag,value));
    else
      addParam(paramIndex,new BoolParam(pluginProcessor,name,numNonAutomatedParams++,automationFlag,loadSaveXmlFlag,value));
  }
  
  void addBoolParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, bool* const values,int *const size,const int maxSize,bool saveOnlySizedArrayFlag=true,bool saveOnlyNonDefaultValuesFlag=true);

  void addBoolParamMatrix(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, bool** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true);
    
  /** Adds a subgroup of parameters to this group and returns the number of parameters 
      in the list */
  void addParamGroup(const int paramGroupIndex, ParamGroup &paramGroup){
    //Oh oh! You are not adding the parameter groups in the same order that you enumerated
    //their indexes. Please go and fix it.
    if (paramGroupIndex!=paramGroupList.size()) {jassertfalse; return;}
    
    paramGroup.setPluginProcessor(getProcessor());
    paramGroup.setParentParamGroup(this);
    paramGroup.setNumAutomatedParams(numAutomatedParams);
    paramGroup.setNumNonAutomatedParams(numNonAutomatedParams);
    
    paramGroup.init();
    paramGroupList.add(&paramGroup);
        
    numAutomatedParams=paramGroup.getNumAutomatedParams();
    numNonAutomatedParams=paramGroup.getNumNonAutomatedParams();      
  }   

  /** Stores the parameter values as an XML attribute.
      If createChild is set to true it will create a child XML node 
      (you want to disable it at the root node). */
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool recursive){
    //create a child XmlElement for this ParamGroup if you are adding xml
    //contents to an already populated XmlElement.
    if (createChild) 
      xml=xml->createNewChildElement(getName());

    for (int i=0;i<getNumParams();i++)
      paramList[i]->saveXml(xml);
      
    if (recursive){
      for (int g=0;g<getNumParamGroups();g++)
        paramGroupList[g]->saveXml(xml,true,recursive);
    }
  } 
  
  /** Save this paramGroup to disk (as one XML file by default). */
  virtual bool saveXmlToDisk(const File &file,const String &dtdToUse=""){
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
  
  /** Preload Xml values from all parameters into undoRedo. It is not 
      advisable to run this on the Processing thread since it may
      take a little while to parse all this parameters from Xml 
      and normalize them. */
  virtual void preLoadXml(XmlElement *xml, const bool recursively){
    //this child couldn't be found
    if (xml==nullptr)
      return;
          
    Param *param;
    for (int i=0;i<getNumParams();i++){
      param=getParam(i);
      if (param->loadXmlIsOn()) param->preLoadXml(xml);      
    }      
    
    if (recursively){
      for (int g=0;g<getNumParamGroups();g++){
        XmlElement *childXml=xml->getChildByName(getParamGroup(g)->getName());
        getParamGroup(g)->preLoadXml(childXml,recursively);
      }
    }        
  }   
  
  /** Load this paramGroup from disk (from an XML file by default). */
  virtual bool preLoadXmlFromDisk(const File &file){  
    XmlDocument myDocument (file);
    ScopedPointer <XmlElement> xml(myDocument.getDocumentElement());
    if (xml->getTagName()!=getName()) { return false; }          
    preLoadXml(xml,true);
    return true;
  }
  
  /* Update all parameters from undoRedo after preLoadXml(...) has been called. 
     This should be considerably faster than preLoadXml so you can risk to 
     put it in the processing thread. */
  virtual void updateProcessorHostAndUiFromXml(const bool recursively,bool forceValueChanged, bool forceUpdateUi){              
    for (int i=0;i<getNumParams();i++){
      getParam(i)->updateProcessorHostAndUiFromXml(forceValueChanged,forceUpdateUi);      
    }  
    
    if (recursively){
      for (int g=0;g<getNumParamGroups();g++){
        getParamGroup(g)->updateProcessorHostAndUiFromXml(recursively,forceValueChanged,forceUpdateUi);
      }
    }        
  }  
  
  /** Implements what is executed after a parameter is updated with a different value
      inside of the children paramGroup.
      This method will be called by the host, probably on the audio thread, so
      it's absolutely time-critical. Don't use critical sections or anything
      UI-related, or anything at all that may block in any way! */
  virtual void runAfterParamGroupChange(ParamGroup * /*paramGroup*/, int /*paramIndex*/, UpdateFromFlags /*updateFromFlag*/) {};
  
  /** Implements what is executed after a parameter is updated with a different value.
      This method will be called by the host, probably on the audio thread, so
      it's absolutely time-critical. Don't use critical sections or anything
      UI-related, or anything at all that may block in any way! */
  virtual void runAfterParamChange(int /*paramIndex*/, UpdateFromFlags /*updateFromFlag*/) {};
  
  /** All parameters and parameter groups must be added in this method */
  virtual void init() = 0;
  
  /** Notify the host about all the parameters in this paramGroup and update the UI. This is 
      useful when you change the parameters of this paramGroup (maybe several times) without
      notifying the Host and the UI (without using update(...)) and at the end of the process 
      you want to update the Host and the UI from all of them at once */
  void updateHostForAll(bool runAfterParamChange,UpdateFromFlags updateFromFlag){
    for (int paramIndex=0;paramIndex<getNumParams();paramIndex++)
      getParam(paramIndex)->updateHost(runAfterParamChange,updateFromFlag);
  }
  
  ParamGroup(const String &name):
    parentParamGroup(nullptr),
    numAutomatedParams(0),
    numNonAutomatedParams(0),
    name(name),
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
    paramList.clear(true);
    paramGroupList.clear(false);
    unallocateAtDestructor.clear(true);

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
  PluginIntType *const size;    
  const int maxSize;
  const bool automationFlag;
  const bool loadSaveXmlFlag;
  const bool saveOnlySizedArrayFlag;      
  const bool saveOnlyNonDefaultValuesFlag;    

public:      
  virtual void init() = 0;
  
  /** Returns the size of the "visible" array */
  PluginIntType getSize() const{
	  return *size;
  }
  
  /** Returns the maximum (allocated) size of the array */
  PluginIntType getMaxSize() const{
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
    
  ParamArray(const String &name,const bool automationFlag, const bool loadSaveXmlFlag, int *const size, const int maxSize, bool saveOnlySizedArrayFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
  ParamGroup(name),
  size(size),
  maxSize(maxSize),  
  automationFlag(automationFlag), 
  loadSaveXmlFlag(loadSaveXmlFlag), 
  saveOnlySizedArrayFlag(saveOnlySizedArrayFlag),
  saveOnlyNonDefaultValuesFlag(saveOnlyNonDefaultValuesFlag)  
  {
    saveXmlFlagCopy=new bool[maxSize];
  }    
  
};

/** ParamGroup containing an array of FloatParams. */
class FloatParamArray : public ParamArray{
private:
  PluginFloatType* const values;
  PluginFloatType minValue;
  PluginFloatType maxValue;

public: 
  
  void init(){
    for (int i=0;i<ParamArray::getMaxSize();i++){
      ParamGroup::addFloatParam(i,(String)(i),ParamArray::automationFlag,ParamArray::loadSaveXmlFlag,&(values[i]),minValue,maxValue);
    }
  }

  /** Returns the value of position i in the array */
  PluginFloatType getValue(int i) const{
	  if (i>=0 && i<*ParamArray::size && i<ParamArray::getMaxSize())
		  return values[i];
	  else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(PluginFloatType minValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getFloatParam(i)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(PluginFloatType maxValueArg){
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
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool recursively){
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
    
    ParamGroup::saveXml(xml,createChild,recursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamArray::maxSize;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  }
  
  FloatParamArray(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType* const values,int *const size,const int maxSize,const PluginFloatType minValue=(PluginFloatType)(0),const PluginFloatType maxValue=(PluginFloatType)(1), bool saveOnlySizedArrayFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
  ParamArray(name,automationFlag,loadSaveXmlFlag,size,maxSize,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag),
  values(values),
  minValue(minValue),
  maxValue(maxValue)
  {} 
};

/** ParamGroup containing an array of LogParams. */
class LogParamArray : public ParamArray{
private:
  PluginFloatType* const values;
  PluginFloatType minValue;
  PluginFloatType maxValue;
  const PluginFloatType factor;
  
public: 
  
  void init(){
    for (int i=0;i<ParamArray::getMaxSize();i++){
      ParamGroup::addLogParam(i,(String)(i),ParamArray::automationFlag,ParamArray::loadSaveXmlFlag,&(values[i]),minValue,maxValue,factor);
    }
  }

  /** Returns the value of position i in the array */
  PluginFloatType getValue(int i) const{
	  if (i>=0 && i<*ParamArray::size && i<ParamArray::getMaxSize())
		  return values[i];
	  else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(PluginFloatType minValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getLogParam(i)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(PluginFloatType maxValueArg){
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
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool recursively){
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
    
    ParamGroup::saveXml(xml,createChild,recursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamArray::maxSize;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  }
  
  LogParamArray(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType* const values,int *const size,const int maxSize,const PluginFloatType minValue=(PluginFloatType)(0),const PluginFloatType maxValue=(PluginFloatType)(1), const PluginFloatType factor=(PluginFloatType)(1), bool saveOnlySizedArrayFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
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
  PluginFloatType* const values;
  const PluginFloatType factor;
  PluginFloatType minValue;
  PluginFloatType maxValue;

public: 
  
  void init(){
    for (int i=0;i<ParamArray::getMaxSize();i++){
      ParamGroup::addLogWith0Param(i,(String)(i),ParamArray::automationFlag,ParamArray::loadSaveXmlFlag,&(values[i]),minValue,maxValue,factor);
    }
  }

  /** Returns the value of position i in the array */
  PluginFloatType getValue(int i) const{
	  if (i>=0 && i<*ParamArray::size && i<ParamArray::getMaxSize())
		  return values[i];
	  else return 0;
  }    
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(PluginFloatType minValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getLogWith0Param(i)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(PluginFloatType maxValueArg){
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
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool recursively){
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
    
    ParamGroup::saveXml(xml,createChild,recursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamArray::maxSize;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  }
  
  LogWith0ParamArray(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType* const values,int *const size,const int maxSize,const PluginFloatType minValue=(PluginFloatType)(0),const PluginFloatType maxValue=(PluginFloatType)(1), const PluginFloatType factor=(PluginFloatType)(1), bool saveOnlySizedArrayFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
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
  PluginFloatType* const values;
  const PluginFloatType factor;
  PluginFloatType minValue;
  PluginFloatType maxValue;
  PluginFloatType minAbsValue;

public: 
  
  void init(){
    for (int i=0;i<ParamArray::getMaxSize();i++){
      ParamGroup::addLogWithSignParam(i,(String)(i),ParamArray::automationFlag,ParamArray::loadSaveXmlFlag,&(values[i]),minValue,maxValue,minAbsValue,factor);
    }
  }

  /** Returns the value of position i in the array */
  PluginFloatType getValue(int i) const{
	  if (i>=0 && i<*ParamArray::size && i<ParamArray::getMaxSize())
		  return values[i];
	  else return 0;
  }
 
  /** Sets the minimum range of each parameter in the array */
  void setMin(PluginFloatType minValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getLogWithSignParam(i)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(PluginFloatType maxValueArg){
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
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool recursively){
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
    
    ParamGroup::saveXml(xml,createChild,recursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamArray::maxSize;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  }
  
  LogWithSignParamArray(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType* const values,int *const size,const int maxSize,const PluginFloatType minValue=(PluginFloatType)(0),const PluginFloatType maxValue=(PluginFloatType)(1), const PluginFloatType minAbsValue=(PluginFloatType)(0.001), const PluginFloatType factor=(PluginFloatType)(1), bool saveOnlySizedArrayFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
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
  PluginIntType* const values;
  PluginIntType minValue;
  PluginIntType maxValue;

public: 
  
  void init(){
    for (int i=0;i<ParamArray::getMaxSize();i++){
      ParamGroup::addIntParam(i,(String)(i),ParamArray::automationFlag,ParamArray::loadSaveXmlFlag,&(values[i]),minValue,maxValue);
    }
  } 

  /** Returns the value of position i in the array */
  PluginIntType getValue(int i) const{
	  if (i>=0 && i<*ParamArray::size && i<ParamArray::getMaxSize())
		  return values[i];
	  else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(PluginIntType minValueArg){
    for (int i=0;i<ParamArray::getMaxSize();i++)
      ParamGroup::getIntParam(i)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(PluginIntType maxValueArg){
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
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool recursively){
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
    
    ParamGroup::saveXml(xml,createChild,recursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<ParamArray::maxSize;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  }
  
  IntParamArray(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginIntType* const values,int *const size,const int maxSize,const PluginIntType minValue=static_cast<PluginIntType>(0),const PluginIntType maxValue=static_cast<PluginIntType>(1), bool saveOnlySizedArrayFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
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
  
  void init(){
    for (int i=0;i<ParamArray::getMaxSize();i++){
      ParamGroup::addBoolParam(i,(String)(i),ParamArray::automationFlag,ParamArray::loadSaveXmlFlag,&(values[i]));
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
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool recursively){
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
    
    ParamGroup::saveXml(xml,createChild,recursively);
    
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
  
  void init(){
    for (int i=0;i<ParamArray::getMaxSize();i++){
      ParamGroup::addStringParam(i,(String)(i),ParamArray::automationFlag,ParamArray::loadSaveXmlFlag,&(values[i]));
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
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool recursively){
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
    
    ParamGroup::saveXml(xml,createChild,recursively);
    
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
  PluginIntType *const numRows;
  PluginIntType *const numCols;
  const int maxRows;
  const int maxCols;      
  const bool automationFlag;  
  const bool loadSaveXmlFlag;  
  bool saveOnlySizedMatrixFlag;
  bool saveOnlyNonDefaultValuesFlag;
  
public:    
  virtual void init() = 0;
  
  /** Returns the number of rows of the "visible" array */
  PluginIntType getNumRows() const{
	  return *numRows;
  }
  
  /** Returns the number of columns of the "visible" array */
  PluginIntType getNumCols() const{
	  return *numCols;
  }
  
  /** Returns the maximum (allocated) size of the array */
  PluginIntType getMaxRows() const{
	  return maxRows;
	}
	
	PluginIntType getMaxCols() const{
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
  void updateHostAndUiRow(int row,bool forceValueChanged,UpdateFromFlags updateFromFlag){
    for (int col=0;col<getNumCols();col++)
      getParam(row*getNumCols()+col)->updateHost(forceValueChanged,updateFromFlag);
  }
  
  /** Update the host and the UI about all parameters in this column */
  void updateHostAndUiCol(int col,bool forceValueChanged,UpdateFromFlags updateFromFlag){
    for (int row=0;row<getNumRows();row++)
      getParam(row*getNumCols()+col)->updateHost(forceValueChanged,updateFromFlag);
  }
  
  ParamMatrix(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, int *const numRows, int *const numCols, const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
  ParamGroup(name),
  numRows(numRows),
  numCols(numCols),
  maxRows(maxRows),  
  maxCols(maxCols),
  automationFlag(automationFlag),
  loadSaveXmlFlag(loadSaveXmlFlag),
  saveOnlySizedMatrixFlag(saveOnlySizedMatrixFlag),
  saveOnlyNonDefaultValuesFlag(saveOnlyNonDefaultValuesFlag)
  {
    saveXmlFlagCopy=new bool[maxCols*maxRows];
  } 
};

/** ParamGroup containing a matrix of FloatParams. */
class FloatParamMatrix : public ParamMatrix{
private:
  PluginFloatType** const values;
  PluginFloatType minValue;
  PluginFloatType maxValue;
public: 
  
  void init(){
    for (int i=0;i<ParamMatrix::getMaxRows();i++){
      for (int j=0;j<ParamMatrix::getMaxCols();j++){
        ParamGroup::addFloatParam(i*ParamMatrix::getMaxCols()+j,(String)(i)+":"+(String)j,ParamMatrix::automationFlag,ParamMatrix::loadSaveXmlFlag,&(values[i][j]),minValue,maxValue);        
      }
    }
  }

  /** Returns the value of position i,j in the array */
  PluginFloatType getValue(int i,int j) const{
	  if (i>=0 && i<ParamMatrix::getNumRows() && i<ParamMatrix::getMaxRows()
	      && j>=0 && j<ParamMatrix::getNumCols() && j<ParamMatrix::getMaxCols())
		  return values[i][j];
	  else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(PluginFloatType minValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getFloatParam(i*ParamMatrix::getMaxCols()+j)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(PluginFloatType maxValueArg){
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
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool recursively){
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
    
    ParamGroup::saveXml(xml,createChild,recursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<maxRows*maxCols;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  }
  
  FloatParamMatrix(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols,const PluginFloatType minValue=(PluginFloatType)(0),const PluginFloatType maxValue=(PluginFloatType)(1), const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
  ParamMatrix(name,automationFlag,loadSaveXmlFlag,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag),
  values(values),
  minValue(minValue),
  maxValue(maxValue)
  {} 
};

/** ParamGroup containing a matrix of LogParams. */
class LogParamMatrix : public ParamMatrix{
private:
  PluginFloatType** const values;
  const PluginFloatType factor;
  PluginFloatType minValue;
  PluginFloatType maxValue;
  bool sparseCompressionFlag; 
  PluginFloatType mostProbableValue;

public: 
  
  void init(){
    for (int i=0;i<ParamMatrix::getMaxRows();i++){
      for (int j=0;j<ParamMatrix::getMaxCols();j++){
        ParamGroup::addLogParam(i*ParamMatrix::getMaxCols()+j,(String)(i)+":"+(String)j,ParamMatrix::automationFlag,ParamMatrix::loadSaveXmlFlag,&(values[i][j]),minValue,maxValue,factor);        
      }
    }
  }

  /** Returns the value of position i,j in the array */
  PluginFloatType getValue(int i,int j) const{
	  if (i>=0 && i<ParamMatrix::getNumRows() && i<ParamMatrix::getMaxRows()
	      && j>=0 && j<ParamMatrix::getNumCols() && j<ParamMatrix::getMaxCols())
		  return values[i][j];
	  else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(PluginFloatType minValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getLogParam(i*ParamMatrix::getMaxCols()+j)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(PluginFloatType maxValueArg){
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
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool recursively){
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
    
    ParamGroup::saveXml(xml,createChild,recursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<maxRows*maxCols;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  }
  
  LogParamMatrix(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginFloatType minValue=(PluginFloatType)(0),const PluginFloatType maxValue=(PluginFloatType)(1), const PluginFloatType factor=(PluginFloatType)(1), const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
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
  PluginFloatType** const values;
  const PluginFloatType factor;
  PluginFloatType minValue;
  PluginFloatType maxValue;
  bool sparseCompressionFlag; 
  PluginFloatType mostProbableValue;

public: 
  
  void init(){
    for (int i=0;i<ParamMatrix::getMaxRows();i++){
      for (int j=0;j<ParamMatrix::getMaxCols();j++){
        ParamGroup::addLogWith0Param(i*ParamMatrix::getMaxCols()+j,(String)(i)+":"+(String)j,ParamMatrix::automationFlag,ParamMatrix::loadSaveXmlFlag,&(values[i][j]),minValue,maxValue,factor);        
      }
    }
  }

  /** Returns the value of position i,j in the array */
  PluginFloatType getValue(int i,int j) const{
	  if (i>=0 && i<ParamMatrix::getNumRows() && i<ParamMatrix::getMaxRows()
	      && j>=0 && j<ParamMatrix::getNumCols() && j<ParamMatrix::getMaxCols())
		  return values[i][j];
	  else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(PluginFloatType minValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getLogWith0Param(i*ParamMatrix::getMaxCols()+j)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(PluginFloatType maxValueArg){
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
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool recursively){
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
    
    ParamGroup::saveXml(xml,createChild,recursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<maxRows*maxCols;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  }
  
  LogWith0ParamMatrix(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginFloatType minValue=(PluginFloatType)(0),const PluginFloatType maxValue=(PluginFloatType)(1), const PluginFloatType factor=(PluginFloatType)(1), const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
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
  PluginFloatType** const values;
  const PluginFloatType factor;
  PluginFloatType minValue;
  PluginFloatType maxValue;
  PluginFloatType minAbsValue;
  bool sparseCompressionFlag; 
  PluginFloatType mostProbableValue;

public: 
  
  void init(){
    for (int i=0;i<ParamMatrix::getMaxRows();i++){
      for (int j=0;j<ParamMatrix::getMaxCols();j++){
        ParamGroup::addLogWithSignParam(i*ParamMatrix::getMaxCols()+j,(String)(i)+":"+(String)j,ParamMatrix::automationFlag,ParamMatrix::loadSaveXmlFlag,&(values[i][j]),minValue,maxValue,minAbsValue,factor);        
      }
    }
  }

  /** Returns the value of position i,j in the array */
  PluginFloatType getValue(int i,int j) const{
	  if (i>=0 && i<ParamMatrix::getNumRows() && i<ParamMatrix::getMaxRows()
	      && j>=0 && j<ParamMatrix::getNumCols() && j<ParamMatrix::getMaxCols())
		  return values[i][j];
	  else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(PluginFloatType minValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getLogWithSignParam(i*ParamMatrix::getMaxCols()+j)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(PluginFloatType maxValueArg){
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
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool recursively){
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
    
    ParamGroup::saveXml(xml,createChild,recursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<maxRows*maxCols;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  }
  
  LogWithSignParamMatrix(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginFloatType minValue=(PluginFloatType)(0),const PluginFloatType maxValue=(PluginFloatType)(1), const PluginFloatType minAbsValue=(PluginFloatType)(0.001), const PluginFloatType factor=(PluginFloatType)(1), const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
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
  PluginIntType** const values;  
  PluginIntType minValue;
  PluginIntType maxValue;

public: 
  
  void init(){
    for (int i=0;i<ParamMatrix::getMaxRows();i++){
      for (int j=0;j<ParamMatrix::getMaxCols();j++){
        ParamGroup::addIntParam(i*ParamMatrix::getMaxCols()+j,(String)(i)+":"+(String)j,ParamMatrix::automationFlag,ParamMatrix::loadSaveXmlFlag,&(values[i][j]),minValue,maxValue);        
      }
    }
  }

  /** Returns the value of position i,j in the array */
  PluginIntType getValue(int i,int j) const{
	  if (i>=0 && i<ParamMatrix::getNumRows() && i<ParamMatrix::getMaxRows()
	      && j>=0 && j<ParamMatrix::getNumCols() && j<ParamMatrix::getMaxCols())
		  return values[i][j];
	  else return 0;
  }  
  
  /** Sets the minimum range of each parameter in the array */
  void setMin(PluginIntType minValueArg){
    for (int i=0;i<ParamMatrix::getMaxRows();i++)
      for (int j=0;j<ParamMatrix::getMaxCols();j++)
        ParamGroup::getIntParam(i*ParamMatrix::getMaxCols()+j)->setMin(minValueArg);
  }

  /** Sets the maximum range of each parameter in the array */
  void setMax(PluginIntType maxValueArg){
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
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool recursively){
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
    
    ParamGroup::saveXml(xml,createChild,recursively);
    
    if (saveXmlFlagCopy){
      for (int i=0;i<maxRows*maxCols;i++){
        getParam(i)->setSaveXml(saveXmlFlagCopy[i]);
      }
    }
  } 
  
  IntParamMatrix(const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginIntType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols,const PluginIntType minValue=static_cast<PluginIntType>(0),const PluginIntType maxValue=static_cast<PluginIntType>(1), const bool saveOnlySizedMatrixFlag=true, bool saveOnlyNonDefaultValuesFlag=true):
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
  
  void init(){
    for (int i=0;i<ParamMatrix::getMaxRows();i++){
      for (int j=0;j<ParamMatrix::getMaxCols();j++){
        ParamGroup::addBoolParam(i*ParamMatrix::getMaxCols()+j,(String)(i)+":"+(String)j,ParamMatrix::automationFlag,ParamMatrix::loadSaveXmlFlag,&(values[i][j]));
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
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool recursively){
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
    
    ParamGroup::saveXml(xml,createChild,recursively);
    
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
  
  void init(){
    for (int i=0;i<ParamMatrix::getMaxRows();i++){
      for (int j=0;j<ParamMatrix::getMaxCols();j++){
        ParamGroup::addStringParam(i*ParamMatrix::getMaxCols()+j,(String)(i)+":"+(String)j,ParamMatrix::automationFlag,ParamMatrix::loadSaveXmlFlag,&(values[i][j]));
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
  virtual void saveXml(XmlElement *xml, const bool createChild, const bool recursively){
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
    
    ParamGroup::saveXml(xml,createChild,recursively);
    
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


//-----------------------------------------------------------------------------------

/**  methods to redirect all parameters to the global list referred by the host        
     and handle automatically automated and not automated parameters */
class PluginProcessor : public AudioProcessor, public ParamGroup{
private:
  ParamGroup **groupAutomated;
  int *indexInGroupAutomated;
  ParamGroup **groupNonAutomated;
  int *indexInGroupNonAutomated;  
   
  /** compues recursively all the mappings between the global index 
    of parameters and their ParamGroup and index in the ParamGroup */
  void mapGlobalIndex(ParamGroup *root, bool countIfAutomate, ParamGroup **map,int *indexMap)  {
    for (int i=0;i<root->getNumParams();i++){      
      Param* param=root->getParam(i);
      if (param->automationIsOn()==countIfAutomate){
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
  
  bool acceptsMidi() const
  {
  #if JucePlugin_WantsMidiInput
      return true;
  #else
      return false;
  #endif
  }

  bool producesMidi() const
  {
  #if JucePlugin_ProducesMidiOutput
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
    int paramIndex;
    if (param->automationIsOn()){ //if automated, notify host     
                                  //and get corresponding localParamGroup and paramIndex 
      //notify host      
      sendParamChangeMessageToListeners (globalIndex, newValue);      
      localParamGroup=groupAutomated[globalIndex];
      paramIndex=indexInGroupAutomated[globalIndex];
      
    } else { //if not automated skip host notification
             // but get corresponding localParamGroup and paramIndex
      localParamGroup=groupNonAutomated[globalIndex];
      paramIndex=indexInGroupNonAutomated[globalIndex];
    }
    
    if (runAfterParamChange){
      //"runAfterParamChange" defined in its ParamGroup
      localParamGroup->runAfterParamChange(paramIndex,param->getUpdateFromFlag());
      //"runAfterParamGroupChange" defined in its parent ParamGroup
      if (localParamGroup->getParentParamGroup()!=nullptr)
        localParamGroup->getParentParamGroup()->runAfterParamGroupChange(localParamGroup,paramIndex,param->getUpdateFromFlag());
    }
    if (updateUi){
      param->requestUpdateUi(true);
    }
    
  }
  
  int getNumParameters(){
    return getNumAutomatedParams();
  }
  
  const String getParameterName (int index){
    if (index>=0 && index<getNumAutomatedParams())
      return groupAutomated[index]->getParam(indexInGroupAutomated[index])->getName(); 
    else   
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
      ParamGroup * const paramGroup=groupAutomated[index];
      const int groupIndex=indexInGroupAutomated[index];
      Param * const param=paramGroup->getParam(groupIndex);      
      if (param->hostSet(newValue)){
        param->requestUpdateUi(true);
        paramGroup->runAfterParamChange(groupIndex,param->getUpdateFromFlag());
      }      
    } 
  }  
   
  /** generates the mappings between all parameters in ParamGroups and 
    the global list of parameters, by looking at paramGroup and
    all its children */
  void initAll(){
    setPluginProcessor(this);
    init();

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