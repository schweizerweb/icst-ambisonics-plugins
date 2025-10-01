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
#include "SpeakerSettingsComponent.h"

class SpeakerTestCustomComponent : public Component,
	private Button::Listener
{
public:
	SpeakerTestCustomComponent(SpeakerSettingsComponent& td) : owner(td)
	{
		addAndMakeVisible(button);
		button.setImages(false, true, true, ImageCache::getFromMemory(BinaryData::speaker_png, BinaryData::speaker_pngSize), 1.0, Colours::transparentBlack, Image(), 1.0, Colours::blue.withAlpha(0.3f), Image(), 1.0, Colours::red.withAlpha(0.3f));
		button.setClickingTogglesState(true);
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
