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
   are available: visit LINK for more information.

  ==============================================================================
*/

#ifndef __MIDISUSTAIN_H
#define __MIDISUSTAIN_H

#include "../JuceLibraryCode/JuceHeader.h"

class MidiSustain: public ParamGroup{
  bool panicFlag;
  int lastChannel;
public:
  bool enable;

  void panic(){
    panicFlag=true;
  }

  void prepareToPlay (double /*sampleRateArg*/, int /*estimatedSamplesPerBlockArg*/) {
  }

  void processBlock(AudioSampleBuffer& /*buffer*/, MidiBuffer& midiMessages){ 
    if (panicFlag){
      midiMessages.addEvent(MidiMessage::allNotesOff(lastChannel),0);
      panicFlag=false;
    }

    if (enable){           
      MidiBuffer oldMidiBuffer=midiMessages;
      midiMessages.clear();    
      MidiBuffer::Iterator nextMidiMessage(oldMidiBuffer);
      MidiMessage midiMessage;
      int samplePosition;
      while (nextMidiMessage.getNextEvent(midiMessage,samplePosition)){        
        if (!midiMessage.isNoteOff()){
          lastChannel=midiMessage.getChannel();
          midiMessages.addEvent(midiMessage,samplePosition);
        }
      }
    }
  }

  enum ParamGroups{
    none = 0
  };
  enum Params{
    enableIndex = 0
  };
  
  void initParameters(){
    //Parameters 
    addBoolParam(enableIndex,"enable",true,true,&enable);     
  }      

  void runAfterParamChange(int index,UpdateFromFlags updateFromFlag){
    if (updateFromFlag&UPDATE_FROM_XML)
      return;
    
    if (index==enableIndex && enable==false)
      panic();
  }

  MidiSustain():
  ParamGroup("MidiSustain"),
  enable(false),
  panicFlag(false),
  lastChannel(1){    
  }
  
  ~MidiSustain(){
  }

  // (prevent copy constructor and operator= being generated..)
  // avoids warning C4512: "assignment operator could not be generated"
  MidiSustain (const MidiSustain&);
  MidiSustain& operator=(const MidiSustain &other);
};

#endif