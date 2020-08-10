#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "OrionGlobalVars.h"
#include "GlobalCoefficients.h"
#include <iostream>
#include <array>

#define NUM_TABS 4

#define __MIRRORVIEWMAIN__
#include "MirrorViews.h"

using namespace std;
using namespace juce;
//==============================================================================

OrionaudioAudioProcessorEditor::OrionaudioAudioProcessorEditor (OrionaudioAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p),
AppDir("Applications",DrawableButton::ButtonStyle::ImageFitted),
DeskDir("Desktop",DrawableButton::ButtonStyle::ImageFitted),
DownDir("Downloads",DrawableButton::ButtonStyle::ImageFitted),
MusicDir("Music",DrawableButton::ButtonStyle::ImageFitted),
DocDir("Documents",DrawableButton::ButtonStyle::ImageFitted),
HomDir("Home",DrawableButton::ButtonStyle::ImageFitted),
UpBut("Up",DrawableButton::ImageOnButtonBackground),
filebrowser(1|4|8|32,File::getSpecialLocation(File::SpecialLocationType::userHomeDirectory),nullptr,nullptr),
mainlist("main", dynamic_cast<ListBoxModel*> (&maindir)), startTime(Time::getMillisecondCounterHiRes()*0.001)
{
    
    menuBar.reset(new OrionMenuBar(&p, this));
    addAndMakeVisible(menuBar.get());
    
    primaryPane.reset(new PrimaryPaneComponent(&p, this));
    addAndMakeVisible(primaryPane.get());
    PrimaryPaneMirror = primaryPane.get();
    
    sidePanel.reset(new SidePanelComponent(&p, this));
    addAndMakeVisible(sidePanel.get());
    
    
    //--!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //tabComponents.reset(new TabComponentHolder(p));
    //addAndMakeVisible(tabComponents.get());
//    for (int i = 0; i < instrumentAmount; i++)
//    {
//        tabComponents->tabComponents[i]->envConfiguration->envelopeMeter->initAudioFile(i);
//    }
//
    //--!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    dropdownTable.reset(new DropdownTable(&p));
    addAndMakeVisible(dropdownTable.get());
    

    arrangementWindow.reset(new ArrangementWindowComponent(&p, this));
    addAndMakeVisible(arrangementWindow.get());
    arrangementWindow->setVisible(arrangementWindowVisible);
    
    footerPath.reset(new DrawablePath());
    addAndMakeVisible(footerPath.get());
    footerPath->replaceColour(Colours::black,Colours::darkgrey);
    //processor->sampler->editor = this;
    //setColour
    
    //indices = {kickButton.index, snareButton.index, clapButton.index, percButton.index, HiHatButton.index, cymbalButton.index, snapButton.index};
//    for (int i=0; i<7; i++)
//    {
//        indices[i] = i;
//    }
    
    backgroundImage.reset(new ImageComponent("backgroundImage"));
    backgroundImage->setImage(ImageCache::getFromMemory(BinaryData::orionBackground_png, BinaryData::orionBackground_pngSize), RectanglePlacement::fillDestination);
    addAndMakeVisible(backgroundImage.get());
    
    
    //--------- DROPDOWN BUTTONS ---------//
    
    
    dropDownBottonBar.reset(new DrawablePath());
    addAndMakeVisible(dropDownBottonBar.get());
    dropDownBottonBar->replaceColour(Colours::black,Colours::black);

     
    // Dropdown EQ BUTTON
    Image downImage = ImageCache::getFromMemory(BinaryData::EQButtonOn_png,  BinaryData::EQButtonOn_pngSize);
    Image upImage   = ImageCache::getFromMemory(BinaryData::EQButtonOff_png, BinaryData::EQButtonOff_pngSize);
    dropDownEQ.reset(new ImageButton());
    dropDownEQ->setClickingTogglesState(true);
    dropDownEQ->onClick = [&]{
        if(dropDownEQ->getToggleState())
        {
           dropdownTabSerial = 0;
        }
        updateDropDownEQState(dropDownEQ->getToggleState());
    };
    addAndMakeVisible(dropDownEQ.get());
    dropDownEQ->setImages(false, true, true, upImage, 1.f, Colours::transparentBlack, upImage, 1.f, Colours::transparentBlack, downImage, 1.f, Colours::transparentBlack);

    // Dropdown Clip BUTTON
    downImage = ImageCache::getFromMemory(BinaryData::ClipButtonOn_png,  BinaryData::ClipButtonOn_pngSize);
    upImage   = ImageCache::getFromMemory(BinaryData::ClipButtonOff_png, BinaryData::ClipButtonOff_pngSize);
    dropDownClip.reset(new ImageButton());
    dropDownClip->setClickingTogglesState(true);
    dropDownClip->setImages(false, true, true, upImage, 1.f, Colours::transparentBlack, upImage, 1.f, Colours::transparentBlack, downImage, 1.f, Colours::transparentBlack);
    dropDownClip->onClick = [&]{
        if(dropDownClip->getToggleState()){
           dropdownTabSerial = 1;
        }
        updateDropDownClipState(dropDownClip->getToggleState());
    };
    addAndMakeVisible(dropDownClip.get());

    // Dropdown ENV BUTTON
    downImage = ImageCache::getFromMemory(BinaryData::ENVButtonOn_png,  BinaryData::ENVButtonOn_pngSize);
    upImage   = ImageCache::getFromMemory(BinaryData::ENVButtonOff_png, BinaryData::ENVButtonOff_pngSize);
    dropDownENV.reset(new ImageButton());
    dropDownENV->setClickingTogglesState(true);
    dropDownENV->setImages(false, true, true, upImage, 1.f, Colours::transparentBlack, upImage, 1.f, Colours::transparentBlack, downImage, 1.f, Colours::transparentBlack);
    dropDownENV->onClick = [&]{
        if(dropDownENV->getToggleState()){
           dropdownTabSerial = 2;
        }
        updateDropDownENVState(dropDownENV->getToggleState());
    };
    addAndMakeVisible(dropDownENV.get());

    // Dropdown FX BUTTON
    downImage = ImageCache::getFromMemory(BinaryData::FXButtonOn_png,  BinaryData::FXButtonOn_pngSize);
    upImage   = ImageCache::getFromMemory(BinaryData::FXButtonOff_png, BinaryData::FXButtonOff_pngSize);
    dropDownFX.reset(new ImageButton());
    dropDownFX->setClickingTogglesState(true);
    dropDownFX->setImages(false, true, true, upImage, 1.f, Colours::transparentBlack, upImage, 1.f, Colours::transparentBlack, downImage, 1.f, Colours::transparentBlack);
    dropDownFX->onClick = [&]
    {
        if(dropDownFX->getToggleState())
        {
           dropdownTabSerial = 3;
        }
        
        updateDropDownFXState(dropDownFX->getToggleState());
    };
    addAndMakeVisible(dropDownFX.get());
    
    
    
    
    
    
    //--------- RESIZE BUTTONS ---------//
    // RESIZE BUTTON
    downImage = ImageCache::getFromMemory(BinaryData::ResizeOff_png, BinaryData::ResizeOff_pngSize);
    upImage   = ImageCache::getFromMemory(BinaryData::ResizeOn_png, BinaryData::ResizeOn_pngSize);
    resizeButton.reset(new ImageButton());
    resizeButton->setClickingTogglesState(true);
    resizeButton->setImages(false, true, true, downImage, 1.f, Colours::transparentBlack, downImage, 1.f, Colours::transparentBlack, upImage, 1.f, Colours::transparentBlack);
    resizeButton->onClick = [&]
    {
        updateResizeViewState(resizeButton->getToggleState());
    };
    addAndMakeVisible(resizeButton.get());
    
    
    
    
    
    //--------------------------------//
    
    std::unique_ptr<Drawable> buttonOff;
    std::unique_ptr<Drawable> buttonOn;
        
    buttonOff = Drawable::createFromImageData(BinaryData::arrow_down_png, BinaryData::arrow_down_pngSize);
    buttonOn = Drawable::createFromImageData(BinaryData::arrow_up_png, BinaryData::arrow_up_pngSize);
    buttonOn = Drawable::createFromImageData(BinaryData::appdir_png, BinaryData::appdir_pngSize);
    
    AppDir.setImages(buttonOn.get(), buttonOn.get(), buttonOn.get());
    //AppDir.onStateChange = [this]/*capture this event 执行后面{}的指令*/{ appdirClicked(); };
    AppDir.setColour(TextButton::buttonColourId, Colours::darkgrey);
    AppDir.setEnabled(true);//防止用户多次按
    addAndMakeVisible(&AppDir);
    
    DeskDir.setImages(buttonOn.get(), buttonOn.get(), buttonOn.get());
    //DeskDir.onStateChange = [this]/*capture this event 执行后面{}的指令*/{ deskdirClicked(); };
    DeskDir.setColour(TextButton::buttonColourId, Colours::darkgrey);
    DeskDir.setEnabled(true);//防止用户多次按
    addAndMakeVisible(&DeskDir);
    
    DownDir.setImages(buttonOn.get(), buttonOn.get(), buttonOn.get());
    //DownDir.onStateChange = [this]/*capture this event 执行后面{}的指令*/{ downdirClicked(); };
    DownDir.setColour(TextButton::buttonColourId, Colours::darkgrey);
    DownDir.setEnabled(true);//防止用户多次按
    addAndMakeVisible(&DownDir);
    
    MusicDir.setImages(buttonOn.get(), buttonOn.get(), buttonOn.get());
    //MusicDir.onStateChange = [this]/*capture this event 执行后面{}的指令*/{ musicdirClicked(); };
    MusicDir.setColour(TextButton::buttonColourId, Colours::darkgrey);
    MusicDir.setEnabled(true);//防止用户多次按
    addAndMakeVisible(&MusicDir);
    
    DocDir.setImages(buttonOn.get(), buttonOn.get(), buttonOn.get());
    //DocDir.onStateChange = [this]/*capture this event 执行后面{}的指令*/{ docdirClicked(); };
    DocDir.setColour(TextButton::buttonColourId, Colours::darkgrey);
    DocDir.setEnabled(true);//防止用户多次按
    addAndMakeVisible(&DocDir);
    
    HomDir.setImages(buttonOn.get(), buttonOn.get(), buttonOn.get());
    //HomDir.onStateChange = [this]/*capture this event 执行后面{}的指令*/{ homedirClicked(); };
    HomDir.setColour(TextButton::buttonColourId, Colours::darkgrey);
    HomDir.setEnabled(true);//防止用户多次按
    addAndMakeVisible(&HomDir);
    
    //set up up button's images
    Path arrowPath;
    arrowPath.addArrow ({ 50.0f, 100.0f, 50.0f, 0.0f }, 40.0f, 100.0f, 50.0f);
    
    DrawablePath arrowImage;
    arrowImage.setFill (Colours::black.withAlpha (0.4f));
    arrowImage.setPath (arrowPath);
    UpBut.setImages(&arrowImage);
    addAndMakeVisible(&UpBut);
    
    formatManager.registerBasicFormats();
    
    
    
    //fileBrowser.reset(new DraggableFileBrowserComponent());
    //addAndMakeVisible(fileBrowser.get());
    
    
    
    
    // BACK BUTTON
//    backButton.reset(new ImageButton());
//    backButton->setAlwaysOnTop(true);
//    Image backImageOff = ImageCache::getFromMemory(BinaryData::arrow_left_png, BinaryData::arrow_left_pngSize);
//    Image backImageOn = ImageCache::getFromMemory(BinaryData::arrow_leftOn_png, BinaryData::arrow_leftOn_pngSize);
//    backButton->setImages(false, true, true, backImageOff, 1.f, Colours::transparentBlack, backImageOff, 1.f, Colours::transparentBlack, backImageOn, 1.f, Colours::transparentBlack);
//    backButton->onClick = [&] {
//        fileBrowser->backButton->onClick();
//        //fileBrowser->scanDirectory(fileBrowser->currentDirectory.getDirectory().getParentDirectory());
//        fileBrowser->scannerThread.stopThread(-1);
//        fileBrowser->windowComponent.setBounds(getLocalBounds());
//        fileBrowser->windowComponent.setNewDirectory(currentDirectory);
//        for (int i = 0; i < fileBrowser->windowComponent.fileComponents.size(); i++){
//            windowComponent.fileComponents[i]->addListener(this);
//        }
//    };
//    addAndMakeVisible(backButton.get());
    

    

    
    meterInput.reset(new CircularMeter());
    meterInput->updaterFunction = [this] {
        return processor.getInputLevel();
    };
    addAndMakeVisible(meterInput.get());
    

    
    constrainer.setFixedAspectRatio((float)OrionGlobalWidth/OrionGlobalHeight);
    constrainer.setSizeLimits((float)OrionGlobalWidth / 2, (float)OrionGlobalHeight / 2, (float)OrionGlobalWidth * 2, (float)OrionGlobalHeight * 2);
    constrainer.setSizeLimits((float)OrionGlobalWidth / 2, (float)OrionGlobalHeight / 2, (float)OrionGlobalWidth * 2, (float)OrionGlobalHeight * 2);
    
    cornerComponent.reset(new OrionResizableCornerComponent<OrionaudioAudioProcessorEditor>(this, this, &constrainer));
    //addAndMakeVisible(cornerComponent.get());
    
    
    setSize (OrionGlobalWidth, OrionGlobalHeight);
    setSize(getWidth(), (getHeight()/18) * 25);
    constrainer.setFixedAspectRatio((float)getWidth()/getHeight());
    setSize(getWidth(), (getHeight()/25) * 18);
    constrainer.setFixedAspectRatio((float)getWidth()/getHeight());
    
    
    //    meterLeft.reset(new CircularMeter());
    //    meterLeft->updaterFunction = [this] {
    //        return processor.getOutputLevel(0);
    //    };
    //    addAndMakeVisible(meterLeft.get());
    //
    //    meterRight.reset(new CircularMeter());
    //    meterRight->updaterFunction = [this] {
    //        return processor.getOutputLevel(1);
    //    };
    //    addAndMakeVisible(meterRight.get());
    
}



