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

#include <JuceHeader.h>
#include "IIRFilterGraph.h"
#include "DecoderSectionControlComponent.h"
#include "../../Common/AmbiSettingsCollection.h"
#include "../../Common/AmbiSpeakerSet.h"
#include "../../Common/ChannelLayout.h"
#include "FilterPresetHelper.h"

class MultiDecoderComponent  : public juce::Component,
                               public juce::Slider::Listener,
                               public ChangeListener,
                               public ChangeBroadcaster
{
public:
    MultiDecoderComponent (AmbiSettingsCollection* _pAmbiSettings, AmbiSpeakerSet* pSpeakerSet, FilterPresetHelper* _pPresetHelper, dsp::ProcessSpec* _pFilterSpecification, ChangeListener* pChangeListener, ChannelLayout* _pChannelLayout);
    ~MultiDecoderComponent() override;

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void refresh();

private:
    void controlDimming();
    void updateUI();

    std::unique_ptr<IIRFilterGraph> filterCurve;
    std::unique_ptr<juce::Slider> sliderDecoderCount;
    std::unique_ptr<juce::Label> labelDecoderCount;

    OwnedArray<DecoderSectionControlComponent> sectionControls;
    
    AmbiSettingsCollection* pAmbiSettings;
    AmbiSpeakerSet* pSpeakerSet;
    FilterPresetHelper* pFilterPresetHelper;
    dsp::ProcessSpec* pFilterSpecification;
    ChannelLayout* pChannelLayout;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiDecoderComponent)

        // Inherited via ChangeListener
        void changeListenerCallback(ChangeBroadcaster* source) override;
};

//[EndFile] You can add extra defines here...
//[/EndFile]

