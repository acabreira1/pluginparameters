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

#ifndef __MIDISUSTAIN_H
#define __MIDISUSTAIN_H

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginParameters.h"

class MidiSustain{
  bool panicFlag;
  int lastChannel;
public:
  bool enable;

  void panic(){
    panicFlag=true;
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

  MidiSustain():
  enable(true),
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

class MidiSustainParamGroup: public ParamGroup{
  MidiSustain *midiSustain;
public:
  enum ParamGroups{
    none = 0
  };
  enum Params{
    enableIndex = 0
  };
      
  MidiSustainParamGroup(MidiSustain *midiSustain):
  ParamGroup("MidiSustain"),
  midiSustain(midiSustain)  {
  }
      
  void init(){
    //Parameters 
    addBoolParam(enableIndex,"enable",true,true,&midiSustain->enable);     
  }      

  void runAfterParamChange(int index,UpdateFromFlags updateFromFlag){
    if (updateFromFlag&UPDATE_FROM_XML)
      return;
    
    if (index==enableIndex && midiSustain->enable==false)
      midiSustain->panic();
  }

};

#endif