OrionaudioAudioProcessorEditor::~OrionaudioAudioProcessorEditor()
{
}


//==============================================================================
void OrionaudioAudioProcessorEditor::tabComponentChanged(int serial)
{
    //    tabComponent[serial]->setBounds(0, (getHeight()/3)*2, getWidth(), getHeight()/3);
    //
    //    addAndMakeVisible(tabComponent[serial].get());
}
//==============================================================================

void OrionaudioAudioProcessorEditor::paint (Graphics& g)
{
    //************************************DO NOT PUT ANY SET BOUNDS METHODS OR ANYTHING OTHER THAN DRAWING METHODS********************************
    //*********************************************************************************************************************************************
    //*********************************************************************************************************************************************
    //*********************************************************************************************************************************************
    
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::grey);
    g.drawHorizontalLine(getHeight()*4/36, getHeight()*4/36, getWidth());
}

void OrionaudioAudioProcessorEditor::resized()
{
    
    
    //int width = getWidth();
    //int height = getHeight();
    
    
    //auto area = getLocalBounds();
    //cout << "............................................."<< endl;
    //cout << "Height: "<< getHeight() << endl;

    
//    // MENU BAR
//    auto menuBarArea = area;
//    menuBarArea = menuBarArea.removeFromTop(getHeight() * .1);
//    menuBarArea.removeFromLeft(getWidth() / 6);
//    menuBar->setBounds(menuBarArea);
//    
//    // PRIMARY PANE
//    auto primaryPaneArea = area;
//    primaryPaneArea.setY(menuBar->getBottom());
//    if (dropDownVisible)
//        primaryPaneArea.setBottom(tabComponents->getY());
//    else
//        primaryPaneArea.setBottom(getHeight());
//    primaryPaneArea.removeFromLeft(getWidth() / 6);
//    if (arrangementWindowVisible)
//        arrangementWindow->setBounds(primaryPaneArea);
//    else
//        primaryPane->setBounds(primaryPaneArea);
//
//    
//    // SIDE PANEL
//    area = getLocalBounds();
//    area = area.removeFromLeft(getWidth() / 6);
//    area.setHeight(h);
//    sidePanel->setBounds(area);
    
    
    
    
    float unite = getHeight()/36;
    float uniteW = getWidth()/30;
    
    // MENU BAR
    auto area = Rectangle<int>(getWidth()/6, 0, getWidth() * 5 / 6, 3 * unite);
    menuBar->setBounds(area);
    
    // SIDE PANEL
    area = Rectangle<int>(0, 0, getWidth()/6, getHeight());
    sidePanel->setBounds(area);
    
    // PRIMARY PANE
    area = Rectangle<int>(sidePanel->getWidth(), 3 * unite, getWidth() - sidePanel->getWidth(), getHeight() - 3 * unite);
    primaryPane->setBounds(area);
    
    // ARRANGEMENT WINDOW
    area = Rectangle<int>(sidePanel->getWidth(), 3 * unite, getWidth() - sidePanel->getWidth(), getHeight() - 4 * unite);
    arrangementWindow->setBounds(area);
    
    
    // DROPDOWN WINDOW
    if (dropDownVisible)
    {
        area = Rectangle<int>(0, getHeight() - 13.1 * unite, getWidth(), 12 * unite);
        //--!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //tabComponents->setBounds(area);
        dropdownTable->setBounds(area);
        //--!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
    
    setUIScale(float(getWidth() / float(orion::defaultWidth)));

    
    //--------- DROPDOWN Window ---------//
    Path path;
    
    // Footer Path
    path.addRectangle (0, getHeight() - 2.1 * unite, getWidth(), 1.0f);
    footerPath->setPath(path);
    
    // Dropdown Bar
    path.addRectangle (0, getHeight() - 2.1 * unite, getWidth(), 4 * unite);
    dropDownBottonBar->setPath(path);
    
    // EQ
    area = Rectangle<int>( 5 * uniteW + 0 * 5 * uniteW, getHeight() - 2 * unite, 2 * uniteW, 2.1  * unite);
    dropDownEQ->setBounds(area);

    // CLIP
    area = Rectangle<int>( 5 * uniteW + 1 * 6 * uniteW, getHeight() - 2 * unite, 2 * uniteW, 2.1  * unite);
    dropDownClip->setBounds(area);

    // ENV
    area = Rectangle<int>( 5 * uniteW + 2 * 6 * uniteW, getHeight() - 2 * unite, 2 * uniteW, 2.1  * unite);
    dropDownENV->setBounds(area);

    // FX
    area = Rectangle<int>( 5 * uniteW + 3 * 6 * uniteW, getHeight() - 2 * unite, 2 * uniteW, 2.1  * unite);
    dropDownFX->setBounds(area);
    
    
    
    //--------- RESIZE BUTTONS ---------//
    area = Rectangle<int>(getWidth() - 2.5 * unite, getHeight() - 2 * unite, 2 * unite, 2 * unite);
    resizeButton->setBounds(area);

    // CORNER COMPONENT
    cornerComponent->setBounds(getWidth() - (getWidth() * .03), getHeight() - (getWidth() * .03), (getWidth() * .03), (getWidth() * .03));
    

    
    //************************************DO NOT PUT ANY SET BOUNDS METHODS OR ANYTHING OTHER THAN DRAWING METHODS********************************
    //*********************************************************************************************************************************************
    //*********************************************************************************************************************************************
    //*********************************************************************************************************************************************
    
    
}

void OrionaudioAudioProcessorEditor::addMessageToList (const MidiMessage& message)
{
    auto time = message.getTimeStamp();
    auto hours   = ((int) (time / 3600.0)) % 24;
    auto minutes = ((int) (time / 60.0)) % 60;
    auto seconds = ((int) time) % 60;
    auto millis  = ((int) (time * 1000.0)) % 1000;
    auto timecode = String::formatted ("%02d:%02d:%02d.%03d",
                                       hours,
                                       minutes,
                                       seconds,
                                       millis);
    //logMessage (timecode + "  -  " + getMidiMessageDescription (message));
}

void OrionaudioAudioProcessorEditor::drumButtonClicked(int midiNote, int tabIndex, bool isDown)
{
    //return;
    if(isDown)
    {
        std::cout<<"                  "<<std::endl;
        //std::cout<<"Down"<<std::endl;
        
        //if (processor.getMidiOutput() != nullptr)
        //{
            //processor.getMidiOutput()->sendMessageNow(MidiMessage::noteOn(1, midiNote, 1.f));
            //std::cout<<"MIDINoteOnFromClick"<<std::endl;
        //}
        
        //tabComponents->setCurrentTab(instrumetSerial);//--!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        
        //tabComponents->tabComponents[instrumetSerial]->setCurrentTabIndex(dropdownTabSerial);//--!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        
        processor.getSampler()->noteOn(1, midiNote, 120);//
        
        instrumetSerial = tabIndex;
        
        dropdownTable->currentTab = instrumetSerial;
        
        std::cout<<"click!!"<<std::endl;
        std::cout<<"instrumetSerial: "<<instrumetSerial<<std::endl;
        std::cout<<"dropdownTabSerial: "<<dropdownTabSerial<<std::endl;
        

        //MARK:- Primary Pane
        primaryPane->setInstrumetsVolumeSliderValue(instrumentsVolumeCoefficient[instrumetSerial]);
        primaryPane->setInstrumetsPanSliderValue(instrumentsPanCoefficient[instrumetSerial]);

        /* Set Solo Button Image */
        if(instrumentsSoloStates[instrumetSerial])
        {
            primaryPane->setInstrumetsSoloButtonImage(true);
        }
        else
        {
           primaryPane->setInstrumetsSoloButtonImage(false);
        }

        /* Set Mute Button Image */
        if(instrumentsMuteStates[instrumetSerial])
        {
            primaryPane->setInstrumetsMuteButtonImage(true);
        }
        else
        {
            primaryPane->setInstrumetsMuteButtonImage(false);
        }
        
        if(dropdownTabSerial == 0)//MARK:- EQ
        {
            dropdownTable->eqConfiguration->EQserial = instrumetSerial;
            dropdownTable->eqConfiguration->repaint();
        }
        else if(dropdownTabSerial == 1)//MARK:- Clip
        {
            
        }
        else if(dropdownTabSerial == 2)//MARK:- ENV
        {
            
            dropdownTable->envConfiguration->envelopeMeter->repaint();
            
            if(envSwitches[instrumetSerial])
            {
               dropdownTable->envConfiguration->setSwitchImage(true);
            }
            else
            {
               dropdownTable->envConfiguration->setSwitchImage(false);
            }
            
            dropdownTable->envConfiguration->knobsInitRange();

            dropdownTable->envConfiguration->aSliderValueChange(envAttackCoefficient[instrumetSerial]);
            
            dropdownTable->envConfiguration->dSliderValueChange(envDecayCoefficient[instrumetSerial]);
            
            dropdownTable->envConfiguration->hSliderValueChange(envSustainCoefficient[instrumetSerial]);
            
            dropdownTable->envConfiguration->rSliderValueChange(envReleaseCoefficient[instrumetSerial]);

            dropdownTable->envConfiguration->aBendSliderValueChange(envAttackBendCoefficient[instrumetSerial]);
            
            dropdownTable->envConfiguration->dBendSliderValueChange(envDecayBendCoefficient[instrumetSerial]);
            
            dropdownTable->envConfiguration->sBendSliderValueChange(envSustainBendCoefficient[instrumetSerial]);
            
            dropdownTable->envConfiguration->rBendSliderValueChange(envReleaseBendCoefficient[instrumetSerial]);
            
            DBG("EVN Change");
            
        }
        else if(dropdownTabSerial == 3)//MARK:- FX
        {
            /* Set Compressor Switch Image */
            if(compSwitches[instrumetSerial])
            {
                dropdownTable->effectConfiguration->setSwitchImage(true, 0);//--!!!!!!!!!!!
            }
            else
            {
                dropdownTable->effectConfiguration->setSwitchImage(false, 0);//--!!!!!!!!!!!
            }
            
            /* Set Reverb Switch Image */
            if(reverbSwitches[instrumetSerial])
            {
                dropdownTable->effectConfiguration->setSwitchImage(true, 1);//--!!!!!!!!!!!
            }
            else
            {
                dropdownTable->effectConfiguration->setSwitchImage(false, 1);//--!!!!!!!!!!!
            }
                 
            /* Set Delay Switch Image */
            if(delaySwitches[instrumetSerial])
            {
                dropdownTable->effectConfiguration->setSwitchImage(true, 2);//--!!!!!!!!!!!
            }
            else
            {
                dropdownTable->effectConfiguration->setSwitchImage(false, 2);//--!!!!!!!!!!!
            }
            
            
            //------------ Set Slidervalues ----------------//
            dropdownTable->effectConfiguration->compThreshSliderValueChange(compressorThreshCoefficient[instrumetSerial]);
            dropdownTable->effectConfiguration->compRatioSliderValueChange(compressorRatioCoefficient[instrumetSerial]);
            dropdownTable->effectConfiguration->compAttackSliderValueChange(compressorAttackCoefficient[instrumetSerial]);
            dropdownTable->effectConfiguration->compReleaseSliderValueChange(compressorReleaseCoefficient[instrumetSerial]);
            dropdownTable->effectConfiguration->compGainSliderValueChange(compressorGainCoefficient[instrumetSerial]);
            
            dropdownTable->effectConfiguration->reverbDrySliderValueChange(reverbDryCoefficient[instrumetSerial]);
            dropdownTable->effectConfiguration->reverbPredelaySliderValueChange(reverbPredelayCoefficient[instrumetSerial]);
            dropdownTable->effectConfiguration->reverbDecaySliderValueChange(reverbDecayCoefficient[instrumetSerial]);
            dropdownTable->effectConfiguration->reverbSizeSliderValueChange(reverbSizeCoefficient[instrumetSerial]);
            dropdownTable->effectConfiguration->reverbColorSliderValueChange(reverbColorCoefficient[instrumetSerial]);
               
            dropdownTable->effectConfiguration->delayDryWetSliderValueChange(delayDryWetCoefficient[instrumetSerial]);
            dropdownTable->effectConfiguration->delayTimeSliderValueChange(delayTimeCoefficient[instrumetSerial]);
            dropdownTable->effectConfiguration->delayFeedbackSliderValueChange(delayFeedbackCoefficient[instrumetSerial]);
            dropdownTable->effectConfiguration->delayColorSliderValueChange(delayColorCoefficient[instrumetSerial]);
            dropdownTable->effectConfiguration->delayPanSliderValueChange(delayPanCoefficient[instrumetSerial]);
            
        }

    }
    else
    {
        //std::cout<<"Up"<<std::endl;
        processor.getSampler()->noteOff(1, midiNote, 0, true /*有淡出*/);
        globalOutputMeterL = 0;
        globalOutputMeterR = 0;
//        if (processor.getMidiOutput() != nullptr)
//            processor.getMidiOutput()->sendMessageNow(MidiMessage::noteOff(1, midiNote, 0.f));
    }
    
}



void OrionaudioAudioProcessorEditor::appdirClicked()
{
    //change the below directory
    whichdir = 1;
    changed = 1;
    
}

void OrionaudioAudioProcessorEditor::deskdirClicked()
{
    whichdir = 2;
    changed = 1;
}

void OrionaudioAudioProcessorEditor::downdirClicked()
{
    whichdir = 3;
    changed = 1;
}

void OrionaudioAudioProcessorEditor::musicdirClicked()
{
    whichdir = 4;
    changed = 1;
}

void OrionaudioAudioProcessorEditor::docdirClicked()
{
    whichdir = 5;
    changed = 1;
}

void OrionaudioAudioProcessorEditor::homedirClicked()
{
    whichdir = 6;
    changed = 1;
}

void OrionaudioAudioProcessorEditor::upbutClicked()
{
    filebrowser.goUp();
};

void OrionaudioAudioProcessorEditor::setDefaultSize()
{
    setSize(orion::defaultWidth, orion::defaultHeight);
}


//--------------------------------------------------------------------------


//MARK:- Botton Bar Button Funcs
void OrionaudioAudioProcessorEditor::updateDropDownEQState(bool newState)
{
    DBG((int)newState);
    std::cout<<"dropdownTabSerial: "<< dropdownTabSerial <<std::endl;
    std::cout<<"instrumetSerial: "<< instrumetSerial <<std::endl;
    
    if(newState)
    {
        dropDownClip->setToggleState(false,sendNotificationSync);
        dropDownENV->setToggleState(false,sendNotificationSync);
        dropDownFX->setToggleState(false,sendNotificationSync);
        dropDownVisible = newState;
        dropdownTable->setVisible(dropDownVisible);//--!!!!!!!!!!!
        dropdownTabSerial = 0;
    }
    else
    {
        dropDownVisible = newState;
        dropdownTable->setVisible(dropDownVisible);//--!!!!!!!!!!!
        if(!dropDownVisible){dropdownTabSerial = -1;};
    }
    dropdownTable->tabChange(0);//--!!!!!!!!!!!
    resized();
}




void OrionaudioAudioProcessorEditor::updateDropDownClipState(bool newState)
{
    DBG((int)newState);
    
    std::cout<<"dropdownTabSerial: "<< dropdownTabSerial <<std::endl;
    std::cout<<"instrumetSerial: "<< instrumetSerial <<std::endl;
    
    if(newState)
    {
        dropDownEQ->setToggleState(false,sendNotificationSync);
        dropDownENV->setToggleState(false,sendNotificationSync);
        dropDownFX->setToggleState(false,sendNotificationSync);
        dropDownVisible = newState;
        dropdownTable->setVisible(dropDownVisible);//--!!!!!!!!!!!
        dropdownTabSerial = 1;
    }
    else
    {
        dropDownVisible = newState;
        dropdownTable->setVisible(dropDownVisible);//--!!!!!!!!!!!
        if(!dropDownVisible){dropdownTabSerial = -1;};
    }
    dropdownTable->tabChange(1);//--!!!!!!!!!!!
    resized();
}


void OrionaudioAudioProcessorEditor::updateDropDownENVState(bool newState)
{
    DBG((int)newState);
    
    std::cout<<"dropdownTabSerial: "<< dropdownTabSerial <<std::endl;
    std::cout<<"instrumetSerial: "<< instrumetSerial <<std::endl;
    
    if(newState)
    {
        dropDownEQ->setToggleState(false,sendNotificationSync);
        dropDownClip->setToggleState(false,sendNotificationSync);
        dropDownFX->setToggleState(false,sendNotificationSync);
        dropDownVisible = newState;
        dropdownTable->setVisible(dropDownVisible);//--!!!!!!!!!!!
        dropdownTabSerial = 2;
    }
    else
    {
        dropDownVisible = newState;
        dropdownTable->setVisible(dropDownVisible);//--!!!!!!!!!!!
        if(!dropDownVisible){dropdownTabSerial = -1;};
    }
    dropdownTable->tabChange(2);//--!!!!!!!!!!!
    resized();
}


void OrionaudioAudioProcessorEditor::updateDropDownFXState(bool newState)
{
    DBG((int)newState);
    
    std::cout<<"dropdownTabSerial: "<< dropdownTabSerial <<std::endl;
    std::cout<<"instrumetSerial: "<< instrumetSerial <<std::endl;
    
    if(newState)
    {
        dropDownEQ->setToggleState(false,sendNotificationSync);
        dropDownClip->setToggleState(false,sendNotificationSync);
        dropDownENV->setToggleState(false,sendNotificationSync);
        dropDownVisible = newState;
        dropdownTable->setVisible(dropDownVisible);//--!!!!!!!!!!!
        dropdownTabSerial = 3;
    }
    else
    {
        dropDownVisible = newState;
        dropdownTable->setVisible(dropDownVisible);//--!!!!!!!!!!!
        if(!dropDownVisible){dropdownTabSerial = -1;};
    }
    
    
    dropdownTable->tabChange(3);//--!!!!!!!!!!!
    resized();
}


//void OrionaudioAudioProcessorEditor::updateDropDownState(bool newState)
//{
//    DBG((int)newState);
//
//    std::cout<<"dropdownTabSerial: "<< dropdownTabSerial <<std::endl;
//    std::cout<<"instrumetSerial: "<< instrumetSerial <<std::endl;
//
//    if(newState)
//    {
//        if(dropdownTabSerial == 0)//EQ
//        {
//            dropDownClip->setToggleState(false,sendNotificationSync);
//            dropDownENV->setToggleState(false,sendNotificationSync);
//            dropDownFX->setToggleState(false,sendNotificationSync);
//        }
//        else if(dropdownTabSerial == 1)//Clip
//        {
//            dropDownEQ->setToggleState(false,sendNotificationSync);
//            dropDownENV->setToggleState(false,sendNotificationSync);
//            dropDownFX->setToggleState(false,sendNotificationSync);
//        }
//        else if(dropdownTabSerial == 2)//Envelope
//        {
//            dropDownClip->setToggleState(false,sendNotificationSync);
//            dropDownEQ->setToggleState(false,sendNotificationSync);
//            dropDownFX->setToggleState(false,sendNotificationSync);
//        }
//        else if(dropdownTabSerial == 3)//FX
//        {
//            dropDownClip->setToggleState(false,sendNotificationSync);
//            dropDownENV->setToggleState(false,sendNotificationSync);
//            dropDownEQ->setToggleState(false,sendNotificationSync);
//        }
//
//        tabComponents->tabComponents[instrumetSerial]->setCurrentTabIndex(dropdownTabSerial);//--!!!!!!!!!!!
//        dropdownTable->tabChange(dropdownTabSerial);//--!!!!!!!!!!!
//
//        dropDownVisible = newState;
//        tabComponents->setVisible(dropDownVisible);//--!!!!!!!!!!!
//        dropdownTable->setVisible(dropDownVisible);//--!!!!!!!!!!!
//    }
//    else
//    {
//        dropDownVisible = newState;
//        tabComponents->setVisible(dropDownVisible);
//        tabComponents->setVisible(dropDownVisible);//--!!!!!!!!!!!
//        dropdownTable->setVisible(dropDownVisible);//--!!!!!!!!!!!
//    }
//
//    resized();
//}


//--------------------------------------------------------------------------





void OrionaudioAudioProcessorEditor::updateResizeViewState(bool newState)
{
    DBG((int)newState);
    resized();
}

void OrionaudioAudioProcessorEditor::toggleArrangmentWindow(bool windowVisible)
{
    arrangementWindowVisible = windowVisible;
    arrangementWindow->setVisible(windowVisible);
    primaryPane->setVisible(!windowVisible);
    resized();
    
    DBG("--------------------------------------");
}

bool OrionaudioAudioProcessorEditor::getDropdownVisible()
{
    return dropDownVisible;
}
