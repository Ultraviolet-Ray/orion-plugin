/*
  ==============================================================================

    OrionClipConfiguration.cpp
    Created: 21 Jun 2019 1:10:50pm
    Author:  Lei Li

  ==============================================================================
*/

#include "OrionClipConfiguration.h"
#include "main.cpp"
//==============================================================================
OrionClipConfiguration::OrionClipConfiguration(OrionaudioAudioProcessor& p) : processor(p)
{
    
    rb = std::make_unique<RubberBand::RubberBandStretcher>(globalSampleRate,instrumentOriginalSampleContainer[instrumetClickedSerial].getNumChannels(),RubberBand::RubberBandStretcher::DefaultOptions,1.0,1.0);//rub!!!
   rb->reset();
    
    
    // Background Image
    Image backgroundImage = ImageCache::getFromMemory(BinaryData::clipBackground_png, BinaryData::clipBackground_pngSize);
    backgroundImageView.reset(new DrawableImage());
    backgroundImageView->setImage(backgroundImage);
    addAndMakeVisible(backgroundImageView.get());
    
    
    
    
    
    //------------------------------------ Decoration Paths ------------------------------------//
    horizontalKnobGroupDecorationPath.reset(new DrawablePath());
    addAndMakeVisible(horizontalKnobGroupDecorationPath.get());
    horizontalKnobGroupDecorationPath->replaceColour(Colours::black,Colours::grey);
    
    
    verticalKnobGroupDecorationPath.reset(new DrawablePath());
    addAndMakeVisible(verticalKnobGroupDecorationPath.get());
    verticalKnobGroupDecorationPath->replaceColour(Colours::black,Colours::grey);
    
    
    //------------------------------------ Clip EFX Knobs ------------------------------------//
    
    //====== Title Labels
    
    // Saturation Name Label
    saturationNameLabel.reset(new Label("SATURATION", "SATURATION"));
    saturationNameLabel->setJustificationType(juce::Justification::horizontallyCentred);
    addAndMakeVisible(saturationNameLabel.get());
    saturationNameLabel->setAlpha(0.8);
    //saturationNameLabel->setColour(Label::outlineColourId, Colour(0xff635E5D));
    
    // Saturation Ratio Label
    saturationRatioTitleLabel.reset(new Label("Ratio", "Ratio"));
    saturationRatioTitleLabel->setJustificationType(juce::Justification::horizontallyCentred);
    addAndMakeVisible(saturationRatioTitleLabel.get());
    saturationRatioTitleLabel->setAlpha(0.8);
    //saturationRatioTitleLabel->setColour(Label::outlineColourId, Colour(0xff635E5D));
     
    // Saturation LPF Label
    saturationLPFTitleLabel.reset(new Label("LPF", "LPF"));
    saturationLPFTitleLabel->setJustificationType(juce::Justification::horizontallyCentred);
    addAndMakeVisible(saturationLPFTitleLabel.get());
    saturationLPFTitleLabel->setAlpha(0.8);
    //saturationLPFTitleLabel->setColour(Label::outlineColourId, Colour(0xff635E5D));
    
    // Pitch Title Label
    pitchTitleLabel.reset(new Label("Pitch", "Pitch"));
    pitchTitleLabel->setJustificationType(juce::Justification::horizontallyCentred);
    addAndMakeVisible(pitchTitleLabel.get());
    pitchTitleLabel->setAlpha(0.8);
    //pitchTitleLabel->setColour(Label::outlineColourId, Colour(0xff635E5D));
    
    // Fine Tune Title Label
    fineTuneTitleLabel.reset(new Label("Fine Tune", "Fine Tune"));
    fineTuneTitleLabel->setJustificationType(juce::Justification::horizontallyCentred);
    addAndMakeVisible(fineTuneTitleLabel.get());
    fineTuneTitleLabel->setAlpha(0.8);
    //fineTuneTitleLabel->setColour(Label::outlineColourId, Colour(0xff635E5D));

    // Stretch Title Label
    stretchTitleLabel.reset(new Label("Stretch", "Stretch"));
    stretchTitleLabel->setJustificationType(juce::Justification::horizontallyCentred);
    addAndMakeVisible(stretchTitleLabel.get());
    stretchTitleLabel->setAlpha(0.8);
    //stretchTitleLabel->setColour(Label::outlineColourId, Colour(0xff635E5D));
    
    //====== Knobs & Variable Labels
    
    // Saturation
    saturationKnob.reset(new Slider());
    saturationKnob->setSliderStyle(Slider::SliderStyle::Rotary);
    saturationKnob->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);//Hide Text
    saturationKnob->setRange(0.0f, 1.0f);
    saturationKnob->setValue(0.0f);
    saturationKnob->addListener(this);
    addAndMakeVisible(saturationKnob.get());
      
    saturationLabel.reset(new Label("0.0 %", "0.0 %"));
    saturationLabel->setJustificationType(juce::Justification::horizontallyCentred);
    addAndMakeVisible(saturationLabel.get());
    saturationLabel->setAlpha(0.8);
    
    saturationLPFKnob.reset(new Slider());
    saturationLPFKnob->setSliderStyle(Slider::SliderStyle::Rotary);
    saturationLPFKnob->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);//Hide Text
    saturationLPFKnob->setRange(20, 20000);
    saturationLPFKnob->setValue(800);
    saturationLPFKnob->addListener(this);
    addAndMakeVisible(saturationLPFKnob.get());
      
    saturationLPFLabel.reset(new Label("800.0 Hz", "800.0 Hz"));
    saturationLPFLabel->setJustificationType(juce::Justification::horizontallyCentred);
    addAndMakeVisible(saturationLPFLabel.get());
    saturationLPFLabel->setAlpha(0.8);
    
    // Pitch
    pitchKnob.reset(new Slider());
    pitchKnob->setSliderStyle(Slider::SliderStyle::Rotary);
    pitchKnob->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);//Hide Text
    pitchKnob->setRange(-36, 36);
    pitchKnob->setValue(0);
    pitchKnob->addListener(this);
    addAndMakeVisible(pitchKnob.get());
      
    pitchLabel.reset(new Label("0 tune", "0 tune"));
    pitchLabel->setJustificationType(juce::Justification::horizontallyCentred);
    addAndMakeVisible(pitchLabel.get());
    pitchLabel->setAlpha(0.8);
    
    // Fine Tune
    fineTuneKnob.reset(new Slider());
    fineTuneKnob->setSliderStyle(Slider::SliderStyle::Rotary);
    fineTuneKnob->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);//Hide Text
    fineTuneKnob->setRange(-1.0, 1.0f);
    fineTuneKnob->setValue(0);
    fineTuneKnob->addListener(this);
    addAndMakeVisible(fineTuneKnob.get());
      
    fineTuneLabel.reset(new Label("0 cent", "0 cent"));
    fineTuneLabel->setJustificationType(juce::Justification::horizontallyCentred);
    addAndMakeVisible(fineTuneLabel.get());
    fineTuneLabel->setAlpha(0.8);
    
    // Stretch
    stretchSpeedKnob.reset(new Slider());
    stretchSpeedKnob->setSliderStyle(Slider::SliderStyle::Rotary);
    stretchSpeedKnob->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);//Hide Text
    stretchSpeedKnob->setRange(0.01, 7.00f);
    stretchSpeedKnob->setValue(1.00);
    stretchSpeedKnob->addListener(this);
    addAndMakeVisible(stretchSpeedKnob.get());
      
    stretchSpeedLabel.reset(new Label("x1.00", "x1.00"));
    stretchSpeedLabel->setJustificationType(juce::Justification::horizontallyCentred);
    addAndMakeVisible(stretchSpeedLabel.get());
    stretchSpeedLabel->setAlpha(0.8);
    
    
    
    
    // Reverse Label
    reverseLabel.reset(new Label("Reverse", "Reverse"));
    reverseLabel->setJustificationType(juce::Justification::left);
    addAndMakeVisible(reverseLabel.get());
    reverseLabel->setAlpha(0.5);
    
    
    
 
    //------------------------------------ Meters ------------------------------------//
    // Clip Meter
    clipMeter.reset(new ClipMeter());
    addAndMakeVisible(clipMeter.get());

    // Imager Meter
    imagerMeter.reset(new ImagerMeter());
    addAndMakeVisible(imagerMeter.get());
    
    //------------------------------------ Imager ------------------------------------//
    
    
    // Reverse Label
    reverseLabel.reset(new Label("Reverse", "Reverse"));
    reverseLabel->setJustificationType(juce::Justification::horizontallyCentred);
    addAndMakeVisible(reverseLabel.get());
    reverseLabel->setAlpha(0.5);
    
    // Imager Label
    imagerLabel.reset(new Label("Stereo Imager", "Stereo Imager"));
    imagerLabel->setJustificationType(juce::Justification::left);
    addAndMakeVisible(imagerLabel.get());
    imagerLabel->setAlpha(0.5);
    
    // Imager M Label
    imagerMLabel.reset(new Label("M", "M"));
    imagerMLabel->setJustificationType(juce::Justification::left);
    addAndMakeVisible(imagerMLabel.get());
    imagerMLabel->setAlpha(0.5);
    
    
    // Imager S Label
    imagerSLabel.reset(new Label("S", "S"));
    imagerSLabel->setJustificationType(juce::Justification::left);
    addAndMakeVisible(imagerSLabel.get());
    imagerSLabel->setAlpha(0.5);
    
    // Imager Center Label
    //imagerCenterLabel.reset(new Label("0", "0"));
    //imagerCenterLabel->setJustificationType(juce::Justification::left);
    //addAndMakeVisible(imagerCenterLabel.get());
    //imagerCenterLabel->setAlpha(0.5);
    //imagerCenterLabel->setVisible(false);//-!!!!!!!!!!
    
    
    // Imager Slider
    clipImagerSlider.reset(new Slider());
    clipImagerSlider->setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    clipImagerSlider->setColour(Slider::backgroundColourId, juce::Colours::grey);
    clipImagerSlider->setColour(Slider::trackColourId, juce::Colours::grey);
    clipImagerSlider->setColour(Slider::thumbColourId, juce::Colours::white);
    clipImagerSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);//Hide Text
    clipImagerSlider->setRange(0, 1);
    clipImagerSlider->setValue(0.5f);
    clipImagerSlider->addListener(this);
    addAndMakeVisible(clipImagerSlider.get());
    
    // clipImagerSharpSlider
    clipImagerSharpSlider.reset(new Slider());
    clipImagerSharpSlider->setSliderStyle(Slider::SliderStyle::Rotary);
    clipImagerSharpSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);//Hide Text
    clipImagerSharpSlider->setRange(0.0075f, 0.1f);
    clipImagerSharpSlider->setValue(0.0075f);
    clipImagerSharpSlider->addListener(this);
    addAndMakeVisible(clipImagerSharpSlider.get());
    
    // clipImagerRightSlider
    clipImagerRightSlider.reset(new Slider());
    clipImagerRightSlider->setSliderStyle(Slider::SliderStyle::Rotary);
    clipImagerRightSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);//Hide Text
    clipImagerRightSlider->setRange(0.0075f, 0.1f);
    clipImagerRightSlider->setValue(0.0075f);
    clipImagerRightSlider->addListener(this);
    addAndMakeVisible(clipImagerRightSlider.get());
    
    
    
    //------------------------------------ Switches ------------------------------------//
    // Clip On Off Switch
    clipOnOffSwitch.reset(new ImageButton());
    clipOnOffSwitch->setImages(false, true, true,
                               switchOff, 1.f, Colours::transparentBlack, switchOff, 1.f,
                               Colours::transparentBlack, switchOff, 1.f, Colours::transparentBlack);
    clipOnOffSwitch->onStateChange = [&]
    {
        clipOnOffSwitchClicked(clipOnOffSwitch->isDown());
    };
    addAndMakeVisible(clipOnOffSwitch.get());
    
    // Imager On Off Switch
    imagerOnOffSwitch.reset(new ImageButton());
    imagerOnOffSwitch->setImages(false, true, true,
                                 switchOff, 1.f, Colours::transparentBlack, switchOff, 1.f,
                                 Colours::transparentBlack, switchOff, 1.f, Colours::transparentBlack);
    imagerOnOffSwitch->onStateChange = [&]
    {
        imagerOnOffSwitchClicked(imagerOnOffSwitch->isDown());
    };
    addAndMakeVisible(imagerOnOffSwitch.get());
    
    // Reverse Switch
    reverseSwitch.reset(new ImageButton());
    reverseSwitch->setImages(false, true, true,
                          reverseSwitchOff, 1.f, Colours::transparentBlack, reverseSwitchOff, 1.f,
                          Colours::transparentBlack, reverseSwitchOff, 1.f, Colours::transparentBlack);
    reverseSwitch->onStateChange = [&]
    {
        reverseSwitchClicked(reverseSwitch->isDown());
    };
    addAndMakeVisible(reverseSwitch.get());
    
    // Imager Mode Switch
    imagerModeSwitch.reset(new ImageButton());
    imagerModeSwitch->setImages(false, true, true,
                          modeSwitchOff, 1.f, Colours::transparentBlack, modeSwitchOff, 1.f,
                          Colours::transparentBlack, modeSwitchOff, 1.f, Colours::transparentBlack);
    imagerModeSwitch->onStateChange = [&]
    {
        imagerSwitchClicked(imagerModeSwitch->isDown());
    };
    addAndMakeVisible(imagerModeSwitch.get());
    
    
    
    // SEPoint Slider
    SEPointSlider.reset(new DoubleThumbSlider(clipMeter.get(),processor));
    addAndMakeVisible(SEPointSlider.get());
    
   
    soundTouch.reset(new soundtouch::SoundTouch());
    soundTouch->setSampleRate(globalSampleRate);
    soundTouch->setChannels(2);
    soundTouch->setRate(1.0);
    soundTouch->setTempo(1.0);
    
}

