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
    grillGrid = ImageCache::getFromMemory(BinaryData::grillGrid_png, BinaryData::grillGrid_pngSize);

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
    g.fillRoundedRectangle(boundsRrenderArea[0], 1.f);
    g.fillRoundedRectangle(boundsRrenderArea[1], 1.f);
    g.fillRoundedRectangle(boundsRrenderArea[2], 1.f);
    g.fillRoundedRectangle(boundsRrenderArea[3], 1.f);

    // level meter for input left/right channel
    g.setGradientFill(gradient);
    const auto scaleY_IL = jmap(levelIL, -48.f, 0.f, static_cast<float>(boundsRrenderArea[0].getY()), static_cast<float>(boundsRrenderArea[0].getBottom()));
    g.fillRect(boundsRrenderArea[0].removeFromBottom(scaleY_IL));
    const auto scaleY_IR = jmap(levelIR, -48.f, 0.f, static_cast<float>(boundsRrenderArea[1].getY()), static_cast<float>(boundsRrenderArea[1].getBottom()));
    g.fillRect(boundsRrenderArea[1].removeFromBottom(scaleY_IR));
    const auto scaleY_OL = jmap(levelOL, -48.f, 0.f, static_cast<float>(boundsRrenderArea[2].getY()), static_cast<float>(boundsRrenderArea[2].getBottom()));
    g.fillRect(boundsRrenderArea[2].removeFromBottom(scaleY_OL));
    const auto scaleY_OR = jmap(levelOR, -48.f, 0.f, static_cast<float>(boundsRrenderArea[3].getY()), static_cast<float>(boundsRrenderArea[3].getBottom()));
    g.fillRect(boundsRrenderArea[3].removeFromBottom(scaleY_OR));

    // draw level meter grids
    boundsRrenderArea = getRenderArea(bounds);
    g.drawImageWithin(grillGrid,
        boundsRrenderArea[0].getX(),
        boundsRrenderArea[0].getY(),
        boundsRrenderArea[0].getWidth(),
        boundsRrenderArea[0].getHeight(),
        RectanglePlacement::stretchToFit);
    g.drawImageWithin(grillGrid,
        boundsRrenderArea[1].getX(),
        boundsRrenderArea[1].getY(),
        boundsRrenderArea[1].getWidth(),
        boundsRrenderArea[1].getHeight(),
        RectanglePlacement::stretchToFit);
    g.drawImageWithin(grillGrid,
        boundsRrenderArea[2].getX(),
        boundsRrenderArea[2].getY(),
        boundsRrenderArea[2].getWidth(),
        boundsRrenderArea[2].getHeight(),
        RectanglePlacement::stretchToFit);
    g.drawImageWithin(grillGrid,
        boundsRrenderArea[3].getX(),
        boundsRrenderArea[3].getY(),
        boundsRrenderArea[3].getWidth(),
        boundsRrenderArea[3].getHeight(),
        RectanglePlacement::stretchToFit);
}

//==============================================================================
void LevelMeterComponent::resized()
{
    auto bounds = getLocalBounds().toFloat();

    gradient = ColourGradient{
        Colours::red,
        bounds.getTopLeft(),
        Colours::green,
        bounds.getBottomLeft(),
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
    levelIL = inputLeftValue;
    levelIR = inputRightValue;
    levelOL = outputLeftValue;
    levelOR = outputRightValue;
}

//==============================================================================
Rectangle<float>* LevelMeterComponent::getRenderArea(Rectangle<int> bounds)
{
    // render area for each channel level meters
    bounds.removeFromTop(2);
    bounds.removeFromBottom(2);
    bounds.removeFromLeft(3);
    bounds.removeFromRight(4);
    auto inputArea = bounds.removeFromLeft(bounds.getWidth() / 2);
    auto outputArea = bounds;

    // input-left channel level meter      -> reanderArea[0]
    // input-right channel level meter     -> reanderArea[1]
    // output-left channel level meter     -> reanderArea[2]
    // output-right channel level meter    -> reanderArea[3]
    Rectangle<float> renderArea[4];

    renderArea[0] = inputArea.removeFromLeft(8).toFloat();
    renderArea[1] = inputArea.removeFromLeft(8).toFloat();

    renderArea[2] = outputArea.removeFromRight(8).toFloat();
    renderArea[3] = outputArea.removeFromRight(8).toFloat();

    return renderArea;
}