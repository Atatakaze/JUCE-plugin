/*
  ==============================================================================

    PannerComponent.h
    Created: 1 Jun 2022 2:46:30pm
    Author:  bobo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SliderLook.h"
#include "../PluginProcessor.h"

using namespace juce;

/*
================================================================================
Panner Component
================================================================================
*/

class PannerComponent  : public Component, public Slider::Listener
{
public:
    PannerComponent(DemoAudioProcessor&);
    ~PannerComponent() override;

    void paint (Graphics&) override;
    void resized() override;

    void sliderValueChanged(Slider* slider) override;
    void updateTheta();
private:
    DemoAudioProcessor& processor;

    SliderLook sliderLook;
    Slider panner;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PannerComponent)
};
