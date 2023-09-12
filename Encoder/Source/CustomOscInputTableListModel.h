/*
  =============================================================================

    CustomOscInputTableListModel.
    Created: 17 Jan 2022 10:24:05p
    Author:  Schweizer Christia

  =============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "EncoderSettings.h"
#include "OSCHandlerEncoder.h"
#include "../../Common/SliderColumnCustomComponent.h"
#include "../../Common/EditableTextCustomComponent.h"
#include "../../Common/EditableCodeCustomComponent.h"
#include "../../Common/CheckBoxCustomComponent.h"
#include "../../Common/ColorDefinition.h"
#include "../../Common/CommonImages.h"

#define COLUMN_ID_ENABLE		201
#define COLUMN_ID_INFO        202
#define	COLUMN_ID_PATH			204
#define COLUMN_ID_COMMAND       205
#define COLUMN_ID_SAVE_AS_PRESET    206
#define ACTION_MESSAGE_DATA_CHANGED "data"
#define ACTION_MESSAGE_SEL_CHANGED "sel"
#define BUTTON_TYPE_PRESET  "preset"
#define BUTTON_TYPE_INFO    "info"


class CustomOscInputTableListModel : public TableListBoxModel, public TableColumnCallback, public ActionBroadcaster, ImageButton::Listener, ActionListener, Timer
{
public:
	CustomOscInputTableListModel(EncoderSettings* _pSettings, OSCHandlerEncoder* _pOscHandler, Component* _pParentComponent, ActionListener* pActionListener): pSettings(_pSettings), pOscHandler(_pOscHandler), pParentComponent(_pParentComponent), pTableListBox(nullptr)
	{
		addActionListener(pActionListener);
        pOscHandler->addActionListener(this);
        startTimerHz(1);
	}

	~CustomOscInputTableListModel() override
	{
        pOscHandler->removeActionListener(this);
		removeAllActionListeners();
    }
    
    void timerCallback() override {
        // force periodic ui updates for status display
        getTable()->repaint();
    }
    
    
    void actionListenerCallback(const juce::String& /*message*/) override { 
        getTable()->repaint();
    }
    
    
    void buttonClicked(juce::Button *b) override {
        if(b->getName() == BUTTON_TYPE_PRESET)
        {
            int rowIndex = b->getComponentID().getIntValue();
            sendActionMessage(String(ACTION_MESSAGE_SAVE_PRESET) + " " + String(rowIndex));
        }
        else if(b->getName() == BUTTON_TYPE_INFO)
        {
            int rowIndex = b->getComponentID().getIntValue();
            bool isInit;
            bool hasIncomingData;
            bool hasSuccessfulIncomingData;
            String errorMessage;
            String info;
            if(!pSettings->oscReceiveFlag)
            {
                info += "OSC Receive disabled";
            }
            else if(pSettings->customOscInput[rowIndex]->enabledFlag)
            {
                pOscHandler->getReceiverStatus(rowIndex, &isInit, &hasIncomingData, &hasSuccessfulIncomingData, &errorMessage);
                if(isInit)
                {
                    info += "Initialized";
                    if(hasIncomingData)
                    {
                        info += " - Receiving data";
                    }
                    else
                    {
                        info += " - No incoming data";
                    }
                }
                else
                {
                    info += "Error initializing";
                }
                
                if(!errorMessage.isEmpty())
                {
                    info += ": " + errorMessage;
                }
            }
            else
            {
                info = "Disabled";
            }
            
            std::unique_ptr<Label> label = std::make_unique<Label>();
            label->setText(info , dontSendNotification);
            int lineCount = label->getFont().getStringWidth(info) / 180;
            int lineHeight = roundToInt(label->getFont().getHeight());
            label->setSize(200, lineHeight * (lineCount+2));
            label->setJustificationType(Justification::centred);
            CallOutBox::launchAsynchronously(std::move(label), b->getScreenBounds(), nullptr);
        }
    }
    

	int getNumRows() override {
		return pSettings->customOscInput.size();
	}

	void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override
	{
        const Colour defaultColor = pParentComponent->getLookAndFeel().findColour(ListBox::backgroundColourId);
		const Colour alternateColour(defaultColor.interpolatedWith(pParentComponent->getLookAndFeel().findColour(ListBox::textColourId), COLOR_DEFINITION_ALTERNATE_INTENSITY));
        
        auto fillColor = rowIsSelected ? COLOR_DEFINITION_SELECTED_ROW : (rowNumber % 2 ? alternateColour : defaultColor);
		g.fillAll(fillColor);
        
        if(pSettings->oscReceiveFlag && pSettings->customOscInput[rowNumber]->enabledFlag)
        {
            bool isInit;
            bool hasIncomingData;
            bool hasSuccessfulIncomingData;
            String errorMessage;
            pOscHandler->getReceiverStatus(rowNumber, &isInit, &hasIncomingData, &hasSuccessfulIncomingData, &errorMessage);
            auto gradientColor = Colours::red;
            if(isInit)
            {
                if(!errorMessage.isEmpty())
                {
                    gradientColor = Colours::darkorange;
                }
                else
                {
                    if(hasSuccessfulIncomingData)
                        gradientColor = Colours::green;
                    else
                        gradientColor = Colours::yellow;
                }
            }
            if(!errorMessage.isEmpty())
            {
                // Todo: Tooltip
            }
            
            g.setGradientFill(ColourGradient(gradientColor, 0.0f, 0.0f, fillColor, (float)width/2.0f, 0.0f, false));
            g.fillRect(0, 0, width, height);
        }
	}

	void paintCell(Graphics& g, int /*rowNumber*/, int /*columnId*/, int width, int height, bool /*rowIsSelected*/) override 
	{
		g.setColour(pParentComponent->getLookAndFeel().findColour(ListBox::backgroundColourId));
		g.fillRect(width - 1, 0, 1, height);
	}

	Component* refreshComponentForCell(int rowNumber, int columnId, bool /*isRowSelected*/, Component* existingComponentToUpdate) override
	{
		if (columnId == COLUMN_ID_PATH)
		{
			EditableTextCustomComponent* textLabel = static_cast<EditableTextCustomComponent*> (existingComponentToUpdate);
			if (textLabel == nullptr)
				textLabel = new EditableTextCustomComponent(*this);

			textLabel->setRowAndColumn(rowNumber, columnId);
			return textLabel;
		}
        else if (columnId == COLUMN_ID_COMMAND)
        {
            EditableCodeCustomComponent* textLabel = static_cast<EditableCodeCustomComponent*> (existingComponentToUpdate);
            if (textLabel == nullptr)
                textLabel = new EditableCodeCustomComponent(*this);

            textLabel->setRowAndColumn(rowNumber, columnId);
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
            ImageButton* btn = static_cast<ImageButton*>(existingComponentToUpdate);
            if (btn == nullptr) {
                btn = new ImageButton();
                btn->setImages (false, true, true,
                                juce::ImageCache::getFromMemory (CommonImages::save_png, CommonImages::save_pngSize), 1.000f, juce::Colour (0x6effffff),
                                juce::ImageCache::getFromMemory (CommonImages::save_png, CommonImages::save_pngSize), 0.400f, juce::Colour (0x6eee1010),
                                juce::ImageCache::getFromMemory (CommonImages::save_png, CommonImages::save_pngSize), 1.000f, juce::Colour (0xc0ee1010));
                btn->setTooltip("Add to presets...");
                btn->setName(BUTTON_TYPE_PRESET);
                btn->addListener(this);
            }
            
            btn->setComponentID(String(rowNumber));
            return btn;
        }
        else if (columnId == COLUMN_ID_INFO)
        {
            ImageButton* btn = static_cast<ImageButton*>(existingComponentToUpdate);
            if(btn == nullptr)
            {
                btn = new ImageButton();
                btn->setImages(false, true, true,
                               juce::ImageCache::getFromMemory (CommonImages::info_png, CommonImages::info_pngSize), 1.000f, juce::Colour (0x6effffff),
                               juce::ImageCache::getFromMemory (CommonImages::info_png, CommonImages::info_pngSize), 0.400f, juce::Colour (0x6eee1010),
                               juce::ImageCache::getFromMemory (CommonImages::info_png, CommonImages::info_pngSize), 1.000f, juce::Colour (0xc0ee1010));
                btn->setName(BUTTON_TYPE_INFO);
                btn->addListener(this);
            }
            
            btn->setComponentID(String(rowNumber));
            return btn;
        }
		
		return nullptr;
	}

	void selectedRowsChanged(int /*lastRowSelected*/) override
	{
		sendActionMessage(ACTION_MESSAGE_SEL_CHANGED);
        getTable()->updateContent();
        getTable()->repaint();
	}

	double getValue(int columnId, int rowNumber) override 
	{
		CustomOscInput* t = pSettings->customOscInput[rowNumber];
		if (t == nullptr)
			return 0.0;

		switch (columnId)
		{
		case COLUMN_ID_ENABLE: return t->enabledFlag;
		default: return 0.0;
		}
	}

	void setValue(int columnId, int rowNumber, double newValue) override
	{
        switch (columnId)
        {
            case COLUMN_ID_ENABLE: pSettings->customOscInput[rowNumber]->enabledFlag = !exactlyEqual(newValue, 0.0); break;
            default: ;
        }
        
        pOscHandler->initialize();
        getTable()->updateContent();
        getTable()->repaint();
	}
	
	SliderRange getSliderRange(int /*columnId*/) override 
	{
		// switch (columnId)
		// {
		// default:
		    return SliderRange(0.0, 1.0, 0.001);
		// }
	}

	TableListBox* getTable() override
	{
		return pTableListBox;
	}

	String getTableText(const int columnId, const int rowNumber) override
	{
        switch (columnId)
		{
		case COLUMN_ID_PATH: return pSettings->customOscInput[rowNumber]->oscString; break;
		case COLUMN_ID_COMMAND: return pSettings->customOscInput[rowNumber]->commandString; break;
        default: return "";
		}
	}

	void setTableText(const int columnId, const int rowNumber, const String& newText) override
	{
        switch (columnId)
        {
            case COLUMN_ID_PATH: pSettings->customOscInput[rowNumber]->oscString = newText; break;
            case COLUMN_ID_COMMAND:
                pSettings->customOscInput[rowNumber]->commandString = newText;
                getTable()->updateContent();
                getTable()->repaint();
                break;
            default: ;
        }
        
        pOscHandler->initialize();
        getTable()->updateContent();
        getTable()->repaint();
	}

	void initTable(TableListBox* tableListBox)
	{
		pTableListBox = tableListBox;
		tableListBox->setModel(this);
		tableListBox->getHeader().addColumn("En", COLUMN_ID_ENABLE, 30, 30, 30);
        tableListBox->getHeader().addColumn("", COLUMN_ID_INFO, 30, 30, 30);
		tableListBox->getHeader().addColumn("OSC-Message", COLUMN_ID_PATH, 300);
        tableListBox->getHeader().addColumn("JS-Code", COLUMN_ID_COMMAND, 270);
        tableListBox->getHeader().addColumn("", COLUMN_ID_SAVE_AS_PRESET, 30, 30, 30);
		tableListBox->getHeader().setStretchToFitActive(true);
		tableListBox->getHeader().resizeAllColumnsToFit(tableListBox->getWidth());
	}

    bool getEnabled(const int /*columnId*/, const int /*rowNumber*/) override
	{
	    return true;
	}
    
    int getCustomIndex(int selectedIndex)
    {
        return selectedIndex;
    }

private:
    
	EncoderSettings* pSettings;
    OSCHandlerEncoder* pOscHandler;
	Component* pParentComponent;
	TableListBox* pTableListBox;
};
