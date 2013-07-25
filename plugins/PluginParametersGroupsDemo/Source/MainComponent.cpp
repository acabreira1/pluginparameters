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
//[/Headers]

#include "MainComponent.h"
#include "MidiSustain/MidiSustainComponent.h"
#include "MidiDelay/MidiDelayComponent.h"
#include "MidiNoteGain/MidiNoteGainComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MainComponent::MainComponent (MyPluginProcessor* processor)
    : AudioProcessorEditor(processor), processor(processor)
{
    addAndMakeVisible (tabbedComponent = new TabbedComponent (TabbedButtonBar::TabsAtTop));
    tabbedComponent->setTabBarDepth (30);
    tabbedComponent->addTab ("Sustain", Colours::white, new MidiSustainComponent (&processor->midiSustain), true);
    tabbedComponent->addTab ("Delay", Colours::white, new MidiDelayComponent (&processor->midiDelay), true);
    tabbedComponent->addTab ("NoteGain", Colours::white, new MidiNoteGainComponent (&processor->midiNoteGain), true);
    tabbedComponent->setCurrentTabIndex (1);

    addAndMakeVisible (bypassToggle = new ToggleButton (String::empty));
    bypassToggle->setButtonText ("bypass");
    bypassToggle->addListener (this);

    addAndMakeVisible (presetsComponent = new PresetsComponent());

    //[UserPreSize]
    tabbedComponent->getTabbedButtonBar().addChangeListener(this);

    processor->updateUi(true,true);
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

    tabbedComponent = nullptr;
    bypassToggle = nullptr;
    presetsComponent = nullptr;


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
    bypassToggle->setBounds (735, 5, 62, 18);
    presetsComponent->setBounds (383, 5, 341, 20);
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
    bypassToggle->setToggleState (bypassParam->uiGet(), dontSendNotification);
  }

  IntParam * const tabParam=processor->getIntParam(MyPluginProcessor::selectedTabIndex);
  if (tabbedComponent){
    if (tabParam->updateUiRequested())
      tabbedComponent->setCurrentTabIndex(tabParam->uiGet(),dontSendNotification);

    Component *component=tabbedComponent->getCurrentContentComponent();

    if (component==tabbedComponent->getTabContentComponent(0))
      dynamic_cast<MidiSustainComponent*>(component)->timerCallback();
    else if (component==tabbedComponent->getTabContentComponent(1))
      dynamic_cast<MidiDelayComponent*>(component)->timerCallback();
    else if (component==tabbedComponent->getTabContentComponent(2))
      dynamic_cast<MidiNoteGainComponent*>(component)->timerCallback();
  }

  if (presetsComponent){
    presetsComponent->timerCallback();
  }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MainComponent" componentName=""
                 parentClasses="public AudioProcessorEditor, public Timer, public ChangeListener"
                 constructorParams="MyPluginProcessor* processor" variableInitialisers="AudioProcessorEditor(processor), processor(processor)"
                 snapPixels="8" snapActive="0" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="800" initialHeight="150">
  <BACKGROUND backgroundColour="ffffffff"/>
  <TABBEDCOMPONENT name="" id="852330df3c5444e9" memberName="tabbedComponent" virtualName=""
                   explicitFocusOrder="0" pos="0 0 800 150" orientation="top" tabBarDepth="30"
                   initialTab="1">
    <TAB name="Sustain" colour="ffffffff" useJucerComp="1" contentClassName=""
         constructorParams="&amp;processor-&gt;midiSustain" jucerComponentFile="MidiSustain/MidiSustainComponent.cpp"/>
    <TAB name="Delay" colour="ffffffff" useJucerComp="1" contentClassName=""
         constructorParams="&amp;processor-&gt;midiDelay" jucerComponentFile="MidiDelay/MidiDelayComponent.cpp"/>
    <TAB name="NoteGain" colour="ffffffff" useJucerComp="1" contentClassName=""
         constructorParams="&amp;processor-&gt;midiNoteGain" jucerComponentFile="MidiNoteGain/MidiNoteGainComponent.cpp"/>
  </TABBEDCOMPONENT>
  <TOGGLEBUTTON name="" id="d90e123570461871" memberName="bypassToggle" virtualName=""
                explicitFocusOrder="0" pos="735 5 62 18" buttonText="bypass"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <JUCERCOMP name="" id="c4dd67befc4f99e6" memberName="presetsComponent" virtualName=""
             explicitFocusOrder="0" pos="383 5 341 20" sourceFile="PresetsComponent.cpp"
             constructorParams=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
