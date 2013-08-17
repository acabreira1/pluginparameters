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

#include "MyPluginProcessor.h"
#include "MainComponent.h"

MyPluginProcessor::MyPluginProcessor():
  bypass(false),
  presets(File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getChildFile(JucePlugin_Name+String(" Presets/")),"xml",true)
{
  initAllParameters();
}

MyPluginProcessor::~MyPluginProcessor(){  
}

//------------------------------------------------------------------------------

void MyPluginProcessor::getStateInformation (MemoryBlock& destData){   
  //Save all the parameter values into an XML tag with name JucePlugin_Name.
  XmlElement xml(JucePlugin_Name);
  
  getParamGroup(presetsIndex)->setSaveXml(true,true);
  getParam(selectedTabIndex)->setSaveXml(true);

  saveXml(&xml,false,true);  

  getParamGroup(presetsIndex)->setSaveXml(false,true);
  getParam(selectedTabIndex)->setSaveXml(false);

  //Save it as binary data
  copyXmlToBinary (xml, destData);
}

void MyPluginProcessor::setStateInformation (const void* data, int sizeInBytes){
  //Convert the binary data saved in getStateInformation(...) back into XML.
  ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

  // Check that it is valid XML and that the tag has name JucePlugin_Name.
  if (xmlState != 0 && xmlState->getTagName()==JucePlugin_Name){         
    getParamGroup(presetsIndex)->setLoadXml(true,true);
    getParam(selectedTabIndex)->setLoadXml(true);
    
    //Preload XML values into memory
    loadXml(xmlState, true);
    //Update the parameter values from the preloaded XML values
    updateProcessorHostAndUiFromXml(true,true,true);     
  }

  getParamGroup(presetsIndex)->setLoadXml(false,true);
  getParam(selectedTabIndex)->setLoadXml(false);
}

//------------------------------------------------------------------------------

void MyPluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock){
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    midiDelay.prepareToPlay(sampleRate,samplesPerBlock);
    midiNoteGain.prepareToPlay(sampleRate,samplesPerBlock);
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

void MyPluginProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages){            
    if (!bypass){      
      midiSustain.processBlock(buffer,midiMessages);
      midiDelay.processBlock(buffer,midiMessages);
      midiNoteGain.processBlock(buffer,midiMessages);
    }
}

//------------------------------------------------------------------------------

const String MyPluginProcessor::getParameterText (int index){
    // Instructions to format the parameter value
    // Now all floating numbers are written with a string of maximum 2 characters
    return String (getParameter (index), 2);
}

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
