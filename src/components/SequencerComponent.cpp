/*
  ==============================================================================

    SequencerComponent.cpp
    Created: 8 Nov 2019 1:53:21am
    Author:  Quin Scacheri

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SequencerComponent.h"
#include "qtils.h"
#include <cmath>

//==============================================================================
SequencerComponent::SequencerComponent(Sequencer &s) : sequencer(s)
{
    sequencer.addListener(this);
    
    setOpaque(true);
        
    startTimerHz(15);
    
    voiceNames.add("KICK");
    voiceNames.add("SNARE");
    voiceNames.add("HH-CLOSED");
    voiceNames.add("HH-OPEN");
    voiceNames.add("CRASH");
    voiceNames.add("CLAP");

    setColour(ColourIds::backgroundColourId, Colours::black);
    setColour(ColourIds::beatColourOffId, Colours::magenta);
    setColour(ColourIds::beatColourOnId, Colours::magenta);

    lengthSlider.reset(new Slider());
    lengthSlider->setSliderStyle(Slider::SliderStyle::Rotary);
    lengthSlider->setColour(Slider::ColourIds::rotarySliderFillColourId, findColour(ColourIds::beatColourOffId));
    lengthSlider->setColour(Slider::ColourIds::backgroundColourId, Colours::white);
    
    double beatLength = 1.0 / (double)sequencer.getSubDivision();

    auto noteButtonFn = [&] (int pitch, double beat) {
        sequencerButtons[pitch][beat].reset(new SequencerButton(NoteSequence::noteValues[pitch], beat * beatLength));
        addAndMakeVisible(sequencerButtons[pitch][beat].get());
        sequencerButtons[pitch][beat]->addListener(this);
    };
    
    for (int pitch = 0; pitch < NUM_VOICES; pitch++) {
        for (double beat = 0; beat < 32; beat++) {
            noteButtonFn(pitch, beat);
        }
    }
    
    auto labelFn = [&] (std::unique_ptr<Label> &l, String name) {
        l = std::make_unique<Label>(name, name);
        l->setFont(Font().withHeight(3));
        addAndMakeVisible(l.get());
    };
    
    labelFn(labels[0], "Kick");
    labelFn(labels[1], "Snare");
    labelFn(labels[2], "Closed Hat");
    labelFn(labels[3], "Open Hat");
    labelFn(labels[4], "Clap");
    labelFn(labels[5], "Perc");
    labelFn(labels[6], "Snap");
    labelFn(labels[7], "Crash");

    
    setValuesFromPlugin();
    

}

SequencerComponent::~SequencerComponent()
{
}

void SequencerComponent::paint(Graphics& g)
{
    g.fillAll(Colours::black);
    paintRows(g);
    paintCols(g);
}

void SequencerComponent::paintRows(Graphics& g)
{
    g.setColour(Colours::white);
    for (int i = 0; i < NUM_VOICES - 1; i++) {
        int yPos = sequencerButtons[i][0]->getBottom() + ((sequencerButtons[i + 1][0]->getY() - sequencerButtons[i][0]->getBottom()) / 2);
        g.fillRect(0, yPos - 1, getWidth(), 2);
    }
}

void SequencerComponent::paintCols(Graphics& g)
{
    g.setColour(Colours::white);
    const int numSections = 4;
    const int numBeats = int(sequencer.getSubDivision());
    const int numBeatsPerSection = numBeats / numSections;
    for (int i = numBeatsPerSection - 1; i < numBeats - numBeatsPerSection; i+=numBeatsPerSection) {
        int xPos = sequencerButtons[0][i]->getRight() + ((sequencerButtons[0][i + 1]->getX() - sequencerButtons[0][i]->getRight()) / 2);
        g.fillRect(xPos - 1, 0, 2, getHeight());
    }
}

void SequencerComponent::resized()
{
    double subdivision = sequencer.getSubDivision();
    auto totalArea = getLocalBounds().removeFromRight(getWidth() * .85);
    float xDist = floor((float)totalArea.getWidth() / (subdivision));
    float yDist = (float)totalArea.getHeight() / NUM_VOICES;
    float diameter = fmin(xDist, yDist);
    
    Rectangle<int> area(totalArea.getX(), 0, xDist, yDist);
    for (int i = 0; i < NUM_VOICES; i++) {
        for (int j = 0; j < 32; j++) {
            sequencerButtons[i][j]->setBounds(area.withSizeKeepingCentre(diameter * .75, diameter * .75));
            sequencerButtons[i][j]->setVisible(j < sequencer.getSubDivision());
            area.translate(xDist, 0);
        }
        area.setX(totalArea.getX());
        area.translate(0, yDist);
    }
    
    area.setBounds(0, 0, getWidth() * .15, yDist);
    
    for (int i = 0; i < labels.size(); i++) {
        labels[i]->setBounds(area);
        area.translate(0, yDist);
    }
}


void SequencerComponent::mouseUp(const MouseEvent& e)
{

}


void SequencerComponent::colourChanged()
{
    if (isColourSpecified(ColourIds::beatColourOffId && lengthSlider != nullptr))
        lengthSlider->setColour(Slider::ColourIds::rotarySliderFillColourId, findColour(ColourIds::beatColourOffId));
}

void SequencerComponent::timerCallback()
{
    std::queue<Note>* noteQueue = &sequencer.lastNotesPlayed;
    for (int i = 0; i < noteQueue->size(); i++) {
        int pitch = NoteSequence::noteValues.indexOf(noteQueue->front().pitch);
        double beat = noteQueue->front().startTime;
        int index = qtils::map(beat, 0, 1, 0, sequencer.getSubDivision());
        sequencerButtons[pitch][index]->startAnimation();
        noteQueue->pop();
    }
}

void SequencerComponent::buttonClicked(Button* b)
{
    SequencerButton* button = dynamic_cast<SequencerButton*>(b);
    if (button != nullptr) {
        int pitch = button->getPitch();
        double beat = button->getBeat();
//        std::cout << pitch << "," << beat << std::endl;
        NoteSequence* sequence = sequencer.getNoteSequence();
        if (sequence->checkAndRemoveNote(pitch, beat))
            DBG("removed");
        else
        {
            sequence->addNote(Note(pitch, 100, beat, beat + (1.0 / sequencer.getSubDivision())));
        }
    }
}

void SequencerComponent::buttonStateChanged(Button* b)
{
    if (b->getToggleStateValue() == true)
        isSelected = true;
    else
        isSelected = false;
}

void SequencerComponent::notePlayed(int part, int beat) {
    
}

void SequencerComponent::handleButtonPress(int pitch, int beat, bool buttonState) { 

}

void SequencerComponent::setSequenceLength(int newLength) {
    if (newLength <= 0) return;
    resized();
}

void SequencerComponent::setSizeWithOverflow(int height) {
    float buttonSize = height / NUM_VOICES;
    setSize(buttonSize * sequenceLength, height);
}


void SequencerComponent::sequenceLengthChanged(int newLength) {
    setSequenceLength(newLength);
}

void SequencerComponent::setSubDivision(NoteSequence::SubDivision s) {
    sequencer.setSubDivision(s);
    double beatLength = 1.0 / (double)sequencer.getSubDivision();

    for (int i = 0; i < NUM_VOICES; i++) {
        for (double j = 0; j < 32; j++) {
            double beat = j * beatLength;
            sequencerButtons[i][j]->setBeat(beat);
            sequencerButtons[i][j]->setToggleState(sequencer.getNoteSequence()->isNotePresent(NoteSequence::noteValues[i], beat), false);
        }
    }
    resized();
}

void SequencerComponent::setValuesFromPlugin() {
    double beatLength = 1.0 / (double)sequencer.getSubDivision();

    for (int i = 0; i < NUM_VOICES; i++) {
        for (double j = 0; j < 32; j++) {
            double beat = j * beatLength;
            sequencerButtons[i][j]->setBeat(beat);
            sequencerButtons[i][j]->setToggleState(sequencer.getNoteSequence()->isNotePresent(NoteSequence::noteValues[i], beat), false);
        }
    }
    resized();

}
