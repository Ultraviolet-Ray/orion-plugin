/*
  ==============================================================================

    ThresholdMeter.h
    Created: 8 Jul 2020 5:48:05pm
    Author:  Lei Li

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
//#include <JuceHeader.h>

//==============================================================================
/*
*/
class ThresholdMeter  : public Component, public Timer
{
public:
    ThresholdMeter();
    ~ThresholdMeter() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    //--------------------------------------
    void timerCallback() override;
    std::function<float()> updaterFunction;
    
    void pointerMove(float value);
    
private:
    
    Colour volumeColor;
    Colour overdriveColor;
    std::unique_ptr<DrawablePath> pointerPath;
    std::unique_ptr<DrawablePath> decoratePathL;
    std::unique_ptr<DrawablePath> decoratePathR;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThresholdMeter)
};
