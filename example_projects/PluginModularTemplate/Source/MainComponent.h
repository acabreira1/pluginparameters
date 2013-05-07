/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  6 Apr 2013 2:03:12pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_MAINCOMPONENT_MAINCOMPONENT_3648244B__
#define __JUCER_HEADER_MAINCOMPONENT_MAINCOMPONENT_3648244B__

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
#include "MyPluginProcessor.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MainComponent  : public AudioProcessorEditor,
                       public Timer,
                       public ChangeListener,
                       public ButtonListener
{
public:
    //==============================================================================
    MainComponent (MyPluginProcessor* processor);
    ~MainComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void timerCallback();
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);



    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    // handy wrapper method to avoid having to cast the filter to a PluginProcessor
	  // every time we need it..
	  MyPluginProcessor *const processor;

    void 	changeListenerCallback (ChangeBroadcaster *source){
      if (source==&tabbedComponent->getTabbedButtonBar()){
        processor->getIntParam(MyPluginProcessor::selectedTabIndex)->updateProcessorAndHostFromUi(tabbedComponent->getCurrentTabIndex());
      }
    }
    //[/UserVariables]

    //==============================================================================
    TabbedComponent* tabbedComponent;
    ToggleButton* bypassToggle;
    Label* label;


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    MainComponent (const MainComponent&);
    const MainComponent& operator= (const MainComponent&);
};


#endif   // __JUCER_HEADER_MAINCOMPONENT_MAINCOMPONENT_3648244B__