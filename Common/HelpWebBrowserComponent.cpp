/*
  ==============================================================================

    HelpWebBrowserComponent.cpp
    Created: 30 Mar 2020 8:04:10pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#include "HelpWebBrowserComponent.h"

HelpWebBrowserComponent::HelpWebBrowserComponent(BrowserMode mode)
{
    switch(mode)
    {
        case Documentation:
            if(String(JucePlugin_Name).contains("Encoder"))
                goToURL("https://bitbucket.org/christian_schweizer/icst-ambisonics-plugins/wiki/Encoder_specification");
            else
                goToURL("https://bitbucket.org/christian_schweizer/icst-ambisonics-plugins/wiki/Decoder_specification");
            break;
        case Tutorials:
            goToURL("https://ambisonics.postach.io");
            break;
    }
}
