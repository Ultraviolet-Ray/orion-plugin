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
#include "OrionLookAndFeel.h"
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
        l->setFont(Font().withHeight(1));
        l->setColour(Label::ColourIds::textColourId, Colour(0xFF32EDD6));
        l->setJustificationType(Justification::centred);
        addAndMakeVisible(l.get());
    };
    
    labelFn(labels[0], "KICK");
    labelFn(labels[1], "SNARE");
    labelFn(labels[2], "CLAP");
    labelFn(labels[3], "PERC");
    labelFn(labels[4], "SNAP");
    labelFn(labels[5], "CLOSED HAT");
    labelFn(labels[6], "OPEN HAT");
    labelFn(labels[7], "CRASH");
    
    StringArray channels;
    channels.add("none");
    for (int i = 0; i < NUM_VOICES; i++) {
        channels.add(String(i + 1));
    }
    
    for (int i = 0; i < NUM_VOICES; i++) {
        channelCombos[i].addItemList(channels, 1);
        channelCombos[i].setSelectedId(1);
        channelCombos[i].addListener(this);
        channelCombos[i].getProperties().set("CHANNEL_COMBO", true);
        addAndMakeVisible(channelCombos[i]);
    }
    
    for (int i = 0; i < barButtons.size(); i++) {
        barButtons[i].reset(new BarButton(i));
        barButtons[i]->onSelection = [&] (int newSelection) {
            handleNewBarSelection(newSelection);
        };
        addAndMakeVisible(barButtons[i].get());
    }
    
    arrangeLabel.setText("ARRANGEMENT MODE", NotificationType::dontSendNotification);
    arrangeLabel.setColour(Label::ColourIds::textColourId, Colour(0xFF36FEE2));
    addAndMakeVisible(arrangeLabel);
    
    StringArray subdivisions;
    subdivisions.add("2n");
    subdivisions.add("4n");
    subdivisions.add("8n");
    subdivisions.add("16n");
    subdivisions.add("32n");
    subdivisions.add("8n triplet");
    subdivisions.add("16n triplet");
    
    subdivisionCombo.addItemList(subdivisions, 1);
    subdivisionCombo.onChange = [&] {
        NoteSequence::SubDivision newSubdivision;
        
        switch (subdivisionCombo.getSelectedId()) {
            case 1:
                newSubdivision = NoteSequence::SubDivision::half;
                break;
            case 2:
                newSubdivision = NoteSequence::SubDivision::quarter;
                break;
            case 3:
                newSubdivision = NoteSequence::SubDivision::eighth;
                break;
            case 4:
                newSubdivision = NoteSequence::SubDivision::sixteenth;
                break;
            case 5:
                newSubdivision = NoteSequence::SubDivision::thirtysecond;
                break;
            case 6:
                newSubdivision = NoteSequence::SubDivision::tripletEigth;
                break;
            case 7:
                newSubdivision = NoteSequence::SubDivision::tripletSixteenth;
                break;
            default:
                break;
        }
        setSubDivision(newSubdivision);
    };
    
    subdivisionCombo.getProperties().set("SUBDIVISION_COMBO", true);
    addAndMakeVisible(subdivisionCombo);
    
    barCountLabel.setText("BAR COUNT", NotificationType::dontSendNotification);
    barCountLabel.attachToComponent(barButtons[0].get(), true);
    addAndMakeVisible(barCountLabel);
    
    beatCountLabel.setText("BEAT COUNT", dontSendNotification);
    beatCountLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(beatCountLabel);
    
    notesLabel.setText("NOTES", NotificationType::dontSendNotification);
    addAndMakeVisible(notesLabel);
    
    instLabel.setText("INSTRUMENT", dontSendNotification);
    instLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(instLabel);
    
    channelLabel.setText("CHANNEL", dontSendNotification);
    channelLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(channelLabel);
    
    setValuesFromPlugin();
    
}

SequencerComponent::~SequencerComponent()
{
}

void SequencerComponent::paint(Graphics& g)
{
    g.fillAll(Colour(OrionLookAndFeel::ThemeColors::backgroundColor));
    paintRows(g);
    paintCols(g);
    paintBar(g);
}

void SequencerComponent::paintRows(Graphics& g)
{
    g.setColour(Colour(0xFF0F3836));
    for (int i = 0; i < NUM_VOICES - 1; i++) {
        int yPos = sequencerButtons[i][0]->getBottom() + ((sequencerButtons[i + 1][0]->getY() - sequencerButtons[i][0]->getBottom()) / 2);
        g.fillRect(plotArea.getX(), yPos - 1, getWidth(), 2);
    }
}

void SequencerComponent::paintCols(Graphics& g)
{
    g.setColour(Colour(0xFF36FEE2));
    for (int i = 0; i < barLines.size(); i++) {
        g.fillRect(barLines[i]);
    }
}

void SequencerComponent::paintBar(Graphics& g)
{
    int start = barLines[currentBeat].getX();
    int end = currentBeat == 3 ? plotArea.getRight() : barLines[currentBeat + 1].getRight();
    int width = end - start;
    Path p;
    
    g.setColour(Colour(0xFF313131));
    p.addRoundedRectangle(plotArea.getX(), plotArea.getY() - getHeight() * .025, plotArea.getWidth(), getHeight() * .025, 8, 8, true, false, false, false);
    g.fillPath(p);
    
    
    g.setColour(Colour(0xFF36FEE2));
    
    p.clear();
    p.addRoundedRectangle(start, plotArea.getY() - getHeight() * .025, width, getHeight() * .025, 8, 8, currentBeat == 0, false, false ,false);
    
    g.fillPath(p);
    
}

