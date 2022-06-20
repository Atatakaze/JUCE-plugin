/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin processor.
  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

using namespace juce;


/*
================================================================================
AudioProcessor
================================================================================
*/

class DemoAudioProcessor : public AudioProcessor
{
public:
    //==============================================================================
    DemoAudioProcessor();
    ~DemoAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; };

    //==============================================================================
    const String getName() const override { return JucePlugin_Name; };
    bool acceptsMidi() const override { return false; };
    bool producesMidi() const override { return false; };
    bool isMidiEffect() const override { return false; };
    double getTailLengthSeconds() const override { return 0.0; };

    //==============================================================================
    int getNumPrograms() override { return 1; };
    int getCurrentProgram() override { return 0; };
    void setCurrentProgram(int index) override { };
    const String getProgramName(int index) override { return {}; };
    void changeProgramName(int index, const String& newName) override {};

    //==============================================================================
    float getGain();
    void ApplyGain(AudioBuffer<float>& buffer);

    //==============================================================================
    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    void releaseResources() override {};

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    //==============================================================================
    // statistics: RTF
    void updateRecord(float newValue);
    float bufferTimeRecords[100] = { 0 };
    float bufferTimeFull = 0.0;
    int recordIndex = 0;

    //==============================================================================
    // levelmeter
    float getRmsValue(const int channel, const int io) const;
    void levelMeterUpdate(
        LinearSmoothedValue<float>& leftChannelLevel, 
        LinearSmoothedValue<float>& rightChannelLevel, 
        AudioBuffer<float>& buffer, 
        int numSamples);

    //==============================================================================
    // vectorscope component
    AudioBuffer<float>& getBuffer() { return wBuffer; };

    //==============================================================================
    // panner component
    int getAzimuth();
    void setAzimuth(int angle);
    
    int getElevation();
    void setElevation(int n);

    int getTheta();
    void setTheta(int angle);

    void updateHRIRFilter();

private:
    //==============================================================================
    AudioProcessorValueTreeState parameters;

    // DAW env
    int previousNumSamples = 0;
    int numInputChannels = 2;
    int numOutputChannels = 2;
    double hostSampleRate = 48000.;

    float previousGain;

    // parameters
    std::atomic<float>* gainSliderParameter = nullptr;
    std::atomic<float>* mixSliderParameter = nullptr;

    // level meter
    LinearSmoothedValue<float> inputLevelLeft, inputLevelRight, outputLevelLeft, outputLevelRight;

    // panner component
    int azimuth = 0;
    int elevation = 0;
    int theta = 72;

    dsp::ProcessSpec spec;
    AudioSampleBuffer monoBuffer;
    dsp::FIR::Filter<float> IR_L, IR_R;

    // vector scope
    AudioBuffer<float> wBuffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DemoAudioProcessor)
};