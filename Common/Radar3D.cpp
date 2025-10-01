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

#include "Radar3D.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
Radar3D::Radar3D (AmbiDataSet* pEditablePoints, AmbiDataSet* pDisplayOnlyPoints, PointSelection* pPointSelection, RadarOptions* pRadarOptions)
{
    //[Constructor_pre] You can add your own custom stuff here..
	fullRadarFlag = false;
    //[/Constructor_pre]

    xzRadar.reset (new Radar2D (Radar2D::XZ_Half, pEditablePoints, pDisplayOnlyPoints, pPointSelection, pRadarOptions));
    addAndMakeVisible (xzRadar.get());
    xzRadar->setName ("xzRadar");

    xzRadar->setBounds (0, 240, 336, 120);

    xyRadar.reset (new Radar2D (Radar2D::XY, pEditablePoints, pDisplayOnlyPoints, pPointSelection, pRadarOptions));
    addAndMakeVisible (xyRadar.get());
    xyRadar->setName ("xyRadar");

    xyRadar->setBounds (0, 0, 336, 240);

    btnFull.reset (new juce::ImageButton ("btnFull"));
    addAndMakeVisible (btnFull.get());
    btnFull->setButtonText (TRANS("Full"));
    btnFull->addListener (this);

    btnFull->setImages (false, true, false,
                        juce::ImageCache::getFromMemory (BinaryData::flat_arrow_down_png, BinaryData::flat_arrow_down_pngSize), 1.000f, juce::Colour (0x00000000),
                        juce::Image(), 1.000f, juce::Colour (0xffdd6060),
                        juce::ImageCache::getFromMemory (BinaryData::flat_arrow_down_png, BinaryData::flat_arrow_down_pngSize), 1.000f, juce::Colour (0x00000000));
    btnFull->setBounds (0, 360, 336, 6);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    xyRadar->addMouseListener(this, true);
    xzRadar->addMouseListener(this, true);
    //[/Constructor]
}

Radar3D::~Radar3D()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    xyRadar->removeMouseListener(this);
    xzRadar->removeMouseListener(this);
    //[/Destructor_pre]

    xzRadar = nullptr;
    xyRadar = nullptr;
    btnFull = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Radar3D::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff505050));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Radar3D::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    if(getWidth() > getHeight())
    {
        xzRadar->setRadarMode(Radar2D::XZ_Full);
        xyRadar->setAnchor(Radar2D::X_Right, Radar2D::Y_Center);
        xzRadar->setAnchor(Radar2D::X_Left, Radar2D::Y_Center);
        xyRadar->setBounds(0, 0, getWidth()/2, getHeight());
        xzRadar->setBounds(getWidth()/2, 0, getWidth()/2, getHeight());
        btnFull->setVisible(false);
    }
    else
    {
        xzRadar->setRadarMode(fullRadarFlag ? Radar2D::XZ_Full : Radar2D::XZ_Half);
        int secondRadarHeight = int((getHeight() - 6) / (fullRadarFlag ? 2.0 : 3.0));
        int topRadarHeight = fullRadarFlag ? secondRadarHeight : secondRadarHeight * 2;
        xyRadar->setAnchor(Radar2D::X_Center, Radar2D::Y_Bottom);
        xzRadar->setAnchor(Radar2D::X_Center, Radar2D::Y_Top);
        xyRadar->setBounds(0, 0, getWidth(), topRadarHeight);
        xzRadar->setBounds(0, topRadarHeight, getWidth(), secondRadarHeight);
        btnFull->setBounds(0, topRadarHeight + secondRadarHeight, getWidth(), getHeight() - topRadarHeight - secondRadarHeight);
        btnFull->setVisible(true);
    }
    //[/UserResized]
}

void Radar3D::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnFull.get())
    {
        //[UserButtonCode_btnFull] -- add your button handler code here..
        setRadarMode(!fullRadarFlag);
        btnFull->setImages (false, true, false,
            ImageCache::getFromMemory (fullRadarFlag ? BinaryData::flat_arrow_up_png : BinaryData::flat_arrow_down_png, fullRadarFlag ? BinaryData::flat_arrow_up_pngSize : BinaryData::flat_arrow_down_pngSize), 1.000f, Colour (0x00000000),
            Image(), 1.000f, Colour (0xffdd6060),
            ImageCache::getFromMemory (fullRadarFlag ? BinaryData::flat_arrow_up_png : BinaryData::flat_arrow_down_png, fullRadarFlag ? BinaryData::flat_arrow_up_pngSize : BinaryData::flat_arrow_down_pngSize), 1.000f, Colour (0x00000000));
        //[/UserButtonCode_btnFull]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void Radar3D::mouseEnter (const juce::MouseEvent& /*e*/)
{
    //[UserCode_mouseEnter] -- Add your code here...
    xyRadar->setRefreshRate(ACTIVE_REFRESH_RATE);
    xzRadar->setRefreshRate(ACTIVE_REFRESH_RATE);
    //[/UserCode_mouseEnter]
}

void Radar3D::mouseExit (const juce::MouseEvent& /*e*/)
{
    //[UserCode_mouseExit] -- Add your code here...
    xyRadar->setRefreshRate(INACTIVE_REFRESH_RATE);
    xzRadar->setRefreshRate(INACTIVE_REFRESH_RATE);
    //[/UserCode_mouseExit]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void Radar3D::setRadarMode(bool fullRadar)
{
	xzRadar->setRadarMode(fullRadar ? Radar2D::XZ_Full : Radar2D::XZ_Half);
	fullRadarFlag = fullRadar;
	resized();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="Radar3D" componentName=""
                 parentClasses="public Component" constructorParams="AmbiDataSet* pEditablePoints, AmbiDataSet* pDisplayOnlyPoints, PointSelection* pPointSelection, RadarOptions* pRadarOptions"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <METHODS>
    <METHOD name="mouseEnter (const juce::MouseEvent&amp; e)"/>
    <METHOD name="mouseExit (const juce::MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff505050"/>
  <GENERICCOMPONENT name="xzRadar" id="9b35aa2c2da622df" memberName="xzRadar" virtualName=""
                    explicitFocusOrder="0" pos="0 240 336 120" class="Radar2D" params="Radar2D::XZ_Half, pEditablePoints, pDisplayOnlyPoints, pPointSelection, pRadarOptions"/>
  <GENERICCOMPONENT name="xyRadar" id="952154a5b6ffaa65" memberName="xyRadar" virtualName=""
                    explicitFocusOrder="0" pos="0 0 336 240" class="Radar2D" params="Radar2D::XY, pEditablePoints, pDisplayOnlyPoints, pPointSelection, pRadarOptions"/>
  <IMAGEBUTTON name="btnFull" id="7f9ff0bdbe393e2c" memberName="btnFull" virtualName=""
               explicitFocusOrder="0" pos="0 360 336 6" buttonText="Full" connectedEdges="0"
               needsCallback="1" radioGroupId="0" keepProportions="0" resourceNormal="flatArrowDownT_png"
               opacityNormal="1.0" colourNormal="0" resourceOver="" opacityOver="1.0"
               colourOver="ffdd6060" resourceDown="flatArrowDownT_png" opacityDown="1.0"
               colourDown="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//[EndFile] You can add extra defines here...
//[/EndFile]
