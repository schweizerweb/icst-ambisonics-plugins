/*
  ==============================================================================

    SourceTableListModel.h
    Created: 19 Jan 2020 8:28:59pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/AmbiSourceSet.h"
#include "../../Common/PointSelection.h"
#include "../../Common/NumericColumnCustomComponent.h"
#include "../../Common/EditableTextCustomComponent.h"
#include "../../Common/ColorEditorCustomComponent.h"
#include "../../Common/SliderColumnCustomComponent.h"
#include "../../Common/CheckBoxCustomComponent.h"
#include "../../Common/ScalingInfo.h"
#include "../../Common/ColorDefinition.h"
#include "../../Common/ChannelLayout.h"

#define COLUMN_ID_NB        2
#define COLUMN_ID_NAME        3
#define COLUMN_ID_X            7
#define COLUMN_ID_Y            8
#define COLUMN_ID_Z            9
#define COLUMN_ID_A            10
#define COLUMN_ID_E            11
#define COLUMN_ID_D            12
#define COLUMN_ID_GAIN        5
#define COLUMN_ID_MUTE          14
#define COLUMN_ID_SOLO          15
#define COLUMN_ID_COLOR        13
#define COLUMN_ID_ENABLED       1


class SourceTableListModel : public TableListBoxModel, public TableColumnCallback, public ChangeListener
{
public:
    SourceTableListModel(AmbiSourceSet* _pSources, PointSelection* _pPointSelection, Component* _pParentComponent, ScalingInfo* _pScaling, ChannelLayout* _pChannelLayout) : pSources(_pSources), pPointSelection(_pPointSelection), pParentComponent(_pParentComponent), pTableListBox(nullptr), pScalingInfo(_pScaling), pChannelLayout(_pChannelLayout)
    {
    }

    ~SourceTableListModel() override {}

    void initTable(TableListBox* tableListBox)
    {
        pTableListBox = tableListBox;
        tableListBox->setModel(this);
        tableListBox->getHeader().addColumn("En", COLUMN_ID_ENABLED, 30);
        tableListBox->getHeader().addColumn("CH", COLUMN_ID_NB, 30);
        tableListBox->getHeader().addColumn("Name", COLUMN_ID_NAME, 100);
        tableListBox->getHeader().addColumn("X", COLUMN_ID_X, 50);
        tableListBox->getHeader().addColumn("Y", COLUMN_ID_Y, 50);
        tableListBox->getHeader().addColumn("Z", COLUMN_ID_Z, 50);
        tableListBox->getHeader().addColumn("A", COLUMN_ID_A, 50);
        tableListBox->getHeader().addColumn("E", COLUMN_ID_E, 50);
        tableListBox->getHeader().addColumn("D", COLUMN_ID_D, 50);
        tableListBox->getHeader().addColumn("Mute", COLUMN_ID_MUTE, 40);
        tableListBox->getHeader().addColumn("Solo", COLUMN_ID_SOLO, 40);
        tableListBox->getHeader().addColumn("Gain [dB]", COLUMN_ID_GAIN, 80);
        tableListBox->getHeader().addColumn("Color", COLUMN_ID_COLOR, 60);
        tableListBox->getHeader().setStretchToFitActive(true);
        tableListBox->getHeader().resizeAllColumnsToFit(tableListBox->getWidth());
        tableListBox->setMultipleSelectionEnabled(true);
    }

private:
    int getNumRows() override
    {
        return jmin(pSources->size(), pChannelLayout->getNumInputChannels());
    }

    void paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override
    {
        if (rowIsSelected)
        {
            Colour baseColor = COLOR_DEFINITION_SELECTED_ROW;
            if (pPointSelection->getMainSelectedPointIndex() == rowNumber)
                g.fillAll(baseColor);
            else
                g.fillAll(baseColor.withAlpha(0.4f));
        }
        else if (rowNumber % 2)
        {
            const Colour alternateColour(pParentComponent->getLookAndFeel().findColour(ListBox::backgroundColourId)
                .interpolatedWith(pParentComponent->getLookAndFeel().findColour(ListBox::textColourId), COLOR_DEFINITION_ALTERNATE_INTENSITY));

            g.fillAll(alternateColour);
        }
    }

    void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool /*rowIsSelected*/) override
    {
        AmbiPoint* pt = pSources->get(rowNumber);
        if (pt == nullptr)
            return;

        g.setColour(pParentComponent->getLookAndFeel().findColour(ListBox::textColourId));
        String text;
        switch (columnId)
        {
        case COLUMN_ID_NB: text = String(rowNumber + 1); break;
        case COLUMN_ID_NAME: text = pt->getName(); break;
        default: text = "";
        }
        g.drawText(text, 2, 0, width - 4, height, Justification::centredLeft, true);
        g.setColour(pParentComponent->getLookAndFeel().findColour(ListBox::backgroundColourId));
        g.fillRect(width - 1, 0, 1, height);
    }

    void selectedRowsChanged(int lastRowSelected) override
    {
        pPointSelection->unselectPoint();

        bool first = true;
        if (lastRowSelected >= 0 && lastRowSelected < pSources->size())
        {
            auto set = pTableListBox->getSelectedRows();
            for (auto r : set.getRanges())
            {
                for (int i = r.getStart(); i < r.getEnd(); i++)
                {
                    if (i != lastRowSelected)
                    {
                        pPointSelection->selectPoint(i, !first);
                        first = false;
                    }
                }
            }

            pPointSelection->selectPoint(lastRowSelected, !first);
        }
    }

    Component* refreshComponentForCell(int rowNumber, int columnId, bool, Component* existingComponentToUpdate) override
    {
        if (columnId == COLUMN_ID_X
            || columnId == COLUMN_ID_Y
            || columnId == COLUMN_ID_Z
            || columnId == COLUMN_ID_A
            || columnId == COLUMN_ID_E
            || columnId == COLUMN_ID_D)
        {
            NumericColumnCustomComponent* numericBox = static_cast<NumericColumnCustomComponent*> (existingComponentToUpdate);
            if (numericBox == nullptr)
                numericBox = new NumericColumnCustomComponent(*this);

            numericBox->setRowAndColumn(rowNumber, columnId);
            numericBox->setJustificationType(Justification::right);
            return numericBox;
        }
        else if (columnId == COLUMN_ID_GAIN)
        {
            SliderColumnCustomComponent* gainBox = static_cast<SliderColumnCustomComponent*> (existingComponentToUpdate);
            if (gainBox == nullptr)
                gainBox = new SliderColumnCustomComponent(*this);

            gainBox->setRowAndColumn(rowNumber, columnId);
            return gainBox;
        }
        else if (columnId == COLUMN_ID_NAME)
        {
            EditableTextCustomComponent* textLabel = static_cast<EditableTextCustomComponent*> (existingComponentToUpdate);
            if (textLabel == nullptr)
                textLabel = new EditableTextCustomComponent(*this);

            textLabel->setRowAndColumn(rowNumber, columnId);
            return textLabel;
        }
        else if (columnId == COLUMN_ID_COLOR)
        {
            ColorEditorCustomComponent* colorBox = static_cast<ColorEditorCustomComponent*> (existingComponentToUpdate);
            if (colorBox == nullptr)
                colorBox = new ColorEditorCustomComponent(*this);

            colorBox->setRowAndColumn(rowNumber, columnId);
            return colorBox;
        }
        else if (columnId == COLUMN_ID_ENABLED
                 || columnId == COLUMN_ID_MUTE
                 || columnId == COLUMN_ID_SOLO)
        {
            CheckBoxCustomComponent* checkBox = static_cast<CheckBoxCustomComponent*>(existingComponentToUpdate);
            if(checkBox == nullptr)
                checkBox = new CheckBoxCustomComponent(*this);
            
            checkBox->setRowAndColumn(rowNumber, columnId);
            return checkBox;
        }
        
        return nullptr;
    }

    double getValue(int columnId, int rowNumber) override
    {
        AmbiPoint* pt = pSources->get(rowNumber);
        if (pt == nullptr)
            return 0.0;

        switch (columnId)
        {
        case COLUMN_ID_GAIN: return Decibels::gainToDecibels(pt->getGain());
        case COLUMN_ID_MUTE: return pt->getMute();
        case COLUMN_ID_SOLO: return pt->getSolo();
        case COLUMN_ID_X: return pt->getRawPoint()->getX();
        case COLUMN_ID_Y: return pt->getRawPoint()->getY();
        case COLUMN_ID_Z: return pt->getRawPoint()->getZ();
        case COLUMN_ID_A: return Constants::RadToGrad(pt->getRawPoint()->getAzimuth());
        case COLUMN_ID_E: return Constants::RadToGrad(pt->getRawPoint()->getElevation());
        case COLUMN_ID_D: return pt->getRawPoint()->getDistance();
        case COLUMN_ID_COLOR: return pt->getColor().getARGB();
        case COLUMN_ID_ENABLED: return pt->getEnabled() ? 1 : 0;
        default: return 0.0;
        }
    }

    bool getEnabled(int columnId, int rowNumber) override
    {
        if(columnId == COLUMN_ID_ENABLED)
            return true;
        
        AmbiSource* pt = pSources->get(rowNumber);
        return (pt != nullptr && pt->getEnabled());
    }

    void setValue(int columnId, int rowNumber, double newValue) override
    {
        switch (columnId)
        {
        case COLUMN_ID_GAIN: pSources->setGain(rowNumber, Decibels::decibelsToGain(newValue)); break;
        case COLUMN_ID_MUTE: pSources->setMute(rowNumber, newValue == 1.0); break;
        case COLUMN_ID_SOLO: pSources->setSolo(rowNumber, newValue == 1.0); break;
        case COLUMN_ID_X: pSources->setX(rowNumber, newValue); break;
        case COLUMN_ID_Y: pSources->setY(rowNumber, newValue); break;
        case COLUMN_ID_Z: pSources->setZ(rowNumber, newValue); break;
        case COLUMN_ID_A: pSources->setAzimuth(rowNumber, Constants::GradToRad(newValue)); break;
        case COLUMN_ID_E: pSources->setElevation(rowNumber, Constants::GradToRad(newValue)); break;
        case COLUMN_ID_D: pSources->setDistance(rowNumber, newValue); break;
        case COLUMN_ID_COLOR: pSources->setChannelColor(rowNumber, Colour(uint32(newValue))); break;
        case COLUMN_ID_ENABLED: pSources->setEnabled(rowNumber, newValue == 1.0 && rowNumber < pChannelLayout->getNumInputChannels()); break;
        default: throw;
        }

        getTable()->updateContent();
        getTable()->repaint();
    }

    SliderRange getSliderRange(int columnId) override
    {
        switch (columnId)
        {
        case COLUMN_ID_X:
        case COLUMN_ID_Y:
        case COLUMN_ID_Z:
            return SliderRange(pScalingInfo->CartesianMin(), pScalingInfo->CartesianMax(), 0.001);

        case COLUMN_ID_D:
            return SliderRange(Constants::DistanceMin, pScalingInfo->DistanceMax(), 0.001);

        case COLUMN_ID_A:
            return SliderRange(Constants::AzimuthGradMin, Constants::AzimuthGradMax, 0.1);

        case COLUMN_ID_E:
            return SliderRange(Constants::ElevationGradMin, Constants::ElevationGradMax, 0.1);

        case COLUMN_ID_GAIN:
            return SliderRange(Constants::GainDbMin, Constants::GainDbMax, 0.5);
        default: return SliderRange(0.0, 1.0, 0.001);
        }
    }

    TableListBox* getTable() override
    {
        return pTableListBox;
    }

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

    void setTableText(const int columnId, const int rowNumber, const String& newText) override
    {
        switch (columnId)
        {
        case COLUMN_ID_NAME: pSources->setChannelName(rowNumber, newText); break;
        default: throw;
        }
    }

    void changeListenerCallback(ChangeBroadcaster* /*source*/) override
    {
        getTable()->updateContent();
        getTable()->repaint();
    }

private:
    AmbiSourceSet* pSources;
    PointSelection* pPointSelection;
    Component* pParentComponent;
    TableListBox* pTableListBox;
    ScalingInfo* pScalingInfo;
    ChannelLayout* pChannelLayout;
};
