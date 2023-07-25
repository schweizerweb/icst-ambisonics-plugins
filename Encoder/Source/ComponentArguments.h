/*
  ==============================================================================

    ComponentArguments.h
    Created: 6 Sep 2022 11:28:30pm
    Author:  Christian Schweizer

  ==============================================================================
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
};
