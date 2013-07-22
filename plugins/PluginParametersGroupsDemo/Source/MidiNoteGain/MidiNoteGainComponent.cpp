/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
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

#include "MyPluginProcessor.h"
//[/Headers]

#include "MidiNoteGainComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MidiNoteGainComponent::MidiNoteGainComponent (MidiNoteGain *midiNoteGain)
    : midiNoteGain(midiNoteGain)
{
    addAndMakeVisible (label12 = new Label ("new label",
                                            "B"));
    label12->setFont (Font (15.00f, Font::plain));
    label12->setJustificationType (Justification::centred);
    label12->setEditable (false, false, false);
    label12->setColour (TextEditor::textColourId, Colours::black);
    label12->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label2 = new Label ("new label",
                                           "C"));
    label2->setFont (Font (15.00f, Font::plain));
    label2->setJustificationType (Justification::centred);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider1 = new Slider ("new slider"));
    slider1->setRange (0, 10, 1);
    slider1->setSliderStyle (Slider::Rotary);
    slider1->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider1->addListener (this);

    addAndMakeVisible (label3 = new Label ("new label",
                                           "Db"));
    label3->setFont (Font (15.00f, Font::plain));
    label3->setJustificationType (Justification::centred);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider2 = new Slider ("new slider"));
    slider2->setRange (0, 10, 1);
    slider2->setSliderStyle (Slider::Rotary);
    slider2->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider2->addListener (this);

    addAndMakeVisible (label4 = new Label ("new label",
                                           "D"));
    label4->setFont (Font (15.00f, Font::plain));
    label4->setJustificationType (Justification::centred);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider3 = new Slider ("new slider"));
    slider3->setRange (0, 10, 1);
    slider3->setSliderStyle (Slider::Rotary);
    slider3->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider3->addListener (this);

    addAndMakeVisible (label5 = new Label ("new label",
                                           "Eb"));
    label5->setFont (Font (15.00f, Font::plain));
    label5->setJustificationType (Justification::centred);
    label5->setEditable (false, false, false);
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider4 = new Slider ("new slider"));
    slider4->setRange (0, 10, 1);
    slider4->setSliderStyle (Slider::Rotary);
    slider4->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider4->addListener (this);

    addAndMakeVisible (label6 = new Label ("new label",
                                           "E"));
    label6->setFont (Font (15.00f, Font::plain));
    label6->setJustificationType (Justification::centred);
    label6->setEditable (false, false, false);
    label6->setColour (TextEditor::textColourId, Colours::black);
    label6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider5 = new Slider ("new slider"));
    slider5->setRange (0, 10, 1);
    slider5->setSliderStyle (Slider::Rotary);
    slider5->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider5->addListener (this);

    addAndMakeVisible (label7 = new Label ("new label",
                                           "F"));
    label7->setFont (Font (15.00f, Font::plain));
    label7->setJustificationType (Justification::centred);
    label7->setEditable (false, false, false);
    label7->setColour (TextEditor::textColourId, Colours::black);
    label7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider6 = new Slider ("new slider"));
    slider6->setRange (0, 10, 1);
    slider6->setSliderStyle (Slider::Rotary);
    slider6->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider6->addListener (this);

    addAndMakeVisible (label8 = new Label ("new label",
                                           "Gb"));
    label8->setFont (Font (15.00f, Font::plain));
    label8->setJustificationType (Justification::centred);
    label8->setEditable (false, false, false);
    label8->setColour (TextEditor::textColourId, Colours::black);
    label8->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider7 = new Slider ("new slider"));
    slider7->setRange (0, 10, 1);
    slider7->setSliderStyle (Slider::Rotary);
    slider7->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider7->addListener (this);

    addAndMakeVisible (label9 = new Label ("new label",
                                           "G"));
    label9->setFont (Font (15.00f, Font::plain));
    label9->setJustificationType (Justification::centred);
    label9->setEditable (false, false, false);
    label9->setColour (TextEditor::textColourId, Colours::black);
    label9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider8 = new Slider ("new slider"));
    slider8->setRange (0, 10, 1);
    slider8->setSliderStyle (Slider::Rotary);
    slider8->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider8->addListener (this);

    addAndMakeVisible (label10 = new Label ("new label",
                                            "A"));
    label10->setFont (Font (15.00f, Font::plain));
    label10->setJustificationType (Justification::centred);
    label10->setEditable (false, false, false);
    label10->setColour (TextEditor::textColourId, Colours::black);
    label10->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider10 = new Slider ("new slider"));
    slider10->setRange (0, 10, 1);
    slider10->setSliderStyle (Slider::Rotary);
    slider10->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider10->addListener (this);

    addAndMakeVisible (label11 = new Label ("new label",
                                            "Bb"));
    label11->setFont (Font (15.00f, Font::plain));
    label11->setJustificationType (Justification::centred);
    label11->setEditable (false, false, false);
    label11->setColour (TextEditor::textColourId, Colours::black);
    label11->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider11 = new Slider ("new slider"));
    slider11->setRange (0, 10, 1);
    slider11->setSliderStyle (Slider::Rotary);
    slider11->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider11->addListener (this);

    addAndMakeVisible (slider12 = new Slider ("new slider"));
    slider12->setRange (0, 10, 1);
    slider12->setSliderStyle (Slider::Rotary);
    slider12->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider12->addListener (this);

    addAndMakeVisible (enableToggle = new ToggleButton ("new toggle button"));
    enableToggle->setButtonText ("on/off");
    enableToggle->addListener (this);

    addAndMakeVisible (label13 = new Label ("new label",
                                            "Ab\n"));
    label13->setFont (Font (15.00f, Font::plain));
    label13->setJustificationType (Justification::centred);
    label13->setEditable (false, false, false);
    label13->setColour (TextEditor::textColourId, Colours::black);
    label13->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider9 = new Slider ("new slider"));
    slider9->setRange (0, 10, 1);
    slider9->setSliderStyle (Slider::Rotary);
    slider9->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider9->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (800, 120);


    //[Constructor] You can add your own custom stuff here..
    IntParam *param;
    param=midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(1);
    slider1->setRange(param->getMin(),param->getMax(),slider1->getInterval());
    slider2->setRange(param->getMin(),param->getMax(),slider2->getInterval());
    slider3->setRange(param->getMin(),param->getMax(),slider3->getInterval());
    slider4->setRange(param->getMin(),param->getMax(),slider4->getInterval());
    slider5->setRange(param->getMin(),param->getMax(),slider5->getInterval());
    slider6->setRange(param->getMin(),param->getMax(),slider6->getInterval());
    slider7->setRange(param->getMin(),param->getMax(),slider7->getInterval());
    slider8->setRange(param->getMin(),param->getMax(),slider8->getInterval());
    slider9->setRange(param->getMin(),param->getMax(),slider9->getInterval());
    slider10->setRange(param->getMin(),param->getMax(),slider10->getInterval());
    slider11->setRange(param->getMin(),param->getMax(),slider11->getInterval());
    slider12->setRange(param->getMin(),param->getMax(),slider12->getInterval());

    timerCallback();
    //[/Constructor]
}

