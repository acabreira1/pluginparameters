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

#ifndef __PLUGINPROCESSOR_H_526ED7A9__
#define __PLUGINPROCESSOR_H_526ED7A9__

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginParameters.h"

//==============================================================================
/**
*/

class PluginProcessor  : public ExtendedAudioProcessor{
  
public:
    //==============================================================================
    PluginProcessor();
    ~PluginProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();
    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);
    void reset();

    //==============================================================================
    bool hasEditor() const                  { return true; }
    AudioProcessorEditor* createEditor();

    //==============================================================================    

    const String getParameterText (int index);
    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    //==============================================================================
    int getNumPrograms()                                                { return 0; }
    int getCurrentProgram()                                             { return 0; }
    void setCurrentProgram (int /*index*/)                              { }
    const String getProgramName (int /*index*/)                         { return String::empty; }
    void changeProgramName (int /*index*/, const String& /*newName*/)   { }

    //==============================================================================    
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);          
    
    class MainParamGroup: public ParamGroup{
      PluginProcessor *processor;
    public:
      enum ParamGroups{
        intArrayIndex=0,
        intMatrixIndex
      };
      
      enum Params{        
        floatIndex=0,
        logIndex,
        logWith0Index,
        symSignedLogIndex,
        asymSignedLogIndex,
        intIndex,
        boolIndex,
        boolButtonIndex   
      };
      
      MainParamGroup( PluginProcessor *processor):
      ParamGroup(JucePlugin_Name),
      processor(processor){}
      
      void init(){        
        //Parameters   
        addFloatParam(floatIndex,"float",true,true,&processor->floatVar,-6.f,6.f);
        addLogParam(logIndex,"logIndex",true,true,&processor->logVar,0.001f,6.f);
        addLogWith0Param(logWith0Index,"logWith0Index",true,true,&processor->logWith0Var,0.001f,6.f);
        addLogWithSignParam(symSignedLogIndex,"symSignedLogIndex",true,true,&processor->symSignedLogVar,-6.f,6.f,0.001f);
        addLogWithSignParam(asymSignedLogIndex,"asymSignedLogIndex",true,true,&processor->asymSignedLogVar,-4.f,3.f,0.001f);        
        addIntParam(intIndex,"int",true,true,&processor->intVar,0,4);
        addBoolParam(boolIndex,"bool",true,true,&processor->boolVar);
        addBoolParam(boolButtonIndex,"boolButton",true,false,&processor->boolButtonVar);
        
        //SubGroups
        addParamGroup(intArrayIndex,new IntParamArray("intArray",true,true,processor->intArray,&processor->intArraySize,10,0,127));
        addParamGroup(intMatrixIndex,new IntParamMatrix("intMatrix",true,true,processor->intMatrix,&processor->intMatrixRows,&processor->intMatrixCols,10,10,0,127));
      }     
      
      void runAfterParamChange(int paramIndex,UpdateFromFlags updateFromFlag){
        if (updateFromFlag&UPDATE_FROM_XML)
          return;
        
        switch(paramIndex){    
          case floatIndex: {
            processor->logVar=processor->floatVar;
            getParam(logIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
            processor->logWith0Var=processor->floatVar;
            getParam(logWith0Index)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
            processor->symSignedLogVar=processor->floatVar;
            getParam(symSignedLogIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
            processor->asymSignedLogVar=processor->floatVar;
            getParam(asymSignedLogIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
            break;
          }     
          case logIndex: {
            processor->floatVar=processor->logVar;
            getParam(floatIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
            break;
          }
          case logWith0Index: {
            processor->floatVar=processor->logWith0Var;
            getParam(floatIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
            break;
          }
          case symSignedLogIndex: {
            processor->floatVar=processor->symSignedLogVar;
            getParam(floatIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
            break;
          }
          case asymSignedLogIndex: {
            processor->floatVar=processor->asymSignedLogVar;
            getParam(floatIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
            break;
          }
          case boolButtonIndex: {
            AlertWindow::showMessageBox(AlertWindow::WarningIcon,"Warning!","Aaaaaah!!");
            break;
          }
          default: break;
        }
                
      }                  
      
    };                     
    
    //---------------------------
    //MainParamGroup Parameters
    //---------------------------
    float floatVar;
    float logVar;
    float logWith0Var;
    float symSignedLogVar;
    float asymSignedLogVar;
    int intVar;
    bool boolVar;
    bool boolButtonVar;

    int* intArray;
    int intArraySize;

    int** intMatrix;
    int intMatrixRows,intMatrixCols;
    //---------------------------    	    

private:
    //==============================================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor);
};

#endif  // __PLUGINPROCESSOR_H_526ED7A9__
