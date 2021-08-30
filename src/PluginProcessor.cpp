#include "PluginProcessor.h"
#include "PluginEditor.h"

# define PI           3.14159265358979323846  /* pi */

//==============================================================================
HyperpanAudioProcessor::HyperpanAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
{

}

HyperpanAudioProcessor::~HyperpanAudioProcessor()
{
}

//==============================================================================
const juce::String HyperpanAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HyperpanAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HyperpanAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HyperpanAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HyperpanAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HyperpanAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HyperpanAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HyperpanAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String HyperpanAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void HyperpanAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void HyperpanAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void HyperpanAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool HyperpanAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void HyperpanAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                           juce::MidiBuffer& midiMessages)
{
    std::atomic<float>* mixParameter = apvts.getRawParameterValue("mix");
    float mix = mixParameter->load() / 100.f;

    //Linear, Square, Sine
    juce::String panlaw = apvts.getParameter("panlaw")->getCurrentValueAsText();

    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    if (totalNumInputChannels != 2) {
        return;
    }

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto* left = buffer.getWritePointer(0);
    auto* right = buffer.getWritePointer(1);
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        float monoSum = (left[sample] + right[sample]) * 0.5f;
        float normalizedAmplitude = (monoSum + 1) * 0.5f;
        float leftMult = 1.f, rightMult = 1.f;
        if (panlaw == "Linear")
        {
            //LINEAR PAN LAW
            leftMult = (1.f - normalizedAmplitude);
            rightMult = normalizedAmplitude;
        }
        if (panlaw == "Square")
        {
            //SQUARE PAN LAW
            leftMult = sqrtf(1.f - normalizedAmplitude);
            rightMult = sqrtf(normalizedAmplitude);
        }
        if (panlaw == "Sine") {
            //SINE PAN LAW
            leftMult = sinf(static_cast<float>((1.f - normalizedAmplitude) * PI / 2.f));
            rightMult = sinf(static_cast<float>(normalizedAmplitude * PI / 2.f));
        }
        float leftWet = left[sample] * leftMult;
        float rightWet = right[sample] * rightMult;

        left[sample] = (leftWet * mix) + (left[sample] * (1.f - mix));
        right[sample] = (rightWet * mix) + (right[sample] * (1.f - mix));
        /*
         * OLD PAN LAW
        if (normalizedAmplitude > 0.5) {
            float multiplier = -2.f*normalizedAmplitude + 2;
            float wet = right[sample] * multiplier;
            right[sample] = (wet * mix) + (right[sample] * (1.f - mix));
        } else {
            float multiplier = 2.f*normalizedAmplitude;
            float wet = left[sample] * multiplier;
            left[sample] = (wet * mix) + (left[sample] * (1.f - mix));
        }
         */
    }
}

//==============================================================================
bool HyperpanAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HyperpanAudioProcessor::createEditor()
{
    return new HyperpanAudioProcessorEditor (*this);
}

//==============================================================================
void HyperpanAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void HyperpanAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}


juce::AudioProcessorValueTreeState::ParameterLayout HyperpanAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    auto cutoffRange = juce::NormalisableRange<float>(0.f, 100.f, 0.1f, 1.f, true);
    layout.add(std::make_unique<juce::AudioParameterFloat>("mix", "Dry/Wet", cutoffRange, 0.f));
    layout.add(std::make_unique<juce::AudioParameterChoice>("panlaw", "Pan Law", juce::StringArray("Linear", "Square", "Sine"), 0));
    return layout;
}

void HyperpanAudioProcessor::reset()
{

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HyperpanAudioProcessor();
}
