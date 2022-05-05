/*
  ==============================================================================

    SliderLook.h
    Created: 27 Apr 2022 8:31:33pm
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