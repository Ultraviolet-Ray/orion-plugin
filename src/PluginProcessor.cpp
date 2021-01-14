/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"


#include "PluginEditor.h"

#define NUM_VOICES 8

#define __COEFFICIENTSMAIN__
#include "GlobalCoefficients.h"

//==============================================================================
static String doubleToString(double val) { return String(val); }
static double stringToDouble(String s){return std::stod(s.toStdString());}
//==============================================================================
OrionaudioAudioProcessor::OrionaudioAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                      #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ), parameters(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif


{
    midiOutput = MidiOutput::createNewDevice("Orion Audio");
    if (midiOutput != nullptr)
        midiOutput->startBackgroundThread();

    
    sampler.reset(new SimpleSynth());
    sampler->setup(globalSampleRate);

    sequencer.reset(new Sequencer(static_cast<Synthesiser*>(sampler.get())));
    sequencer->setActive(true);
    
    
}

OrionaudioAudioProcessor::~OrionaudioAudioProcessor()
{

}


void OrionaudioAudioProcessor::createAnalyserPlot (Path& p, const Rectangle<int> bounds, float minFreq, bool input)
{
    if (input)
        inputAnalyser.createPath (p, bounds.toFloat(), minFreq);
    else
        outputAnalyser.createPath (p, bounds.toFloat(), minFreq);
}


//==============================================================================
const String OrionaudioAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OrionaudioAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OrionaudioAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OrionaudioAudioProcessor::isMidiEffect() const
{
//   #if JucePlugin_IsMidiEffect
//    return true;
//   #else
//    return false;
//   #endif
    return false;
}

double OrionaudioAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OrionaudioAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OrionaudioAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OrionaudioAudioProcessor::setCurrentProgram (int index)
{
}

const String OrionaudioAudioProcessor::getProgramName (int index)
{
    return {};
}

void OrionaudioAudioProcessor::changeProgramName (int index, const String& newName)
{

}




//==============================================================================
void OrionaudioAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    const int numInputChannels = getTotalNumInputChannels();
    const int delayBufferSize = 2 * (sampleRate + samplesPerBlock);//2 seconds of delay buffer
    mDelayBuffer.setSize(numInputChannels, delayBufferSize);
    mSampleRate = sampleRate;
    sampler->setCurrentPlaybackSampleRate(sampleRate);

    sequencer->prepareToPlay(sampleRate);
}

void OrionaudioAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OrionaudioAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void OrionaudioAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    inputLevel = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
    buffer.clear();
    
    /*
    sampler->renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    outputLevels.left = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
    if (buffer.getNumChannels() > 1)
        outputLevels.right = buffer.getRMSLevel(1, 0, buffer.getNumSamples());
    else
        outputLevels.right = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
     */
    
    outputLevels.left = globalOutputMeterL;
    outputLevels.right = globalOutputMeterR;
    

    AudioPlayHead* playhead = getPlayHead();
    
    sequencer->processBlock(playhead, buffer, midiMessages);
    //inputAnalyser.addAudioData(buffer, 0, getTotalNumInputChannels());

}

//==============================================================================
bool OrionaudioAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* OrionaudioAudioProcessor::createEditor()
{
    return new OrionaudioAudioProcessorEditor (*this);;
}

//==============================================================================
void OrionaudioAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    ValueTree state
    { "ORION_PLUGIN", {},
        {
            { "SEQUENCER", {}},
        }
    };
    state.addChild(parameters.copyState(), 0, nullptr);
    //state.getChildWithName("SEQUENCER").addChild(sequencer->getStateInformation(), 1, nullptr);
//    DBG(state.toXmlString());
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void OrionaudioAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if(xmlState == nullptr){return;}
    ValueTree state = ValueTree::fromXml(*xmlState);
    DBG(state.toXmlString());
    parameters.replaceState(state.getChildWithName("PARAMETERS"));
    sequencer->setStateInformation(state.getChildWithName("SEQUENCER").getChild(0));
}

