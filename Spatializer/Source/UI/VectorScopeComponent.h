/*
  ==============================================================================

    VectorScopeComponent.h
    Created: 9 Jun 2022 4:09:45pm
    Author:  bobo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "../PluginProcessor.h"

using namespace juce;

/*
================================================================================
Vector Scope
================================================================================
*/

class VectorScopeComponent  : public Component, public Timer
{
public:
    VectorScopeComponent(DemoAudioProcessor&);
    ~VectorScopeComponent() override;

    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;

    // rotation matrix
    float* mapping(float x, float y);
    Rectangle<int> getRenderArea();
private:
    DemoAudioProcessor& processor;

    Image vectorScopeBg;

    Path p;
    std::vector<float> pointsX, pointsY;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VectorScopeComponent)
};
