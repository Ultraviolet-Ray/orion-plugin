/*
  ==============================================================================

    OrionInstrumentView.h
    Created: 16 Aug 2020 12:41:18pm
    Author:  Lei Li

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalCoefficients.h"
//#include "DragAndDropButton.h"
//#include "OrionGlobalVars.h"


class OrionInstrumentView : public Component, Timer
{
public:
    OrionInstrumentView();//OrionaudioAudioProcessorEditor*
    ~OrionInstrumentView();
    void paint (juce::Graphics&) override;
    void resized() override;
    
    virtual void timerCallback() override;
    void startAnimation();
    
    // Pads
    //std::array<std::unique_ptr<DragAndDropButton>, 8> drumButtons;
    std::array<std::unique_ptr<DrawableImage>, 8> drumButtonCoverImageViews;
    
    Image downImages[8];
    Image upImages[8];

        
private:
    //OrionaudioAudioProcessorEditor* editor;
    
    bool *instrumentOnArray;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OrionInstrumentView)

};
