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



//[Headers] You can add your own extra header files here...
//[/Headers]

#include "RadarComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
RadarComponent::RadarComponent (AmbiDataSet* pEditablePoints, AmbiDataSet* pDisplayOnlyPoints, PointSelection* pPointSelection, RadarOptions* pRadarOptions)
{
    //[Constructor_pre] You can add your own custom stuff here..
    showPointInfo = true;
    //[/Constructor_pre]

    radar.reset (new Radar3D (pEditablePoints, pDisplayOnlyPoints, pPointSelection, pRadarOptions));
    addAndMakeVisible (radar.get());
    radar->setName ("radar");

    pointInfo.reset (new PointInfoControl (pEditablePoints, pPointSelection, pRadarOptions));
    addAndMakeVisible (pointInfo.get());
    pointInfo->setName ("pointInfo");


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

RadarComponent::~RadarComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    radar = nullptr;
    pointInfo = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void RadarComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff505050));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void RadarComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	if(!showPointInfo)
	{
		radar->setBounds(0, 0, getWidth() - 0, getHeight());
		pointInfo->setBounds(0, 0, 0, 0);
	}
	else
	{
    //[/UserPreResize]

    radar->setBounds (0, 0, getWidth() - 0, getHeight() - 100);
    pointInfo->setBounds (0, getHeight() - 100, getWidth() - 0, 100);
    //[UserResized] Add your own custom resize handling here..
	}
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void RadarComponent::setPointInfoVisible(bool visible)
{
    this->showPointInfo = visible;
    startTimer(100); // pseudo async resize call to avoid OSX crash
}

void RadarComponent::timerCallback()
{
    stopTimer();
    resized();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="RadarComponent" componentName=""
                 parentClasses="public Component, public Timer" constructorParams="AmbiDataSet* pEditablePoints, AmbiDataSet* pDisplayOnlyPoints, PointSelection* pPointSelection, RadarOptions* pRadarOptions"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff505050"/>
  <GENERICCOMPONENT name="radar" id="bb1556089d26688f" memberName="radar" virtualName=""
                    explicitFocusOrder="0" pos="0 0 0M 100M" class="Radar3D" params="pEditablePoints, pDisplayOnlyPoints, pPointSelection, pRadarOptions"/>
  <GENERICCOMPONENT name="pointInfo" id="328b0557e3704175" memberName="pointInfo"
                    virtualName="" explicitFocusOrder="0" pos="0 100R 0M 100" class="PointInfoControl"
                    params="pEditablePoints, pPointSelection, pRadarOptions"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

