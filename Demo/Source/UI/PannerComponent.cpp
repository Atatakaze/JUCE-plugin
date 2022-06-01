/*
  ==============================================================================

    PannerComponent.cpp
    Created: 1 Jun 2022 2:46:30pm
    Author:  bobo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PannerComponent.h"

//==============================================================================
PannerComponent::PannerComponent(DemoAudioProcessor& p) : processor(p)
{
    addAndMakeVisible(panner);
    panner.setSliderStyle(Slider::RotaryVerticalDrag);
    panner.setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    panner.setRange(0, 360, 15);
    panner.addListener(this);

    sliderLook.setSliderColour(Colour(255, 192, 0), Colour(190, 159, 102));
    panner.setLookAndFeel(&sliderLook);

    updateTheta();
}


PannerComponent::~PannerComponent()
{
    panner.removeListener(this);
    panner.setLookAndFeel(nullptr);
}

//==============================================================================
void PannerComponent::paint (Graphics& g)
{
}

//==============================================================================
void PannerComponent::resized()
{
    panner.setBounds(getLocalBounds());
}

//==============================================================================
void PannerComponent::sliderValueChanged(Slider* slider)
{
    if (slider == &panner)
    {
        processor.setTheta(panner.getValue());
    }
}

//==============================================================================
void PannerComponent::updateTheta()
{
    panner.setValue(processor.getTheta());
}