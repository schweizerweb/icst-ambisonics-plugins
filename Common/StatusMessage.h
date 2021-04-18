/*
  ==============================================================================

    StatusMessage.h
    Created: 17 Apr 2021 4:43:45pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
struct StatusMessage
{
    enum MessageStyle { Error, Success } messageStyle;
    Time timestamp;
    String message;
};
