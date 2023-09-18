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



#pragma once

#include <JuceHeader.h>
#include "TableColumnCallback.h"

#define JS_EDITOR_COMPONENT_WIDTH 400
#define JS_EDITOR_ADDITIONAL_WIDTH  450
#define JS_EDITOR_COMPONENT_HEIGHT 400


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
    std::unique_ptr<juce::TextEditor> help;
    bool helpFlag;
    
    CodeDocument codeDocument;

private:
    AsyncUpdater* pCallback;
    String* pCode;
    bool* pCloseFlag;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JsEditorComponent)
};
