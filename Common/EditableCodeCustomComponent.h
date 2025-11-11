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
#include "TableColumnCallback.h"
#include "JsEditorComponent.h"

class EditableCodeCustomComponent : public Label, public AsyncUpdater
{
public:
	EditableCodeCustomComponent(TableColumnCallback& td) : owner(td)
	{
        standardTextColour = findColour(Label::textColourId);
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
        String text = owner.getTableText(columnId, row);
        
        // if code starts with a comment, show the comment only
        if(text.startsWith("//"))
        {
            text = text.fromFirstOccurrenceOf("//", false, true).upToFirstOccurrenceOf("\n", false, true);

            textColour = Colours::lightgreen;
        }
        // otherwise, if more than one code line, just show the number of code lines
        else if(text.contains(NewLine::getDefault()))
        {
            int count = 0;
            int index = 0;
            while((index = text.indexOf(index+1, NewLine::getDefault())) != -1)
                count++;
            text = String(count + 1) + " lines of code";
            textColour = Colours::lightblue;
        }
        else
        {
            textColour = standardTextColour;
        }
        
		setText(text, dontSendNotification);
	}

private:
	void paint(Graphics& g) override
	{
        setColour(Label::textColourId, textColour);
		Label::paint(g);
    }
	
	void mouseUp(const MouseEvent& /*event*/) override
    {
        localCode = owner.getTableText(columnId, row);
        auto topComponent = owner.getTable()->getParentComponent();
        auto relPos = topComponent->getScreenPosition();
        CallOutBox::launchAsynchronously(std::make_unique<JsEditorComponent>(&localCode, &localCloseFlag, this), getScreenBounds().translated(-relPos.getX(), -relPos.getY()), topComponent);
    }
    
    void handleAsyncUpdate() override {
        if(owner.getTableText(columnId, row) != localCode)
        {
            bool doSave = true;
            if(localCloseFlag)
            {
                MessageBoxOptions o;
                auto ret = AlertWindow::show(
                              o.withTitle("Save changes?")
                              .withMessage("The code has changed, save?")
                              .withButton("Save")
                              .withButton("Discard"));
                if(ret == 0)
                    doSave = false;
            }
            if(doSave)
                owner.setTableText(columnId, row, localCode);
        }
    }

private:
	TableColumnCallback& owner;
	int row, columnId;
	Colour textColour;
    Colour standardTextColour;
    String localCode;
    bool localCloseFlag;
};
