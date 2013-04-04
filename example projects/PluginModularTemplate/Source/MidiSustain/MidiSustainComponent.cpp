/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  4 Apr 2013 12:13:10am

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

#include "MidiSustainComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MidiSustainComponent::MidiSustainComponent (ParamGroup *paramGroup)
    : paramGroup(paramGroup),
      enableToggle (0)
{
    addAndMakeVisible (enableToggle = new ToggleButton (L"new toggle button"));
    enableToggle->setButtonText (L"on/off");
    enableToggle->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (800, 120);


    //[Constructor] You can add your own custom stuff here..
    timerCallback();
    //[/Constructor]
}

MidiSustainComponent::~MidiSustainComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (enableToggle);


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MidiSustainComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffc4c4c4));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MidiSustainComponent::resized()
{
    enableToggle->setBounds (7, 0, 58, 22);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MidiSustainComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == enableToggle)
    {
        //[UserButtonCode_enableToggle] -- add your button handler code here..
        paramGroup->getBoolParam(MidiSustainParamGroup::enableIndex)->updateProcessorAndHostFromUi(enableToggle->getToggleState());
        //[/UserButtonCode_enableToggle]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MidiSustainComponent::timerCallback()
{
    BoolParam *enableParam=paramGroup->getBoolParam(MidiSustainParamGroup::enableIndex);
    if (enableToggle && enableParam->updateUiRequested()){
      enableToggle->setToggleState(enableParam->uiGet(),false);
    }

}


//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MidiSustainComponent" componentName=""
                 parentClasses="public Component" constructorParams="ParamGroup *paramGroup"
                 variableInitialisers="paramGroup(paramGroup)" snapPixels="8"
                 snapActive="1" snapShown="1" overlayOpacity="0.330000013" fixedSize="1"
                 initialWidth="800" initialHeight="120">
  <BACKGROUND backgroundColour="ffc4c4c4"/>
  <TOGGLEBUTTON name="new toggle button" id="6cce19736891af98" memberName="enableToggle"
                virtualName="" explicitFocusOrder="0" pos="7 0 58 22" buttonText="on/off"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
