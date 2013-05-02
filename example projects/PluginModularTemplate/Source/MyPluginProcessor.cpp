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

#include "MyPluginProcessor.h"
#include "MainComponent.h"

MyPluginProcessor::MyPluginProcessor():
  bypass(false)
{
  initAllParameters();
}

MyPluginProcessor::~MyPluginProcessor(){  
}

const String MyPluginProcessor::getParameterText (int index)
{
    // Instructions to format the parameter value
    // Now all floating numbers are written with a string of maximum 2 characters
    return String (getParameter (index), 2);
}

//------------------------------------------------------------------------------
//save all parameters in sessions and xml presets
//------------------------------------------------------------------------------

void MyPluginProcessor::getStateInformation (MemoryBlock& destData)
{   
  // You should use this method to store your parameters in the memory block.
  // Here's an example of how you can use XML to make it easy and more robust:

  // Create an outer XML element..

  XmlElement xml(JucePlugin_Name);

  // add some attributes to it..
  saveXml(&xml,false,true);  

  // then use this helper function to stuff it into the binary blob and return it..
  copyXmlToBinary (xml, destData);
}

void MyPluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
  // You should use this method to restore your parameters from this memory block,
  // whose contents will have been created by the getStateInformation() call.

  // This getXmlFromBinary() helper function retrieves our XML from the binary blob..
  ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

  // make sure that it's actually our type of XML object..
  if (xmlState != 0 && xmlState->getTagName()==JucePlugin_Name){  
           
    loadXml(xmlState, true);   
    updateProcessorHostAndUiFromXml(true,true,true);     
  }
}

//------------------------------------------------------------------------------

void MyPluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    midiDelay.prepareToPlay(sampleRate,samplesPerBlock);
    midiNoteGain.prepareToPlay(sampleRate,samplesPerBlock);
}

void MyPluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void MyPluginProcessor::reset()
{
    // Use this method as the place to clear any delay lines, buffers, etc, as it
    // means there's been a break in the audio's continuity.
    //reset the phase
}

void MyPluginProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{            
    if (!bypass){      
      midiSustain.processBlock(buffer,midiMessages);
      midiDelay.processBlock(buffer,midiMessages);
      midiNoteGain.processBlock(buffer,midiMessages);
    }
}

//------------------------------------------------------------------------------
AudioProcessorEditor* MyPluginProcessor::createEditor()
{
    return new MainComponent (this);
}

//------------------------------------------------------------------------------

const String MyPluginProcessor::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String MyPluginProcessor::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool MyPluginProcessor::isInputChannelStereoPair (int /*index*/) const
{
    return true;
}

bool MyPluginProcessor::isOutputChannelStereoPair (int /*index*/) const
{
    return true;
}

//------------------------------------------------------------------------------
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MyPluginProcessor();
}
