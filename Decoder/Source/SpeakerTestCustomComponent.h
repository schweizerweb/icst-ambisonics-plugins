/*
  ==============================================================================

    SpeakerTestCustomComponent.h
    Created: 9 Oct 2017 2:52:14pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "SpeakerSettingsComponent.h"

class SpeakerTestCustomComponent : public Component,
	private Button::Listener
{
public:
	SpeakerTestCustomComponent(SpeakerSettingsComponent& td) : owner(td)
	{
		addAndMakeVisible(button);
		button.setImages(false, true, true, ImageCache::getFromMemory(BinaryData::speaker_png, BinaryData::speaker_pngSize), 1.0, Colours::transparentBlack, Image::null, 1.0, Colours::blue.withAlpha(0.3f), Image::null, 1.0, Colours::red.withAlpha(0.3f));
		button.addListener(this);
		button.setWantsKeyboardFocus(false);
	}

	void resized() override
	{
		button.setBoundsInset(BorderSize<int>(2));
	}

	void setRowAndColumn(int newRow, int newColumn)
	{
		row = newRow;
		columnId = newColumn;
	}

	void buttonClicked(Button*) override
	{
		owner.speakerTest(row);
	}

private:
	SpeakerSettingsComponent& owner;
	ImageButton button;
	int row, columnId;
};