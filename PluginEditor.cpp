#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HyperpanAudioProcessorEditor::HyperpanAudioProcessorEditor (HyperpanAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    //mix visual config
    mixSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mixSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::white);
    mixSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::grey);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 75);
    //addAndMakeVisible calls
    addAndMakeVisible(mixSlider);

    mixSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, "mix", mixSlider);

    juce::ignoreUnused (processorRef);

    setSize (400, 400);
}

HyperpanAudioProcessorEditor::~HyperpanAudioProcessorEditor()
{
}


//==============================================================================
void HyperpanAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void HyperpanAudioProcessorEditor::resized()
{
    //component position function
    mixSlider.setBounds (getLocalBounds());
}
