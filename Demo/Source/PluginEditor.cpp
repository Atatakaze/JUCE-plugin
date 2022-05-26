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
    levelMeter(processor), waveformComponent(processor)
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
    inputGainLabel.setFont(Font(16.0f, Font::bold));
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
    outputGainLabel.setFont(Font(16.0f, Font::bold));
    outputGainLabel.setColour(Label::textColourId, Colour(157, 249, 241));
    outputGainLabel.setJustificationType(Justification::centred);

    // monitoring
    addAndMakeVisible(bufferTimeLabel);
    bufferTimeLabel.setText("Buffer Time: ", dontSendNotification);

    // switch button (switch between spectrum and waveform)
    addAndMakeVisible(switchBtn);
    switchBtn.addListener(this);
    switchBtn.setLookAndFeel(&buttonLook);

    setSize(550, 360);
    startTimerHz(10);
}


DemoAudioProcessorEditor::~DemoAudioProcessorEditor()
{
    inputGainSlider.setLookAndFeel(nullptr);
    outputGainSlider.setLookAndFeel(nullptr);
    switchBtn.removeListener(this);
    switchBtn.setLookAndFeel(nullptr);
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

    // switch button
    auto switchBtnArea = bounds.removeFromTop(50);
    auto renderArea = Rectangle<int>((switchBtnArea.getWidth() / 7) * 2, 10, (switchBtnArea.getWidth() / 7) * 3, 30);
    g.drawRect(renderArea);
    //switchBtn.setBounds(renderArea);

    // waveform component
    auto waveformArea = bounds.removeFromTop(bounds.getHeight() / 2);
    renderArea = waveformArea.removeFromLeft(bounds.getWidth() / 5);
    g.drawRect(waveformArea);
    //waveformComponent.setBounds(waveformArea);

    // import area
    auto importArea = Rectangle<int>(10, renderArea.getY() + 10, renderArea.getWidth() - 20, renderArea.getHeight() - 20);
    g.drawRect(importArea);

    // vector scope
    auto vectorScopeArea = bounds.removeFromRight(bounds.getWidth() / 2);
    renderArea = Rectangle<int>(vectorScopeArea.getX() + 20, vectorScopeArea.getY() + 10, vectorScopeArea.getWidth() - 50, vectorScopeArea.getHeight() - 30);
    g.drawRect(renderArea);

    // input gain
    auto inputGainArea = Rectangle<int>(40, bounds.getY() + 10, 30, 45);
    auto inputGainLabelArea = inputGainArea.removeFromBottom(15);
    //inputGainLabel.setBounds(inputGainLabelArea);
    //inputGainSlider.setBounds(inputGainArea);
    g.drawRect(inputGainLabelArea);
    g.drawRect(inputGainArea);

    // output gain
    auto outputGainArea = Rectangle<int>(40, inputGainLabelArea.getBottom() + 5, 30, 45);
    auto outputGainLabelArea = outputGainArea.removeFromBottom(15);
    //outputGainLabel.setBounds(outputGainLabelArea);
    //outputGainSlider.setBounds(outputGainArea);
    g.drawRect(outputGainLabelArea);
    g.drawRect(outputGainArea);

    // level meter
    auto levelMeterArea = Rectangle<int>(75, bounds.getY() + 10, 170, 95);
    //levelMeter.setBounds(levelMeterArea);
    g.drawRect(levelMeterArea);

    // --- RTF monitor --- //
    auto RTFArea = Rectangle<int>(40, outputGainLabelArea.getBottom() + 10, 150, 30);
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

//==============================================================================
void DemoAudioProcessorEditor::buttonClicked(Button* button)
{
    if (button == &switchBtn)
    {
        if (buttonLook.getStage() == 0)
        {
            buttonLook.setStage(1);
            waveformComponent.setVisible(true);
        }
        else
        {
            buttonLook.setStage(0);
            waveformComponent.setVisible(false);
        }
    }
}
