/*
  ==============================================================================

    AmbiHelper.cpp
    Created: 13 Oct 2017 11:41:02pm
    Author:  chris

  ==============================================================================
*/

#include "AmbiHelper.h"

String AmbiHelper::getAmbiChannelName(int channelId)
{
	String ret = "";
	
	switch (channelId)
	{
	case 0: ret = "W"; break;
	case 1: ret = "X"; break;
	case 2: ret = "Y"; break;
	case 3: ret = "Z"; break;
	default: ret = String(channelId);
	}

	return ret;
}
