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

#ifndef __PLUGINPARAMETERS_PARAMS_HEADER__
#define __PLUGINPARAMETERS_PARAMS_HEADER__

using namespace juce;
  
class PluginProcessor;

class ParamGroup;

/** UpdateFromFlags are a set of flags that describe the origin of a value update which can
    be triggered by the Host, the Processing thread or the User Interface:

    * UPDATE_FROM_HOST in only set by default when the host sends automation changes.
    * UPDATE_FROM_UI is meant to indicate when the value is changed from the User Interface.
      It is set when the following methods are called: 
      getXXXParam(paramIndex)->updateProcessorAndHostFromUi(...), 
      getParam(paramIndex)->updateProcessorHostAndUi(...,UPDATE_FROM_UI), 
      getParam(paramIndex)->updateHostAndUi(...,UPDATE_FROM_UI) 
      or getParam(paramIndex)-> updateHost(...,UPDATE_FROM_UI).
    * UPDATE_FROM_PROCESSOR is meant to indicate when the value is changed in the processing thread. 
      It is set when the following methods are called: 
      getParam(paramIndex)->updateProcessorHostAndUi(...,UPDATE_FROM_PROCESSOR), 
      getParam(paramIndex)->updateHostAndUi(...,UPDATE_FROM_PROCESSOR) 
      or getParam(paramIndex)-> updateHost(...,UPDATE_FROM_PROCESSOR).    
*/
enum UpdateFromFlags{
  UPDATE_FROM_HOST=0x01,
  UPDATE_FROM_UI=0x02,
  UPDATE_FROM_PROCESSOR=0x04
}; 

enum LoadSaveOptions{
  DONT_SAVE=0,
  SAVE, 
  SAVE_ONLY_IN_SESSION,
  SAVE_ONLY_IN_PRESETS
};

enum XmlType{
  SESSION=0,
  PRESET
};

/** Base class for all parameters. Distinctions by type are made below. */
class Param{
private:            
  /** This flag determines if a parameter will be automated in the host.  */  
  const bool registerAtHostFlag;

  const String name;

  String xmlName;

  bool updateUiFlag;

  friend class ParamGroup;

  int globalIndex;

  UpdateFromFlags updateFromFlag;    

  /** Pointer to the AudioProcessor class to be able notify parameter changes to the 
      host with PluginProcessor::updateHostAndUi(...) */
  PluginProcessor* pluginProcessor;
    
public:     
  /**Advanced settings:

    loadFromSession:                    Try to load this parameter value from the plugin 
                                        session saved in the host.

    saveToSession:                      Save this parameter value to the plugin session 
                                        saved in the host.

    loadFromPresets:                    Try to load this parameter value from the plugin 
                                        presets (managed using the PluginParameters module).

    saveToPresets:                      Save this parameter value to the plugin presets 
                                        (managed using the PluginParameters module).
    
    forceRunAfterParamChangeInHost:     if enabled runAfterParamChange(...) will *always* 
                                        be called when an (automated) parameter is updated 
                                        from the host. Otherwise it will be called only
                                        if the parameter value changes.
    
    createThreadForParamChange: runAfterParamChange(...) is usually run in the UI 
                                        thread if updateProcessorAndHostFromUi(...) is called
                                        or in the processing thread (uh uh!) if the host sends a new
                                        value for it. Specially in the latter case, it is important
                                        to make sure that we are not doing anything to block it. 
                                        If that were the case (e.g. reading a new preset from
                                        from disk), please enable this option to create a new thread.
 
    autoChangeGestures                  If  true, beginParameterChangeGesture() and endParameterChangeGesture() 
                                        are called before and after (respectively) of each host update.
                                        Otherwise you should call them manually (e.g. at sliderDragStarted() 
                                        and sliderDragEnded()). Without them hosts like Logic won't receive the 
                                        value updates.
  */
  enum Options{
    loadFromSession=0,
    saveToSession,
    loadFromPresets,
    saveToPresets,
    saveOnlyNonDefaultValues,
    forceRunAfterParamChangeInHost,
    createThreadForParamChange,
    autoChangeGestures,
    numOptions
  };

protected:
  /** Stores all settings for this parameter */
  bool settings[numOptions];

  /** if true readXml(...) tells updateProcessorFromXml(...) to update *value according to xmlValue */
  bool updateXml;

  PluginParameters_HostFloatType xmlHostValue;        

  /** Update the host with a normalized value, set the UpdateFrom flag to UPDATE_FROM_UI 
      and skip the UI update */
  void updateProcessorHostFromUi(PluginParameters_HostFloatType newValue);  

  // (prevent copy constructor and operator= being generated..)
  // avoids warning C4512: "assignment operator could not be generated"
  Param (const Param&);
  Param& operator=(const Param &other);

public:
  /** Check if a specific setting is set. Refer to Param::Options to see what options are available. */
  void setOption(const Options settingId,const bool newValue){
    settings[settingId]=newValue;
  }

  /* Set a new setting. Refer to Param::Options to see what options are available. */
  bool getOption(const Options settingId) const{
    return settings[settingId];
  }
 
  /** returns true if this parameter is registered at the host */
  bool registeredAtHost() const { return registerAtHostFlag; }

  /** Index in the global list of automated parameters or the list of non automated parameters
      (if registerAtHostFlag==true or registerAtHostFlag==false respectively) */
  int getGlobalIndex() const { return globalIndex; } 
        
  /** Returns a string label for this parameter */
  const String getName() const { return name; }

  /** Returns the attribute name under which this parameter is stored in XML.
      xmlName is set to name by default but you can change it with setXmlName(...). 
      See readXml(...) and saveXml(...) for more details. */ 
  const String getXmlName() const { return xmlName; }

  /** Sets the attribute name under which this parameter is stored in XML 
      xmlName is set to name by default. 
      See readXml(...) and saveXml(...) for more details. */ 
  void setXmlName(const String xmlNameArg){
    xmlName=xmlNameArg;
  }
    
  /** Submit a request for an update in the UI. updateUiRequested() must be called then from
      the UI to query its state. */
  void updateUi(const bool request=true){
    updateUiFlag=request;
  }

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
    
