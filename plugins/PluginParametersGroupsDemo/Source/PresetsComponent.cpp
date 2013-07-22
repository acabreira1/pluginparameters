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

  Author: MarC

  Creation date:  10 Mar 2012 3:36:55pm

  Description: Implements the user interface for the preset system.

  Copyright 2012 by MarC

  ==============================================================================
*/
//[/Headers]

#include "PresetsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-11 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

//[/MiscUserDefs]

//==============================================================================
PresetsComponent::PresetsComponent ()
{
    addAndMakeVisible (delPresetButton = new ImageButton ("delPresetButton"));
    delPresetButton->setTooltip ("delete preset");
    delPresetButton->setButtonText ("delete preset");
    delPresetButton->addListener (this);

    delPresetButton->setImages (false, true, true,
                                ImageCache::getFromMemory (delete_png, delete_pngSize), 1.000f, Colour (0x00000000),
                                ImageCache::getFromMemory (delete_png, delete_pngSize), 0.600f, Colour (0x00000000),
                                ImageCache::getFromMemory (delete_png, delete_pngSize), 0.400f, Colour (0x00000000));
    addAndMakeVisible (newPresetButton = new ImageButton ("newPresetButton"));
    newPresetButton->setTooltip ("new preset");
    newPresetButton->setButtonText ("new preset");
    newPresetButton->addListener (this);

    newPresetButton->setImages (false, true, true,
                                ImageCache::getFromMemory (new_png, new_pngSize), 1.000f, Colour (0x00000000),
                                ImageCache::getFromMemory (new_png, new_pngSize), 0.600f, Colour (0x00000000),
                                ImageCache::getFromMemory (new_png, new_pngSize), 0.400f, Colour (0x00000000));
    addAndMakeVisible (renamePresetButton = new ImageButton ("renamePresetButton"));
    renamePresetButton->setTooltip ("rename preset");
    renamePresetButton->setButtonText ("rename preset");
    renamePresetButton->addListener (this);

    renamePresetButton->setImages (false, true, true,
                                   ImageCache::getFromMemory (rename_png, rename_pngSize), 1.000f, Colour (0x00000000),
                                   ImageCache::getFromMemory (rename_png, rename_pngSize), 0.600f, Colour (0x00000000),
                                   ImageCache::getFromMemory (rename_png, rename_pngSize), 0.400f, Colour (0x00000000));
    addAndMakeVisible (reloadPresetButton = new ImageButton ("reloadPresetButton"));
    reloadPresetButton->setTooltip ("reload preset");
    reloadPresetButton->setButtonText ("reload preset");
    reloadPresetButton->addListener (this);

    reloadPresetButton->setImages (false, true, true,
                                   ImageCache::getFromMemory (reload_png, reload_pngSize), 1.000f, Colour (0x00000000),
                                   ImageCache::getFromMemory (reload_png, reload_pngSize), 0.600f, Colour (0x00000000),
                                   ImageCache::getFromMemory (reload_png, reload_pngSize), 0.400f, Colour (0x00000000));
    addAndMakeVisible (savePresetButton = new ImageButton ("savePresetButton"));
    savePresetButton->setTooltip ("save preset");
    savePresetButton->setButtonText ("save preset");
    savePresetButton->addListener (this);

    savePresetButton->setImages (false, true, true,
                                 ImageCache::getFromMemory (save_png, save_pngSize), 1.000f, Colour (0x00000000),
                                 ImageCache::getFromMemory (save_png, save_pngSize), 0.600f, Colour (0x00000000),
                                 ImageCache::getFromMemory (save_png, save_pngSize), 0.400f, Colour (0x00000000));
    addAndMakeVisible (saveAsNewPresetButton = new ImageButton ("saveAsNewPresetButton"));
    saveAsNewPresetButton->setTooltip ("save as new file");
    saveAsNewPresetButton->setButtonText ("save as new file");
    saveAsNewPresetButton->addListener (this);

    saveAsNewPresetButton->setImages (false, true, true,
                                      ImageCache::getFromMemory (saveasnew_png, saveasnew_pngSize), 1.000f, Colour (0x00000000),
                                      ImageCache::getFromMemory (saveasnew_png, saveasnew_pngSize), 0.600f, Colour (0x00000000),
                                      ImageCache::getFromMemory (saveasnew_png, saveasnew_pngSize), 0.400f, Colour (0x00000000));
    addAndMakeVisible (leftButton = new ImageButton ("leftButton"));
    leftButton->setTooltip ("previous preset");
    leftButton->setButtonText ("previous preset");
    leftButton->addListener (this);

    leftButton->setImages (false, true, true,
                           ImageCache::getFromMemory (left_png, left_pngSize), 1.000f, Colour (0x00000000),
                           ImageCache::getFromMemory (left_png, left_pngSize), 0.600f, Colour (0x00000000),
                           ImageCache::getFromMemory (left_png, left_pngSize), 0.400f, Colour (0x00000000));
    addAndMakeVisible (rescanPresetsButton = new ImageButton ("rescanPresetsButton"));
    rescanPresetsButton->setTooltip ("refresh preset list");
    rescanPresetsButton->setButtonText ("refresh preset list");
    rescanPresetsButton->addListener (this);

    rescanPresetsButton->setImages (false, true, true,
                                    ImageCache::getFromMemory (rescan_png, rescan_pngSize), 1.000f, Colour (0x00000000),
                                    ImageCache::getFromMemory (rescan_png, rescan_pngSize), 0.600f, Colour (0x00000000),
                                    ImageCache::getFromMemory (rescan_png, rescan_pngSize), 0.400f, Colour (0x00000000));
    addAndMakeVisible (rightButton = new ImageButton ("rightButton"));
    rightButton->setTooltip ("next preset");
    rightButton->setButtonText ("next preset");
    rightButton->addListener (this);

    rightButton->setImages (false, true, true,
                            ImageCache::getFromMemory (right_png, right_pngSize), 1.000f, Colour (0x00000000),
                            ImageCache::getFromMemory (right_png, right_pngSize), 0.600f, Colour (0x00000000),
                            ImageCache::getFromMemory (right_png, right_pngSize), 0.400f, Colour (0x00000000));
    addAndMakeVisible (presetsListComponent = new PresetsListComponent ("presets"));
    presetsListComponent->setName ("presetsListComponent");


    //[UserPreSize]
    //presetsListComponent->addChangeListener(this);
    timerCallback();
    //[/UserPreSize]

    setSize (341, 20);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

PresetsComponent::~PresetsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    delPresetButton = nullptr;
    newPresetButton = nullptr;
    renamePresetButton = nullptr;
    reloadPresetButton = nullptr;
    savePresetButton = nullptr;
    saveAsNewPresetButton = nullptr;
    leftButton = nullptr;
    rescanPresetsButton = nullptr;
    rightButton = nullptr;
    presetsListComponent = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PresetsComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PresetsComponent::resized()
{
    delPresetButton->setBounds (25, 2, 16, 16);
    newPresetButton->setBounds (2, 2, 16, 16);
    renamePresetButton->setBounds (71, 2, 16, 16);
    reloadPresetButton->setBounds (48, 2, 16, 16);
    savePresetButton->setBounds (94, 2, 16, 16);
    saveAsNewPresetButton->setBounds (117, 2, 16, 16);
    leftButton->setBounds (277, 2, 16, 16);
    rescanPresetsButton->setBounds (323, 2, 16, 16);
    rightButton->setBounds (300, 2, 16, 16);
    presetsListComponent->setBounds (138, 1, 135, 18);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void PresetsComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == delPresetButton)
    {
        //[UserButtonCode_delPresetButton] -- add your button handler code here..
        //[/UserButtonCode_delPresetButton]
    }
    else if (buttonThatWasClicked == newPresetButton)
    {
        //[UserButtonCode_newPresetButton] -- add your button handler code here..
        //presets->reset();
        //[/UserButtonCode_newPresetButton]
    }
    else if (buttonThatWasClicked == renamePresetButton)
    {
        //[UserButtonCode_renamePresetButton] -- add your button handler code here..
        /*if (presets->getLastSelectedPreset()!=File::nonexistent){
          String extensionFilter="*."+presets->getPresetFileExtension();
          if (presets->presetsAreFolders())
            extensionFilter="*";
		      FileChooser myChooser ("Please enter a new name...",
          File(presets->getDefaultPresetPath()),extensionFilter,true);

          if (myChooser.browseForFileToSave(true)){
            if (!myChooser.getResult().hasWriteAccess()){
              AlertWindow::showMessageBox (AlertWindow::WarningIcon,
                                          "Error reading file" ,
                                          "Sorry mate, this preset file is already open.");
            } else
              presets->rename(myChooser.getResult(),true);
          }
        }*/
        //[/UserButtonCode_renamePresetButton]
    }
    else if (buttonThatWasClicked == reloadPresetButton)
    {
        //[UserButtonCode_reloadPresetButton] -- add your button handler code here..
        /*if (presets->getLastSelectedPreset()!=File::nonexistent){
          presets->reread();
        }*/
        //[/UserButtonCode_reloadPresetButton]
    }
    else if (buttonThatWasClicked == savePresetButton)
    {
        //[UserButtonCode_savePresetButton] -- add your button handler code here..
        //presets->save();
        //[/UserButtonCode_savePresetButton]
    }
    else if (buttonThatWasClicked == saveAsNewPresetButton)
    {
        //[UserButtonCode_saveAsNewPresetButton] -- add your button handler code here..
        //presets->saveAsNew();
        //[/UserButtonCode_saveAsNewPresetButton]
    }
    else if (buttonThatWasClicked == leftButton)
    {
        //[UserButtonCode_leftButton] -- add your button handler code here..
        previous();
        //[/UserButtonCode_leftButton]
    }
    else if (buttonThatWasClicked == rescanPresetsButton)
    {
        //[UserButtonCode_rescanPresetsButton] -- add your button handler code here..
        update();
        //[/UserButtonCode_rescanPresetsButton]
    }
    else if (buttonThatWasClicked == rightButton)
    {
        //[UserButtonCode_rightButton] -- add your button handler code here..
        next();
        //[/UserButtonCode_rightButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void PresetsComponent::changeListenerCallback(ChangeBroadcaster * /*source*/){

}

void PresetsComponent::previous(){

}

void PresetsComponent::next(){

}

void PresetsComponent::update(){

}

void PresetsComponent::selectLastSelectedPreset(){

}

void PresetsComponent::timerCallback(){

}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PresetsComponent" componentName=""
                 parentClasses="public Component, public ChangeListener" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="0" snapShown="1"
                 overlayOpacity="0.330000013" fixedSize="1" initialWidth="341"
                 initialHeight="20">
  <BACKGROUND backgroundColour="ffffff"/>
  <IMAGEBUTTON name="delPresetButton" id="c99e5b0420e312f1" memberName="delPresetButton"
               virtualName="" explicitFocusOrder="0" pos="25 2 16 16" tooltip="delete preset"
               buttonText="delete preset" connectedEdges="0" needsCallback="1"
               radioGroupId="0" keepProportions="1" resourceNormal="delete_png"
               opacityNormal="1" colourNormal="0" resourceOver="delete_png"
               opacityOver="0.600000024" colourOver="0" resourceDown="delete_png"
               opacityDown="0.400000006" colourDown="0"/>
  <IMAGEBUTTON name="newPresetButton" id="39e610a657642ea3" memberName="newPresetButton"
               virtualName="" explicitFocusOrder="0" pos="2 2 16 16" tooltip="new preset"
               buttonText="new preset" connectedEdges="0" needsCallback="1"
               radioGroupId="0" keepProportions="1" resourceNormal="new_png"
               opacityNormal="1" colourNormal="0" resourceOver="new_png" opacityOver="0.600000024"
               colourOver="0" resourceDown="new_png" opacityDown="0.400000006"
               colourDown="0"/>
  <IMAGEBUTTON name="renamePresetButton" id="cd0ce05b70ce246a" memberName="renamePresetButton"
               virtualName="" explicitFocusOrder="0" pos="71 2 16 16" tooltip="rename preset"
               buttonText="rename preset" connectedEdges="0" needsCallback="1"
               radioGroupId="0" keepProportions="1" resourceNormal="rename_png"
               opacityNormal="1" colourNormal="0" resourceOver="rename_png"
               opacityOver="0.600000024" colourOver="0" resourceDown="rename_png"
               opacityDown="0.400000006" colourDown="0"/>
  <IMAGEBUTTON name="reloadPresetButton" id="4656a0b5a5213f6f" memberName="reloadPresetButton"
               virtualName="" explicitFocusOrder="0" pos="48 2 16 16" tooltip="reload preset"
               buttonText="reload preset" connectedEdges="0" needsCallback="1"
               radioGroupId="0" keepProportions="1" resourceNormal="reload_png"
               opacityNormal="1" colourNormal="0" resourceOver="reload_png"
               opacityOver="0.600000024" colourOver="0" resourceDown="reload_png"
               opacityDown="0.400000006" colourDown="0"/>
  <IMAGEBUTTON name="savePresetButton" id="80d155297f3a6d60" memberName="savePresetButton"
               virtualName="" explicitFocusOrder="0" pos="94 2 16 16" tooltip="save preset"
               buttonText="save preset" connectedEdges="0" needsCallback="1"
               radioGroupId="0" keepProportions="1" resourceNormal="save_png"
               opacityNormal="1" colourNormal="0" resourceOver="save_png" opacityOver="0.600000024"
               colourOver="0" resourceDown="save_png" opacityDown="0.400000006"
               colourDown="0"/>
  <IMAGEBUTTON name="saveAsNewPresetButton" id="1fb5b6c3999a50b0" memberName="saveAsNewPresetButton"
               virtualName="" explicitFocusOrder="0" pos="117 2 16 16" tooltip="save as new file"
               buttonText="save as new file" connectedEdges="0" needsCallback="1"
               radioGroupId="0" keepProportions="1" resourceNormal="saveasnew_png"
               opacityNormal="1" colourNormal="0" resourceOver="saveasnew_png"
               opacityOver="0.600000024" colourOver="0" resourceDown="saveasnew_png"
               opacityDown="0.400000006" colourDown="0"/>
  <IMAGEBUTTON name="leftButton" id="db56d300db5cbfab" memberName="leftButton"
               virtualName="" explicitFocusOrder="0" pos="277 2 16 16" tooltip="previous preset"
               buttonText="previous preset" connectedEdges="0" needsCallback="1"
               radioGroupId="0" keepProportions="1" resourceNormal="left_png"
               opacityNormal="1" colourNormal="0" resourceOver="left_png" opacityOver="0.600000024"
               colourOver="0" resourceDown="left_png" opacityDown="0.400000006"
               colourDown="0"/>
  <IMAGEBUTTON name="rescanPresetsButton" id="d43234fc8040a0c9" memberName="rescanPresetsButton"
               virtualName="" explicitFocusOrder="0" pos="323 2 16 16" tooltip="refresh preset list"
               buttonText="refresh preset list" connectedEdges="0" needsCallback="1"
               radioGroupId="0" keepProportions="1" resourceNormal="rescan_png"
               opacityNormal="1" colourNormal="0" resourceOver="rescan_png"
               opacityOver="0.600000024" colourOver="0" resourceDown="rescan_png"
               opacityDown="0.400000006" colourDown="0"/>
  <IMAGEBUTTON name="rightButton" id="acdc89354d333af1" memberName="rightButton"
               virtualName="" explicitFocusOrder="0" pos="300 2 16 16" tooltip="next preset"
               buttonText="next preset" connectedEdges="0" needsCallback="1"
               radioGroupId="0" keepProportions="1" resourceNormal="right_png"
               opacityNormal="1" colourNormal="0" resourceOver="right_png" opacityOver="0.600000024"
               colourOver="0" resourceDown="right_png" opacityDown="0.400000006"
               colourDown="0"/>
  <GENERICCOMPONENT name="presetsListComponent" id="20346992263b5250" memberName="presetsListComponent"
                    virtualName="" explicitFocusOrder="0" pos="138 1 135 18" class="PresetsListComponent"
                    params="&quot;presets&quot;"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: delete_png, 1056, "resources/delete.png"
static const unsigned char resource_PresetsComponent_delete_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,16,0,0,0,16,8,6,0,0,0,31,243,255,97,0,0,0,25,116,69,88,116,83,111,102,116,119,
97,114,101,0,65,100,111,98,101,32,73,109,97,103,101,82,101,97,100,121,113,201,101,60,0,0,3,32,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,112,97,99,107,
101,116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,61,34,87,53,77,48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,32,60,120,58,120,109,112,109,101,116,97,
32,120,109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,48,45,99,48,54,48,
32,54,49,46,49,51,52,55,55,55,44,32,50,48,49,48,47,48,50,47,49,50,45,49,55,58,51,50,58,48,48,32,32,32,32,32,32,32,32,34,62,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,
116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,62,32,60,114,100,102,58,68,101,115,99,114,105,112,116,
105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,32,120,109,108,110,115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,
32,120,109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,109,109,47,34,32,120,109,108,110,115,58,115,116,82,101,
102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,82,101,102,35,34,32,120,109,112,58,67,114,
101,97,116,111,114,84,111,111,108,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,83,53,32,87,105,110,100,111,119,115,34,32,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,
61,34,120,109,112,46,105,105,100,58,48,49,54,54,52,70,68,48,52,66,70,48,49,49,69,50,56,57,67,52,69,55,57,67,51,49,50,54,56,48,48,53,34,32,120,109,112,77,77,58,68,111,99,117,109,101,110,116,73,68,61,34,
120,109,112,46,100,105,100,58,48,49,54,54,52,70,68,49,52,66,70,48,49,49,69,50,56,57,67,52,69,55,57,67,51,49,50,54,56,48,48,53,34,62,32,60,120,109,112,77,77,58,68,101,114,105,118,101,100,70,114,111,109,
32,115,116,82,101,102,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,48,49,54,54,52,70,67,69,52,66,70,48,49,49,69,50,56,57,67,52,69,55,57,67,51,49,50,54,56,48,48,53,34,32,115,
116,82,101,102,58,100,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,48,49,54,54,52,70,67,70,52,66,70,48,49,49,69,50,56,57,67,52,69,55,57,67,51,49,50,54,56,48,48,53,34,47,62,32,60,
47,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,62,32,60,47,114,100,102,58,82,68,70,62,32,60,47,120,58,120,109,112,109,101,116,97,62,32,60,63,120,112,97,99,107,101,116,32,101,110,100,61,34,
114,34,63,62,211,149,22,239,0,0,0,150,73,68,65,84,120,218,172,147,111,7,192,32,16,198,179,189,138,62,82,140,125,230,17,49,162,239,212,219,118,177,56,183,187,43,217,241,144,174,231,199,253,201,152,159,
227,0,93,32,203,228,218,93,0,121,205,92,64,21,116,19,72,59,231,55,87,56,200,137,204,149,64,176,185,34,200,129,1,129,60,232,74,140,185,43,208,250,146,240,80,2,91,174,73,105,213,60,11,249,152,55,6,178,43,
99,214,114,198,41,13,195,202,92,9,179,102,17,18,23,198,24,49,192,51,139,212,27,230,132,69,242,218,42,211,110,187,209,42,99,72,16,230,236,70,159,105,41,30,1,6,0,171,216,111,120,142,167,22,7,0,0,0,0,73,
69,78,68,174,66,96,130,0,0};

