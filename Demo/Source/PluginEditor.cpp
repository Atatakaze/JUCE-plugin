/*
  ==============================================================================
    This file was auto-generated!
    It contains the basic framework code for a JUCE plugin editor.
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DemoAudioProcessorEditor::DemoAudioProcessorEditor(DemoAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(&p), processor(p), valueTreeState(vts),
    levelMeter(processor), waveformComponent(processor), modeComponent(processor), pannerComponent(processor)
{
    // Waveform
    addAndMakeVisible(waveformComponent);

    // LevelMeterComponent
    addAndMakeVisible(levelMeter);

    // input gain
    addAndMakeVisible(inputGainSlider);
    inputGainSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    inputGainSlider.setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    slider1.setSliderColour(Colour(180, 136, 245), Colour(101, 95, 125));
    inputGainSlider.setLookAndFeel(&slider1);
    inputGainAttachment.reset(new SliderAttachment(
        valueTreeState, "Input Gain", inputGainSlider));

    addAndMakeVisible(inputGainLabel);
    inputGainLabel.setText("Input Gain", dontSendNotification);
    inputGainLabel.setFont(Font(10.0f, Font::bold));
    inputGainLabel.setColour(Label::textColourId, Colour(180, 136, 245));
    inputGainLabel.setJustificationType(Justification::centred);

    // output gain
    addAndMakeVisible(outputGainSlider);
    outputGainSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    outputGainSlider.setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    slider2.setSliderColour(Colour(157, 249, 241), Colour(94, 136, 132));
    outputGainSlider.setLookAndFeel(&slider2);
    outputGainAttachment.reset(new SliderAttachment(
        valueTreeState, "Output Gain", outputGainSlider));

    addAndMakeVisible(outputGainLabel);
    outputGainLabel.setText("Output Gain", dontSendNotification);
    outputGainLabel.setFont(Font(10.0f, Font::bold));
    outputGainLabel.setColour(Label::textColourId, Colour(157, 249, 241));
    outputGainLabel.setJustificationType(Justification::centred);

    // monitoring
    addAndMakeVisible(bufferTimeLabel);
    bufferTimeLabel.setText("Buffer Time: ", dontSendNotification);

    // mode component (switch between regular mode and HRTF mode)
    addAndMakeVisible(modeComponent);

    // import components
    addAndMakeVisible(pannerComponent);

    addAndMakeVisible(importComponent);

    // appearence
    panelBg = ImageCache::getFromMemory(BinaryData::panel_png, BinaryData::panel_pngSize);
    vectorScopeBg = ImageCache::getFromMemory(BinaryData::vectorScope_png, BinaryData::vectorScope_pngSize);

    setSize(610, 360);
    startTimerHz(10);
}


DemoAudioProcessorEditor::~DemoAudioProcessorEditor()
{
    inputGainSlider.setLookAndFeel(nullptr);
    outputGainSlider.setLookAndFeel(nullptr);
}

//==============================================================================
void DemoAudioProcessorEditor::paint(Graphics& g)
{
    g.fillAll(Colours::black);
    g.drawImage(background, getLocalBounds().toFloat());
}

//==============================================================================
void DemoAudioProcessorEditor::resized()
{
    background = Image(Image::PixelFormat::RGB, getWidth(), getHeight(), true);

    Graphics g(background);
    g.fillAll(Colour(58, 58, 58));

    auto bounds = getLocalBounds();
    bounds.removeFromTop(1);
    bounds.removeFromBottom(1);
    bounds.removeFromLeft(1);
    bounds.removeFromRight(1);

    auto panelArea = bounds.removeFromRight(180);
    bounds.removeFromRight(2);
    g.setColour(Colours::black);
    g.fillRoundedRectangle(panelArea.toFloat(), 10.f);
    g.fillRoundedRectangle(bounds.toFloat(), 10.f);

    // panel background color
    auto panel = Rectangle<int>(panelArea.getX() + 2, panelArea.getY() + 20, panelArea.getWidth() - 4, (panelArea.getHeight() * 0.8 ) - 20);
    g.drawImageWithin(panelBg, panel.getX(), panel.getY(), panel.getWidth(), panel.getHeight(), RectanglePlacement::stretchToFit);

    g.setColour(Colours::red);

    // input gain
    auto inputGainArea = Rectangle<int>(panel.getX() + 65, panel.getY() + 10, 50, 60);
    auto inputGainLabelArea = inputGainArea.removeFromBottom(10);
    inputGainSlider.setBounds(inputGainArea);
    inputGainLabel.setBounds(inputGainLabelArea);

    // output gain
    auto outputGainArea = Rectangle<int>(panel.getX() + 65, inputGainArea.getBottom() + 15, 50, 60);
    auto outputGainLabelArea = outputGainArea.removeFromBottom(10);
    outputGainSlider.setBounds(outputGainArea);
    outputGainLabel.setBounds(outputGainLabelArea);

    // import components
    auto importArea = Rectangle<int>(panel.getX() + 2, panel.getY() + panel.getHeight() / 2 + 15, panel.getWidth() - 4, 50);
    
    // panner component
    auto pannerComponentArea = Rectangle<int>(importArea.getX() + importArea.getWidth() / 2 + 20, importArea.getY() + 5, 45, 45);
    pannerComponent.setBounds(pannerComponentArea);

    auto importComponentArea = Rectangle<int>(importArea.getX(), importArea.getY(), importArea.getWidth() / 2, 50);
    importComponent.setBounds(importComponentArea);

    // mode component
    auto modeComponentArea = Rectangle<int>(panel.getX() + 4, panel.getY() + panel.getHeight() / 2 + 70, panel.getWidth() - 8, 35);
    modeComponent.setBounds(modeComponentArea);

    // --- RTF monitor --- //
    auto RTFArea = Rectangle<int>(panel.getX() + 2, panel.getY() + panel.getHeight() / 2 + 105, panel.getWidth() - 4, 20);
    bufferTimeLabel.setBounds(RTFArea);

    // level meter
    panelArea = panelArea.removeFromBottom(panelArea.getHeight() * 0.2);
    auto levelMeterArea = panelArea.removeFromRight(panelArea.getWidth() / 4);
    levelMeter.setBounds(levelMeterArea);

    // waveform component
    auto waveformArea = panelArea;
    waveformComponent.setBounds(waveformArea);

    // vector scope
    auto vectorScopeArea = bounds;
    g.drawImageWithin(vectorScopeBg, bounds.getX() + 20, bounds.getY(), bounds.getWidth() - 40, bounds.getHeight(), RectanglePlacement::fillDestination);
}

//==============================================================================
void DemoAudioProcessorEditor::timerCallback()
{
    // show Real-Time Factor (RTF)
    float ave = 0.0;
    for (auto i = 0; i < 100; ++i)
    {
        ave += processor.bufferTimeRecords[i];
    }
    ave /= 100;

    ave = processor.getTheta();

    bufferTimeLabel.setText("Buffer Time: " + std::to_string(ave), dontSendNotification);
}
