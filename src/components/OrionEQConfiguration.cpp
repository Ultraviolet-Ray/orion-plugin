/*
 Copyright (c) 2018, Daniel Walz - Foleys Finest Audio UG All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
 Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 
 Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 
 Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
  ==============================================================================

    OrionEQConfiguration.cpp
    Created: 11 Jun 2019 9:44:35pm
    Author:  Lei Li

  ==============================================================================
*/



#include "../JuceLibraryCode/JuceHeader.h"
#include "OrionEQConfiguration.h"
#include "OrionGlobalVars.h"

static float maxDB       = 24.0f;
static int   clickRadius = 4;
//==============================================================================
OrionEQConfiguration::OrionEQConfiguration(OrionaudioAudioProcessor& p,int serial) : processor(p)
{
    EQserial = serial;
    
    //-------------------------------- Background Image -------------------------------//
//    Image backgroundImage = ImageCache::getFromMemory(BinaryData::EQBackground_png, BinaryData::EQBackground_pngSize);
//    backgroundImageView.reset(new DrawableImage());
//    backgroundImageView->setImage(backgroundImage);
//    addAndMakeVisible(backgroundImageView.get());
    
    
    frame.setText (TRANS ("Output"));
    frame.setTextLabelPosition (Justification::centred);
    addAndMakeVisible (frame);
    updateFrequencyResponses();
    Image eqbackground = ImageCache::getFromMemory(BinaryData::EQ_Background_png, BinaryData::EQ_Background_pngSize);
    
    for (int i = 0; i < freqLabels.size(); i++)
    {
        freqLabels[i].reset(new Label());
        freqLabels[i].reset(new Label(freqLabelStrings[i], freqLabelStrings[i]));
        addAndMakeVisible(freqLabels[i].get());
        freqLabels[i]->setColour(Label::textColourId, freqLabelColor);
    }
    
    

}

