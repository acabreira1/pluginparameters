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

#ifndef __JUCER_HEADER_MIDISUSTAINCOMPONENT_MIDISUSTAINCOMPONENT_793FF71B__
#define __JUCER_HEADER_MIDISUSTAINCOMPONENT_MIDISUSTAINCOMPONENT_793FF71B__

//[Headers]     -- You can add your own extra header files here --
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

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
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
    MidiSustainComponent (ParamGroup *paramGroup);
    ~MidiSustainComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    //used to update the GUI from changes in the processor
    void timerCallback();
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);



    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    ParamGroup *const paramGroup;
    //[/UserVariables]

    //==============================================================================
    ToggleButton* enableToggle;


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    MidiSustainComponent (const MidiSustainComponent&);
    const MidiSustainComponent& operator= (const MidiSustainComponent&);
};


#endif   // __JUCER_HEADER_MIDISUSTAINCOMPONENT_MIDISUSTAINCOMPONENT_793FF71B__
