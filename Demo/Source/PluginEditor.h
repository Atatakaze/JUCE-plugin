/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin editor.
  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/SliderLook.h"
#include "UI/ModeComponent.h"
#include "UI/WaveformComponent.h"
#include "UI/LevelMeterComponent.h"
#include "UI/ImportComponent.h"

#include <string>

using namespace juce;

/*
================================================================================
AudioProcessorEditor
================================================================================
*/

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class DemoAudioProcessorEditor : public AudioProcessorEditor, public Timer
{
public:
    DemoAudioProcessorEditor(DemoAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~DemoAudioProcessorEditor() override;

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    DemoAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;

    Image background;

    // envs
    double hostSampleRate = 48000.;

    // -- input gain -- //
    Slider inputGainSlider;
    Label inputGainLabel;

    // -- output gain -- //
    Slider outputGainSlider;
    Label outputGainLabel;

    // -- monitor -- //
    Label bufferTimeLabel;

    // attachments
    std::unique_ptr<SliderAttachment> inputGainAttachment;
    std::unique_ptr<SliderAttachment> outputGainAttachment;

    // -- waveform component -- //
    WaveformComponent waveformComponent;

    // -- level meter -- //
    LevelMeterComponent levelMeter;

    // -- mode component -- //
    ModeComponent modeComponent;

    // -- import components -- //
    Slider importSlider;
    ImportComponent importComponent;

    // appearence
    SliderLook slider1, slider2, slider3;
    Image panelBg, vectorScopeBg;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DemoAudioProcessorEditor)
};