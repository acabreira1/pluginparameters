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
  JUCE_DECLARE_NON_COPYABLE (Param)

  /** This flag determines if a parameter will be automated in the host.  */  
  const bool registerAtHostFlag;

  const String name;

  String xmlName;

  bool updateUiFlag;

  friend class ParamGroup;

  int globalIndex;

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
    automationUndos,
    numOptions
  };

protected:
  /** Stores all settings for this parameter */
  bool settings[numOptions];

  /** if true readXml(...) tells updateProcessorFromXml(...) to update *value according to xmlValue */
  bool updateXml;

  PluginParameters_HostFloatType xmlHostValue;  

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

  /** Update the host with a normalized value, set the UpdateFrom flag to UPDATE_FROM_UI 
      and skip the UI update */
  void updateProcessorAndHostFromNormUi(PluginParameters_HostFloatType newValue, UndoManager *const undoManager=nullptr, const bool dontCreateNewUndoTransaction=false, UpdateFromFlags updateFromFlag=UPDATE_FROM_UI);

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
  registerAtHostFlag(registerAtHostFlag),
  name(name),
  xmlName(name),
  updateUiFlag(false),
  globalIndex(0),  
  pluginProcessor(nullptr),
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

  virtual ~Param() {}
};

/* Class that uses templates to define the common
   methods and variables for all parameters.
   (Not used in class Param to allow type independent
    casts) */
template<class Type>
class TypeParam: public Param{
  JUCE_DECLARE_NON_COPYABLE (TypeParam)

protected:
  Type * const value;
  Type defaultValue;
  Type xmlValue;  

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

  const Type getDefaultValue() const{
    return defaultValue;    
  }

  Type getPreloadvalue() const{
    return xmlValue;
  }
    
  Type getValue() const{
    return *value;
  }

  TypeParam(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, Type * const value):
  Param(name,registerAtHostFlag,loadSaveOptions),
  value(value),
  defaultValue(*value),
  xmlValue(*value){
  }

  virtual ~TypeParam() {}
};

//-----------------------------------------------------------------------------------


/** String parameters can not be automated so their
    automation is merely simulated so that their
    interface presents no difference with the 
    other parameters. */
class StringParam : public TypeParam<String>{
private:
  JUCE_DECLARE_NON_COPYABLE (StringParam)

  String virtualHostValue;    
      
public:
  bool hostSet(const PluginParameters_HostFloatType ){
    *value=virtualHostValue;
    return true;
  }
     
  /** Updates the value from its UI denormalized value (it doesn't call load since Strings
      can't be automated */ 
  void updateProcessorAndHostFromUi(const String valueArg, UndoManager *const undoManager=nullptr, const bool dontCreateNewUndoTransaction=false, UpdateFromFlags updateFromFlag=UPDATE_FROM_UI);  

  /** Returns the parameter value to set the UI */
  String uiGet() const{    
    return (*value);
  }

  PluginParameters_HostFloatType hostGet() const{
    return (PluginParameters_HostFloatType)(0.f);
  }
    
  const double getMin() const{
    //bogus
    return 0;
  }
    
  const double getMax() const{
    //bogus
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
  TypeParam<String>(name,registerAtHostFlag,loadSaveOptions,value)
  {
    // Strings cannot be automated! 
    // (They aren't supported at least in VST)
    // Try again setting argument registerAtHostFlag=false
    jassert(registerAtHostFlag==false);
  }

  virtual ~StringParam() {}
};

//-----------------------------------------------------------------------------------

/** Base class for mappings in parameters of float type 

   The parameter is stored at the processor unmapped,
   shown at the UI mapped and its mapped value 
   normalized to [0,1] at the host.
*/
template<class FloatType>
class FloatTypeBaseMap {  
  JUCE_DECLARE_NON_COPYABLE (FloatTypeBaseMap)
protected:
  FloatType minMappedValue;
  FloatType maxMappedValue;
  FloatType minValue;
  FloatType maxValue;
  FloatType uiMin;
  FloatType uiMax;

  virtual void setMin(FloatType minValueArg,bool updateHostFlag=true) = 0;

  virtual void setMax(FloatType maxValueArg,bool updateHostFlag=true) = 0;

  virtual const FloatType map(const FloatType unmappedValue) const = 0;  

  virtual const FloatType unmap(const FloatType mappedValue) const = 0;

  virtual void updateMinMappedValue(){
    uiMin=minMappedValue=map(minValue);    
  }

  virtual void updateMaxMappedValue(){
    uiMax=maxMappedValue=map(maxValue);
  }

  virtual const PluginParameters_HostFloatType mapProcessorToHost(FloatType *processorValue) const{
    if (maxMappedValue==minMappedValue)
      return (PluginParameters_HostFloatType)(0.);

    const FloatType mappedValue=map(*processorValue);
    
    if (mappedValue<minMappedValue){
      *processorValue=minValue;
      return (PluginParameters_HostFloatType)(0.);
    }

    if (mappedValue>maxMappedValue){
      *processorValue=maxValue;
      return (PluginParameters_HostFloatType)(1.);
    }
    
    return (PluginParameters_HostFloatType)((mappedValue-minMappedValue)/(maxMappedValue-minMappedValue));
  }

