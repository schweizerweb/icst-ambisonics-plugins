/*
  ==============================================================================

    GroupPointsSelectionComponent.h
    Created: 9 Nov 2019 3:35:35pm
    Author:  chris

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../Common/AmbiSourceSet.h"
#define CHECKBOX_HEIGHT 30
#define COMPONENT_WIDTH 200
//==============================================================================
/*
*/
class GroupPointsSelectionComponent    : public Component, public ToggleButton::Listener
{
public:
    GroupPointsSelectionComponent(AmbiSourceSet* pSources, int groupIndex): pSources(pSources)
    {
	    // In your constructor, you should add any child components, and
	    // initialise any special settings that your component needs.

		pGroup = pSources->getGroup(groupIndex);

		for (int i = 0; i < pSources->size(); i++)
		{
			ToggleButton* b = new ToggleButton(pSources->get(i)->getName());
			b->setToggleState(pGroup->groupPoints.contains(pSources->get(i)), dontSendNotification);
			b->addListener(this);
			toggleButtons.add(b);
			addAndMakeVisible(b);
		}

		setSize(COMPONENT_WIDTH, toggleButtons.size() * CHECKBOX_HEIGHT);
    }

	~GroupPointsSelectionComponent()
    {
		deleteAllChildren();
    }

    void paint (Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

		int y = 0;

		for(ToggleButton* b : toggleButtons)
		{
			b->setBounds(0, y, COMPONENT_WIDTH, CHECKBOX_HEIGHT);
			y += CHECKBOX_HEIGHT;
		}
    }

	void buttonClicked(Button* b) override
	{
		int index = toggleButtons.indexOf(static_cast<ToggleButton*>(b));
		
		if(index >= 0)
		{
			if (static_cast<ToggleButton*>(b)->getToggleState())
				pGroup->groupPoints.add(pSources->get(index));
			else
			{
				for (int i = 0; i < pGroup->groupPoints.size(); i++)
				{
					if (pGroup->groupPoints[i] == pSources->get(index))
					{
						pGroup->groupPoints.remove(i);
						break;
					}
				}
			}
		}
	}

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GroupPointsSelectionComponent)

	AmbiSourceSet* pSources;
	Array<ToggleButton*> toggleButtons;
	AmbiGroup* pGroup;
};
