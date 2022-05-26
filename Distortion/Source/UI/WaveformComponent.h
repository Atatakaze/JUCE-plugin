/*
  ==============================================================================

    WaveformComponent.h
    Created: 10 May 2022 2:31:00pm
    Author:  bobo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"

using namespace juce;

/*
================================================================================
Waveform Component
================================================================================
*/

class WaveformComponent  : public Component
{
public:
    WaveformComponent(DemoAudioProcessor&);
    ~WaveformComponent() override;

    void paint (Graphics&) override;
    void resized() override;

    Rectangle<int> getResponseArea();
    Rectangle<int> getRenderArea();
private:
    DemoAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformComponent)
};
