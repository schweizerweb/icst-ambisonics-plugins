#include "DebugLogWindow.h"

DebugLogWindow::DebugLogWindow(DebugLogHandler& handler)
    : DocumentWindow("Debug Log",
                    juce::Colours::white,
                    DocumentWindow::allButtons),
      logHandler(handler)
{
    setUsingNativeTitleBar(true);
    setResizable(true, true);
    
    // Set up the text editor
    textEditor.setMultiLine(true);
    textEditor.setReadOnly(true);
    textEditor.setScrollbarsShown(true);
    textEditor.setCaretVisible(false);
    textEditor.setFont(juce::FontOptions(14.0f));
    
    // Make the text editor the content
    setContentNonOwned(&textEditor, false);
    
    setSize(600, 400);
    setVisible(true);
    
    // Load existing messages
    auto messages = logHandler.getRecentMessages();
    for (const auto& message : messages)
    {
        if (textEditor.getText().isNotEmpty())
            textEditor.insertTextAtCaret("\n");
        textEditor.insertTextAtCaret(message);
    }
    
    // Scroll to bottom
    textEditor.setCaretPosition(textEditor.getText().length());
    
    logHandler.registerWindow(this);
}

DebugLogWindow::~DebugLogWindow()
{
    logHandler.unregisterWindow(this);
}

void DebugLogWindow::closeButtonPressed()
{
    setVisible(false);
}

void DebugLogWindow::addMessages(const juce::StringArray& messages, const std::vector<juce::Colour>& /*colours*/)
{
    for (const auto& message : messages)
    {
        if (textEditor.getText().isNotEmpty())
            textEditor.insertTextAtCaret("\n");
        textEditor.insertTextAtCaret(message);
    }
    
    textEditor.setCaretPosition(textEditor.getText().length());
}
