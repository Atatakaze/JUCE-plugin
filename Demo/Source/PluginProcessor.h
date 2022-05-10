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
Spectrum Curve
================================================================================
*/

#include <array>
template<typename T>
struct Fifo
{
    void prepare(int numChannels, int numSamples)
    {
        //static_assert(std::is_same<T, juce::AudioBuffer<float>>,
        //    "prepare(numChannels, numSamples) should only be used when the Fifo is holding juce::AudioBuffer<float>");
        for (auto& buffer : buffers)
        {
            buffer.setSize(numChannels,
                numSamples,
                false,   //clear everything?
                true,    //including the extra space?
                true);   //avoid reallocating if you can?
            buffer.clear();
        }
    }

    void prepare(size_t numElements)
    {
        //static_assert(std::is_same<T, std::vector<float>>,
        //    "prepare(numElements) should only be used when the Fifo is holding std::vector<float>");
        for (auto& buffer : buffers)
        {
            buffer.clear();
            buffer.resize(numElements, 0);
        }
    }

    bool push(const T& t)
    {
        auto write = fifo.write(1);
        if (write.blockSize1 > 0)
        {
            buffers[write.startIndex1] = t;
            return true;
        }

        return false;
    }

    bool pull(T& t)
    {
        auto read = fifo.read(1);
        if (read.blockSize1 > 0)
        {
            t = buffers[read.startIndex1];
            return true;
        }

        return false;
    }

    int getNumAvailableForReading() const
    {
        return fifo.getNumReady();
    }
private:
    static constexpr int Capacity = 30;
    std::array<T, Capacity> buffers;
    AbstractFifo fifo{ Capacity };
};

enum Channel
{
    Right, //effectively 0
    Left //effectively 1
};

template<typename BlockType>
struct SingleChannelSampleFifo
{
    SingleChannelSampleFifo(Channel ch) : channelToUse(ch)
    {
        prepared.set(false);
    }

    void update(const BlockType& buffer)
    {
        jassert(prepared.get());
        jassert(buffer.getNumChannels() > channelToUse);
        auto* channelPtr = buffer.getReadPointer(channelToUse);

        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            pushNextSampleIntoFifo(channelPtr[i]);
        }
    }

    void prepare(int bufferSize)
    {
        prepared.set(false);
        size.set(bufferSize);

        bufferToFill.setSize(1,             //channel
            bufferSize,    //num samples
            false,         //keepExistingContent
            true,          //clear extra space
            true);         //avoid reallocating
        audioBufferFifo.prepare(1, bufferSize);
        fifoIndex = 0;
        prepared.set(true);
    }
    //==============================================================================
    int getNumCompleteBuffersAvailable() const { return audioBufferFifo.getNumAvailableForReading(); }
    bool isPrepared() const { return prepared.get(); }
    int getSize() const { return size.get(); }
    //==============================================================================
    bool getAudioBuffer(BlockType& buf) { return audioBufferFifo.pull(buf); }
private:
    Channel channelToUse;
    int fifoIndex = 0;
    Fifo<BlockType> audioBufferFifo;
    BlockType bufferToFill;
    Atomic<bool> prepared = false;
    Atomic<int> size = 0;

    void pushNextSampleIntoFifo(float sample)
    {
        if (fifoIndex == bufferToFill.getNumSamples())
        {
            auto ok = audioBufferFifo.push(bufferToFill);

            juce::ignoreUnused(ok);

            fifoIndex = 0;
        }

        bufferToFill.setSample(0, fifoIndex, sample);
        ++fifoIndex;
    }
};


/*
================================================================================
AudioProcessor
================================================================================
*/

class DemoAudioProcessor : public AudioProcessor
{
    using firFilter = dsp::ProcessorDuplicator<dsp::FIR::Filter<float>, dsp::FIR::Coefficients<float>>;
public:
    //==============================================================================
    DemoAudioProcessor();
    ~DemoAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    float getInputGain();
    float getOutputGain();
    void ApplyInputGain(AudioBuffer<float>& buffer);
    void ApplyOutputGain(AudioBuffer<float>& buffer);

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
    // spectrum curve
    using BlockType = AudioBuffer<float>;
    SingleChannelSampleFifo<BlockType> leftChannelFifo{ Channel::Left };
    SingleChannelSampleFifo<BlockType> rightChannelFifo{ Channel::Right };

    // waveform viewer
    AudioVisualiserComponent waveViewer;

    //==============================================================================
    // levelmeter
    float getRmsValue(const int, const int) const;

    //==============================================================================
    // characteristic function component
    int getDistortionType();
    void setDistortionType(int type);
    void reset() noexcept { distortion.reset(); };
    void updateDistortionType();

private:
    //==============================================================================
    AudioProcessorValueTreeState parameters;

    // filter
//    dsp::Convolution myfilter;
//    AudioBuffer<float> coeffBuffer;
    firFilter myfilter;

    Array<float> coeff{
            0.000000000000000000,
            -0.000009290744190889,
            -0.000003179945796776,
            0.000082640816061341,
            0.000229223614031907,
            0.000263693640932877,
            -0.000037675811380258,
            -0.000683760580094780,
            -0.001253639060684718,
            -0.001027123632937930,
            0.000442319424678493,
            0.002640563958995431,
            0.003947942580809822,
            0.002523482958339593,
            -0.002055129745209706,
            -0.007561189288896839,
            -0.009677304885113274,
            -0.004738033395627680,
            0.006739390459359920,
            0.018445648506312032,
            0.020795751620257420,
            0.007230813110385699,
            -0.019183700519560042,
            -0.043745289771951856,
            -0.045627091908622811,
            -0.009231438741921514,
            0.064759296816463446,
            0.155809509825300757,
            0.230923660241676043,
            0.259999820916768321,
            0.230923660241676099,
            0.155809509825300757,
            0.064759296816463446,
            -0.009231438741921514,
            -0.045627091908622811,
            -0.043745289771951870,
            -0.019183700519560049,
            0.007230813110385697,
            0.020795751620257423,
            0.018445648506312043,
            0.006739390459359923,
            -0.004738033395627683,
            -0.009677304885113274,
            -0.007561189288896845,
            -0.002055129745209709,
            0.002523482958339594,
            0.003947942580809828,
            0.002640563958995434,
            0.000442319424678492,
            -0.001027123632937931,
            -0.001253639060684717,
            -0.000683760580094779,
            -0.000037675811380258,
            0.000263693640932877,
            0.000229223614031908,
            0.000082640816061341,
            -0.000003179945796776,
            -0.000009290744190889,
            0.000000000000000000,
    };

    // DAW env
    int previousNumSamples = 0;
    int numInputChannels = 2;
    int numOutputChannels = 2;
    double hostSampleRate = 48000.;

    // gains
    float previousInputGain;
    float previousOutputGain;

    // characteristic funciton
    // distortionType = 0 -> Tanh
    // distortionType = 1 -> Arctan
    // distortionType = 2 -> Soft
    // distortionType = 3 -> Hard
    int distortionType = 0;
    dsp::WaveShaper<float> distortion;

    // parameters
    std::atomic<float>* inputGainParameter = nullptr;
    std::atomic<float>* outputGainParameter = nullptr;

    // level meter
    LinearSmoothedValue<float> inputLevelLeft, inputLevelRight, outputLevelLeft, outputLevelRight;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DemoAudioProcessor)
};