const char* PresetsComponent::delete_png = (const char*) resource_PresetsComponent_delete_png;
const int PresetsComponent::delete_pngSize = 1056;

// JUCER_RESOURCE: new_png, 1002, "resources/new.png"
static const unsigned char resource_PresetsComponent_new_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,16,0,0,0,16,8,6,0,0,0,31,243,255,97,0,0,0,25,116,69,88,116,83,111,102,116,119,97,
114,101,0,65,100,111,98,101,32,73,109,97,103,101,82,101,97,100,121,113,201,101,60,0,0,3,32,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,112,97,99,107,101,
116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,61,34,87,53,77,48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,32,60,120,58,120,109,112,109,101,116,97,32,120,
109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,48,45,99,48,54,48,32,54,49,
46,49,51,52,55,55,55,44,32,50,48,49,48,47,48,50,47,49,50,45,49,55,58,51,50,58,48,48,32,32,32,32,32,32,32,32,34,62,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,
112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,62,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,
110,32,114,100,102,58,97,98,111,117,116,61,34,34,32,120,109,108,110,115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,32,120,
109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,109,109,47,34,32,120,109,108,110,115,58,115,116,82,101,102,61,
34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,82,101,102,35,34,32,120,109,112,58,67,114,101,97,
116,111,114,84,111,111,108,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,83,53,32,87,105,110,100,111,119,115,34,32,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,61,34,120,
109,112,46,105,105,100,58,68,52,70,50,68,48,68,55,52,66,70,48,49,49,69,50,56,52,67,57,66,53,52,54,51,51,56,50,66,65,68,52,34,32,120,109,112,77,77,58,68,111,99,117,109,101,110,116,73,68,61,34,120,109,112,
46,100,105,100,58,68,52,70,50,68,48,68,56,52,66,70,48,49,49,69,50,56,52,67,57,66,53,52,54,51,51,56,50,66,65,68,52,34,62,32,60,120,109,112,77,77,58,68,101,114,105,118,101,100,70,114,111,109,32,115,116,
82,101,102,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,68,52,70,50,68,48,68,53,52,66,70,48,49,49,69,50,56,52,67,57,66,53,52,54,51,51,56,50,66,65,68,52,34,32,115,116,82,101,
102,58,100,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,68,52,70,50,68,48,68,54,52,66,70,48,49,49,69,50,56,52,67,57,66,53,52,54,51,51,56,50,66,65,68,52,34,47,62,32,60,47,114,100,
102,58,68,101,115,99,114,105,112,116,105,111,110,62,32,60,47,114,100,102,58,82,68,70,62,32,60,47,120,58,120,109,112,109,101,116,97,62,32,60,63,120,112,97,99,107,101,116,32,101,110,100,61,34,114,34,63,
62,150,5,126,38,0,0,0,96,73,68,65,84,120,218,98,96,128,0,123,32,62,9,196,255,9,224,75,64,204,207,128,5,60,39,66,51,3,148,62,12,196,156,232,6,252,39,193,0,16,222,8,196,204,148,24,0,194,211,64,2,140,72,
18,132,0,35,22,87,51,146,98,0,86,67,71,13,24,53,0,57,113,61,33,50,57,35,227,39,200,38,121,16,153,35,97,248,57,84,15,3,64,128,1,0,152,195,81,217,39,189,164,133,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PresetsComponent::new_png = (const char*) resource_PresetsComponent_new_png;
const int PresetsComponent::new_pngSize = 1002;

