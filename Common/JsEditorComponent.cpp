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
    
    setSize(400, 300);
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

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("JsEditorComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void JsEditorComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    label->setBounds(0, 0, getWidth(), 20);
    codeEditor->setBounds(0, 20, getWidth(), getHeight() - 50);
    btnSave->setBounds(0, getHeight()-25, getWidth(), 25);
}

void JsEditorComponent::buttonClicked(juce::Button *)
{
    *pCloseFlag = false;
    *pCode = codeDocument.getAllContent();
    pCallback->triggerAsyncUpdate();
}
