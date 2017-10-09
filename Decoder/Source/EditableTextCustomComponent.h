/*
  ==============================================================================

    EditableTextCustomComponent.h
    Created: 9 Oct 2017 2:04:27pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "SpeakerSettingsComponent.h"

class EditableTextCustomComponent : public Label
{
public:
	EditableTextCustomComponent(SpeakerSettingsComponent& td) : owner(td)
	{
		setEditable(false, true, false);
	}

	void mouseDown(const MouseEvent& event) override
	{
		owner.getTable()->selectRowsBasedOnModifierKeys(row, event.mods, false);
		Label::mouseDown(event);
	}

	void textWasEdited() override
	{
		owner.setTableText(columnId, row, getText());
	}

	void setRowAndColumn(const int newRow, const int newColumn)
	{
		row = newRow;
		columnId = newColumn;
		setText(owner.getTableText(columnId, row), dontSendNotification);
	}

	void paint(Graphics& g) override
	{
		auto& lf = getLookAndFeel();
		if (!dynamic_cast<LookAndFeel_V4*> (&lf))
			lf.setColour(textColourId, Colours::black);

		Label::paint(g);
	}

private:
	SpeakerSettingsComponent& owner;
	int row, columnId;
	Colour textColour;
};