  /**  Returns the parameter value to set the host. */
  virtual PluginParameters_HostFloatType hostGet() const = 0;  

  /** Preloads an XML attribute into the auxiliary internal variable xmlValue.       
      If nothing can't be read from the XML it is set to the parameter default value.
      Note that it doesn't change the parameter value. 
      Call updateProcessorHostAndUiFromXml(...) to update it. */
  virtual void readXml(XmlElement *xml, XmlType xmlType) = 0;

  /** Returns the current updateFrom flag */
  UpdateFromFlags getUpdateFromFlag(){
    return updateFromFlag;
  }  
    
  /** Reset flags used in runAfterParamChange(...) to determine the origin and reason of the update */    
  void resetUpdateFromFlag(){
    updateFromFlag=UPDATE_FROM_HOST;    
  }    

  /** Returns the minimum range of this parameter */  
  virtual const double getMin() const = 0;
    
  /** Returns the maximum range of this parameter */  
  virtual const double getMax() const = 0;

  /** Stores the parameter value as an XML attribute */
  virtual void saveXml(XmlElement *xml, XmlType xmlType) const = 0;      
    
  /**  Sets the parameter from a given value from the host.
        returns true: if it is set to a new value or forceLoad,skipLoad is enabled.
              false: if it set to the same value. */
  virtual bool hostSet(const PluginParameters_HostFloatType hostValue) = 0;  
    
  /** Set a parameter from the Processor to a new value and notify the host and 
      the UI (when it has changed) */  
  void updateProcessorHostAndUi(PluginParameters_HostFloatType newValue, const UpdateFromFlags updateFromFlag=UPDATE_FROM_PROCESSOR);  
    
  /** Notify the host about the current value of a parameter and update the UI.
      This is useful when you change the value of this parameter (maybe several times) 
      and you don't want to notify the host and the UI until the end. */  
  void updateHostAndUi(bool runAfterParamChange, UpdateFromFlags updateFromFlag=UPDATE_FROM_PROCESSOR);
      
  /** Notify the host about the current value of a parameter without updating the UI. 
      This is useful when you change the value of this parameter (maybe several times) 
      and you don't want to notify the host until the end. */  
  void updateHost(bool runAfterParamChange, UpdateFromFlags updateFromFlag=UPDATE_FROM_PROCESSOR);
    
  /** Update value with the xmlValue read with readXml and return true if it was different */
  virtual bool updateProcessorFromXml() = 0;

  /** Update the parameter value from the value loaded from Xml (stored in the variable
      xmlValue) and notify the host and the UI (if it has changed). */
  void updateProcessorHostAndUiFromXml(bool forceUpdateHost=false,bool forceUpdateUi=false);

  /** Update value with the defaultValue and return true if it was different */
  virtual bool updateProcessorFromDefaultXml() = 0;

  /** Update the parameter value from the default value (stored in the variable
      defaultValue) and notify the host and the UI (if it has changed). */
  void updateProcessorHostAndUiFromDefaultXml(bool forceRunAfterParamChange=false,bool forceUpdateUi=false);

  /* Hosts like Logic require that you indicate when a parameter starts a gesture (changes). 
     Use this method indicate the start of a gesture for this parameter. 
     However bear in mind that Options::autoChangeGestures is set to true by default so
     you won't need to call it manually unless you disable it first using:
     param->setOptions(Param::autoChangeGestures,false). */
  void beginChangeGesture();
  
  /* Hosts like Logic require that you indicate when a parameter ends a gesture (changes). 
    Use this method indicate the end of a gesture for this parameter. 
    However bear in mind that Options::autoChangeGestures is set to true by default so
    you won't need to call it manually unless you disable it first using:
    param->setOptions(Param::autoChangeGestures,false). */
  void endChangeGesture();
    
