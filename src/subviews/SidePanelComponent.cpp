/*
  ==============================================================================

    SidePanelComponent.cpp
    Created: 6 Feb 2020 2:08:33pm
    Author:  Quin Scacheri

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SidePanelComponent.h"
#include "PluginEditor.h"

//==============================================================================
SidePanelComponent::SidePanelComponent(OrionaudioAudioProcessor* p, OrionaudioAudioProcessorEditor* e)
{
    processor = p;
    editor = e;

    // Background Image
    
    backgroundImageView.reset(new DrawableImage());
    addAndMakeVisible(backgroundImageView.get());
    Image backgroundImage = ImageCache::getFromMemory(BinaryData::sidePanelBackground_png, BinaryData::sidePanelBackground_pngSize);
    backgroundImageView->setImage(backgroundImage);
    
    // Logo Image
    logoImage.reset(new ImageComponent());
    logoImage->setImage(ImageCache::getFromMemory(BinaryData::logo_png, BinaryData::logo_pngSize), RectanglePlacement::fillDestination);
    addAndMakeVisible(logoImage.get());
    
    // FileBrowser Image
    fileBrowser.reset(new DraggableFileBrowserComponent());
    addAndMakeVisible(fileBrowser.get());
    
}

SidePanelComponent::~SidePanelComponent()
{
    
}

void SidePanelComponent::paint (Graphics& g)
{
    //const String colourString ("404041");
    //const Colour colour (Colour::fromString ("FF" + colourString));
    //g.setColour(colour);
    //g.fillAll();
    
    g.setColour (juce::Colours::lightyellow);
    g.drawRect (getLocalBounds().toFloat(),  0.5);
    
}

void SidePanelComponent::resized()
{
    
    //Background
    Rectangle<int> backgroundArea(0, 0, getWidth(), getHeight());
    backgroundImageView->setTransformToFit(backgroundArea.toFloat(), RectanglePlacement::stretchToFit);
    
    //auto area = getLocalBounds();
    //auto logoArea = area.removeFromTop(getHeight() / 12);
    
    float unite = getHeight()/33;
    
    auto logoArea = Rectangle<int>(0, 0, getWidth(), 2.9 * unite);
    
    logoImage->setBounds(logoArea);
    
    Rectangle<int> fileBrowerframeSize(0, 3 * unite, getWidth(), getHeight() * 0.86);
    fileBrowser->setBounds(fileBrowerframeSize);
    
    
}