  virtual const FloatType mapHostToProcessor(const PluginParameters_HostFloatType hostValue) const{
    if (hostValue>1)
      return maxValue;
    if (hostValue<0)
      return minValue;

    return unmap(minMappedValue+(FloatType)(hostValue)*(maxMappedValue-minMappedValue));
  }

  virtual const PluginParameters_HostFloatType mapUiToHost(const FloatType uiValue) const{
    if (maxMappedValue==minMappedValue)
      return (PluginParameters_HostFloatType)(0.);
    
    if (uiValue<minMappedValue)
      return (PluginParameters_HostFloatType)(0.);

    if (uiValue>maxMappedValue)
      return (PluginParameters_HostFloatType)(1.);

    return (PluginParameters_HostFloatType)((uiValue-minMappedValue)/(maxMappedValue-minMappedValue));
  }

  virtual const FloatType mapProcessorToUi(FloatType *processorValue) const{
    const FloatType mappedValue=map(*processorValue);

    if (mappedValue>maxMappedValue){
      return maxMappedValue;
    }
    if (mappedValue<minMappedValue)
      return minMappedValue;

    return mappedValue;
  }

  virtual const FloatType mapHostToUi(const PluginParameters_HostFloatType hostValue) const{
    if (hostValue>1)
      return maxMappedValue;
    if (hostValue<0)
      return minMappedValue;

    return minMappedValue+(FloatType)(hostValue)*(maxMappedValue-minMappedValue);
  }  

public:
  FloatTypeBaseMap(const FloatType minValue,const FloatType maxValue):
    minMappedValue(minValue),
    maxMappedValue(maxValue),
    minValue(minValue),
    maxValue(maxValue),
    uiMin(minValue),
    uiMax(maxValue)
  {}

  virtual ~FloatTypeBaseMap() {}
};

/** Identity map in parameters of float type */
template<class FloatType>
class IdentityMap: public FloatTypeBaseMap<FloatType>{
protected:
  const FloatType map(const FloatType unmappedValue) const override{
    return unmappedValue;
  }
  
  const FloatType unmap(const FloatType mappedValue) const override{
    return mappedValue;
  }

public:
  IdentityMap(const FloatType minValue,const FloatType maxValue)
  :FloatTypeBaseMap<FloatType>(minValue,maxValue) 
  {
    FloatTypeBaseMap<FloatType>::updateMinMappedValue();
    FloatTypeBaseMap<FloatType>::updateMaxMappedValue();
  }
};

/** Log map in parameters of float type */
template<class FloatType, int factor>
class LogMap: public FloatTypeBaseMap<FloatType>{
protected:
  const FloatType map(const FloatType unmappedValue) const override{
    return (FloatType)(factor*log10(fabs((double)unmappedValue)));
  }
  
  const FloatType unmap(const FloatType mappedValue) const override{
    return (FloatType)(pow(10,(double)(mappedValue/factor)));   
  }

  void updateMinMappedValue() override{
    //log values are stricly positive, please define a strictly positive range
    jassert(FloatTypeBaseMap<FloatType>::minValue>0);

    FloatTypeBaseMap<FloatType>::uiMin=FloatTypeBaseMap<FloatType>::minMappedValue=map(FloatTypeBaseMap<FloatType>::minValue);
  }

  void updateMaxMappedValue() override{
    //log values are stricly positive, please define a strictly positive range
    jassert(FloatTypeBaseMap<FloatType>::maxValue>0);

    FloatTypeBaseMap<FloatType>::uiMax=FloatTypeBaseMap<FloatType>::maxMappedValue=map(FloatTypeBaseMap<FloatType>::maxValue);
  }

public:
  LogMap(const FloatType minValue,const FloatType maxValue)
  :FloatTypeBaseMap<FloatType>(minValue,maxValue) 
  {
    updateMinMappedValue();
    updateMaxMappedValue();
  }
};

/** Log map with 0 in parameters of float type 

    using the host parameter scale of [0,1]
    [0,width0] stores the value 0 (we round [0,FloatTypeBaseMap<FloatType>::minValue/2] to 0))
    [width0,1] stores the values from FloatTypeBaseMap<FloatType>::minMappedValue to FloatTypeBaseMap<FloatType>::maxMappedValue  

    ui0WidthDenominator=-1 sets automatically ui0Width to reproduce the scale of host0Width in the UI
*/
template<class FloatType, int factor=1, int host0WidthDenominator=100, int ui0WidthDenominator=100>
class LogWith0Map: public FloatTypeBaseMap<FloatType>{
  FloatType ui0Width;
  const PluginParameters_HostFloatType host0Width;  
protected:
  const FloatType map(const FloatType unmappedValue) const override{
    return (FloatType)(factor*log10(fabs((double)unmappedValue)));
  }
  
  const FloatType unmap(const FloatType mappedValue) const override{
    return (FloatType)(pow(10,(double)(mappedValue/factor)));   
  }  

  void updateMinMappedValue() override{
    //log values are stricly positive, please define a strictly positive range
    jassert(FloatTypeBaseMap<FloatType>::minValue>0);

    FloatTypeBaseMap<FloatType>::minMappedValue=map(FloatTypeBaseMap<FloatType>::minValue);
    FloatTypeBaseMap<FloatType>::uiMin=FloatTypeBaseMap<FloatType>::minMappedValue-ui0Width;
  }

