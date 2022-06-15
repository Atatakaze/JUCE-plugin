/*
  ==============================================================================

    modeComponent.cpp
    Created: 27 May 2022 10:40:06am
    Author:  bobo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "modeComponent.h"


//==============================================================================
ModeComponent::ModeComponent(DemoAudioProcessor& p) : processor(p)
{
    addAndMakeVisible(mode1Label);
    mode1Label.setText("Regular\n(mono/stereo)", dontSendNotification);
    mode1Label.setFont(Font(10.0f));
    mode1Label.setJustificationType(Justification::centred);

    addAndMakeVisible(mode2Label);
    mode2Label.setText("HRTF\n(mono2stereo) ", dontSendNotification);
    mode2Label.setFont(Font(10.0f));
    mode2Label.setJustificationType(Justification::centred);
    
    // toggle button 1
    addAndMakeVisible(mode1);
    mode1.addListener(this);
    mode1.setLookAndFeel(&mode1Look);

    // toggle button 2
    addAndMakeVisible(mode2);
    mode2.addListener(this);
    mode2.setLookAndFeel(&mode2Look);

    modeSwitched();
}


ModeComponent::~ModeComponent()
{
    mode1.removeListener(this);
    mode1.setLookAndFeel(nullptr);
    mode2.removeListener(this);
    mode2.setLookAndFeel(nullptr);
}

//==============================================================================
void ModeComponent::paint (Graphics& g)
{
}

//==============================================================================
void ModeComponent::resized()
{
    auto bounds = getLocalBounds();

    auto mode1Area = bounds.removeFromLeft(bounds.getWidth() / 2);
    auto btn1Area = mode1Area.removeFromLeft(10);
    btn1Area.removeFromTop(12);
    btn1Area.removeFromBottom(13);
    mode1.setBounds(btn1Area);
    mode1Label.setBounds(mode1Area);
    
    auto mode2Area = bounds;
    auto btn2Area = mode2Area.removeFromLeft(10);
    btn2Area.removeFromTop(12);
    btn2Area.removeFromBottom(13);
    mode2.setBounds(btn2Area);
    mode2Label.setBounds(mode2Area);
}

//==============================================================================
void ModeComponent::buttonClicked(Button* button)
{
    if (button == &mode1)
    {
        processor.setMode(1);
        modeSwitched();
    }
    else if(button == &mode2)
    {
        processor.setMode(2);
        modeSwitched();
    }
}

//==============================================================================
void ModeComponent::modeSwitched()
{
    switch (processor.getMode())
    {
    case 1:
        mode1Label.setColour(Label::textColourId, Colour(33, 255, 59));
        mode2Label.setColour(Label::textColourId, Colours::darkgrey);
        mode1Look.setStage(0);      // stage0 -> on
        mode2Look.setStage(1);      // stage1 -> off
        break;
    case 2:
        mode1Label.setColour(Label::textColourId, Colours::darkgrey);
        mode2Label.setColour(Label::textColourId, Colour(33, 255, 59));
        mode1Look.setStage(1);      // stage0 -> on
        mode2Look.setStage(0);      // stage1 -> off
        break;
    default:
        jassertfalse;
        break;
    }

    repaint();
}