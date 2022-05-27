/*
  ==============================================================================

    modeComponent.h
    Created: 27 May 2022 10:40:06am
    Author:  bobo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ButtonLook.h"
#include "../PluginProcessor.h"

using namespace juce;

/*
================================================================================
mode component: switch between regular and HRTF
================================================================================
*/

class ModeComponent  : public Component, public Button::Listener
{
public:
    ModeComponent(DemoAudioProcessor&);
    ~ModeComponent() override;

    void paint (Graphics&) override;
    void resized() override;
    void buttonClicked(Button*) override;

    void modeSwitched();
private:
    DemoAudioProcessor& processor;

    ButtonLook mode1Look, mode2Look;

    // option1 -> regular, option2 -> HRTF
    TextButton mode1{ " " }, mode2{ " " };

    Label modeLabel, mode1Label, mode2Label;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModeComponent)
};
