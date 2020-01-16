/*
  ==============================================================================

    ColorEditorCustomComponent.h
    Created: 4 Nov 2019 1:42:24pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "TableColumnCallback.h"

class ColorEditorCustomComponent : public Label, public ChangeListener, public Button::Listener
{
public:
	ColorEditorCustomComponent(TableColumnCallback& td, bool groupFlag = false) : owner(td), row(0), columnId(0), groupFlag(groupFlag)
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
        
        Component* c = new Component();
        c->setSize(200, groupFlag ? 230 : 200);
        c->addAndMakeVisible(selector);
        
        if(groupFlag)
        {
            TextButton* btn = new TextButton("Apply color to sub-points");
            btn->addListener(this);
            btn->setSize(200, 25);
            btn->setTopLeftPosition(0, 202);
            c->addAndMakeVisible(btn);
        }
        
        CallOutBox::launchAsynchronously(c, getScreenBounds(), nullptr);
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
