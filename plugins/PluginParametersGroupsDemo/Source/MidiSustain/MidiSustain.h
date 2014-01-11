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
    addBoolParam(enableIndex,"enable",true,SAVE,&enable);     
  }  

  void runAfterParamChange(int index,UpdateFromFlags /*updateFromFlag*/){
    if (index==enableIndex && enable==false)
      panic();
  }

  MidiSustain():
  ParamGroup("Sustain"),
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