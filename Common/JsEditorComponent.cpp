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



#include <JuceHeader.h>
#include "JsEditorComponent.h"

static String helpText =
R"(Custom OSC - JS Help:

Title (optional):");
  If the first line starts with a comment (//), this comment
  is used as title and will be shown in the Custom OSC list.

Access to OSC data:
  s.path([index]);
    Returns the specified part of the OSC path.
    Example: /this/is/my/path -> s.path(3); -> 'my'
  s.arg([index]);
    Returns the specified argument of the OSC message.
    Example: /demo 1 2 3 -> s.arg(2); -> 2

Access to current data:
  s.getX([index]); (*)
  s.getY([index]); (*)
  s.getZ([index]); (*)
  s.getA([index]); (*)
  s.getE([index]); (*)
  s.getD([index]); (*)
  s.getName([index]);
  s.getAbsX([index]); (group mode only)
  s.getAbsY([index]); (group mode only)
  s.getAbsZ([index]); (group mode only)
  s.getGroupX([index]);
  s.getGroupY([index]);
  s.getGroupZ([index]);
  s.getGroupA([index]);
  s.getGroupE([index]);
  s.getGroupD([index]);
(*) Note: in group mode, positions are relative to the attached group!

Manipulation of data set:
  s.setXYZ([index], [x], [y], [z]);
    Sets XYZ coordinates of the specified source index.
    Example: s.setXYZ(1, 1.0, 0.5, 0.1);
  s.setXYZbyName([name], [x], [y], [z]);
    Sets XYZ coordinates of the source with the specified name.
    Example: s.setXYZbyName(\"flute\", 0.5, 0.5, 0);
  s.setAED([index], [x], [y], [z]);
  s.setAEDbyName([name], [x], [y], [z]);
    Same for AED coordinates.

Group manipulations:
  s.setGroupXYZ([index], [x], [y], [z], [optional: moveSub]);
    Sets XYZ coordinates of the specified group index.
    Example: s.setGroupXYZ(1, 1.0, 0.5, 0.1, 1);
  s.setXYZbyName([name], [x], [y], [z], [optional: moveSub]);
    Sets XYZ coordinates of the source with the specified name.
    Example: s.setXYZbyName(\"woodwinds\", 0.5, 0.5, 0, 0);
  s.setGroupAED([index], [x], [y], [z], [optional: moveSub]);
  s.setGroupAEDbyName([name], [x], [y], [z], [optional: moveSub]);
    Same for AED coordinates.
  s.rotateGroup([index], [x], [y], [z]);
    Rotates the attached sources around the group point.
    x, y, z define the relative axis-rotation in degrees.
    Example: s.rotateGroup(1, 0.0, 0.0, 10.0);
  s.rotateAroundOrigin([index], [x], [y], [z], [optional: moveSub]);
    Rotates the group point around the origin (0,0,0).
    x, y, z define the relative axis-rotation in degrees.
    Example: s.rotateGroupAroundOrigin(1, 0.0, 0.0, 10.0);
  s.rotateGroupByName([name], [x], [y], [z]);
  s.rotateAroundOriginByName([name], [x], [y], [z], [optional: moveSub]);
    Same with identification of the group by name.

  The optional parameter moveSub defines:
    0: group point only
    1: group point and attached sources, preserving relative positions.
    (default is 1)

Additional methods for group mode (absolute rotation and stretch):
  s.setGroupRotation([index], [x], [y], [z], [w]);
    Sets the absolute rotation of the group to the specified quaternion.
  s.setGroupRotationEuler([index], [x], [y], [z]);
    Sets the absolute rotation of the group to the specified axis rotations.
  s.setGroupStretch([index], [stretchFactor]);
    Sets the absolute stretch factor.
  s.setGroupRotationByName([name], [x], [y], [z], [w]);
  s.setGroupRotationEulerByName([name], [x], [y], [z], [w]);
  s.setGroupStretchByName([name], [stretchFactor]);
    Same with identification of the group by name.

Local buffer:
  The local buffer allows to store values between OSC messages.
  Up to 1000 float values can be stored and recalled anytime.
  s.setBufferValue([index], [value]);
    Saves the specified value at buffer position [index].
    Example: s.setBufferValue(1, 25.0);
  s.getBufferValue([index]);
    Gets the value stored at buffer position [index].
    Example: s.getBufferValue(1);

Error handling:
  Java Script syntax errors will be displayed at interpretation time.
  Errors in the methods defined above, will be automatically displayed,
  however, it's possible to handle these errors in the Java Script code:
    All getter-methods return the value if available, 'undefined' otherwise.
    All setter-methods return true if successful, 'undefined' otherwise.
    s.reportError([message]);
      Allows custom error reporting.
      Example: if(s.setXYZ(i, x, y, z) != true) s.reportError("beep");
    s.reportError("");
      Resets the automatically generated error message to ignore the error.

Note: all 'index' parameters are 1-based, except for the 'Local Buffer' methods)";

//==============================================================================
JsEditorComponent::JsEditorComponent(String* _pCode, bool* _pCloseFlag, AsyncUpdater* _pCallback) : pCallback(_pCallback), pCode(_pCode), pCloseFlag(_pCloseFlag)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    codeDocument.replaceAllContent(*pCode);
    
    helpFlag = false;
    
    label.reset(new Label("jsCode"));
    addAndMakeVisible(label.get());
    label->setText("JavaScript code:", dontSendNotification);
    
    codeTokenizer.reset(new CPlusPlusCodeTokeniser());
    codeEditor.reset(new CodeEditorComponent(codeDocument, codeTokenizer.get()));
    codeEditor->setColour(CodeEditorComponent::backgroundColourId, Colours::lightgrey);
    addAndMakeVisible (codeEditor.get());
    
    btnSave.reset(new TextButton("btnSave"));
    addAndMakeVisible(btnSave.get());
    btnSave->setButtonText(TRANS("Save"));
    btnSave->addListener(this);
    
    btnHelp.reset(new TextButton("btnHelp"));
    addAndMakeVisible(btnHelp.get());
    btnHelp->setButtonText("JS Help");
    btnHelp->addListener(this);
    
    help.reset(new TextEditor("help"));
    addAndMakeVisible(help.get());
    help->setScrollbarsShown(true);
    help->setMultiLine(true);
    help->setText(helpText, dontSendNotification);
    help->setReadOnly(true);
    //help->setJustificationType(Justification::topLeft);
    help->setFont(Font(FontOptions(Font::getDefaultMonospacedFontName(), 12.0f, 0)));
    setSize(JS_EDITOR_COMPONENT_WIDTH, JS_EDITOR_COMPONENT_HEIGHT);
}

JsEditorComponent::~JsEditorComponent()
{
    *pCode = codeDocument.getAllContent();
    *pCloseFlag = true;
    pCallback->triggerAsyncUpdate();
    
    codeEditor = nullptr;
}

void JsEditorComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    //g.setColour (juce::Colours::white);
    //g.setFont (14.0f);
    //g.drawText ("JsEditorComponent", getLocalBounds(),
    //            juce::Justification::centred, true);   // draw some placeholder text
}

void JsEditorComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    label->setBounds(0, 0, JS_EDITOR_COMPONENT_WIDTH - 60, 25);
    btnHelp->setBounds(JS_EDITOR_COMPONENT_WIDTH - 60, 0, 60, 25);
    btnHelp->setColour(TextButton::buttonColourId, helpFlag ? Colours::darkred : Colours::darkgreen);
    codeEditor->setBounds(0, 27, JS_EDITOR_COMPONENT_WIDTH, JS_EDITOR_COMPONENT_HEIGHT - 54);
    btnSave->setBounds(0, JS_EDITOR_COMPONENT_HEIGHT-25, JS_EDITOR_COMPONENT_WIDTH, 25);
    if(helpFlag)
        help->setBounds(JS_EDITOR_COMPONENT_WIDTH, 0, JS_EDITOR_ADDITIONAL_WIDTH, JS_EDITOR_COMPONENT_HEIGHT);
}

void JsEditorComponent::buttonClicked(juce::Button* b)
{
    if(b == btnSave.get())
    {
        *pCloseFlag = false;
        *pCode = codeDocument.getAllContent();
        pCallback->triggerAsyncUpdate();
    }
    else if(b == btnHelp.get())
    {
        helpFlag = !helpFlag;
        setSize(JS_EDITOR_COMPONENT_WIDTH + (helpFlag ? JS_EDITOR_ADDITIONAL_WIDTH : 0), JS_EDITOR_COMPONENT_HEIGHT);
    }
}