  void updateMaxMappedValue() override{
    //log values are stricly positive, please define a strictly positive range
    jassert(FloatTypeBaseMap<FloatType>::maxValue>0);

    FloatTypeBaseMap<FloatType>::maxMappedValue=map(FloatTypeBaseMap<FloatType>::maxValue);
    FloatTypeBaseMap<FloatType>::uiMax=FloatTypeBaseMap<FloatType>::maxMappedValue;
  }      

  const PluginParameters_HostFloatType mapProcessorToHost(FloatType *processorValue) const override{
    if (FloatTypeBaseMap<FloatType>::maxMappedValue==FloatTypeBaseMap<FloatType>::minMappedValue)
      return (*processorValue>0)?(PluginParameters_HostFloatType)(1.):(PluginParameters_HostFloatType)(0.);
          
    if (*processorValue<=FloatTypeBaseMap<FloatType>::minValue/2){
      *processorValue=0;
      return (PluginParameters_HostFloatType)(0.f);
    }
    
    const FloatType mappedValue=map(*processorValue);

    if (mappedValue<FloatTypeBaseMap<FloatType>::minMappedValue){
      *processorValue=FloatTypeBaseMap<FloatType>::minValue;
      return host0Width;
    }

    if (mappedValue>FloatTypeBaseMap<FloatType>::maxMappedValue){
      *processorValue=FloatTypeBaseMap<FloatType>::maxValue;
      return (PluginParameters_HostFloatType)(1.);
    }
    
    return (PluginParameters_HostFloatType)(host0Width+(1-host0Width)*(mappedValue-FloatTypeBaseMap<FloatType>::minMappedValue)/(FloatTypeBaseMap<FloatType>::maxMappedValue-FloatTypeBaseMap<FloatType>::minMappedValue));
  }

  const FloatType mapHostToProcessor(const PluginParameters_HostFloatType hostValue) const override{
    if (hostValue>1)
      return FloatTypeBaseMap<FloatType>::maxValue;

    if (host0Width==0 && hostValue==0)
      return 0;

    //leave a margin of hostWidth0/2 to avoid making rounding errors confuse +/-FloatTypeBaseMap<FloatType>::minMappedValue with 0
    if (hostValue<host0Width/2)
      return 0;

    if (hostValue<host0Width)
      return FloatTypeBaseMap<FloatType>::minValue;

    return unmap(FloatTypeBaseMap<FloatType>::minMappedValue+(FloatType)((jmax<PluginParameters_HostFloatType>(host0Width,hostValue)-host0Width)*(FloatTypeBaseMap<FloatType>::maxMappedValue-FloatTypeBaseMap<FloatType>::minMappedValue)/(1-host0Width)));
  }

  const PluginParameters_HostFloatType mapUiToHost(const FloatType uiValue) const override{
    if (FloatTypeBaseMap<FloatType>::maxMappedValue==FloatTypeBaseMap<FloatType>::minMappedValue)
      return (uiValue>0)?(PluginParameters_HostFloatType)(1.):(PluginParameters_HostFloatType)(0.);    
    
    if (uiValue<=FloatTypeBaseMap<FloatType>::minMappedValue-ui0Width/2)
      return (PluginParameters_HostFloatType)(0.);

    if (uiValue<FloatTypeBaseMap<FloatType>::minMappedValue)
      return host0Width;

    if (uiValue>FloatTypeBaseMap<FloatType>::maxMappedValue)
      return (PluginParameters_HostFloatType)(1.);

    return (PluginParameters_HostFloatType)(host0Width+(1-host0Width)*(uiValue-FloatTypeBaseMap<FloatType>::minMappedValue)/(FloatTypeBaseMap<FloatType>::maxMappedValue-FloatTypeBaseMap<FloatType>::minMappedValue));
  }

  const FloatType mapProcessorToUi(FloatType *processorValue) const override{
    if (FloatTypeBaseMap<FloatType>::maxMappedValue==FloatTypeBaseMap<FloatType>::minMappedValue)
      return (*processorValue>0)?(PluginParameters_HostFloatType)(FloatTypeBaseMap<FloatType>::maxMappedValue):(FloatTypeBaseMap<FloatType>::minMappedValue-ui0Width);

    if (*processorValue<=FloatTypeBaseMap<FloatType>::minValue/2)
      return FloatTypeBaseMap<FloatType>::uiMin;
    
    const FloatType mappedValue=map(*processorValue);

    if (mappedValue>FloatTypeBaseMap<FloatType>::maxMappedValue)
      return FloatTypeBaseMap<FloatType>::uiMax;

    if (mappedValue<FloatTypeBaseMap<FloatType>::minMappedValue)
      return FloatTypeBaseMap<FloatType>::minMappedValue;

    return mappedValue;
  }

