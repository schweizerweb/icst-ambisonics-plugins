/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.1.6

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
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
                        juce::ImageCache::getFromMemory (flatArrowDownT_png, flatArrowDownT_pngSize), 1.000f, juce::Colour (0x00000000),
                        juce::Image(), 1.000f, juce::Colour (0xffdd6060),
                        juce::ImageCache::getFromMemory (flatArrowDownT_png, flatArrowDownT_pngSize), 1.000f, juce::Colour (0x00000000));
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
            ImageCache::getFromMemory (fullRadarFlag ? flatArrowUpT_png : flatArrowDownT_png, fullRadarFlag ? flatArrowUpT_pngSize : flatArrowDownT_pngSize), 1.000f, Colour (0x00000000),
            Image(), 1.000f, Colour (0xffdd6060),
            ImageCache::getFromMemory (fullRadarFlag ? flatArrowUpT_png : flatArrowDownT_png, fullRadarFlag ? flatArrowUpT_pngSize : flatArrowDownT_pngSize), 1.000f, Colour (0x00000000));
        //[/UserButtonCode_btnFull]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void Radar3D::mouseEnter (const juce::MouseEvent& e)
{
    //[UserCode_mouseEnter] -- Add your code here...
    xyRadar->setRefreshRate(ACTIVE_REFRESH_RATE);
    xzRadar->setRefreshRate(ACTIVE_REFRESH_RATE);
    //[/UserCode_mouseEnter]
}

void Radar3D::mouseExit (const juce::MouseEvent& e)
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

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: flatArrowDownT_png, 295, "../Resources/FlatArrowDownT.png"
static const unsigned char resource_Radar3D_flatArrowDownT_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,253,0,0,0,10,8,6,0,0,0,127,162,88,93,0,0,0,1,115,82,71,66,0,174,206,28,233,0,0,
0,4,103,65,77,65,0,0,177,143,11,252,97,5,0,0,0,9,112,72,89,115,0,0,14,195,0,0,14,195,1,199,111,168,100,0,0,0,188,73,68,65,84,104,67,237,214,205,13,130,64,16,64,225,209,131,5,24,18,45,131,14,60,122,177,
6,122,208,66,164,7,107,160,1,59,216,50,52,49,22,64,72,20,216,181,2,227,238,204,248,190,132,159,61,66,120,195,46,4,101,93,110,231,241,124,140,139,191,210,74,179,61,165,123,100,68,244,10,212,221,227,21,
158,67,90,249,183,219,172,228,186,95,243,237,21,178,76,87,20,20,14,213,20,64,155,14,151,166,208,71,243,51,18,124,89,188,124,109,226,118,255,195,244,182,127,254,163,223,251,56,200,216,202,171,65,244,154,
25,28,0,132,174,31,209,91,161,120,0,16,186,45,68,111,145,130,1,64,232,118,17,189,117,25,7,0,161,251,64,244,158,252,96,0,16,186,63,68,239,213,23,3,128,208,61,19,121,3,8,232,64,240,202,144,92,246,0,0,0,
0,73,69,78,68,174,66,96,130,0,0};

const char* Radar3D::flatArrowDownT_png = (const char*) resource_Radar3D_flatArrowDownT_png;
const int Radar3D::flatArrowDownT_pngSize = 295;

// JUCER_RESOURCE: flatArrowUpT_png, 291, "../Resources/FlatArrowUpT.png"
static const unsigned char resource_Radar3D_flatArrowUpT_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,253,0,0,0,10,8,6,0,0,0,127,162,88,93,0,0,0,1,115,82,71,66,0,174,206,28,233,0,0,0,
4,103,65,77,65,0,0,177,143,11,252,97,5,0,0,0,9,112,72,89,115,0,0,14,195,0,0,14,195,1,199,111,168,100,0,0,0,184,73,68,65,84,104,67,237,212,49,10,2,49,16,70,225,201,10,162,157,157,178,87,19,188,195,122,
16,189,131,173,247,18,177,178,83,4,55,186,193,194,102,64,68,204,204,240,190,38,36,117,222,159,4,241,237,142,155,225,152,141,83,119,190,229,242,244,174,121,254,130,62,203,182,92,86,139,117,57,17,22,209,
71,245,10,189,157,54,221,225,210,151,167,79,76,70,73,174,247,204,0,4,70,244,145,124,25,186,134,1,136,137,232,189,251,113,232,26,6,32,14,162,247,232,79,161,107,24,0,223,136,222,139,202,161,107,24,0,127,
136,222,50,163,161,107,24,0,31,136,222,26,103,161,107,24,0,187,136,222,130,32,161,107,24,0,91,136,190,178,118,127,202,17,67,215,148,1,88,206,249,119,213,136,60,0,234,110,96,11,202,105,211,212,0,0,0,0,
73,69,78,68,174,66,96,130,0,0};

const char* Radar3D::flatArrowUpT_png = (const char*) resource_Radar3D_flatArrowUpT_png;
const int Radar3D::flatArrowUpT_pngSize = 291;


//[EndFile] You can add extra defines here...
//[/EndFile]

