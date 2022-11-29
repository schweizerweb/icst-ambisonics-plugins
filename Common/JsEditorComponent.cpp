/*
  ==============================================================================

    JsEditorComponent.cpp
    Created: 5 Jul 2022 7:34:00pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#include <JuceHeader.h>
#include "JsEditorComponent.h"

//==============================================================================
JsEditorComponent::JsEditorComponent(String* pCode, bool* pCloseFlag, AsyncUpdater* pCallback)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    this->pCallback = pCallback;
    this->pCode = pCode;
    this->pCloseFlag = pCloseFlag;
    
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
    
    std::vector<String> lines;
    lines.push_back("Custom OSC - JS Help:");
    lines.push_back("");
    lines.push_back("Title (optional):");
    lines.push_back("  If the first line starts with a comment (//), this comment");
    lines.push_back("  is used as title and will be shown in the Custom OSC list.");
    lines.push_back("");
    lines.push_back("Access to OSC data:");
    lines.push_back("  s.path([index]);");
    lines.push_back("    Returns the specified part of the OSC path.");
    lines.push_back("    Example: /this/is/my/path -> s.path(3); -> 'my'");
    lines.push_back("  s.arg([index]);");
    lines.push_back("    Returns the specified argument of the OSC message.");
    lines.push_back("    Example: /demo 1 2 3 -> s.arg(2); -> 2");
    lines.push_back("");
    lines.push_back("Access to current data:");
    lines.push_back("  s.getX([index]); (*)");
    lines.push_back("  s.getY([index]); (*)");
    lines.push_back("  s.getZ([index]); (*)");
    lines.push_back("  s.getA([index]); (*)");
    lines.push_back("  s.getE([index]); (*)");
    lines.push_back("  s.getD([index]); (*)");
    lines.push_back("  s.getName([index]);");
    lines.push_back("  s.getAbsX([index]); (group mode only)");
    lines.push_back("  s.getAbsY([index]); (group mode only)");
    lines.push_back("  s.getAbsZ([index]); (group mode only)");
    lines.push_back("  s.getGroupX([index]);");
    lines.push_back("  s.getGroupY([index]);");
    lines.push_back("  s.getGroupZ([index]);");
    lines.push_back("  s.getGroupA([index]);");
    lines.push_back("  s.getGroupE([index]);");
    lines.push_back("  s.getGroupD([index]);");
    lines.push_back("(*) Note: in group mode, positions are relative to the attached group!");
    lines.push_back("");
    lines.push_back("Manipulation of data set:");
    lines.push_back("  s.setXYZ([index], [x], [y], [z]);");
    lines.push_back("    Sets XYZ coordinates of the specified source index.");
    lines.push_back("    Example: s.setXYZ(1, 1.0, 0.5, 0.1);");
    lines.push_back("  s.setXYZbyName([name], [x], [y], [z]);");
    lines.push_back("    Sets XYZ coordinates of the source with the specified name.");
    lines.push_back("    Example: s.setXYZbyName(\"flute\", 0.5, 0.5, 0);");
    lines.push_back("  s.setAED([index], [x], [y], [z]);");
    lines.push_back("  s.setAEDbyName([name], [x], [y], [z]);");
    lines.push_back("    Same for AED coordinates.");
    lines.push_back("");
    lines.push_back("Group manipulations:");
    lines.push_back("  s.setGroupXYZ([index], [x], [y], [z], [optional: moveSub]);");
    lines.push_back("    Sets XYZ coordinates of the specified group index.");
    lines.push_back("    Example: s.setGroupXYZ(1, 1.0, 0.5, 0.1, 1);");
    lines.push_back("  s.setXYZbyName([name], [x], [y], [z], [optional: moveSub]);");
    lines.push_back("    Sets XYZ coordinates of the source with the specified name.");
    lines.push_back("    Example: s.setXYZbyName(\"woodwinds\", 0.5, 0.5, 0, 0);");
    lines.push_back("  s.setGroupAED([index], [x], [y], [z], [optional: moveSub]);");
    lines.push_back("  s.setGroupAEDbyName([name], [x], [y], [z], [optional: moveSub]);");
    lines.push_back("    Same for AED coordinates.");
    lines.push_back("  s.rotateGroup([index], [x], [y], [z]);");
    lines.push_back("    Rotates the attached sources around the group point.");
    lines.push_back("    x, y, z define the relative axis-rotation in degrees.");
    lines.push_back("    Example: s.rotateGroup(1, 0.0, 0.0, 10.0);");
    lines.push_back("  s.rotateAroundOrigin([index], [x], [y], [z], [optional: moveSub]);");
    lines.push_back("    Rotates the group point around the origin (0,0,0).");
    lines.push_back("    x, y, z define the relative axis-rotation in degrees.");
    lines.push_back("    Example: s.rotateGroupAroundOrigin(1, 0.0, 0.0, 10.0);");
    lines.push_back("  s.rotateGroupByName([name], [x], [y], [z]);");
    lines.push_back("  s.rotateAroundOriginByName([name], [x], [y], [z], [optional: moveSub]);");
    lines.push_back("    Same with identification of the group by name.");
    lines.push_back("");
    lines.push_back("  The optional parameter moveSub defines:");
    lines.push_back("    0: group point only");
    lines.push_back("    1: group point and attached sources, preserving relative positions.");
    lines.push_back("    (default is 1)");
    lines.push_back("");
    lines.push_back("Additional methods for group mode (absolute rotation and stretch):");
    lines.push_back("  s.setGroupRotation([index], [x], [y], [z], [w]);");
    lines.push_back("    Sets the absolute rotation of the group to the specified quaternion.");
    lines.push_back("  s.setGroupStretch([index], [stretchFactor]);");
    lines.push_back("    Sets the absolute stretch factor.");
    lines.push_back("  s.setGroupRotationByName([name], [x], [y], [z], [w]);");
    lines.push_back("  s.setGroupStretchByName([name], [stretchFactor]);");
    lines.push_back("    Same with identification of the group by name.");
    lines.push_back("");
    lines.push_back("Local buffer:");
    lines.push_back("  The local buffer allows to store values between OSC messages.");
    lines.push_back("  Up to 1000 float values can be stored and recalled anytime.");
    lines.push_back("  s.setBufferValue([index], [value]);");
    lines.push_back("    Saves the specified value at buffer position [index].");
    lines.push_back("    Example: s.setBufferValue(1, 25.0);");
    lines.push_back("  s.getBufferValue([index]);");
    lines.push_back("    Gets the value stored at buffer position [index].");
    lines.push_back("    Example: s.getBufferValue(1);");
    lines.push_back("");
    lines.push_back("Error handling:");
    lines.push_back("  Java Script syntax errors will be displayed at interpretation time. ");
    lines.push_back("  Errors in the methods defined above, will be automatically displayed,");
    lines.push_back("  however, it's possible to handle these errors in the Java Script code:");
    lines.push_back("    All getter-methods return the value if available, 'undefined' otherwise.");
    lines.push_back("    All setter-methods return true if successful, 'undefined' otherwise.");
    lines.push_back("    s.reportError([message]);");
    lines.push_back("      Allows custom error reporting.");
    lines.push_back("      Example: if(s.setXYZ(i, x, y, z) != true) s.reportError(\"beep\");");
    lines.push_back("    s.reportError(\"\")");
    lines.push_back("      Resets the automatically generated error message to ignore the error.");
    lines.push_back("");
    lines.push_back("Note: all 'index' parameters are 1-based, except for the 'Local Buffer' methods");
    String helpText;
    
    for(auto l : lines)
    {
        helpText.append(l, l.length());
        helpText.append(NewLine::getDefault(), strlen(NewLine::getDefault()));
    }
    
    help.reset(new TextEditor("help"));
    addAndMakeVisible(help.get());
    help->setScrollbarsShown(true);
    help->setMultiLine(true);
    help->setText(helpText, dontSendNotification);
    help->setReadOnly(true);
    //help->setJustificationType(Justification::topLeft);
    help->setFont(Font(Font::getDefaultMonospacedFontName(), 12.0f, 0));
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