  const FloatType mapHostToUi(const PluginParameters_HostFloatType hostValue) const override{
    if (hostValue>1)
      return FloatTypeBaseMap<FloatType>::uiMax;

    if (hostValue<0)
      return FloatTypeBaseMap<FloatType>::uiMin;

    if (host0Width==0 && hostValue==0)
      return FloatTypeBaseMap<FloatType>::uiMin;
    
    if (hostValue<host0Width/2)
      return FloatTypeBaseMap<FloatType>::uiMin;
    
    if (hostValue<host0Width)
      return FloatTypeBaseMap<FloatType>::minMappedValue;

    return FloatTypeBaseMap<FloatType>::minMappedValue+(FloatType)((jmax<PluginParameters_HostFloatType>(host0Width,hostValue)-host0Width)*(FloatTypeBaseMap<FloatType>::maxMappedValue-FloatTypeBaseMap<FloatType>::minMappedValue)/(1-host0Width));    
  }  

public:
  LogWith0Map(const FloatType minValue,const FloatType maxValue)
  :FloatTypeBaseMap<FloatType>(minValue,maxValue),
  ui0Width((FloatType)((ui0WidthDenominator<0)?
                        ((host0WidthDenominator<=0)?
                          0
                          :((1./jmax<int>(host0WidthDenominator,1))/(1-(1./jmax<int>(host0WidthDenominator,1)))*(map(FloatTypeBaseMap<FloatType>::maxValue)-map(FloatTypeBaseMap<FloatType>::minValue))))
                       :((ui0WidthDenominator==0)?
                          0
                          :(1./jmax<int>(ui0WidthDenominator,1))))),
   host0Width((PluginParameters_HostFloatType)((host0WidthDenominator==0)?
                                                0
                                                :(1./jmax<int>(host0WidthDenominator,1))))
  {
    updateMinMappedValue();
    updateMaxMappedValue();
  }
};

/** Log map in parameters of float type 
    
    using the host parameter scale of [0,1]
    (host0-host0Width,host0+host0Width) stores the value 0 (we round [-minAbsValue/2,minAbsValue/2] to 0))
    [host0+host0Width,1] stores the values from minAbsMappedValue to FloatTypeBaseMap<FloatType>::maxMappedValue  
    [0,host0-host0Width] stores the values from minAbsMappedValue to FloatTypeBaseMap<FloatType>::minMappedValue (negative)    

    ui0WidthDenominator=-1 sets automatically ui0Width to reproduce the scale of host0Width in the UI
*/
template<class FloatType, int factor=1, int minAbsValueDenominator=100, int host0WidthDenominator=100, int ui0WidthDenominator=100>
class LogWithSignMap: public FloatTypeBaseMap<FloatType>{
  FloatType ui0Width;
  const PluginParameters_HostFloatType host0Width;
  const FloatType minAbsValue;
  const FloatType minAbsMappedValue;
  const PluginParameters_HostFloatType host0; 
protected:
  const FloatType map(const FloatType unmappedValue) const override{
    return (FloatType)(factor*log10(fabs((double)unmappedValue)));
  }
  
  const FloatType unmap(const FloatType mappedValue) const override{
    return (FloatType)(pow(10,(double)(mappedValue/factor)));   
  }  

  void updateMinMappedValue() override{
    //use class LogWith0Map to deal with FloatTypeBaseMap<FloatType>::minValue>0
    jassert(FloatTypeBaseMap<FloatType>::minValue<0);

    FloatTypeBaseMap<FloatType>::minMappedValue=map(-FloatTypeBaseMap<FloatType>::minValue);
    FloatTypeBaseMap<FloatType>::uiMin=-(FloatTypeBaseMap<FloatType>::minMappedValue-minAbsMappedValue+ui0Width);
  }

  void updateMaxMappedValue() override{
    //log values are stricly positive, please define a strictly positive range
    jassert(FloatTypeBaseMap<FloatType>::maxValue>0);

    FloatTypeBaseMap<FloatType>::maxMappedValue=map(FloatTypeBaseMap<FloatType>::maxValue);    
    FloatTypeBaseMap<FloatType>::uiMax=FloatTypeBaseMap<FloatType>::maxMappedValue-minAbsMappedValue+ui0Width;
  }      

  const PluginParameters_HostFloatType mapProcessorToHost(FloatType *processorValue) const override{
    if (FloatTypeBaseMap<FloatType>::maxMappedValue==minAbsMappedValue || FloatTypeBaseMap<FloatType>::minMappedValue==minAbsMappedValue)
      return (*processorValue==0)?host0:((*processorValue>0)?(PluginParameters_HostFloatType)(1.):(PluginParameters_HostFloatType)(0.));
          
    if (fabs(*processorValue)<=minAbsValue/2){
      *processorValue=0;
      return host0;
    }
    
    const FloatType mappedValue=map(fabs(*processorValue));

    if (*processorValue>0){
      if (mappedValue<minAbsMappedValue){
        *processorValue=minAbsValue;
        return host0+host0Width;
      }

      if (mappedValue>FloatTypeBaseMap<FloatType>::maxMappedValue){
        *processorValue=FloatTypeBaseMap<FloatType>::maxValue;
        return (PluginParameters_HostFloatType)(1.);
      }
    
      return (PluginParameters_HostFloatType)(host0+host0Width+(1-host0-host0Width)*(mappedValue-minAbsMappedValue)/(FloatTypeBaseMap<FloatType>::maxMappedValue-minAbsMappedValue));

    } else { //if (*processorValue<0)
      if (mappedValue<minAbsMappedValue){
        *processorValue=-minAbsValue;
        return host0-host0Width;
      }

      if (mappedValue>FloatTypeBaseMap<FloatType>::maxMappedValue){
        *processorValue=FloatTypeBaseMap<FloatType>::minValue;
        return (PluginParameters_HostFloatType)(0.);
      }
    
      return (PluginParameters_HostFloatType)(host0-host0Width-(host0-host0Width)*(mappedValue-minAbsMappedValue)/(FloatTypeBaseMap<FloatType>::minMappedValue-minAbsMappedValue));
    }    
  }

