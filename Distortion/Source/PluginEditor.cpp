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
    spectrumComponent(processor), levelMeter(processor), characteristicFunctionComponent(processor), waveformComponent(processor)
{
    // SpectrumComponent
    addAndMakeVisible(spectrumComponent);

    // Waveform
    addAndMakeVisible(waveformComponent);
    waveformComponent.setVisible(false);

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
    inputGainLabel.setText("Gain", dontSendNotification);
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
    outputGainLabel.setText("Level", dontSendNotification);
    outputGainLabel.setFont(Font(16.0f, Font::bold));
    outputGainLabel.setColour(Label::textColourId, Colour(157, 249, 241));
    outputGainLabel.setJustificationType(Justification::centred);

    // monitoring
    addAndMakeVisible(bufferTimeLabel);
    bufferTimeLabel.setText("Buffer Time: ", dontSendNotification);

    // appearence
    panelBg = ImageCache::getFromMemory(BinaryData::panel_png, BinaryData::panel_pngSize);

    // characteristicFunctionComponent
    addAndMakeVisible(characteristicFunctionComponent);

    // switch button (switch between spectrum and waveform)
    addAndMakeVisible(switchBtn);
    switchBtn.addListener(this);
    switchBtn.setLookAndFeel(&buttonLook2);

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

    Graphics g(background);
    g.fillAll(Colour(58, 58, 58));
    auto bounds = getLocalBounds();
    auto panelArea = bounds.removeFromBottom(getHeight() * 0.45);
    bounds.removeFromBottom(2);
    g.setColour(Colours::black);
    g.fillRoundedRectangle(panelArea.toFloat(), 10.f);
    g.fillRoundedRectangle(bounds.toFloat(), 10.f);

    bounds.removeFromTop(5);
    bounds.removeFromBottom(10);
    bounds.removeFromLeft(5);

    // spectrum component
    spectrumComponent.setBounds(bounds);
    // waveform component
    waveformComponent.setBounds(bounds);

    // panel background color
    int side = 7;
    auto panel = Rectangle<int>(panelArea.getX() + side, panelArea.getY() + side, (panelArea.getWidth() * 0.8) - side, panelArea.getHeight() - (2 * side));
    g.drawImageWithin(panelBg, panel.getX(), panel.getY(), panel.getWidth(), panel.getHeight(), RectanglePlacement::stretchToFit);

    // level meter
    auto levelMeterArea = Rectangle<int>(panel.getRight() + 10, panelArea.getY() + 10, panelArea.getWidth() * 0.2 - 20, panelArea.getHeight() - 20);
    levelMeter.setBounds(levelMeterArea);

    // input gain
    auto inputGainArea = Rectangle<int>(panel.getX() + 10, panel.getY() + 25, 80, 100);
    auto inputGainLabelArea = inputGainArea.removeFromBottom(inputGainArea.getHeight() * 0.2);
    inputGainLabel.setBounds(inputGainLabelArea);
    inputGainSlider.setBounds(inputGainArea);

    // output gain
    auto outputGainArea = Rectangle<int>(inputGainArea.getRight() + 10, panel.getY() + 25, 80, 100);
    auto outputGainLabelArea = outputGainArea.removeFromBottom(outputGainArea.getHeight() * 0.2);
    outputGainLabel.setBounds(outputGainLabelArea);
    outputGainSlider.setBounds(outputGainArea);

    // characteristic function window
    auto characteristicFunctionArea = Rectangle<int>(outputGainArea.getRight() + 20, panel.getY() + 10, panel.getRight() - outputGainArea.getRight() - 40, panel.getHeight() * 0.75);
    characteristicFunctionComponent.setBounds(characteristicFunctionArea);

    // switch button
    auto switchBtnArea = Rectangle<int>(characteristicFunctionArea.getRight() - 20, characteristicFunctionArea.getY(), 20, 20);
    switchBtn.setBounds(switchBtnArea);

    // --- RTF monitor --- //
    auto RTFArea = Rectangle<int>(panel.getX() + panel.getWidth() / 2 + 10, panel.getBottom() - 30, 150, 30);
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
        if (buttonLook2.getStage() == 0)
        {
            buttonLook2.setStage(1);
            spectrumComponent.setVisible(false);
            waveformComponent.setVisible(true);
        }
        else
        {
            buttonLook2.setStage(0);
            spectrumComponent.setVisible(true);
            waveformComponent.setVisible(false);
        }
    }
}
