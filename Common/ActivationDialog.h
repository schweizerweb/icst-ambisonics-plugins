/*
  ==============================================================================

    ActivationDialog.h
    Created: 8 Feb 2021 7:00:28pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class ActivationDialog : public Component, ComboBox::Listener, Button::Listener
{
public:
    ActivationDialog(AmbiDataSet* pData, Point<float> newPosition, bool xyRadar) : pData(pData), newPosition(newPosition), xyRadar(xyRadar)
    {        
        comboLabel.reset(new Label("channel"));
        comboLabel->setText("Channel:", dontSendNotification);
        addAndMakeVisible(comboLabel.get());

        combo.reset(new ComboBox("combo"));
        for(int i = 0; i < pData->size(); i++)
        {
            if(pData->get(i) != nullptr && !pData->get(i)->getEnabled())
            {
                combo->addItem(String(i + 1), i + 1);
            }
        }
        combo->addListener(this);
        addAndMakeVisible(combo.get());

        textLabel.reset(new Label("name"));
        textLabel->setText("Name:", dontSendNotification);
        addAndMakeVisible(textLabel.get());

        text.reset(new TextEditor("textedit"));
        addAndMakeVisible(text.get());

        btn.reset(new TextButton("btn"));
        btn->setButtonText("Activate");
        btn->addListener(this);
        addAndMakeVisible(btn.get());

        combo->setSelectedItemIndex(0);

        setSize(200, 90);
    }

    ~ActivationDialog() override
    {
        comboLabel = nullptr;
        combo = nullptr;
        textLabel = nullptr;
        text = nullptr;
        btn = nullptr;
    }

private:
    void resized() override
    {
        comboLabel->setBounds(0, 0, getWidth() / 3, 24);
        combo->setBounds(comboLabel->getRight(), 0, getWidth() - comboLabel->getRight(), 24);

        textLabel->setBounds(0, 30, getWidth() / 3, 24);
        text->setBounds(textLabel->getRight(), 30, getWidth() - textLabel->getRight(), 24);

        btn->setBounds(0, 60, getWidth(), getHeight() - 60);
    }

    void comboBoxChanged(ComboBox*) override
    {
        text->setText(pData->get(combo->getSelectedId() - 1)->getName());
    }

    void buttonClicked(Button*) override
    {
        AmbiPoint* p = pData->get(combo->getSelectedId() - 1);
        p->setEnabled(true);
        p->setName(text->getText());
        p->getRawPoint()->setXYZ(newPosition.getX(), xyRadar ? newPosition.getY() : 0.0, xyRadar ? 0.0 : newPosition.getY());
        findParentComponentOfClass<CallOutBox>()->exitModalState(0);
    }

private:
    std::unique_ptr<Label> comboLabel;
    std::unique_ptr<ComboBox> combo;

    std::unique_ptr<Label> textLabel;
    std::unique_ptr<TextEditor> text;

    std::unique_ptr<TextButton> btn;
    AmbiDataSet* pData;
    Point<float> newPosition;
    bool xyRadar;
};
