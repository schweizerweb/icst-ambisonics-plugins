#pragma once

#include "JuceHeader.h"
#include "TimelineViewport.h"
#include "StatusBarComponent.h"

class AnimatorMainView : public juce::Component,
                        public juce::Timer,
                        public juce::ApplicationCommandTarget
{
public:
    AnimatorMainView();
    ~AnimatorMainView() override;

    void setTimelines(juce::OwnedArray<TimelineModel>* timelines);
    void setSelectionControl(PointSelection* pPointSelection);
    void setSourceSet(AmbiSourceSet* pSources);
    void setPlayheadPosition(ms_t timeMs);
    void setAutoFollow(bool shouldAutoFollow);
    void setPlayheadProvider(std::function<PlayheadSnapshot()> provider);

    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;

    // Status bar functionality
    void setStatusMessage(const juce::AttributedString& message);
    void clearStatusMessage();
    
    // Validation interface
    void setValidationFrequency(double frequencyHz);
    void validateTimelines();
    std::function<void(const juce::AttributedString&)> getStatusMessageFunction();

    enum CommandIDs
    {
        CMD_cut = 0x2000,
        CMD_copy,
        CMD_paste,
        CMD_deleteSelected,
        CMD_duplicate,
        CMD_selectAll,
        CMD_deselectAll,
        CMD_zoomIn,
        CMD_zoomOut,
        CMD_resetZoom,
        CMD_addMovementClip,
        CMD_addActionClip,
        CMD_toggleAutoFollow
    };
    
private:
    // Menu bar
    class MainMenuBarModel : public juce::MenuBarModel
    {
    public:
        MainMenuBarModel(AnimatorMainView* ownerView) : owner(ownerView) {}
        
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
    
    // Zoom state
    static constexpr float ZOOM_STEP = 1.2f;

    void handleMenuAction(int menuItemID);
    
    // Edit operations
    void cutSelectedClips();
    void copySelectedClips();
    void pasteClips();
    void selectAllClips();
    void deselectAllClips();
    void toggleAutoFollow();
    
    // Import/Export
    void importScene(int timelineIndex);
    void exportScene(int timelineIndex);
    
    // Status bar
    std::unique_ptr<StatusBarComponent> statusBar;
    void updateStatusBarValidation();
    bool validationResult = true;
    juce::String validationDetails;
        
    // ApplicationCommandTarget implementation
    std::unique_ptr<juce::ApplicationCommandManager> commandManager;
    juce::ApplicationCommandTarget* getNextCommandTarget() override { return nullptr; }
    void getAllCommands(juce::Array<juce::CommandID>& commands) override;
    void getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;
    bool perform(const juce::ApplicationCommandTarget::InvocationInfo& info) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnimatorMainView)
};
