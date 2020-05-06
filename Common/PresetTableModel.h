/*
  ==============================================================================

    PresetTableModel.h
    Created: 21 Jan 2020 5:45:57pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "NumericColumnCustomComponent.h"
#include "EditableTextCustomComponent.h"
#include "ColorEditorCustomComponent.h"
#include "SliderColumnCustomComponent.h"
#include "PresetHelper.h"
#include "PresetComparators.h"

#define COLUMN_ID_PRESET_NAME 1
#define COLUMN_ID_PRESET_DATE 2
#define COLUMN_ID_PRESET_APPLY 3

class PresetTableModel : public TableListBoxModel, Button::Listener
{
public:
    PresetTableModel(PresetHelper* pPresetHelper, Component* pParentComponent) : pPresetHelper(pPresetHelper), pParentComponent(pParentComponent)
    {
    }

    ~PresetTableModel() override {}

    void initTable(TableListBox* tableListBox)
    {
        tableListBox->setModel(this);
        tableListBox->getHeader().addColumn("Name", COLUMN_ID_PRESET_NAME, 100);
        tableListBox->getHeader().addColumn("Creation Date", COLUMN_ID_PRESET_DATE, 100);
        tableListBox->getHeader().addColumn("", COLUMN_ID_PRESET_APPLY, 40);
        tableListBox->getHeader().setStretchToFitActive(true);
        tableListBox->getHeader().resizeAllColumnsToFit(tableListBox->getWidth());
    }

private:
    int getNumRows() override
    {
        return pPresetHelper->presetFiles.size();
    }

    void paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override
    {
        const Colour alternateColour(pParentComponent->getLookAndFeel().findColour(ListBox::backgroundColourId)
            .interpolatedWith(pParentComponent->getLookAndFeel().findColour(ListBox::textColourId), 0.03f));
        if (rowIsSelected)
            g.fillAll(Colours::lightblue);
        else if (rowNumber % 2)
            g.fillAll(alternateColour);
    }

    void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool /*rowIsSelected*/) override
    {
        File file = pPresetHelper->presetFiles.getReference(rowNumber);
        
        g.setColour(pParentComponent->getLookAndFeel().findColour(ListBox::textColourId));
        String text;
        switch (columnId)
        {
        case COLUMN_ID_PRESET_NAME: text = file.getFileNameWithoutExtension(); break;
        case COLUMN_ID_PRESET_DATE: text = file.getCreationTime().toString(true, true); break;
        default: text = "";
        }
        g.drawText(text, 2, 0, width - 4, height, Justification::centredLeft, true);
        g.setColour(pParentComponent->getLookAndFeel().findColour(ListBox::backgroundColourId));
        g.fillRect(width - 1, 0, 1, height);
    }

    void selectedRowsChanged(int /*lastRowSelected*/) override
    {
        pParentComponent->repaint();
    }

    void buttonClicked(Button* button) override
    {
        pPresetHelper->selectPresetName(button->getComponentID());
    }
    
    Component* refreshComponentForCell(int rowNumber, int columnId, bool, Component* existingComponentToUpdate) override
    {
        /*if (columnId == COLUMN_ID_PRESET_NAME)
        {
            EditableTextCustomComponent* textLabel = static_cast<EditableTextCustomComponent*> (existingComponentToUpdate);
            if (textLabel == nullptr)
                textLabel = new EditableTextCustomComponent(*this);

            textLabel->setRowAndColumn(rowNumber, columnId);
            return textLabel;
        }
        */
        if(columnId == COLUMN_ID_PRESET_APPLY)
        {
            TextButton* button = static_cast<TextButton*>(existingComponentToUpdate);
            if(button == nullptr)
            {
                button = new TextButton("Apply");
                button->addListener(this);
            }
            
            button->setComponentID(pPresetHelper->presetFiles[rowNumber].getFileNameWithoutExtension());
            return button;
        }
        return nullptr;
    }

    void sortOrderChanged(int newSortColumnId, bool isForwards) override
    {
        if(newSortColumnId == COLUMN_ID_PRESET_NAME)
        {
            if(isForwards)
            {
                PresetNameComparatorAscending comparator;
                pPresetHelper->presetFiles.sort(comparator);
            }
            else
            {
                PresetNameComparatorDescending comparator;
                pPresetHelper->presetFiles.sort(comparator);
            }
        }
        else if(newSortColumnId == COLUMN_ID_PRESET_DATE)
        {
            if(isForwards)
            {
                PresetDateComparatorAscending comparator;
                pPresetHelper->presetFiles.sort(comparator);
            }
            else
            {
                PresetDateComparatorDescending comparator;
                pPresetHelper->presetFiles.sort(comparator);
            }
        }
        
        pPresetHelper->notifyPresetListChanged();
    }
    
    /*
    String getTableText(const int columnId, const int rowNumber) override
    {
        AmbiPoint* pt = pSources->get(rowNumber);
        if (pt == nullptr)
            return "";

        switch (columnId)
        {
        case COLUMN_ID_NAME: return pt->getName();
        default: return "";
        }
    }
*/
    
private:
    PresetHelper* pPresetHelper;
    Component* pParentComponent;
};
