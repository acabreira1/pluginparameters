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

#ifndef __MIDIDELAY_H
#define __MIDIDELAY_H

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginParameters.h"


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

  void releaseResources(){
  }

  void reset(){
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

  void init(){
    //Parameters 
    addBoolParam(enableIndex,"enable",true,true,&enable);
    addFloatParam(delayIndex,"delay",true,true,&delay,0.001f,1.000f);
    addFloatParam(feedbackIndex,"feedback",true,true,&feedback,0.f,1.f);
  }

  MidiDelay():
  ParamGroup("MidiDelay"),
  enable(true),
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