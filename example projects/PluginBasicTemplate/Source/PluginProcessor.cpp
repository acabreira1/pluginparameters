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

#include "PluginProcessor.h"
#include "MainComponent.h"

//==============================================================================
PluginProcessor::PluginProcessor():
floatVar(0),
logVar(0),
logWith0Var(0),
symSignedLogVar(0),
asymSignedLogVar(0),
intArray(nullptr),
intMatrix(nullptr)
{
    intArray=new int[10];
    for (int i=0;i<10;i++)
      intArray[i]=0;
    intArraySize=2;    

    intMatrix=new int*[10];
    for (int i=0;i<10;i++){
      intMatrix[i]=new int[10];
      for (int j=0;j<10;j++)
        intMatrix[i][j]=0;
    }
    intMatrixRows=2;
    intMatrixCols=2;

    init(new MainParamGroup(this));           
}

PluginProcessor::~PluginProcessor()
{  
  if (intArray)
    delete[] intArray;

  if (intMatrix){
    for (int i=0;i<10;i++)
      delete[] intMatrix[i];
    delete[] intMatrix;
  }
}

const String PluginProcessor::getParameterText (int index)
{
    // Instructions to format the parameter value
    // Now all floating numbers are written with a string of maximum 2 characters
    return String (getParameter (index), 2);
}

//------------------------------------------------------------------------------
//save all parameters in sessions and xml presets
//------------------------------------------------------------------------------

void PluginProcessor::getStateInformation (MemoryBlock& destData)
{   
  // You should use this method to store your parameters in the memory block.
  // Here's an example of how you can use XML to make it easy and more robust:

  // Create an outer XML element..

  XmlElement xml(JucePlugin_Name);

  // add some attributes to it..
  getParamGroup()->saveXml(&xml,false,true);  

  // then use this helper function to stuff it into the binary blob and return it..
  copyXmlToBinary (xml, destData);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
  // You should use this method to restore your parameters from this memory block,
  // whose contents will have been created by the getStateInformation() call.

  // This getXmlFromBinary() helper function retrieves our XML from the binary blob..
  ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

  // make sure that it's actually our type of XML object..
  if (xmlState != 0 && xmlState->getTagName()==JucePlugin_Name){  
           
    getParamGroup()->preLoadXml(xmlState, true);   
    getParamGroup()->updateProcessorHostAndUiFromXml(true,true,true);     
  }
}

//------------------------------------------------------------------------------

void PluginProcessor::prepareToPlay (double /*sampleRate*/, int /*samplesPerBlock*/)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void PluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void PluginProcessor::reset()
{
    // Use this method as the place to clear any delay lines, buffers, etc, as it
    // means there's been a break in the audio's continuity.
    //reset the phase
}

void PluginProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& /*midiMessages*/)
{            
    const int numSamples = buffer.getNumSamples();    
    buffer.applyGain(0,numSamples,floatVar);
}

//------------------------------------------------------------------------------
AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new MainComponent (this);
}

//------------------------------------------------------------------------------

const String PluginProcessor::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String PluginProcessor::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool PluginProcessor::isInputChannelStereoPair (int /*index*/) const
{
    return true;
}

bool PluginProcessor::isOutputChannelStereoPair (int /*index*/) const
{
    return true;
}

//------------------------------------------------------------------------------
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
