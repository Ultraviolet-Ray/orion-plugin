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
#define NUM_VOICES 6
//==============================================================================
/*
*/
class SequencerComponent : public Component, public Timer, public Button::Listener, public Sequencer::Listener {
public:
    
    
    struct LookAndFeelMethods
    {
    public:
        virtual void drawNoteBox(Graphics &g, SequencerComponent& s, Rectangle<float> bounds, bool isActive, bool isPlaying)
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

    
    void paint (Graphics&) override;
    void paintRows(Graphics&);
    void paintCols(Graphics&);

    void resized() override;
    
    virtual void colourChanged() override;
    
    void mouseUp(const MouseEvent& e) override;
    
    void timerCallback() override;
    
    // button listener
    void buttonClicked(Button* b) override;
    void buttonStateChanged(Button* b) override;
    
    bool isSelected = true;
    
    
    void notePlayed(int part, int beat) override;
   
    virtual void sequenceChanged() override { repaint(); }
    virtual void sequenceLengthChanged(int newLength) override;


    void addListener(Sequencer::Listener* listener) { listeners.push_back(listener); }
    
    void handleButtonPress(int pitch, int beat, bool buttonState);
    
    void setSequenceLength(int newLength);
    int getSequenceLength() { return sequenceLength; }
    
    void setSizeWithOverflow(int height);

    void setSubDivision(NoteSequence::SubDivision s);

private:
    Sequencer &sequencer;
    std::unique_ptr<Slider> lengthSlider;
    std::vector<Note> notesToBePlayed;
    
    std::array<std::array<std::unique_ptr<SequencerButton>, 32>, NUM_VOICES> sequencerButtons;
    
    int selectedRow = 0;
    int lastBeat = -1;
    bool shouldFlip = false;
    
    int sequenceLength = 16;
    
    StringArray voiceNames;
    
    float gridPhase = 0;
    float gridDirection = 1;
    std::vector<Sequencer::Listener*> listeners;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SequencerComponent)
};
