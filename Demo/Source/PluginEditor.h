/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin editor.
  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/SliderLook.h"
#include "UI/LevelMeterComponent.h"
#include "UI/PannerComponent.h"
#include "UI/VectorScopeComponent.h"

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

    // -- gain -- //
    Slider gainSlider;
    Label gainSliderLabel;

    // -- mix -- //
    Slider mixSlider;
    Label mixSliderLabel;

    // -- monitor -- //
    Label bufferTimeLabel;

    // attachments
    std::unique_ptr<SliderAttachment> gainSliderAttachment;
    std::unique_ptr<SliderAttachment> mixSliderAttachment;

    // -- level meter -- //
    LevelMeterComponent levelMeter;

    // -- vector scope -- //
    VectorScopeComponent vectorScopeComponent;

    // -- import components -- //
    PannerComponent pannerComponent;

    // appearence
    SliderLook slider1, slider2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DemoAudioProcessorEditor)
};