void OrionClipConfiguration::paint (Graphics& g)
{
    //std::cout<<"paint Clip : "<<instrumetClickedSerial<<std::endl;
}

void OrionClipConfiguration::resized()
{
    Rectangle<int> area(0, 0, getWidth(), getHeight());
    backgroundImageView->setTransformToFit(area.toFloat(), RectanglePlacement::stretchToFit);
    
    float knobY1 = getWidth()/30;
    float knobY2 = getWidth()/8.25;
    float knobSize = getWidth()/25;
    
    float TextY1 =  getWidth()/13.25;
    float TextY2 =  getWidth()/6.00;
    float TextHeight =  getWidth()/60;
    
    float onOffSwitchSize = getWidth() * 0.0181;
    float switchSize = getWidth()/69;
    float reverseSwitchSize = getWidth()/77;
    
    // Clip On/Off Switch
    //area = Rectangle<int>(getWidth() * JUCE_LIVE_CONSTANT(0.0999), getHeight() * 0.020, onOffSwitchSize, onOffSwitchSize);
    area = Rectangle<int>(getWidth() * 0.0999, getHeight() * 0.020, onOffSwitchSize, onOffSwitchSize);
    clipOnOffSwitch->setBounds(area);

    // Imager On/Off Switch
    area = Rectangle<int>(getWidth() * 0.889, getHeight() * 0.640, onOffSwitchSize, onOffSwitchSize);
    imagerOnOffSwitch->setBounds(area);
    
    // Reverse Switch
    area = Rectangle<int>(getWidth() * 0.720, getHeight() * 0.740,  reverseSwitchSize, switchSize);
    reverseSwitch->setBounds(area);
    
    // Imager Mode Switch
    area = Rectangle<int>(getWidth() * 0.8925, getHeight() * 0.845,  switchSize, switchSize);
    imagerModeSwitch->setBounds(area);
    
    
    
    
    
    //------------------------------------ Decoration Paths ------------------------------------//
    
    
    // horizontal Knob Group Decoration Path
    
//    float PathY =  getHeight() * JUCE_LIVE_CONSTANT(0.47);
//    float verticalPathX =  getWidth() * JUCE_LIVE_CONSTANT(0.108);
//    float horizontalPathWidth =  getWidth() * JUCE_LIVE_CONSTANT(0.2168);
//    float verticalPathHeight =  getHeight() * JUCE_LIVE_CONSTANT(0.5);
    
    float PathY =  getHeight() * 0.47;
    float verticalPathX =  getWidth() * 0.108;
    float horizontalPathWidth =  getWidth() * 0.2168;
    float verticalPathHeight =  getHeight() * 0.5;
    
    Path path;
    path.addRectangle (0, PathY, horizontalPathWidth, 1);
    horizontalKnobGroupDecorationPath->setPath(path);
    horizontalKnobGroupDecorationPath->setAlpha(0.3);
    
    Path path2;
    path2.addRectangle (verticalPathX, PathY, 1, verticalPathHeight);
    verticalKnobGroupDecorationPath->setPath(path2);
    verticalKnobGroupDecorationPath->setAlpha(0.3);
    

//    Point<float> point3    = {static_cast<float>(verticalPathX),static_cast<float>(PathY)};
//
//    Point<float> point4    = {static_cast<float>(verticalPathX),static_cast<float>(getHeight())};
//
//    p.startNewSubPath(point3);
//    p.lineTo(point4);
//
//    verticalKnobGroupDecorationPath->setAlpha(0.3);
//    verticalKnobGroupDecorationPath->setPath(p);
    
 
    

    
    //------------------------------------ Knobs Name Label ------------------------------------//
    

//    float LabelWidth =  getWidth() * JUCE_LIVE_CONSTANT(0.069);
//    float LabelHeight =  getHeight()* JUCE_LIVE_CONSTANT(0.15);
    
    float LabelWidth =  getWidth() * 0.069;
    float LabelHeight =  getHeight()* 0.15;
    
    
    
    
//    float LabelX1 =  getWidth() * JUCE_LIVE_CONSTANT(0.021);
//    float LabelX2 =  getWidth() * JUCE_LIVE_CONSTANT(0.131);
//    float LabelY1 =  getHeight() * JUCE_LIVE_CONSTANT(0.031);
//    float LabelY2 =  getHeight() * JUCE_LIVE_CONSTANT(0.4725);
    
    float LabelX1 =  getWidth() * 0.021;
    float LabelX2 =  getWidth() * 0.131;
    float LabelY1 =  getHeight() * 0.031;
    float LabelY2 =  getHeight() * 0.4725;
    
    
    
//    float fineTuneTitleLabelX =  getWidth() * JUCE_LIVE_CONSTANT(0.22);
//    float fineTuneTitleLabelY =  getHeight() * JUCE_LIVE_CONSTANT(0.7);
    
//    float saturationNameLabelX =  getWidth() * JUCE_LIVE_CONSTANT(0.074);
//    float saturationNameLabelY =  getHeight() * JUCE_LIVE_CONSTANT(0.37);
    float saturationNameLabelX =  getWidth() * 0.074;
    float saturationNameLabelY =  getHeight() * 0.37;
    
    float fineTuneTitleLabelX =  getWidth() * 0.22;
    float fineTuneTitleLabelY =  getHeight() * 0.7;
    
   
//    float satuLabelWidth = getWidth() * JUCE_LIVE_CONSTANT(0.069);
//    float satuLabelHeight = getHeight() * JUCE_LIVE_CONSTANT(0.1);
    float satuLabelWidth = getWidth() * 0.069;
    float satuLabelHeight = getHeight() * 0.1;

    area = Rectangle<int>(saturationNameLabelX, saturationNameLabelY, satuLabelWidth, satuLabelHeight);
    //area = Rectangle<int>(saturationNameLabelX, saturationNameLabelY, LabelWidth * 1.070, LabelHeight);
    saturationNameLabel->setBounds(area);
    
    
    area = Rectangle<int>(LabelX1, LabelY1, LabelWidth, LabelHeight);
    saturationRatioTitleLabel->setBounds(area);
    
    area = Rectangle<int>(LabelX2, LabelY1, LabelWidth, LabelHeight);
    saturationLPFTitleLabel->setBounds(area);
    
    area = Rectangle<int>(LabelX1, LabelY2, LabelWidth, LabelHeight);
    pitchTitleLabel->setBounds(area);
    
    area = Rectangle<int>(LabelX2, LabelY2, LabelWidth, LabelHeight);
    fineTuneTitleLabel->setBounds(area);
    
    
    
    area = Rectangle<int>(fineTuneTitleLabelX, fineTuneTitleLabelY, LabelWidth, LabelHeight);
    stretchTitleLabel->setBounds(area);
    

    //------------------------------------ Knobs & Variable Label ------------------------------------//
    // Saturation
    area = Rectangle<int>(getWidth()/29, knobY1,  knobSize, knobSize);
    saturationKnob->setBounds(area);

    area = Rectangle<int>(getWidth()/29 - knobSize/2, TextY1,  2 * knobSize, TextHeight);
    saturationLabel->setBounds(area);
    
   
  
    // Decay
    area = Rectangle<int>(getWidth()/7, knobY1,  knobSize, knobSize);
    saturationLPFKnob->setBounds(area);

    area = Rectangle<int>(getWidth()/7 - knobSize/2, TextY1,  2 * knobSize, TextHeight);
    saturationLPFLabel->setBounds(area);
   
    // Hold
    area = Rectangle<int>(getWidth()/29, knobY2,  knobSize, knobSize);
    pitchKnob->setBounds(area);

    area = Rectangle<int>(getWidth()/29 - knobSize/2, TextY2,  2 * knobSize, TextHeight);
    pitchLabel->setBounds(area);
    
    // Fine Tune
    area = Rectangle<int>(getWidth()/7, knobY2,  knobSize, knobSize);
    fineTuneKnob->setBounds(area);

    area = Rectangle<int>(getWidth()/7 - knobSize/2, TextY2,  2 * knobSize, TextHeight);
    fineTuneLabel->setBounds(area);
    
    // Stretch
    
//    float stretchSpeedKnobX = getWidth() * JUCE_LIVE_CONSTANT(0.296);
//    float stretchSpeedKnobY = getHeight() * JUCE_LIVE_CONSTANT(0.65);
//    float stretchSpeedLabelX = getWidth() * JUCE_LIVE_CONSTANT(0.276);
//    float stretchSpeedLabelY = getHeight() * JUCE_LIVE_CONSTANT(0.84);
    float stretchSpeedKnobX = getWidth() * 0.296;
    float stretchSpeedKnobY = getHeight() * 0.65;
    float stretchSpeedLabelX = getWidth() * 0.276;
    float stretchSpeedLabelY = getHeight() * 0.825;
    
    area = Rectangle<int>(stretchSpeedKnobX, stretchSpeedKnobY,  knobSize, knobSize);
    stretchSpeedKnob->setBounds(area);

    area = Rectangle<int>(stretchSpeedLabelX, stretchSpeedLabelY,  2 * knobSize, TextHeight);
    stretchSpeedLabel->setBounds(area);
   
    
    
    //------------------------------------ Meters ------------------------------------//
    // Clip Meter
    area = Rectangle<int>(getWidth() * 0.2175, 0,  getWidth() * 0.580, getHeight() * 0.625);
    clipMeter->setBounds(area);
    
    
    // Imager Meter
    area = Rectangle<int>(getWidth() * 0.7975, 0,  getWidth() * 0.2025, getHeight() * 0.6225);
    imagerMeter->setBounds(area);
    
    
    // SEPoint Slider
    area = Rectangle<int>(getWidth() * 0.2129, getHeight() * 0.598,  getWidth() * 0.588, getHeight() * 0.0500);
    SEPointSlider->setBounds(area);
    
    //------------------------------------ Imager ------------------------------------//
    // Imager Slider
    float sliderHeight = getWidth() * 0.02;
    float sliderWidth = getWidth() * 0.1245;
    
    area = Rectangle<int>(getWidth() * 0.8365, getHeight() * 0.715,  sliderWidth, sliderHeight);
    clipImagerSlider->setBounds(area);
    
    // Imager Sharp Slider
    area = Rectangle<int>(getWidth() * 0.8000, getHeight() * 0.680,  knobSize, knobSize);
    clipImagerSharpSlider->setBounds(area);
    
    // Imager Right Slider
    area = Rectangle<int>(getWidth() * 0.9570, getHeight() * 0.680,  knobSize, knobSize);
    clipImagerRightSlider->setBounds(area);
    

    // Imager Labels
    float labelHeight = getWidth() * 0.0215;
    float labelWidth = getWidth() * 0.08;
    
    float RX = getWidth() * 0.715;
    float RY = getHeight() * 0.7125;
    
    float MX = getWidth() * 0.832;
    float SX = getWidth() * 0.949;
    float MSY = getHeight() * 0.615;
    
    float imagerLableX = getWidth() * 0.862;
    float imagerLabelY = getHeight() * 0.765;
    
    area = Rectangle<int>(RX, RY,  labelWidth, labelHeight * 1.05);
    reverseLabel->setBounds(area);
    
    area = Rectangle<int>(imagerLableX, imagerLabelY,  labelWidth, labelHeight * 1.05);
    imagerLabel->setBounds(area);
    
    area = Rectangle<int>(MX, MSY,  labelWidth, labelHeight);
    imagerMLabel->setBounds(area);
    
    area = Rectangle<int>(SX, MSY,  labelWidth, labelHeight);
    imagerSLabel->setBounds(area);
    
    //area = Rectangle<int>(MX + ((SX - MX)/2), MSY,  labelWidth, labelHeight);
    //imagerCenterLabel->setBounds(area);
    
}


