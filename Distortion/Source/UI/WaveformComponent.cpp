/*
  ==============================================================================

    WaveformComponent.cpp
    Created: 10 May 2022 2:31:00pm
    Author:  bobo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformComponent.h"

using namespace juce;

//==============================================================================
WaveformComponent::WaveformComponent(DemoAudioProcessor& p) : processor(p)
{
    addAndMakeVisible(processor.waveViewer);
    processor.waveViewer.setColours(Colours::black, Colours::whitesmoke.withAlpha(0.5f));
}


WaveformComponent::~WaveformComponent()
{
}

//==============================================================================
void WaveformComponent::paint (Graphics& g)
{
    // draw waveform responseArea boundaries
    g.setColour(Colour(127u, 148u, 171u));
    g.drawRoundedRectangle(getResponseArea().toFloat(), 1.5f, 1.5f);
}

//==============================================================================
void WaveformComponent::resized()
{
    processor.waveViewer.setBounds(getRenderArea());
}

//==============================================================================
Rectangle<int> WaveformComponent::getResponseArea()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(20);
    bounds.removeFromBottom(2);
    bounds.removeFromLeft(20);
    bounds.removeFromRight(20);

    return bounds;
}

//==============================================================================
Rectangle<int> WaveformComponent::getRenderArea()
{
    auto bounds = getResponseArea();
    bounds.removeFromTop(2);
    bounds.removeFromBottom(2);
    bounds.removeFromLeft(2);
    bounds.removeFromRight(2);

    return bounds;
}