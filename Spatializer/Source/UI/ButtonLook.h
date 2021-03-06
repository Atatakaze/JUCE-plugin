/*
  ==============================================================================

    ButtonLook.h
    Created: 26 May 2022 3:57:41pm
    Author:  bobo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "../PluginProcessor.h"

using namespace juce;

/*
=================================================================================
ButtonLook
=================================================================================
*/


class ButtonLook : public LookAndFeel_V4
{
public:
    ButtonLook();
    ~ButtonLook();

    void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour,
        bool, bool isButtonDown) override;

    void setStage(int state);
    int getStage();

private:
    // stage = 0 -> on
    // stage = 1 -> off
    int stage = 0;
    Image btn[2];
};