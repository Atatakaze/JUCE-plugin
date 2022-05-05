/*
  ==============================================================================

    ButtonLook.h
    Created: 4 May 2022 2:54:41pm
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

    void setStage(int n);
    int getStage();

private:
    // stage = 0 -> tanh
    // stage = 1 -> arctan
    // stage = 2 -> soft
    // stage = 3 -> hard
    int stage = 0;
    Image btn[4];
};

/*
=================================================================================
ButtonLook2
=================================================================================
*/

class ButtonLook2 : public LookAndFeel_V4
{
public:
    ButtonLook2();
    ~ButtonLook2();

    void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour,
        bool, bool isButtonDown) override;

    void setStage(int n);
    int getStage();

private:
    // stage = 0 -> spectrum
    // stage = 1 -> waveform
    int stage = 0;
    Image btn[2];
};