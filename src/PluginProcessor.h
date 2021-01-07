/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "./SimpleSynth.h"
#include "./Analyser.h"
#include "LookAndFeelHolder.h"
#include "OrionLookAndFeel.h"
#include "Sequencer.h"
#include "GlobalCoefficients.h"

//==============================================================================
/**
*/
class OrionaudioAudioProcessor  : public AudioProcessor
{
public:
    struct OutputLevels
    {
        float left = 0;
        float right = 0;
    };
   
    //==============================================================================
    OrionaudioAudioProcessor();
    ~OrionaudioAudioProcessor();
    
    //void createFrequencyPlot (Path& p, const std::vector<double>& mags, const Rectangle<int> bounds, float pixelsPerDouble);
    
    void createAnalyserPlot (Path& p, const Rectangle<int> bounds, float minFreq, bool input);

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    float getOutputLevel(int channel)
    {
        if (channel == 0)
            return outputLevels.left;
        else
            return outputLevels.right;
    }
    
    float getInputLevel()
    {
        return inputLevel;
    }
    
    
    
    MidiOutput* getMidiOutput() { return midiOutput.get(); }
    
    Sequencer* getSequencer() { return sequencer.get(); }
    
    AudioTransportSource transport;
    
    //AudioProcessorValueTreeState tree;
    
    AudioProcessorValueTreeState& getValueTree() { return parameters; }
    
    
    std::unique_ptr<SimpleSynth> sampler;
    SimpleSynth* getSampler() {return sampler.get();}
    
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    
   
    //------------------------------------
    //unsigned int OrionGlobalTabIndex = 0;/*  储存 Tab 的变量 */

    AudioProcessorValueTreeState parameters;

private:
    // midi output object to write to, enables recording
    std::unique_ptr<MidiOutput> midiOutput;

    // creates and sets look and feel globally
    LookAndFeelHolder<OrionLookAndFeel> lafHolder;
    
    std::unique_ptr<Sequencer> sequencer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OrionaudioAudioProcessor)
    AudioBuffer<float> mDelayBuffer;
    AudioBuffer<float> tempbuffer;
    int mWritePosition { 0 };
    int mSampleRate { int(globalSampleRate) };
    
    float inputLevel;
    
    OutputLevels outputLevels;
    
    Analyser<float> inputAnalyser;
    Analyser<float> outputAnalyser;
    
    
    //std::unique_ptr<soundtouch::SoundTouch> soundTouch;
    
};
