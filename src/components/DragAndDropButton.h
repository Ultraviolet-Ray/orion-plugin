/*
  ==============================================================================

    DragAndDropButton.h
    Created: 6 Feb 2020 4:34:15pm
    Author:  Quin Scacheri

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalCoefficients.h"
#include "PluginProcessor.h"


//==============================================================================
/*
*/
class DragAndDropButton : public ImageButton,
                          public FileDragAndDropTarget
{
public:
    
    DragAndDropButton(OrionaudioAudioProcessor* p,const String n);
    ~DragAndDropButton();
    
    void paint(Graphics& g) override;
    void resized() override;
 
    int tag = 0;
    String name = "";

    bool isInterestedInFileDrag(const juce::StringArray &files) override { return true; }
    
    void filesDropped(const juce::StringArray &files, int x, int y) override;
    
    //std::function<void(File)> onFilesDropped;
private:
    
    OrionaudioAudioProcessor* processor;
    
    
    AudioFormatManager audioFormatManager;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DragAndDropButton)
};
