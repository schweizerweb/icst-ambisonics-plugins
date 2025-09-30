#pragma once

#include "JuceHeader.h"
#include "TimelineComponent.h"

class AnimatorMainView : public juce::Component
{
public:
    AnimatorMainView();
    ~AnimatorMainView() override;

    void setTimelines(juce::OwnedArray<TimelineModel>* timelines);
    void setPlayheadPosition(ms_t timeMs);
    void setAutoFollow(bool shouldAutoFollow);
    void setPlayheadProvider(std::function<PlayheadSnapshot()> provider);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    // Menu bar
    class MainMenuBarModel : public juce::MenuBarModel
    {
    public:
        MainMenuBarModel(AnimatorMainView* owner) : owner(owner) {}
        
        juce::StringArray getMenuBarNames() override { return { "File", "Edit", "View", "Insert" }; }
        juce::PopupMenu getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName) override;
        void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;
        
    private:
        AnimatorMainView* owner;
    };

    // Toolbar component
    class ToolbarComponent : public juce::Component
    {
    public:
        ToolbarComponent(AnimatorMainView& owner);
        void paint(juce::Graphics& g) override;
        void drawButtonIcon(juce::Graphics& g, juce::Button* button, const juce::Path& icon, bool isToggled = false);
        void resized() override;
        std::unique_ptr<juce::TextButton> autoFollowButton;
        
    private:
        AnimatorMainView& owner;
        std::unique_ptr<juce::TextButton> addMovementButton;
        std::unique_ptr<juce::TextButton> addActionButton;
        std::unique_ptr<juce::TextButton> deleteButton;
        std::unique_ptr<juce::TextButton> zoomInButton;
        std::unique_ptr<juce::TextButton> zoomOutButton;
        std::unique_ptr<juce::TextButton> resetZoomButton;
        
        juce::Path createMovementIcon();
        juce::Path createActionIcon();
        juce::Path createDeleteIcon();
        juce::Path createZoomInIcon();
        juce::Path createZoomOutIcon();
        juce::Path createResetZoomIcon();
        juce::Path createAutoFollowIcon();
    };

    std::unique_ptr<juce::MenuBarComponent> menuBar;
    std::unique_ptr<MainMenuBarModel> menuBarModel;
    std::unique_ptr<ToolbarComponent> toolbar;
    std::unique_ptr<TimelineComponent> timelineComponent;

    // Application state
    juce::OwnedArray<TimelineModel>* timelines = nullptr;
    bool autoFollowEnabled = true;
    
    ClipboardData clipboard;

    // Zoom state
    float zoomLevel = 1.0f;
    static constexpr float MIN_ZOOM = 0.1f;
    static constexpr float MAX_ZOOM = 10.0f;
    static constexpr float ZOOM_STEP = 1.2f;
    static constexpr float DEFAULT_ZOOM = 1.0f;

    void handleMenuAction(int menuItemID);
    void addMovementClip();
    void addActionClip();
    void deleteSelectedClips();
    void zoomIn();
    void zoomOut();
    void resetZoom();
    void toggleAutoFollow();
    void updateTimelineZoom();
    
    // Import/Export
    void importScene(int timelineIndex);
    void exportScene(int timelineIndex);
    
    // Edit operations
    void cutSelectedClips();
    void copySelectedClips();
    void pasteClips();
    void selectAllClips();
    void deselectAllClips();

private:
private:
    // Helper method to generate unique clip IDs
    juce::String generateUniqueClipId(const juce::Array<MovementClip>& existingClips, const juce::String& baseId)
    {
        juce::String newId = baseId;
        int counter = 1;
        
        // Check if the ID already exists
        bool idExists;
        do {
            idExists = false;
            for (const auto& clip : existingClips)
            {
                if (clip.id == newId)
                {
                    idExists = true;
                    newId = baseId + " (" + juce::String(counter++) + ")";
                    break;
                }
            }
        } while (idExists);
        
        return newId;
    }
    
    juce::String generateUniqueClipId(const juce::Array<ActionClip>& existingClips, const juce::String& baseId)
    {
        juce::String newId = baseId;
        int counter = 1;
        
        // Check if the ID already exists
        bool idExists;
        do {
            idExists = false;
            for (const auto& clip : existingClips)
            {
                if (clip.id == newId)
                {
                    idExists = true;
                    newId = baseId + " (" + juce::String(counter++) + ")";
                    break;
                }
            }
        } while (idExists);
        
        return newId;
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnimatorMainView)
};
