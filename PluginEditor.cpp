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

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 400);
}

HyperpanAudioProcessorEditor::~HyperpanAudioProcessorEditor()
{
}


//==============================================================================
void HyperpanAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);
}

void HyperpanAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    mixSlider.setBounds (getLocalBounds());
}
