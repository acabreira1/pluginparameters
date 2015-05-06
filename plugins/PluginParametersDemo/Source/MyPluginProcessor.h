/*
  ==============================================================================

  This file is part of the PluginParametersDemo plugin.
  Copyright 2012-2013 by MarC

  ------------------------------------------------------------------------------

  PluginParametersDemo is provided under the terms of The MIT License (MIT):

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.

  ------------------------------------------------------------------------------

  In order to build this Software you will need the JUCE library (juce.com)
  along with the PluginParameters module (github.com/4drX/pluginparameters),
  which have separate licenses.

  ==============================================================================
*/

#ifndef __PLUGINPROCESSOR_H_526ED7A9__
#define __PLUGINPROCESSOR_H_526ED7A9__

#include "../JuceLibraryCode/JuceHeader.h"

class MyPluginProcessor  : public PluginProcessor{
  
public:
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
    int getNumPrograms()                                                { return 1; }
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
      addFloatParam(floatIndex,"float",true,SAVE,&floatVar,-6.f,6.f);
      addLogParam(logIndex,"log",true,SAVE,&logVar,0.001f,6.f);
      addLogWith0Param(logWith0Index,"logWith0",true,SAVE,&logWith0Var,0.001f,6.f);
      addLogWithSignParam(symSignedLogIndex,"symSignedLog",true,SAVE,&symSignedLogVar,-6.f,6.f);
      addLogWithSignParam(asymSignedLogIndex,"asymSignedLog",true,SAVE,&asymSignedLogVar,-4.f,3.f);
      addIntParam(intIndex,"int",true,SAVE,&intVar,0,3);
      addBoolParam(boolIndex,"bool",true,SAVE,&boolVar);
      addBoolParam(boolButtonIndex,"boolButton",true,SAVE,&boolButtonVar);        
    }     

    /** Should return true if this parameter is a "meta" parameter.

        A meta-parameter is a parameter that changes other params. It is used
        by some hosts (e.g. AudioUnit hosts).

        By default this returns false.
    */
    bool isMetaParameter (int parameterIndex) const{
      //in this plugin all parameters except boolIndex change other parameters (floatVar or logVar)
      //parameterIndex refers to the global Parameter index, not to the parameter index in the ParamGroup
      //so it should be retrieved with the getGlobalIndex() method
      if (parameterIndex==getParam(boolIndex)->getGlobalIndex())
        return false;
      else
        return true;
    }
    
    void runAfterParamChange(int paramIndex,UpdateFromFlags /*updateFromFlag*/){
      switch(paramIndex){    
        case floatIndex: {          
          logVar=floatVar;
          runAfterParamGroupUpdate();
          intVar=0; //Custom setting
          getParam(intIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
          break;
        }     
        case logIndex: {
          floatVar=logVar;
          getParam(floatIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
          intVar=0; //Custom setting
          getParam(intIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
          break;
        }
        case logWith0Index: {
          floatVar=logWith0Var;
          getParam(floatIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
          intVar=0; //Custom Setting
          getParam(intIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
          break;
        }
        case symSignedLogIndex: {
          floatVar=symSignedLogVar;
          getParam(floatIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
          intVar=0; //Custom Setting
          getParam(intIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
          break;
        }
        case asymSignedLogIndex: {
          floatVar=asymSignedLogVar;
          getParam(floatIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
          intVar=0; //Custom Setting
          getParam(intIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
          break;
        }
        case boolButtonIndex: {
          //reset
          floatVar=1.f;
          getParam(floatIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
          runAfterParamGroupUpdate();
          intVar=2; //Linear 1.0 Setting
          getParam(intIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
          break;
        }
        case intIndex: {
          //settings
          if (intVar==1){
            floatVar=0.0f; //Linear 0.0 (Mute) Setting
            getParam(floatIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
            runAfterParamGroupUpdate();
          } else if (intVar==2){
            floatVar=1.f; //Linear 1.0 (Bypass) Setting
            getParam(floatIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
            runAfterParamGroupUpdate();   
          } else if (intVar==3){
            floatVar=2.f; //Linear 2.0 Setting
            getParam(floatIndex)->updateHostAndUi(false,UPDATE_FROM_PROCESSOR);
            runAfterParamGroupUpdate();
          }
        }
        default: break;
      }
                
    }        

    void runAfterParamGroupUpdate(){
      //updates all the other parameters with the value of logVar so that they
      //all represent the same value in different logarithmic ranges.

      //this called after a new set of values is loaded to make sure
      //that all variables represent the same value (floatVar).
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
