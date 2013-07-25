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

#ifndef __JUCE_HEADER_12F186E49FAB249A__
#define __JUCE_HEADER_12F186E49FAB249A__

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

class PresetsListComponent: public ComboBox{
public:
  PresetsListComponent(String name):ComboBox(name){
  }
};
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PresetsComponent  : public Component,
                          public ChangeListener,
                          public ButtonListener
{
public:
    //==============================================================================
    PresetsComponent ();
    ~PresetsComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void timerCallback();
    void selectLastSelectedPreset();
    void changeListenerCallback(ChangeBroadcaster *source);
    void update();
    void next();
    void previous();
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);

    // Binary resources:
    static const char* delete_png;
    static const int delete_pngSize;
    static const char* new_png;
    static const int new_pngSize;
    static const char* rename_png;
    static const int rename_pngSize;
    static const char* reload_png;
    static const int reload_pngSize;
    static const char* save_png;
    static const int save_pngSize;
    static const char* saveasnew_png;
    static const int saveasnew_pngSize;
    static const char* left_png;
    static const int left_pngSize;
    static const char* rescan_png;
    static const int rescan_pngSize;
    static const char* right_png;
    static const int right_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //Presets* presets;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<ImageButton> delPresetButton;
    ScopedPointer<ImageButton> newPresetButton;
    ScopedPointer<ImageButton> renamePresetButton;
    ScopedPointer<ImageButton> reloadPresetButton;
    ScopedPointer<ImageButton> savePresetButton;
    ScopedPointer<ImageButton> saveAsNewPresetButton;
    ScopedPointer<ImageButton> leftButton;
    ScopedPointer<ImageButton> rescanPresetsButton;
    ScopedPointer<ImageButton> rightButton;
    ScopedPointer<PresetsListComponent> presetsListComponent;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_12F186E49FAB249A__
