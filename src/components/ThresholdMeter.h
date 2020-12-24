/*
  ==============================================================================

    ThresholdMeter.h
    Created: 8 Jul 2020 5:48:05pm
    Author:  Lei Li

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalCoefficients.h"


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
    
    void meterUpdate();
    
    //int serial = -1;
    
private:
    
    Colour volumeColor;
    Colour overdriveColor;
    std::unique_ptr<DrawablePath> pointerPath;
    std::unique_ptr<DrawablePath> decoratePathL;
    std::unique_ptr<DrawablePath> decoratePathR;
    
    std::unique_ptr<DrawablePath> meterPath;
    
    Rectangle<int> area = getLocalBounds();
    
    bool isVertical = false;
    bool meterUpdated = false;
    
    int count = 1;
    float preLevel = -1.0;
    float level = -1.0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThresholdMeter)
};