void OrionEQConfiguration::paint(Graphics& g)
{
   DBG("paint EQ");
    
    Graphics::ScopedSaveState state (g);
    
    //RectanglePlacement orionBackgroundRectanglePlacement(64);
    
    g.fillAll (Colours::black.withAlpha((.7f)));
    
    /* Draw Frame */
//    g.setFont (12.0f);
//    g.setColour (Colours::silver);
//    g.drawRoundedRectangle (plotFrame.toFloat(), 5, 2);
    
    // Upper Line
    g.setColour (Colours::whitesmoke);
    //g.drawHorizontalLine(0, 0, getWidth());
    
    g.drawLine (0, 0, getWidth(), 0, 0.5);
    
    /* Vertical Lines */
    for (int i=0; i < 70; ++i)
    {
        g.setColour (Colours::white/*.withAlpha (0.75f)*/);
        auto x = plotFrame.getX() + plotFrame.getWidth() * i * 0.0160f;
        
        if (i > 0)
        {
            //g.drawVerticalLine (roundToInt (x), plotFrame.getY(), plotFrame.getBottom());
            g.drawLine (roundToInt (x), plotFrame.getY(), roundToInt (x), plotFrame.getBottom(), 0.25);
        }
        
        
        
         //Draw Freq Text
        //g.setColour (Colours::silver);
//        auto freq = getFrequencyForPosition (i * 0.025f);
//        if(abs(round(freq) - freq) < 0.000000000000001)
//        {
//            g.drawFittedText ((freq < 1000) ? String (freq) + " Hz" : String (freq / 1000, 1) + " kHz",roundToInt (x + 3), getHeight()/20, 50, 15, Justification::left, 1);
//        }
        
    }
    
    //Horizontal Lines
    g.setColour (Colours::silver.withAlpha (0.3f));
    //g.drawHorizontalLine (roundToInt (plotFrame.getY() + 0.25 * plotFrame.getHeight()), plotFrame.getX(), plotFrame.getRight());
    //g.drawHorizontalLine (roundToInt (plotFrame.getY() + 0.75 * plotFrame.getHeight()), plotFrame.getX(), plotFrame.getRight());
    g.setColour (Colours::silver);
    
    //g.drawFittedText (String (maxDB) + " dB", plotFrame.getX() + 3, plotFrame.getY() + 2, 50, 14, Justification::left, 1);
    //g.drawFittedText (String (maxDB / 2) + " dB", plotFrame.getX() + 3, roundToInt (plotFrame.getY() + 2 + 0.25 * plotFrame.getHeight()), 50, 14, Justification::left, 1);
    //g.drawFittedText (" 0 dB", plotFrame.getX() + 3, roundToInt (plotFrame.getY() + 2 + 0.5  * plotFrame.getHeight()), 50, 14, Justification::left, 1);
    // g.drawFittedText (String (- maxDB / 2) + " dB", plotFrame.getX() + 3, roundToInt (plotFrame.getY() + 2 + 0.75 * plotFrame.getHeight()), 50, 14, Justification::left, 1);
    
    
     //plot input and output audio data
    const Colour inputColour = Colours::greenyellow;
    const Colour outputColour = Colours::indianred;
    g.setFont (16.0f);
    processor.createAnalyserPlot (analyserPath, plotFrame, 20.0f, true);
    g.setColour (inputColour);
    //g.drawFittedText ("Input", plotFrame.reduced (8), Justification::topRight, 1);
    g.strokePath (analyserPath, PathStrokeType (1.0));
    processor.createAnalyserPlot (analyserPath, plotFrame, 20.0f, false);
    g.setColour (outputColour);
    //g.drawFittedText ("Output", plotFrame.reduced (8, 28), Justification::topRight, 1);
    g.strokePath (analyserPath, PathStrokeType (1.0));
    
    
    updateFrequencyResponses();
    
    
    //******** Filter Plots *************
    for (int i=0; i < 5; ++i)
    {
        if(auto* voice = dynamic_cast<OrionSamplerVoice*> (processor.getSampler()->getVoice(EQserial)))
        {
            auto band = voice->eq.bands[i];
            //g.setColour (band.active ? band.colour : band.colour.withAlpha (0.3f));
            //std::cout<<"is frequency response empty "<<band.frequencyResponse.getLength()<<" "<<EQserial<<"\n";
            //g.strokePath (band.frequencyResponse, PathStrokeType (2.0));
            g.setColour (draggingBand == int (i) ? band.colour : band.colour.withAlpha (0.3f));
            auto x = roundToInt (plotFrame.getX() + plotFrame.getWidth() * getPositionForFrequency (float (band.frequency)));
            auto y = roundToInt (getPositionForGain (float (band.gain), plotFrame.getY(), plotFrame.getBottom()));
        
            /*
            g.drawLine(x, plotFrame.getY(), x, y - 5, 10.0f);//画五条竖线，可以拖filter frequency, 上半部分
            g.drawLine(x, y + 5, x, plotFrame.getBottom(), 10.0f);//画五条竖线，可以拖filter frequency, 下半部分
             */
            
            g.setColour (freqLabelColor);
            g.setOpacity(1);
            g.fillEllipse (x - 3, y - 3, 6/* R-W */, 6/* R-H */);//调圆点大小
        }
    }
    
    //g.setColour (Colours::darkcyan);
    g.setColour (freqLabelColor);
    
    /* Draw the Frequency Curve */
    g.strokePath (frequencyResponse, PathStrokeType (1.0));
    
    /* Set Opacity */
    g.setOpacity(0.5);
    
    /* ADD SHADING */
    g.fillPath(frequencyResponse);
    //    PathFlatteningIterator i (frequencyResponse);
    //    while (i.next())
    //    {
    //        //g.drawLine(i.x2, i.y2, i.x2, getHeight()/2, 4.0f);//画IR阴影
    //        g.drawLine(i.x1, i.y1, i.x1, getHeight()/2, i.x2 - i.x1);//画IR阴影
    //    }
}



void OrionEQConfiguration::resized()
{
  
    
    Rectangle<int> area = getLocalBounds();
    //backgroundImageView->setTransformToFit(area.toFloat(), RectanglePlacement::stretchToFit);
    
    //plotFrame = getLocalBounds();//.reduced (3, 3);
    plotFrame = Rectangle<int>(0, 0,  getWidth(), getHeight() * 0.925);
    
    updateFrequencyResponses();
    float uniteW = getWidth()/100;
    

    /* 30 */
    area = Rectangle<int>( 2 * uniteW, 0.5 * uniteW,  6 * uniteW, 2.25 * uniteW);
    freqLabels[0]->setBounds(area);
    /* 50 */
    area = Rectangle<int>(13 * uniteW, 0.5 * uniteW,  6 * uniteW, 2.25 * uniteW);
    freqLabels[1]->setBounds(area);
    /* 100 */
    area = Rectangle<int>(24 * uniteW, 0.5 * uniteW,  6 * uniteW, 2.25 * uniteW);
    freqLabels[2]->setBounds(area);
    /* 200 */
    area = Rectangle<int>(35 * uniteW, 0.5 * uniteW,  6 * uniteW, 2.25 * uniteW);
    freqLabels[3]->setBounds(area);
    /* 500 */
    area = Rectangle<int>(48 * uniteW, 0.5 * uniteW,  6 * uniteW, 2.25 * uniteW);
    freqLabels[4]->setBounds(area);
    /* 1K */
    area = Rectangle<int>(60 * uniteW, 0.5 * uniteW,  6 * uniteW, 2.25 * uniteW);
    freqLabels[5]->setBounds(area);
    /* 2K */
    area = Rectangle<int>(72 * uniteW, 0.5 * uniteW,  6 * uniteW, 2.25 * uniteW);
    freqLabels[6]->setBounds(area);
    /* 5K */
    area = Rectangle<int>(getWidth() - 24 * uniteW, 0.5 * uniteW,  6 * uniteW, 2.25 * uniteW);
    freqLabels[7]->setBounds(area);
    /* 10K */
    area = Rectangle<int>(getWidth() - 13 * uniteW, 0.5 * uniteW,  6 * uniteW, 2.25 * uniteW);
    freqLabels[8]->setBounds(area);
    /* 20K */
    area = Rectangle<int>(getWidth() - 3.1 * uniteW, 0.5 * uniteW,  6 * uniteW, 2.25 * uniteW);
    freqLabels[9]->setBounds(area);
    

    
}

