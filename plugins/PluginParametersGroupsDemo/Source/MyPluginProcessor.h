/*
  ==============================================================================

  This file is part of the PluginParametersGroupsDemo plugin.
  Copyright 2012-2013 by MarC

  ------------------------------------------------------------------------------

  PluginParametersGroupsDemo is provided under the terms of The MIT License (MIT):

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
#include "MidiNoteGain/MidiNoteGain.h"
#include "MidiDelay/MidiDelay.h"
#include "MidiSustain/MidiSustain.h"

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
      return midiDelay.delay;
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
   
    bool bypass;    
    int selectedTab;

    //Presets presets;
    MidiDelay midiDelay;
    MidiNoteGain midiNoteGain;
    MidiSustain midiSustain;    
   
    enum ParamGroups{
      //presetsIndex=0,
      midiDelayIndex=0,
      midiNoteGainIndex,
      midiSustainIndex,
      lastParamGroupIndex
    };
      
    enum Params{        
      bypassIndex=0,
      selectedTabIndex
    };
           
    void initParameters(){        
      //Parameters   
      addBoolParam(bypassIndex,"bypass",true,SAVE,&bypass);     
      //numTabs=lastParamGroupIndex-1
      addIntParam(selectedTabIndex,"selectedTab",false,SAVE_ONLY_IN_SESSION,&selectedTab,0,lastParamGroupIndex-1);
        
      //Parameter Groups
      //addParamGroup(presetsIndex,&presets);
      addParamGroup(midiDelayIndex,&midiDelay);
      addParamGroup(midiNoteGainIndex,&midiNoteGain);
      addParamGroup(midiSustainIndex,&midiSustain);
    }

    void runAfterNonSavedChangesChange(){
      //presets.setNonSavedChanges(true);
    }
       
private:
    //==============================================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyPluginProcessor);
};

#endif  // __PLUGINPROCESSOR_H_526ED7A9__