// JUCER_RESOURCE: rename_png, 1143, "resources/rename.png"
static const unsigned char resource_PresetsComponent_rename_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,16,0,0,0,16,8,6,0,0,0,31,243,255,97,0,0,0,25,116,69,88,116,83,111,102,116,119,
97,114,101,0,65,100,111,98,101,32,73,109,97,103,101,82,101,97,100,121,113,201,101,60,0,0,3,32,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,112,97,99,107,
101,116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,61,34,87,53,77,48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,32,60,120,58,120,109,112,109,101,116,97,
32,120,109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,48,45,99,48,54,48,
32,54,49,46,49,51,52,55,55,55,44,32,50,48,49,48,47,48,50,47,49,50,45,49,55,58,51,50,58,48,48,32,32,32,32,32,32,32,32,34,62,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,
116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,62,32,60,114,100,102,58,68,101,115,99,114,105,112,116,
105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,32,120,109,108,110,115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,
32,120,109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,109,109,47,34,32,120,109,108,110,115,58,115,116,82,101,
102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,82,101,102,35,34,32,120,109,112,58,67,114,
101,97,116,111,114,84,111,111,108,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,83,53,32,87,105,110,100,111,119,115,34,32,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,
61,34,120,109,112,46,105,105,100,58,66,65,53,67,48,53,49,50,52,66,70,48,49,49,69,50,56,52,67,68,65,69,66,65,66,70,68,56,66,54,70,49,34,32,120,109,112,77,77,58,68,111,99,117,109,101,110,116,73,68,61,34,
120,109,112,46,100,105,100,58,66,65,53,67,48,53,49,51,52,66,70,48,49,49,69,50,56,52,67,68,65,69,66,65,66,70,68,56,66,54,70,49,34,62,32,60,120,109,112,77,77,58,68,101,114,105,118,101,100,70,114,111,109,
32,115,116,82,101,102,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,66,65,53,67,48,53,49,48,52,66,70,48,49,49,69,50,56,52,67,68,65,69,66,65,66,70,68,56,66,54,70,49,34,32,115,
116,82,101,102,58,100,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,66,65,53,67,48,53,49,49,52,66,70,48,49,49,69,50,56,52,67,68,65,69,66,65,66,70,68,56,66,54,70,49,34,47,62,32,60,
47,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,62,32,60,47,114,100,102,58,82,68,70,62,32,60,47,120,58,120,109,112,109,101,116,97,62,32,60,63,120,112,97,99,107,101,116,32,101,110,100,61,34,
114,34,63,62,174,47,76,225,0,0,0,237,73,68,65,84,120,218,98,100,128,0,30,32,14,7,98,102,6,220,224,17,16,239,0,98,81,32,222,2,196,23,128,56,29,36,193,15,196,183,129,248,63,30,252,16,136,21,161,154,75,129,
120,47,84,124,38,200,128,124,40,167,154,1,63,0,105,190,10,85,219,5,196,187,128,248,24,72,162,2,42,200,70,164,102,24,46,130,186,30,110,0,46,32,129,69,243,25,32,22,130,41,192,103,0,62,205,114,64,156,70,
200,128,93,120,108,190,7,196,237,76,120,156,94,4,141,182,251,80,254,89,32,118,6,226,119,80,190,34,33,47,28,6,226,63,80,131,86,192,2,12,9,128,244,180,179,224,176,29,228,76,93,32,222,10,196,191,128,56,2,
151,51,113,25,0,210,36,12,196,127,9,164,13,156,6,124,97,32,18,32,7,34,27,3,241,128,13,217,128,215,72,161,78,44,128,169,189,3,203,76,247,8,100,38,108,24,164,135,159,17,106,18,63,52,59,27,19,233,2,80,86,
94,6,196,31,1,2,12,0,13,182,82,227,225,13,237,248,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PresetsComponent::rename_png = (const char*) resource_PresetsComponent_rename_png;
const int PresetsComponent::rename_pngSize = 1143;