//MARK:- Clip On/Off Switch
// Set Imager On/Off Switch  Image
void OrionClipConfiguration::setClipOnOffSwitchImage(bool On)
{
    if(On)
    {
        clipOnOffSwitch->setImages(false, true, true,
                                   switchOn, 1.f, Colours::transparentBlack, switchOn, 1.f,
                                   Colours::transparentBlack, switchOn, 1.f, Colours::transparentBlack);
    }
    else
    {
        clipOnOffSwitch->setImages(false, true, true,
                                   switchOff, 1.f, Colours::transparentBlack, switchOff, 1.f,
                                   Colours::transparentBlack, switchOff, 1.f, Colours::transparentBlack);
    }
}

// Imager On/Off Switch Clicked
void OrionClipConfiguration::clipOnOffSwitchClicked(bool isDown)
{
    if(isDown)
    {
        if(auto* voice = dynamic_cast<OrionSamplerVoice*> (processor.getSampler()->getVoice(instrumetClickedSerial)))
        {
            //voice->envswitch = !voice->envswitch;
            clipOnOffSwitches[instrumetClickedSerial] = !clipOnOffSwitches[instrumetClickedSerial];
            voice->clipOnOffSwitch = clipOnOffSwitches[instrumetClickedSerial];
            std::cout<<"instrumetClickedSerial: "<<instrumetClickedSerial<<std::endl;
            std::cout<<"voice->clipOnOffSwitch: "<<voice->clipOnOffSwitch<<std::endl;
            
            setClipOnOffSwitchImage(voice->clipOnOffSwitch);
        }
    }
}



