/*
  ==============================================================================

    ImportComponent.h
    Created: 27 May 2022 5:20:16pm
    Author:  bobo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

enum TransportState
{
    Stopped, 
    Starting, 
    Playing, 
    Stopping
};

/*
================================================================================
Waveform Component
================================================================================
*/

class ImportComponent  : public Component
{
public:
    ImportComponent();
    ~ImportComponent() override;

    void paint (Graphics&) override;
    void resized() override;

private:
    TextButton loadBtn{ "Load" }, normalizeBtn{ "Normalize" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImportComponent)
};
