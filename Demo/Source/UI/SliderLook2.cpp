/*
  ==============================================================================

    SliderLook2.cpp
    Created: 8 Jun 2022 6:09:06pm
    Author:  bobo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SliderLook2.h"

//==============================================================================
RotarySliderLook::RotarySliderLook()
{
}


RotarySliderLook::~RotarySliderLook()
{
}

//==============================================================================
void RotarySliderLook::drawRotarySlider(
    Graphics& g, int x, int y, int width, int height,
    float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
    Slider& slider)
{
    auto radius = (float)jmin(width / 2, height / 2) - 2.0f;
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto isMouseOver = slider.isMouseOverOrDragging();

    g.setColour(Colour(17, 17, 17));
    Path filledArc;
    filledArc.addPieSegment(rx + 2, ry + 2, rw - 4, rw - 4, rotaryStartAngle, rotaryEndAngle, 0.5f);
    g.fillPath(filledArc);

    g.setColour(Colours::darkgrey);
    g.fillEllipse(
        centreX - float(jmin(width, height)) * 0.3,
        centreY - float(jmin(width, height)) * 0.3,
        float(jmin(width, height)) * 0.6,
        float(jmin(width, height)) * 0.6);

    // pointer
    Path p;

    g.setColour(Colours::white);
    p.addRectangle(-1, -15, 2, 7);
    p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
    g.fillPath(p);
}