// JUCER_RESOURCE: reload_png, 1106, "resources/reload.png"
static const unsigned char resource_PresetsComponent_reload_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,16,0,0,0,16,8,6,0,0,0,31,243,255,97,0,0,0,25,116,69,88,116,83,111,102,116,119,
97,114,101,0,65,100,111,98,101,32,73,109,97,103,101,82,101,97,100,121,113,201,101,60,0,0,3,32,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,112,97,99,107,
101,116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,61,34,87,53,77,48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,32,60,120,58,120,109,112,109,101,116,97,
32,120,109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,48,45,99,48,54,48,
32,54,49,46,49,51,52,55,55,55,44,32,50,48,49,48,47,48,50,47,49,50,45,49,55,58,51,50,58,48,48,32,32,32,32,32,32,32,32,34,62,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,
116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,62,32,60,114,100,102,58,68,101,115,99,114,105,112,116,
105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,32,120,109,108,110,115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,
32,120,109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,109,109,47,34,32,120,109,108,110,115,58,115,116,82,101,
102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,82,101,102,35,34,32,120,109,112,58,67,114,
101,97,116,111,114,84,111,111,108,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,83,53,32,87,105,110,100,111,119,115,34,32,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,
61,34,120,109,112,46,105,105,100,58,68,66,54,56,68,57,55,50,52,66,70,50,49,49,69,50,57,66,70,53,70,57,65,54,54,66,69,55,51,51,48,67,34,32,120,109,112,77,77,58,68,111,99,117,109,101,110,116,73,68,61,34,
120,109,112,46,100,105,100,58,68,66,54,56,68,57,55,51,52,66,70,50,49,49,69,50,57,66,70,53,70,57,65,54,54,66,69,55,51,51,48,67,34,62,32,60,120,109,112,77,77,58,68,101,114,105,118,101,100,70,114,111,109,
32,115,116,82,101,102,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,68,66,54,56,68,57,55,48,52,66,70,50,49,49,69,50,57,66,70,53,70,57,65,54,54,66,69,55,51,51,48,67,34,32,115,
116,82,101,102,58,100,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,68,66,54,56,68,57,55,49,52,66,70,50,49,49,69,50,57,66,70,53,70,57,65,54,54,66,69,55,51,51,48,67,34,47,62,32,60,
47,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,62,32,60,47,114,100,102,58,82,68,70,62,32,60,47,120,58,120,109,112,109,101,116,97,62,32,60,63,120,112,97,99,107,101,116,32,101,110,100,61,34,
114,34,63,62,61,203,206,165,0,0,0,200,73,68,65,84,120,218,98,96,24,64,16,14,34,24,113,72,114,2,177,43,16,75,64,249,47,128,120,55,16,127,135,242,235,128,56,4,136,245,176,105,46,2,226,15,64,252,31,13,127,
128,202,85,64,249,151,176,105,94,132,164,225,42,16,207,132,226,171,88,12,196,48,160,20,42,241,25,136,163,176,24,190,2,159,1,32,63,191,133,74,120,96,209,156,76,200,5,222,80,193,99,88,52,199,98,209,12,55,
128,5,170,72,29,74,31,199,98,192,101,32,54,193,34,254,29,217,0,124,224,2,49,137,2,159,23,208,1,40,128,211,128,88,20,61,16,63,224,9,68,24,240,64,74,19,156,184,162,241,3,142,104,140,66,138,169,82,92,54,
44,69,11,229,105,80,124,9,73,124,41,33,63,226,75,202,24,54,19,202,76,210,80,254,83,180,204,4,7,0,1,6,0,33,129,81,8,252,149,60,155,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PresetsComponent::reload_png = (const char*) resource_PresetsComponent_reload_png;
const int PresetsComponent::reload_pngSize = 1106;

