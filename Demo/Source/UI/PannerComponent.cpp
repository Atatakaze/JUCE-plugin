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
    addAndMakeVisible(aziSlider);
    aziSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    aziSlider.setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    aziSlider.setRange(-180.0f, 180.0f, 1.0f);
    aziSlider.addListener(this);

    sliderLook.setSliderColour(Colour(255, 192, 0), Colour(190, 159, 102));
    aziSlider.setLookAndFeel(&sliderLook);

    addAndMakeVisible(eleSlider);
    eleSlider.setSliderStyle(Slider::LinearVertical);
    eleSlider.setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    eleSlider.setRange(-40.f, 90.0f, 5.0f);
    eleSlider.addListener(this);

    updateTheta();
}


PannerComponent::~PannerComponent()
{
    aziSlider.removeListener(this);
    aziSlider.setLookAndFeel(nullptr);

    eleSlider.removeListener(this);
}

//==============================================================================
void PannerComponent::paint (Graphics& g)
{
}

//==============================================================================
void PannerComponent::resized()
{
    auto bounds = getLocalBounds();

    eleSlider.setBounds(bounds.removeFromLeft(20));
    aziSlider.setBounds(bounds);
}

//==============================================================================
void PannerComponent::sliderValueChanged(Slider* slider)
{
    if (slider == &aziSlider)
    {
        processor.setAzimuth(aziSlider.getValue());
    }
    if (slider == &eleSlider)
    {
        processor.setElevation(eleSlider.getValue());
    }
}

//==============================================================================
void PannerComponent::updateTheta()
{
    aziSlider.setValue(processor.getAzimuth());
    eleSlider.setValue(processor.getElevation());
}