  const FloatType mapHostToProcessor(const PluginParameters_HostFloatType hostValue) const override{
    if (FloatTypeBaseMap<FloatType>::maxMappedValue==minAbsMappedValue || FloatTypeBaseMap<FloatType>::minMappedValue==minAbsMappedValue)
      return (hostValue==host0)?0:((hostValue>0)?FloatTypeBaseMap<FloatType>::maxValue:FloatTypeBaseMap<FloatType>::minValue);

    if (hostValue>1)
      return FloatTypeBaseMap<FloatType>::maxValue;

    if (host0Width==0 && hostValue==host0)
      return  0;

    //leave a margin of hostWidth0/2 to avoid making rounding errors confuse +/-FloatTypeBaseMap<FloatType>::minMappedValue with 0
    if (hostValue>host0+host0Width/2){
      if (hostValue<host0+host0Width)
        return minAbsValue;
      return unmap(minAbsMappedValue+(FloatType)((jmax<PluginParameters_HostFloatType>(0,hostValue-(host0+host0Width)))*(FloatTypeBaseMap<FloatType>::maxMappedValue-minAbsMappedValue)/(1-host0-host0Width)));

    } else if (hostValue<host0-host0Width/2){
      if (hostValue>host0-host0Width)
        return -minAbsValue;
      return -unmap(minAbsMappedValue+(FloatType)((host0-host0Width)-jmin<PluginParameters_HostFloatType>(host0-host0Width,hostValue))*(FloatTypeBaseMap<FloatType>::minMappedValue-minAbsMappedValue)/(host0-host0Width));

    } else {
      return 0;
    }   
  }

  const PluginParameters_HostFloatType mapUiToHost(const FloatType uiValue) const override{
    if (FloatTypeBaseMap<FloatType>::maxMappedValue==minAbsMappedValue || FloatTypeBaseMap<FloatType>::minMappedValue==minAbsMappedValue)
      return (uiValue==0)?host0:((uiValue>0)?(PluginParameters_HostFloatType)(1.):(PluginParameters_HostFloatType)(0.));
     
    if (uiValue<=-(FloatTypeBaseMap<FloatType>::minMappedValue-minAbsMappedValue+ui0Width))
      return (PluginParameters_HostFloatType)(0.);

    if (uiValue>FloatTypeBaseMap<FloatType>::maxMappedValue-minAbsMappedValue+ui0Width)
      return (PluginParameters_HostFloatType)(1.);

    if (ui0Width==0 && uiValue==0)
      return host0;

    if (uiValue>=ui0Width){
      return (PluginParameters_HostFloatType)(host0+host0Width+(1-host0-host0Width)*(uiValue-ui0Width)/(FloatTypeBaseMap<FloatType>::maxMappedValue-minAbsMappedValue));
    } else if (uiValue<=-ui0Width){
      return (PluginParameters_HostFloatType)(host0-host0Width+(host0-host0Width)*(uiValue+ui0Width)/(FloatTypeBaseMap<FloatType>::minMappedValue-minAbsMappedValue));
    } else {
      if (uiValue>ui0Width/2)
        return host0+host0Width;
      else if (uiValue<-ui0Width/2)
        return host0-host0Width;
      else
        return host0;
    }
  }

  const FloatType mapProcessorToUi(FloatType *processorValue) const override{
    if (FloatTypeBaseMap<FloatType>::maxMappedValue==minAbsMappedValue || FloatTypeBaseMap<FloatType>::minMappedValue==minAbsMappedValue)
      return (*processorValue==0)?0:((*processorValue>0)?FloatTypeBaseMap<FloatType>::uiMax:FloatTypeBaseMap<FloatType>::uiMin);

    //using the host parameter scale of [0,1]
    //[0,width0] stores the value 0
    //[width0,1] stores the values from FloatTypeBaseMap<FloatType>::minMappedValue to FloatTypeBaseMap<FloatType>::maxMappedValue  
    if (fabs(*processorValue)<=minAbsValue/2)
      return 0;
    
    const FloatType mappedValue=map(fabs(*processorValue));

    if (*processorValue>0){
      if (mappedValue<minAbsMappedValue){
        return ui0Width;
      }
      if (mappedValue>FloatTypeBaseMap<FloatType>::maxMappedValue){
        return FloatTypeBaseMap<FloatType>::uiMax;
      }
      return ui0Width+mappedValue-minAbsMappedValue;
    
    } else { //if (*processorValue<0)
      if (mappedValue<minAbsMappedValue){
        return -ui0Width;
      }
      if (mappedValue>FloatTypeBaseMap<FloatType>::minMappedValue){
        return FloatTypeBaseMap<FloatType>::uiMin;
      }
      return -(mappedValue-minAbsMappedValue+ui0Width);
    }
  }

