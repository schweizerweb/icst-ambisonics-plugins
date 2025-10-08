#pragma once

#include <JuceHeader.h>

class DebugLogWindow;

class DebugLogHandler : public juce::AsyncUpdater
{
public:
    DebugLogHandler();
    ~DebugLogHandler() override;
    
    // Log message with different levels
    void logMessage(const juce::String& message, juce::Colour colour = juce::Colours::white);
    void logWarning(const juce::String& message);
    void logError(const juce::String& message);
    void logSuccess(const juce::String& message);
    
    // Window management
    void registerWindow(DebugLogWindow* window);
    void unregisterWindow(DebugLogWindow* window);
    
    // Clear all messages
    void clear();
    
    // Get recent messages (for when window opens)
    juce::StringArray getRecentMessages(int maxMessages = 1000) const;
    
    // AsyncUpdater override
    void handleAsyncUpdate() override;

private:
    struct LogEntry
    {
        juce::String message;
        juce::Colour colour;
        juce::Time timestamp;
        
        LogEntry(const juce::String& msg, juce::Colour c)
            : message(msg), colour(c), timestamp(juce::Time::getCurrentTime()) {}
    };
    
    juce::CriticalSection lock;
    std::vector<LogEntry> messageQueue;
    std::vector<LogEntry> messageHistory;
    DebugLogWindow* activeWindow = nullptr;
    
    static constexpr size_t MAX_HISTORY = 2000;
    
    void addMessageToHistory(const LogEntry& entry);
    juce::String formatMessage(const LogEntry& entry) const;
};
