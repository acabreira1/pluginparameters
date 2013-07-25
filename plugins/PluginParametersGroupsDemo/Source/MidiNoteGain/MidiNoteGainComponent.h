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

#ifndef __JUCE_HEADER_366350E7F2E5F95A__
#define __JUCE_HEADER_366350E7F2E5F95A__

//[Headers]     -- You can add your own extra header files here --
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



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    MidiNoteGain *midiNoteGain;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> label12;
    ScopedPointer<Label> label2;
    ScopedPointer<Slider> slider1;
    ScopedPointer<Label> label3;
    ScopedPointer<Slider> slider2;
    ScopedPointer<Label> label4;
    ScopedPointer<Slider> slider3;
    ScopedPointer<Label> label5;
    ScopedPointer<Slider> slider4;
    ScopedPointer<Label> label6;
    ScopedPointer<Slider> slider5;
    ScopedPointer<Label> label7;
    ScopedPointer<Slider> slider6;
    ScopedPointer<Label> label8;
    ScopedPointer<Slider> slider7;
    ScopedPointer<Label> label9;
    ScopedPointer<Slider> slider8;
    ScopedPointer<Label> label10;
    ScopedPointer<Slider> slider10;
    ScopedPointer<Label> label11;
    ScopedPointer<Slider> slider11;
    ScopedPointer<Slider> slider12;
    ScopedPointer<ToggleButton> enableToggle;
    ScopedPointer<Label> label13;
    ScopedPointer<Slider> slider9;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiNoteGainComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_366350E7F2E5F95A__
