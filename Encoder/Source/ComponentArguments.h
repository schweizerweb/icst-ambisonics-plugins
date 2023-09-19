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
#include "../../Common/StatusMessageHandler.h"
#include "EncoderPresetHelper.h"
#include "DistanceEncodingPresetHelper.h"
#include "CustomOscRxPresetHelper.h"
#include "CustomOscTxPresetHelper.h"
#include "../../Common/OSCLogDialogManager.h"
#include "../../Common/DawParameter.h"
#include "../../Common/ChannelLayout.h"
#include "OSCHandlerEncoder.h"

struct EncoderSettingsComponentArgs
{
    ChangeListener* pChangeListener;
    EncoderSettings* pSettings;
    AmbiSourceSet* pSourceSet;
    PointSelection* pPointSelection;
    AudioParams* pAudioParams;
    ZoomSettings* pZoomSettings;
    StatusMessageHandler* pStatusMessageHandler;
    EncoderPresetHelper* pPresetHelper;
    DistanceEncodingPresetHelper* pDistanceEncodingPresetHelper;
    CustomOscRxPresetHelper* pCustomOscRxPresetHelper;
    CustomOscTxPresetHelper* pCustomOscTxPresetHelper;
    OSCLogDialogManager* pOscLogManager;
    DawParameter* pDawParams;
    OSCHandlerEncoder* pOscHandler;
    ChannelLayout* pChannelLayout;
};
