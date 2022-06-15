/*
  ==============================================================================

    LevelMeterComponent.cpp
    Created: 26 May 2022 3:57:41pm
    Author:  bobo

  ==============================================================================
*/

#include "LevelMeterComponent.h"

using namespace juce;

//==============================================================================
LevelMeterComponent::LevelMeterComponent(DemoAudioProcessor& p) : processor(p)
{
    // level meter grids
    grid = ImageCache::getFromMemory(BinaryData::bulbGrid_png, BinaryData::bulbGrid_pngSize);

    startTimerHz(10);
}


LevelMeterComponent::~LevelMeterComponent()
{
}

//==============================================================================
void LevelMeterComponent::paint(Graphics& g)
{
    auto bounds = getLocalBounds();
    auto boundsRrenderArea = getRenderArea(bounds);

    // level meter background
    g.setColour(Colours::white.withBrightness(0.4f));
    g.fillRect(boundsRrenderArea[0]);
    g.fillRect(boundsRrenderArea[1]);

    // level meter for input left/right channel
    g.setGradientFill(gradient);
    const auto scaleX_I = jmap(inputLevel, -48.f, 0.f, 0.f, boundsRrenderArea[0].getWidth());
    g.fillRect(boundsRrenderArea[0].removeFromLeft(scaleX_I));
    const auto scaleX_O = jmap(outputLevel, -48.f, 0.f, 0.f, boundsRrenderArea[1].getWidth());
    g.fillRect(boundsRrenderArea[1].removeFromLeft(scaleX_O));

    // draw level meter grids
    boundsRrenderArea = getRenderArea(bounds);
    g.drawImageWithin(grid,
        boundsRrenderArea[0].getX(),
        boundsRrenderArea[0].getY(),
        boundsRrenderArea[0].getWidth(),
        boundsRrenderArea[0].getHeight(),
        RectanglePlacement::stretchToFit);
    g.drawImageWithin(grid,
        boundsRrenderArea[1].getX(),
        boundsRrenderArea[1].getY(),
        boundsRrenderArea[1].getWidth(),
        boundsRrenderArea[1].getHeight(),
        RectanglePlacement::stretchToFit);
}

//==============================================================================
void LevelMeterComponent::resized()
{
    auto bounds = getLocalBounds().toFloat();

    gradient = ColourGradient{
        Colours::green,
        bounds.getTopLeft(),
        Colours::red,
        bounds.getTopRight(),
        false };
    gradient.addColour(0.5, Colours::yellow);
}

//==============================================================================
void LevelMeterComponent::timerCallback()
{
    setLevel(
        processor.getRmsValue(0, 0),
        processor.getRmsValue(1, 0),
        processor.getRmsValue(0, 1),
        processor.getRmsValue(1, 1));

    repaint();
}

//==============================================================================
void LevelMeterComponent::setLevel(const float inputLeftValue, const float inputRightValue, const float outputLeftValue, const float outputRightValue)
{
    inputLevel = jmax(inputLeftValue, inputRightValue);
    outputLevel = jmax(outputLeftValue, outputRightValue);
}

//==============================================================================
Rectangle<float>* LevelMeterComponent::getRenderArea(Rectangle<int> bounds)
{
    // render area for each channel level meters
    bounds.removeFromTop(15);
    bounds.removeFromBottom(10);
    auto inputArea = bounds.removeFromTop(15);
    auto outputArea = bounds.removeFromBottom(15);

    // left channel level meter      -> reanderArea[0]
    // right channel level meter     -> reanderArea[1]
    Rectangle<float> renderArea[2];

    renderArea[0] = inputArea.toFloat();
    renderArea[1] = outputArea.toFloat();

    return renderArea;
}