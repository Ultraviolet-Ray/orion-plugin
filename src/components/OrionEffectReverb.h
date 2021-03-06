/*
   ==============================================================================

    OrionEffectReverb.h
    Created: 28 Jun 2019 10:45:30am
    Author:  Lei Li

   ==============================================================================
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "OrionRotarySlider.h"
typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

class OrionEffectReverb : public Component,
        public Slider::Listener
{
public:
OrionEffectReverb(OrionaudioAudioProcessor&, int serial);
~OrionEffectReverb();

void paint (Graphics&) override;
void resized() override;

void sliderValueChanged (Slider* slider) override;
int effectReverbSerial;

//void componentMovedOrResized (Component& component, bool /*wasMoved*/, bool /*wasResized*/) override;

private:

OrionaudioAudioProcessor& processor;
    

std::unique_ptr<SliderAttachment> reverbPredelayTree;
std::unique_ptr<SliderAttachment> reverbSizeTree;
std::unique_ptr<SliderAttachment> reverbColorTree;
std::unique_ptr<SliderAttachment> reverbDecayTree;
std::unique_ptr<SliderAttachment> reverbDryTree;
std::unique_ptr<ButtonAttachment> reverbSwitchButtonAttachment;

//ScopedPointer<Slider> delayTimeSlider;

Slider reverbPredelaySlider;
Slider reverbSizeSlider;
Slider reverbColorSlider;
Slider reverbDecaySlider;
Slider reverbDrySlider;

ToggleButton reverbSwitchButton;

Label reverbPredelayLabel;
Label reverbSizeLabel;
Label reverbColorLabel;
Label reverbDecayLabel;
Label reverbDryLabel;
Label reverbWetLabel;

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OrionEffectReverb)
};