//MARK:- Imager On/Off Switch
// Set Imager On/Off Switch  Image
void OrionClipConfiguration::setImagerOnOffSwitchImage(bool On)
{
    if(On)
    {
        imagerOnOffSwitch->setImages(false, true, true,
                                     switchOn, 1.f, Colours::transparentBlack, switchOn, 1.f,
                                     Colours::transparentBlack, switchOn, 1.f, Colours::transparentBlack);
    }
    else
    {
        imagerOnOffSwitch->setImages(false, true, true,
                                     switchOff, 1.f, Colours::transparentBlack, switchOff, 1.f,
                                     Colours::transparentBlack, switchOff, 1.f, Colours::transparentBlack);
    }
}

// Imager On/Off Switch Clicked
void OrionClipConfiguration::imagerOnOffSwitchClicked(bool isDown)
{
    if(isDown)
    {
        if(auto* voice = dynamic_cast<OrionSamplerVoice*> (processor.getSampler()->getVoice(instrumetClickedSerial)))
        {
            imagerOnOffSwitches[instrumetClickedSerial] = !imagerOnOffSwitches[instrumetClickedSerial];
            voice->imagerOnOffSwitch = imagerOnOffSwitches[instrumetClickedSerial];
            setImagerOnOffSwitchImage(voice->imagerOnOffSwitch);
        }
    }
}







