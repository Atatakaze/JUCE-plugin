/*
  ==============================================================================
    
    LevelMeterComponent.h
    Created: 26 May 2022 3:57:41pm
    Author:  bobo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "../PluginProcessor.h"

using namespace juce;

/*
================================================================================
Level Meter
================================================================================
*/

class LevelMeterComponent : public Component, Timer
{
public:
    LevelMeterComponent(DemoAudioProcessor&);
    ~LevelMeterComponent() override;

    void paint(Graphics&) override;
    void resized() override;
    void timerCallback() override;

    void setLevel(const float inputLeftValue, const float inputRightValue, const float outputLeftValue, const float outputRightValue);
    Rectangle<float>* getRenderArea(Rectangle<int>);
private:
    DemoAudioProcessor& processor;

    ColourGradient gradient{};
    Image grillGrid;

    float levelIL = -48.0;
    float levelIR = -48.0;
    float levelOL = -48.0;
    float levelOR = -48.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMeterComponent)
};