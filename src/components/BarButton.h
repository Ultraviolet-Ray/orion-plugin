/*
  ==============================================================================

    BarButton.h
    Created: 23 Aug 2020 12:59:03pm
    Author:  Quin Scacheri

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class BarButton : public Button
{
public:
    BarButton(int num) : Button(String("")), barNum(num)
    {
    }

    ~BarButton()
    {
    }

    juce::String getTooltip() override
    {
        return "Command click to toggle active, click to edit sequence";
    }

    virtual void paintButton(Graphics &g,
                             bool shouldDrawButtonAsHighlighted,
                             bool shouldDrawButtonAsDown) override
    {
        Colour textColour;
        if (selected)
        {
            g.setColour(Colour(0xFF36FEE2));
            textColour = Colours::black;
        }

        else if (active)
        {
            g.setColour(Colour(0xFF36FEE2).darker(.5));
            textColour = Colours::black;
        }

        else
        {
            g.setColour(Colour(0xFF313131));
            textColour = Colours::white;
        }

        g.fillRoundedRectangle(getLocalBounds().toFloat(), 8.f);
        g.setColour(textColour);
        g.drawFittedText(String(barNum + 1), getLocalBounds(), Justification::centred, 1);
    }

    virtual void clicked(const ModifierKeys &modifiers) override
    {
        Button::clicked(modifiers);
        if (modifiers.isCommandDown())
        {
            active = !active;
            if (onActiveChange != nullptr)
            {
                onActiveChange(barNum);
            }
        }
        else
        {
            if (onSelection != nullptr)
                onSelection(barNum);
        }
    }

    int getBarNum() { return barNum; };

    bool getActive() { return active; };
    void setActive(bool a)
    {
        active = a;
        repaint();
    };

    bool getSelected() { return selected; };
    void setSelected(bool s)
    {
        selected = s;
        repaint();
    };

    std::function<void(int)> onSelection;
    std::function<void(int)> onActiveChange;

private:
    bool active = false;
    bool selected = false;
    int barNum;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BarButton)
};