//MARK:- Reverse Switch
// Set Reverse Switch Image
void OrionClipConfiguration::setReverseSwitchImage(bool On)
{
    if(On)
    {
        reverseSwitch->setImages(false, true, true,
        reverseSwitchOn, 1.f, Colours::transparentBlack, reverseSwitchOn, 1.f,
        Colours::transparentBlack, reverseSwitchOn, 1.f, Colours::transparentBlack);
    }
    else
    {
        reverseSwitch->setImages(false, true, true,
        reverseSwitchOff, 1.f, Colours::transparentBlack, reverseSwitchOff, 1.f,
        Colours::transparentBlack, reverseSwitchOff, 1.f, Colours::transparentBlack);
    }
}

// Reverse Switch Clicked
void OrionClipConfiguration::reverseSwitchClicked(bool isDown)
{
    if(isDown)
    {
        
        instrumentOriginalSampleContainer[instrumetClickedSerial].reverse(0, instrumentOriginalSampleContainer[instrumetClickedSerial].getNumSamples());
        instrumentSampleContainer[instrumetClickedSerial].reverse(0, instrumentSampleContainer[instrumetClickedSerial].getNumSamples());
        reverseSwitches[instrumetClickedSerial] = !reverseSwitches[instrumetClickedSerial];
        setReverseSwitchImage(reverseSwitches[instrumetClickedSerial]);
        clipMeter->setReverseMode(reverseSwitches[instrumetClickedSerial]);
        
        SEPointSlider->audioRangeChange();
    }
}


//MARK:- Switch Image Changer
void OrionClipConfiguration::setimagerSwitchImage(bool On)
{
    if(On)
    {
        imagerModeSwitch->setImages(false, true, true,
        modeSwitchOn, 1.f, Colours::transparentBlack, modeSwitchOn, 1.f,
        Colours::transparentBlack, modeSwitchOn, 1.f, Colours::transparentBlack);
    }
    else
    {
        imagerModeSwitch->setImages(false, true, true,
        modeSwitchOff, 1.f, Colours::transparentBlack, modeSwitchOff, 1.f,
        Colours::transparentBlack, modeSwitchOff, 1.f, Colours::transparentBlack);
    }
}

