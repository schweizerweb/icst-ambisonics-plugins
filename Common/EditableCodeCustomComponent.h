/*
  ==============================================================================

    EditableCodeCustomComponent.h
    Created: 5 Jul 2022 7:34:00pm
    Author:  Schweizer Christian

  ==============================================================================
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

	void setRowAndColumn(const int newRow, const int newColumn, const bool readOnly = false)
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
	
	void mouseUp(const MouseEvent &event) override
    {
        localCode = owner.getTableText(columnId, row);
        CallOutBox::launchAsynchronously(std::make_unique<JsEditorComponent>(&localCode, &localCloseFlag, this), getScreenBounds(), nullptr);
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