  const FloatType mapHostToUi(const PluginParameters_HostFloatType hostValue) const override{
    if (hostValue>1)
      return FloatTypeBaseMap<FloatType>::uiMax;

    if (hostValue<0)
      return FloatTypeBaseMap<FloatType>::uiMin;

    if (host0Width==0 && hostValue==host0)
      return  0;

    //leave a margin of hostWidth0/2 to avoid making rounding errors confuse +/-FloatTypeBaseMap<FloatType>::minMappedValue with 0
    if (hostValue>host0+host0Width/2){
      if (hostValue<host0+host0Width)
        return ui0Width;
      return ui0Width+(FloatType)((jmax<PluginParameters_HostFloatType>(0,hostValue-(host0+host0Width)))*(FloatTypeBaseMap<FloatType>::maxMappedValue-minAbsMappedValue)/(1-host0-host0Width));

    } else if (hostValue<host0-host0Width/2){
      if (hostValue>host0-host0Width)
        return -ui0Width;
      return -ui0Width-(FloatType)((host0-host0Width)-jmin<PluginParameters_HostFloatType>(host0-host0Width,hostValue))*(FloatTypeBaseMap<FloatType>::maxMappedValue-minAbsMappedValue)/(host0-host0Width);

    } else {
      return 0;
    }   
  }  

public:
  LogWithSignMap(const FloatType minValue,const FloatType maxValue)
  :FloatTypeBaseMap<FloatType>(minValue,maxValue),
   ui0Width((FloatType)((ui0WidthDenominator<0)?
                        ((host0WidthDenominator<=0)?
                          0
                          :((1./jmax<int>(host0WidthDenominator,1))/(1-(1./jmax<int>(host0WidthDenominator,1)))*(map(maxValue)-map(minValue))))
                       :((ui0WidthDenominator==0)?
                          0
                          :(1./jmax<int>(ui0WidthDenominator,1))))),
   host0Width((PluginParameters_HostFloatType)((host0WidthDenominator==0)?
                                                0
                                                :(1./jmax<int>(host0WidthDenominator,1)))),
   minAbsValue((FloatType)(1./jmax<int>(minAbsValueDenominator,1))),
   minAbsMappedValue((FloatType)(factor*log10((double)(1./jmax<int>(minAbsValueDenominator,1))))),
   host0((PluginParameters_HostFloatType)((factor*log10(-(double)minValue)+factor*log10((double)(1./jmax<int>(minAbsValueDenominator,1))))/(factor*log10(-(double)minValue)+factor*log10((double)maxValue)+2*factor*log10((double)(1./jmax<int>(minAbsValueDenominator,1))))))
  {
    //FloatTypeBaseMap<FloatType>::minValue should be negative and FloatTypeBaseMap<FloatType>::maxValue positive, otherwise use LogWith0Map.
    jassert(FloatTypeBaseMap<FloatType>::minValue<0);
    jassert(FloatTypeBaseMap<FloatType>::maxValue>0);

    updateMinMappedValue();
    updateMaxMappedValue();
  }
};

/** Class that defines parameters of float type (float, double) */
template<class FloatType, class FloatTypeMap = IdentityMap<FloatType> >
class FloatTypeParam : public TypeParam<FloatType>, public FloatTypeMap{

  JUCE_DECLARE_NON_COPYABLE (FloatTypeParam)

public:  

  void setMin(FloatType minValueArg,bool updateHostFlag=true){        
    if (TypeParam<FloatType>::defaultValue>=minValueArg){      
      FloatTypeMap::minValue=minValueArg;
      FloatTypeMap::updateMinMappedValue();
      if (*TypeParam<FloatType>::value<minValueArg)
        *TypeParam<FloatType>::value=minValueArg;
      if (TypeParam<FloatType>::xmlValue<minValueArg)
        TypeParam<FloatType>::xmlValue=minValueArg;
    } else //minValue can't be set to be greater than the (constant) default value
      jassertfalse;

    if (updateHostFlag){
      Param::updateHost(false);
    }
  }

  void setMax(FloatType maxValueArg,bool updateHostFlag=true){    
    if (TypeParam<FloatType>::defaultValue<=maxValueArg){
      FloatTypeMap::maxValue=maxValueArg;
      FloatTypeMap::updateMaxMappedValue();
      if (*TypeParam<FloatType>::value>maxValueArg)
        *TypeParam<FloatType>::value=maxValueArg;
      if (TypeParam<FloatType>::xmlValue>maxValueArg)
        TypeParam<FloatType>::xmlValue=maxValueArg;
    } else //maxValue can't be set to be less than the (constant) default value
      jassertfalse;

    if (updateHostFlag){
      Param::updateHost(false);
    }
  }

  bool hostSet(const PluginParameters_HostFloatType hostValue){    
    FloatType oldValue=*TypeParam<FloatType>::value;
    *TypeParam<FloatType>::value=FloatTypeMap::mapHostToProcessor(hostValue);

    if (fabs(*TypeParam<FloatType>::value-oldValue)>PluginParameters_Epsilon)
      return true;
      
    return false;
  }
    
  /** Updates the value from its UI denormalized value */
  void updateProcessorAndHostFromUi(FloatType valueArg, UndoManager *const undoManager=nullptr, const bool dontCreateNewUndoTransaction=false, UpdateFromFlags updateFromFlag=UPDATE_FROM_UI){
    Param::updateProcessorAndHostFromNormUi(FloatTypeMap::mapUiToHost(valueArg),undoManager,dontCreateNewUndoTransaction,updateFromFlag);
  }
    
  /** Returns the parameter value to set the UI */
  FloatType uiGet() const{    
    return FloatTypeMap::mapProcessorToUi(TypeParam<FloatType>::value);
  }

