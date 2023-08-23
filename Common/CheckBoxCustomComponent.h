/*
  ==============================================================================

    CheckBoxCustomComponent.h
    Created: 10 Jul 2020 9:36:22pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "TableColumnCallback.h"

class CheckBoxCustomComponent : public ToggleButton, public ToggleButton::Listener
{
public:
    CheckBoxCustomComponent(TableColumnCallback& td) : owner(td)
    {
        addListener(this);
    }

    void mouseDown(const MouseEvent& event) override
    {
        owner.getTable()->selectRowsBasedOnModifierKeys(row, event.mods, false);
        ToggleButton::mouseDown(event);
    }

    void buttonClicked(Button*) override
    {
        owner.setValue(columnId, row, getToggleState()?1:0);
    }

    void setRowAndColumn(const int newRow, const int newColumn)
    {
        row = newRow;
        columnId = newColumn;
        setToggleState(owner.getValue(columnId, row) == 1.0, dontSendNotification);
        bool enabled = owner.getEnabled(columnId, row);
        setEnabled(enabled);
    }

    void paint(Graphics& g) override
    {
        auto& lf = getLookAndFeel();
        if (!dynamic_cast<LookAndFeel_V4*> (&lf))
            lf.setColour(textColourId, Colours::black);

        ToggleButton::paint(g);
    }

private:
    TableColumnCallback& owner;
    int row, columnId;
    Colour textColour;
};

