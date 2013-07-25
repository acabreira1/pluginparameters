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

#ifndef __JUCE_HEADER_D763E2A108F478AA__
#define __JUCE_HEADER_D763E2A108F478AA__

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
#include "MidiSustain.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MidiSustainComponent  : public Component,
                              public ButtonListener
{
public:
    //==============================================================================
    MidiSustainComponent (MidiSustain *midiSustain);
    ~MidiSustainComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    //used to update the GUI from changes in the processor
    void timerCallback();
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    MidiSustain *midiSustain;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<ToggleButton> enableToggle;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiSustainComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_D763E2A108F478AA__
