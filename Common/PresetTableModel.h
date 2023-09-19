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
#include "JuceHeader.h"
#include "NumericColumnCustomComponent.h"
#include "EditableTextCustomComponent.h"
#include "ColorEditorCustomComponent.h"
#include "SliderColumnCustomComponent.h"
#include "PresetHelper.h"
#include "PresetComparators.h"
#include "ColorDefinition.h"

#define COLUMN_ID_PRESET_NAME 1
#define COLUMN_ID_PRESET_DATE 2
#define COLUMN_ID_PRESET_APPLY 3

class PresetTableModel : public TableListBoxModel, Button::Listener
{
public:
    PresetTableModel(PresetHelper* _pPresetHelper, Component* _pParentComponent) : pPresetHelper(_pPresetHelper), pParentComponent(_pParentComponent)
    {
    }

    ~PresetTableModel() override {}

    void initTable(TableListBox* tableListBox, bool allowApply)
    {
        tableListBox->setModel(this);
        tableListBox->getHeader().addColumn("Name", COLUMN_ID_PRESET_NAME, 100);
        tableListBox->getHeader().addColumn("Creation Date", COLUMN_ID_PRESET_DATE, 100);
        if(allowApply)
        {
            tableListBox->getHeader().addColumn("", COLUMN_ID_PRESET_APPLY, 40);
        }
        
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
            .interpolatedWith(pParentComponent->getLookAndFeel().findColour(ListBox::textColourId), COLOR_DEFINITION_ALTERNATE_INTENSITY));
        if (rowIsSelected)
            g.fillAll(COLOR_DEFINITION_SELECTED_ROW);
        else if (rowNumber % 2)
            g.fillAll(alternateColour);
    }

    void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool /*rowIsSelected*/) override
    {
        if(rowNumber >= pPresetHelper->presetFiles.size())
            return;
        
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
