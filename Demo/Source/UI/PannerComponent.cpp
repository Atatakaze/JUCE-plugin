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
    aziSlider.setRotaryParameters(0.0, (2.0 * float_Pi), false); // make it 187 levels
    aziSlider.setRange(0, 360, 15);
    aziSlider.addListener(this);

    aziSlider.setLookAndFeel(&sliderLook);

    addAndMakeVisible(eleSlider);
    eleSlider.setSliderStyle(Slider::LinearVertical);
    eleSlider.setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    eleSlider.setRange(-45, 90, 15);
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
        azimuth = aziSlider.getValue();
        elevation = processor.getElevation();
        processor.setAzimuth(azimuth);

        if (0 <= elevation && elevation <= 144) //if -45<= elevation <= 45
            processor.setTheta(azimuth / 15 + elevation); // the increment of azimuth is 15 degrees
        else if (168 == elevation) // if elevation = 60
        {
            processor.setTheta(azimuth / 30 + elevation);
        }
        else if (elevation == 180) // if elevation = 75
        {
            processor.setTheta(azimuth / 60 + elevation);
        }
        else //elevation == 186
            processor.setTheta(186);
    }
    if (slider == &eleSlider)
    {
        elevation = eleSlider.getValue();

        if (elevation == -45)
        {
            elevation = 0;
            aziSlider.setRange(0.0, 360, 15);
            processor.setTheta(elevation);
            processor.setElevation(elevation);
        }
        else if (elevation == -30)
        {
            elevation = 24;
            aziSlider.setRange(0.0, 360, 15);
            processor.setTheta(elevation);
            processor.setElevation(elevation);
        }
        else if (elevation == -15)
        {
            elevation = 48;
            aziSlider.setRange(0.0, 360, 15);
            processor.setTheta(elevation);
            processor.setElevation(elevation);
        }
        else if (elevation == 0)
        {
            elevation = 72;
            aziSlider.setRange(0.0, 360, 15);
            processor.setTheta(elevation);
            processor.setElevation(elevation);
        }
        else if (elevation == 15)
        {
            elevation = 96;
            aziSlider.setRange(0.0, 360, 15);
            processor.setTheta(elevation);
            processor.setElevation(elevation);
        }
        else if (elevation == 30)
        {
            elevation = 120;
            aziSlider.setRange(0.0, 360, 15);
            processor.setTheta(elevation);
            processor.setElevation(elevation);
        }
        else if (elevation == 45)
        {
            elevation = 144;
            aziSlider.setRange(0.0, 360, 15);
            processor.setTheta(elevation);
            processor.setElevation(elevation);
        }
        else if (elevation == 60)
        {
            elevation = 168;
            aziSlider.setValue(0.0);
            aziSlider.setRange(0.0, 360, 30);
            processor.setTheta(elevation);
            processor.setElevation(elevation);
        }
        else if (elevation == 75)
        {
            elevation = 180;
            aziSlider.setValue(0.0);
            aziSlider.setRange(0.0, 360, 60);
            processor.setTheta(elevation);
            processor.setElevation(elevation);
        }
        else
        {
            elevation = 186;
            aziSlider.setValue(0.0);
            processor.setTheta(elevation);
            processor.setElevation(elevation);
        }
    }
}

//==============================================================================
void PannerComponent::updateTheta()
{
    azimuth = processor.getAzimuth();
    aziSlider.setValue(azimuth);

    elevation = processor.getElevation();
    eleSlider.setValue(elevation);
}