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


/*
=================================================================================
LinearSliderLook
=================================================================================
*/

class LinearSliderLook : public LookAndFeel_V4
{
public:
    LinearSliderLook();
    ~LinearSliderLook();

    Slider::SliderLayout getSliderLayout(Slider& slider) override;

    void drawLinearSlider(
        Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle,
        Slider& s) override;
};