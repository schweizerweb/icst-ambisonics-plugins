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
    btnHelp->setButtonText("?");
    btnHelp->addListener(this);
    
    std::vector<String> lines;
    lines.push_back("Custom OSC Help:");
    lines.push_back("");
    lines.push_back("Access to OSC data:");
    lines.push_back("  s.path([index]);");
    lines.push_back("    Returns the specified part of the OSC path.");
    lines.push_back("    Example: /this/is/my/path -> s.path(3); -> 'my'");
    lines.push_back("  s.arg([index]);");
    lines.push_back("    Returns the specified argument of the OSC message.");
    lines.push_back("    Example: /demo 1 2 3 -> s.arg(2); -> 2");
    lines.push_back("");
    lines.push_back("Manipulation of data set:");
    lines.push_back("  s.setXYZ([index], [x], [y], [z]);");
    lines.push_back("    Sets XYZ coordinates of the specified source index.");
    lines.push_back("    Example: s.set(1, 1.0, 0.5, 0.1);");
    lines.push_back("  s.setXYZbyName([name], [x], [y], [z]);");
    lines.push_back("    Sets XYZ coordinates of the source with the sp. name.");
    lines.push_back("    Example: s.set(\"flute\", 0.5, 0.5, 0);");
    lines.push_back("  s.setAED([index], [x], [y], [z]);");
    lines.push_back("  s.setAEDbyName([name], [x], [y], [z]);");
    lines.push_back("    Same for AED coordinates.");
    lines.push_back("");
    lines.push_back("Access to current data:");
    lines.push_back("  s.x([index]);");
    lines.push_back("  s.y([index]);");
    lines.push_back("  s.z([index]);");
    lines.push_back("  s.a([index]);");
    lines.push_back("  s.e([index]);");
    lines.push_back("  s.d([index]);");
    lines.push_back("  s.name([index]);");
    lines.push_back("");
    lines.push_back("");
    lines.push_back("Note: all 'index' parameters are 1-based");
    String helpText;
    
    for(auto l : lines)
    {
        helpText.append(l, l.length());
        helpText.append(NewLine::getDefault(), strlen(NewLine::getDefault()));
    }
    
    help.reset(new Label("help"));
    addAndMakeVisible(help.get());
    help->setText(helpText, dontSendNotification);
    help->setJustificationType(Justification::topLeft);
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

    label->setBounds(0, 0, JS_EDITOR_COMPONENT_WIDTH - 20, 20);
    btnHelp->setBounds(JS_EDITOR_COMPONENT_WIDTH - 20, 0, 20, 20);
    codeEditor->setBounds(0, 20, JS_EDITOR_COMPONENT_WIDTH, JS_EDITOR_COMPONENT_HEIGHT - 50);
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
