/*
  ==============================================================================

    Sequencer.cpp
    Created: 8 Nov 2019 1:53:40am
    Author:  Quin Scacheri

  ==============================================================================
*/

#include "Sequencer.h"

Sequencer::Sequencer(Synthesiser* s)
{
    sampler = s;
    
    formatManager.registerBasicFormats();
    for (int i = 0; i < 4; i++) {
        sequences[i].reset(new NoteSequence());
    }
    
    for (int i = 0; i < NoteSequence::noteValues.size(); i++) {
        channels[NoteSequence::noteValues[i]] = 1;
    }
}

void Sequencer::prepareToPlay(double sampleRate)
{
    lastSampleRate = sampleRate;
    sampler->setCurrentPlaybackSampleRate(sampleRate);
}

void Sequencer::processBlock(AudioPlayHead* p, AudioBuffer<float> &buffer, MidiBuffer &midiBuffer)
{
    if (!isActive)
        return;
    
    playhead = p;
    
    int numSamples = buffer.getNumSamples();
    if (playhead == nullptr)
        return;
    

    addToBufferIfNeeded(0, numSamples, midiBuffer);
    
    sampler->renderNextBlock(buffer, midiBuffer, 0, buffer.getNumSamples());
}
            
void Sequencer::addToBufferIfNeeded(int which, int samplesPerBlock, MidiBuffer &midiBuffer)
{
    
    // get current position
    AudioPlayHead::CurrentPositionInfo currentPos;
    playhead->getCurrentPosition(currentPos);
    if (!currentPos.isPlaying)
        return;
    long long posInSamples = currentPos.timeInSamples;
    posInSamples %= NoteSequence::ppqToSamples(loopEnd, currentPos.bpm, lastSampleRate);
    double loopEndInSamples = NoteSequence::ppqToSamples(loopEnd, currentPos.bpm, lastSampleRate);

    for (int i = 0; i < 4; i++) {
        double downbeat = NoteSequence::ppqToSamples((int)i * .25, currentPos.bpm, lastSampleRate);
        double nextBeat = NoteSequence::ppqToSamples((int)(i + 1) * .25, currentPos.bpm, lastSampleRate);
        if (posInSamples + samplesPerBlock >= downbeat && posInSamples < nextBeat) {
            currentDownbeat = i;
            break;
        }
    }
    
    int beatOffset = 0;
    // iterate through all notes
    for (int s = 0; s < activeSequences.size(); s++) {
        if (!activeSequences[s]) continue;
        auto notes = sequences[s]->getNotes();

        for (int i = 0; i < notes.size(); i++) {
        int beatInSamples = NoteSequence::ppqToSamples(beatOffset + notes[i].startTime, currentPos.bpm, lastSampleRate);
        
        // check first beat
        if (posInSamples + samplesPerBlock >= loopEndInSamples && posInSamples <= loopEndInSamples && notes[i].startTime + beatOffset == 0)
        {
            long long offset = loopEndInSamples - posInSamples;
            
            int channel = channels[notes[i].pitch];
            if (channel != 1) {
                midiBuffer.addEvent(MidiMessage::allNotesOff(channel), (int)offset);
                midiBuffer.addEvent(MidiMessage::allSoundOff(channel), (int)offset);
            }
            midiBuffer.addEvent(MidiMessage::noteOn(channel, notes[i].pitch, .8f), (int)offset);
            lastNotesPlayed.push(notes[i]);
        }
        
        // check all other beats
        else if (beatInSamples > posInSamples &&
                 beatInSamples < posInSamples + samplesPerBlock)
        {
            long long offset = beatInSamples - posInSamples;
            
            int channel = channels[notes[i].pitch];
            if (channel != 1) {
                midiBuffer.addEvent(MidiMessage::allNotesOff(channel), (int)offset);
                midiBuffer.addEvent(MidiMessage::allSoundOff(channel), (int)offset);
            }
            midiBuffer.addEvent(MidiMessage::noteOn(channel, notes[i].pitch, .8f), (int)offset);
            lastNotesPlayed.push(notes[i]);
        }}
        beatOffset++;
    }
}

ValueTree Sequencer::getStateInformation()
{
//    return sequence->toValueTree();
}

void Sequencer::setStateInformation(ValueTree tree)
{
//    sequence->fromValueTree(tree);
    
}

void Sequencer::addListener(Listener* l)
{
    listeners.push_back(l);
}

void Sequencer::removeListener(Listener* l)
{
    for (int i = 0; i < listeners.size(); i++)
    {
        if (listeners[i] == l)
            listeners.erase(listeners.begin()+i);
    }
}

void Sequencer::addNote(int barNum, Note n)
{
    sequences[barNum]->addNote(n);
}

void Sequencer::removeNote(Note n)
{

}

void Sequencer::createSynthesizerSound(Sequencer::Layout layout)
{
    std::unique_ptr<AudioFormatReader> reader;
    BigInteger range;
    range.setBit(layout.midiNote);
    reader.reset(formatManager.createReaderFor (std::make_unique<MemoryInputStream>(layout.data, layout.size, false)));
    samplerSounds.push_back(new SamplerSound(layout.name, *reader.get(), range, layout.midiNote, 0, 10, 10.0));
}

void Sequencer::notifyListenersNotePlayed(int pitch, int note) { 
    for (int i = 0; i < listeners.size(); i++){
        MessageManager::callAsync ([&] {listeners[i]->notePlayed(pitch, note); });
    }
}

void Sequencer::setSequenceLength(int newLength) {
    sequenceLength = (newLength > 0 ? newLength : sequenceLength);
}

void Sequencer::setSubDivision(NoteSequence::SubDivision s) {
    subdivision = s;
    
    for (int i = 0; i < sequences[0]->getNotes().size(); i++) {
        Note currentNote = sequences[0]->getNotes()[i];
        DBG(currentNote.startTime / double(subdivision));
        if (std::fmod(currentNote.startTime, subdivision) != 0)
            sequences[0]->removeNote(currentNote.pitch, currentNote.startTime);
    }
}
