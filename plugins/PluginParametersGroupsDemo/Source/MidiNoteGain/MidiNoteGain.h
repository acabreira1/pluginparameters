/*
  ==============================================================================

  Copyright 2012-13 by MarC

  ------------------------------------------------------------------------------

   This file can be redistributed and/or modified under the terms of the GNU 
   General Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   This file is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
   
  ------------------------------------------------------------------------------

  ==============================================================================
*/

#ifndef __MIDINOTEVELOCITY_H
#define __MIDINOTEVELOCITY_H

#include "../JuceLibraryCode/JuceHeader.h"

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
    addBoolParam(enableIndex,"enable",true,true,&enable); 
    
    //Parameter Groups
    addIntParamArray(noteVelocitiesIndex,"noteVelocities",true,true,noteVelocities,&numNotes,12,0,127);
  }

  MidiNoteGain():
  ParamGroup("MidiNoteGain"),
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