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

  This file is part of the PluginParametersGroupsDemo plugin.
  Copyright 2012-2013 by MarC

  ------------------------------------------------------------------------------

  PluginParametersGroupsDemo is provided under the terms of The MIT License (MIT):

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.

  ------------------------------------------------------------------------------

  In order to build this Software you will need the JUCE library (juce.com)
  along with the PluginParameters module (github.com/4drX/pluginparameters),
  which have separate licenses.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

class PresetsListComponent: public ComboBox, public ComboBox::Listener, public ChangeBroadcaster {
  /*Presets *presets*/;
  Array<File> fileList;
public:

  PresetsListComponent(/*Presets *presets*/)
    :ComboBox("presetsListComboBox")/*,presets(presets)*/{
    setEditableText (false);
    setJustificationType (Justification::centredLeft);
    setTextWhenNothingSelected (String::empty);
    setTextWhenNoChoicesAvailable ("(no presets selected)");
    ComboBox::addListener(this);
    refresh();
  }

  void comboBoxChanged (ComboBox* /*comboBoxThatHasChanged*/){
    sendChangeMessage();
  }

  /** Returns the file that the user has currently selected. */
  File getSelectedFile () const{
    if (getSelectedId()<=0)
      return File::nonexistent;

    return fileList[getSelectedId()-1];
  }

  /** Select a file */
  void setSelectedFile (const File& file){
    for (int i=0;i<fileList.size();i++){
      if (file==fileList[i])
        setSelectedId(i+1,dontSendNotification);
    }
  }

  /** Show the contents of a folder */
  void openFolder (const File&){
    //Disabled. It would be confusing to open folders in a ComboBox.
    setSelectedId(0,dontSendNotification);
  }

  /** Search again for preset files in the rootFolder */
  void refresh(){
  }

  /** Flags the item to be modified or not. Modified items will be followed by an "*" */
  void setModified(const File &/*file*/, const bool /*isModified*/){
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
    PresetsComponent (/*Presets *presets*/);
    ~PresetsComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void timerCallback();
    void selectLastLoadRequested();
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
    static const char* openinexplorer_png;
    static const int openinexplorer_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    /*Presets *presets*/
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
    ScopedPointer<ImageButton> openFolderButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_12F186E49FAB249A__
