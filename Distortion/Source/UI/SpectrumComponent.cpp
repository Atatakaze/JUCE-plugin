/*
================================================================================
    SpectrumComponent.cpp
    Created: 22 Apr 2022 2:53:31pm
    Author:  bobo
================================================================================
*/

#include "SpectrumComponent.h"

using namespace juce;

/*
================================================================================
PathProducer
================================================================================
*/

void PathProducer::process(Rectangle<float> fftBounds, double sampleRate)
{
    AudioBuffer<float> tempIncomingBuffer;

    while (leftChannelFifo->getNumCompleteBuffersAvailable() > 0)
    {
        if (leftChannelFifo->getAudioBuffer(tempIncomingBuffer))
        {
            auto size = tempIncomingBuffer.getNumSamples();

            FloatVectorOperations::copy(monoBuffer.getWritePointer(0, 0),
                monoBuffer.getReadPointer(0, size),
                monoBuffer.getNumSamples() - size);

            FloatVectorOperations::copy(monoBuffer.getWritePointer(0, monoBuffer.getNumSamples() - size),
                tempIncomingBuffer.getReadPointer(0, 0),
                size);

            leftChannelFFTDataGenerator.produceFFTDataForRendering(monoBuffer, -48.f);
        }
    }

    // const auto fftBounds = getAnalysisArea().toFloat();
    const auto fftSize = leftChannelFFTDataGenerator.getFFTSize();

    const auto binWidth = sampleRate / (double)fftSize;

    while (leftChannelFFTDataGenerator.getNumAvailableFFTDataBlocks() > 0)
    {
        std::vector<float> fftData;
        if (leftChannelFFTDataGenerator.getFFTData(fftData))
        {
            pathProducer.generatePath(fftData, fftBounds, fftSize, binWidth, -48.f);
        }
    }

    while (pathProducer.getNumPathsAvailable())
    {
        pathProducer.getPath(leftChannelFFTPath);
    }
}

//==============================================================================
SpectrumComponent::SpectrumComponent(DemoAudioProcessor& p) : processor(p),
    leftPathProducer(processor.leftChannelFifo),
    rightPathProducer(processor.rightChannelFifo)
{
    startTimerHz(60);
}

SpectrumComponent::~SpectrumComponent()
{
}

//==============================================================================
void SpectrumComponent::paint(Graphics& g)
{
    // draw spectrum
    g.drawImage(background, getLocalBounds().toFloat());

    auto responseArea = getRenderArea();
    auto w = responseArea.getWidth();

    // draw left channel spectrum curve
    auto leftChannelFFTPath = leftPathProducer.getPath();
    leftChannelFFTPath.applyTransform(AffineTransform().translation(responseArea.getX(), responseArea.getY()));
    g.setColour(Colour(159u, 34u, 198u));
    g.strokePath(leftChannelFFTPath, PathStrokeType(1.f));
    g.setColour(Colour(159u, 34u, 198u).withAlpha(0.5f));
    g.fillPath(leftChannelFFTPath);

    // draw right channel spectrum curve
    auto rightChannelFFTPath = rightPathProducer.getPath();
    rightChannelFFTPath.applyTransform(AffineTransform().translation(responseArea.getX(), responseArea.getY()));
    g.setColour(Colour(0u, 172u, 1u));
    g.strokePath(rightChannelFFTPath, PathStrokeType(1.f));
    g.setColour(Colour(0u, 172u, 1u).withAlpha(0.5f));
    g.fillPath(rightChannelFFTPath);

    // draw spectrum responseArea boundaries
    g.setColour(Colour(127u, 148u, 171u));
    g.drawRoundedRectangle(getResponseArea().toFloat(), 1.5f, 1.5f);
}

