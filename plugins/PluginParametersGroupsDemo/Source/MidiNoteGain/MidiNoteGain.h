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

#ifndef __MIDINOTEVELOCITY_H
#define __MIDINOTEVELOCITY_H

#include "../../JuceLibraryCode/JuceHeader.h"

class MidiNoteGain: public ParamGroup{
  double sampleRate;
  
public:
  bool enable;
  
  int noteVelocities[12];

  int numNotes;

  void prepareToPlay (double sampleRateArg, int /*estimatedSamplesPerBlockArg*/) {
    sampleRate=sampleRateArg;
  }

  void processBlock(AudioSampleBuffer& /*buffer*/, MidiBuffer& midiMessages){ 
    if (enable){           
      MidiBuffer oldMidiBuffer=midiMessages;
      midiMessages.clear();    
      MidiBuffer::Iterator nextMidiMessage(oldMidiBuffer);
      MidiMessage midiMessage;
      int samplePosition;
      while (nextMidiMessage.getNextEvent(midiMessage,samplePosition)){
        if (midiMessage.isNoteOn()){
          midiMessage.multiplyVelocity(noteVelocities[midiMessage.getNoteNumber()%12]/127.f);
          if (midiMessage.getVelocity()>0)
            midiMessages.addEvent(midiMessage,samplePosition);
        } else midiMessages.addEvent(midiMessage,samplePosition);
      }
    }
  }

  enum ParamGroups{
    noteVelocitiesIndex = 0
  };
  enum Params{
    enableIndex = 0
  };

  void initParameters(){
    //Parameters 
    addBoolParam(enableIndex,"enable",true,SAVE,&enable); 
    
    //Parameter Groups
    addIntParamArray(noteVelocitiesIndex,"noteVelocities",true,SAVE,noteVelocities,&numNotes,12,0,127);
  }

  MidiNoteGain():
  ParamGroup("NoteGain"),
  enable(false),
  numNotes(12){
    for (int i=0;i<12;i++)
      noteVelocities[i]=127;
  }
  
  ~MidiNoteGain(){
  }

  // (prevent copy constructor and operator= being generated..)
  // avoids warning C4512: "assignment operator could not be generated"
  MidiNoteGain (const MidiNoteGain&);
  MidiNoteGain& operator=(const MidiNoteGain &other);
};

#endif