void SequencerComponent::resized()
{
    double subdivision = sequencer.getSubDivision();
    
    plotArea = getLocalBounds().removeFromRight(getWidth() * widthPerc);
    plotArea = plotArea.removeFromBottom(getHeight() * .85);
    
    float xDist = floor((float)plotArea.getWidth() / (subdivision));
    float yDist = (float)plotArea.getHeight() / NUM_VOICES;
    float diameter = fmin(xDist, yDist);
    
    Rectangle<int> area(plotArea.getX(), plotArea.getY(), xDist, yDist);
    for (int i = 0; i < NUM_VOICES; i++) {
        for (int j = 0; j < 32; j++) {
            sequencerButtons[i][j]->setBounds(area.withSizeKeepingCentre(diameter * .5, diameter * .5));
            sequencerButtons[i][j]->setVisible(j < sequencer.getSubDivision());
            area.translate(xDist, 0);
        }
        area.setX(plotArea.getX());
        area.translate(0, yDist);
    }
    
    area.setBounds(0, plotArea.getY(), getWidth() * .1, yDist);
    
    for (int i = 0; i < labels.size(); i++) {
        labels[i]->setBounds(area.withSizeKeepingCentre(area.getWidth(), area.getHeight() * .5));
        channelCombos[i].setBounds(area.translated(area.getWidth(), 0).withSizeKeepingCentre(area.getWidth() * .85, area.getHeight() * .5));
        area.translate(0, yDist);
    }
    
    const int numSections = 4;
    const int numBeats = int(sequencer.getSubDivision());
    const int numBeatsPerSection = numBeats / numSections;
    int barNum = 0;
    barLines[barNum] = Rectangle<int>(plotArea.getX(),  plotArea.getY(), 2, getHeight());
    barNum++;
    for (int i = numBeatsPerSection - 1; i < numBeats - numBeatsPerSection; i+=numBeatsPerSection) {
        int right = sequencerButtons[0][i + 1]->getX();
        int left = sequencerButtons[0][i]->getRight();
        int xPos = right + ((left - right) / 2);
        barLines[barNum] = {int(xPos - 1), (int)(plotArea.getY() - getHeight() * .025), 2, getHeight()};
        barNum++;
    }
    
    barButtonGroupArea = Rectangle<int>(plotArea.getX(), 0, plotArea.getWidth(), getHeight() * .15);
    barButtonGroupArea = barButtonGroupArea.withSizeKeepingCentre(plotArea.getWidth() * .33, plotArea.getHeight() * .05);
    barButtonGroupArea.translate(0, -barButtonGroupArea.getHeight() / 2);
    
    Rectangle<int> barButtonArea(barButtonGroupArea.getX(), barButtonGroupArea.getY(), barButtonGroupArea.getWidth() / 4, barButtonGroupArea.getHeight());
    for (int i = 0; i < barButtons.size(); i++) {
        barButtons[i]->setBounds(barButtonArea.withSizeKeepingCentre(barButtonArea.getWidth() * .85, barButtonArea.getHeight()));
        barButtonArea.translate(barButtonArea.getWidth(), 0);
    }
    
    int subDivisionWidth = barButtonGroupArea.getWidth() / 3;
    auto subDivisionArea = barButtonArea.withWidth(subDivisionWidth).withX(getWidth() - getWidth() * .05 - subDivisionWidth);
    subdivisionCombo.setBounds(subDivisionArea);
    
    beatCountLabel.setBounds(barButtonGroupArea.translated(0, barButtonGroupArea.getHeight()).withSizeKeepingCentre(barButtonGroupArea.getWidth(), barButtonGroupArea.getHeight()));
    
    arrangeLabel.setBounds(Rectangle<int>(0, 0, plotArea.getX(), getHeight() * .15).withSizeKeepingCentre(plotArea.getX(), getHeight() * .05).withY(barButtonArea.getY()));
    
    
    
    instLabel.setBounds(labels[0]->getBounds().translated(0, -labels[0]->getHeight()).withHeight(labels[0]->getHeight()));
    
    channelLabel.setBounds(instLabel.getBounds().translated(instLabel.getWidth(), 0));
    
    repaint();
    
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
    
    previousBeat = currentBeat;
    currentBeat = sequencer.currentDownbeat;
    if (previousBeat != currentBeat) repaint();
}

void SequencerComponent::buttonClicked(Button* b)
{
    SequencerButton* button = dynamic_cast<SequencerButton*>(b);
    if (button != nullptr) {
        int pitch = button->getPitch();
        double beat = button->getBeat();
        //        std::cout << pitch << "," << beat << std::endl;
        NoteSequence* sequence = sequencer.getNoteSequence(currentSequence);
        if (!sequence->checkAndRemoveNote(pitch, beat))
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
            sequencerButtons[i][j]->setToggleState(sequencer.getNoteSequence(currentSequence)->isNotePresent(NoteSequence::noteValues[i], beat), false);
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
            sequencerButtons[i][j]->setToggleState(sequencer.getNoteSequence(currentSequence)->isNotePresent(NoteSequence::noteValues[i], beat), false);
        }
    }
    resized();
    
}

void SequencerComponent::comboBoxChanged(juce::ComboBox *combo) {
    for (int i = 0; i < channelCombos.size(); i++) {
        if (combo == &channelCombos[i]) {
            sequencer.setChannel(NoteSequence::noteValues[i], combo->getSelectedId());
        }
    }
}

void SequencerComponent::handleNewBarSelection(int newSequence) {
    barButtons[currentSequence]->setSelected(false);
    barButtons[newSequence]->setSelected(true);
    currentSequence = newSequence;
    setValuesFromPlugin();
}

void SequencerComponent::handleNewBarActive(int s) {
    sequencer.setSequenceActive(s, !sequencer.getSequenceActive(s));
}




