/*
  ==============================================================================

    SliderLook2.cpp
    Created: 8 Jun 2022 6:09:06pm
    Author:  bobo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SliderLook2.h"

/*
=================================================================================
RotarySliderLook
=================================================================================
*/

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


/*
=================================================================================
LinearSliderLook
=================================================================================
*/

LinearSliderLook::LinearSliderLook()
{
}


LinearSliderLook::~LinearSliderLook()
{
}

//==============================================================================
Slider::SliderLayout LinearSliderLook::getSliderLayout(Slider& slider)
{
    Slider::SliderLayout layout;
    layout.sliderBounds = slider.getLocalBounds();
    return layout;
}

//==============================================================================
void LinearSliderLook::drawLinearSlider(
    Graphics& g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle,
    Slider& s)
{
    Rectangle<int> rint(x, y, width, height);
    Rectangle<float> r = rint.toFloat().reduced(0.5f);

    g.setColour(Colour(17, 17, 17));
    g.fillRoundedRectangle(r, 5);

    g.setColour(Colours::black);
    g.drawRoundedRectangle(r, 5, 1);

    float radius = (r.getWidth() - 1) / 2.f;

    auto l = (r.getHeight() - radius) - (r.getY() + radius);
    float originalPos = sliderPos / (float)height;

    sliderPos = l * originalPos; //scaled to our inner size
    
    g.setColour(Colours::lightgrey);
    g.fillEllipse(x + width * 0.5f - radius,
        sliderPos,
        radius * 2.f,
        radius * 2.f);
}