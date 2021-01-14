/*
  ==============================================================================

    OrionClipConfiguration.h
    Created: 21 Jun 2019 1:10:50pm
    Author:  Lei Li

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalCoefficients.h"
#include "PluginProcessor.h"
#include "ClipMeter.h"
#include "ImagerMeter.h"
#include "DoubleThumbSlider.h"

#include <rubberband/RubberBandStretcher.h> //rub!!!


//==============================================================================
class OrionClipConfiguration    : public Component,
                                  public Slider::Listener
{
public:
    OrionClipConfiguration(OrionaudioAudioProcessor& p);
    ~OrionClipConfiguration();
    
    void paint(Graphics& g) override;
    void resized() override;

    void setClipOnOffSwitchImage(bool On);
    void clipOnOffSwitchClicked(bool isDown);
    
    void setImagerOnOffSwitchImage(bool On);
    void imagerOnOffSwitchClicked(bool isDown);
    
    void setReverseSwitchImage(bool On);
    void reverseSwitchClicked(bool isDown);
    
    void setimagerSwitchImage(bool On);
    void imagerSwitchClicked(bool isDown);
    void sliderValueChanged (Slider* slider)override;
    void sliderDragEnded (Slider* slider)override;

    void setImagerMode(bool inputVal){imagerMeter->setImagerMode(inputVal);}
    void setReverseMode(bool inputVal){clipMeter->setReverseMode(inputVal);}
    void clipSaturationValueChange(double value){saturationKnob->setValue(value,sendNotificationSync);};
    void clipSaturationFreqValueChange(double value){saturationLPFKnob->setValue(value,sendNotificationSync);};
    
    void clipPitchKnobValueChange(double value){pitchKnob->setValue(value,sendNotificationSync);};
    void clipFineTuneKnobValueChange(double value){fineTuneKnob->setValue(value,sendNotificationSync);};
    void clipStretchSpeedKnobValueChange(double value){stretchSpeedKnob->setValue(value,sendNotificationSync);};
    void clipStretchUpdate(double value);
    
    void clipImagerSliderValueChange(double value)
    {
        clipImagerSlider->setValue(value,sendNotificationSync);
    };
    
    void clipImagerSharpSliderValueChange(double value)
    {
        clipImagerSharpSlider->setValue(value,sendNotificationSync);
    };
    
    void refreashClipMeter(){clipMeter->repaint();};
    
    void refreashImagerMeter(){imagerMeter->repaint();};
    void initSEPointSlider(float* s, float* n)
    {
        SEPointSlider->initThumb(s, n);
        clipMeter->updateZone();
    };
    
private:
    OrionaudioAudioProcessor& processor;
    
    std::unique_ptr<RubberBand::RubberBandStretcher> rb;//rub!!!
    AudioBuffer<float> mTempBuffer;//rub!!!
    
    std::unique_ptr<soundtouch::SoundTouch> soundTouch;
    
    /* Images */
    
    Image switchOn = ImageCache::getFromMemory(BinaryData::PowerOn_png, BinaryData::PowerOn_pngSize);
    Image switchOff = ImageCache::getFromMemory(BinaryData::PowerOff_png, BinaryData::PowerOff_pngSize);
    
    Image modeSwitchOn = ImageCache::getFromMemory(BinaryData::ModeSwitchOn_png, BinaryData::ModeSwitchOn_pngSize);
    Image modeSwitchOff = ImageCache::getFromMemory(BinaryData::ModeSwitchOff_png, BinaryData::ModeSwitchOff_pngSize);
    
    Image reverseSwitchOn = ImageCache::getFromMemory(BinaryData::RectSwitchOn_png, BinaryData::RectSwitchOn_pngSize);
    Image reverseSwitchOff = ImageCache::getFromMemory(BinaryData::RectSwitchOff_png, BinaryData::RectSwitchOff_pngSize);
    
    std::unique_ptr<DrawableImage> backgroundImageView;
    
    
    
    /* Decorations */
    
    std::unique_ptr<DrawablePath> horizontalKnobGroupDecorationPath;
    std::unique_ptr<DrawablePath> verticalKnobGroupDecorationPath;
    
    /* On Off Switches */
    
    std::unique_ptr<ImageButton> clipOnOffSwitch;
    std::unique_ptr<ImageButton> imagerOnOffSwitch;
    std::unique_ptr<ImageButton> reverseSwitch;
    std::unique_ptr<ImageButton> imagerModeSwitch;
    
    /* Clip Meter */
    std::unique_ptr<ClipMeter> clipMeter;
    std::unique_ptr<DoubleThumbSlider> SEPointSlider;
    std::unique_ptr<Label> reverseLabel;

    /* Clip EFX */
    std::unique_ptr<Slider> saturationKnob;
    std::unique_ptr<Slider> saturationLPFKnob;
    std::unique_ptr<Slider> pitchKnob;
    std::unique_ptr<Slider> fineTuneKnob;
    std::unique_ptr<Slider> stretchSpeedKnob;
    
    std::unique_ptr<Label> saturationLabel;
    std::unique_ptr<Label> saturationLPFLabel;
    std::unique_ptr<Label> pitchLabel;
    std::unique_ptr<Label> fineTuneLabel;
    std::unique_ptr<Label> stretchSpeedLabel;
    
    
    std::unique_ptr<Label> saturationNameLabel;
    std::unique_ptr<Label> saturationRatioTitleLabel;
    std::unique_ptr<Label> saturationLPFTitleLabel;
    std::unique_ptr<Label> pitchTitleLabel;
    std::unique_ptr<Label> fineTuneTitleLabel;
    std::unique_ptr<Label> stretchTitleLabel;

    
    
    
    /* Imager */
    std::unique_ptr<ImagerMeter> imagerMeter;
    std::unique_ptr<Slider> clipImagerSlider;
    std::unique_ptr<Slider> clipImagerSharpSlider;
    std::unique_ptr<Slider> clipImagerRightSlider;

    std::unique_ptr<Label> imagerLabel;
    std::unique_ptr<Label> imagerMLabel;
    std::unique_ptr<Label> imagerSLabel;
    std::unique_ptr<Label> imagerCenterLabel;
    
    

//    std::unique_ptr<Slider> clipImagerSlider;
//    std::unique_ptr<Slider> clipImagerSlider;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OrionClipConfiguration)
};
