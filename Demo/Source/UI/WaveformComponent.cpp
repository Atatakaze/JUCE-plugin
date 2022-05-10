///*
//  ==============================================================================
//
//    WaveformComponent.cpp
//    Created: 10 May 2022 2:31:00pm
//    Author:  bobo
//
//  ==============================================================================
//*/
//
//#include <JuceHeader.h>
//#include "WaveformComponent.h"
//
//using namespace juce;
//
////==============================================================================
//WaveformComponent::WaveformComponent() : thumbnailCache(5), thumbnail(512, formatManager, thumbnailCache)
//{
//    thumbnail.addChangeListener(this);
//
//}
//
//WaveformComponent::~WaveformComponent()
//{
//}
//
//void WaveformComponent::paint (juce::Graphics& g)
//{
//    /* This demo code just fills the component's background and
//       draws some placeholder text to get you started.
//
//       You should replace everything in this method with your own
//       drawing code..
//    */
//
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
//
//    g.setColour (juce::Colours::grey);
//    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
//
//    g.setColour (juce::Colours::white);
//    g.setFont (14.0f);
//    g.drawText ("WaveformComponent", getLocalBounds(),
//                juce::Justification::centred, true);   // draw some placeholder text
//}
//
//void WaveformComponent::resized()
//{
//    // This method is where you should set the bounds of any child
//    // components that your component contains..
//
//}
