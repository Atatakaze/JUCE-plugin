/*
  ==============================================================================

    SliderLook2.h
    Created: 8 Jun 2022 6:09:06pm
    Author:  bobo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

using namespace juce;

/*
=================================================================================
RotarySliderLook
=================================================================================
*/

class RotarySliderLook : public LookAndFeel_V4
{
public:
    RotarySliderLook();
    ~RotarySliderLook();

    void drawRotarySlider(
        Graphics& g, int x, int y, int width, int height,
        float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
        Slider& slider) override;
};