OrionEQConfiguration::~OrionEQConfiguration()
{
    
}

float OrionEQConfiguration::getFrequencyForPosition (float pos)
{
    return 20.0f * std::pow (2.0f, pos * 10.0f);
}

float OrionEQConfiguration::getPositionForFrequency (float freq)
{
    return (std::log (freq / 20.0f) / std::log (2.0f)) / 10.0f;
}

float OrionEQConfiguration::getPositionForGain (float gain, float top, float bottom)
{
    return jmap (Decibels::gainToDecibels (gain, -maxDB), -maxDB, maxDB, bottom, top);
}

float OrionEQConfiguration::getGainForPosition (float pos, float top, float bottom)
{
    return Decibels::decibelsToGain (jmap (pos, bottom, top, -maxDB, maxDB), -maxDB);
}


void OrionEQConfiguration::mouseMove (const MouseEvent& e)
{
    if (plotFrame.contains (e.x, e.y))
    {
        for (int i=0; i < 5; ++i)
        {
            if(auto* voice = dynamic_cast<OrionSamplerVoice*> (processor.getSampler()->getVoice(EQserial)))
            {
                auto pos = plotFrame.getX() + getPositionForFrequency (float (voice->eq.bands[i].frequency)) * plotFrame.getWidth();
                
                if (std::abs (pos - e.position.getX()) < clickRadius)
                {
                    if (std::abs (getPositionForGain (float (voice->eq.bands[i].gain), plotFrame.getY(), plotFrame.getBottom())
                                  - e.position.getY()) < clickRadius)
                    {
                        draggingGain = true;
                        setMouseCursor (MouseCursor (MouseCursor::UpDownLeftRightResizeCursor));
                    }
                    else
                    {
                        setMouseCursor (MouseCursor (MouseCursor::LeftRightResizeCursor));
                    }
                    
                    if (i != draggingBand)
                    {
                        draggingBand = i;
                        repaint (plotFrame);
                    }
                    return;
                }
            }
        }
    }
    draggingBand = -1;
    draggingGain = false;
    setMouseCursor (MouseCursor (MouseCursor::NormalCursor));
    repaint (plotFrame);
}

void OrionEQConfiguration::mouseDown (const MouseEvent& e)
{
    if (! e.mods.isPopupMenu() || ! plotFrame.contains (e.x, e.y))
        return;
    
    for (int i=0; i < 5; ++i)
    {
        if(auto* voice = dynamic_cast<OrionSamplerVoice*> (processor.getSampler()->getVoice(EQserial)))
        {
            if (std::abs (plotFrame.getX() + getPositionForFrequency (int (voice->eq.bands[i].frequency)) * plotFrame.getWidth()
                          - e.position.getX()) < clickRadius)
            {
                
                contextMenu.clear();
                const auto& names = voice->getFilterTypeNames();
                for (int t=0; t < names.size(); ++t)
                    contextMenu.addItem (t + 1, names [t], true, voice->eq.bands[i].type == t);
                
                contextMenu.showMenuAsync (PopupMenu::Options()
                                           .withTargetComponent (this)
                                           .withTargetScreenArea ({e.getScreenX(), e.getScreenY(), 1, 1})
                                           , [this, i,voice](int selected)
                                           {
                                               if (selected > 0)
                                               {
                                                   if(voice->eq.bands[i].type != voice->getTypefromindex(selected - 1))
                                                   {
                                                       voice->eq.bands[i].type = voice->getTypefromindex(selected - 1);
                                                       voice->eq.updateBand(draggingBand);
                                                       voice->updatePlots();
                                                       repaint (plotFrame);
                                                   }
                                               }
                                           });
                return;
            }
        }
    }
}