MidiNoteGainComponent::~MidiNoteGainComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label12 = nullptr;
    label2 = nullptr;
    slider1 = nullptr;
    label3 = nullptr;
    slider2 = nullptr;
    label4 = nullptr;
    slider3 = nullptr;
    label5 = nullptr;
    slider4 = nullptr;
    label6 = nullptr;
    slider5 = nullptr;
    label7 = nullptr;
    slider6 = nullptr;
    label8 = nullptr;
    slider7 = nullptr;
    label9 = nullptr;
    slider8 = nullptr;
    label10 = nullptr;
    slider10 = nullptr;
    label11 = nullptr;
    slider11 = nullptr;
    slider12 = nullptr;
    enableToggle = nullptr;
    label13 = nullptr;
    slider9 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MidiNoteGainComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MidiNoteGainComponent::resized()
{
    label12->setBounds (730, 92, 72, 23);
    label2->setBounds (11, 91, 56, 24);
    slider1->setBounds (8, 28, 63, 56);
    label3->setBounds (70, 92, 72, 23);
    slider2->setBounds (73, 28, 63, 56);
    label4->setBounds (142, 91, 56, 24);
    slider3->setBounds (139, 28, 63, 56);
    label5->setBounds (202, 92, 72, 23);
    slider4->setBounds (205, 28, 63, 56);
    label6->setBounds (274, 91, 56, 24);
    slider5->setBounds (271, 28, 63, 56);
    label7->setBounds (333, 92, 72, 23);
    slider6->setBounds (336, 28, 63, 56);
    label8->setBounds (405, 91, 56, 24);
    slider7->setBounds (402, 28, 63, 56);
    label9->setBounds (466, 92, 72, 23);
    slider8->setBounds (469, 28, 63, 56);
    label10->setBounds (604, 91, 56, 24);
    slider10->setBounds (601, 28, 63, 56);
    label11->setBounds (664, 92, 72, 23);
    slider11->setBounds (667, 28, 63, 56);
    slider12->setBounds (733, 28, 63, 56);
    enableToggle->setBounds (7, 3, 58, 22);
    label13->setBounds (538, 91, 56, 24);
    slider9->setBounds (535, 28, 63, 56);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MidiNoteGainComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider1)
    {
        //[UserSliderCode_slider1] -- add your slider handling code here..
        midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(0)->updateProcessorAndHostFromUi((int)slider1->getValue());
        //[/UserSliderCode_slider1]
    }
    else if (sliderThatWasMoved == slider2)
    {
        //[UserSliderCode_slider2] -- add your slider handling code here..
        midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(1)->updateProcessorAndHostFromUi((int)slider2->getValue());
        //[/UserSliderCode_slider2]
    }
    else if (sliderThatWasMoved == slider3)
    {
        //[UserSliderCode_slider3] -- add your slider handling code here..
        midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(2)->updateProcessorAndHostFromUi((int)slider3->getValue());
        //[/UserSliderCode_slider3]
    }
    else if (sliderThatWasMoved == slider4)
    {
        //[UserSliderCode_slider4] -- add your slider handling code here..
        midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(3)->updateProcessorAndHostFromUi((int)slider4->getValue());
        //[/UserSliderCode_slider4]
    }
    else if (sliderThatWasMoved == slider5)
    {
        //[UserSliderCode_slider5] -- add your slider handling code here..
        midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(4)->updateProcessorAndHostFromUi((int)slider5->getValue());
        //[/UserSliderCode_slider5]
    }
    else if (sliderThatWasMoved == slider6)
    {
        //[UserSliderCode_slider6] -- add your slider handling code here..
        midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(5)->updateProcessorAndHostFromUi((int)slider6->getValue());
        //[/UserSliderCode_slider6]
    }
    else if (sliderThatWasMoved == slider7)
    {
        //[UserSliderCode_slider7] -- add your slider handling code here..
        midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(6)->updateProcessorAndHostFromUi((int)slider7->getValue());
        //[/UserSliderCode_slider7]
    }
    else if (sliderThatWasMoved == slider8)
    {
        //[UserSliderCode_slider8] -- add your slider handling code here..
        midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(7)->updateProcessorAndHostFromUi((int)slider8->getValue());
        //[/UserSliderCode_slider8]
    }
    else if (sliderThatWasMoved == slider10)
    {
        //[UserSliderCode_slider10] -- add your slider handling code here..
        midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(9)->updateProcessorAndHostFromUi((int)slider10->getValue());
        //[/UserSliderCode_slider10]
    }
    else if (sliderThatWasMoved == slider11)
    {
        //[UserSliderCode_slider11] -- add your slider handling code here..
        midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(10)->updateProcessorAndHostFromUi((int)slider11->getValue());
        //[/UserSliderCode_slider11]
    }
    else if (sliderThatWasMoved == slider12)
    {
        //[UserSliderCode_slider12] -- add your slider handling code here..
        midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(11)->updateProcessorAndHostFromUi((int)slider12->getValue());
        //[/UserSliderCode_slider12]
    }
    else if (sliderThatWasMoved == slider9)
    {
        //[UserSliderCode_slider9] -- add your slider handling code here..
        midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(8)->updateProcessorAndHostFromUi((int)slider9->getValue());
        //[/UserSliderCode_slider9]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void MidiNoteGainComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == enableToggle)
    {
        //[UserButtonCode_enableToggle] -- add your button handler code here..
        midiNoteGain->getBoolParam(MidiNoteGain::enableIndex)->updateProcessorAndHostFromUi(enableToggle->getToggleState());
        //[/UserButtonCode_enableToggle]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MidiNoteGainComponent::timerCallback()
{
    BoolParam *enableParam=midiNoteGain->getBoolParam(MidiNoteGain::enableIndex);
    if (enableToggle && enableParam->updateUiRequested()){
      enableToggle->setToggleState(enableParam->uiGet(),dontSendNotification);
    }

    IntParam *param;
    param=midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(0);
    if (slider1 && param->updateUiRequested()){
      slider1->setValue (param->uiGet(),dontSendNotification);
    }

    param=midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(1);
    if (slider2 && param->updateUiRequested()){
      slider2->setValue (param->uiGet(),dontSendNotification);
    }

    param=midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(2);
    if (slider3 && param->updateUiRequested()){
      slider3->setValue (param->uiGet(),dontSendNotification);
    }

    param=midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(3);
    if (slider4 && param->updateUiRequested()){
      slider4->setValue (param->uiGet(),dontSendNotification);
    }

    param=midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(4);
    if (slider5 && param->updateUiRequested()){
      slider5->setValue (param->uiGet(),dontSendNotification);
    }

    param=midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(5);
    if (slider6 && param->updateUiRequested()){
      slider6->setValue (param->uiGet(),dontSendNotification);
    }

    param=midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(6);
    if (slider7 && param->updateUiRequested()){
      slider7->setValue (param->uiGet(),dontSendNotification);
    }

    param=midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(7);
    if (slider8 && param->updateUiRequested()){
      slider8->setValue (param->uiGet(),dontSendNotification);
    }

    param=midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(8);
    if (slider9 && param->updateUiRequested()){
      slider9->setValue (param->uiGet(),dontSendNotification);
    }

    param=midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(9);
    if (slider10 && param->updateUiRequested()){
      slider10->setValue (param->uiGet(),dontSendNotification);
    }

    param=midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(10);
    if (slider11 && param->updateUiRequested()){
      slider11->setValue (param->uiGet(),dontSendNotification);
    }

    param=midiNoteGain->getParamGroup(MidiNoteGain::noteVelocitiesIndex)->getIntParam(11);
    if (slider12 && param->updateUiRequested()){
      slider12->setValue (param->uiGet(),dontSendNotification);
    }

}


//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MidiNoteGainComponent" componentName=""
                 parentClasses="public Component" constructorParams="MidiNoteGain *midiNoteGain"
                 variableInitialisers="midiNoteGain(midiNoteGain)" snapPixels="8"
                 snapActive="1" snapShown="1" overlayOpacity="0.330000013" fixedSize="1"
                 initialWidth="800" initialHeight="120">
  <BACKGROUND backgroundColour="c4c4c4"/>
  <LABEL name="new label" id="2f649db6d367e3da" memberName="label12" virtualName=""
         explicitFocusOrder="0" pos="730 92 72 23" edTextCol="ff000000"
         edBkgCol="0" labelText="B" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="4b0d09074b5a8fae" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="11 91 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="C" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="new slider" id="55cf3e64187acaa2" memberName="slider1"
          virtualName="" explicitFocusOrder="0" pos="8 28 63 56" min="0"
          max="10" int="1" style="Rotary" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="3e5302becd1c6e38" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="70 92 72 23" edTextCol="ff000000"
         edBkgCol="0" labelText="Db" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="new slider" id="5ba66a53049acbe5" memberName="slider2"
          virtualName="" explicitFocusOrder="0" pos="73 28 63 56" min="0"
          max="10" int="1" style="Rotary" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="75e3bdd05d7c10e2" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="142 91 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="D" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="new slider" id="a9799c98f9027856" memberName="slider3"
          virtualName="" explicitFocusOrder="0" pos="139 28 63 56" min="0"
          max="10" int="1" style="Rotary" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="2f5637ca96e668b4" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="202 92 72 23" edTextCol="ff000000"
         edBkgCol="0" labelText="Eb" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="new slider" id="9b20397b595d2d39" memberName="slider4"
          virtualName="" explicitFocusOrder="0" pos="205 28 63 56" min="0"
          max="10" int="1" style="Rotary" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="eb816a83611c9d75" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="274 91 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="E" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="new slider" id="f2627e7b85469e46" memberName="slider5"
          virtualName="" explicitFocusOrder="0" pos="271 28 63 56" min="0"
          max="10" int="1" style="Rotary" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="89f54732c240c4a9" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="333 92 72 23" edTextCol="ff000000"
         edBkgCol="0" labelText="F" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="new slider" id="97cfd82c84ddf8e3" memberName="slider6"
          virtualName="" explicitFocusOrder="0" pos="336 28 63 56" min="0"
          max="10" int="1" style="Rotary" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="b9fd5417277d7478" memberName="label8" virtualName=""
         explicitFocusOrder="0" pos="405 91 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Gb" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="new slider" id="480d27ccc0a3a475" memberName="slider7"
          virtualName="" explicitFocusOrder="0" pos="402 28 63 56" min="0"
          max="10" int="1" style="Rotary" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="87c630b8731e97d" memberName="label9" virtualName=""
         explicitFocusOrder="0" pos="466 92 72 23" edTextCol="ff000000"
         edBkgCol="0" labelText="G" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="new slider" id="9d9e7c1e7750ffaa" memberName="slider8"
          virtualName="" explicitFocusOrder="0" pos="469 28 63 56" min="0"
          max="10" int="1" style="Rotary" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="389710d2b362fe6c" memberName="label10" virtualName=""
         explicitFocusOrder="0" pos="604 91 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="A" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="new slider" id="78bf185a1fcf1cdb" memberName="slider10"
          virtualName="" explicitFocusOrder="0" pos="601 28 63 56" min="0"
          max="10" int="1" style="Rotary" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="953a0ae67afc3877" memberName="label11" virtualName=""
         explicitFocusOrder="0" pos="664 92 72 23" edTextCol="ff000000"
         edBkgCol="0" labelText="Bb" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="new slider" id="2a0a22c96af39d50" memberName="slider11"
          virtualName="" explicitFocusOrder="0" pos="667 28 63 56" min="0"
          max="10" int="1" style="Rotary" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="7a85cfbf246c30a3" memberName="slider12"
          virtualName="" explicitFocusOrder="0" pos="733 28 63 56" min="0"
          max="10" int="1" style="Rotary" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <TOGGLEBUTTON name="new toggle button" id="6cce19736891af98" memberName="enableToggle"
                virtualName="" explicitFocusOrder="0" pos="7 3 58 22" buttonText="on/off"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="new label" id="9769bbff7c3670cb" memberName="label13" virtualName=""
         explicitFocusOrder="0" pos="538 91 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Ab&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="new slider" id="a78c721d040ddc23" memberName="slider9"
          virtualName="" explicitFocusOrder="0" pos="535 28 63 56" min="0"
          max="10" int="1" style="Rotary" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
