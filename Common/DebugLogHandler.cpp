#include "DebugLogHandler.h"
#include "DebugLogWindow.h"

DebugLogHandler::DebugLogHandler()
{
}

DebugLogHandler::~DebugLogHandler()
{
    cancelPendingUpdate();
}

void DebugLogHandler::logMessage(const juce::String& message, juce::Colour colour)
{
    const juce::ScopedLock sl(lock);
    
    LogEntry entry(message, colour);
    messageQueue.push_back(entry);
    addMessageToHistory(entry);
    
    // Only trigger update if window is open
    if (activeWindow != nullptr)
    {
        triggerAsyncUpdate();
    }
}

void DebugLogHandler::logWarning(const juce::String& message)
{
    logMessage("WARNING: " + message, juce::Colours::yellow);
}

void DebugLogHandler::logError(const juce::String& message)
{
    logMessage("ERROR: " + message, juce::Colours::red);
}

void DebugLogHandler::logSuccess(const juce::String& message)
{
    logMessage("SUCCESS: " + message, juce::Colours::green);
}

void DebugLogHandler::registerWindow(DebugLogWindow* window)
{
    const juce::ScopedLock sl(lock);
    activeWindow = window;
    
    // Clear queue when window opens to avoid flooding with old messages
    messageQueue.clear();
}

void DebugLogHandler::unregisterWindow(DebugLogWindow* window)
{
    const juce::ScopedLock sl(lock);
    if (activeWindow == window)
    {
        activeWindow = nullptr;
    }
}

void DebugLogHandler::clear()
{
    const juce::ScopedLock sl(lock);
    messageQueue.clear();
    messageHistory.clear();
}

juce::StringArray DebugLogHandler::getRecentMessages(int maxMessages) const
{
    const juce::ScopedLock sl(lock);
    juce::StringArray messages;
    
    int startIndex = juce::jmax(0, (int)messageHistory.size() - maxMessages);
    for (int i = startIndex; i < (int)messageHistory.size(); ++i)
    {
        messages.add(formatMessage(messageHistory[(size_t)i]));
    }
    
    return messages;
}

void DebugLogHandler::handleAsyncUpdate()
{
    const juce::ScopedLock sl(lock);
    
    if (activeWindow != nullptr && !messageQueue.empty())
    {
        juce::StringArray newMessages;
        std::vector<juce::Colour> colours;
        
        for (const auto& entry : messageQueue)
        {
            newMessages.add(formatMessage(entry));
            colours.push_back(entry.colour);
        }
        
        messageQueue.clear();
        
        // Call window update on message thread
        activeWindow->addMessages(newMessages, colours);
    }
}

void DebugLogHandler::addMessageToHistory(const LogEntry& entry)
{
    messageHistory.push_back(entry);
    
    // Limit history size
    if (messageHistory.size() > MAX_HISTORY)
    {
        messageHistory.erase(messageHistory.begin(), 
                           messageHistory.begin() + int(messageHistory.size() - MAX_HISTORY));
    }
}

juce::String DebugLogHandler::formatMessage(const LogEntry& entry) const
{
    return entry.timestamp.formatted("%H:%M:%S") + " - " + entry.message;
}
