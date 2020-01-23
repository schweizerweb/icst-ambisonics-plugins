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
#define COLUMN_ID_PRESET_NAME 1

class PresetTableModel : public TableListBoxModel
{
public:
    PresetTableModel(Array<File>* pPresetFiles, Component* pParentComponent) : pPresetFiles(pPresetFiles), pParentComponent(pParentComponent)
    {
    }

    ~PresetTableModel() override {}

    void initTable(TableListBox* tableListBox)
    {
        tableListBox->setModel(this);
        tableListBox->getHeader().addColumn("Name", COLUMN_ID_PRESET_NAME, 100);
        tableListBox->getHeader().setStretchToFitActive(true);
        tableListBox->getHeader().resizeAllColumnsToFit(tableListBox->getWidth());
    }

private:
    int getNumRows() override
    {
        return pPresetFiles->size();
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
        File file = pPresetFiles->getReference(rowNumber);
        
        g.setColour(pParentComponent->getLookAndFeel().findColour(ListBox::textColourId));
        String text;
        switch (columnId)
        {
        case COLUMN_ID_PRESET_NAME: text = file.getFileNameWithoutExtension(); break;
        default: text = "";
        }
        g.drawText(text, 2, 0, width - 4, height, Justification::centredLeft, true);
        g.setColour(pParentComponent->getLookAndFeel().findColour(ListBox::backgroundColourId));
        g.fillRect(width - 1, 0, 1, height);
    }

    void selectedRowsChanged(int lastRowSelected) override
    {
        pParentComponent->repaint();
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
        return nullptr;
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
    Array<File>* pPresetFiles;
    Component* pParentComponent;
};
