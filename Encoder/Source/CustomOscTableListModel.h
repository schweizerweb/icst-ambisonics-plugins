/*
  ==============================================================================

    CustomOscTableListModel.h
    Created: 24 Jan 2021 9:53:48pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "EncoderSettings.h"
#include "../../Common/SliderColumnCustomComponent.h"
#include "../../Common/EditableTextCustomComponent.h"
#include "../../Common/CheckBoxCustomComponent.h"
#include "../../Common/ColorDefinition.h"
#include "../../Common/PresetHelper.h"

#define COLUMN_ID_ENABLE		201
#define COLUMN_ID_HOST			202
#define	COLUMN_ID_PORT			203
#define	COLUMN_ID_PATH			204
#define COLUMN_ID_SAVE_AS_PRESET    206
#define ACTION_MESSAGE_DATA_CHANGED "data"
#define ACTION_MESSAGE_SEL_CHANGED "sel"


class CustomOscTableListModel : public TableListBoxModel, public TableColumnCallback, public ActionBroadcaster, ImageButton::Listener
{
public:
	CustomOscTableListModel(EncoderSettings* pSettings, Component* pParentComponent, ActionListener* pActionListener, const char* save_png, const int save_pngSize): pSettings(pSettings), pParentComponent(pParentComponent), pTableListBox(nullptr), save_png(save_png), save_pngSize(save_pngSize)
	{
		addActionListener(pActionListener);
        standardTargets.add(new StandardTarget("ICST AmbiPlugins Standard XYZ Name", pSettings->oscSendExtXyz.get()));
        standardTargets.add(new StandardTarget("ICST AmbiPlugins Standard AED Name", pSettings->oscSendExtAed.get()));
        standardTargets.add(new StandardTarget("ICST AmbiPlugins Standard XYZ Index", pSettings->oscSendExtXyzIndex.get()));
        standardTargets.add(new StandardTarget("ICST AmbiPlugins Standard AED Index", pSettings->oscSendExtAedIndex.get()));
	}

	~CustomOscTableListModel() override
	{
		removeAllActionListeners();
	}

    void buttonClicked(juce::Button *b) override {
        int rowIndex = b->getComponentID().getIntValue();
        sendActionMessage(String(ACTION_MESSAGE_SAVE_PRESET) + " " + String(rowIndex));
    }
    
	int getNumRows() override {
		return standardTargets.size() + pSettings->customOscTargets.size();
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

	void paintCell(Graphics& g, int /*rowNumber*/, int /*columnId*/, int width, int height, bool /*rowIsSelected*/) override 
	{
		g.setColour(pParentComponent->getLookAndFeel().findColour(ListBox::backgroundColourId));
		g.fillRect(width - 1, 0, 1, height);
	}

	Component* refreshComponentForCell(int rowNumber, int columnId, bool /*isRowSelected*/, Component* existingComponentToUpdate) override
	{
		if (columnId == COLUMN_ID_PORT)
		{
			SliderColumnCustomComponent* sliderBox = static_cast<SliderColumnCustomComponent*> (existingComponentToUpdate);
			if (sliderBox == nullptr)
				sliderBox = new SliderColumnCustomComponent(*this);

			sliderBox->setRowAndColumn(rowNumber, columnId);
			return sliderBox;
		}
		else if (columnId == COLUMN_ID_HOST
			|| columnId == COLUMN_ID_PATH)
		{
			EditableTextCustomComponent* textLabel = static_cast<EditableTextCustomComponent*> (existingComponentToUpdate);
			if (textLabel == nullptr)
				textLabel = new EditableTextCustomComponent(*this);

			textLabel->setRowAndColumn(rowNumber, columnId, columnId == COLUMN_ID_PATH && rowNumber < standardTargets.size());
			return textLabel;
		}
		else if (columnId == COLUMN_ID_ENABLE)
		{
			CheckBoxCustomComponent* checkBox = static_cast<CheckBoxCustomComponent*> (existingComponentToUpdate);
			if (checkBox == nullptr)
				checkBox = new CheckBoxCustomComponent(*this);

			checkBox->setRowAndColumn(rowNumber, columnId);
			return checkBox;
		}
        else if (columnId == COLUMN_ID_SAVE_AS_PRESET)
        {
            int customTargetIndex = getCustomTargetIndex(rowNumber);
            if(customTargetIndex >= 0)
            {
                ImageButton* btn = static_cast<ImageButton*>(existingComponentToUpdate);
                if (btn == nullptr) {
                    btn = new ImageButton();
                    btn->setImages (false, true, true,
                                    juce::ImageCache::getFromMemory (save_png, save_pngSize), 1.000f, juce::Colour (0x6effffff),
                                    juce::ImageCache::getFromMemory (save_png, save_pngSize), 0.400f, juce::Colour (0x6eee1010),
                                    juce::ImageCache::getFromMemory (save_png, save_pngSize), 1.000f, juce::Colour (0xc0ee1010));
                    btn->setTooltip("Add to presets...");
                    btn->addListener(this);
                }
                btn->setComponentID(String(customTargetIndex));
                return btn;
            }
        }
        
		return nullptr;
	}

	void selectedRowsChanged(int /*lastRowSelected*/) override
	{
		sendActionMessage(ACTION_MESSAGE_SEL_CHANGED);
	}

	double getValue(int columnId, int rowNumber) override 
	{
		if(rowNumber < standardTargets.size())
        {
            switch (columnId)
            {
                case COLUMN_ID_PORT: return standardTargets[rowNumber]->pTarget->targetPort;
                case COLUMN_ID_ENABLE: return standardTargets[rowNumber]->pTarget->enabledFlag;
                default: return 0.0;
            }
        }
        
		CustomOscTarget* t = pSettings->customOscTargets[rowNumber - standardTargets.size()];
		if (t == nullptr)
			return 0.0;

		switch (columnId)
		{
		case COLUMN_ID_PORT: return t->targetPort;
		case COLUMN_ID_ENABLE: return t->enabledFlag;
		default: return 0.0;
		}
	}

	void setValue(int columnId, int rowNumber, double newValue) override
	{
        if(rowNumber < standardTargets.size())
        {
            switch (columnId)
            {
                case COLUMN_ID_PORT: standardTargets[rowNumber]->pTarget->targetPort = (int)newValue; break;
                case COLUMN_ID_ENABLE: standardTargets[rowNumber]->pTarget->enabledFlag = newValue != 0.0; break;
                default: ;
            }
        }
        else
        {
            rowNumber -= standardTargets.size();
            switch (columnId)
            {
                case COLUMN_ID_PORT: pSettings->customOscTargets[rowNumber]->targetPort = (int)newValue; break;
                case COLUMN_ID_ENABLE: pSettings->customOscTargets[rowNumber]->enabledFlag = newValue != 0.0; break;
                default: ;
            }
        }
        
		getTable()->updateContent();
		getTable()->repaint();

		sendActionMessage(ACTION_MESSAGE_DATA_CHANGED);
	}
	
	SliderRange getSliderRange(int columnId) override 
	{
		switch (columnId)
		{
		case COLUMN_ID_PORT:
			return SliderRange(0, 65535, 1);
		default: return SliderRange(0.0, 1.0, 0.001);
		}
	}

	TableListBox* getTable() override
	{
		return pTableListBox;
	}

	String getTableText(const int columnId, const int rowNumber) override
	{
        if(rowNumber < standardTargets.size())
        {
            switch (columnId)
            {
                case COLUMN_ID_HOST: return standardTargets[rowNumber]->pTarget->targetHost;
                case COLUMN_ID_PATH: return standardTargets[rowNumber]->name;
            default: return "";
            }
        }
        
        int row = rowNumber - standardTargets.size();
		switch (columnId)
		{
		case COLUMN_ID_HOST: return pSettings->customOscTargets[row]->targetHost; break;
		case COLUMN_ID_PATH: return pSettings->customOscTargets[row]->oscString; break;
		default: return "";
		}
	}

	void setTableText(const int columnId, const int rowNumber, const String& newText) override
	{
        if(rowNumber < standardTargets.size())
        {
            switch (columnId)
            {
                case COLUMN_ID_HOST: standardTargets[rowNumber]->pTarget->targetHost = newText; break;
                case COLUMN_ID_PATH: return;
            default: return;
            }
        }
        else
        {
            int row = rowNumber - standardTargets.size();
            switch (columnId)
            {
                case COLUMN_ID_HOST: pSettings->customOscTargets[row]->targetHost = newText; break;
                case COLUMN_ID_PATH: pSettings->customOscTargets[row]->oscString = newText; break;
                default: ;
            }
        }
        
		sendActionMessage(ACTION_MESSAGE_DATA_CHANGED);
	}

	void initTable(TableListBox* tableListBox)
	{
		pTableListBox = tableListBox;
		tableListBox->setModel(this);
		tableListBox->getHeader().addColumn("Enable", COLUMN_ID_ENABLE, 20);
		tableListBox->getHeader().addColumn("Host", COLUMN_ID_HOST, 70);
		tableListBox->getHeader().addColumn("Port", COLUMN_ID_PORT, 50);
		tableListBox->getHeader().addColumn("OSC-Message", COLUMN_ID_PATH, 300);
        tableListBox->getHeader().addColumn("", COLUMN_ID_SAVE_AS_PRESET, 20);
        tableListBox->getHeader().setStretchToFitActive(true);
		tableListBox->getHeader().resizeAllColumnsToFit(tableListBox->getWidth());
	}

    bool getEnabled(const int /*columnId*/, const int /*rowNumber*/) override
	{
	    return true;
	}
    
    int getCustomTargetIndex(int selectedIndex)
    {
        return jmax(-1, selectedIndex - standardTargets.size());
    }

private:
    struct StandardTarget {
        StandardTarget(String name, StandardOscTarget* pTarget) : name(name), pTarget(pTarget) {};
        String name;
        StandardOscTarget* pTarget;
    };
    
	EncoderSettings* pSettings;
	Component* pParentComponent;
	TableListBox* pTableListBox;
    OwnedArray<StandardTarget> standardTargets;
    const char* save_png;
    const int save_pngSize;
};
