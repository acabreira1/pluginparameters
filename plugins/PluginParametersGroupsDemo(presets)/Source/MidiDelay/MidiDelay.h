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

#ifndef __MIDIDELAY_H
#define __MIDIDELAY_H

#include "../JuceLibraryCode/JuceHeader.h"

class MidiDelay: public ParamGroup{
  double sampleRate;
  bool notesOn[128];
  
public:
  bool enable;
  float delay;
  float feedback;
  MidiBuffer delayBuffer;
  MidiBuffer delayBufferNew;
  int delayBufferStartSample;
  
  void prepareToPlay (double sampleRateArg, int /*estimatedSamplesPerBlockArg*/) {
    sampleRate=sampleRateArg;
  }

  void applyGain(MidiBuffer& midiBuffer, float gain){
    MidiBuffer oldMidiBuffer=midiBuffer;
    midiBuffer.clear();    
    MidiBuffer::Iterator nextMidiMessage(oldMidiBuffer);
    MidiMessage midiMessage;
    int samplePosition;    
    while (nextMidiMessage.getNextEvent(midiMessage,samplePosition)){
      if (midiMessage.isNoteOn()){        
        int newVelocity=(int)(midiMessage.getVelocity()*gain);
        midiMessage.setVelocity(newVelocity/127.f);
        if (midiMessage.getVelocity()>0){
          midiBuffer.addEvent(midiMessage,samplePosition);      
          notesOn[midiMessage.getNoteNumber()]=true;
        } else{
          midiBuffer.addEvent(MidiMessage::noteOff(midiMessage.getChannel(),midiMessage.getNoteNumber()),samplePosition);
          notesOn[midiMessage.getNoteNumber()]=false;
        }
        
      } else if (midiMessage.isNoteOff()){
        if (notesOn[midiMessage.getNoteNumber()]){
          midiBuffer.addEvent(midiMessage,samplePosition);
          notesOn[midiMessage.getNoteNumber()]=false;
        }
      }
    }
  }

  void processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages){    
    if (enable){

      MidiBuffer::Iterator nextMidiMessage(midiMessages);
      MidiMessage midiMessage;
      int samplePosition;
      const int delayBufferSize=(int)(delay*sampleRate);     

      while (delayBufferStartSample+delayBufferSize<buffer.getNumSamples()){
        nextMidiMessage.setNextSamplePosition(delayBufferStartSample);
        while (nextMidiMessage.getNextEvent(midiMessage,samplePosition) && samplePosition<delayBufferStartSample+delayBufferSize){
          jassert(samplePosition-delayBufferStartSample>=0);
          delayBufferNew.addEvent(midiMessage,samplePosition-delayBufferStartSample);        
        }

        jassert(jmax<int>(0,-delayBufferStartSample)+delayBufferStartSample>=0);
        midiMessages.addEvents(delayBuffer,jmax<int>(0,-delayBufferStartSample),jmin<int>(buffer.getNumSamples()-delayBufferStartSample,buffer.getNumSamples()), delayBufferStartSample);        
        
        delayBuffer.addEvents(delayBufferNew,0,delayBufferSize,0);
        applyGain(delayBuffer,feedback);
        delayBufferNew.clear();
        delayBufferStartSample+=delayBufferSize;
      }

      if (delayBufferStartSample<buffer.getNumSamples()){
        nextMidiMessage.setNextSamplePosition (delayBufferStartSample);
        while (nextMidiMessage.getNextEvent(midiMessage,samplePosition) && samplePosition<delayBufferStartSample+delayBufferSize){
          jassert(samplePosition-delayBufferStartSample>=0);
          delayBufferNew.addEvent(midiMessage,samplePosition-delayBufferStartSample);        
        }
            
        jassert(jmax<int>(0,-delayBufferStartSample)+delayBufferStartSample>=0);
        midiMessages.addEvents(delayBuffer,jmax<int>(0,-delayBufferStartSample),jmin<int>(buffer.getNumSamples()-delayBufferStartSample,buffer.getNumSamples()), delayBufferStartSample);
        delayBufferStartSample-=buffer.getNumSamples();
      } else jassertfalse;
    }
  }  

  enum ParamGroups{
    none = 0
  };
  enum Params{  
    enableIndex = 0,                 
    delayIndex,
    feedbackIndex  
  };

  void initParameters(){
    //Parameters 
    addBoolParam(enableIndex,"enable",true,true,&enable);
    addFloatParam(delayIndex,"delay",true,true,&delay,0.001f,1.000f);
    addFloatParam(feedbackIndex,"feedback",true,true,&feedback,0.f,1.f);
  }

  MidiDelay():
  ParamGroup("Delay"),
  enable(false),
  delay(0.5f),
  feedback(0.5f),
  delayBufferStartSample(0){
    for (int i=0;i<128;i++)
      notesOn[i]=false;
  }
  
  ~MidiDelay(){
  }

  // (prevent copy constructor and operator= being generated..)
  // avoids warning C4512: "assignment operator could not be generated"
  MidiDelay (const MidiDelay&);
  MidiDelay& operator=(const MidiDelay &other);
};

#endif