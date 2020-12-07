/*
  ==============================================================================

    SequencerComponent.h
    Created: 8 Nov 2019 1:53:21am
    Author:  Quin Scacheri

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Sequencer.h"
#include "SequencerButton.h"
#include "BarButton.h"

#define NUM_VOICES 8
//==============================================================================
/*
*/

const float widthPerc = .8;

class SequencerComponent : public Component, public Timer, public Button::Listener, public Sequencer::Listener, public ComboBox::Listener
{
public:
    struct LookAndFeelMethods
    {
    public:
        virtual void drawNoteBox(Graphics &g, SequencerComponent &s, Rectangle<float> bounds, bool isActive, bool isPlaying)
        {
            if (isActive)
                g.drawRect(bounds);
            else
                g.fillRect(bounds);
        }
    };

    enum ColourIds : int
    {
        backgroundColourId = 1,
        dividerColourId,
        beatColourOffId,
        beatColourOnId,
        playheadColourId
    };

    SequencerComponent(Sequencer &s);
    ~SequencerComponent();

    void comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged) override;

    void paint(Graphics &) override;
    void paintRows(Graphics &);
    void paintCols(Graphics &);
    void paintBar(Graphics &);

    void resized() override;

    virtual void colourChanged() override;

    void mouseUp(const MouseEvent &e) override;

    void timerCallback() override;

    // button listener
    void buttonClicked(Button *b) override;
    void buttonStateChanged(Button *b) override;

    bool isSelected = true;

    void notePlayed(int part, int beat) override;

    virtual void sequenceChanged() override { repaint(); }
    virtual void sequenceLengthChanged(int newLength) override;

    void addListener(Sequencer::Listener *listener) { listeners.push_back(listener); }

    void handleButtonPress(int pitch, int beat, bool buttonState);

    void setSequenceLength(int newLength);
    int getSequenceLength() { return sequenceLength; }

    void setSizeWithOverflow(int height);

    void setSubDivision(NoteSequence::SubDivision s);

    void setValuesFromPlugin();

    void handleNewBarSelection(int);

    void handleNewBarActive(int);

private:
    Label arrangeLabel;
    Label notesLabel;
    Label barCountLabel;
    Label beatCountLabel;
    Label instLabel;
    Label channelLabel;

    ComboBox subdivisionCombo;

    std::array<Rectangle<int>, 4> barLines;
    std::array<ComboBox, NUM_VOICES> channelCombos;
    std::array<std::array<std::unique_ptr<SequencerButton>, 32>, NUM_VOICES> sequencerButtons;
    std::array<std::unique_ptr<Label>, NUM_VOICES> labels;
    std::array<std::unique_ptr<BarButton>, 4> barButtons;

    Rectangle<int> plotArea;
    Rectangle<int> playheadArea;
    Rectangle<int> barArea;
    Rectangle<int> barButtonGroupArea;
    int currentSequence = 0;

    Sequencer &sequencer;
    std::unique_ptr<Slider> lengthSlider;
    std::vector<Note> notesToBePlayed;
    int currentBeat = 0;
    int previousBeat = 0;
    int selectedRow = 0;
    int lastBeat = -1;
    bool shouldFlip = false;

    int sequenceLength = 16;

    StringArray voiceNames;

    float gridPhase = 0;
    float gridDirection = 1;
    std::vector<Sequencer::Listener *> listeners;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequencerComponent)
};