// JUCER_RESOURCE: save_png, 986, "resources/save.png"
static const unsigned char resource_PresetsComponent_save_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,16,0,0,0,16,8,6,0,0,0,31,243,255,97,0,0,0,25,116,69,88,116,83,111,102,116,119,97,
114,101,0,65,100,111,98,101,32,73,109,97,103,101,82,101,97,100,121,113,201,101,60,0,0,3,32,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,112,97,99,107,101,
116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,61,34,87,53,77,48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,32,60,120,58,120,109,112,109,101,116,97,32,120,
109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,48,45,99,48,54,48,32,54,49,
46,49,51,52,55,55,55,44,32,50,48,49,48,47,48,50,47,49,50,45,49,55,58,51,50,58,48,48,32,32,32,32,32,32,32,32,34,62,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,
112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,62,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,
110,32,114,100,102,58,97,98,111,117,116,61,34,34,32,120,109,108,110,115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,32,120,
109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,109,109,47,34,32,120,109,108,110,115,58,115,116,82,101,102,61,
34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,82,101,102,35,34,32,120,109,112,58,67,114,101,97,
116,111,114,84,111,111,108,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,83,53,32,87,105,110,100,111,119,115,34,32,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,61,34,120,
109,112,46,105,105,100,58,66,51,48,57,55,65,57,54,52,66,70,48,49,49,69,50,65,67,67,49,69,56,70,68,65,53,53,55,54,70,49,67,34,32,120,109,112,77,77,58,68,111,99,117,109,101,110,116,73,68,61,34,120,109,112,
46,100,105,100,58,66,51,48,57,55,65,57,55,52,66,70,48,49,49,69,50,65,67,67,49,69,56,70,68,65,53,53,55,54,70,49,67,34,62,32,60,120,109,112,77,77,58,68,101,114,105,118,101,100,70,114,111,109,32,115,116,
82,101,102,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,66,51,48,57,55,65,57,52,52,66,70,48,49,49,69,50,65,67,67,49,69,56,70,68,65,53,53,55,54,70,49,67,34,32,115,116,82,101,
102,58,100,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,66,51,48,57,55,65,57,53,52,66,70,48,49,49,69,50,65,67,67,49,69,56,70,68,65,53,53,55,54,70,49,67,34,47,62,32,60,47,114,100,
102,58,68,101,115,99,114,105,112,116,105,111,110,62,32,60,47,114,100,102,58,82,68,70,62,32,60,47,120,58,120,109,112,109,101,116,97,62,32,60,63,120,112,97,99,107,101,116,32,101,110,100,61,34,114,34,63,
62,238,197,184,18,0,0,0,80,73,68,65,84,120,218,98,96,160,18,248,79,2,174,35,100,0,33,113,172,134,144,106,0,220,16,70,36,65,24,96,36,194,187,112,181,52,49,128,20,64,185,1,232,1,147,79,132,166,124,108,1,
14,19,96,35,144,46,24,160,106,112,26,64,106,194,27,132,46,32,217,0,170,69,35,217,0,32,192,0,244,70,71,56,71,208,237,106,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PresetsComponent::save_png = (const char*) resource_PresetsComponent_save_png;
const int PresetsComponent::save_pngSize = 986;

