/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  5 Apr 2013 4:09:38pm

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
//[/Headers]

#include "MainComponent.h"
#include "MidiSustain/MidiSustainComponent.h"
#include "MidiDelay/MidiDelayComponent.h"
#include "MidiNoteGain/MidiNoteGainComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MainComponent::MainComponent (MyPluginProcessor* processor)
    : AudioProcessorEditor(processor), processor(processor),
      tabbedComponent (0),
      bypassToggle (0),
      label (0)
{
    addAndMakeVisible (tabbedComponent = new TabbedComponent (TabbedButtonBar::TabsAtTop));
    tabbedComponent->setTabBarDepth (30);
    tabbedComponent->addTab (L"Sustain", Colours::lightgrey, new MidiSustainComponent (&processor->midiSustain), true);
    tabbedComponent->addTab (L"Delay", Colours::lightgrey, new MidiDelayComponent (&processor->midiDelay), true);
    tabbedComponent->addTab (L"NoteGain", Colours::lightgrey, new MidiNoteGainComponent (&processor->midiNoteGain), true);
    tabbedComponent->setCurrentTabIndex (1);

    addAndMakeVisible (bypassToggle = new ToggleButton (String::empty));
    bypassToggle->setButtonText (L"bypass");
    bypassToggle->addListener (this);

    addAndMakeVisible (label = new Label (L"new label",
                                          L"MIDI FX"));
    label->setFont (Font (15.0000f, Font::plain));
    label->setJustificationType (Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (Label::backgroundColourId, Colour (0x1c000000));
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x0));


    //[UserPreSize]
    processor->requestUpdateUiRecursively(true);
    timerCallback();
    //[/UserPreSize]

    setSize (800, 150);


    //[Constructor] You can add your own custom stuff here..
    startTimer (50);
    //[/Constructor]
}

MainComponent::~MainComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (tabbedComponent);
    deleteAndZero (bypassToggle);
    deleteAndZero (label);


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MainComponent::resized()
{
    tabbedComponent->setBounds (0, 0, 800, 150);
    bypassToggle->setBounds (660, 4, 62, 18);
    label->setBounds (733, 0, 67, 26);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MainComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == bypassToggle)
    {
        //[UserButtonCode_bypassToggle] -- add your button handler code here..
		    BoolParam *param=processor->getBoolParam(MyPluginProcessor::bypassIndex);
        param->updateProcessorAndHostFromUi(bypassToggle->getToggleState());
        //[/UserButtonCode_bypassToggle]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void MainComponent::timerCallback(){
	BoolParam * const bypassParam=processor->getBoolParam(MyPluginProcessor::bypassIndex);
  if (bypassToggle && bypassParam->updateUiRequested()){
    bypassToggle->setToggleState (bypassParam->uiGet(), false);
  }

  if (tabbedComponent){
    if (tabbedComponent->getCurrentTabIndex()==0)
      dynamic_cast<MidiSustainComponent*>(tabbedComponent->getCurrentContentComponent())->timerCallback();
    else if (tabbedComponent->getCurrentTabIndex()==1)
      dynamic_cast<MidiDelayComponent*>(tabbedComponent->getCurrentContentComponent())->timerCallback();
    else if (tabbedComponent->getCurrentTabIndex()==2)
      dynamic_cast<MidiNoteGainComponent*>(tabbedComponent->getCurrentContentComponent())->timerCallback();
  }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MainComponent" componentName=""
                 parentClasses="public AudioProcessorEditor, public Timer" constructorParams="MyPluginProcessor* processor"
                 variableInitialisers="AudioProcessorEditor(processor), processor(processor)"
                 snapPixels="8" snapActive="0" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="800" initialHeight="150">
  <BACKGROUND backgroundColour="ffffffff"/>
  <TABBEDCOMPONENT name="" id="852330df3c5444e9" memberName="tabbedComponent" virtualName=""
                   explicitFocusOrder="0" pos="0 0 800 150" orientation="top" tabBarDepth="30"
                   initialTab="1">
    <TAB name="Sustain" colour="ffd3d3d3" useJucerComp="1" contentClassName=""
         constructorParams="&amp;processor-&gt;midiSustain" jucerComponentFile="MidiSustain/MidiSustainComponent.cpp"/>
    <TAB name="Delay" colour="ffd3d3d3" useJucerComp="1" contentClassName=""
         constructorParams="&amp;processor-&gt;midiDelay" jucerComponentFile="MidiDelay/MidiDelayComponent.cpp"/>
    <TAB name="NoteGain" colour="ffd3d3d3" useJucerComp="1" contentClassName=""
         constructorParams="&amp;processor-&gt;midiNoteGain" jucerComponentFile="MidiNoteGain/MidiNoteGainComponent.cpp"/>
  </TABBEDCOMPONENT>
  <TOGGLEBUTTON name="" id="d90e123570461871" memberName="bypassToggle" virtualName=""
                explicitFocusOrder="0" pos="660 4 62 18" buttonText="bypass"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="new label" id="23815bc91bf9bce6" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="733 0 67 26" bkgCol="1c000000" edTextCol="ff000000"
         edBkgCol="0" labelText="MIDI FX" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
