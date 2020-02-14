/*
  ==============================================================================

    OrionLookAndFeel.cpp
    Created: 29 Oct 2019 12:52:09pm
    Author:  Quin Scacheri

  ==============================================================================
*/

#include "OrionLookAndFeel.h"
#include "AutoLabel.h"

OrionLookAndFeel::OrionLookAndFeel()
{
    
    Image on = ImageCache::getFromMemory(BinaryData::env_on_png, BinaryData::env_on_pngSize);
    Image off = ImageCache::getFromMemory(BinaryData::env_off_png, BinaryData::env_off_pngSize);
    tabButtonImages[TabButtonType::envelope].reset(new ImagePair(on, off));

    on = ImageCache::getFromMemory(BinaryData::fx_on_png, BinaryData::fx_on_pngSize);
    off = ImageCache::getFromMemory(BinaryData::fx_off_png, BinaryData::fx_off_pngSize);
    tabButtonImages[TabButtonType::fx].reset(new ImagePair(on, off));

    on = ImageCache::getFromMemory(BinaryData::eq_on_png, BinaryData::eq_on_pngSize);
    off = ImageCache::getFromMemory(BinaryData::eq_off_png, BinaryData::env_off_pngSize);
    tabButtonImages[TabButtonType::eq].reset(new ImagePair(on, off));
    

    on = ImageCache::getFromMemory(BinaryData::clip_on_png, BinaryData::clip_on_pngSize);
    off = ImageCache::getFromMemory(BinaryData::clip_off_png, BinaryData::clip_off_pngSize);
    tabButtonImages[TabButtonType::clip].reset(new ImagePair(on, off));

}

void OrionLookAndFeel::drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
    auto radius = jmin (width / 2, height / 2) - 4.0f;
    auto centreX = x + width  * 0.5f;
    auto centreY = y + height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    
    // fill
    g.setColour (Colours::darkgrey);
    g.fillEllipse (rx, ry, rw, rw);
    // outline
    g.setColour (Colours::black);
    g.drawEllipse (rx, ry, rw, rw, 1.0f);

    Path p;
    auto pointerLength = radius;
    auto pointerThickness = 2.0f;
    p.addRectangle (-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform (AffineTransform::rotation (angle).translated (centreX, centreY));

    // pointer
    g.setColour (Colours::black);
    g.fillPath (p);
}

void OrionLookAndFeel::drawTabButton (TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown)
{
    NamedValueSet properties = button.getProperties();
    if (properties.contains("tabButtonType"))
    {
        int type = properties["tabButtonType"];
        if (button.isDown() || button.getToggleState())
              g.drawImage(tabButtonImages[type]->onImage, button.getLocalBounds().toFloat(), juce::RectanglePlacement::centred);
        else
            g.drawImage(tabButtonImages[type]->offImage, button.getLocalBounds().toFloat(), juce::RectanglePlacement::centred);
    }
    else
        jassertfalse;

};

Font OrionLookAndFeel::getLabelFont (Label &l)
{
    if (l.getProperties().contains(AutoLabel::propertyName))
        return Font();
    
    Font f;
    f.setHeight(l.getHeight() * .75);
    return f;
}

void OrionLookAndFeel::drawComboBox(juce::Graphics &g, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox &combo) {
    
    g.setColour(Colours::black);
    g.fillRoundedRectangle(combo.getLocalBounds().toFloat(), 2.f);
   
    // DRAW ARROW
    Path arrow;
    Rectangle<float> arrowBounds(combo.getRight() - combo.getHeight() * 1.25, 0, combo.getHeight() / 2, combo.getHeight() / 4);
    arrowBounds.translate(0, arrowBounds.getHeight() * 1.5);
    g.setColour(Colours::white);
    arrow.startNewSubPath(arrowBounds.getX(), arrowBounds.getY());
    arrow.lineTo(arrowBounds.getCentreX(), arrowBounds.getBottom());
    arrow.lineTo(arrowBounds.getRight(), arrowBounds.getY());
    g.strokePath(arrow, PathStrokeType(1.f));
}
