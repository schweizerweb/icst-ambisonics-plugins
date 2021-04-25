/*
  ==============================================================================

    AnimatorAction.h
    Created: 25 Apr 2021 12:56:18pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
class AnimatorAction
{
public:
    AnimatorAction()
    {
        enabled = false;
        groupIndex = -1;
    }

    bool enabled;
    int groupIndex;
    double rotationX;
    double rotationY;
    double rotationZ;
    double rotationOriginX;
    double rotationOriginY;
    double rotationOriginZ;
    double stretch;
};
