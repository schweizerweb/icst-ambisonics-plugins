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
#define CHECKBOX_HEIGHT 25
#define COMPONENT_WIDTH 200
//==============================================================================
/*
*/
class GroupPointsSelectionComponent    : public Component, public ToggleButton::Listener, ChangeBroadcaster
{
public:
    GroupPointsSelectionComponent(AmbiSourceSet* pSources, int groupIndex, ChangeListener* pListener): pSources(pSources)
    {
	    // In your constructor, you should add any child components, and
	    // initialise any special settings that your component needs.
		
    	this->addChangeListener(pListener);
		
    	pGroup = pSources->getGroup(groupIndex);

        columnCount = (int)std::ceil(pSources->size() / 16.0);
        rowCount = columnCount > 0 ?  (int)(std::ceil(pSources->size() / (double)columnCount)) : 0;
        
		for (int i = 0; i < pSources->size(); i++)
		{
            AmbiSource* s = pSources->get(i);
			ToggleButton* b = new ToggleButton(s->getName());
			b->setToggleState(pGroup->groupPoints.contains(s), dontSendNotification);
            b->setEnabled(s->getEnabled());
			b->addListener(this);
			toggleButtons.add(b);
			addAndMakeVisible(b);
		}

		setSize(COMPONENT_WIDTH * columnCount, rowCount * CHECKBOX_HEIGHT);
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
        int index = 0;

		for(ToggleButton* b : toggleButtons)
		{
			b->setBounds((int)(std::floor(index / rowCount) * COMPONENT_WIDTH), (index % rowCount) * CHECKBOX_HEIGHT , COMPONENT_WIDTH, CHECKBOX_HEIGHT);
            index++;
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

			sendChangeMessage();
		}
	}

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GroupPointsSelectionComponent)

	AmbiSourceSet* pSources;
	Array<ToggleButton*> toggleButtons;
	AmbiGroup* pGroup;
    int columnCount;
    int rowCount;
};
