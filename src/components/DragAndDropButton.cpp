/*
  ==============================================================================

    DragAndDropButton.cpp
    Created: 12 Jan 2021 3:30:43pm
    Author:  Lei Li

  ==============================================================================
*/

#include "DragAndDropButton.h"

//#include "main.cpp"
DragAndDropButton::DragAndDropButton(OrionaudioAudioProcessor* p,const String n) : ImageButton(n)
{
    tag = 0;
    
    name = n;
    
    processor = p;
    
    audioFormatManager.registerBasicFormats();
}


DragAndDropButton::~DragAndDropButton()
{
    
}


void DragAndDropButton::paint (Graphics& g)
{
    //std::cout<<"paint Clip : "<<instrumetClickedSerial<<std::endl;
}

void DragAndDropButton::resized()
{
    
}



void DragAndDropButton::filesDropped(const juce::StringArray &files, int x, int y) 
{
    
    
    //DBG("Droped", String(files.p));
    instrumentSamplePathes[tag] = files[0];
    std::unique_ptr<AudioFormatReader> reader;
    
    //AudioFormatReader reader;
    
    DBG("path name: " + String(instrumentSamplePathes[tag].getFullPathName()));
    
//    OrionSamplerSound *sampler = new OrionSamplerSound(String(i), *reader.get(), note/* Note Range */, MidiNote/* Center Note */, 0.0f, 10.0f, 10.0f);
//
//    //sampler->reverseSource();
//
//
//
//    instrumentSampleBufferPointer[i] = sampler->getAudioData();
//    instrumentOriginalSampleContainer[i] = *instrumentSampleBufferPointer[i];
    
    reader.reset(audioFormatManager.createReaderFor(instrumentSamplePathes[tag]));
    if (reader == nullptr)
    {
        return;
    }
    else
    {
        if (reader.get() != nullptr)
        {
            auto duration = (float) reader->lengthInSamples / reader->sampleRate;

            if (duration > 2)
            {
                int numChannels = reader->numChannels;
                int sampleLength = int(reader->lengthInSamples);
                
                DBG("numChannels: "+ (String)reader->numChannels + " " + " lengthInSamples: " + (String) reader->lengthInSamples);
                
                instrumentOriginalSampleContainer[tag].setSize ((int) reader->numChannels, (int) reader->lengthInSamples);
                
                instrumentSampleContainer[tag].setSize ((int) reader->numChannels, (int) reader->lengthInSamples);
                
                reader->read (&instrumentOriginalSampleContainer[tag],
                              0,
                              (int) reader->lengthInSamples,
                              0,
                              true,
                              true);
                
                instrumentSampleBufferPointer[tag]->setSize(numChannels, sampleLength,/* keepExistingContent: */false,/* clearExtraSpace: */true,/* avoidReallocating: */false);
                instrumentSampleBufferPointer[tag]->setDataToReferTo(instrumentSampleContainer[instrumetClickedSerial].getArrayOfWritePointers(), numChannels, 0, sampleLength);
                
                // Update To the Part of Selected Audio
                //processor->audioRangeChange(tag);
                //SEPointSlider->audioRangeChange();
                //processor->editor
                
                // Update Audio Image
                //clipMeter->repaint();
            }
            else
            {
                // handle the error that the file is 2 seconds or longer..
                DBG("error, sample is too short");
            }
        }
                    
    }
        

    
    
    instrumentSampleContainer[tag] = instrumentOriginalSampleContainer[tag];
    if (auto* sound = dynamic_cast<OrionSamplerSound*> (processor->sampler->getSound(instrumetClickedSerial).get()))
    {
        sound->setLength(instrumentSampleContainer[instrumetClickedSerial].getNumSamples());
    }
    
    //BigInteger note;
    //note.setBit(MidiNote);
    
    //OrionSamplerSound *sampler = new OrionSamplerSound(String(tag), *reader.get(), note/* Note Range */, MidiNote/* Center Note */, 0.0f, 10.0f, 10.0f);
    

    //instrumentSampleBufferPointer[tag] = sampler->getAudioData();
    instrumentOriginalSampleContainer[tag] = *instrumentSampleBufferPointer[tag];
    instrumentSampleContainer[tag] = instrumentOriginalSampleContainer[tag];
  
    //addSound(sampler);
//        if(auto* voice = dynamic_cast<OrionSamplerVoice*>(processor.getSampler()->getVoice(instrumetClickedSerial)))
//        {
//            voice->imagerSharp = imagerSharpCoefficient[instrumetClickedSerial];
//        }
    
    //onFilesDropped(files[0]);
}
