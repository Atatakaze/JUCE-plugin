/*
  ==============================================================================

    CharacteristicFunctionComponent.cpp
    Created: 4 May 2022 10:41:02am
    Author:  bobo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CharacteristicFunctionComponent.h"

//==============================================================================
CharacteristicFunctionComponent::CharacteristicFunctionComponent(DemoAudioProcessor& p) : processor(p)
{
    addAndMakeVisible(modeButton);
    modeButton.addListener(this);
    modeButton.setLookAndFeel(&buttonLook);

    addAndMakeVisible(option1);
    option1.setText("Tanh", dontSendNotification);
    option1.setFont(Font(14.0f, Font::bold));
    option1.setJustificationType(Justification::centred);

    addAndMakeVisible(option2);
    option2.setText("Arctan", dontSendNotification);
    option2.setFont(Font(14.0f, Font::bold));
    option2.setJustificationType(Justification::centred);

    addAndMakeVisible(option3);
    option3.setText("Soft", dontSendNotification);
    option3.setFont(Font(14.0f, Font::bold));
    option3.setJustificationType(Justification::centred);

    addAndMakeVisible(option4);
    option4.setText("Hard", dontSendNotification);
    option4.setFont(Font(14.0f, Font::bold));
    option4.setJustificationType(Justification::centred);

    // read initial value from plugin processor
    setLabelColour();
}


CharacteristicFunctionComponent::~CharacteristicFunctionComponent()
{
    modeButton.removeListener(this);
    modeButton.setLookAndFeel(nullptr);
}

//==============================================================================
void CharacteristicFunctionComponent::paint (Graphics& g)
{
    g.setColour(Colours::white);
    g.fillRect(getWindowArea());
}

//==============================================================================
void CharacteristicFunctionComponent::resized()
{
    auto area = getComponentArea();

    modeButton.setBounds(area[0]);
    option1.setBounds(area[1]);
    option2.setBounds(area[2]);
    option3.setBounds(area[3]);
    option4.setBounds(area[4]);
}

//==============================================================================
void CharacteristicFunctionComponent::buttonClicked(Button* button)
{
    if (button == &modeButton)
    {
        if (processor.getDistortionType() < 3) processor.setDistortionType(processor.getDistortionType() + 1);
        else processor.setDistortionType(0);
        
        processor.updateDistortionType();
        setLabelColour();
    }   
}

//==============================================================================
Rectangle<int> CharacteristicFunctionComponent::getWindowArea()
{
    auto bounds = getLocalBounds();

    bounds.removeFromTop(30);
    bounds.removeFromBottom(10);
    bounds.removeFromRight(30);
    bounds.removeFromLeft(30);

    return bounds;
}

//==============================================================================
Rectangle<int>* CharacteristicFunctionComponent::getComponentArea()
{
    Rectangle<int> area[5];
    auto bounds = getLocalBounds().removeFromTop(20);;

    area[0] = bounds.removeFromLeft(30);                        // area[0] -> modeButton area

    bounds.removeFromRight(20);
    bounds.removeFromTop(3);
    bounds.removeFromBottom(3);
    area[4] = bounds.removeFromRight(bounds.getWidth() / 4);    // area[4] -> option4 area
    area[3] = bounds.removeFromRight(bounds.getWidth() / 3);    // area[3] -> option3 area
    area[2] = bounds.removeFromRight(bounds.getWidth() / 2);    // area[2] -> option2 area
    area[1] = bounds;                                           // area[1] -> option1 area

    return area;
}

//==============================================================================
void CharacteristicFunctionComponent::setLabelColour()
{
    switch (processor.getDistortionType())
    {
    case 0:
        option1.setColour(Label::textColourId, Colour(33, 255, 59));
        option2.setColour(Label::textColourId, Colours::darkgrey);
        option3.setColour(Label::textColourId, Colours::darkgrey);
        option4.setColour(Label::textColourId, Colours::darkgrey);
        buttonLook.setStage(0);
        break;
    case 1:
        option1.setColour(Label::textColourId, Colours::darkgrey);
        option2.setColour(Label::textColourId, Colour(251, 202, 63));
        option3.setColour(Label::textColourId, Colours::darkgrey);
        option4.setColour(Label::textColourId, Colours::darkgrey);
        buttonLook.setStage(1);
        break;
    case 2:
        option1.setColour(Label::textColourId, Colours::darkgrey);
        option2.setColour(Label::textColourId, Colours::darkgrey);
        option3.setColour(Label::textColourId, Colour(133, 214, 255));
        option4.setColour(Label::textColourId, Colours::darkgrey);
        buttonLook.setStage(2);
        break;
    case 3:
        option1.setColour(Label::textColourId, Colours::darkgrey);
        option2.setColour(Label::textColourId, Colours::darkgrey);
        option3.setColour(Label::textColourId, Colours::darkgrey);
        option4.setColour(Label::textColourId, Colour(247, 163, 241));
        buttonLook.setStage(3);
        break;
    default:
        jassertfalse;
        break;
    }
}