#pragma once

#include "JuceHeader.h"
#include "TimelineModel.h"
#include "TimelineComponent.h"
#include "TimelineTypes.h"

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
        MainMenuBarModel(AnimatorMainView& owner) : owner(owner) {}
        
        juce::StringArray getMenuBarNames() override
        {
            return { "File", "Edit", "View", "Insert" };
        }
        
        juce::PopupMenu getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName) override;
        void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;
        
    private:
        AnimatorMainView& owner;
    };

    // Toolbar component
    class ToolbarComponent : public juce::Component
    {
    public:
        ToolbarComponent(AnimatorMainView& owner);
        void paint(juce::Graphics& g) override;
        void resized() override;

    private:
        AnimatorMainView& owner;
        std::unique_ptr<juce::TextButton> addMovementButton;
        std::unique_ptr<juce::TextButton> addActionButton;
        std::unique_ptr<juce::TextButton> deleteButton;
        std::unique_ptr<juce::TextButton> zoomInButton;
        std::unique_ptr<juce::TextButton> zoomOutButton;
        
        juce::Path createMovementIcon();
        juce::Path createActionIcon();
        juce::Path createDeleteIcon();
        juce::Path createZoomInIcon();
        juce::Path createZoomOutIcon();
    };

    std::unique_ptr<juce::MenuBarComponent> menuBar;
    std::unique_ptr<MainMenuBarModel> menuBarModel;
    std::unique_ptr<ToolbarComponent> toolbar;
    std::unique_ptr<TimelineComponent> timelineComponent;

    void handleMenuAction(int menuItemID);
    void addMovementClip();
    void addActionClip();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnimatorMainView)
};
