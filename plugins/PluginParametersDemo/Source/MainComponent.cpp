/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  19 Jul 2013 8:03:11pm

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
//[/Headers]

#include "MainComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MainComponent::MainComponent (MyPluginProcessor* processor)
    : AudioProcessorEditor(processor), processor(processor),
      logSlider (0),
      logWith0Slider (0),
      symSignedLogSlider (0),
      asymSignedLogSlider (0),
      floatSlider (0),
      comboBox (0),
      toggleButton (0),
      textButton (0),
      label (0),
      label2 (0),
      label3 (0)
{
    addAndMakeVisible (logSlider = new Slider ("new slider"));
    logSlider->setRange (0, 10, 0);
    logSlider->setSliderStyle (Slider::LinearHorizontal);
    logSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    logSlider->addListener (this);

    addAndMakeVisible (logWith0Slider = new Slider ("new slider"));
    logWith0Slider->setRange (0, 10, 0);
    logWith0Slider->setSliderStyle (Slider::LinearHorizontal);
    logWith0Slider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    logWith0Slider->addListener (this);

    addAndMakeVisible (symSignedLogSlider = new Slider ("new slider"));
    symSignedLogSlider->setRange (0, 10, 0);
    symSignedLogSlider->setSliderStyle (Slider::LinearHorizontal);
    symSignedLogSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    symSignedLogSlider->addListener (this);

    addAndMakeVisible (asymSignedLogSlider = new Slider ("new slider"));
    asymSignedLogSlider->setRange (0, 10, 0);
    asymSignedLogSlider->setSliderStyle (Slider::LinearHorizontal);
    asymSignedLogSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    asymSignedLogSlider->addListener (this);

    addAndMakeVisible (floatSlider = new Slider (String::empty));
    floatSlider->setRange (0, 10, 0);
    floatSlider->setSliderStyle (Slider::LinearHorizontal);
    floatSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    floatSlider->addListener (this);

    addAndMakeVisible (comboBox = new ComboBox ("new combo box"));
    comboBox->setEditableText (false);
    comboBox->setJustificationType (Justification::centredLeft);
    comboBox->setTextWhenNothingSelected (String::empty);
    comboBox->setTextWhenNoChoicesAvailable ("(no choices)");
    comboBox->addItem ("Custom", 1);
    comboBox->addItem ("Linear 0.0 (mute)", 2);
    comboBox->addItem ("Linear 1.0 (bypass)", 3);
    comboBox->addItem ("Linear 2.0", 4);
    comboBox->addListener (this);

    addAndMakeVisible (toggleButton = new ToggleButton ("new toggle button"));
    toggleButton->setButtonText ("bypass");
    toggleButton->addListener (this);

    addAndMakeVisible (textButton = new TextButton ("new button"));
    textButton->setButtonText ("reset");
    textButton->addListener (this);

    addAndMakeVisible (label = new Label ("new label",
                                          "LOG GAIN"));
    label->setFont (Font (15.0000f, Font::plain));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (Label::backgroundColourId, Colour (0x590a243c));
    label->setColour (Label::textColourId, Colours::white);
    label->setColour (TextEditor::textColourId, Colour (0xff266177));
    label->setColour (TextEditor::backgroundColourId, Colour (0x92979e));

    addAndMakeVisible (label2 = new Label ("new label",
                                           "LINEAR GAIN"));
    label2->setFont (Font (15.0000f, Font::plain));
    label2->setJustificationType (Justification::centredRight);
    label2->setEditable (false, false, false);
    label2->setColour (Label::backgroundColourId, Colour (0x590a243c));
    label2->setColour (Label::textColourId, Colours::white);
    label2->setColour (TextEditor::textColourId, Colour (0xff266177));
    label2->setColour (TextEditor::backgroundColourId, Colour (0x92979e));

    addAndMakeVisible (label3 = new Label ("new label",
                                           "Settings"));
    label3->setFont (Font (15.0000f, Font::plain));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x0));


    //[UserPreSize]
    FloatParam *floatParam=processor->getFloatParam(MyPluginProcessor::floatIndex);
    LogParam *logParam=processor->getLogParam(MyPluginProcessor::logIndex);
    LogWith0Param *logWith0Param=processor->getLogWith0Param(MyPluginProcessor::logWith0Index);
    LogWithSignParam *logWithSignParam=processor->getLogWithSignParam(MyPluginProcessor::symSignedLogIndex);
    LogWithSignParam *alogWithSignParam=processor->getLogWithSignParam(MyPluginProcessor::asymSignedLogIndex);

    floatSlider->setRange(floatParam->getMin(),floatParam->getMax(),floatSlider->getInterval());
    logSlider->setRange(logParam->getMin(),logParam->getMax(),logSlider->getInterval());
    logWith0Slider->setRange(logWith0Param->getMin(),logWith0Param->getMax(),logWith0Slider->getInterval());
    symSignedLogSlider->setRange(logWithSignParam->getMin(),logWithSignParam->getMax(),symSignedLogSlider->getInterval());
    asymSignedLogSlider->setRange(alogWithSignParam->getMin(),alogWithSignParam->getMax(),asymSignedLogSlider->getInterval());

    processor->updateUi(true,true);

    timerCallback();
    //[/UserPreSize]

    setSize (400, 200);


    //[Constructor] You can add your own custom stuff here..
    startTimer (50);
    //[/Constructor]
}

