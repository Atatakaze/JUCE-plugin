/*
  ==============================================================================

    ImportComponent.cpp
    Created: 27 May 2022 5:20:16pm
    Author:  bobo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ImportComponent.h"

//==============================================================================
ImportComponent::ImportComponent()
{
    addAndMakeVisible(loadBtn);

    addAndMakeVisible(normalizeBtn);
}


ImportComponent::~ImportComponent()
{
}

//==============================================================================
void ImportComponent::paint (juce::Graphics& g)
{
}

//==============================================================================
void ImportComponent::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(5);
    bounds.removeFromBottom(5);
    bounds.removeFromLeft(20);

    loadBtn.setBounds(bounds.removeFromTop(18));

    normalizeBtn.setBounds(bounds.removeFromBottom(18));
}
