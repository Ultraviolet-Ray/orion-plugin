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

void SequencerButton::paintButton(Graphics &g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    Rectangle<float> buttonArea = getLocalBounds().toFloat();
    float diameter = std::min(buttonArea.getWidth(), buttonArea.getHeight());
    buttonArea.setSize(diameter, diameter);
    diameter = 10;

    Colour colour;
    if (getToggleState() && frameCount == 0)
    {
        g.setColour(Colours::white);
        g.fillEllipse(buttonArea.withSizeKeepingCentre(diameter - 2, diameter - 2));
    }

    else if (!getToggleState())
    {
        g.setColour(Colours::darkgrey);
        g.fillEllipse(buttonArea.withSizeKeepingCentre(diameter - 2, diameter - 2));
    }

    else if (frameCount != 0)
    {
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
