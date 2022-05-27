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
    levelMeter(processor), waveformComponent(processor), modeComponent(processor)
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

    //addAndMakeVisible(inputGainLabel);
    //inputGainLabel.setText("Input Gain", dontSendNotification);
    //inputGainLabel.setFont(Font(10.0f, Font::bold));
    //inputGainLabel.setColour(Label::textColourId, Colour(180, 136, 245));
    //inputGainLabel.setJustificationType(Justification::centred);

    // output gain
    addAndMakeVisible(outputGainSlider);
    outputGainSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    outputGainSlider.setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    slider2.setSliderColour(Colour(157, 249, 241), Colour(94, 136, 132));
    outputGainSlider.setLookAndFeel(&slider2);
    outputGainAttachment.reset(new SliderAttachment(
        valueTreeState, "Output Gain", outputGainSlider));

    //addAndMakeVisible(outputGainLabel);
    //outputGainLabel.setText("Output Gain", dontSendNotification);
    //outputGainLabel.setFont(Font(10.0f, Font::bold));
    //outputGainLabel.setColour(Label::textColourId, Colour(157, 249, 241));
    //outputGainLabel.setJustificationType(Justification::centred);

    // monitoring
    addAndMakeVisible(bufferTimeLabel);
    bufferTimeLabel.setText("Buffer Time: ", dontSendNotification);

    // mode component (switch between regular mode and HRTF mode)
    addAndMakeVisible(modeComponent);

    setSize(550, 360);
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
    auto bounds = getLocalBounds();

    Graphics g(background);
    g.fillAll(Colours::black);

    g.setColour(Colours::red);

    // mode component
    auto modeComponentArea = bounds.removeFromTop(50);
    auto renderArea = Rectangle<int>((modeComponentArea.getWidth() / 8) * 2, 15, (modeComponentArea.getWidth() / 8) * 4, 30);
    modeComponent.setBounds(renderArea);

    // waveform component
    auto waveformArea = bounds.removeFromTop(bounds.getHeight() / 2);
    renderArea = waveformArea.removeFromLeft(bounds.getWidth() / 5);
    waveformComponent.setBounds(waveformArea);

    // import area
    auto importArea = Rectangle<int>(10, renderArea.getY() + 10, renderArea.getWidth() - 20, renderArea.getHeight() - 20);
    g.drawRect(importArea);

    // vector scope
    auto vectorScopeArea = bounds.removeFromRight(bounds.getWidth() / 2);
    renderArea = Rectangle<int>(vectorScopeArea.getX() + 20, vectorScopeArea.getY() + 10, vectorScopeArea.getWidth() - 50, vectorScopeArea.getHeight() - 30);
    g.drawRect(renderArea);

    // input gain
    auto inputGainArea = Rectangle<int>(40, bounds.getY() + 15, 50, 50);
    inputGainSlider.setBounds(inputGainArea);

    // output gain
    auto outputGainArea = Rectangle<int>(40, inputGainArea.getBottom() + 4, 50, 50);
    outputGainSlider.setBounds(outputGainArea);

    // level meter
    auto levelMeterArea = Rectangle<int>(95, bounds.getY() + 10, 170, 104);
    levelMeter.setBounds(levelMeterArea);

    // --- RTF monitor --- //
    auto RTFArea = Rectangle<int>(40, outputGainArea.getBottom() + 2, 150, 30);
    bufferTimeLabel.setBounds(RTFArea);
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
    bufferTimeLabel.setText("Buffer Time: " + std::to_string(ave), dontSendNotification);
}
