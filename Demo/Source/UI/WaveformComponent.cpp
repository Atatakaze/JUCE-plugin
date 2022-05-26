/*
  ==============================================================================

    WaveformComponent.cpp
    Created: 26 May 2022 3:57:41pm
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
    g.drawRoundedRectangle(getResponseArea().toFloat(), 5.0f, 2.0f);
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
    bounds.removeFromTop(5);
    bounds.removeFromBottom(10);
    bounds.removeFromLeft(10);
    bounds.removeFromRight(10);

    return bounds;
}

//==============================================================================
Rectangle<int> WaveformComponent::getRenderArea()
{
    auto bounds = getResponseArea();
    bounds.removeFromTop(3);
    bounds.removeFromBottom(3);
    bounds.removeFromLeft(2);
    bounds.removeFromRight(2);

    return bounds;
}