//MARK:- Switch Clicker
void OrionClipConfiguration::imagerSwitchClicked(bool isDown)
{
    if(isDown)
    {
        if(auto* voice = dynamic_cast<OrionSamplerVoice*> (processor.getSampler()->getVoice(instrumetClickedSerial)))
        {
            //voice->envswitch = !voice->envswitch;
            imagerModeSwitches[instrumetClickedSerial] = !imagerModeSwitches[instrumetClickedSerial];
            voice->clipImagerMode = imagerModeSwitches[instrumetClickedSerial];
            setimagerSwitchImage(voice->clipImagerMode);
            imagerMeter->setImagerMode(voice->clipImagerMode);
        }
    }
}

void OrionClipConfiguration::sliderValueChanged (Slider* slider)
{
    
    //------------------------------------ Clip EFX ------------------------------------//
    if(slider == saturationKnob.get())// Saturation Knob
    {
        
        clipSaturationCoefficient[instrumetClickedSerial] = saturationKnob->getValue();
        
        if(auto* voice = dynamic_cast<OrionSamplerVoice*>(processor.getSampler()->getVoice(instrumetClickedSerial)))
        {
            voice->setSaturationVal(clipSaturationCoefficient[instrumetClickedSerial]);
        }
        
        String value = String(int(clipSaturationCoefficient[instrumetClickedSerial]* 100) );
        value.append(" %",5);
        saturationLabel->setText(value,dontSendNotification);

    }
    else if(slider == saturationLPFKnob.get())// Saturation LPF Freq Knob
    {
        clipSaturationFreqCoefficient[instrumetClickedSerial] = saturationLPFKnob->getValue();
        if(auto* voice = dynamic_cast<OrionSamplerVoice*>(processor.getSampler()->getVoice(instrumetClickedSerial)))
        {
            voice->setSaturationLPFFreqVal(clipSaturationFreqCoefficient[instrumetClickedSerial]);
        }
        
        String value = String(int(clipSaturationFreqCoefficient[instrumetClickedSerial]));
        value.append(" Hz",5);
        saturationLPFLabel->setText(value,dontSendNotification);
    }
    else if(slider == pitchKnob.get())// Pitch Knob
    {
        clipPitchCoefficient[instrumetClickedSerial] = pitchKnob->getValue();
        if(auto* voice = dynamic_cast<OrionSamplerVoice*>(processor.getSampler()->getVoice(instrumetClickedSerial)))
        {
            voice->setPitchVal(clipPitchCoefficient[instrumetClickedSerial]);
        }
        
        String value = String(int(clipPitchCoefficient[instrumetClickedSerial]));
        value.append(" tune",5);
        pitchLabel->setText(value,dontSendNotification);
    }
    else if(slider == fineTuneKnob.get())// FineTune Knob
    {
        clipFineTuneCoefficient[instrumetClickedSerial] = fineTuneKnob->getValue();
        if(auto* voice = dynamic_cast<OrionSamplerVoice*>(processor.getSampler()->getVoice(instrumetClickedSerial)))
        {
            voice->setPitchCent(clipFineTuneCoefficient[instrumetClickedSerial]);
        }
        
        double tmp = jmap<double>(clipFineTuneCoefficient[instrumetClickedSerial], -1.0f, 1.0f, -12.0f, 12.0f);
        
        String value = String(int(tmp));
        value.append(" cent",5);
        fineTuneLabel->setText(value,dontSendNotification);
    }
    else if(slider == stretchSpeedKnob.get())// Stretch Speed Knob
    {
        clipStretchSpeedCoefficient[instrumetClickedSerial] = stretchSpeedKnob->getValue();
        
        String preVale = "x";
        
        String value = "";
        
        if(clipStretchSpeedCoefficient[instrumetClickedSerial] == 7)
        {
            value = "7.00";
        }
        else
        {
            value = String(clipStretchSpeedCoefficient[instrumetClickedSerial]);
        }
        preVale.append(value,4);
        stretchSpeedLabel->setText(preVale,dontSendNotification);
        //clipStretchUpdate(clipStretchSpeedCoefficient[instrumetClickedSerial]);
    }
    
    
    //------------------------------------ Imager ------------------------------------//
    if(slider == clipImagerSlider.get())// Imager Slider
    {
        imagerCoefficient[instrumetClickedSerial] = clipImagerSlider->getValue();
        
        if(auto* voice = dynamic_cast<OrionSamplerVoice*>(processor.getSampler()->getVoice(instrumetClickedSerial)))
        {
            voice->ImagerCoefChange(imagerCoefficient[instrumetClickedSerial]);
        }
         
        imagerMeter->setImagerCoef(imagerCoefficient[instrumetClickedSerial] );
    }
    else if(slider == clipImagerSharpSlider.get())// Imager Speed Slider
    {
        imagerSharpCoefficient[instrumetClickedSerial] = clipImagerSharpSlider->getValue();
        if(auto* voice = dynamic_cast<OrionSamplerVoice*>(processor.getSampler()->getVoice(instrumetClickedSerial)))
        {
            voice->imagerSharp = imagerSharpCoefficient[instrumetClickedSerial];
        }
        
        imagerMeter->updateImager();
        
    }
    
}

void OrionClipConfiguration::sliderDragEnded (Slider* slider)
{
    if(slider == stretchSpeedKnob.get())// Stretch Speed Knob
    {
        
        clipStretchSpeedCoefficient[instrumetClickedSerial] = stretchSpeedKnob->getValue();
        
//        if(auto* voice = dynamic_cast<OrionSamplerVoice*>(processor.getSampler()->getVoice(instrumetClickedSerial)))
//        {
//            voice->setPitchCent(clipStretchSpeedCoefficient[instrumetClickedSerial]);
//        }
        
        String preVale = "x";
        
        String value = "";
        
        if(clipStretchSpeedCoefficient[instrumetClickedSerial] == 7)
        {
            value = "7.00";
        }
        else
        {
            value = String(clipStretchSpeedCoefficient[instrumetClickedSerial]);
        }
        preVale.append(value,4);
        stretchSpeedLabel->setText(preVale,dontSendNotification);
        clipStretchUpdate(clipStretchSpeedCoefficient[instrumetClickedSerial]);

    }
}