  PluginParameters_HostFloatType hostGet() const{
    return FloatTypeMap::mapProcessorToHost(TypeParam<FloatType>::value);
  }
  
  const double getMin() const{
    return FloatTypeMap::uiMin;
  }

  const double getMax() const{
    return FloatTypeMap::uiMax;
  }

  void readXml(XmlElement *xml,XmlType xmlType=SESSION){    
    if ((xmlType==SESSION && Param::settings[Param::loadFromSession]) 
        || (xmlType==PRESET && Param::settings[Param::loadFromPresets])){
      
      //tell updateProcessorFromXml(...) to update *value from xmlValue
      Param::updateXml=true;
      
      if (xml==nullptr)
        TypeParam<FloatType>::xmlValue=TypeParam<FloatType>::defaultValue;
      else
        TypeParam<FloatType>::xmlValue=(FloatType)(xml->getDoubleAttribute(Param::getXmlName(),TypeParam<FloatType>::defaultValue));      

      Param::xmlHostValue=FloatTypeMap::mapProcessorToHost(&(TypeParam<FloatType>::xmlValue));
    }
  }

  void saveXml(XmlElement *xml, XmlType xmlType=SESSION) const{
    if ((!Param::settings[Param::saveOnlyNonDefaultValues] || *TypeParam<FloatType>::value!=TypeParam<FloatType>::defaultValue) 
        && ( (xmlType==SESSION && Param::settings[Param::saveToSession]) 
           || (xmlType==PRESET && Param::settings[Param::saveToPresets])) ){
      xml->setAttribute(Param::getXmlName(),(double)(*TypeParam<FloatType>::value));
    }
  }
    
  FloatTypeParam(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, FloatType * const value, const FloatType minValue=(FloatType)(0),const FloatType maxValue=(FloatType)(1)):
  TypeParam<FloatType>(name,registerAtHostFlag,loadSaveOptions,value),
  FloatTypeMap(minValue,maxValue){        
    //force *value to the [minValue,maxValue] range
    TypeParam<FloatType>::xmlValue=*TypeParam<FloatType>::value=TypeParam<FloatType>::defaultValue=jlimit<FloatType>(FloatTypeMap::minValue,FloatTypeMap::maxValue,*TypeParam<FloatType>::value);
  }
};

typedef FloatTypeParam<float> FloatParam;
typedef FloatTypeParam<float,LogMap<float,1> > LogParam;
typedef FloatTypeParam<float,LogWith0Map<float,1,0,0> > LogWith0Param;
typedef FloatTypeParam<float,LogWithSignMap<float,1,1000,10,10> > LogWithSignParam;

//-----------------------------------------------------------------------------------

/** Class that defines parameters of int type (char,int,long,etc) */
template<class IntType>
class IntTypeParam : public TypeParam<IntType>{
private:
  JUCE_DECLARE_NON_COPYABLE (IntTypeParam)

  IntType minValue;
  IntType maxValue;

protected:
  const PluginParameters_HostFloatType mapProcessorToHost(IntType *processorValue) const{
    if (maxValue==minValue)
      return (PluginParameters_HostFloatType)(0.);
    
    if (*processorValue<minValue){
      *processorValue=minValue;
      return (PluginParameters_HostFloatType)(0.);
    }

    if (*processorValue>maxValue){
      *processorValue=maxValue;
      return (PluginParameters_HostFloatType)(1.);
    }
    
    return (PluginParameters_HostFloatType)(*processorValue-minValue)/(maxValue-minValue);
  }

  const IntType mapHostToProcessor(const PluginParameters_HostFloatType hostValue) const{
    if (hostValue>1)
      return maxValue;
    if (hostValue<0)
      return minValue;

    return (IntType)(roundToInt<PluginParameters_HostFloatType>(minValue+(hostValue)*(maxValue-minValue)));
  }

  const PluginParameters_HostFloatType mapUiToHost(const IntType uiValue) const{
    if (maxValue==minValue)
      return (PluginParameters_HostFloatType)(0.);
    
    if (uiValue<minValue)
      return (PluginParameters_HostFloatType)(0.);

    if (uiValue>maxValue)
      return (PluginParameters_HostFloatType)(1.);

    return (PluginParameters_HostFloatType)(uiValue-minValue)/(maxValue-minValue);
  }

  const IntType mapProcessorToUi(IntType *processorValue) const{
    if (*processorValue>maxValue)
      return maxValue;
    if (*processorValue<minValue)
      return minValue;

    return *processorValue;
  }

  const IntType mapHostToUi(const PluginParameters_HostFloatType hostValue) const{
    if (hostValue>1)
      return maxValue;
    if (hostValue<0)
      return minValue;

    return (IntType)(roundToInt<PluginParameters_HostFloatType>(minValue+(hostValue)*(maxValue-minValue)));
  }

public:    
  
  void setMin(IntType minValueArg,bool updateHostFlag=true){        
    if (TypeParam<IntType>::defaultValue>=minValueArg){
      minValue=minValueArg;
      if (*TypeParam<IntType>::value<minValueArg)
        *TypeParam<IntType>::value=minValueArg;
      if (TypeParam<IntType>::xmlValue<minValueArg)
        TypeParam<IntType>::xmlValue=minValueArg;
    } else //minValue can't be greater than the (constant) default value
      jassertfalse;

    if (updateHostFlag){
      Param::updateHost(false);
    }
  }

