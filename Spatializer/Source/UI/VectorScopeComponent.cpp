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
    vectorScopeBg = ImageCache::getFromMemory(BinaryData::vectorScope_png, BinaryData::vectorScope_pngSize);

    startTimerHz(20);
}


VectorScopeComponent::~VectorScopeComponent()
{
}

//==============================================================================
void VectorScopeComponent::paint (Graphics& g)
{   
    // clear last paint
    p.clear();
    pointsX.clear();
    pointsY.clear();

    auto renderArea = getRenderArea();
    auto waveform = processor.getBuffer();
    auto ratio = 100;

    auto bufferY = waveform.getReadPointer(0);
    auto bufferX = waveform.getReadPointer(1);

    for (int sample = 0; sample < waveform.getNumSamples(); sample += ratio)
    {
        pointsX.push_back(bufferX[sample]);
        pointsY.push_back(bufferY[sample]);
    }

    for (int sample = 0; sample < pointsX.size(); ++sample)
    {
        auto pointX = jmap<float>(pointsX[sample], -1.0, 1.0, 0.0, renderArea.getWidth());
        auto pointY = jmap<float>(pointsY[sample], -1.0, 1.0, 0.0, renderArea.getHeight());

        auto point = mapping(pointX, pointY);

        p.addEllipse(point[0], point[1], 3, 3);
    }

    g.setColour(Colours::lightgrey);
    g.fillPath(p);

    auto bounds = getLocalBounds();
    g.drawImageWithin(vectorScopeBg, bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), RectanglePlacement::fillDestination);
}

//==============================================================================
void VectorScopeComponent::resized()
{
}

//==============================================================================
void VectorScopeComponent::timerCallback()
{
    repaint();
}

//==============================================================================
float* VectorScopeComponent::mapping(float x, float y)
{
    auto originX = getRenderArea().getX() + (getRenderArea().getWidth() / 2);
    auto originY = getRenderArea().getBottom();

    float point[2];

    point[0] = originX + x * 0.707 - y * 0.707;
    point[1] = originY - y * 0.707 - x * 0.707;

    if (point[0] > getRenderArea().getRight())
        point[0] = getRenderArea().getRight();
    if (point[0] < getRenderArea().getX())
        point[0] = getRenderArea().getX();

    if (point[1] > getRenderArea().getBottom())
        point[1] = getRenderArea().getBottom();
    if (point[1] < getRenderArea().getY())
        point[1] = getRenderArea().getY();

    return point;
}

//==============================================================================
Rectangle<int> VectorScopeComponent::getRenderArea()
{
    auto bounds = getLocalBounds();

    bounds.removeFromTop(80);
    bounds.removeFromBottom(80);
    bounds.removeFromLeft(90);
    bounds.removeFromRight(90);

    return bounds;
}