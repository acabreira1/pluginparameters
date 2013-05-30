/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  5 Apr 2013 4:04:56pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_MIDINOTEGAINCOMPONENT_MIDINOTEGAINCOMPONENT_E053D963__
#define __JUCER_HEADER_MIDINOTEGAINCOMPONENT_MIDINOTEGAINCOMPONENT_E053D963__

//[Headers]     -- You can add your own extra header files here --
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

#include "../JuceLibraryCode/JuceHeader.h"
#include "MyPluginProcessor.h"
#include "MidiNoteGain.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MidiNoteGainComponent  : public Component,
                               public SliderListener,
                               public ButtonListener
{
public:
    //==============================================================================
    MidiNoteGainComponent (MidiNoteGain *midiNoteGain);
    ~MidiNoteGainComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    //used to update the GUI from changes in the processor
    void timerCallback();
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);



    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    MidiNoteGain *midiNoteGain;
    //[/UserVariables]

    //==============================================================================
    Label* label12;
    Label* label2;
    Slider* slider1;
    Label* label3;
    Slider* slider2;
    Label* label4;
    Slider* slider3;
    Label* label5;
    Slider* slider4;
    Label* label6;
    Slider* slider5;
    Label* label7;
    Slider* slider6;
    Label* label8;
    Slider* slider7;
    Label* label9;
    Slider* slider8;
    Label* label10;
    Slider* slider10;
    Label* label11;
    Slider* slider11;
    Slider* slider12;
    ToggleButton* enableToggle;
    Label* label13;
    Slider* slider9;


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    MidiNoteGainComponent (const MidiNoteGainComponent&);
    const MidiNoteGainComponent& operator= (const MidiNoteGainComponent&);
};


#endif   // __JUCER_HEADER_MIDINOTEGAINCOMPONENT_MIDINOTEGAINCOMPONENT_E053D963__
