/*
  ==============================================================================

    ColorEditorCustomComponent.h
    Created: 4 Nov 2019 1:42:24pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "TableColumnCallback.h"
#include "ColorSelectionComponent.h"

class ColorEditorCustomComponent : public Label, public ChangeListener, public Button::Listener
{
public:
	ColorEditorCustomComponent(TableColumnCallback& td, bool groupFlag = false) : owner(td), row(0), columnId(0), groupFlag(groupFlag)
	{
	}

	void mouseDown(const MouseEvent& event) override
	{
        if(isEnabled())
        {
            owner.getTable()->selectRowsBasedOnModifierKeys(row, event.mods, false);
            CallOutBox::launchAsynchronously(new ColorSelectionComponent(color, this, this, groupFlag), getScreenBounds(), nullptr);
        }
	}

	void setRowAndColumn(const int newRow, const int newColumn)
	{
		row = newRow;
		columnId = newColumn;
		color = Colour(uint32(owner.getValue(columnId, row)));
        bool enabled = owner.getEnabled(columnId, row);
        setEnabled(enabled);
        setColour(textColourId, enabled ? Colours::white : Colours::darkgrey);
	}

	void paint(Graphics& g) override
	{
        if(isEnabled())
            g.fillAll(color);
        else
            g.fillAll(color.withAlpha(0.3f));
	}

	void changeListenerCallback(ChangeBroadcaster* source) override
	{
		ColourSelector* selector = dynamic_cast<ColourSelector*> (source);
		if (selector != nullptr)
		{
			uint32 c = uint32(selector->getCurrentColour().getARGB());
			owner.setValue(columnId, row, c);
		}
	}

	void buttonClicked(Button *) override
    {
        owner.setValue(columnId, row, -1);
    }
    
private:
	TableColumnCallback& owner;
	int row, columnId;
	Colour color;
    bool groupFlag;
};