  Param(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions):
  pluginProcessor(nullptr),
  globalIndex(0),
  name(name),
  xmlName(name),
  registerAtHostFlag(registerAtHostFlag),
  updateUiFlag(false),
  updateFromFlag(UPDATE_FROM_HOST),
  updateXml(false)
  {
    switch (loadSaveOptions){
      case SAVE:{
        settings[loadFromSession]=true;
        settings[saveToSession]=true;
        settings[loadFromPresets]=true;
        settings[saveToPresets]=true;
        break;
      }
      case SAVE_ONLY_IN_SESSION:{
        settings[loadFromSession]=true;
        settings[saveToSession]=true;
        settings[loadFromPresets]=false;
        settings[saveToPresets]=false;
        break;
      }
      case SAVE_ONLY_IN_PRESETS:{
        settings[loadFromSession]=false;
        settings[saveToSession]=false;
        settings[loadFromPresets]=true;
        settings[saveToPresets]=true;
        break;
      }
      case DONT_SAVE:{
        settings[loadFromSession]=false;
        settings[saveToSession]=false;
        settings[loadFromPresets]=false;
        settings[saveToPresets]=false;
        break;
      }
      default:{
        settings[loadFromSession]=true;
        settings[saveToSession]=true;
        settings[loadFromPresets]=true;
        settings[saveToPresets]=true;
        break;
      }
    }
    
    settings[saveOnlyNonDefaultValues]=true;
    settings[forceRunAfterParamChangeInHost]=false;
    settings[createThreadForParamChange]=false;
    settings[autoChangeGestures]=true;

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
  bool updateProcessorFromXml(){
    if (*value!=xmlValue && updateXml){     
      *value=xmlValue;
      updateXml=false;
      return true;
    }
    updateXml=false;
    return false;
  }
    
  bool updateProcessorFromDefaultXml(){
    if (*value!=defaultValue){
      *value=defaultValue;
      return true;
    }
    return false;
  }

  bool hostSet(const PluginParameters_HostFloatType ){
    *value=virtualHostValue;
    return true;
  }
     
  /** Updates the value from its UI denormalized value (it doesn't call load since Strings
      can't be automated */ 
  void updateProcessorAndHostFromUi(const String valueArg);  

  /** Returns the parameter value to set the UI */
  String uiGet() const{    
    return (*value);
  }

  PluginParameters_HostFloatType hostGet() const{
    return (PluginParameters_HostFloatType)(0.f);
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
    
  void readXml(XmlElement *xml,XmlType xmlType=SESSION) {
    if ((xmlType==SESSION && settings[loadFromSession]) 
        || (xmlType==PRESET && settings[loadFromPresets])){      
      
      //tell updateProcessorFromXml(...) to update *value from xmlValue
      updateXml=true;

      if (xml==nullptr)
        xmlValue=defaultValue;
      else
        xmlValue=xml->getStringAttribute(Param::getXmlName(),defaultValue);
    }
    xmlHostValue=(PluginParameters_HostFloatType)(0.f);
  }  

  void saveXml(XmlElement *xml, XmlType xmlType=SESSION) const{
    if ((!settings[saveOnlyNonDefaultValues] || *value!=defaultValue) 
        && ( (xmlType==SESSION && settings[saveToSession]) 
           || (xmlType==PRESET && settings[saveToPresets])) ){
      xml->setAttribute(Param::getXmlName(),(*value));
    }
  }

  StringParam(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, String * const value):
  Param(name,registerAtHostFlag,loadSaveOptions),
  value(value),
  defaultValue(*value),
  xmlValue(*value){
    // Strings cannot be automated! 
    // (They aren't supported at least in VST)
    // Try again setting argument registerAtHostFlag=false
    jassert(registerAtHostFlag==false);
  }
};

class FloatParam : public Param{
private:
  PluginParameters_PluginFloatType minValue;
  PluginParameters_PluginFloatType maxValue;

  // (prevent copy constructor and operator= being generated..)
  // avoids warning C4512: "assignment operator could not be generated"
  FloatParam (const FloatParam&);
  FloatParam& operator=(const FloatParam &other);
    
protected:
  PluginParameters_PluginFloatType * const value;
  const PluginParameters_PluginFloatType defaultValue;
  PluginParameters_PluginFloatType xmlValue;  
    
public:  
  bool updateProcessorFromXml(){
    if (*value!=xmlValue && updateXml){     
      *value=xmlValue;
      updateXml=false;
      return true;
    }
    updateXml=false;
    return false;
  }
    
  bool updateProcessorFromDefaultXml(){
    if (*value!=defaultValue){
      *value=defaultValue;
      return true;
    }
    return false;
  }
    
  bool hostSet(const PluginParameters_HostFloatType hostValue){    
    PluginParameters_PluginFloatType oldValue=*value;
    if (hostValue>1)
      *value=maxValue;
    else if (hostValue<0)
      *value=minValue;
    else
      *value=minValue+(PluginParameters_PluginFloatType)(hostValue)*(maxValue-minValue);

    if (fabs(*value-oldValue)>PluginParameters_Epsilon)
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

  PluginParameters_HostFloatType hostGet() const{    
    if (maxValue==minValue)
      return (PluginParameters_HostFloatType)(0.f);
    PluginParameters_HostFloatType newHostValue=(PluginParameters_HostFloatType)(*value-minValue)/(maxValue-minValue);
    if (newHostValue<0){
      *value=minValue;
      return (PluginParameters_HostFloatType)(0.f);
    } else if (newHostValue>1){
      *value=maxValue;
      return (PluginParameters_HostFloatType)(1.f);
    } else
      return newHostValue;
  }  

  void setMin(PluginParameters_PluginFloatType minValueArg,bool updateHostFlag=true){        
    if (defaultValue>=minValueArg){
      minValue=minValueArg;
      if (*value<minValueArg)
        *value=minValueArg;
      if (xmlValue<minValueArg)
        xmlValue=minValueArg;
    } else //minValue can't be greater than the (constant) default value
      jassertfalse;

    if (updateHostFlag){
      updateHost(false);
    }
  }

  void setMax(PluginParameters_PluginFloatType maxValueArg,bool updateHostFlag=true){    
    if (defaultValue<=maxValueArg){
      maxValue=maxValueArg;
      if (*value>maxValueArg)
        *value=maxValueArg;
      if (xmlValue>maxValueArg)
        xmlValue=maxValueArg;
    } else //maxValue can't be less than the (constant) default value
      jassertfalse;

    if (updateHostFlag){
      updateHost(false);
    }
  }

  const PluginParameters_PluginFloatType getDefaultValue() const{
    return defaultValue;    
  }

  PluginParameters_PluginFloatType getPreloadvalue() const{
    return xmlValue;
  }
    
  PluginParameters_PluginFloatType getValue() const{
    return *value;
  }

  const double getMin() const{
    return minValue;
  }

  const double getMax() const{
    return maxValue;
  }

  void readXml(XmlElement *xml,XmlType xmlType=SESSION){    
    if ((xmlType==SESSION && settings[loadFromSession]) 
        || (xmlType==PRESET && settings[loadFromPresets])){
      
      //tell updateProcessorFromXml(...) to update *value from xmlValue
      updateXml=true;
      
      if (xml==nullptr)
        xmlValue=defaultValue;
      else
        xmlValue=(PluginParameters_PluginFloatType)(xml->getDoubleAttribute(Param::getXmlName(),defaultValue));
      if (maxValue==minValue){
        xmlHostValue=(PluginParameters_HostFloatType)(0.f);
        return;
      }
      xmlHostValue=(PluginParameters_HostFloatType)(xmlValue-minValue)/(maxValue-minValue);
      if (xmlHostValue<0){
        xmlValue=minValue;
        xmlHostValue=(PluginParameters_HostFloatType)(0.f);
        return;
      }
      else if (xmlHostValue>1){
        xmlValue=maxValue;
        xmlHostValue=(PluginParameters_HostFloatType)(1.f);
        return;
      }      
    }
  }

  void saveXml(XmlElement *xml, XmlType xmlType=SESSION) const{
    if ((!settings[saveOnlyNonDefaultValues] || *value!=defaultValue) 
        && ( (xmlType==SESSION && settings[saveToSession]) 
           || (xmlType==PRESET && settings[saveToPresets])) ){
      xml->setAttribute(Param::getXmlName(),(double)(*value));
    }
  }
    
  FloatParam(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, PluginParameters_PluginFloatType * const value, const PluginParameters_PluginFloatType minValue=(PluginParameters_PluginFloatType)(0),const PluginParameters_PluginFloatType maxValue=(PluginParameters_PluginFloatType)(0)):
  Param(name,registerAtHostFlag,loadSaveOptions),
  defaultValue(jmax<PluginParameters_PluginFloatType>(minValue,jmin<PluginParameters_PluginFloatType>(*value,maxValue))),
  minValue(minValue),
  maxValue(maxValue),
  value(value){    
    //force *value to the [minValue,maxValue] range
    xmlValue=*value=defaultValue;
  }
};

class LogParam : public Param{
private:
  PluginParameters_PluginFloatType minLogValue;
  PluginParameters_PluginFloatType maxLogValue;
  const PluginParameters_PluginFloatType factor;
    
  // (prevent copy constructor and operator= being generated..)
  // avoids warning C4512: "assignment operator could not be generated"
  LogParam (const LogParam&);
  LogParam& operator=(const LogParam &other);

protected:
  PluginParameters_PluginFloatType * const value;
  const PluginParameters_PluginFloatType defaultValue; 
  PluginParameters_PluginFloatType xmlValue;  

public:  
  bool updateProcessorFromXml(){
    if (*value!=xmlValue && updateXml){     
      *value=xmlValue;
      updateXml=false;
      return true;
    }
    updateXml=false;
    return false;
  }
    
  bool updateProcessorFromDefaultXml(){
    if (*value!=defaultValue){
      *value=defaultValue;
      return true;
    }
    return false;
  }

  bool hostSet(const PluginParameters_HostFloatType hostValue){
    PluginParameters_PluginFloatType oldValue=*value;
    if (hostValue>1)
      *value=maxLogValue;
    else if (hostValue<0)
      *value=minLogValue;
    else
      *value=(PluginParameters_PluginFloatType)pow(10,(double)((minLogValue+hostValue*(maxLogValue-minLogValue))/factor));    

    if (fabs(*value-oldValue)>PluginParameters_Epsilon)
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

  PluginParameters_HostFloatType hostGet() const{        
    if (maxLogValue==minLogValue)
      return (PluginParameters_HostFloatType)(0.f);
    if (*value<=0){
      *value=(PluginParameters_PluginFloatType)(pow(10,(double)(minLogValue/factor)));
      return (PluginParameters_HostFloatType)(0.f);
    }
    PluginParameters_HostFloatType newHostValue=(PluginParameters_HostFloatType)(factor*log10(fabs((double)*value))-minLogValue)/(maxLogValue-minLogValue);
    if (newHostValue<0){
      *value=(PluginParameters_PluginFloatType)(pow(10,(double)(minLogValue/factor)));
      return (PluginParameters_HostFloatType)(0.f);
    } else if (newHostValue>1){
      *value=(PluginParameters_PluginFloatType)(pow(10,(double)(maxLogValue/factor)));
      return (PluginParameters_HostFloatType)(1.f);
    } else
      return newHostValue;
  }  

  void setMin(PluginParameters_PluginFloatType minValueArg,bool updateHostFlag=true){
    if (defaultValue>=minValueArg && minValueArg>0){
      minLogValue=(PluginParameters_PluginFloatType)(factor*log10((double)(minValueArg)));
      if (*value<minValueArg)
        *value=minValueArg;
      if (xmlValue<minValueArg)
        xmlValue=minValueArg;
    } else //minValue can't be <=0 or greater than the (constant) default value
      jassertfalse;
    
    if (updateHostFlag){
      updateHost(false);
    }
  }

  void setMax(PluginParameters_PluginFloatType maxValueArg,bool updateHostFlag=true){    
    if (defaultValue<=maxValueArg && maxValueArg>0){
      maxLogValue=(PluginParameters_PluginFloatType)(factor*log10((double)(maxValueArg)));
      if (*value>maxValueArg)
        *value=maxValueArg;
      if (xmlValue>maxValueArg)
        xmlValue=maxValueArg;
    } else //maxValue can't be <=0 or less than the (constant) default value
      jassertfalse;

    if (updateHostFlag){
      updateHost(false);
    }
  }
    
  const PluginParameters_PluginFloatType getDefaultValue() const{
    return defaultValue;
  }
    
  PluginParameters_PluginFloatType getValue() const{
    return *value;
  }
    
  const PluginParameters_PluginFloatType getFactor() const{
    return factor;
  }  
    
  const double getMin() const{
    return minLogValue;
  }

  const double getMax() const{
    return maxLogValue;
  }

  void readXml(XmlElement *xml,XmlType xmlType=SESSION){
    if ((xmlType==SESSION && settings[loadFromSession]) 
        || (xmlType==PRESET && settings[loadFromPresets])){
      
      //tell updateProcessorFromXml(...) to update *value from xmlValue
      updateXml=true;

      if (xml==nullptr)
        xmlValue=defaultValue;
      else
        xmlValue=(PluginParameters_PluginFloatType)(xml->getDoubleAttribute(Param::getXmlName(),defaultValue));          
      if (maxLogValue==minLogValue){
        xmlHostValue=(PluginParameters_HostFloatType)(0.f);
        return;
      }
      
      if (xmlValue<=0){
        xmlValue=(PluginParameters_PluginFloatType)(pow(10,(double)(minLogValue/factor))); //minValue
        xmlHostValue=(PluginParameters_HostFloatType)(0.f);
        return;
      }    
      xmlHostValue=(PluginParameters_HostFloatType)(factor*log10(fabs((double)xmlValue))-minLogValue)/(maxLogValue-minLogValue);
      if (xmlHostValue<0){
        xmlValue=(PluginParameters_PluginFloatType)(pow(10,(double)(minLogValue/factor)));
        xmlHostValue=(PluginParameters_HostFloatType)(0.f);
        return;
      } else if (xmlHostValue>1){
        xmlValue=(PluginParameters_PluginFloatType)(pow(10,(double)(maxLogValue/factor)));
        xmlHostValue=(PluginParameters_HostFloatType)(0.f);
        return;
      }
    }
  }

  void saveXml(XmlElement *xml, XmlType xmlType=SESSION) const{
    if ((!settings[saveOnlyNonDefaultValues] || *value!=defaultValue) 
        && ( (xmlType==SESSION && settings[saveToSession]) 
           || (xmlType==PRESET && settings[saveToPresets])) ){
      xml->setAttribute(Param::getXmlName(),(double)(*value));
    }
  }
    
  LogParam(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, PluginParameters_PluginFloatType * const value, const PluginParameters_PluginFloatType minValue=(PluginParameters_PluginFloatType)(0),const PluginParameters_PluginFloatType maxValue=(PluginParameters_PluginFloatType)(0),const PluginParameters_PluginFloatType factor=(PluginParameters_PluginFloatType)(1)):
  Param(name,registerAtHostFlag,loadSaveOptions),
  defaultValue(jmax<PluginParameters_PluginFloatType>(minValue,jmin<PluginParameters_PluginFloatType>(*value,maxValue))),
  minLogValue((PluginParameters_PluginFloatType)(factor*log10((double)(minValue)))),
  maxLogValue((PluginParameters_PluginFloatType)(factor*log10((double)(maxValue)))),
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
  const PluginParameters_PluginFloatType factor;
  PluginParameters_PluginFloatType minLogValue;
  PluginParameters_PluginFloatType maxLogValue;  
  PluginParameters_PluginFloatType minValue;
  PluginParameters_PluginFloatType maxValue;  
    
  // (prevent copy constructor and operator= being generated..)
  // avoids warning C4512: "assignment operator could not be generated"
  LogWith0Param (const LogWith0Param&);
  LogWith0Param& operator=(const LogWith0Param &other);
    
protected:
  PluginParameters_PluginFloatType * const value;
  const PluginParameters_PluginFloatType defaultValue;  
  PluginParameters_PluginFloatType xmlValue;   

public:   
  bool updateProcessorFromXml(){
    if (*value!=xmlValue && updateXml){     
      *value=xmlValue;
      updateXml=false;
      return true;
    }
    updateXml=false;
    return false;
  }
    
  bool updateProcessorFromDefaultXml(){
    if (*value!=defaultValue){
      *value=defaultValue;
      return true;
    }
    return false;
  }

  bool hostSet(const PluginParameters_HostFloatType hostValue){
    PluginParameters_PluginFloatType oldValue=*value;
    if (hostValue>1)
      *value=maxLogValue;
    else if (hostValue<0.03){
      //leave a margin of 0.02 to avoid precision errors to confuse +/-minLog with 0
      *value=(PluginParameters_PluginFloatType)(0);
    } else {
      *value=(PluginParameters_PluginFloatType)pow(10,(double)((minLogValue+(jmax<double>((PluginParameters_HostFloatType)0.05,hostValue)-(PluginParameters_HostFloatType)0.05)*(maxLogValue-minLogValue)/0.95)/factor));
    }
    if (fabs(*value-oldValue)>PluginParameters_Epsilon)
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
    
  PluginParameters_HostFloatType hostGet() const{    
    if (maxLogValue==minLogValue){ //do not let idiots make this crash
      return (PluginParameters_HostFloatType)(0.f); //stupid question, stupid answer
    }
      
    //using the host parameter scale of [0,1]
    //store positive log value above 0.05 
    //all values in the range of [0,minLogValue] will be stored as 0
    //at 0 (a margin of 0.05 should be safe to avoid confusing 0 with the former)    
    if (*value<=0){
      *value=(PluginParameters_PluginFloatType)0;
      return (PluginParameters_HostFloatType)(0.f);
    }
              
    PluginParameters_HostFloatType newHostValue=(PluginParameters_HostFloatType)(0.05+(factor*log10(fabs((double)*value))-minLogValue)*0.95/(maxLogValue-minLogValue));
      
    if (newHostValue<0){
      *value=(PluginParameters_PluginFloatType)0;
      return (PluginParameters_HostFloatType)(0.f);
    }else if (newHostValue>1){
      *value=(PluginParameters_PluginFloatType)(pow(10,(double)(maxLogValue/factor)));
      return (PluginParameters_HostFloatType)(1.f);
    } else
      return newHostValue;    
  }
    
  const PluginParameters_PluginFloatType getDefaultValue() const{
    return defaultValue;
  }
    
  PluginParameters_PluginFloatType getValue() const{
    return *value;
  }
    
  const PluginParameters_PluginFloatType getFactor() const{
    return factor;
  }
      
  void setMin(PluginParameters_PluginFloatType minValueArg,bool updateHostFlag=true){
    if (defaultValue>=minValueArg && minValueArg>0){
      minLogValue=(PluginParameters_PluginFloatType)(factor*log10((double)(minValueArg)));
      if (*value<minValueArg)
        *value=0;
      if (xmlValue<minValueArg)
        xmlValue=0;
    } else //minValue can't be <=0 or greater than the (constant) default value
      jassertfalse;

    if (updateHostFlag){
      updateHost(false);
    }
  }

  void setMax(PluginParameters_PluginFloatType maxValueArg,bool updateHostFlag=true){    
    if (defaultValue<=maxValueArg && maxValueArg>0){
      maxLogValue=(PluginParameters_PluginFloatType)(factor*log10((double)(maxValueArg)));
      if (*value>maxValueArg)
        *value=maxValueArg;
      if (xmlValue>maxValueArg)
        xmlValue=maxValueArg;
    } else //maxValue can't be <=0 or less than the (constant) default value
      jassertfalse;

    if (updateHostFlag){
      updateHost(false);
    }
  }  
    
  const double getMin() const{
    return minLogValue-0.05;
  }

  const double getMax() const{
    return maxLogValue;
  }

  void readXml(XmlElement *xml,XmlType xmlType=SESSION){   
    if ((xmlType==SESSION && settings[loadFromSession]) 
        || (xmlType==PRESET && settings[loadFromPresets])){

      //tell updateProcessorFromXml(...) to update *value from xmlValue
      updateXml=true;

      if (xml==nullptr)
        xmlValue=defaultValue;
      else
        xmlValue=(PluginParameters_PluginFloatType)(xml->getDoubleAttribute(Param::getXmlName(),defaultValue));  
                       
      if (maxLogValue==minLogValue){ //do not let idiots make this crash
        xmlHostValue=(PluginParameters_HostFloatType)(0.f); //stupid question, stupid answer
        return; 
      }
      //using the host parameter scale of [0,1]
      //store positive log value above 0.05 
      //all values in the range of [0,minLogValue] will be stored as 0
      //at 0 (a margin of 0.05 should be safe to avoid confusing 0 with the former)     
      if (xmlValue<=0){
        xmlValue=0;
        xmlHostValue=(PluginParameters_HostFloatType)(0);
        return;
      } else {
        double xmlLogValue=factor*log10(fabs((double)xmlValue));
        if (xmlLogValue<minLogValue){
          xmlValue=0;
          xmlHostValue=(PluginParameters_HostFloatType)(0);
        }else{
          xmlHostValue=(PluginParameters_HostFloatType)(0.05+(xmlLogValue-minLogValue)*0.95/(maxLogValue-minLogValue));
        }     
      }
      if (xmlHostValue>1){
        xmlValue=(PluginParameters_PluginFloatType)(pow(10,(double)(maxLogValue/factor)));
        xmlHostValue=(PluginParameters_HostFloatType)(1.f);
      }
    }       
  }

  void saveXml(XmlElement *xml, XmlType xmlType=SESSION) const{
    if ((!settings[saveOnlyNonDefaultValues] || *value!=defaultValue) 
        && ( (xmlType==SESSION && settings[saveToSession]) 
           || (xmlType==PRESET && settings[saveToPresets])) ){
      xml->setAttribute(Param::getXmlName(),(double)(*value));
    }
  }

  LogWith0Param(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, PluginParameters_PluginFloatType * const value, const PluginParameters_PluginFloatType minValue=(PluginParameters_PluginFloatType)(0.001),const PluginParameters_PluginFloatType maxValue=(PluginParameters_PluginFloatType)(1),const PluginParameters_PluginFloatType factor=(PluginParameters_PluginFloatType)(1)):
  Param(name,registerAtHostFlag,loadSaveOptions),
  defaultValue(jmax<PluginParameters_PluginFloatType>(0,jmin<PluginParameters_PluginFloatType>(*value,maxValue))),
  minLogValue((PluginParameters_PluginFloatType)(factor*log10((double)(minValue)))),
  maxLogValue((PluginParameters_PluginFloatType)(factor*log10((double)(maxValue)))),
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
  PluginParameters_PluginFloatType maxNegLogValue;
  PluginParameters_PluginFloatType maxPosLogValue;  
  PluginParameters_PluginFloatType minAbsLogValue;
  const PluginParameters_PluginFloatType factor;
  const PluginParameters_HostFloatType centerValue;
    
  // (prevent copy constructor and operator= being generated..)
  // avoids warning C4512: "assignment operator could not be generated"
  LogWithSignParam (const LogWithSignParam&);
  LogWithSignParam& operator=(const LogWithSignParam &other);
    
protected:
  PluginParameters_PluginFloatType * const value;
  const PluginParameters_PluginFloatType defaultValue;  
  PluginParameters_PluginFloatType xmlValue;   

public:
  bool updateProcessorFromXml(){
    if (*value!=xmlValue && updateXml){     
      *value=xmlValue;
      updateXml=false;
      return true;
    }
    updateXml=false;
    return false;
  }
    
  bool updateProcessorFromDefaultXml(){
    if (*value!=defaultValue){
      *value=defaultValue;
      return true;
    }
    return false;
  }

  bool hostSet(const PluginParameters_HostFloatType hostValue){  
    PluginParameters_PluginFloatType oldValue=*value;    
    if (hostValue>1)
      *value=maxPosLogValue;
    else if (hostValue<0)
      *value=maxNegLogValue;
    else{
      //leave a margin of 0.02 to avoid precision errors to confuse +/-minLog with 0
      if (hostValue>centerValue+0.03){
        *value=(PluginParameters_PluginFloatType)pow(10,((double)(minAbsLogValue+(jmax<double>(0,hostValue-(centerValue+0.05)))*(maxPosLogValue-minAbsLogValue)/(1-centerValue-0.05))/(factor)));
      } else if (hostValue<centerValue-0.03) {
        *value=-(PluginParameters_PluginFloatType)pow(10,((double)(minAbsLogValue+((centerValue-0.05)-jmin<double>((centerValue-0.05),hostValue))*(maxNegLogValue-minAbsLogValue)/(centerValue-0.05))/(factor)));
      } else
        *value=(PluginParameters_PluginFloatType)(0);
    }      

    if (fabs(*value-oldValue)>PluginParameters_Epsilon)
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

  PluginParameters_HostFloatType hostGet() const{    
    if (maxPosLogValue==minAbsLogValue || maxNegLogValue==minAbsLogValue){ //do not let idiots make this crash
      if (*value>0)
        return (PluginParameters_HostFloatType)(1.f); //stupid question, stupid answer
      else
        return (PluginParameters_HostFloatType)(0.f); //stupid question, stupid answer
    }
    PluginParameters_HostFloatType newHostValue; 
      
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
      newHostValue=(PluginParameters_HostFloatType)(centerValue+0.05+(logValue-minAbsLogValue)/(maxPosLogValue-minAbsLogValue)*(1-centerValue-0.05));
    } else {
      newHostValue=(PluginParameters_HostFloatType)(centerValue-0.05-(logValue-minAbsLogValue)/(maxNegLogValue-minAbsLogValue)*(centerValue-0.05));
    }
      
    if (newHostValue<0){
      *value=-(PluginParameters_PluginFloatType)(pow(10,(double)(maxNegLogValue/factor)));
      return (PluginParameters_HostFloatType)(0.f);
    }else if (newHostValue>1){
      *value=(PluginParameters_PluginFloatType)(pow(10,(double)(maxPosLogValue/factor)));
      return (PluginParameters_HostFloatType)(1.f);
    } else
      return newHostValue;   
  }  
    
  const PluginParameters_PluginFloatType getDefaultValue() const{
    return defaultValue;
  }

  PluginParameters_PluginFloatType getValue() const{
    return *value;
  }
    
  const PluginParameters_PluginFloatType getFactor() const{
    return factor;
  }
    
  void setMin(PluginParameters_PluginFloatType minValueArg){
    if (defaultValue>=minValueArg && minValueArg<0){
      maxNegLogValue=(PluginParameters_PluginFloatType)(factor*log10(fabs((double)(minValueArg))));
      if (*value<minValueArg)
        *value=minValueArg;
      if (xmlValue<minValueArg)
        xmlValue=minValueArg;
    } else //minValue can't be >=0 or greater than the (constant) default value
      jassertfalse;
  }
    
  void setMinAbs(PluginParameters_PluginFloatType minAbsValueArg,bool updateHostFlag=true){    
    if (fabs(defaultValue)<=minAbsValueArg && minAbsValueArg>0){
      minAbsLogValue=(PluginParameters_PluginFloatType)(factor*log10((double)(minAbsValueArg)));
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

    if (updateHostFlag){
      updateHost(false);
    }
  }  

  void setMax(PluginParameters_PluginFloatType maxValueArg, bool updateHostFlag=true){    
    if (defaultValue<=maxValueArg && maxValueArg>0){
      maxPosLogValue=(PluginParameters_PluginFloatType)(factor*log10((double)(maxValueArg)));
      if (*value>maxValueArg)
        *value=maxValueArg;
      if (xmlValue>maxValueArg)
        xmlValue=maxValueArg;
    } else //maxValue can't be <=0 or less than the (constant) default value
      jassertfalse;
    
    if (updateHostFlag){
      updateHost(false);
    }
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

  void readXml(XmlElement *xml,XmlType xmlType=SESSION){  
    if ((xmlType==SESSION && settings[loadFromSession]) 
        || (xmlType==PRESET && settings[loadFromPresets])){
      
      //tell updateProcessorFromXml(...) to update *value from xmlValue
      updateXml=true;

      if (xml==nullptr)
        xmlValue=defaultValue;
      else
        xmlValue=(PluginParameters_PluginFloatType)(xml->getDoubleAttribute(Param::getXmlName(),defaultValue));              
      if (maxPosLogValue==minAbsLogValue || maxNegLogValue==minAbsLogValue){ //do not let idiots make this crash
        if (xmlValue>0){
          xmlHostValue=(PluginParameters_HostFloatType)(1.f); //stupid question, stupid answer
          return;
        } else{
          xmlHostValue=(PluginParameters_HostFloatType)(0.f); //stupid question, stupid answer
          return;
        }
      }        
      
      //using the host parameter scale of [0,1]
      //store positive log value above 0.55 and negative log values below 0.45
      //all values in the range of [-minLogValue,minLogValue] will be stored as 0
      //at 0.5 (a margin of 0.05 should be safe to avoid confusing 0 with the former)    
      if (xmlValue==0){
        xmlValue=0;
        xmlHostValue=centerValue;
        return;
      }    
      
      double logValue=factor*log10(fabs((double)(xmlValue)));
      
      if (logValue<minAbsLogValue){
        xmlValue=0;
        xmlHostValue=centerValue;
        return;
      }
      
      if (xmlValue>0){
        xmlHostValue=(PluginParameters_HostFloatType)(centerValue+0.05+(logValue-minAbsLogValue)/(maxPosLogValue-minAbsLogValue)*(1-centerValue-0.05));
      } else { // (xmlValue<0)
        xmlHostValue=(PluginParameters_HostFloatType)(centerValue-0.05-(logValue-minAbsLogValue)/(maxNegLogValue-minAbsLogValue)*(centerValue-0.05));
      }
      
      if (xmlHostValue<0){
        xmlValue=-(PluginParameters_PluginFloatType)(pow(10,(double)(maxNegLogValue/factor)));
        xmlHostValue=(PluginParameters_HostFloatType)(0.f);
        return;
      } else if (xmlHostValue>1){
        xmlValue=(PluginParameters_PluginFloatType)(pow(10,(double)(maxPosLogValue/factor)));
        xmlHostValue=(PluginParameters_HostFloatType)(1.f);
        return;
      }
    }    
  }

  void saveXml(XmlElement *xml, XmlType xmlType=SESSION) const{
    if ((!settings[saveOnlyNonDefaultValues] || *value!=defaultValue) 
        && ( (xmlType==SESSION && settings[saveToSession]) 
           || (xmlType==PRESET && settings[saveToPresets])) ){
      xml->setAttribute(Param::getXmlName(),(double)(*value));
    }
  }

  LogWithSignParam(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, PluginParameters_PluginFloatType * const value, const PluginParameters_PluginFloatType minNegativeValue=(PluginParameters_PluginFloatType)(-1),const PluginParameters_PluginFloatType maxPositiveValue=(PluginParameters_PluginFloatType)(1), const PluginParameters_PluginFloatType minAbsValue=(PluginParameters_PluginFloatType)(0.001),const PluginParameters_PluginFloatType factor=(PluginParameters_PluginFloatType)(1)):
  Param(name,registerAtHostFlag,loadSaveOptions),
  defaultValue(jmax<PluginParameters_PluginFloatType>(minNegativeValue,jmin<PluginParameters_PluginFloatType>(*value,maxPositiveValue))),
  maxNegLogValue((PluginParameters_PluginFloatType)(factor*log10(-(double)minNegativeValue))),
  maxPosLogValue((PluginParameters_PluginFloatType)(factor*log10((double)maxPositiveValue))),
  minAbsLogValue((PluginParameters_PluginFloatType)(factor*log10((double)minAbsValue))),
  centerValue((PluginParameters_HostFloatType)((factor*log10(-(double)minNegativeValue)+factor*log10((double)minAbsValue))/(factor*log10(-(double)minNegativeValue)+factor*log10((double)maxPositiveValue)+2*factor*log10((double)minAbsValue)))),
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
  PluginParameters_PluginIntType minValue;
  PluginParameters_PluginIntType maxValue;
    
  // (prevent copy constructor and operator= being generated..)
  // avoids warning C4512: "assignment operator could not be generated"
  IntParam (const IntParam&);
  IntParam& operator=(const IntParam &other);

protected:
  PluginParameters_PluginIntType * const value;
  const PluginParameters_PluginIntType defaultValue;
  PluginParameters_PluginIntType xmlValue;   

public:  
  bool updateProcessorFromXml(){
    if (*value!=xmlValue && updateXml){     
      *value=xmlValue;
      updateXml=false;
      return true;
    }
    updateXml=false;
    return false;
  }
    
  bool updateProcessorFromDefaultXml(){
    if (*value!=defaultValue){
      *value=defaultValue;
      return true;
    }
    return false;
  }

  bool hostSet(const PluginParameters_HostFloatType hostValue){    
    PluginParameters_PluginIntType oldValue=*value;
    if (hostValue>1)
      *value=maxValue;
    else if (hostValue<0)
      *value=minValue;
    else{
      *value=roundToInt<PluginParameters_HostFloatType>(minValue+hostValue*(maxValue-minValue));
    }

    if (abs(*value-oldValue)>PluginParameters_Epsilon)
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

  PluginParameters_HostFloatType hostGet() const{
    if (maxValue==minValue)
      return (PluginParameters_HostFloatType)(0.f);
    PluginParameters_HostFloatType newHostValue=(PluginParameters_HostFloatType)(*value-minValue)/(maxValue-minValue);
    if (newHostValue<0){
      *value=minValue;
      return (PluginParameters_HostFloatType)(0.f);
    } else if (newHostValue>1){
      *value=maxValue;
      return (PluginParameters_HostFloatType)(1.f);
    } else{
      return newHostValue;
    }
  }
    
  void setMin(PluginParameters_PluginIntType minValueArg,bool updateHostFlag=true){        
    if (defaultValue>=minValueArg){
      minValue=minValueArg;
      if (*value<minValueArg)
        *value=minValueArg;
      if (xmlValue<minValueArg)
        xmlValue=minValueArg;
    } else //minValue can't be greater than the (constant) default value
      jassertfalse;

    if (updateHostFlag){
      updateHost(false);
    }
  }

  void setMax(PluginParameters_PluginIntType maxValueArg,bool updateHostFlag=true){    
    if (defaultValue<=maxValueArg){
      maxValue=maxValueArg;
      if (*value>maxValueArg)
        *value=maxValueArg;
      if (xmlValue>maxValueArg)
        xmlValue=maxValueArg;
    } else //maxValue can't be less than the (constant) default value
      jassertfalse;
    
    if (updateHostFlag){
      updateHost(false);
    }
  }

  const PluginParameters_PluginIntType getDefaultValue() const{
    return defaultValue;
  }
   
  PluginParameters_PluginIntType getValue() const{
    return *value;
  }
    
  const double getMin() const{
    return minValue;
  }

  const double getMax() const{
    return maxValue;
  }

  void readXml(XmlElement *xml,XmlType xmlType=SESSION){    
    if ((xmlType==SESSION && settings[loadFromSession]) 
        || (xmlType==PRESET && settings[loadFromPresets])){

      //tell updateProcessorFromXml(...) to update *value from xmlValue
      updateXml=true;

      if (xml==nullptr)
        xmlValue=defaultValue;
      else
        xmlValue=(PluginParameters_PluginIntType)(xml->getIntAttribute(Param::getXmlName(),defaultValue));    
      xmlHostValue=(PluginParameters_HostFloatType)(xmlValue-minValue)/(maxValue-minValue);
      if (maxValue==minValue){
        xmlHostValue=(PluginParameters_HostFloatType)(0.f);
        return;
      }
      if (xmlHostValue<0){
        xmlValue=minValue;
        xmlHostValue=(PluginParameters_HostFloatType)(0.f);
        return;
      }else if (xmlHostValue>1){
        xmlValue=maxValue;
        xmlHostValue=(PluginParameters_HostFloatType)(1.f);
        return;
      }
    }
  }

  void saveXml(XmlElement *xml, XmlType xmlType=SESSION) const{
    if ((!settings[saveOnlyNonDefaultValues] || *value!=defaultValue) 
        && ( (xmlType==SESSION && settings[saveToSession]) 
           || (xmlType==PRESET && settings[saveToPresets])) ){
      xml->setAttribute(Param::getXmlName(),(int)(*value));
    }
  }
    
  IntParam(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, PluginParameters_PluginIntType * const value, const PluginParameters_PluginIntType minValue=0,const PluginParameters_PluginIntType maxValue=1):
  Param(name,registerAtHostFlag,loadSaveOptions),
  value(value),
  defaultValue(jmax<PluginParameters_PluginIntType>(minValue,jmin<PluginParameters_PluginIntType>(*value,maxValue))),
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
  bool updateProcessorFromXml(){
    if (*value!=xmlValue && updateXml){     
      *value=xmlValue;
      updateXml=false;
      return true;
    }
    updateXml=false;
    return false;
  }

  bool updateProcessorFromDefaultXml(){
    if (*value!=defaultValue){
      *value=defaultValue;
      return true;
    }
    return false;
  }

  bool hostSet(const PluginParameters_HostFloatType hostValue){    
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

  PluginParameters_HostFloatType hostGet() const{
    return (*value)?(PluginParameters_HostFloatType)(1.f):(PluginParameters_HostFloatType)(0.f);
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

  void readXml(XmlElement *xml,XmlType xmlType=SESSION){    
    if ((xmlType==SESSION && settings[loadFromSession]) 
        || (xmlType==PRESET && settings[loadFromPresets])){

      //tell updateProcessorFromXml(...) to update *value from xmlValue
      updateXml=true;

      if (xml==nullptr)
        xmlValue=defaultValue;
      else
        xmlValue=xml->getBoolAttribute(Param::getXmlName(),defaultValue);
      xmlHostValue=(xmlValue)?(PluginParameters_HostFloatType)(1.f):(PluginParameters_HostFloatType)(0.f);
    }    
  }

  void saveXml(XmlElement *xml, XmlType xmlType=SESSION) const{
    if ((!settings[saveOnlyNonDefaultValues] || *value!=defaultValue) 
        && ( (xmlType==SESSION && settings[saveToSession]) 
           || (xmlType==PRESET && settings[saveToPresets])) ){
      xml->setAttribute(Param::getXmlName(),(*value)?String("true"):String("false"));
    }
  }
    
  BoolParam(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, bool * const value):
  Param(name,registerAtHostFlag,loadSaveOptions),
  defaultValue(*value),
  xmlValue(*value),
  value(value){
  }

};

#endif