//==============================================================================
void SpectrumComponent::resized()
{
    // draw spectrum
    background = Image(Image::PixelFormat::RGB, getWidth(), getHeight(), true);
    Graphics g(background);
    g.fillAll(Colours::black);

    auto responseArea = getResponseArea();
    auto left = responseArea.getX();
    auto right = responseArea.getRight();
    auto top = responseArea.getY();
    auto bottom = responseArea.getBottom();
    auto width = responseArea.getWidth();

    // draw frequency grid
    Array<float> freqGrid
    {
        20, 30, 40, 50, 60, 70, 80, 90, 100,
        200, 300, 400, 500, 600, 700, 800, 900, 1000,
        2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000,
        20000
    };

    Array<float> xGrid;
    for (auto f : freqGrid)
    {
        auto normX = mapFromLog10(f, 20.f, 20000.f);
        xGrid.add(left + width * normX);
    };

    g.setColour(Colours::darkgrey);
    for (auto x : xGrid)
    {
        g.drawVerticalLine(x, top, bottom);
    }

    // draw frequency axis
    Array<float> freqs
    {
        20, 50, 100,
        200, 500, 1000,
        2000, 5000, 10000,
        20000
    };

    Array<float> xs;
    for (auto f : freqs)
    {
        auto normX = mapFromLog10(f, 20.f, 20000.f);
        xs.add(left + width * normX);
    };

    g.setColour(Colours::darkgrey);
    for (auto x : xs)
    {
        g.drawVerticalLine(x, top, bottom);
    }

    // draw gain axis
    Array<float> gain
    {
        -48, -36, -24, -12, 0
    };

    for (auto gDb : gain)
    {
        auto y = jmap(gDb, -48.f, 0.f, float(bottom), float(top));
        g.drawHorizontalLine(y, left, right);
    }

    // draw unit of axises
    g.setColour(Colours::lightgrey);
    const int fontHeight = 10;
    g.setFont(fontHeight);

    for (int i = 0; i < freqs.size(); ++i)
    {
        auto f = freqs[i];
        auto x = xs[i];

        bool addK = false;
        String str;
        if (f > 999.f)
        {
            addK = true;
            f /= 1000.f;
        }

        str << f;
        if (addK)
            str << "k";
        str << "Hz";

        auto textWidth = g.getCurrentFont().getStringWidth(str);

        Rectangle<int> r;
        r.setSize(textWidth, fontHeight);
        r.setCentre(x, 0);
        r.setY(getLocalBounds().getY() + 5);

        g.drawFittedText(str, r, Justification::centred, 1);
    }

    for (auto gDb : gain)
    {
        auto y = jmap(gDb, -48.f, 0.f, float(bottom), float(top));

        String str;
        auto textWidth = g.getCurrentFont().getStringWidth(str);
        str << gDb;

        Rectangle<int> r;
        textWidth = g.getCurrentFont().getStringWidth(str);
        r.setX(getResponseArea().getX() - textWidth - 5);
        r.setSize(textWidth, fontHeight);
        r.setCentre(r.getCentreX(), y);
        g.drawFittedText(str, r, Justification::centred, 1);
    }

    // draw the boundaries of response area
    g.setColour(Colour(127u, 148u, 171u));
    g.drawRoundedRectangle(responseArea.toFloat(), 1.5f, 1.5f);
}

//==============================================================================
void SpectrumComponent::timerCallback()
{
    // draw spectrum curve
    auto fftBounds = getRenderArea().toFloat();
    auto sampleRate = processor.getSampleRate();

    leftPathProducer.process(fftBounds, sampleRate);
    rightPathProducer.process(fftBounds, sampleRate);

    repaint();
}

//==============================================================================
Rectangle<int> SpectrumComponent::getResponseArea()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(20);
    bounds.removeFromBottom(2);
    bounds.removeFromLeft(20);
    bounds.removeFromRight(20);

    return bounds;
}

Rectangle<int> SpectrumComponent::getRenderArea()
{
    auto bounds = getResponseArea();
    //bounds.removeFromBottom(10);
    //bounds.removeFromRight(20);

    return bounds;
}