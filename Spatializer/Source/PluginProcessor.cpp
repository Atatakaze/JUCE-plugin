/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin processor.
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "UI/HRIR3D.h"

using namespace juce;

//==============================================================================
DemoAudioProcessor::DemoAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
    #if ! JucePlugin_IsMidiEffect
        #if ! JucePlugin_IsSynth
            .withInput("Input", AudioChannelSet::stereo(), true)
        #endif
        .withOutput("Output", AudioChannelSet::stereo(), true)
    #endif
    ), waveViewer(2),
#endif
    parameters(*this, nullptr, Identifier("APVT"),
        {
            std::make_unique<AudioParameterFloat>(
                    "Input Gain",      // parameter ID
                    "Input Gain",      // parameter name
                    NormalisableRange<float>(
                        -30.0f,
                        30.0f,
                        0.01f),         // min, max,step
                     0.0f),
            std::make_unique<AudioParameterFloat>(
                    "Output Gain",      // parameter ID
                    "Output Gain",      // parameter name
                    NormalisableRange<float>(
                        -30.0f,
                        30.0f,
                        0.01f),         // min, max,step
                     0.0f),        // default value
        })

{
    // knob values
    inputGainParameter = parameters.getRawParameterValue("Input Gain");
    outputGainParameter = parameters.getRawParameterValue("Output Gain");

    // gains
    previousOutputGain = *outputGainParameter;
    previousInputGain = *inputGainParameter;

    // waveform
    waveViewer.setRepaintRate(30);
    waveViewer.setBufferSize(1024);
}


DemoAudioProcessor::~DemoAudioProcessor()
{
}

//==============================================================================
void DemoAudioProcessor::prepareToPlay(double sampleRate, int numSamples)
{
    // set host sampling env
    numInputChannels = getTotalNumInputChannels();
    numOutputChannels = getTotalNumOutputChannels();
    hostSampleRate = sampleRate;
    previousNumSamples = numSamples;
    bufferTimeFull = numSamples / sampleRate;

    std::cout << "========================" << std::endl;
    std::cout << "            sampleRate: " << sampleRate << std::endl;
    std::cout << "       samplesPerBlock: " << numSamples << std::endl;
    std::cout << " totalNumInputChannels: " << numInputChannels << std::endl;
    std::cout << "========================" << std::endl;

    // spec for filter
    dsp::ProcessSpec spec;
    spec.sampleRate = hostSampleRate;
    spec.maximumBlockSize = numSamples;
    spec.numChannels = 1;

    //std::cout << " > Filter Size: " << coeff.size() << std::endl;

    //// -- Using Convolution  for panning -- //
    //coeffBuffer = AudioBuffer<float>(1, (int) coeff.size());     /* Initialize the buffer */
    //coeffBuffer.copyFrom(0, 0, coeff.data(), (int) coeff.size()); /* Vector to buffer */
    //// coeffBuffer.reverse(0, 0, convInSize); /* Call this line when load params from pytorch */

    //panner.reset(); /* Resets the processing pipeline ready to start a new stream of data */
    //panner.loadImpulseResponse( /* Load coeff as IR */
    //           std::move (coeffBuffer),
    //           spec.sampleRate,
    //           dsp::Convolution::Stereo::yes,
    //           dsp::Convolution::Trim::no,
    //           dsp::Convolution::Normalise::no);
    // panner.prepare(spec); /* Must be called before first calling process */
    
    // panner component
    IR_L.prepare(spec);
    IR_R.prepare(spec);
    updateHRIRFilter();
    monoBuffer.setSize(1, numSamples);
    IR_L.reset();
    IR_R.reset();

    // waveform viewer
    waveViewer.clear();

    // level meter
    inputLevelLeft.reset(sampleRate, 0.5);
    inputLevelRight.reset(sampleRate, 0.5);
    outputLevelLeft.reset(sampleRate, 0.5);
    outputLevelRight.reset(sampleRate, 0.5);
    inputLevelLeft.setCurrentAndTargetValue(-100.f);
    inputLevelRight.setCurrentAndTargetValue(-100.f);
    outputLevelLeft.setCurrentAndTargetValue(-100.f);
    outputLevelRight.setCurrentAndTargetValue(-100.f);

    // -- clear caches -- //
    // clear gain cache
    previousOutputGain = *outputGainParameter + 0.0;
    previousInputGain = *inputGainParameter + 0.0;

    // clear bufferTimeRecords
    memset(bufferTimeRecords, 0, sizeof(bufferTimeRecords));
    recordIndex = 0;
}

