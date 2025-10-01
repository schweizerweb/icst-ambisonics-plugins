#pragma once

#include "JuceHeader.h"
#include "TimelineViewport.h"

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
        
    private:
        AnimatorMainView& owner;
        std::unique_ptr<juce::DrawableButton> addMovementButton, addActionButton, deleteButton;
        std::unique_ptr<juce::DrawableButton> zoomInButton, zoomOutButton, resetZoomButton;
        std::unique_ptr<juce::DrawableButton> autoFollowButton;
        
        juce::OwnedArray<juce::Drawable> drawables; // To maintain ownership of drawables

        void loadSVGIcon(juce::DrawableButton* button, const char* svgData, size_t svgDataSize, const juce::String& tooltip);
    };

    std::unique_ptr<juce::MenuBarComponent> menuBar;
    std::unique_ptr<MainMenuBarModel> menuBarModel;
    std::unique_ptr<ToolbarComponent> toolbar;
    std::unique_ptr<TimelineViewport> timelineViewport;

    // Application state
    juce::OwnedArray<TimelineModel>* timelines = nullptr;
    bool autoFollowEnabled = true;
    
    ClipboardData clipboard;

    // Zoom state
    static constexpr float ZOOM_STEP = 1.2f;

    void handleMenuAction(int menuItemID);
    void addMovementClip();
    void addActionClip();
    void deleteSelectedClips();
    void zoomIn();
    void zoomOut();
    void resetZoom();
    void toggleAutoFollow();
    
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
