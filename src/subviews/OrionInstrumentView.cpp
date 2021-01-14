/*
  ==============================================================================

    OrionInstrumentView.cpp
    Created: 16 Aug 2020 12:41:18pm
    Author:  Lei Li

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"
#include "OrionInstrumentView.h"
//#include "PluginEditor.h"
OrionInstrumentView::OrionInstrumentView()//OrionaudioAudioProcessorEditor* e
{
    //processor = p;
    //editor = e;
    downImages[0] = ImageCache::getFromMemory(BinaryData::KickOn_png, BinaryData::KickOn_pngSize);
    downImages[1] = ImageCache::getFromMemory(BinaryData::SnareOn_png, BinaryData::SnareOn_pngSize);
    downImages[2] = ImageCache::getFromMemory(BinaryData::ClapOn_png, BinaryData::ClapOn_pngSize);
    downImages[3] = ImageCache::getFromMemory(BinaryData::PercOn_png, BinaryData::PercOn_pngSize);
    downImages[4] = ImageCache::getFromMemory(BinaryData::SnapOn_png, BinaryData::SnapOn_pngSize);
    downImages[5] = ImageCache::getFromMemory(BinaryData::HHOOn_png, BinaryData::HHOOn_pngSize);
    downImages[6] = ImageCache::getFromMemory(BinaryData::HHCOn_png, BinaryData::HHCOn_pngSize);
    downImages[7] = ImageCache::getFromMemory(BinaryData::CrashOn_png, BinaryData::CrashOn_pngSize);
    
    upImages[0] = ImageCache::getFromMemory(BinaryData::KickOff_png, BinaryData::KickOff_pngSize);
    upImages[1] = ImageCache::getFromMemory(BinaryData::SnareOff_png, BinaryData::SnareOff_pngSize);
    upImages[2] = ImageCache::getFromMemory(BinaryData::ClapOff_png, BinaryData::ClapOff_pngSize);
    upImages[3] = ImageCache::getFromMemory(BinaryData::PercOff_png, BinaryData::PercOff_pngSize);
    upImages[4] = ImageCache::getFromMemory(BinaryData::SnapOff_png, BinaryData::SnapOff_pngSize);
    upImages[5] = ImageCache::getFromMemory(BinaryData::HHOOff_png, BinaryData::HHOOff_pngSize);
    upImages[6] = ImageCache::getFromMemory(BinaryData::HHCOff_png, BinaryData::HHCOff_pngSize);
    upImages[7] = ImageCache::getFromMemory(BinaryData::CrashOff_png, BinaryData::CrashOff_pngSize);
    

    
    for (int i = 0; i < drumButtonCoverImageViews.size(); i++)
    {
        //drumButtons[i].reset(new DragAndDropButton());
        //addAndMakeVisible(drumButtons[i].get());
        drumButtonCoverImageViews[i].reset(new DrawableImage());
        drumButtonCoverImageViews[i]->setImage(upImages[i]);
        addAndMakeVisible(drumButtonCoverImageViews[i].get());
        drumButtonCoverImageViews[i]->setVisible(true);
    }
    

   
   
    startTimerHz(30);
    instrumentOnArray = instrumentsOnStates;
    
}


//MARK:- PAINT
void OrionInstrumentView::paint (Graphics& g)
{
    //g.setColour (juce::Colours::lightyellow);
    //g.drawRect (getLocalBounds().toFloat(),  1);
}

//MARK:- RESIZE
void OrionInstrumentView::resized()
{
    Rectangle<int> area;
    
    double padWidth = getWidth()/5.5;
    double padHeight = getHeight()/2;
    
    area.setSize(padWidth, padHeight);
    
    int drumCount = 0;
    
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            area.setPosition(j * padWidth * 1.5, i * padHeight);
            drumButtonCoverImageViews[drumCount]->setBounds(area);
     
            drumButtonCoverImageViews[drumCount]->setTransformToFit(area.toFloat(), RectanglePlacement::stretchToFit);
            drumCount++;
        }
    }
    
    
    
}


void OrionInstrumentView::timerCallback()
{
    for (int i = 0; i < instrumentAmount; i++)
    {
        if( *(instrumentOnArray + i))
        {
            //drumButtonCoverImageViews[i]->setVisible(true);
            drumButtonCoverImageViews[i]->setImage(downImages[i]);
        }
        else
        {
            drumButtonCoverImageViews[i]->setImage(upImages[i]);
        }
    }
}


//MARK:- 析构函数
OrionInstrumentView::~OrionInstrumentView()
{
    
}



/*
 for (int i = 0; i < instrumentAmount; i++)
 {
     if( *(instrumentOnArray + i))
     {
         drumButtonCoverImageViews[i]->setVisible(true);
     }
     else
     {
         drumButtonCoverImageViews[i]->setVisible(false);
     }
 }
 
 */
