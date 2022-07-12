/*
  ==============================================================================

    JsEditorComponent.h
    Created: 5 Jul 2022 7:34:00pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TableColumnCallback.h"

#define JS_EDITOR_COMPONENT_WIDTH 400
#define JS_EDITOR_ADDITIONAL_WIDTH  380
#define JS_EDITOR_COMPONENT_HEIGHT 400
//==============================================================================
/*
*/
class JsEditorComponent  : public juce::Component, TextButton::Listener
{
public:
    JsEditorComponent(String* pCode, bool* pCloseFlag, AsyncUpdater* pCallback);
    ~JsEditorComponent() override;
    
    void buttonClicked(juce::Button *) override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    std::unique_ptr<juce::Label> label;
    std::unique_ptr<juce::CodeEditorComponent> codeEditor;
    std::unique_ptr<juce::CodeTokeniser> codeTokenizer;
    std::unique_ptr<juce::TextButton> btnSave;
    std::unique_ptr<juce::TextButton> btnHelp;
    std::unique_ptr<juce::Label> help;
    bool helpFlag;
    
    CodeDocument codeDocument;

private:
    AsyncUpdater* pCallback;
    String* pCode;
    bool* pCloseFlag;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JsEditorComponent)
};
