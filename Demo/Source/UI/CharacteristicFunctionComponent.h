/*
  ==============================================================================

    CharacteristicFunctionComponent.h
    Created: 4 May 2022 10:41:02am
    Author:  bobo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ButtonLook.h"
#include "../PluginProcessor.h"

using namespace juce;

//==============================================================================
/*
*/
class CharacteristicFunctionComponent  : public Component, public Button::Listener
{
public:
    CharacteristicFunctionComponent(DemoAudioProcessor&);
    ~CharacteristicFunctionComponent() override;

    void paint (Graphics&) override;
    void resized() override;
    void buttonClicked(Button*) override;

    Rectangle<int> getWindowArea();
    Rectangle<int>* getComponentArea();
    void setLabelColour();
private:
    DemoAudioProcessor& processor;

    // appearence
    ButtonLook buttonLook;
    TextButton modeButton{ " " };
    
    Label option1, option2, option3, option4;

    Image option[4];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CharacteristicFunctionComponent)
};
