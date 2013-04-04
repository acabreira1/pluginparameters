/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  4 Apr 2013 12:13:07am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
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
//[/Headers]

#include "MidiDelayComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MidiDelayComponent::MidiDelayComponent (ParamGroup *paramGroup)
    : paramGroup(paramGroup),
      label2 (0),
      delaySlider (0),
      label3 (0),
      feedbackSlider (0),
      enableToggle (0)
{
    addAndMakeVisible (label2 = new Label (L"new label",
                                           L"Delay"));
    label2->setFont (Font (15.0000f, Font::plain));
    label2->setJustificationType (Justification::centred);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (delaySlider = new Slider (L"new slider"));
    delaySlider->setRange (0.001, 10, 0.001);
    delaySlider->setSliderStyle (Slider::Rotary);
    delaySlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    delaySlider->addListener (this);

    addAndMakeVisible (label3 = new Label (L"new label",
                                           L"Feedback"));
    label3->setFont (Font (15.0000f, Font::plain));
    label3->setJustificationType (Justification::centred);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (feedbackSlider = new Slider (L"new slider"));
    feedbackSlider->setRange (0, 10, 0.001);
    feedbackSlider->setSliderStyle (Slider::Rotary);
    feedbackSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    feedbackSlider->addListener (this);

    addAndMakeVisible (enableToggle = new ToggleButton (L"new toggle button"));
    enableToggle->setButtonText (L"on/off");
    enableToggle->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (800, 120);


    //[Constructor] You can add your own custom stuff here..
    FloatParam *delayParam=paramGroup->getFloatParam(MidiDelayParamGroup::delayIndex);
    FloatParam *feedbackParam=paramGroup->getFloatParam(MidiDelayParamGroup::feedbackIndex);
    delaySlider->setRange(delayParam->getMin(),delayParam->getMax(),delaySlider->getInterval());
    feedbackSlider->setRange(feedbackParam->getMin(),feedbackParam->getMax(),feedbackSlider->getInterval());

    timerCallback();
    //[/Constructor]
}

MidiDelayComponent::~MidiDelayComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (label2);
    deleteAndZero (delaySlider);
    deleteAndZero (label3);
    deleteAndZero (feedbackSlider);
    deleteAndZero (enableToggle);


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MidiDelayComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffc4c4c4));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MidiDelayComponent::resized()
{
    label2->setBounds (11, 91, 56, 23);
    delaySlider->setBounds (8, 28, 63, 56);
    label3->setBounds (87, 92, 72, 22);
    feedbackSlider->setBounds (90, 28, 63, 56);
    enableToggle->setBounds (7, 0, 58, 22);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MidiDelayComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == delaySlider)
    {
        //[UserSliderCode_delaySlider] -- add your slider handling code here..
        paramGroup->getFloatParam(MidiDelayParamGroup::delayIndex)->updateProcessorAndHostFromUi(delaySlider->getValue());
        //[/UserSliderCode_delaySlider]
    }
    else if (sliderThatWasMoved == feedbackSlider)
    {
        //[UserSliderCode_feedbackSlider] -- add your slider handling code here..
        paramGroup->getFloatParam(MidiDelayParamGroup::feedbackIndex)->updateProcessorAndHostFromUi(feedbackSlider->getValue());
        //[/UserSliderCode_feedbackSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void MidiDelayComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == enableToggle)
    {
        //[UserButtonCode_enableToggle] -- add your button handler code here..
        paramGroup->getBoolParam(MidiDelayParamGroup::enableIndex)->updateProcessorAndHostFromUi(enableToggle->getToggleState());
        //[/UserButtonCode_enableToggle]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MidiDelayComponent::timerCallback()
{
    BoolParam *enableParam=paramGroup->getBoolParam(MidiDelayParamGroup::enableIndex);
    if (enableToggle && enableParam->updateUiRequested()){
      enableToggle->setToggleState(enableParam->uiGet(),false);
    }

    FloatParam *delayParam=paramGroup->getFloatParam(MidiDelayParamGroup::delayIndex);
    if (delaySlider && delayParam->updateUiRequested()){
      delaySlider->setValue (delayParam->uiGet(),false);
    }

    FloatParam *feedbackParam=paramGroup->getFloatParam(MidiDelayParamGroup::feedbackIndex);
    if (feedbackSlider && feedbackParam->updateUiRequested()){
      feedbackSlider->setValue (feedbackParam->uiGet(),false);
    }
}


//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MidiDelayComponent" componentName=""
                 parentClasses="public Component" constructorParams="ParamGroup *paramGroup"
                 variableInitialisers="paramGroup(paramGroup)" snapPixels="8"
                 snapActive="1" snapShown="1" overlayOpacity="0.330000013" fixedSize="1"
                 initialWidth="800" initialHeight="120">
  <BACKGROUND backgroundColour="ffc4c4c4"/>
  <LABEL name="new label" id="4b0d09074b5a8fae" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="11 91 56 23" edTextCol="ff000000"
         edBkgCol="0" labelText="Delay" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="new slider" id="55cf3e64187acaa2" memberName="delaySlider"
          virtualName="" explicitFocusOrder="0" pos="8 28 63 56" min="0.001"
          max="10" int="0.001" style="Rotary" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="3e5302becd1c6e38" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="87 92 72 22" edTextCol="ff000000"
         edBkgCol="0" labelText="Feedback" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="new slider" id="5ba66a53049acbe5" memberName="feedbackSlider"
          virtualName="" explicitFocusOrder="0" pos="90 28 63 56" min="0"
          max="10" int="0.001" style="Rotary" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <TOGGLEBUTTON name="new toggle button" id="6cce19736891af98" memberName="enableToggle"
                virtualName="" explicitFocusOrder="0" pos="7 0 58 22" buttonText="on/off"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
