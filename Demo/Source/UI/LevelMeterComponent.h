/*
  ==============================================================================
    RLLevelMeterComponent.h
    Created: 26 Apr 2022 10:44:25am
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

    ColourGradient gradientLR{}, gradientIO{};
    Image bulbGrid, grillGrid;

    Label inputLevelValue, outputLevelValue, inputLevel, outputLevel;

    float levelL = -48.0;
    float levelR = -48.0;
    float levelI = -48.0;
    float levelO = -48.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMeterComponent)
};