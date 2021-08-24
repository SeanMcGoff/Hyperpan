#pragma once

#include "PluginProcessor.h"


//==============================================================================
class HyperpanAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    explicit HyperpanAudioProcessorEditor (HyperpanAudioProcessor&);
    ~HyperpanAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //GUI Controls
    juce::Slider mixSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixSliderAttachment;

    //juce::AudioProcessorValueTreeState::Listener paramUIListener;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HyperpanAudioProcessor& processorRef;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HyperpanAudioProcessorEditor)
};