//==============================================================================
void DemoAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    int numSamples = buffer.getNumSamples();
    auto begin = std::chrono::high_resolution_clock::now();

    // Main DSP procssing block
    if (numSamples > 0)
    {

        // We need reseting here beacuse DAWs usually sent testing signal after preparetoplay()
        if (previousNumSamples != numSamples)
        {
            prepareToPlay(hostSampleRate, numSamples);
        }

        // ===== Start Coding ===== //

        // intput level meter - smooth
        levelMeterUpdate(inputLevelLeft, inputLevelRight, buffer, numSamples);

        ApplyInputGain(buffer);

        // panner component
        if (mode == 2)
        {
            //dsp::AudioBlock<float> block(buffer);
            //dsp::ProcessContextReplacing<float> context(block);
            //panner.process(context);
            auto bufferL = buffer.getWritePointer(0);
            auto bufferR = buffer.getWritePointer(1);

            const auto BLength = buffer.getNumSamples();
            if (totalNumInputChannels == 2)
            {
                buffer.addFrom(0, 0, buffer, 1, 0, BLength);
                buffer.copyFrom(1, 0, buffer, 0, 0, BLength);
                buffer.applyGain(0.5f);
            }

            monoBuffer.copyFrom(0, 0, buffer, 0, 0, BLength);

            updateHRIRFilter();
            dsp::AudioBlock<float> blockL = dsp::AudioBlock<float>(&bufferL, 1, BLength);
            dsp::AudioBlock<float> blockR = dsp::AudioBlock<float>(&bufferR, 1, BLength);
            IR_L.process(dsp::ProcessContextReplacing<float>(blockL));
            IR_R.process(dsp::ProcessContextReplacing<float>(blockR));
        }

        ApplyOutputGain(buffer);

        // waveform viewer
        waveViewer.pushBuffer(buffer);

        wBuffer = buffer;

        // outtput level meter - smooth
        levelMeterUpdate(outputLevelLeft, outputLevelRight, buffer, numSamples);
        // ===== End Coding ===== //
    }
    else
    {
        return; //avoid exception when DAW sents zero length audio buffer
    }

    // update RTF
    auto end = std::chrono::high_resolution_clock::now();
    float duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() / 1e9;
    float bufferTime = duration / bufferTimeFull;
    updateRecord(bufferTime);
}


/*
================================================================================
LevelMeterComponent
================================================================================
*/

float DemoAudioProcessor::getRmsValue(const int channel, const int io) const
{
    jassert(channel == 0 || channel == 1);
    if (channel == 0 && io == 0)        // input level of left channel
        return inputLevelLeft.getCurrentValue();
    if (channel == 1 && io == 0)        // input level of right channel
        return inputLevelRight.getCurrentValue();
    if (channel == 0 && io == 1)        // output level of left channel
        return outputLevelLeft.getCurrentValue();
    if (channel == 1 && io == 1)        // output level of right channel
        return outputLevelRight.getCurrentValue();
    return 0.f;
}

void DemoAudioProcessor::levelMeterUpdate(LinearSmoothedValue<float>& levelMeterLeft, LinearSmoothedValue<float>& levelMeterRight, AudioBuffer<float>& buffer, int numSamples)
{
    levelMeterLeft.skip(numSamples);
    levelMeterRight.skip(numSamples);
    {
        const auto value = Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, numSamples));
        if (value < levelMeterLeft.getCurrentValue())
            levelMeterLeft.setTargetValue(value);
        else
            levelMeterLeft.setCurrentAndTargetValue(value);
    }

    {
        const auto value = Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, numSamples));
        if (value < levelMeterRight.getCurrentValue())
            levelMeterRight.setTargetValue(value);
        else
            levelMeterRight.setCurrentAndTargetValue(value);
    }
}


/*
================================================================================
mode component: regular or HRTF
================================================================================
*/

int DemoAudioProcessor::getMode()
{
    return mode;
}

void DemoAudioProcessor::setMode(int type)
{
    mode = type;
}


/*
================================================================================
panner component
================================================================================
*/

int DemoAudioProcessor::getAzimuth()
{
    return azimuth;
}

void DemoAudioProcessor::setAzimuth(int angle)
{
    azimuth = angle;
}

int DemoAudioProcessor::getElevation()
{
    return elevation;
}

void DemoAudioProcessor::setElevation(int n)
{
    elevation = n;
}

int DemoAudioProcessor::getTheta()
{
    return theta;
}

void DemoAudioProcessor::setTheta(int angle)
{
    theta = angle;
}

void DemoAudioProcessor::updateHRIRFilter()
{
    *(IR_L.coefficients) = dsp::FIR::Coefficients<float>(hrir_l[theta], 256);
    *(IR_R.coefficients) = dsp::FIR::Coefficients<float>(hrir_r[theta], 256);
}


/*
================================================================================
State Block
================================================================================
*/

float DemoAudioProcessor::getInputGain()
{
    return powf(10, (*inputGainParameter) / 20.0);
}

float DemoAudioProcessor::getOutputGain()
{
    return powf(10, (*outputGainParameter) / 20.0);
}

void DemoAudioProcessor::ApplyInputGain(AudioBuffer<float>& buffer)
{
    auto currentInputGain = getInputGain();
    if (currentInputGain == previousInputGain)
    {
        buffer.applyGain(currentInputGain);
    }
    else
    {
        auto numSamples = buffer.getNumSamples();
        buffer.applyGainRamp(0, numSamples, previousInputGain, currentInputGain);
        previousInputGain = currentInputGain;
    }
}

void DemoAudioProcessor::ApplyOutputGain(AudioBuffer<float>& buffer)
{
    auto currentOutputGain = getOutputGain();
    
    if(mode == 2)
        currentOutputGain += 5;

    if (currentOutputGain == previousOutputGain)
    {
        buffer.applyGain(currentOutputGain);
    }
    else
    {
        auto numSamples = buffer.getNumSamples();
        buffer.applyGainRamp(0, numSamples, previousOutputGain, currentOutputGain);
        previousOutputGain = currentOutputGain;
    }
}


/*
================================================================================
State Block
================================================================================
*/

void DemoAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DemoAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}


/*
================================================================================
Misc
================================================================================
*/

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DemoAudioProcessor();
}

AudioProcessorEditor* DemoAudioProcessor::createEditor()
{
    return new DemoAudioProcessorEditor(*this, parameters);
}

bool DemoAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{

    // Ensure # input channel == #output channel
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    return true;
}


/*
================================================================================
Stastics
================================================================================
*/

void DemoAudioProcessor::updateRecord(float newValue)
{
    bufferTimeRecords[recordIndex] = newValue;
    ++recordIndex;
    recordIndex %= 100;
}