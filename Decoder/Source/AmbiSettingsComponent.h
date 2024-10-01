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
#include "../../Common/AmbiSettings.h"
#include "../../Common/AmbiSpeakerSet.h"
#include "../../Common/MultiSliderControl.h"
#include "../../Common/ChannelLayout.h"
#include "FilterPresetHelper.h"

class AmbiSettingsComponent : public juce::Component,
                               public juce::ComboBox::Listener,
                               public ChangeListener,
                               public ChangeBroadcaster,
                               public ActionListener
{
public:
    AmbiSettingsComponent(AmbiSettings* _pAmbiSettings, ChangeListener* pChangeListener, ChannelLayout* _pChannelLayout);
    ~AmbiSettingsComponent() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    void controlDimming();
    void updateUI() const;
    void handleAmbiOrders();

    AmbiSettings* pAmbiSettings;
    ChannelLayout* pChannelLayout;

    std::unique_ptr<juce::Label> labelAmbiOrder;
    std::unique_ptr<juce::ComboBox> comboAmbiOrder;

    std::unique_ptr<juce::ComboBox> comboBoxChannelWeightingMode;
    std::unique_ptr<MultiSliderControl> ambiChannelControl;
    std::unique_ptr<juce::Label> labelChannelWeights;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmbiSettingsComponent)

        // Inherited via ChangeListener
        void changeListenerCallback(ChangeBroadcaster* source) override;

    // Inherited via Listener
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;

    // Inherited via ActionListener
    void actionListenerCallback(const String& message) override;
};