void OrionClipConfiguration::clipStretchUpdate(double value)
{
    DBG(value);
    instrumentSampleContainer[instrumetClickedSerial] = instrumentOriginalSampleContainer[instrumetClickedSerial];
    
    soundTouch->setTempo(1);
    soundTouch->putSamples(instrumentSampleContainer[instrumetClickedSerial].getWritePointer(0), instrumentSampleContainer[instrumetClickedSerial].getNumSamples());
    
    soundTouch->setPitch(value);
    
    // to the original tempo (-50 .. +100 %)
    //soundTouch->setTempoChange(value * 10);
    
    //soundTouch->setTempo((value - 2) * 10);
    // Sets new tempo control value as a difference in percents compared
    soundTouch->receiveSamples(instrumentSampleContainer[instrumetClickedSerial].getWritePointer(0), instrumentSampleContainer[instrumetClickedSerial].getNumSamples());
    soundTouch->clear();

//    soundTouch->flush();


    //instrumentSampleContainer[instrumetClickedSerial].setSize(originalChannelNum, newSampleLength2 ,/* keepExistingContent: */false,/* clearExtraSpace: */true,/* avoidReallocating: */false);
    
    //instrumentSampleContainer[instrumetClickedSerial] = soundTouch->audio;
    

    if (auto* sound = dynamic_cast<OrionSamplerSound*> (processor.sampler->getSound(instrumetClickedSerial).get()))
    {
        sound->setLength(instrumentSampleContainer[instrumetClickedSerial].getNumSamples());
    }

    // Update To the Part of Selected Audio
    SEPointSlider->audioRangeChange();
    // Update Audio Image
    clipMeter->repaint();

}