// JUCER_RESOURCE: saveasnew_png, 1029, "resources/saveasnew.png"
static const unsigned char resource_PresetsComponent_saveasnew_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,16,0,0,0,16,8,6,0,0,0,31,243,255,97,0,0,0,25,116,69,88,116,83,111,102,116,119,
97,114,101,0,65,100,111,98,101,32,73,109,97,103,101,82,101,97,100,121,113,201,101,60,0,0,3,32,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,112,97,99,107,
101,116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,61,34,87,53,77,48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,32,60,120,58,120,109,112,109,101,116,97,
32,120,109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,48,45,99,48,54,48,
32,54,49,46,49,51,52,55,55,55,44,32,50,48,49,48,47,48,50,47,49,50,45,49,55,58,51,50,58,48,48,32,32,32,32,32,32,32,32,34,62,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,
116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,62,32,60,114,100,102,58,68,101,115,99,114,105,112,116,
105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,32,120,109,108,110,115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,
32,120,109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,109,109,47,34,32,120,109,108,110,115,58,115,116,82,101,
102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,82,101,102,35,34,32,120,109,112,58,67,114,
101,97,116,111,114,84,111,111,108,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,83,53,32,87,105,110,100,111,119,115,34,32,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,
61,34,120,109,112,46,105,105,100,58,68,66,50,53,65,52,54,56,52,66,70,48,49,49,69,50,65,67,49,68,68,70,70,70,69,48,68,65,65,48,54,69,34,32,120,109,112,77,77,58,68,111,99,117,109,101,110,116,73,68,61,34,
120,109,112,46,100,105,100,58,68,66,50,53,65,52,54,57,52,66,70,48,49,49,69,50,65,67,49,68,68,70,70,70,69,48,68,65,65,48,54,69,34,62,32,60,120,109,112,77,77,58,68,101,114,105,118,101,100,70,114,111,109,
32,115,116,82,101,102,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,68,66,50,53,65,52,54,54,52,66,70,48,49,49,69,50,65,67,49,68,68,70,70,70,69,48,68,65,65,48,54,69,34,32,115,
116,82,101,102,58,100,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,68,66,50,53,65,52,54,55,52,66,70,48,49,49,69,50,65,67,49,68,68,70,70,70,69,48,68,65,65,48,54,69,34,47,62,32,60,
47,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,62,32,60,47,114,100,102,58,82,68,70,62,32,60,47,120,58,120,109,112,109,101,116,97,62,32,60,63,120,112,97,99,107,101,116,32,101,110,100,61,34,
114,34,63,62,159,129,87,159,0,0,0,123,73,68,65,84,120,218,98,248,255,255,63,3,8,3,193,25,32,254,143,7,31,131,169,69,198,140,80,205,12,140,140,140,16,6,30,0,84,203,136,46,134,98,0,186,2,116,67,177,25,192,
128,228,133,255,132,48,146,186,51,88,189,128,213,6,36,215,128,228,97,174,130,169,37,57,12,240,26,48,234,2,236,6,192,21,210,202,5,200,9,105,62,129,132,52,7,57,193,193,245,97,203,32,184,48,16,68,17,101,
0,16,124,32,224,154,45,24,73,153,92,0,16,96,0,99,131,19,255,20,30,163,96,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PresetsComponent::saveasnew_png = (const char*) resource_PresetsComponent_saveasnew_png;
const int PresetsComponent::saveasnew_pngSize = 1029;