AudioProcessorValueTreeState::ParameterLayout OrionaudioAudioProcessor::createParameterLayout()
{
    AudioProcessorValueTreeState::ParameterLayout layout;
    for(int i = 0; i < NUM_VOICES; i++)
    {

        // compressor
        layout.add(std::make_unique<AudioParameterFloat>("compRatio" + String(i), "compRatio" + String(i), NormalisableRange<float>(1.0f, 30.0f, 1.0f), 1.f, "", AudioProcessorParameter::Category::genericParameter));

        layout.add(std::make_unique<AudioParameterFloat>("compAttack" + String(i), "compAttack" + String(i), NormalisableRange<float>(0.1f, 80.0f, 0.1f), 0.1f, "", AudioProcessorParameter::Category::genericParameter));

        layout.add(std::make_unique<AudioParameterFloat>("compRelease" + String(i), "compRelease", NormalisableRange<float>(0.1f, 1000.0f, 0.1f), 0.1f, "", AudioProcessorParameter::Category::genericParameter));

        layout.add(std::make_unique<AudioParameterFloat>("compGain" + String(i), "compGain" + String(i), NormalisableRange<float>(0.0f, 40.0f, 0.1f), 0.0f, "", AudioProcessorParameter::Category::genericParameter));

        layout.add(std::make_unique<AudioParameterFloat>("compThresh" + String(i), "compThresh" + String(i), NormalisableRange<float>(-60.0f, 0.0f, 0.1f), -60.0f, "", AudioProcessorParameter::Category::genericParameter));

        layout.add(std::make_unique<AudioParameterFloat>("compSwitch" + String(i), "compSwitch" + String(i), NormalisableRange<float>(0, 1, 1), 0, "", AudioProcessorParameter::Category::genericParameter));

        // reverb
        layout.add(std::make_unique<AudioParameterFloat>("reverbPredelay" + String(i), "reverbPredelay" + String(i), NormalisableRange<float>(0.0f, 1.0f), 0.0f, "", AudioProcessorParameter::Category::genericParameter));

        layout.add(std::make_unique<AudioParameterFloat>("reverbSize" + String(i), "reverbSize" + String(i), NormalisableRange<float>(0.0f, 1.0f), 0.0f, "", AudioProcessorParameter::Category::genericParameter));

        layout.add(std::make_unique<AudioParameterFloat>("reverbColor" + String(i), "reverbColor", NormalisableRange<float>(-1.0f, 1.0f/*, 1*/), 0.0f, "", AudioProcessorParameter::Category::genericParameter));

        layout.add(std::make_unique<AudioParameterFloat>("reverbDecay" + String(i), "reverbDecay", NormalisableRange<float>(0.0f, 1.0f), 0.0f, "", AudioProcessorParameter::Category::genericParameter));

        layout.add(std::make_unique<AudioParameterFloat>("reverbDry" + String(i), "reverbDry", NormalisableRange<float>(0.0f, 1.0f), 0.0f, "", AudioProcessorParameter::Category::genericParameter));

        layout.add(std::make_unique<AudioParameterFloat>("reverbSwitch" + String(i), "reverbSwitch", NormalisableRange<float>(0, 1, 1), 0, "", AudioProcessorParameter::Category::genericParameter));


        // delay
        layout.add(std::make_unique<AudioParameterFloat>("delayTime" + String(i), "delayTime", NormalisableRange<float>(0.0f, 0.5f), 0.1f, "", AudioProcessorParameter::Category::genericParameter));

        layout.add(std::make_unique<AudioParameterFloat>("delayFeedback" + String(i), "delayFeedback", NormalisableRange<float>(0.0f, 0.95f), 0.0f, "", AudioProcessorParameter::Category::genericParameter));

        layout.add(std::make_unique<AudioParameterFloat>("delayColor" + String(i), "delayColor", NormalisableRange<float>(-1.0f, 1.0f/*, 1*/), 0.0f, "", AudioProcessorParameter::Category::genericParameter));

        layout.add(std::make_unique<AudioParameterFloat>("delayPan" + String(i), "delayPan" + String(i),NormalisableRange<float>(-50.0f, 50.0f, 1.0f), 0.0f, "", AudioProcessorParameter::Category::genericParameter));

        layout.add(std::make_unique<AudioParameterFloat>("delayDryWet" + String(i), "delayDryWet" + String(i), NormalisableRange<float>(0.0f, 1.0f), 0.3f, "", AudioProcessorParameter::Category::genericParameter));

        layout.add(std::make_unique<AudioParameterFloat>("delaySwitch" + String(i), "delaySwitch" + String(i), NormalisableRange<float>(0, 1, 1), 0, "", AudioProcessorParameter::Category::genericParameter));


        // envelope
        layout.add(std::make_unique<AudioParameterFloat>("envAttack" + String(i), "envAttack", NormalisableRange<float>(0.1f, 1500.0f, 0.1f), 0.1f, "", AudioProcessorParameter::Category::genericParameter));

        layout.add(std::make_unique<AudioParameterFloat>("envHold" + String(i), "envHold", NormalisableRange<float>(0.1f, 1500.0f, 0.1f), 0.1f, "", AudioProcessorParameter::Category::genericParameter));

        layout.add(std::make_unique<AudioParameterFloat>("envDecay" + String(i), "envDecay", NormalisableRange<float>(0.1f, 2500.0f, 0.1f), 0.1f, "", AudioProcessorParameter::Category::genericParameter));

        layout.add(std::make_unique<AudioParameterFloat>("envRelease" + String(i), "envRelease", NormalisableRange<float>(0.1f, 2500.0f, 0.1f), 1000.0f, "", AudioProcessorParameter::Category::genericParameter));

        layout.add(std::make_unique<AudioParameterFloat>("envAttackBend" + String(i), "envAttackBend", NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.01f, "", AudioProcessorParameter::Category::genericParameter));

        layout.add(std::make_unique<AudioParameterFloat>("envSustain" + String(i), "envSustain", NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f, "", AudioProcessorParameter::Category::genericParameter));

        layout.add(std::make_unique<AudioParameterFloat>("envDecayBend" + String(i), "envDecayBend", NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.01f, "", AudioProcessorParameter::Category::genericParameter));

        layout.add(std::make_unique<AudioParameterFloat>("envReleaseBend" + String(i), "envReleaseBend", NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.01f, "", AudioProcessorParameter::Category::genericParameter));
        
    }
    
    return layout;

}


void OrionaudioAudioProcessor::audioRangeChange(int serial)
{
    int originalLength = instrumentSampleContainer[serial].getNumSamples();
    int originalChannelNum = instrumentSampleContainer[serial].getNumChannels();
    
    int newSampleLength = RVal[serial] * originalLength - LVal[serial] * originalLength;
    
    instrumentSampleBufferPointer[serial]->setSize(originalChannelNum, newSampleLength,/* keepExistingContent: */false,/* clearExtraSpace: */true,/* avoidReallocating: */false);
    instrumentSampleBufferPointer[serial]->setDataToReferTo(instrumentSampleContainer[serial].getArrayOfWritePointers(), originalChannelNum, LVal[serial] * originalLength, RVal[serial] * originalLength);
    
    if (auto* sound = dynamic_cast<OrionSamplerSound*> (sampler->getSound(serial).get()))
    {
        sound->setLength(newSampleLength);
    }
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OrionaudioAudioProcessor();
}
