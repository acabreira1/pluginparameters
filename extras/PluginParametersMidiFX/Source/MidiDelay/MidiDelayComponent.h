/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  5 Apr 2013 4:05:34pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_MIDIDELAYCOMPONENT_MIDIDELAYCOMPONENT_3988CE56__
#define __JUCER_HEADER_MIDIDELAYCOMPONENT_MIDIDELAYCOMPONENT_3988CE56__

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
#include "MidiDelay.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MidiDelayComponent  : public Component,
                            public SliderListener,
                            public ButtonListener
{
public:
    //==============================================================================
    MidiDelayComponent (MidiDelay *midiDelay);
    ~MidiDelayComponent();

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
    MidiDelay *midiDelay;
    //[/UserVariables]

    //==============================================================================
    Label* label2;
    Slider* delaySlider;
    Label* label3;
    Slider* feedbackSlider;
    ToggleButton* enableToggle;


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    MidiDelayComponent (const MidiDelayComponent&);
    const MidiDelayComponent& operator= (const MidiDelayComponent&);
};


#endif   // __JUCER_HEADER_MIDIDELAYCOMPONENT_MIDIDELAYCOMPONENT_3988CE56__
