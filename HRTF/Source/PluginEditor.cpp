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
    levelMeter(processor), pannerComponent(processor), vectorScopeComponent(processor)
{
    // LevelMeterComponent
    addAndMakeVisible(levelMeter);

    // gain slider
    addAndMakeVisible(gainSlider);
    gainSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    slider1.setSliderColour(Colour(255, 192, 0));
    gainSlider.setLookAndFeel(&slider1);
    gainSliderAttachment.reset(new SliderAttachment(
        valueTreeState, "Gain", gainSlider));

    addAndMakeVisible(gainSliderLabel);
    gainSliderLabel.setText("Gain", dontSendNotification);
    gainSliderLabel.setFont(Font(12.0f, Font::bold));
    gainSliderLabel.setColour(Label::textColourId, Colours::lightgrey);
    gainSliderLabel.setJustificationType(Justification::centred);

    // mix slider
    addAndMakeVisible(mixSlider);
    mixSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    mixSlider.setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    slider2.setSliderColour(Colour(42, 179, 236));
    mixSlider.setLookAndFeel(&slider2);
    mixSliderAttachment.reset(new SliderAttachment(
        valueTreeState, "Mix", mixSlider));

    addAndMakeVisible(mixSliderLabel);
    mixSliderLabel.setText("Wet (%)", dontSendNotification);
    mixSliderLabel.setFont(Font(12.0f, Font::bold));
    mixSliderLabel.setColour(Label::textColourId, Colours::lightgrey);
    mixSliderLabel.setJustificationType(Justification::centred);

    // monitoring
    addAndMakeVisible(bufferTimeLabel);
    bufferTimeLabel.setFont(Font(14.0f));
    bufferTimeLabel.setText("Buffer Time: ", dontSendNotification);

    // vector scope component
    addAndMakeVisible(vectorScopeComponent);

    // panner components
    addAndMakeVisible(pannerComponent);

    setSize(360, 300);
    startTimerHz(10);
}


DemoAudioProcessorEditor::~DemoAudioProcessorEditor()
{
    gainSlider.setLookAndFeel(nullptr);
    mixSlider.setLookAndFeel(nullptr);
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

    int side = 4;
    auto bounds = getLocalBounds();
    bounds.removeFromTop(side);
    bounds.removeFromBottom(side);
    bounds.removeFromLeft(side);
    bounds.removeFromRight(side);

    auto pannerArea = bounds.removeFromTop(200);
    pannerArea.removeFromBottom(side);
    g.setColour(Colours::black);
    g.fillRoundedRectangle(pannerArea.toFloat(), 10.f);

    auto monitoringArea = bounds.removeFromLeft(bounds.getWidth() * 0.6);
    monitoringArea.removeFromRight(side);
    g.fillRoundedRectangle(monitoringArea.toFloat(), 10.f);

    auto mixSliderArea = bounds.removeFromLeft((bounds.getWidth() - side) * 0.5);
    g.fillRoundedRectangle(mixSliderArea.toFloat(), 10.f);

    bounds.removeFromLeft(side);
    g.fillRoundedRectangle(bounds.toFloat(), 10.f);

    // --- level meter --- //
    auto levelMeterArea = monitoringArea.removeFromTop(monitoringArea.getHeight() - 35);
    levelMeterArea.removeFromLeft(15);
    levelMeter.setBounds(levelMeterArea.removeFromLeft(135));

    // --- RTF monitor --- //
    auto RTFArea = Rectangle<int>(monitoringArea.getX() + 15, monitoringArea.getY(), monitoringArea.getWidth() - 4, 35);
    bufferTimeLabel.setBounds(RTFArea);

    // --- mix slider --- //
    auto mixSliderLabelArea = mixSliderArea.removeFromTop(25);
    mixSliderArea.removeFromTop(8);
    mixSliderArea.removeFromBottom(8);
    mixSliderArea.removeFromLeft(8);
    mixSliderArea.removeFromRight(8);
    mixSliderLabel.setBounds(mixSliderLabelArea);
    mixSlider.setBounds(mixSliderArea);

    // --- gain slider --- //
    auto gainSliderLabelArea = bounds.removeFromTop(25);
    bounds.removeFromTop(8);
    bounds.removeFromBottom(8);
    bounds.removeFromLeft(8);
    bounds.removeFromRight(8);
    gainSliderLabel.setBounds(gainSliderLabelArea);
    gainSlider.setBounds(bounds);

    // vector scope
    auto vectorScopeArea = Rectangle<int> (80, 45, 120, 130);
    vectorScopeComponent.setBounds(vectorScopeArea);

    // panner component
    pannerComponent.setBounds(pannerArea);
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