// JUCER_RESOURCE: left_png, 997, "resources/left.png"
static const unsigned char resource_PresetsComponent_left_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,16,0,0,0,16,8,6,0,0,0,31,243,255,97,0,0,0,25,116,69,88,116,83,111,102,116,119,97,
114,101,0,65,100,111,98,101,32,73,109,97,103,101,82,101,97,100,121,113,201,101,60,0,0,3,32,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,112,97,99,107,101,
116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,61,34,87,53,77,48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,32,60,120,58,120,109,112,109,101,116,97,32,120,
109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,48,45,99,48,54,48,32,54,49,
46,49,51,52,55,55,55,44,32,50,48,49,48,47,48,50,47,49,50,45,49,55,58,51,50,58,48,48,32,32,32,32,32,32,32,32,34,62,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,
112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,62,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,
110,32,114,100,102,58,97,98,111,117,116,61,34,34,32,120,109,108,110,115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,32,120,
109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,109,109,47,34,32,120,109,108,110,115,58,115,116,82,101,102,61,
34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,82,101,102,35,34,32,120,109,112,58,67,114,101,97,
116,111,114,84,111,111,108,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,83,53,32,87,105,110,100,111,119,115,34,32,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,61,34,120,
109,112,46,105,105,100,58,70,53,57,55,69,48,69,57,52,66,69,70,49,49,69,50,65,52,70,57,67,67,69,70,52,52,55,48,55,55,49,51,34,32,120,109,112,77,77,58,68,111,99,117,109,101,110,116,73,68,61,34,120,109,112,
46,100,105,100,58,70,53,57,55,69,48,69,65,52,66,69,70,49,49,69,50,65,52,70,57,67,67,69,70,52,52,55,48,55,55,49,51,34,62,32,60,120,109,112,77,77,58,68,101,114,105,118,101,100,70,114,111,109,32,115,116,
82,101,102,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,70,53,57,55,69,48,69,55,52,66,69,70,49,49,69,50,65,52,70,57,67,67,69,70,52,52,55,48,55,55,49,51,34,32,115,116,82,101,
102,58,100,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,70,53,57,55,69,48,69,56,52,66,69,70,49,49,69,50,65,52,70,57,67,67,69,70,52,52,55,48,55,55,49,51,34,47,62,32,60,47,114,100,
102,58,68,101,115,99,114,105,112,116,105,111,110,62,32,60,47,114,100,102,58,82,68,70,62,32,60,47,120,58,120,109,112,109,101,116,97,62,32,60,63,120,112,97,99,107,101,116,32,101,110,100,61,34,114,34,63,
62,37,40,48,235,0,0,0,91,73,68,65,84,120,218,98,252,255,255,63,3,37,128,113,192,13,96,0,25,128,11,3,65,5,62,121,176,26,60,154,219,129,248,3,89,6,64,53,255,39,203,0,36,205,248,240,7,172,6,16,169,25,197,
0,38,180,48,101,166,56,22,128,160,139,108,47,96,49,228,3,37,209,216,69,145,1,80,67,170,9,25,192,56,244,51,19,64,128,1,0,31,176,101,252,251,62,140,199,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PresetsComponent::left_png = (const char*) resource_PresetsComponent_left_png;
const int PresetsComponent::left_pngSize = 997;

