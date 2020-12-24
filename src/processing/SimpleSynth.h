/*
  ==============================================================================

    SimpleSynth.h
    Created: 29 May 2019 9:07:54am
    Author:  Lily H

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "OrionSamplerVoice.h"
#include "OrionSamplerSound.h"
#include "DragAndDropButton.h"



class SimpleSynth : public Synthesiser
//,public AudioProcessorValueTreeState::Listener
{
public:
    
    //SimpleSynth(OrionaudioAudioProcessorEditor*);
    //~SimpleSynth();

    void setup(double sr);
    void loadSamples();
    void noteOn(int midiChannel,
                int midiNoteNumber,
                float velocity) override;
    void noteOff(int midiChannel,
                 int midiNoteNumber,
                 float velocity,
                 bool allowTailOff) override;
    typedef enum Sounds{KICK=0, SNARE, CLAP, PERC, SNAP, HHO, HHC, CRASH} Sounds_t;
    double sampleRate;
    
    void changeSamples(int index,const String &f,int midi);


    std::array<std::unique_ptr<DragAndDropButton>, 8> drumButtons;
    
    //OrionaudioAudioProcessorEditor* editor;
    
private:
    
    File audiofolder;
    AudioFormatManager audioFormatManager;
    File* file;
    bool isChokeGroupActive;
};
