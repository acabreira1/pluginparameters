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
   are available: visit http://www.rawmaterialsoftware.com/viewtopic.php?f=6&t=11122 
   for more information.

  ==============================================================================
*/

#ifndef __PLUGINPROCESSOR_H_526ED7A9__
#define __PLUGINPROCESSOR_H_526ED7A9__

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/

class MyPluginProcessor  : public PluginProcessor{
  
public:
    //==============================================================================
    MyPluginProcessor();
    ~MyPluginProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();
    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);
    double getTailLengthSeconds(void) const{
      return 0;
    } 

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

    String *stringArray;
    int stringArraySize;
    
    String **stringMatrix;
    int stringMatrixRows,stringMatrixCols;

    enum ParamGroups{
      intArrayIndex=0,
      intMatrixIndex,
      stringArrayIndex,
      stringMatrixIndex
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
      
    void initParameters(){        
      //Parameters   
      addFloatParam(floatIndex,"float",true,true,&floatVar,-6.f,6.f);
      addLogParam(logIndex,"log",true,true,&logVar,0.001f,6.f);
      addLogWith0Param(logWith0Index,"logWith0",true,true,&logWith0Var,0.001f,6.f);
      addLogWithSignParam(symSignedLogIndex,"symSignedLog",true,true,&symSignedLogVar,-6.f,6.f,0.001f);
      addLogWithSignParam(asymSignedLogIndex,"asymSignedLog",true,true,&asymSignedLogVar,-4.f,3.f,0.001f);        
      addIntParam(intIndex,"int",true,true,&intVar,0,4);
      addBoolParam(boolIndex,"bool",true,true,&boolVar);
      addBoolParam(boolButtonIndex,"boolButton",true,false,&boolButtonVar);
        
      //Parameter Groups
      addIntParamArray(intArrayIndex,"intArray",true,true,intArray,&intArraySize,10,0,127);
      addIntParamMatrix(intMatrixIndex,"intMatrix",true,true,intMatrix,&intMatrixRows,&intMatrixCols,10,10,0,127);
      addStringParamArray(stringArrayIndex,"stringArray",false,true,stringArray,&stringArraySize,10);
      addStringParamMatrix(stringMatrixIndex,"stringMatrix",false,true,stringMatrix,&stringMatrixRows,&stringMatrixCols,10,10);
    }     
    
    void runAfterParamChange(int paramIndex,UpdateFromFlags updateFromFlag){
      //Generally you don't want to run any updates when you are loading from XML 
      //because it is often more efficient to initialize everything all at once when 
      //all the parameter values have been loaded by overriding 
      //updateProcessorHostAndUiFromXml(...) (see below).
      if (updateFromFlag&UPDATE_FROM_XML) return;
        
      switch(paramIndex){    
        case floatIndex: {
          logVar=floatVar;
          getParam(logIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
          logWith0Var=floatVar;
          getParam(logWith0Index)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
          symSignedLogVar=floatVar;
          getParam(symSignedLogIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
          asymSignedLogVar=floatVar;
          getParam(asymSignedLogIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
          break;
        }     
        case logIndex: {
          floatVar=logVar;
          getParam(floatIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
          break;
        }
        case logWith0Index: {
          floatVar=logWith0Var;
          getParam(floatIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
          break;
        }
        case symSignedLogIndex: {
          floatVar=symSignedLogVar;
          getParam(floatIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
          break;
        }
        case asymSignedLogIndex: {
          floatVar=asymSignedLogVar;
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
    
    void updateProcessorHostAndUiFromXml(bool forceValueChanged, bool forceUpdateUi, const bool applyRecursively){
      ParamGroup::updateProcessorHostAndUiFromXml(forceValueChanged, forceUpdateUi, applyRecursively);
            
      //Initialize the plugin from the values read from XML
      logVar=floatVar;
      getParam(logIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
      logWith0Var=floatVar;
      getParam(logWith0Index)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
      symSignedLogVar=floatVar;
      getParam(symSignedLogIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
      asymSignedLogVar=floatVar;
      getParam(asymSignedLogIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
    }       

private:
    //==============================================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyPluginProcessor);
};

#endif  // __PLUGINPROCESSOR_H_526ED7A9__
