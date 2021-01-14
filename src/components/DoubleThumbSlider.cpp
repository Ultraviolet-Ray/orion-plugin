/*
  ==============================================================================

    DoubleThumbSlider.cpp
    Created: 15 Sep 2020 12:29:24pm
    Author:  Lei Li

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DoubleThumbSlider.h"
#include "OrionClipConfiguration.h"


DoubleThumbSlider::DoubleThumbSlider(ClipMeter* cm,OrionaudioAudioProcessor& p) : processor(p)
{
    ellipseDiameter = getHeight();
    TLX = 0.0f;
    TRX = getWidth() - ellipseDiameter;
    meter = cm;
}

void DoubleThumbSlider::paint (Graphics& g)
{
    //g.fillAll();
    //g.setColour (juce::Colours::white);
    //g.drawRect (getLocalBounds().toFloat(),  0.5);
    //std::cout<<"Paint Clip Meter: "<<instrumetClickedSerial<<std::endl;

    //------------------------------------ Draw Waveform -------------------------------------//
    /* Draw Thumb */
    ellipseDiameter = getHeight();
    g.setColour (juce::Colours::white);
    g.fillEllipse (TLX,getHeight() - ellipseDiameter, ellipseDiameter, ellipseDiameter);
    g.fillEllipse (TRX,getHeight() - ellipseDiameter, ellipseDiameter, ellipseDiameter);

}


void DoubleThumbSlider::resized()
{
    ellipseDiameter = getHeight();
    TLX = 0.0f;
    TRX = getWidth() - ellipseDiameter;
    DBG("Resize");
}



void DoubleThumbSlider::mouseDown (const MouseEvent& e)
{
    DBG("Mouse Down");
    
    if (e.x >= TLX - ellipseDiameter && e.x <= TLX + ellipseDiameter)
    {
        TLDown = true;
    }
    
    
    if (e.x >= TRX - ellipseDiameter && e.x <= TRX + ellipseDiameter)
    {
        TRDown = true;
    }

}

void DoubleThumbSlider::mouseUp (const MouseEvent& e)
{
    DBG("Mouse Up");
    TLDown = false;
    TRDown = false;
    valueChange();
    audioRangeChange();
}

void DoubleThumbSlider::audioRangeChange()
{
    
    
    
    processor.audioRangeChange(instrumetClickedSerial);
    
//    int originalLength = instrumentSampleContainer[instrumetClickedSerial].getNumSamples();
//    int originalChannelNum = instrumentSampleContainer[instrumetClickedSerial].getNumChannels();
//
//    int newSampleLength = RVal[instrumetClickedSerial] * originalLength - LVal[instrumetClickedSerial] * originalLength;
//
//
//    instrumentSampleBufferPointer[instrumetClickedSerial]->setSize(originalChannelNum, newSampleLength,/* keepExistingContent: */false,/* clearExtraSpace: */true,/* avoidReallocating: */false);
//
//    instrumentSampleBufferPointer[instrumetClickedSerial]->setDataToReferTo(instrumentSampleContainer[instrumetClickedSerial].getArrayOfWritePointers(),
//                                                                            originalChannelNum, LVal[instrumetClickedSerial] * originalLength,
//                                                                            RVal[instrumetClickedSerial] * originalLength);
//
//
//    if (auto* sound = dynamic_cast<OrionSamplerSound*> (processor.sampler->getSound(instrumetClickedSerial).get()))
//    {
//        sound->setLength(newSampleLength);
//    }
    
    //*instrumentSampleLength[instrumetClickedSerial] = newSampleLength;
    
}


void DoubleThumbSlider::mouseDrag (const MouseEvent& e)
{
    if(TLDown && !TRDown)
    {
        if(e.x > TRX)
        {
            TLX = TRX;
        }
        else if(e.x < 0.0f)
        {
            TLX = 0.0f;
        }
        else
        {
            TLX = e.x - ellipseDiameter/2;
        }
        repaint();
        valueChange();
    }
    
    if(TRDown && !TLDown)
    {
        if(e.x > getWidth() - ellipseDiameter)
        {
            TRX = getWidth() - ellipseDiameter;
        }
        else if(e.x < TLX)
        {
            TRX = TLX;
        }
        else
        {
            TRX = e.x - ellipseDiameter/2;
        }
        repaint();
        valueChange();
    }
    
    if(TRDown && TLDown)
    {
        
        if(e.x > TRX + ellipseDiameter/2)
        {
            if(e.x > getWidth() - ellipseDiameter)
            {
                TRX = getWidth() - ellipseDiameter;
            }
            else if(e.x < TLX)
            {
                TRX = TLX;
            }
            else
            {
                TRX = e.x - ellipseDiameter/2;
            }
        }
        
        if(e.x < TLX + ellipseDiameter/2)
        {
            if(e.x > TRX)
            {
                TLX = TRX;
            }
            else if(e.x < 0.0f)
            {
                TLX = 0.0f;
            }
            else
            {
                TLX = e.x - ellipseDiameter/2;
            }
        }
        repaint();
        valueChange();
    }
}


void DoubleThumbSlider::valueChange()
{
    LVal[instrumetClickedSerial] = jmap<float>(TLX, 0.0f, getWidth() - ellipseDiameter, 0.0f,1.0f);
    RVal[instrumetClickedSerial] = jmap<float>(TRX, 0.0f, getWidth() - ellipseDiameter, 0.0f,1.0f);
    
    if(LValPtr != nullptr)
    {
      *LValPtr = LVal[instrumetClickedSerial];
    }
    
    if(RValPtr != nullptr)
    {
      *RValPtr = RVal[instrumetClickedSerial];
    }
    
    meter->updateZone();
    
}

void DoubleThumbSlider::initThumb(float* LValIn, float* RValIn)
{
    if(LValIn != nullptr)
    {
        LVal[instrumetClickedSerial] = *LValIn;
        TLX = jmap<float>(*LValIn, 0.0f,1.0f, 0.0f, getWidth() - ellipseDiameter);
        LValPtr = LValIn;
    }
    
    if(RValIn != nullptr)
    {
        RVal[instrumetClickedSerial] = *RValIn;
        TRX = jmap<float>(*RValIn, 0.0f,1.0f, 0.0f, getWidth() - ellipseDiameter);
        RValPtr = RValIn;
    }
    
    repaint();
}


//MARK:- Destructor
DoubleThumbSlider::~DoubleThumbSlider()
{
    
}
