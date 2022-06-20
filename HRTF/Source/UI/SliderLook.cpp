/*
  ==============================================================================

    SliderLook.cpp
    Created: 26 May 2022 3:57:41pm
    Author:  bobo

  ==============================================================================
*/

#include "SliderLook.h"

//==============================================================================
SliderLook::SliderLook()
{
}


SliderLook::~SliderLook()
{
}

//==============================================================================
void SliderLook::drawRotarySlider(
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

    g.setColour(Colour(58, 58, 58).withAlpha(isMouseOver ? 1.0f : 0.6f));
    Path outlineArc;
    outlineArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, 0.9f);
    outlineArc.closeSubPath();
    g.fillPath(outlineArc);

    g.setColour(frontColour.withAlpha(isMouseOver ? 1.0f : 0.6f));
    Path filledArc;
    filledArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, angle, 0.9f);
    g.fillPath(filledArc);

    g.setColour(Colours::lightgrey);
    g.fillEllipse(
        centreX - float(jmin(width, height)) * 0.35,
        centreY - float(jmin(width, height)) * 0.35,
        float(jmin(width, height)) * 0.7,
        float(jmin(width, height)) * 0.7);

    // pointer
    Path p;
    g.setColour(Colours::lightgrey);
    p.addRectangle(-1, -25, 4, 10);
    p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
    g.fillPath(p);
}

//==============================================================================
void SliderLook::setSliderColour(Colour colourFront)
{
    frontColour = colourFront;
}