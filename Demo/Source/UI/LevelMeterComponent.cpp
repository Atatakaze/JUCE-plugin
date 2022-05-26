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
    // input level
    addAndMakeVisible(inputLevel);
    inputLevel.setText("IN", dontSendNotification);
    inputLevel.setFont(Font(10, Font::bold));
    addAndMakeVisible(inputLevelValue);
    inputLevelValue.setFont(Font(10, Font::bold));
    inputLevelValue.setText(std::to_string(int(levelI)), dontSendNotification);

    // output level
    addAndMakeVisible(outputLevel);
    outputLevel.setText("OUT", dontSendNotification);
    outputLevel.setFont(Font(10, Font::bold));
    addAndMakeVisible(outputLevelValue);
    outputLevelValue.setFont(Font(10, Font::bold));
    outputLevelValue.setText(std::to_string(int(levelO)), dontSendNotification);

    // level meter grids
    bulbGrid = ImageCache::getFromMemory(BinaryData::bulbGrid_png, BinaryData::bulbGrid_pngSize);
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
    g.setColour(Colours::black);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 3.f);
    auto boundsRrenderArea = getRenderArea(bounds);

    // level meter background
    g.setColour(Colours::white.withBrightness(0.4f));
    g.fillRect(boundsRrenderArea[0]);
    g.fillRect(boundsRrenderArea[1]);
    g.fillRoundedRectangle(boundsRrenderArea[2], 1.f);
    g.fillRoundedRectangle(boundsRrenderArea[3], 1.f);

    // level meter for left and right channel
    g.setGradientFill(gradientLR);
    const auto scaleX_L = jmap(levelL, 0.f, -48.f, static_cast<float>(boundsRrenderArea[0].getWidth()), static_cast<float>(boundsRrenderArea[0].getX()));
    g.fillRect(boundsRrenderArea[0].removeFromRight(scaleX_L));
    const auto scaleX_R = jmap(levelR, 0.f, -48.f, static_cast<float>(boundsRrenderArea[1].getWidth()), static_cast<float>(boundsRrenderArea[1].getX()));
    g.fillRect(boundsRrenderArea[1].removeFromRight(scaleX_R));

    // level meter for input and output channel
    g.setGradientFill(gradientIO);
    const auto scaleY_I = jmap(levelI, 0.f, -48.f, static_cast<float>(boundsRrenderArea[2].getBottom()), static_cast<float>(boundsRrenderArea[2].getY()));
    g.fillRect(boundsRrenderArea[2].removeFromBottom(scaleY_I));
    const auto scaleY_O = jmap(levelO, 0.f, -48.f, static_cast<float>(boundsRrenderArea[3].getBottom()), static_cast<float>(boundsRrenderArea[3].getY()));
    g.fillRect(boundsRrenderArea[3].removeFromBottom(scaleY_O));

    // draw level meter grids
    boundsRrenderArea = getRenderArea(bounds);
    g.drawImageWithin(bulbGrid,
        boundsRrenderArea[0].getX(),
        boundsRrenderArea[0].getY(),
        boundsRrenderArea[0].getWidth(),
        boundsRrenderArea[0].getHeight(),
        RectanglePlacement::fillDestination);
    g.drawImageWithin(bulbGrid,
        boundsRrenderArea[1].getX(),
        boundsRrenderArea[1].getY(),
        boundsRrenderArea[1].getWidth(),
        boundsRrenderArea[1].getHeight(),
        RectanglePlacement::fillDestination);
    g.drawImageWithin(grillGrid,
        boundsRrenderArea[2].getX(),
        boundsRrenderArea[2].getY(),
        boundsRrenderArea[2].getWidth(),
        boundsRrenderArea[2].getHeight(),
        RectanglePlacement::fillDestination);
    g.drawImageWithin(grillGrid,
        boundsRrenderArea[3].getX(),
        boundsRrenderArea[3].getY(),
        boundsRrenderArea[3].getWidth(),
        boundsRrenderArea[3].getHeight(),
        RectanglePlacement::fillDestination);

    // level meter label
    g.setFont(10);
    inputLevel.setBounds(bounds.getX() + 5, bounds.getY() + 5, 30, 15);
    outputLevel.setBounds(boundsRrenderArea[2].getRight() + 2, bounds.getY() + 5, 30, 15);
    inputLevelValue.setBounds(boundsRrenderArea[2].getX() - 12, bounds.getBottom() - 32, 30, 10);
    outputLevelValue.setBounds(boundsRrenderArea[3].getX() - 12, bounds.getBottom() - 32, 30, 10);
}

//==============================================================================
void LevelMeterComponent::resized()
{
    auto bounds = getLocalBounds().toFloat();
    auto LRBounds = bounds.removeFromBottom(20);

    gradientIO = ColourGradient{
        Colours::red,
        bounds.getTopLeft(),
        Colours::green,
        bounds.getBottomLeft(),
        false };
    gradientIO.addColour(0.5, Colours::yellow);

    gradientLR = ColourGradient{
        Colours::red,
        LRBounds.getTopLeft(),
        Colours::green,
        LRBounds.getTopRight(),
        false };
    gradientLR.addColour(0.5, Colours::yellow);
}

//==============================================================================
void LevelMeterComponent::timerCallback()
{
    setLevel(
        processor.getRmsValue(0, 0),
        processor.getRmsValue(1, 0),
        processor.getRmsValue(0, 1),
        processor.getRmsValue(1, 1));


    inputLevelValue.setText(std::to_string(int(levelI)), dontSendNotification);
    outputLevelValue.setText(std::to_string(int(levelO)), dontSendNotification);

    repaint();
}

//==============================================================================
void LevelMeterComponent::setLevel(const float inputLeftValue, const float inputRightValue, const float outputLeftValue, const float outputRightValue)
{
    levelL = outputLeftValue;
    levelR = outputRightValue;
    levelI = jmin(inputLeftValue, inputRightValue);
    levelO = jmin(outputLeftValue, outputRightValue);
}

//==============================================================================
Rectangle<float>* LevelMeterComponent::getRenderArea(Rectangle<int> bounds)
{
    int LRBounds = 2;
    int IOBounds = 5;
    // render area for left and right channel level meters
    auto LRArea = bounds.removeFromBottom(20);
    LRArea.removeFromTop(LRBounds);
    LRArea.removeFromBottom(LRBounds);

    // render area for input and output channel level meters
    bounds.removeFromBottom(15);
    bounds.removeFromTop(IOBounds);
    bounds.removeFromLeft(IOBounds);
    bounds.removeFromRight(IOBounds);

    // left channel level meter      -> reanderArea[0]
    // right channel level meter     -> reanderArea[1]
    // input channel level meter     -> reanderArea[2]
    // output channel level meter    -> reanderArea[3]
    Rectangle<float> renderArea[4];

    renderArea[0] = LRArea.removeFromTop((LRArea.getHeight() - (2 * LRBounds)) / 2).toFloat();
    LRArea.removeFromTop(2 * LRBounds);
    renderArea[1] = LRArea.toFloat();

    auto IOArea = bounds.removeFromRight((bounds.getWidth() - IOBounds) / 2 + IOBounds);
    renderArea[2] = bounds.removeFromRight(10).toFloat();
    renderArea[3] = IOArea.removeFromRight(10).toFloat();

    return renderArea;
}