MainComponent::~MainComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (logSlider);
    deleteAndZero (logWith0Slider);
    deleteAndZero (symSignedLogSlider);
    deleteAndZero (asymSignedLogSlider);
    deleteAndZero (floatSlider);
    deleteAndZero (comboBox);
    deleteAndZero (toggleButton);
    deleteAndZero (textButton);
    deleteAndZero (label);
    deleteAndZero (label2);
    deleteAndZero (label3);


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff3c6292));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MainComponent::resized()
{
    logSlider->setBounds (13, 90, 150, 24);
    logWith0Slider->setBounds (13, 114, 150, 24);
    symSignedLogSlider->setBounds (13, 138, 150, 24);
    asymSignedLogSlider->setBounds (13, 162, 150, 24);
    floatSlider->setBounds (237, 90, 150, 24);
    comboBox->setBounds (79, 11, 165, 24);
    toggleButton->setBounds (311, 11, 78, 24);
    textButton->setBounds (303, 159, 84, 24);
    label->setBounds (12, 56, 150, 24);
    label2->setBounds (237, 56, 150, 24);
    label3->setBounds (9, 11, 75, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MainComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == logSlider)
    {
        //[UserSliderCode_logSlider] -- add your slider handling code here..
        LogParam *param=processor->getLogParam(MyPluginProcessor::logIndex);
        param->updateProcessorAndHostFromUi(logSlider->getValue());
        //[/UserSliderCode_logSlider]
    }
    else if (sliderThatWasMoved == logWith0Slider)
    {
        //[UserSliderCode_logWith0Slider] -- add your slider handling code here..
        LogWith0Param *param=processor->getLogWith0Param(MyPluginProcessor::logWith0Index);
        param->updateProcessorAndHostFromUi(logWith0Slider->getValue());
        //[/UserSliderCode_logWith0Slider]
    }
    else if (sliderThatWasMoved == symSignedLogSlider)
    {
        //[UserSliderCode_symSignedLogSlider] -- add your slider handling code here..
        LogWithSignParam *param=processor->getLogWithSignParam(MyPluginProcessor::symSignedLogIndex);
        param->updateProcessorAndHostFromUi(symSignedLogSlider->getValue());
        //[/UserSliderCode_symSignedLogSlider]
    }
    else if (sliderThatWasMoved == asymSignedLogSlider)
    {
        //[UserSliderCode_asymSignedLogSlider] -- add your slider handling code here..
        LogWithSignParam *param=processor->getLogWithSignParam(MyPluginProcessor::asymSignedLogIndex);
        param->updateProcessorAndHostFromUi(asymSignedLogSlider->getValue());
        //[/UserSliderCode_asymSignedLogSlider]
    }
    else if (sliderThatWasMoved == floatSlider)
    {
        //[UserSliderCode_floatSlider] -- add your slider handling code here..
        FloatParam *param=processor->getFloatParam(MyPluginProcessor::floatIndex);
        param->updateProcessorAndHostFromUi(floatSlider->getValue());
        //[/UserSliderCode_floatSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void MainComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBox)
    {
        //[UserComboBoxCode_comboBox] -- add your combo box handling code here..
        if (comboBox->getSelectedId()>0){
          IntParam* param=processor->getIntParam(MyPluginProcessor::intIndex);
          param->updateProcessorAndHostFromUi(comboBox->getSelectedId()-1);
        }
        //[/UserComboBoxCode_comboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void MainComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == toggleButton)
    {
        //[UserButtonCode_toggleButton] -- add your button handler code here..
        BoolParam *param=processor->getBoolParam(MyPluginProcessor::boolIndex);
        param->updateProcessorAndHostFromUi(toggleButton->getToggleState());
        //[/UserButtonCode_toggleButton]
    }
    else if (buttonThatWasClicked == textButton)
    {
        //[UserButtonCode_textButton] -- add your button handler code here..
        BoolParam *param=processor->getBoolParam(MyPluginProcessor::boolButtonIndex);
        param->updateProcessorAndHostFromUi(!param->uiGet());
        //[/UserButtonCode_textButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void MainComponent::timerCallback(){

    FloatParam *floatParam=processor->getFloatParam(MyPluginProcessor::floatIndex);
    if (floatSlider && floatParam->updateUiRequested()){
      floatSlider->setValue (floatParam->uiGet(), dontSendNotification);
    }

    LogParam *logParam=processor->getLogParam(MyPluginProcessor::logIndex);
    if (logSlider && logParam->updateUiRequested()){
      logSlider->setValue (logParam->uiGet(), dontSendNotification);
    }

    LogWith0Param *logWith0Param=processor->getLogWith0Param(MyPluginProcessor::logWith0Index);
    if (logWith0Slider && logWith0Param->updateUiRequested()){
      logWith0Slider->setValue (logWith0Param->uiGet(), dontSendNotification);
    }

    LogWithSignParam *logWithSignParam=processor->getLogWithSignParam(MyPluginProcessor::symSignedLogIndex);
    if (symSignedLogSlider && logWithSignParam->updateUiRequested()){
      symSignedLogSlider->setValue (logWithSignParam->uiGet(), dontSendNotification);
    }

    logWithSignParam=processor->getLogWithSignParam(MyPluginProcessor::asymSignedLogIndex);
    if (asymSignedLogSlider && logWithSignParam->updateUiRequested()){
      asymSignedLogSlider->setValue (logWithSignParam->uiGet(), dontSendNotification);
    }

    IntParam *intParam=processor->getIntParam(MyPluginProcessor::intIndex);
    if (comboBox && intParam->updateUiRequested()){
      comboBox->setSelectedId (intParam->uiGet()+1, dontSendNotification);
    }

    BoolParam * const boolParam=processor->getBoolParam(MyPluginProcessor::boolIndex);
    if (toggleButton && boolParam->updateUiRequested()){
      toggleButton->setToggleState (boolParam->uiGet(), dontSendNotification);
    }

}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MainComponent" componentName=""
                 parentClasses="public AudioProcessorEditor, public Timer" constructorParams="MyPluginProcessor* processor"
                 variableInitialisers="AudioProcessorEditor(processor), processor(processor)"
                 snapPixels="8" snapActive="0" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="400" initialHeight="200">
  <BACKGROUND backgroundColour="ff3c6292"/>
  <SLIDER name="new slider" id="a5d6233074112b4" memberName="logSlider"
          virtualName="" explicitFocusOrder="0" pos="13 90 150 24" min="0"
          max="10" int="0" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="54e3452881627b37" memberName="logWith0Slider"
          virtualName="" explicitFocusOrder="0" pos="13 114 150 24" min="0"
          max="10" int="0" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="66f7bfbc2934e087" memberName="symSignedLogSlider"
          virtualName="" explicitFocusOrder="0" pos="13 138 150 24" min="0"
          max="10" int="0" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="dffe0563584a9c80" memberName="asymSignedLogSlider"
          virtualName="" explicitFocusOrder="0" pos="13 162 150 24" min="0"
          max="10" int="0" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="" id="740babeabc6ef7d5" memberName="floatSlider" virtualName=""
          explicitFocusOrder="0" pos="237 90 150 24" min="0" max="10" int="0"
          style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <COMBOBOX name="new combo box" id="f289956ff4c19016" memberName="comboBox"
            virtualName="" explicitFocusOrder="0" pos="79 11 165 24" editable="0"
            layout="33" items="Custom&#10;Linear 0.0 (mute)&#10;Linear 1.0 (bypass)&#10;Linear 2.0"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TOGGLEBUTTON name="new toggle button" id="66ae8e553c25df8d" memberName="toggleButton"
                virtualName="" explicitFocusOrder="0" pos="311 11 78 24" buttonText="bypass"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTBUTTON name="new button" id="93165ce6899027b0" memberName="textButton"
              virtualName="" explicitFocusOrder="0" pos="303 159 84 24" buttonText="reset"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="cfe5f4afbc32a96" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="12 56 150 24" bkgCol="590a243c" textCol="ffffffff"
         edTextCol="ff266177" edBkgCol="92979e" labelText="LOG GAIN" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="3ccb78d98753f837" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="237 56 150 24" bkgCol="590a243c"
         textCol="ffffffff" edTextCol="ff266177" edBkgCol="92979e" labelText="LINEAR GAIN"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="6760eb32c21991bd" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="9 11 75 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Settings" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif



//[EndFile] You can add extra defines here...
//[/EndFile]
