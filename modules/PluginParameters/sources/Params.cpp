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

#include "PluginProcessor.h"
#include "Params.h"

class UpdateStringParamAction : public UndoableAction{
  public:
    UpdateStringParamAction( StringParam* const stringParam,
                    const String &newValue,
                    const String &oldValue,
                    UpdateFromFlags updateFromFlag)
      :stringParam(stringParam),
       newValue(newValue),
       oldValue(oldValue),
       updateFromFlag(updateFromFlag),
       firstPerform(true)
    { jassert(stringParam!=nullptr); }

    bool perform(){
      //no need to updateUi() first (because it comes from an UI update)
      if (firstPerform){
        stringParam->updateProcessorAndHostFromUi(newValue,nullptr,false,updateFromFlag);
        firstPerform=false;
      } else {
        //result of a redo action
        stringParam->updateProcessorAndHostFromUi(newValue,nullptr,false,UPDATE_FROM_PROCESSOR);
        stringParam->updateUi();
      }
      return true;
    }

    bool undo(){
      stringParam->updateProcessorAndHostFromUi(oldValue,nullptr,false,UPDATE_FROM_PROCESSOR);
      stringParam->updateUi();
      return true;
    }

    int getSizeInUnits(){
      return (int) sizeof (*this);
    }

    Param *getParam() const{
      return static_cast<Param*>(stringParam);
    }

  private:
    StringParam *const stringParam;
    const String newValue;
    const String oldValue;
    UpdateFromFlags updateFromFlag;
    bool firstPerform;

  JUCE_DECLARE_NON_COPYABLE (UpdateStringParamAction)
};

class UpdateParamFromUiAction: public UndoableAction{
  public:
    UpdateParamFromUiAction( Param * const param,
                    PluginProcessor *const pluginProcessor,
                    const PluginParameters_HostFloatType newHostValue,
                    const PluginParameters_HostFloatType oldHostValue)
      :param(param),
       pluginProcessor(pluginProcessor),
       newHostValue(newHostValue),
       oldHostValue(oldHostValue),
       firstPerform(true)
    {}

    bool perform(){
      if (pluginProcessor==nullptr) return false;  
      //no need to updateUi() first (because it comes from an UI update)
      if (firstPerform){
        pluginProcessor->updateProcessorHostAndUi(param,newHostValue,UPDATE_FROM_UI,1,0); 
        firstPerform=false;
      } else {
        //result of a redo action
        pluginProcessor->updateProcessorHostAndUi(param,newHostValue,UPDATE_FROM_PROCESSOR,1,1); 
      }
      return true;
    }

    bool undo(){
      if (pluginProcessor==nullptr) return false;  
      pluginProcessor->updateProcessorHostAndUi(param,oldHostValue,UPDATE_FROM_PROCESSOR,1,1); 
      return true;
    }

    int getSizeInUnits(){
      return (int) sizeof (*this);
    }

    Param *getParam() const{
      return param;
    }

  private:
    Param *const param;
    PluginProcessor *const pluginProcessor;
    const PluginParameters_HostFloatType newHostValue;
    const PluginParameters_HostFloatType oldHostValue;
    bool firstPerform;

  JUCE_DECLARE_NON_COPYABLE (UpdateParamFromUiAction)
};

/*
Only methods who implicate a circular reference and therefore
can't be written in the header file are placed here.
*/

//public

void Param::updateProcessorHostAndUi(PluginParameters_HostFloatType newHostValue, UpdateFromFlags updateFromFlagArg){     
  if (pluginProcessor==nullptr) return;
  pluginProcessor->updateProcessorHostAndUi(this,newHostValue,updateFromFlagArg,1,1);    
}

void Param::updateHostAndUi(bool runAfterParamChange, UpdateFromFlags updateFromFlagArg){      
  if (pluginProcessor==nullptr) return;
  pluginProcessor->updateProcessorHostAndUi(this,hostGet(),updateFromFlagArg,(runAfterParamChange)?2:0,2);
}

