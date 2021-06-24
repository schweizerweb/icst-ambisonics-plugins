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
        reset();
    }

    void reset()
    {
        enabled = false;
        groupIndex = -1;
        rotationX = 0.0;
        rotationY = 0.0;
        rotationZ = 0.0;
        rotationOriginX = 0.0;
        rotationOriginY = 0.0;
        rotationOriginZ = 0.0;
        stretch = 0.0;
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