void OrionEQConfiguration::mouseDrag (const MouseEvent& e)
{
    if (isPositiveAndBelow(draggingBand, 5))
    {
        //std::cout<<"are you positive\n";
        auto pos = (e.position.getX() - plotFrame.getX()) / plotFrame.getWidth();
        
        if(auto* voice = dynamic_cast<OrionSamplerVoice*> (processor.getSampler()->getVoice(EQserial)))
        {
            voice->eq.bands[draggingBand].frequency = getFrequencyForPosition (pos);
            if (draggingGain)
                if(getGainForPosition (e.position.getY(), plotFrame.getY(), plotFrame.getBottom()) > -24 and getGainForPosition (e.position.getY(), plotFrame.getY(), plotFrame.getBottom()) < 24)
                   {
                       voice->eq.bands[draggingBand].gain = getGainForPosition (e.position.getY(), plotFrame.getY(), plotFrame.getBottom());
                   }
            voice->eq.updateBand(draggingBand);
            voice->updatePlots();
        }
    }
    repaint (plotFrame);
}


void OrionEQConfiguration::mouseWheelMove (const MouseEvent& e, const MouseWheelDetails & wheel)
{
    if (plotFrame.contains (e.x, e.y))
    {
        for (int i=0; i < 5; ++i)
        {
            if(auto* voice = dynamic_cast<OrionSamplerVoice*> (processor.getSampler()->getVoice(EQserial)))
            {
                
                auto pos = plotFrame.getX() + getPositionForFrequency (float (voice->eq.bands[i].frequency)) * plotFrame.getWidth();
                
                if (std::abs (pos - e.position.getX()) < clickRadius)
                {
                    wheelQ = true;
                    wheelBand = i;
                }
                
                if (wheelQ)
                {
                    //getQforPosition Q range: 0.025 to 40
                    if(wheel.deltaY > 0)
                    {
                        if(voice->eq.bands[draggingBand].quality <= 20)
                        {
                            if(voice->eq.bands[wheelBand].quality + wheel.deltaY < 20)
                            {
                                voice->eq.bands[wheelBand].quality += wheel.deltaY;
                            }
                            else
                            {
                                voice->eq.bands[wheelBand].quality = 20;
                            }
                            //voice->eq.bands[wheelBand].quality += wheel.deltaY;
                            voice->eq.updateBand(wheelBand);
                            voice->updatePlots();
                        }
                        
                    }
                    else if(wheel.deltaY <=0)
                    {
                        if(voice->eq.bands[draggingBand].quality >= 0.025)
                        {
                            if(voice->eq.bands[wheelBand].quality + wheel.deltaY > 0)
                            {
                                voice->eq.bands[wheelBand].quality += wheel.deltaY;
                            }
                            else
                            {
                                voice->eq.bands[wheelBand].quality = 0.025;
                            }
                            
                            voice->eq.updateBand(wheelBand);
                            voice->updatePlots();
                        }
                        
                    }
                    
                }
                    
                
            }
        }
    }
    wheelQ = false;
    repaint (plotFrame);
}



void OrionEQConfiguration::mouseDoubleClick (const MouseEvent& e)
{
    if (plotFrame.contains (e.x, e.y))
    {
        for (int i=0; i < 5; ++i)
        {
            if(auto* voice = dynamic_cast<OrionSamplerVoice*> (processor.getSampler()->getVoice(EQserial)))
            {
                
                if (std::abs (plotFrame.getX() + getPositionForFrequency (float (voice->eq.bands[i].frequency)) * plotFrame.getWidth()
                              - e.position.getX()) < clickRadius)
                {
                    for(int j = 0; j < 5; ++j)
                    {
                        if(i != j)
                        {
                            voice->eq.bands[j].active = false;
                            voice->setactivity(j,0);
                        }
                    }
                    voice->eq.updateBand(i);
                }
                voice->updatePlots();
            
            }
        }
        
    }
    repaint(plotFrame);
}

void OrionEQConfiguration::updateFrequencyResponses()
{
    auto pixelsPerDouble = 2.0f * plotFrame.getHeight() / Decibels::decibelsToGain (maxDB);
    
    for (int i=0; i < 5; ++i)
    {
        
        if(auto* voice = dynamic_cast<OrionSamplerVoice*> (processor.getSampler()->getVoice(EQserial)))
        {
            voice->eq.bands[i].frequencyResponse.clear();
            
            voice->createFrequencyPlot (voice->eq.bands[i].frequencyResponse, voice->eq.bands[i].magnitudes, plotFrame.withX (plotFrame.getX() + 1), pixelsPerDouble);
            
        }
        
    }
    
    frequencyResponse.clear();
    if(auto* voice = dynamic_cast<OrionSamplerVoice*> (processor.getSampler()->getVoice(EQserial)))
    {
        voice->createFrequencyPlot (frequencyResponse, voice->getMagnitudes(), plotFrame, pixelsPerDouble);
        //std::cout<<"overall magnitudes "<<voice->getMagnitudes()[30]<<"\n";
    }
}