void Param::updateHost(bool runAfterParamChange, UpdateFromFlags updateFromFlagArg){      
  if (pluginProcessor==nullptr) return;
  pluginProcessor->updateProcessorHostAndUi(this,hostGet(),updateFromFlagArg,(runAfterParamChange)?2:0,0);
}

void Param::updateProcessorHostAndUiFromXml(bool forceUpdateHost,bool forceUpdateUi){
  if (pluginProcessor==nullptr) return;
  if (settings[loadFromSession] || settings[loadFromPresets]){
    if (updateProcessorFromXml()){
      pluginProcessor->updateProcessorHostAndUi(this,xmlHostValue,UPDATE_FROM_PROCESSOR,0,(forceUpdateUi)?2:1); 
    } else if (forceUpdateHost){
      pluginProcessor->updateProcessorHostAndUi(this,xmlHostValue,UPDATE_FROM_PROCESSOR,0,(forceUpdateUi)?2:1);
    } else if (forceUpdateUi){
      updateUi(true);
    }   
	}
}

void Param::updateProcessorHostAndUiFromDefaultXml(bool forceUpdateHost,bool forceUpdateUi){
  if (pluginProcessor==nullptr) return;
  if (settings[loadFromSession] || settings[loadFromPresets]){
    if (updateProcessorFromDefaultXml()){
      pluginProcessor->updateProcessorHostAndUi(this,xmlHostValue,UPDATE_FROM_PROCESSOR,0,(forceUpdateUi)?2:1);
    } else if (forceUpdateHost){
      pluginProcessor->updateProcessorHostAndUi(this,xmlHostValue,UPDATE_FROM_PROCESSOR,0,(forceUpdateUi)?2:1);
    } else if (forceUpdateUi){
      updateUi(true);
    }   
  }
}

void Param::updateProcessorAndHostFromNormUi(PluginParameters_HostFloatType newHostValue, UndoManager *const undoManager, const bool dontCreateNewUndoTransaction, UpdateFromFlags updateFromFlag){
  if (pluginProcessor==nullptr) return;
  if (undoManager==nullptr){
    pluginProcessor->updateProcessorHostAndUi(this,newHostValue,updateFromFlag,1,0); 
  } else {
    if (!dontCreateNewUndoTransaction)
      undoManager->beginNewTransaction("Param: '"+getName()+ "' changed.");
    undoManager->perform(new UpdateParamFromUiAction(this,pluginProcessor,newHostValue,hostGet()));
  }
}

void Param::beginChangeGesture(){
  if (pluginProcessor==nullptr) return; 
  pluginProcessor->beginParameterChangeGesture(globalIndex);
}

void Param::endChangeGesture(){
  if (pluginProcessor==nullptr) return; 
  pluginProcessor->endParameterChangeGesture(globalIndex);
}

void StringParam::updateProcessorAndHostFromUi(const String valueArg, UndoManager *const undoManager, const bool dontCreateNewUndoTransaction, UpdateFromFlags updateFromFlag) {    
  if (*value!=valueArg){  
    if (undoManager==nullptr){
      virtualHostValue=valueArg;
      Param::updateProcessorAndHostFromNormUi((PluginParameters_HostFloatType)(0.f),undoManager,dontCreateNewUndoTransaction,updateFromFlag);   
      //this parameter can't be automated (maped as a PluginParameters_HostFloatType)
      //so hostSet doesn't do anything apart from updating *value from virtualHostValue
      //the host always is notified with 0 but this is bogus since StrinParams can not be 
      //registered at the host
    } else {
      if (dontCreateNewUndoTransaction){
        Array<const UndoableAction*> undoableActions;
        undoManager->getActionsInCurrentTransaction(undoableActions);
        const UpdateParamFromUiAction *previousAction=dynamic_cast<const UpdateParamFromUiAction*>(undoableActions.getLast());
        if (previousAction==nullptr || previousAction->getParam()!=this)
          undoManager->beginNewTransaction("Param: '"+getName()+ "' changed.");
      } else {
        undoManager->beginNewTransaction("Param: '"+getName()+ "' changed.");
      }
      undoManager->perform(new UpdateStringParamAction(this,valueArg,*value,updateFromFlag));
    }      
  }     
}