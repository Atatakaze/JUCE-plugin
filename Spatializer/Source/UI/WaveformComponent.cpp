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
    g.setColour(Colours::darkgrey);
    g.fillRect(getResponseArea());
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
    bounds.removeFromTop(2);
    bounds.removeFromBottom(4);
    bounds.removeFromLeft(5);
    bounds.removeFromRight(2);

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