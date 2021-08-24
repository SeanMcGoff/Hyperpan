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
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    mixSlider.setTextValueSuffix("%");

    panLawBox.setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colours::transparentBlack);
    panLawBox.addItemList(processorRef.apvts.getParameter("panlaw")->getAllValueStrings(), 1);
    //addAndMakeVisible calls
    addAndMakeVisible(mixSlider);
    addAndMakeVisible(panLawBox);

    //setting attachment pointers
    mixSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, "mix", mixSlider);
    panLawBoxAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(processorRef.apvts, "panlaw", panLawBox);

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
    //let p = percent size of whole screen, for center: x = ((1-p)width)/2, y = x = ((1-p)height)/2
    panLawBox.centreWithSize(75, 50);
}
