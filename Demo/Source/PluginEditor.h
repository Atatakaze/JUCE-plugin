/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin editor.
  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/SliderLook.h"
#include "UI/ButtonLook.h"
#include "UI/SpectrumComponent.h"
#include "UI/LevelMeterComponent.h"
#include "UI/CharacteristicFunctionComponent.h"

#include <string>

using namespace juce;

/*
==================================================================================================
AudioProcessorEditor
==================================================================================================
*/

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class DemoAudioProcessorEditor : public AudioProcessorEditor, public Timer, public Button::Listener
{
public:
    DemoAudioProcessorEditor(DemoAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~DemoAudioProcessorEditor() override;

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void buttonClicked(Button* button) override;

private:
    DemoAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;

    Image background;

    // envs
    double hostSampleRate = 48000.;

    // input gain
    Slider inputGainSlider;
    Label inputGainLabel;

    // output gain
    Slider outputGainSlider;
    Label outputGainLabel;

    // -- monitor -- //
    Label bufferTimeLabel;

    // attachments
    std::unique_ptr<SliderAttachment> inputGainAttachment;
    std::unique_ptr<SliderAttachment> outputGainAttachment;

    // spectrum Curve
    SpectrumComponent spectrumComponent;

    // level meter for right and left channel
    LevelMeterComponent levelMeter;

    // appearence
    Image panelBg;
    SliderLook slider1, slider2;
    ButtonLook2 buttonLook2;
    TextButton switchBtn{ " " };

    // characteristic function component
    CharacteristicFunctionComponent characteristicFunctionComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DemoAudioProcessorEditor)
};