  void setMax(IntType maxValueArg,bool updateHostFlag=true){    
    if (TypeParam<IntType>::defaultValue<=maxValueArg){
      maxValue=maxValueArg;
      if (*TypeParam<IntType>::value>maxValueArg)
        *TypeParam<IntType>::value=maxValueArg;
      if (TypeParam<IntType>::xmlValue>maxValueArg)
        TypeParam<IntType>::xmlValue=maxValueArg;
    } else //maxValue can't be less than the (constant) default value
      jassertfalse;
    
    if (updateHostFlag){
      Param::updateHost(false);
    }
  }

  const double getMin() const{
    return minValue;
  }

  const double getMax() const{
    return maxValue;
  }

  bool hostSet(const PluginParameters_HostFloatType hostValue){    
    IntType oldValue=*TypeParam<IntType>::value;
    *TypeParam<IntType>::value=mapHostToProcessor(hostValue);

    if (abs((int)(*TypeParam<IntType>::value-oldValue))>0)
      return true;
      
    return false;
  }
    
  /** Updates the value from its UI denormalized value */
  void updateProcessorAndHostFromUi(IntType valueArg, UndoManager *const undoManager=nullptr, const bool dontCreateNewUndoTransaction=false, UpdateFromFlags updateFromFlag=UPDATE_FROM_UI){
    Param::updateProcessorAndHostFromNormUi(mapUiToHost(valueArg),undoManager,dontCreateNewUndoTransaction,updateFromFlag);
  }
    
  /** Returns the parameter value to set the UI */
  IntType uiGet() const{    
    return mapProcessorToUi(TypeParam<IntType>::value);
  }

  PluginParameters_HostFloatType hostGet() const{
    return mapProcessorToHost(TypeParam<IntType>::value);
  }

  void readXml(XmlElement *xml,XmlType xmlType=SESSION){    
    if ((xmlType==SESSION && Param::settings[Param::loadFromSession]) 
        || (xmlType==PRESET && Param::settings[Param::loadFromPresets])){

      //tell updateProcessorFromXml(...) to update *value from xmlValue
      Param::updateXml=true;

      if (xml==nullptr)
        TypeParam<IntType>::xmlValue=TypeParam<IntType>::defaultValue;
      else
        TypeParam<IntType>::xmlValue=(IntType)(xml->getIntAttribute(Param::getXmlName(),TypeParam<IntType>::defaultValue));    

      Param::xmlHostValue=mapProcessorToHost(&(TypeParam<IntType>::xmlValue));

      if (Param::xmlHostValue<=0){
        TypeParam<IntType>::xmlValue=minValue;
      } else if (Param::xmlHostValue>=1){
        TypeParam<IntType>::xmlValue=maxValue;
      }
    }
  }

  void saveXml(XmlElement *xml, XmlType xmlType=SESSION) const{
    if ((!Param::settings[Param::saveOnlyNonDefaultValues] || *TypeParam<IntType>::value!=TypeParam<IntType>::defaultValue) 
        && ( (xmlType==SESSION && Param::settings[Param::saveToSession])
           || (xmlType==PRESET && Param::settings[Param::saveToPresets])) ){
      xml->setAttribute(Param::getXmlName(),(int)(*TypeParam<IntType>::value));
    }
  }
    
  IntTypeParam(const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, IntType * const value, const IntType minValue=0,const IntType maxValue=1):
  TypeParam<IntType>(name,registerAtHostFlag,loadSaveOptions,value),
  minValue(minValue),
  maxValue(maxValue){
    //force *value to the [minValue,maxValue] range
    TypeParam<IntType>::xmlValue=*TypeParam<IntType>::value=TypeParam<IntType>::defaultValue;
    jassert(minValue<maxValue);
  }
};

typedef IntTypeParam<int> IntParam;

//-----------------------------------------------------------------------------------

/** Class that defines parameters of bool type */
class BoolParam : public TypeParam<bool>{
  JUCE_DECLARE_NON_COPYABLE (BoolParam)

public:
  bool hostSet(const PluginParameters_HostFloatType hostValue){    
    bool oldValue=*value;
    *value=(hostValue>0.5)?true:false;

    if (*value!=oldValue)
      return true;
      
    return false;
  }
    
  /** Updates the value from its UI value */ 
  void updateProcessorAndHostFromUi(const bool valueArg, UndoManager *const undoManager=nullptr, const bool dontCreateNewUndoTransaction=false, UpdateFromFlags updateFromFlag=UPDATE_FROM_UI){  
  PluginParameters_HostFloatType newHostValue=(valueArg)?(PluginParameters_HostFloatType)(1.f):(PluginParameters_HostFloatType)(0.f);
  
  Param::updateProcessorAndHostFromNormUi(newHostValue,undoManager,dontCreateNewUndoTransaction,updateFromFlag);
}
    
  /** Returns the parameter value to set the UI */
  bool uiGet() const{  
    return *value;
  }

  PluginParameters_HostFloatType hostGet() const{
    return (*value)?(PluginParameters_HostFloatType)(1.f):(PluginParameters_HostFloatType)(0.f);
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
  TypeParam<bool>(name,registerAtHostFlag,loadSaveOptions,value)
  {
  }

};

#endif