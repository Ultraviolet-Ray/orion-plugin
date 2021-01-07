/*
  ==============================================================================

    PrimaryPaneComponent.cpp
    Created: 6 Feb 2020 1:20:29pm
    Author:  Quin Scacheri

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "PrimaryPaneComponent.h"
#include "PluginEditor.h"
#include "OrionGlobalVars.h"
#include "OrionSamplerVoice.h"

#include <iostream>
#include <string>
#include "GlobalCoefficients.h"





//==============================================================================
PrimaryPaneComponent::PrimaryPaneComponent(OrionaudioAudioProcessor* p, OrionaudioAudioProcessorEditor* e)
{
    processor = p;
    editor = e;
    
    // Background Image
    Image backgroundImage = ImageCache::getFromMemory(BinaryData::PrimaryPane_Footer_png, BinaryData::PrimaryPane_Footer_pngSize);
    backgroundImageView.reset(new DrawableImage());
    backgroundImageView->setImage(backgroundImage);
    addAndMakeVisible(backgroundImageView.get());
    
    
    
    Image downImage;
    Image upImage;
    
    // SOLO & MUTE BUTTONS
    downImage = ImageCache::getFromMemory(BinaryData::SoloOn_png, BinaryData::SoloOn_pngSize);
    upImage = ImageCache::getFromMemory(BinaryData::SoloOff_png, BinaryData::SoloOff_pngSize);
    soloButton.reset(new ImageButton());
    soloButton->setImages(false, true, true, soloButtonUp, 1.f, Colours::transparentBlack, soloButtonUp, 1.f, Colours::transparentBlack, soloButtonUp, 1.f, Colours::transparentBlack);
    soloButton->onStateChange = [&] { instrumentSoloButtonClicked(soloButton->isDown());};
    addAndMakeVisible(soloButton.get());
    
    downImage = ImageCache::getFromMemory(BinaryData::MuteOn_png, BinaryData::MuteOn_pngSize);
    upImage = ImageCache::getFromMemory(BinaryData::MuteOff_png, BinaryData::MuteOff_pngSize);
    muteButton.reset(new ImageButton());
    muteButton->setImages(false, true, true, muteButtonUp, 1.f, Colours::transparentBlack, muteButtonUp, 1.f, Colours::transparentBlack, muteButtonUp, 1.f, Colours::transparentBlack);
    muteButton->onStateChange = [&] { instrumentMuteButtonClicked(muteButton->isDown());};
    addAndMakeVisible(muteButton.get());
    
    InstrumentView.reset(new OrionInstrumentView());
    addAndMakeVisible(InstrumentView.get());
    
    for (int i = 0; i < drumButtons.size(); i++)
    {
        drumButtons[i].reset(new DragAndDropButton());
        addAndMakeVisible(drumButtons[i].get());
        //drumButtons[i]->setVisible(false);
        
//        drumButtonCoverImageViews[i].reset(new DrawableImage());
//        addAndMakeVisible(drumButtonCoverImageViews[i].get());
//        drumButtonCoverImageViews[i]->setVisible(false);
    }
    

    
    downImage = ImageCache::getFromMemory(BinaryData::KickOn_png, BinaryData::KickOn_pngSize);
    upImage = ImageCache::getFromMemory(BinaryData::KickOff_png, BinaryData::KickOff_pngSize);
    drumButtons[0]->setImages(false, true, true, upImage, 1.f, Colours::transparentBlack, upImage, 1.f, Colours::transparentBlack, downImage, 1.f, Colours::transparentBlack);
    drumButtons[0]->onStateChange = [&] { editor->drumButtonClicked(orion::MidiNotes::kick, orion::Tabs::kickTab, drumButtons[0]->isDown()); };
    //drumButtonCoverImageViews[0]->setImage(downImage);

    
    downImage = ImageCache::getFromMemory(BinaryData::SnareOn_png, BinaryData::SnareOn_pngSize);
    upImage = ImageCache::getFromMemory(BinaryData::SnareOff_png, BinaryData::SnareOff_pngSize);
    drumButtons[1]->setImages(false, true, true, upImage, 1.f, Colours::transparentBlack, upImage, 1.f, Colours::transparentBlack, downImage, 1.f, Colours::transparentBlack);
    drumButtons[1]->onStateChange = [&] { editor->drumButtonClicked(orion::MidiNotes::snare, orion::Tabs::snareTab, drumButtons[1]->isDown()); };
    //drumButtonCoverImageViews[1]->setImage(downImage);

    downImage = ImageCache::getFromMemory(BinaryData::ClapOn_png, BinaryData::ClapOn_pngSize);
    upImage = ImageCache::getFromMemory(BinaryData::ClapOff_png, BinaryData::ClapOff_pngSize);
    drumButtons[2]->setImages(false, true, true, upImage, 1.f, Colours::transparentBlack, upImage, 1.f, Colours::transparentBlack, downImage, 1.f, Colours::transparentBlack);
    drumButtons[2]->onStateChange = [&] { editor->drumButtonClicked(orion::MidiNotes::clap, orion::Tabs::clapTab, drumButtons[2]->isDown()); };
    //drumButtonCoverImageViews[2]->setImage(downImage);

    downImage = ImageCache::getFromMemory(BinaryData::PercOn_png, BinaryData::PercOn_pngSize);
    upImage = ImageCache::getFromMemory(BinaryData::PercOff_png, BinaryData::PercOff_pngSize);
    drumButtons[3]->setImages(false, true, true, upImage, 1.f, Colours::transparentBlack, upImage, 1.f, Colours::transparentBlack, downImage, 1.f, Colours::transparentBlack);
    drumButtons[3]->onStateChange = [&] { editor->drumButtonClicked(orion::MidiNotes::perc, orion::Tabs::percTab, drumButtons[3]->isDown()); };
    //drumButtonCoverImageViews[3]->setImage(downImage);

    
    downImage = ImageCache::getFromMemory(BinaryData::SnapOn_png, BinaryData::SnapOn_pngSize);
    upImage = ImageCache::getFromMemory(BinaryData::SnapOff_png, BinaryData::SnapOff_pngSize);
    drumButtons[4]->setImages(false, true, true, upImage, 1.f, Colours::transparentBlack, upImage, 1.f, Colours::transparentBlack, downImage, 1.f, Colours::transparentBlack);
    drumButtons[4]->onStateChange = [&] { editor->drumButtonClicked(orion::MidiNotes::snap, orion::Tabs::snapTab, drumButtons[4]->isDown()); };
    //drumButtonCoverImageViews[4]->setImage(downImage);

    
    downImage = ImageCache::getFromMemory(BinaryData::HHOOn_png, BinaryData::HHOOn_pngSize);
    upImage = ImageCache::getFromMemory(BinaryData::HHOOff_png, BinaryData::HHOOff_pngSize);
    drumButtons[5]->setImages(false, true, true, upImage, 1.f, Colours::transparentBlack, upImage, 1.f, Colours::transparentBlack, downImage, 1.f, Colours::transparentBlack);
    drumButtons[5]->onStateChange = [&] { editor->drumButtonClicked(orion::MidiNotes::hho, orion::Tabs::hhoTab, drumButtons[5]->isDown()); };
    //drumButtonCoverImageViews[5]->setImage(downImage);

    
    downImage = ImageCache::getFromMemory(BinaryData::HHCOn_png, BinaryData::HHCOn_pngSize);
    upImage = ImageCache::getFromMemory(BinaryData::HHCOff_png, BinaryData::HHCOff_pngSize);
    drumButtons[6]->setImages(false, true, true, upImage, 1.f, Colours::transparentBlack, upImage, 1.f, Colours::transparentBlack, downImage, 1.f, Colours::transparentBlack);
    drumButtons[6]->onStateChange = [&] { editor->drumButtonClicked(orion::MidiNotes::hhc, orion::Tabs::hhcTab, drumButtons[6]->isDown()); };
    //drumButtonCoverImageViews[6]->setImage(downImage);

    
    downImage = ImageCache::getFromMemory(BinaryData::CrashOn_png, BinaryData::CrashOn_pngSize);
    upImage = ImageCache::getFromMemory(BinaryData::CrashOff_png, BinaryData::CrashOff_pngSize);
    drumButtons[7]->setImages(false, true, true, upImage, 1.f, Colours::transparentBlack, upImage, 1.f, Colours::transparentBlack, downImage, 1.f, Colours::transparentBlack);
    drumButtons[7]->onStateChange = [&] { editor->drumButtonClicked(orion::MidiNotes::crash, orion::Tabs::crashTab, drumButtons[7]->isDown()); };
    //drumButtonCoverImageViews[7]->setImage(downImage);
    
    
    

    
    //METERS
    meterLeft.reset(new CircularMeter());
    meterLeft->updaterFunction = [this] { return processor->getOutputLevel(0);};
    meterLeft->numCircles = 8;
    //meterLeft->backgroundColorHide();
    meterLeft->setColour(meterRight->ColourIds::backgroundColourId, Colours::darkgrey);
    meterLeft->setColour(meterRight->ColourIds::filledColourId, Colours::lightgrey);
    addAndMakeVisible(meterLeft.get());
    
    meterRight.reset(new CircularMeter());
    meterRight->updaterFunction = [this] { return processor->getOutputLevel(1);};
    meterRight->numCircles = 8;
    //meterRight->backgroundColorHide();
    meterRight->setColour(meterRight->ColourIds::backgroundColourId, Colours::darkgrey);
    meterRight->setColour(meterRight->ColourIds::filledColourId, Colours::lightgrey);
    addAndMakeVisible(meterRight.get());
    
    
    

//    // DROPDOWN BUTTON
//    downImage = ImageCache::getFromMemory(BinaryData::arrow_down_png, BinaryData::arrow_down_pngSize);
//    upImage = ImageCache::getFromMemory(BinaryData::arrow_up_png, BinaryData::arrow_up_pngSize);
//    dropDownButton.reset(new ImageButton());
//    dropDownButton->setClickingTogglesState(true);
//    dropDownButton->setImages(false, true, true, downImage, 1.f, Colours::transparentBlack, downImage, 1.f, Colours::transparentBlack, upImage, 1.f, Colours::transparentBlack);
//    //dropDownButton->onStateChange = [&] { editor->updateDropDownState(dropDownButton->getToggleState()); };
//    dropDownButton->onClick = [&] {
//        //printf("Click!");
//
//        //if(!editor->getDropdownVisible()){
//            //int unite = editor->getHeight()/36;
//            //editor->setSize(unite * 60, unite * 50);
//            //editor->constrainer.setFixedAspectRatio((float)editor->getWidth()/editor->getHeight());
//        //}else{
//            //int unite = editor->getHeight()/50;
//
//            //editor->setSize(unite * 60, unite * 36);
//            //editor->constrainer.setFixedAspectRatio((float)editor->getWidth()/editor->getHeight());
//        //};
//
//        editor->updateDropDownState(dropDownButton->getToggleState());
//    };
//    addAndMakeVisible(dropDownButton.get());

    
    
    // WAVE WIGGLE
    waveWiggle.reset(new WaveWiggle());
    addAndMakeVisible(waveWiggle.get());
    waveWiggle->waveColor = Colour(146, 148, 150);
    //waveWiggle->setVisible(false);
    
    
    //--------- INSTRUMENT VOLUME ---------//
    
    // INSTRUMETS VOLUME SLIDER
    instrumentsVolumeSlider.reset(new Slider());
    instrumentsVolumeSlider->setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    instrumentsVolumeSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);//Hide Text Box
    instrumentsVolumeSlider->setColour(Slider::backgroundColourId, juce::Colours::grey);
    instrumentsVolumeSlider->setColour(Slider::trackColourId, juce::Colours::grey);
    instrumentsVolumeSlider->setColour(Slider::thumbColourId, juce::Colours::white);
    instrumentsVolumeSlider->setRange(0.0f, 1.0f);
    instrumentsVolumeSlider->setValue(0.75f);
    instrumentsVolumeSlider->addListener(this);
    addAndMakeVisible(instrumentsVolumeSlider.get());
    
    // INSTRUMETS VOLUME LABEL
    instrumentsVolumeLabel.reset(new Label("INST VOL", "INST VOL"));
    addAndMakeVisible(instrumentsVolumeLabel.get());
    instrumentsVolumeLabel->setAlpha(0.5);
    
    // INSTRUMETS VOLUME SLIDER LABEL
    instrumentsVolumeSliderLabel.reset(new Label("0.75db", "0.75db"));
    addAndMakeVisible(instrumentsVolumeSliderLabel.get());
    instrumentsVolumeSliderLabel->setAlpha(0.5);
    
    // INSTRUMETS VOLUME SUBTRACT LABEL
    instrumentsVolumeSubtractLabel.reset(new Label("-", "-"));
    addAndMakeVisible(instrumentsVolumeSubtractLabel.get());
    instrumentsVolumeSubtractLabel->setAlpha(0.5);
    
    // INSTRUMETS VOLUME PLUS LABEL
    instrumentsVolumePlusLabel.reset(new Label("+", "+"));
    addAndMakeVisible(instrumentsVolumePlusLabel.get());
    instrumentsVolumePlusLabel->setAlpha(0.5);
    
    
    
    
    //--------- INSTRUMENT PAN ---------//
    // INSTRUMETS PAN SLIDER
    instrumentsPanSlider.reset(new Slider());
    instrumentsPanSlider->setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    instrumentsPanSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);//Hide Text Box
    instrumentsPanSlider->setColour(Slider::backgroundColourId, juce::Colours::grey);
    instrumentsPanSlider->setColour(Slider::trackColourId, juce::Colours::grey);
    instrumentsPanSlider->setColour(Slider::thumbColourId, juce::Colours::white);
    instrumentsPanSlider->setRange(-1.0f, 1.0f);
    instrumentsPanSlider->setValue(0.0f);
    instrumentsPanSlider->addListener(this);
    addAndMakeVisible(instrumentsPanSlider.get());
    
    // INSTRUMETS PAN LABEL
    instrumentsPanLabel.reset(new Label("INST PAN", "INST PAN"));
    addAndMakeVisible(instrumentsPanLabel.get());
    instrumentsPanLabel->setAlpha(0.5);
    
    // INSTRUMETS PAN LCR LABEL
    instrumentsPanLCRLabel.reset(new Label("L              C              R",
                                           "L              C              R"));
    
    addAndMakeVisible(instrumentsPanLCRLabel.get());
    instrumentsPanLCRLabel->setAlpha(0.5);
    
    
    //--------- MASTER VOLUME ---------//
    
    // MASTER VOLUME SLIDER
    MasterVolumeSlider.reset(new Slider());
    MasterVolumeSlider->setSliderStyle(Slider::SliderStyle::LinearVertical);
    MasterVolumeSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);//Hide Text Box
    MasterVolumeSlider->setColour(Slider::backgroundColourId, juce::Colours::grey);
    MasterVolumeSlider->setColour(Slider::trackColourId, juce::Colours::grey);
    MasterVolumeSlider->setColour(Slider::thumbColourId, juce::Colours::lightpink);
    MasterVolumeSlider->setRange(0.0f, 1.0f);
    MasterVolumeSlider->setValue(0.75f);
    MasterVolumeSlider->addListener(this);
    addAndMakeVisible(MasterVolumeSlider.get());
    
    // MASTER VOLUME SLIDER LABEL
    MasterVolumeSliderLabel.reset(new Label("0.75db", "0.75db"));
    addAndMakeVisible(MasterVolumeSliderLabel.get());
    MasterVolumeSliderLabel->setAlpha(0.5);
    //MasterVolumeSliderLabel->setColour(Slider::backgroundColourId, juce::Colours::darkgrey);
    
    // MASTER VOLUME PLUS LABEL
    MasterVolumePlusLabel.reset(new Label("+", "+"));
    addAndMakeVisible(MasterVolumePlusLabel.get());
    MasterVolumePlusLabel->setAlpha(0.5);
    
    // MASTER VOLUME SUBTRACT LABEL
    MasterVolumeSubtractLabel.reset(new Label("-", "-"));
    addAndMakeVisible(MasterVolumeSubtractLabel.get());
    MasterVolumeSubtractLabel->setAlpha(0.5);

    resized();
    
}

PrimaryPaneComponent::~PrimaryPaneComponent()
{
    
}

void PrimaryPaneComponent::paint (Graphics& g)
{
    //g.setGradientFill(backgroundGradient);
    
    //Image background = ImageCache::getFromMemory(BinaryData::PrimaryPaneBackground_png, BinaryData::PrimaryPaneBackground_pngSize);
  
    //g.drawImageAt (background, 0, 0);
    
    g.fillAll();
}

//MARK:- RESIZE
void PrimaryPaneComponent::resized()
{
    float uniteW = getWidth()/100;
    
    //--------- Background Image ---------//
    Rectangle<int> backgroundArea(0, 0, getWidth(), getHeight());
    backgroundImageView->setTransformToFit(backgroundArea.toFloat(), RectanglePlacement::stretchToFit);
    //backgroundGradient = ColourGradient::horizontal(Colour(0xFF0C0C0D), 0, Colours::black, getWidth() / 2);

    
    //--------- Solo and Mute Buttons ---------//
    Rectangle<int> area(3 * uniteW, 2.5 * uniteW, 4 * uniteW, 4 * uniteW);
    soloButton->setBounds(area);
    
    area.translate(5 * uniteW, 0);
    muteButton->setBounds(area);
    

    //--------- Instrument Pads ---------//
    int drumCount = 0;
    
    double padViewX = getWidth()* 0.1;
    double padViewY = getHeight() * 0.2;
    double padViewWidth = getWidth() * 0.785;
    double padViewHeight = getHeight() * 0.375;
    
    double padWidth = padViewWidth/5.5;
    double padHeight = padViewHeight/2;
    
    area.setSize(padWidth, padHeight);
    
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            area.setPosition(padViewX + j * padWidth * 1.5, padViewY + i * padHeight);
            drumButtons[drumCount]->setBounds(area);
            //drumButtonCoverImageViews[drumCount]->setBounds(area);
            //drumButtonCoverImageViews[drumCount]->setTransformToFit(area.toFloat(), RectanglePlacement::onlyReduceInSize);
            drumCount++;
        }
    }

    //--------- Instrument View ---------//
    
    area = Rectangle<int>(padViewX, padViewY,  padViewWidth, padViewHeight);
    
    InstrumentView->setBounds(area);
    
    
    //--------- Wave Wiggle ---------//
    area = Rectangle<int>(getWidth()/2 - 18.5 * uniteW, getHeight()/1.75,  getWidth() * 7/20, 16 * uniteW);
    waveWiggle->setBounds(area);
    //waveWiggle->setCentrePosition(getWidth()/2,39 * uniteW);

    
    
    //--------- Instrument Volume ---------//
    // Instruments Volume Label
    area = Rectangle<int>(20 * uniteW, 1 * uniteW,  14 * uniteW, 2 * uniteW);
    instrumentsVolumeLabel->setBounds(area);
    
    // Instruments Volume "-" Label
    area = Rectangle<int>(15 * uniteW, 3.55 * uniteW,  2 * uniteW, 2 * uniteW);
    instrumentsVolumeSubtractLabel->setBounds(area);
    
    // Instruments Volume Slider
    area = Rectangle<int>(16 * uniteW, 3.6 * uniteW,  14 * uniteW, 2 * uniteW);
    instrumentsVolumeSlider->setBounds(area);
    
    // Instruments Volume "+" Label
    area = Rectangle<int>(29 * uniteW, 3.55 * uniteW,  2 * uniteW, 2 * uniteW);
    instrumentsVolumePlusLabel->setBounds(area);
    
    // Instruments Volume Slider Label
    area = Rectangle<int>(20.5 * uniteW, 5.75 * uniteW,  16 * uniteW, 2 * uniteW);
    instrumentsVolumeSliderLabel->setBounds(area);
    
    //--------- Instrument Pan ---------//
    // Instruments Pan Label
    area = Rectangle<int>(getWidth()/2 - 9 * uniteW, 1 * uniteW,  14 * uniteW, 2 * uniteW);
    instrumentsPanLabel->setBounds(area);
    
    // Instruments Pan Slider
    area = Rectangle<int>(getWidth()/2 - 13.5 * uniteW, 3.6 * uniteW, 14 * uniteW, 2 * uniteW);
    instrumentsPanSlider->setBounds(area);
    
    // Instruments Pan "L C R" Label
    area = Rectangle<int>(getWidth()/2.605, 5.75 * getWidth()/100,  14 * getWidth()/100, 2 * getWidth()/100);
    instrumentsPanLCRLabel->setBounds(area);

    
    
    //--------- Master Volume ---------//
    // Master Volume Meter Left
    area = Rectangle<int>(getWidth() - 8 * uniteW, 1 * uniteW, 1.75 * uniteW, 11 * uniteW);
    meterLeft->setBounds(area);
    
    // Master Volume Meter Right
    area = Rectangle<int>(getWidth() - 6.25 * uniteW, 1 * uniteW, 1.75 * uniteW, 11 * uniteW);
    meterRight->setBounds(area);
    
    // Master Volume Slider Label
    area = Rectangle<int>(getWidth() - 9 * uniteW, 12.75 * uniteW, 8 * uniteW, 2 * uniteW);
    MasterVolumeSliderLabel->setBounds(area);
    
    // Master Volume "+" Label
    area = Rectangle<int>(getWidth() - 3 * uniteW, 0.75 * uniteW, 2 * uniteW, 2 * uniteW);
    MasterVolumePlusLabel->setBounds(area);
    
    // Master Volume Slider
    area = Rectangle<int>(getWidth() - 3 * uniteW, 2 * uniteW, 2 * uniteW, 10 * uniteW);
    MasterVolumeSlider->setBounds(area);
    
    // Master Volume "-" Label
    area = Rectangle<int>(getWidth() - 2.88 * uniteW, 11.25 * uniteW, 2 * uniteW, 2 * uniteW);
    MasterVolumeSubtractLabel->setBounds(area);

    
    //----------------------------------
    repaint();
}


void PrimaryPaneComponent::sliderValueChanged (Slider* slider)
{
    if(slider == MasterVolumeSlider.get())
    {
        masterVolumeCoefficient = MasterVolumeSlider->getValue();
        
        double a = masterVolumeCoefficient;
        float b = (int)(a * 100 + .5);
        float c =  (float)b / 100;
        String d = String(c);
        if(d == "0")
        {
            d = "0.00";
        }
        else if(d == "1")
        {
            d = "1.00";
        }
        d.append(" db",3);
        MasterVolumeSliderLabel->setText(d,dontSendNotification);
    }
    else if(slider == instrumentsVolumeSlider.get())
    {
        instrumentsVolumeCoefficient[instrumetClickedSerial] = instrumentsVolumeSlider->getValue();
        
        double a = instrumentsVolumeCoefficient[instrumetClickedSerial];
        float b = (int)(a * 100 + .5);
        float c =  (float)b / 100;
        String d = String(c);
        if(d == "0")
        {
            d = "0.00";
        }
        else if(d == "1")
        {
            d = "1.00";
        }
        d.append(" db",3);
        instrumentsVolumeSliderLabel->setText(d,dontSendNotification);
    }
    else if(slider == instrumentsPanSlider.get())
    {
        instrumentsPanCoefficient[instrumetClickedSerial] = instrumentsPanSlider->getValue();
        
        if(auto* voice = dynamic_cast<OrionSamplerVoice*>(processor->getSampler()->getVoice(instrumetClickedSerial)))
        {
            voice->panchange(instrumentsPanCoefficient[instrumetClickedSerial]);
        }
        
    }
}




void PrimaryPaneComponent::instrumentSoloButtonClicked(bool isDown)
{
    if(isDown)
    {
        instrumentsSoloStates[instrumetClickedSerial] = !instrumentsSoloStates[instrumetClickedSerial];
        
      
        if(instrumentsSoloStates[instrumetClickedSerial])
        {
            instrumentsMuteStates[instrumetClickedSerial] = false;
            soloButton->setImages(false, true, true, soloButtonDown, 1.f, Colours::transparentBlack, soloButtonDown, 1.f, Colours::transparentBlack, soloButtonDown, 1.f, Colours::transparentBlack);
            muteButton->setImages(false, true, true, muteButtonUp, 1.f, Colours::transparentBlack, muteButtonUp, 1.f, Colours::transparentBlack, muteButtonUp, 1.f, Colours::transparentBlack);
            
            /* Set Solo & Mute Button state Arrays */
            for (int i = 0; i < instrumentAmount; i++)
            {
                if(instrumentsSoloStates[i])
                {
                    instrumentsMuteStates[i] = false;
                }
                else
                {
                    instrumentsMuteStates[i] = true;
                }
            }
        }
        else
        {
            soloButton->setImages(false, true, true, soloButtonUp, 1.f, Colours::transparentBlack, soloButtonUp, 1.f, Colours::transparentBlack, soloButtonUp, 1.f, Colours::transparentBlack);
            
            /* Incase Every Solo Button Back to Normal */
            int count = 0;
            for (int i = 0; i < instrumentAmount; i++)
            {
                if(instrumentsMuteStates[i])
                {
                    count += 1;
                }
            }
            
            DBG(count);
            if(count == instrumentAmount - 1)
            {
                //DBG("Hey");
                for (int j = 0; j < instrumentAmount; j++)
                {
                    instrumentsMuteStates[j] = false;
                    instrumentsSoloStates[j] = false;
                }
                soloButton->setImages(false, true, true, soloButtonUp, 1.f, Colours::transparentBlack, soloButtonUp, 1.f, Colours::transparentBlack, soloButtonUp, 1.f, Colours::transparentBlack);
                muteButton->setImages(false, true, true, muteButtonUp, 1.f, Colours::transparentBlack, muteButtonUp, 1.f, Colours::transparentBlack, muteButtonUp, 1.f, Colours::transparentBlack);
            }
            else
            {
                for (int j = 0; j < instrumentAmount; j++)
                {
                    if(instrumentsSoloStates[j])
                    {
                        instrumentsMuteStates[j] = false;
                    }
                    else
                    {
                        instrumentsMuteStates[j] = true;
                    }
                }
            }
        }
        
        
        
        /* Incase Previous Mute State */
        if(instrumentsMuteStates[instrumetClickedSerial])
        {
            muteButton->setImages(false, true, true, muteButtonDown, 1.f, Colours::transparentBlack, muteButtonDown, 1.f, Colours::transparentBlack, muteButtonDown, 1.f, Colours::transparentBlack);
        }
        else
        {
            muteButton->setImages(false, true, true, muteButtonUp, 1.f, Colours::transparentBlack, muteButtonUp, 1.f, Colours::transparentBlack, muteButtonUp, 1.f, Colours::transparentBlack);
        }
        
    }
}


void PrimaryPaneComponent::instrumentMuteButtonClicked(bool isDown)
{
    if(isDown)
    {
        instrumentsMuteStates[instrumetClickedSerial] = !instrumentsMuteStates[instrumetClickedSerial];
        
        /* Set Solo & Mute Button Images */
        if(instrumentsMuteStates[instrumetClickedSerial])
        {
            instrumentsSoloStates[instrumetClickedSerial] = false;
            muteButton->setImages(false, true, true, muteButtonDown, 1.f, Colours::transparentBlack, muteButtonDown, 1.f, Colours::transparentBlack, muteButtonDown, 1.f, Colours::transparentBlack);
            
            soloButton->setImages(false, true, true, soloButtonUp, 1.f, Colours::transparentBlack, soloButtonUp, 1.f, Colours::transparentBlack, soloButtonUp, 1.f, Colours::transparentBlack);
        }
        else
        {
            muteButton->setImages(false, true, true, muteButtonUp, 1.f, Colours::transparentBlack, muteButtonUp, 1.f, Colours::transparentBlack, muteButtonUp, 1.f, Colours::transparentBlack);
        }

    }
}
