/*
  ==============================================================================

    DoubleThumbSlider.h
    Created: 15 Sep 2020 12:29:24pm
    Author:  Lei Li

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalCoefficients.h"
#include "ClipMeter.h"
#include "PluginProcessor.h"
class DoubleThumbSlider  : public Component
{
    public:
    DoubleThumbSlider(ClipMeter*,OrionaudioAudioProcessor& p);
    ~DoubleThumbSlider() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void mouseDown (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;

    void audioRangeChange();
    
    void valueChange();
    
    void initThumb(float* LVal, float* RVal);
    
    //float LVal = 0.0f;
    //float RVal = 1.0f;
    
    Value LValue;
    Value RValue;
    
    private:
    OrionaudioAudioProcessor& processor;
    ClipMeter* meter;


    Colour thumbColor;
    
    float TLX = 0.0f;
    float TRX = getWidth();
    
    
    
    float* LValPtr = nullptr;
    float* RValPtr = nullptr;
    
    
    bool TLDown = false;
    bool TRDown = false;


    float ellipseDiameter = 2.5f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DoubleThumbSlider)
    
};
