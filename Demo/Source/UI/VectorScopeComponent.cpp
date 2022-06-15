/*
  ==============================================================================

    VectorScopeComponent.cpp
    Created: 9 Jun 2022 4:09:45pm
    Author:  bobo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "VectorScopeComponent.h"

//==============================================================================
VectorScopeComponent::VectorScopeComponent(DemoAudioProcessor& p) : processor(p)
{
    //vectorScopeBg = ImageCache::getFromMemory(BinaryData::vectorScope_png, BinaryData::vectorScope_pngSize);

    for (int i = 0; i < 10; i++)
    {
        point[i][0] = -48.0;
        point[i][1] = -48.0;
    }

    //startTimerHz(30);
}


VectorScopeComponent::~VectorScopeComponent()
{
}

//==============================================================================
void VectorScopeComponent::paint (Graphics& g)
{    
    auto bounds = getLocalBounds();
    //g.drawImageWithin(vectorScopeBg, bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), RectanglePlacement::fillDestination);

    //g.setColour(Colours::white);
    //g.fillEllipse(point[0][0], point[0][1], 2, 2);

    //g.setColour(Colours::white.withBrightness(0.9f));
    //g.fillEllipse(point[1][0], point[1][1], 2, 2);

    //g.setColour(Colours::white.withBrightness(0.8f));
    //g.fillEllipse(point[2][0], point[2][1], 2, 2);

    //g.setColour(Colours::white.withBrightness(0.7f));
    //g.fillEllipse(point[3][0], point[3][1], 2, 2);

    //g.setColour(Colours::white.withBrightness(0.6f));
    //g.fillEllipse(point[4][0], point[4][1], 2, 2);

    //g.setColour(Colours::white.withBrightness(0.5f));
    //g.fillEllipse(point[5][0], point[5][1], 2, 2);

    //g.setColour(Colours::white.withBrightness(0.4f));
    //g.fillEllipse(point[6][0], point[6][1], 2, 2);

    //g.setColour(Colours::white.withBrightness(0.3f));
    //g.fillEllipse(point[7][0], point[7][1], 2, 2);

    //g.setColour(Colours::white.withBrightness(0.2f));
    //g.fillEllipse(point[8][0], point[8][1], 2, 2);

    //g.setColour(Colours::white.withBrightness(0.1f));
    //g.fillEllipse(point[9][0], point[9][1], 2, 2);
}

//==============================================================================
void VectorScopeComponent::resized()
{
}

//==============================================================================
void VectorScopeComponent::timerCallback()
{
    mapping(processor.getRmsValue(1, 1), processor.getRmsValue(0, 1));

    repaint();
}

//==============================================================================
void VectorScopeComponent::mapping(float x, float y)
{
    auto originX = getRenderArea().getX() + (getRenderArea().getWidth() / 2);
    auto originY = getRenderArea().getBottom();

    float x0, y0;
    y0 = jmap(y, -48.f, 0.f, 0.f, float(getRenderArea().getHeight()));
    x0 = jmap(x, -48.f, 0.f, 0.f, float(getRenderArea().getWidth()));

    for (int i = 0; i < 9; i++)
    {
        point[9 - i][0] = point[8 - i][0];
        point[9 - i][1] = point[8 - i][1];
    }

    point[0][0] = originX + x0 * 0.707 - y0 * 0.707;
    point[0][1] = originY - y0 * 0.707 - x0 * 0.707;

    if (point[0][0] > getRenderArea().getRight())
        point[0][0] = getRenderArea().getRight();
    if (point[0][0] < getRenderArea().getX())
        point[0][0] = getRenderArea().getX();

    if (point[0][1] > getRenderArea().getBottom())
        point[0][1] = getRenderArea().getBottom();
    if (point[0][1] < getRenderArea().getY())
        point[0][1] = getRenderArea().getY();
}

//==============================================================================
Rectangle<int> VectorScopeComponent::getRenderArea()
{
    auto bounds = getLocalBounds();
    return bounds;
}