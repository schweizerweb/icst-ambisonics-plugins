/*
================================================================================
    This file is part of the ICST AmbiPlugins.

    ICST AmbiPlugins are free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICST AmbiPlugins are distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ICSTAmbiPlugins.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
*/



#pragma once
#include "SoloMuteCallback.h"
#include "JuceHeader.h"

class SoloMuteCustomComponent : public Component, private ToggleButton::Listener
{
public:
    SoloMuteCustomComponent(SoloMuteCallback& td) : owner(td), row(0)
    {
        btnMute.setButtonText("M");
        addAndMakeVisible(btnMute);
        btnMute.addListener(this);
        btnMute.setToggleable(true);
        btnMute.setClickingTogglesState(true);
        btnMute.setColour(TextButton::ColourIds::buttonOnColourId, Colours::red);
        btnMute.setWantsKeyboardFocus(false);

        btnSolo.setButtonText("S");
        addAndMakeVisible(btnSolo);
        btnSolo.addListener(this);
        btnSolo.setToggleable(true);
        btnSolo.setClickingTogglesState(true);
        btnSolo.setColour(TextButton::ColourIds::buttonOnColourId, Colours::yellow);
        btnSolo.setWantsKeyboardFocus(false);

        setWantsKeyboardFocus(false);
    }

    ~SoloMuteCustomComponent() override
    {
    }

    void buttonClicked(Button* btn) override
    {
        if (btn == &btnMute)
        {
            owner.setMute(row, btnMute.getToggleState() ? 1 : 0);
        }
        else if (btn == &btnSolo)
        {
            owner.setSolo(row, btnSolo.getToggleState() ? 1 : 0);
        }
    }

    void setRowAndColumn(const int newRow, const int newColumn)
    {
        row = newRow;
        columnId = newColumn;
        btnMute.setToggleState(owner.getMute(row), dontSendNotification);
        btnSolo.setToggleState(owner.getSolo(row), dontSendNotification);
        bool enabled = owner.getEnabled(columnId, row);
        setEnabled(enabled);
    }

private:
    void resized() override
    {
        btnMute.setBounds(0, 0, getHeight(), getHeight());
        btnSolo.setBounds(getHeight(), 0, getHeight(), getHeight());
    }


private:
    TextButton btnMute;
    TextButton btnSolo;
    SoloMuteCallback& owner;
    int row, columnId;
    Colour textColour;
};

