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
#include "MidiNoteGain/MidiNoteGain.h"
#include "MidiDelay/MidiDelay.h"
#include "MidiSustain/MidiSustain.h"

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
        midiDelayIndex=0,
        midiNoteGainIndex,
        midiSustainIndex
      };
      
      enum Params{        
        bypassIndex=0
      };
      
      MainParamGroup( PluginProcessor *processor):
      ParamGroup(JucePlugin_Name),
      processor(processor){}
      
      void init(){        
        //Parameters   
        addBoolParam(bypassIndex,"bypass",true,true,&processor->bypass);     
        
        //SubGroups
        addParamGroup(midiDelayIndex,new MidiDelayParamGroup(&processor->midiDelay));
        addParamGroup(midiNoteGainIndex,new MidiNoteGainParamGroup(&processor->midiNoteGain));
        addParamGroup(midiSustainIndex,new MidiSustainParamGroup(&processor->midiSustain));

      }     
                               
      
    };                    
    
    //---------------------------
    //MainParamGroup Parameters
    //---------------------------
    bool bypass;
    //---------------------------    	    

    MidiDelay midiDelay;
    MidiNoteGain midiNoteGain;
    MidiSustain midiSustain;

private:
    //==============================================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor);
};

#endif  // __PLUGINPROCESSOR_H_526ED7A9__
