/*
  ==============================================================================

    ColorEditorCustomComponent.h
    Created: 4 Nov 2019 1:42:24pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "TableColumnCallback.h"

class ColorEditorCustomComponent : public Label, public ChangeListener
{
public:
	ColorEditorCustomComponent(TableColumnCallback& td) : owner(td), row(0), columnId(0)
	{
	}

	void mouseDown(const MouseEvent& event) override
	{
		owner.getTable()->selectRowsBasedOnModifierKeys(row, event.mods, false);
		ColourSelector* selector = new ColourSelector(ColourSelector::showColourspace);
		selector->setName("Colour");
		selector->setCurrentColour(color);
		selector->addChangeListener(this);
		selector->setColour(ColourSelector::backgroundColourId, Colours::transparentBlack);
		selector->setSize(200, 200);
		CallOutBox::launchAsynchronously(selector, getScreenBounds(), nullptr);
	}

	void setRowAndColumn(const int newRow, const int newColumn)
	{
		row = newRow;
		columnId = newColumn;
		color = Colour(uint32(owner.getValue(columnId, row)));
	}

	void paint(Graphics& g) override
	{
		g.fillAll(color);
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

	
private:
	TableColumnCallback& owner;
	int row, columnId;
	Colour color;
};
