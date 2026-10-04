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
#include "../../Common/AmbiSourceSet.h"
#include "OscPointScope.h"

// Two-section (sources / groups) toggle picker for an OSC target's "Select..." scope, shown in a
// CallOutBox anchored to the grid's "..." button. Edits apply immediately into *pSelection (same
// immediate-apply pattern as the clip colour picker elsewhere in this codebase) - there's no
// separate OK/Apply step.
//
// Each section wraps into multiple columns (same approach as GroupPointsSelectionComponent) once it
// exceeds maxRowsPerColumn entries, so a full 64-source set stays a few hundred pixels tall instead
// of one long list that runs off the bottom of the screen.
class OscPointSelectionComponent : public Component, public ToggleButton::Listener
{
public:
    OscPointSelectionComponent(AmbiSourceSet* _pSources, OscPointSelection* _pSelection, std::function<void()> _onChanged)
        : pSources(_pSources), pSelection(_pSelection), onChanged(std::move(_onChanged))
    {
        warningLabel.setText(
            "Selection is by position in the list. Reordering, adding, or removing "
            "sources or groups later may change what this target actually sends.",
            dontSendNotification);
        warningLabel.setFont(Font(FontOptions(12.0f)));
        warningLabel.setColour(Label::textColourId, Colours::orange);
        warningLabel.setJustificationType(Justification::topLeft);
        addAndMakeVisible(warningLabel);

        sourcesHeader.setText("Sources", dontSendNotification);
        sourcesHeader.setFont(Font(FontOptions(13.0f, Font::bold)));
        addAndMakeVisible(sourcesHeader);

        for (int i = 0; i < pSources->size(); i++)
        {
            AmbiPoint* s = pSources->get(i);
            auto* b = new ToggleButton(String(i + 1) + ": " + s->getName());
            b->setToggleState(pSelection->sourceIndices.contains(i), dontSendNotification);
            b->addListener(this);
            sourceToggles.add(b);
            addAndMakeVisible(b);
        }

        groupsHeader.setText("Groups", dontSendNotification);
        groupsHeader.setFont(Font(FontOptions(13.0f, Font::bold)));
        addAndMakeVisible(groupsHeader);

        for (int i = 0; i < pSources->groupCount(); i++)
        {
            AmbiGroup* g = pSources->getGroup(i);
            auto* b = new ToggleButton(String(i + 1) + ": " + g->getName());
            b->setToggleState(pSelection->groupIndices.contains(i), dontSendNotification);
            b->addListener(this);
            groupToggles.add(b);
            addAndMakeVisible(b);
        }

        sourceColumns = jmax(1, (int)std::ceil(sourceToggles.size() / (double)maxRowsPerColumn));
        sourceRows = sourceToggles.size() > 0 ? (int)std::ceil(sourceToggles.size() / (double)sourceColumns) : 0;

        groupColumns = jmax(1, (int)std::ceil(groupToggles.size() / (double)maxRowsPerColumn));
        groupRows = groupToggles.size() > 0 ? (int)std::ceil(groupToggles.size() / (double)groupColumns) : 0;

        const int width = jmax(minWidth, jmax(sourceColumns, groupColumns) * columnWidth + 2 * margin);
        const int height = 2 * margin + warningHeight + spacing
                          + rowHeight + sourceRows * rowHeight
                          + spacing
                          + rowHeight + groupRows * rowHeight;
        setSize(width, height);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(margin);
        int x = area.getX();
        int y = area.getY();

        warningLabel.setBounds(x, y, area.getWidth(), warningHeight);
        y += warningHeight + spacing;

        sourcesHeader.setBounds(x, y, area.getWidth(), rowHeight);
        y += rowHeight;

        for (int i = 0; i < sourceToggles.size(); i++)
        {
            const int col = i / sourceRows;
            const int row = i % sourceRows;
            sourceToggles[i]->setBounds(x + col * columnWidth, y + row * rowHeight, columnWidth, rowHeight);
        }
        y += sourceRows * rowHeight + spacing;

        groupsHeader.setBounds(x, y, area.getWidth(), rowHeight);
        y += rowHeight;

        for (int i = 0; i < groupToggles.size(); i++)
        {
            const int col = i / groupRows;
            const int row = i % groupRows;
            groupToggles[i]->setBounds(x + col * columnWidth, y + row * rowHeight, columnWidth, rowHeight);
        }
    }

    void buttonClicked(Button* b) override
    {
        auto* toggle = static_cast<ToggleButton*>(b);

        int sourceIndex = sourceToggles.indexOf(toggle);
        if (sourceIndex >= 0)
        {
            if (toggle->getToggleState())
                pSelection->sourceIndices.addIfNotAlreadyThere(sourceIndex);
            else
                pSelection->sourceIndices.removeFirstMatchingValue(sourceIndex);
        }
        else
        {
            int groupIndex = groupToggles.indexOf(toggle);
            if (groupIndex >= 0)
            {
                if (toggle->getToggleState())
                    pSelection->groupIndices.addIfNotAlreadyThere(groupIndex);
                else
                    pSelection->groupIndices.removeFirstMatchingValue(groupIndex);
            }
        }

        if (onChanged)
            onChanged();
    }

private:
    static constexpr int columnWidth = 170;
    static constexpr int rowHeight = 22;
    static constexpr int warningHeight = 44;
    static constexpr int spacing = 8;
    static constexpr int margin = 10;
    static constexpr int minWidth = 320;
    static constexpr int maxRowsPerColumn = 16;

    AmbiSourceSet* pSources;
    OscPointSelection* pSelection;
    std::function<void()> onChanged;

    Label warningLabel;
    Label sourcesHeader, groupsHeader;
    OwnedArray<ToggleButton> sourceToggles;
    OwnedArray<ToggleButton> groupToggles;
    int sourceColumns = 1, sourceRows = 0;
    int groupColumns = 1, groupRows = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscPointSelectionComponent)
};
