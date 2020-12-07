/*
  ==============================================================================

    Sequencer.h
    Created: 8 Nov 2019 1:53:40am
    Author:  Quin Scacheri

  ==============================================================================
*/

#pragma once
#include "NoteSequence.h"
#include "JuceHeader.h"
#include <array>
#include <queue>
#include "GlobalCoefficients.h"

class Sequencer
{
public:
    struct Layout
    {
        Layout(String n, const char *d, int s, int m) : name(n), data(d), size(s), midiNote(m)
        {
        }

        String name;
        const char *data;
        int size;
        int midiNote;
    };

    class Listener
    {
    public:
        virtual void sequenceChanged() = 0;
        virtual void sequenceLengthChanged(int newLength) = 0;
        virtual void notePlayed(int part, int beat) = 0;
    };

    Sequencer(Synthesiser *sampler);

    NoteSequence *getNoteSequence() { return sequence.get(); };
    void setNewSequence(NoteSequence *newSequence);

    void prepareToPlay(double sampleRate);

    void processBlock(AudioPlayHead *p, AudioBuffer<float> &buffer, MidiBuffer &midiBuffer);
    void addToBufferIfNeeded(int which, int samplesPerBlock, MidiBuffer &midiBuffer);

    ValueTree getStateInformation();
    void setStateInformation(ValueTree tree);

    // listener methods
    void addListener(Listener *);
    void removeListener(Listener *);

    void swapSequences(Sequencer &otherSequencer, bool clearOther = true);
    void clearSequence();
    std::unique_ptr<NoteSequence> &getSequenceForSwap();

    void addNote(Note n);
    void removeNote(Note n);

    void createSynthesizerSound(Layout l);

    bool getActive() { return isActive; }
    void setActive(bool active) { isActive = active; }

    std::queue<Note> lastNotesPlayed;
    int currentDownbeat = 0;

    int getTotalLength() { return sequenceLength; }
    void setSequenceLength(int newLength);

    NoteSequence::SubDivision getSubDivision() { return subdivision; };
    void setSubDivision(NoteSequence::SubDivision s);

    void setChannel(int voice, int channel) { channels[voice] = channel; };

private:
    std::map<int, int> channels;

    NoteSequence::SubDivision subdivision = NoteSequence::SubDivision::sixteenth;
    double loopEnd = 1;
    void notifyListenersNotePlayed(int pitch, int note);
    std::vector<Listener *> listeners;

    std::unique_ptr<NoteSequence> sequence;
    AudioPlayHead *playhead;
    Synthesiser *sampler;
    AudioFormatManager formatManager;
    float lastSampleRate = globalSampleRate;
    int sequenceLength = 16;
    NoteSequence::SubDivision currentSubDivision = NoteSequence::SubDivision::sixteenth;
    bool isActive = true;

    std::vector<SynthesiserSound::Ptr> samplerSounds;
};
