/*
  ==============================================================================

    SliderLook.h
    Created: 26 May 2022 3:57:41pm
    Author:  bobo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

using namespace juce;

/*
=================================================================================
SliderLook
=================================================================================
*/

class SliderLook : public LookAndFeel_V4
{
public:
    SliderLook();
    ~SliderLook();

    void drawRotarySlider(
        Graphics& g, int x, int y, int width, int height,
        float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
        Slider& slider) override;
    void setSliderColour(Colour colourFront, Colour colourBack);
private:
    Image knob;
    Colour frontColour = Colours::white, backColour = Colours::darkgrey;
};