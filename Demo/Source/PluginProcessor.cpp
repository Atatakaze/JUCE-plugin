/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin processor.
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

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
    ),
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

    // characteristic function
    distortion.functionToUse = [](float x) { return tanh(x); };
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
    spec.numChannels = numInputChannels;

    std::cout << " > Filter Size: " << coeff.size() << std::endl;

    // -- Method 1 - Use Convolution -- //
//    coeffBuffer = AudioBuffer<float>(1, (int) coeff.size());     /* Initialize the buffer */
//    coeffBuffer.copyFrom(0, 0, coeff.data(), (int) coeff.size()); /* Vector to buffer */
//    // coeffBuffer.reverse(0, 0, convInSize); /* Call this line when load params from pytorch */
//
//    myfilter.reset(); /* Resets the processing pipeline ready to start a new stream of data */
//    myfilter.loadImpulseResponse( /* Load coeff as IR */
//               std::move (coeffBuffer),
//               spec.sampleRate,
//               dsp::Convolution::Stereo::yes,
//               dsp::Convolution::Trim::no,
//               dsp::Convolution::Normalise::no);
//     myfilter.prepare(spec); /* Must be called before first calling process */


    // -- Method 2 - Use FIR -- //
    //dsp::FIR::Coefficients<float>::Ptr coeffPtr = new dsp::FIR::Coefficients<float>(
    //    coeff.getRawDataPointer(), coeff.size());
    //myfilter.reset(); /* Resets the processing pipeline ready to start a new stream of data */
    //myfilter.state = *coeffPtr;
    //myfilter.prepare(spec); /* Must be called before first calling process */

    // spectrum curve
    leftChannelFifo.prepare(numSamples);
    rightChannelFifo.prepare(numSamples);

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

    // characteristic function
    distortion.prepare(spec);

    // clear bufferTimeRecords
    memset(bufferTimeRecords, 0, sizeof(bufferTimeRecords));
    recordIndex = 0;
}

//==============================================================================
void DemoAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

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
        inputLevelLeft.skip(numSamples);
        inputLevelRight.skip(numSamples);
        {
            const auto value = Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, numSamples));
            if (value < inputLevelLeft.getCurrentValue())
                inputLevelLeft.setTargetValue(value);
            else
                inputLevelLeft.setCurrentAndTargetValue(value);
        }

        {
            const auto value = Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, numSamples));
            if (value < inputLevelRight.getCurrentValue())
                inputLevelRight.setTargetValue(value);
            else
                inputLevelRight.setCurrentAndTargetValue(value);
        }

        ApplyInputGain(buffer);

        dsp::AudioBlock<float> block(buffer);
        //dsp::ProcessContextReplacing<float> context(block);
        //myfilter.process(context);

        // characteristic function
        distortion.process(dsp::ProcessContextReplacing<float>(block));

        ApplyOutputGain(buffer);

        // spectrum Curve
        leftChannelFifo.update(buffer);
        rightChannelFifo.update(buffer);

        // output level meter - smooth
        outputLevelLeft.skip(numSamples);
        outputLevelRight.skip(numSamples);
        {
            const auto value = Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, numSamples));
            if (value < outputLevelLeft.getCurrentValue())
                outputLevelLeft.setTargetValue(value);
            else
                outputLevelLeft.setCurrentAndTargetValue(value);
        }

        {
            const auto value = Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, numSamples));
            if (value < outputLevelRight.getCurrentValue())
                outputLevelRight.setTargetValue(value);
            else
                outputLevelRight.setCurrentAndTargetValue(value);
        }
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
Characteristic function
================================================================================
*/

int DemoAudioProcessor::getDistortionType()
{
    return distortionType;
}

void DemoAudioProcessor::setDistortionType(int type)
{
    distortionType = type;
}

void DemoAudioProcessor::updateDistortionType()
{
    //switch (distortionType)
    //{
    //case 0: // None
    //    distortion.functionToUse = [](float x) { return x; };
    //    break;
    //case 1: // Soft clipping
    //    distortion.functionToUse = [](float x) { return jlimit<float>(-0.8f, 0.8f, x); };
    //    break;
    //case 2: // Hard clipping
    //    distortion.functionToUse = [](float x) { return jlimit<float>(-0.3f, 0.3f, x); };
    //    break;
    //case 3: // Arctangent nonlinearity
    //    distortion.functionToUse = [](float x) { return (2.0f / MathConstants<float>::pi) * atan(x * 10); };
    //    break;
    //case 4: // tanh
    //    distortion.functionToUse = [](float x) { return tanh(x); };
    //    break;
    //default:
    //    jassertfalse;
    //    break;
    //}
    switch (distortionType)
    {
    case 0: // tanh
        distortion.functionToUse = [](float x) { return tanh(x); };
        break;
    case 1: // Soft clipping
        distortion.functionToUse = [](float x) { return jlimit<float>(-0.8f, 0.8f, x); };
        break;
    case 2: // Hard clipping
        distortion.functionToUse = [](float x) { return jlimit<float>(-0.3f, 0.3f, x); };
        break;
    default:
        jassertfalse;
        break;
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
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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