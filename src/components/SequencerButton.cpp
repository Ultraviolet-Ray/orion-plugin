/*
  ==============================================================================

    SequencerButton.cpp
    Created: 16 Jun 2020 8:49:02pm
    Author:  Quin Scacheri

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SequencerButton.h"

//==============================================================================
SequencerButton::SequencerButton(int p, double b) : ToggleButton(), pitch(p), beat(b)
{
    setColour(SequencerButton::ColourIds::borderColourOn, Colours::white);
    setColour(SequencerButton::ColourIds::borderColourOff, Colours::white);
    setColour(SequencerButton::ColourIds::fillColourOn, Colours::white);
    setColour(SequencerButton::ColourIds::fillColourOff, Colours::transparentBlack);
}

SequencerButton::~SequencerButton()
{
    
}

void SequencerButton::paintButton (Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    Rectangle<float> buttonArea = getLocalBounds().toFloat();
    float diameter = std::min(buttonArea.getWidth(), buttonArea.getHeight());
    buttonArea.setSize(diameter, diameter);
    
    Colour colour;
    if (getToggleState() && frameCount == 0) {
        g.setColour(findColour(ColourIds::fillColourOn));
        g.fillEllipse(buttonArea.withSizeKeepingCentre(diameter - 2, diameter - 2));
        g.setColour(findColour(ColourIds::borderColourOn));
        g.drawEllipse(buttonArea.withSizeKeepingCentre(diameter - 2, diameter - 2), 1.f);
    }
    
    else if (!getToggleState()) {
        g.setColour(findColour(ColourIds::fillColourOff));
        g.fillEllipse(buttonArea.withSizeKeepingCentre(diameter - 2, diameter - 2));
        g.setColour(findColour(ColourIds::borderColourOff));
        g.drawEllipse(buttonArea.withSizeKeepingCentre(diameter - 2, diameter - 2), 1.f);
    }
    
    else if (frameCount != 0) {
        float normalizedCount = (frameCount) / (1.f * ANIMATION_LENGTH);
        float ratio = 1.f - normalizedCount;
        colour = Colour(0xff00FEE2).interpolatedWith(Colours::white, ratio);
        g.setColour(colour);
        g.fillEllipse(buttonArea.withSizeKeepingCentre(diameter - 2, diameter - 2));
    }

}

void SequencerButton::timerCallback()
{
    frameCount += 1;
    if (frameCount >= ANIMATION_LENGTH)
    {
        frameCount = 0;
        stopTimer();
    }
    repaint();
}

void SequencerButton::startAnimation()
{
    startTimerHz(30);
}


