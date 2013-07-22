/*
  ==============================================================================

   This file is part of the PluginParameters module
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
      addBoolParam(bypassIndex,"bypass",true,true,&bypass);     
      //numTabs=lastParamGroupIndex-2
      addIntParam(selectedTabIndex,"selectedTab",false,false,&selectedTab,0,lastParamGroupIndex-2);
        
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
