/*
  ==============================================================================

    AmbiSpeaker.cpp
    Created: 30 Oct 2019 11:40:30am
    Author:  chris

  ==============================================================================
*/

#include "AmbiSpeaker.h"


AmbiSpeaker::AmbiSpeaker(AmbiSpeaker* other, bool copyImage) : AmbiPoint(other, copyImage), isSubwoofer(false)
{
}

AmbiSpeaker::AmbiSpeaker(String id, Point3D<double> point, String name, Colour color, double gain) : AmbiPoint(id, point, name, color, gain), isSubwoofer(false)
{
}

AmbiSpeaker::AmbiSpeaker(XmlElement* element) : AmbiPoint(element), isSubwoofer(false)
{
}

void AmbiSpeaker::setSubwooferFlag(bool flag)
{
	isSubwoofer = flag;
}

bool AmbiSpeaker::getSubwooferFlag()
{
	return isSubwoofer;
}

float AmbiSpeaker::getDisplayScaler()
{
	return 1.0;
}
