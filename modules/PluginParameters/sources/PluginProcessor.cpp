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

/** Executes host paramChanges and runAfterParamChanges in a different thread

    if runAfterParamChange==0: runAfterParamChange will never be called.
    if runAfterParamChange==1: runAfterParamChange will be called if there's a value change.
    if runAfterParamChange==2: runAfterParamChange will be called in any case.
    if runAfterParamChange==3: execute paramChange and runAfterParamChange will be called if 
                               there's a value change.. (scheduled at setParameter(...))
      
    if updateUi==0: no updateUi is requested.
    if updateUi==1: an updateUi is requested if there's a value change and runAfterParamChange==1.
    if updateUi==2: an updateUi is requested in any case.
  */ 
ThreadPoolJob::JobStatus runAfterParamChangeThread::runJob(){
  switch (runAfterParamChange){
    case 3:
      if (param->hostSet(newValue)){
        //register non saved changes in the ParamGroup
        if (param->getOption(Param::saveToPresets))
          paramGroup->setNonSavedChanges(true);
        paramGroup->runAfterParamChange(groupIndex,updateFromFlag);
        if (paramGroup->getParentParamGroup()!=nullptr)
          paramGroup->getParentParamGroup()->runAfterParamGroupChange(paramGroup->getIndex(),groupIndex,updateFromFlag);
        if (updateUi>=1)
          param->updateUi(true);
      } else {
        if (param->getOption(Param::forceRunAfterParamChangeInHost)){
          paramGroup->runAfterParamChange(groupIndex,updateFromFlag);
          if (paramGroup->getParentParamGroup()!=nullptr)
            paramGroup->getParentParamGroup()->runAfterParamGroupChange(paramGroup->getIndex(),groupIndex,updateFromFlag);
        }
        if (updateUi>1){
          param->updateUi(true);
        }
      }
      break;
    case 2:
      paramGroup->runAfterParamChange(groupIndex,updateFromFlag);
      if (paramGroup->getParentParamGroup()!=nullptr)
        paramGroup->getParentParamGroup()->runAfterParamGroupChange(paramGroup->getIndex(),groupIndex,updateFromFlag);  
      if (updateUi>=1){
        param->updateUi(true);
      }
      break;
    case 1:
      if (param->hostSet(newValue)){
        if (param->getOption(Param::saveToPresets))
          paramGroup->setNonSavedChanges(true);
        paramGroup->runAfterParamChange(groupIndex,updateFromFlag);
        if (paramGroup->getParentParamGroup()!=nullptr)
          paramGroup->getParentParamGroup()->runAfterParamGroupChange(paramGroup->getIndex(),groupIndex,updateFromFlag);
        if (updateUi>=1)
          param->updateUi(true);
      } else if (updateUi==2){
        param->updateUi(true);
      }
      break;
    case 0:
      if (updateUi>=1){
        param->updateUi(true);
      }
      break;
    default:
      break;
  }  
  return ThreadPoolJob::jobHasFinished;
}

void PluginProcessor::setParameter (int index, float newValue){
  if (index>=0 && index<getNumAutomatedParams()){
    ParamGroup * const localParamGroup=groupAutomated[index];
    const int groupIndex=indexInGroupAutomated[index];
    Param * const param=localParamGroup->getParam(groupIndex);
        
    if (param->getOption(Param::createThreadForParamChange)){
      //create a different thread to run runAfterParamChange(..) and runAfterParamGroupChange(...)
      runAfterParamChangeThreads.addJob(new runAfterParamChangeThread(groupIndex,localParamGroup,param,newValue,UPDATE_FROM_HOST,3,1),true);
    } else {
      if (param->hostSet(newValue)){
        //register non saved changes in the ParamGroup
        if (param->getOption(Param::saveToPresets))
          localParamGroup->setNonSavedChanges(true);
        localParamGroup->runAfterParamChange(groupIndex,UPDATE_FROM_HOST);
        if (localParamGroup->getParentParamGroup()!=nullptr)
          localParamGroup->getParentParamGroup()->runAfterParamGroupChange(localParamGroup->getIndex(),groupIndex,UPDATE_FROM_HOST);
        param->updateUi(true);
      } else if (param->getOption(Param::forceRunAfterParamChangeInHost)){
        localParamGroup->runAfterParamChange(groupIndex,UPDATE_FROM_HOST);
        if (localParamGroup->getParentParamGroup()!=nullptr)
          localParamGroup->getParentParamGroup()->runAfterParamGroupChange(localParamGroup->getIndex(),groupIndex,UPDATE_FROM_HOST);
      }
    }
  } 
}