//void OrionClipConfiguration::clipStretchUpdate(double value)
//{
//    DBG(value);
//
//    int origSampleLength = instrumentOriginalSampleContainer[instrumetClickedSerial].getNumSamples();
//    int originalChannelNum = instrumentOriginalSampleContainer[instrumetClickedSerial].getNumChannels();
//
//    instrumentSampleContainer[instrumetClickedSerial] = instrumentOriginalSampleContainer[instrumetClickedSerial];
//
//    AudioBuffer<float> localAudioContainerBuffer;
//    int newSampleLength = origSampleLength;
//    int times =  floor(value);
//
//
//
//
//    //if(value < 1.0f){value = 1.0f;}//临时
//
//    // < 1
//    if(value < 1.0f)
//    {
//
//
//
//        newSampleLength = origSampleLength * value;
//        int countGlobal = origSampleLength;
//
//
//
//
//        while(countGlobal > newSampleLength)
//        {
//
//
//
//            int n = instrumentSampleContainer[instrumetClickedSerial].getNumSamples() - newSampleLength;//Amount of points need to be deleted
//
//            if(n == 1)
//            {
//                float source = 0;
//                localAudioContainerBuffer.setSize(originalChannelNum, instrumentSampleContainer[instrumetClickedSerial].getNumSamples(),/* keepExistingContent: */false,/* clearExtraSpace: */true,/* avoidReallocating: */false);
//
//                localAudioContainerBuffer = instrumentSampleContainer[instrumetClickedSerial];
//                int length = instrumentSampleContainer[instrumetClickedSerial].getNumSamples() - 1;
//
//                instrumentSampleContainer[instrumetClickedSerial].setSize(originalChannelNum, length,/* keepExistingContent: */false,/* clearExtraSpace: */true,/* avoidReallocating: */false);
//                for (int i = 0; i < length; i++)
//                {
//                    for (int ch = 0; ch < 2; ch++)
//                    {
//                        source = localAudioContainerBuffer.getSample(ch, i);
//                        instrumentSampleContainer[instrumetClickedSerial].copyFrom(ch/* CH */, i/* Dinstinate Start Sample */, &source /* Source Data */, 1/* numSamples */);
//                    }
//                }
//
//                if (auto* sound = dynamic_cast<OrionSamplerSound*> (processor.sampler->getSound(instrumetClickedSerial).get()))
//                {
//                    sound->setLength(length);
//                }
//
//                return;
//            }
//
//            int detelePointRuler = instrumentSampleContainer[instrumetClickedSerial].getNumSamples()/n;
//
//            int tmp = 0;
//            float source = 0;
//            int count = 0;
//
//            std::cout<< "n: "<< n<<std::endl;
//            std::cout<< "instrumentSampleContainer[instrumetClickedSerial].getNumSamples(): "<< instrumentSampleContainer[instrumetClickedSerial].getNumSamples()<<std::endl;
//            std::cout<< "detelePointRuler: "<< detelePointRuler<<std::endl;
//            localAudioContainerBuffer.setSize(originalChannelNum, instrumentSampleContainer[instrumetClickedSerial].getNumSamples() ,/* keepExistingContent: */false,/* clearExtraSpace: */true,/* avoidReallocating: */false);
//            for (int i = 0; i < instrumentSampleContainer[instrumetClickedSerial].getNumSamples(); i++)
//            {
//                if (tmp != detelePointRuler)
//                {
//                    for (int ch = 0; ch < 2; ch++)
//                    {
//                        source = instrumentSampleContainer[instrumetClickedSerial].getSample(ch, i);
//                        localAudioContainerBuffer.copyFrom(ch/* CH */, count/* Dinstinate Start Sample */, &source /* Source Data */, 1/* numSamples */);
//                    }
//                    count += 1;
//                    tmp += 1;
//                }
//                else
//                {
//                    tmp = 0;
//                }
//            }
//
//            countGlobal = count;
//            instrumentSampleContainer[instrumetClickedSerial].setSize(originalChannelNum, countGlobal,/* keepExistingContent: */false,/* clearExtraSpace: */true,/* avoidReallocating: */false);
//            for (int i = 0; i < countGlobal; i++)
//            {
//                for (int ch = 0; ch < 2; ch++)
//                {
//                    source = localAudioContainerBuffer.getSample(ch, i);
//                    instrumentSampleContainer[instrumetClickedSerial].copyFrom(ch/* CH */, i/* Dinstinate Start Sample */, &source /* Source Data */, 1/* numSamples */);
//                }
//            }
//
//            if (auto* sound = dynamic_cast<OrionSamplerSound*> (processor.sampler->getSound(instrumetClickedSerial).get()))
//            {
//                sound->setLength(instrumentSampleContainer[instrumetClickedSerial].getNumSamples());
//            }
//        }
//
//
//
//
//
//
//    }
//
//    // == 1
//    if(value == 1.0f)
//    {
//        instrumentSampleContainer[instrumetClickedSerial].setSize(originalChannelNum, newSampleLength ,/* keepExistingContent: */false,/* clearExtraSpace: */true,/* avoidReallocating: */false);
//
//        if (auto* sound = dynamic_cast<OrionSamplerSound*> (processor.sampler->getSound(instrumetClickedSerial).get()))
//        {
//            sound->setLength(newSampleLength);
//        }
//    }
//
//
//    // > 1
//    if(value > 1.0f)
//    {
//        // Integer Part
//        if(value >= 2.0f)
//        {
//
//            newSampleLength = newSampleLength * times;
//
//            localAudioContainerBuffer.setSize(originalChannelNum, newSampleLength,/* keepExistingContent: */false,/* clearExtraSpace: */true,/* avoidReallocating: */false);
//
//            int sourceCount = 0;
//            int repeatCount = 0;
//            bool repeatBool = false;
//            for (int i = 0; i < newSampleLength; i++)
//            {
//                if (repeatBool)
//                {
//                    for (int ch = 0; ch < 2; ch++)
//                    {
//                        float source = 0;
//                        source = localAudioContainerBuffer.getSample(ch, i - 1);
//                        localAudioContainerBuffer.copyFrom(ch/* CH */, i/* Destine Start Sample */, &source /* Source Data */, 1/* numSamples */);
//                    }
//
//                    sourceCount += 1;
//                }
//                else
//                {
//                    for (int ch = 0; ch < 2; ch++)
//                    {
//                        float source = 0;
//                        source = instrumentSampleContainer[instrumetClickedSerial].getSample(ch, i - sourceCount);
//                        localAudioContainerBuffer.copyFrom(ch/* CH */, i/* Destine Start Sample */, &source /* Source Data */, 1/* numSamples */);
//                    }
//
//
//                }
//
//
//                repeatCount += 1;
//
//                if(repeatCount == times)
//                {
//                    repeatCount = 0;
//                    repeatBool = false;
//                }
//                else
//                {
//                    repeatBool = true;
//                }
//
//            }
//
//            instrumentSampleContainer[instrumetClickedSerial] = localAudioContainerBuffer;
//            instrumentSampleContainer[instrumetClickedSerial].setSize(originalChannelNum, newSampleLength ,/* keepExistingContent: */false,/* clearExtraSpace: */true,/* avoidReallocating: */false);
//
//            if (auto* sound = dynamic_cast<OrionSamplerSound*> (processor.sampler->getSound(instrumetClickedSerial).get()))
//            {
//                sound->setLength(newSampleLength);
//            }
//        }
//
//
//
//        // Decimal Times Part
//
//
//        std::cout<< "value: "<< value<<std::endl;
//        std::cout<< "times: "<< times<<std::endl;
//
//        float decimal = value - times;
//
//
//        std::cout<< "Decimal: "<< decimal<<std::endl;
//
//        if(decimal != 0)
//        {
//            int n = origSampleLength * decimal;//Amount of Adding Decimal Point
//
//            std::cout<< "n: "<< n<<std::endl;
//
//
//
//            int newSampleLength2 = newSampleLength + n;
//            std::cout<< "newSampleLength: "<< newSampleLength<<std::endl;
//            std::cout<< "newSampleLength2: "<< newSampleLength2<<std::endl;
//
//            int addingPointRuler = newSampleLength2/n;
//
//            std::cout<< "addingPointRuler: "<< addingPointRuler<<std::endl;
//
//            localAudioContainerBuffer.setSize(originalChannelNum, newSampleLength2 ,/* keepExistingContent: */false,/* clearExtraSpace: */true,/* avoidReallocating: */false);
//
//            int tmp = 0;
//            int count = 0;
//
//            float source = 0;
//
//            for (int i = 0; i < newSampleLength2; i++)
//            {
//                if (tmp == addingPointRuler)
//                {
//                    for (int ch = 0; ch < 2; ch++)
//                    {
//                        source = localAudioContainerBuffer.getSample(ch, i - 1);
//                        localAudioContainerBuffer.copyFrom(ch/* CH */, i/* Dinstinate Start Sample */, &source /* Source Data */, 1/* numSamples */);
//                    }
//                    count += 1;
//                    tmp = 0;
//                }
//                else
//                {
//                    for (int ch = 0; ch < 2; ch++)
//                    {
//                        source = instrumentSampleContainer[instrumetClickedSerial].getSample(ch, i - count);
//                        localAudioContainerBuffer.copyFrom(ch/* CH */, i/* Dinstinate Start Sample */, &source /* Source Data */, 1/* numSamples */);
//                    }
//                }
//
//                tmp += 1;
//            }
//
//            instrumentSampleContainer[instrumetClickedSerial] = localAudioContainerBuffer;
//            instrumentSampleContainer[instrumetClickedSerial].setSize(originalChannelNum, newSampleLength2 ,/* keepExistingContent: */false,/* clearExtraSpace: */true,/* avoidReallocating: */false);
//
//            if (auto* sound = dynamic_cast<OrionSamplerSound*> (processor.sampler->getSound(instrumetClickedSerial).get()))
//            {
//                sound->setLength(newSampleLength2);
//            }
//        }
//    }
//    // Update To the Part of Selected Audio
//    SEPointSlider->audioRangeChange();
//    // Update Audio Image
//    clipMeter->repaint();
//}




OrionClipConfiguration::~OrionClipConfiguration()
{
    
}


