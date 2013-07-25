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

#include "MyPluginProcessor.h"
#include "MainComponent.h"

//==============================================================================
MyPluginProcessor::MyPluginProcessor():
floatVar(1.f),
boolVar(false),
logVar(0),
intVar(2),
logWith0Var(0),
symSignedLogVar(0),
asymSignedLogVar(0)
{    
    initAllParameters();
}

MyPluginProcessor::~MyPluginProcessor(){  
}

//------------------------------------------------------------------------------

void MyPluginProcessor::getStateInformation (MemoryBlock& destData){   
  //Save all the parameter values into an XML tag with name JucePlugin_Name.
  XmlElement xml(JucePlugin_Name);
  saveXml(&xml,false,true);  
  //Save it as binary data
  copyXmlToBinary (xml, destData);
}

void MyPluginProcessor::setStateInformation (const void* data, int sizeInBytes){
  //Convert the binary data saved in getStateInformation(...) back into XML.
  ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

  // Check that it is valid XML and that the tag has name JucePlugin_Name.
  if (xmlState != 0 && xmlState->getTagName()==JucePlugin_Name){         
    //Preload XML values into memory
    loadXml(xmlState, true);
    //Update the parameter values from the preloaded XML values
    updateProcessorHostAndUiFromXml(true,true,true);     
  }
}

//------------------------------------------------------------------------------

const String MyPluginProcessor::getParameterText (int index){
    // Instructions to format the parameter value
    // Now all floating numbers are written with a string of maximum 2 characters
    return String (getParameter (index), 2);
}

void MyPluginProcessor::prepareToPlay (double /*sampleRate*/, int /*samplesPerBlock*/){
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MyPluginProcessor::releaseResources(){
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void MyPluginProcessor::reset(){
    // Use this method as the place to clear any delay lines, buffers, etc, as it
    // means there's been a break in the audio's continuity.
    //reset the phase
}

void MyPluginProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& /*midiMessages*/){            
    const int numSamples = buffer.getNumSamples();    
    if (!boolVar) 
      buffer.applyGain(0,numSamples,floatVar);
}

//------------------------------------------------------------------------------

const String MyPluginProcessor::getInputChannelName (const int channelIndex) const{
    return String (channelIndex + 1);
}

const String MyPluginProcessor::getOutputChannelName (const int channelIndex) const{
    return String (channelIndex + 1);
}

bool MyPluginProcessor::isInputChannelStereoPair (int /*index*/) const{
    return true;
}

bool MyPluginProcessor::isOutputChannelStereoPair (int /*index*/) const{
    return true;
}

//------------------------------------------------------------------------------
// Hook up the User Interface.
AudioProcessorEditor* MyPluginProcessor::createEditor(){
    return new MainComponent (this);
}

//------------------------------------------------------------------------------
// This creates new instances of the plugin.
AudioProcessor* JUCE_CALLTYPE createPluginFilter(){
    return new MyPluginProcessor();
}