// JUCER_RESOURCE: rescan_png, 1271, "resources/rescan.png"
static const unsigned char resource_PresetsComponent_rescan_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,16,0,0,0,16,8,6,0,0,0,31,243,255,97,0,0,0,25,116,69,88,116,83,111,102,116,119,
97,114,101,0,65,100,111,98,101,32,73,109,97,103,101,82,101,97,100,121,113,201,101,60,0,0,3,32,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,112,97,99,107,
101,116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,61,34,87,53,77,48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,32,60,120,58,120,109,112,109,101,116,97,
32,120,109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,48,45,99,48,54,48,
32,54,49,46,49,51,52,55,55,55,44,32,50,48,49,48,47,48,50,47,49,50,45,49,55,58,51,50,58,48,48,32,32,32,32,32,32,32,32,34,62,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,
116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,62,32,60,114,100,102,58,68,101,115,99,114,105,112,116,
105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,32,120,109,108,110,115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,
32,120,109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,109,109,47,34,32,120,109,108,110,115,58,115,116,82,101,
102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,82,101,102,35,34,32,120,109,112,58,67,114,
101,97,116,111,114,84,111,111,108,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,83,53,32,87,105,110,100,111,119,115,34,32,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,
61,34,120,109,112,46,105,105,100,58,69,51,51,57,70,66,54,49,52,66,70,50,49,49,69,50,66,66,52,65,69,70,55,65,51,55,51,65,66,69,52,68,34,32,120,109,112,77,77,58,68,111,99,117,109,101,110,116,73,68,61,34,
120,109,112,46,100,105,100,58,69,51,51,57,70,66,54,50,52,66,70,50,49,49,69,50,66,66,52,65,69,70,55,65,51,55,51,65,66,69,52,68,34,62,32,60,120,109,112,77,77,58,68,101,114,105,118,101,100,70,114,111,109,
32,115,116,82,101,102,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,69,51,51,57,70,66,53,70,52,66,70,50,49,49,69,50,66,66,52,65,69,70,55,65,51,55,51,65,66,69,52,68,34,32,115,
116,82,101,102,58,100,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,69,51,51,57,70,66,54,48,52,66,70,50,49,49,69,50,66,66,52,65,69,70,55,65,51,55,51,65,66,69,52,68,34,47,62,32,60,
47,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,62,32,60,47,114,100,102,58,82,68,70,62,32,60,47,120,58,120,109,112,109,101,116,97,62,32,60,63,120,112,97,99,107,101,116,32,101,110,100,61,34,
114,34,63,62,228,203,60,249,0,0,1,109,73,68,65,84,120,218,124,147,79,68,68,81,20,135,223,51,26,134,217,182,138,33,82,12,179,138,168,85,90,69,180,109,211,34,134,161,68,12,109,91,205,170,68,219,105,25,17,
173,74,105,145,18,81,155,34,101,40,69,74,137,22,49,100,82,38,175,223,25,223,227,118,223,212,225,243,222,189,231,220,115,207,191,27,4,73,233,19,83,98,75,52,193,254,43,232,254,149,89,81,22,41,81,16,13,17,
137,7,209,35,54,209,39,36,45,246,196,173,24,113,246,55,112,96,100,112,220,196,54,237,58,88,113,12,141,42,251,227,206,94,138,20,226,245,114,124,184,128,215,200,195,114,206,241,127,137,109,213,209,191,136,
124,200,237,231,98,95,244,139,81,81,34,196,33,113,34,102,68,93,172,227,104,73,220,83,151,150,65,202,171,137,41,142,56,80,35,149,6,145,76,96,211,109,186,144,22,93,112,187,241,37,158,197,169,232,18,119,
172,119,197,35,105,229,197,188,217,155,131,79,175,162,111,164,83,199,97,224,124,125,121,15,104,157,21,229,137,30,103,69,145,194,174,137,87,49,45,174,219,20,186,102,185,247,138,15,49,44,14,197,32,253,239,
160,85,147,68,104,250,109,113,198,96,89,106,87,113,27,7,8,41,67,123,226,27,114,206,52,30,160,143,165,194,217,150,44,48,129,69,47,68,187,249,216,89,91,23,198,24,168,69,127,148,119,8,205,119,48,215,38,247,
196,40,199,82,118,66,142,168,71,150,155,35,116,191,30,83,248,199,115,46,145,210,55,83,106,83,215,41,86,197,141,107,252,35,192,0,208,162,122,16,10,12,57,70,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PresetsComponent::rescan_png = (const char*) resource_PresetsComponent_rescan_png;
const int PresetsComponent::rescan_pngSize = 1271;

// JUCER_RESOURCE: right_png, 988, "resources/right.png"
static const unsigned char resource_PresetsComponent_right_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,16,0,0,0,16,8,6,0,0,0,31,243,255,97,0,0,0,25,116,69,88,116,83,111,102,116,119,97,
114,101,0,65,100,111,98,101,32,73,109,97,103,101,82,101,97,100,121,113,201,101,60,0,0,3,32,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,112,97,99,107,101,
116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,61,34,87,53,77,48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,32,60,120,58,120,109,112,109,101,116,97,32,120,
109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,48,45,99,48,54,48,32,54,49,
46,49,51,52,55,55,55,44,32,50,48,49,48,47,48,50,47,49,50,45,49,55,58,51,50,58,48,48,32,32,32,32,32,32,32,32,34,62,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,
112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,62,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,
110,32,114,100,102,58,97,98,111,117,116,61,34,34,32,120,109,108,110,115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,32,120,
109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,109,109,47,34,32,120,109,108,110,115,58,115,116,82,101,102,61,
34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,82,101,102,35,34,32,120,109,112,58,67,114,101,97,
116,111,114,84,111,111,108,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,83,53,32,87,105,110,100,111,119,115,34,32,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,61,34,120,
109,112,46,105,105,100,58,70,55,51,51,49,68,55,52,52,66,69,70,49,49,69,50,57,70,69,68,70,67,56,56,65,66,69,55,65,55,54,68,34,32,120,109,112,77,77,58,68,111,99,117,109,101,110,116,73,68,61,34,120,109,112,
46,100,105,100,58,70,55,51,51,49,68,55,53,52,66,69,70,49,49,69,50,57,70,69,68,70,67,56,56,65,66,69,55,65,55,54,68,34,62,32,60,120,109,112,77,77,58,68,101,114,105,118,101,100,70,114,111,109,32,115,116,
82,101,102,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,70,55,51,51,49,68,55,50,52,66,69,70,49,49,69,50,57,70,69,68,70,67,56,56,65,66,69,55,65,55,54,68,34,32,115,116,82,101,
102,58,100,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,70,55,51,51,49,68,55,51,52,66,69,70,49,49,69,50,57,70,69,68,70,67,56,56,65,66,69,55,65,55,54,68,34,47,62,32,60,47,114,100,
102,58,68,101,115,99,114,105,112,116,105,111,110,62,32,60,47,114,100,102,58,82,68,70,62,32,60,47,120,58,120,109,112,109,101,116,97,62,32,60,63,120,112,97,99,107,101,116,32,101,110,100,61,34,114,34,63,
62,96,222,251,176,0,0,0,82,73,68,65,84,120,218,98,252,255,255,63,3,37,128,113,192,13,96,0,25,128,15,3,1,63,16,151,226,148,39,210,128,255,184,12,33,197,0,172,134,224,82,136,15,151,82,106,0,138,33,76,100,
134,61,51,70,44,144,224,130,10,74,2,177,130,146,88,168,160,36,26,43,112,201,51,14,253,204,4,16,96,0,75,183,75,131,95,54,204,43,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* PresetsComponent::right_png = (const char*) resource_PresetsComponent_right_png;
const int PresetsComponent::right_pngSize = 988;


//[EndFile] You can add extra defines here...
//[/EndFile]
