#include "AnimatorMainView.h"

AnimatorMainView::AnimatorMainView()
{
    // Create menu bar
    menuBarModel = std::make_unique<MainMenuBarModel>(*this);
    menuBar = std::make_unique<juce::MenuBarComponent>(menuBarModel.get());
    
    // Create toolbar
    toolbar = std::make_unique<ToolbarComponent>(*this);
    
    // Create timeline component
    timelineComponent = std::make_unique<TimelineComponent>();
    
    addAndMakeVisible(menuBar.get());
    addAndMakeVisible(toolbar.get());
    addAndMakeVisible(timelineComponent.get());
}

AnimatorMainView::~AnimatorMainView()
{
    // Remove menu bar before destruction to prevent JUCE assertion
    if (menuBar != nullptr)
    {
        removeChildComponent(menuBar.get());
        menuBar.reset();
    }
    menuBarModel.reset();
}

void AnimatorMainView::setTimelines(juce::OwnedArray<TimelineModel>* timelines)
{
    timelineComponent->setTimelines(timelines);
}

void AnimatorMainView::setPlayheadPosition(ms_t timeMs)
{
    timelineComponent->setPlayheadPosition(timeMs);
}

void AnimatorMainView::setAutoFollow(bool shouldAutoFollow)
{
    timelineComponent->setAutoFollow(shouldAutoFollow);
}

void AnimatorMainView::setPlayheadProvider(std::function<PlayheadSnapshot()> provider)
{
    // If you need to pass this to the timeline component
}

void AnimatorMainView::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1e1e1e));
}

void AnimatorMainView::resized()
{
    auto area = getLocalBounds();
    
    // Menu bar at top (24 pixels)
    menuBar->setBounds(area.removeFromTop(24));
    
    // Toolbar below menu (40 pixels)
    toolbar->setBounds(area.removeFromTop(40));
    
    // Rest goes to timeline component
    timelineComponent->setBounds(area);
}

// Menu implementation
juce::PopupMenu AnimatorMainView::MainMenuBarModel::getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName)
{
    juce::PopupMenu menu;
    
    switch (topLevelMenuIndex)
    {
        case 0: // File
            menu.addItem(1, "New Project");
            menu.addItem(2, "Open Project...");
            menu.addItem(3, "Save Project");
            menu.addItem(4, "Save Project As...");
            menu.addSeparator();
            menu.addItem(5, "Export...");
            menu.addSeparator();
            menu.addItem(6, "Preferences");
            break;
            
        case 1: // Edit
            menu.addItem(10, "Undo");
            menu.addItem(11, "Redo");
            menu.addSeparator();
            menu.addItem(12, "Cut");
            menu.addItem(13, "Copy");
            menu.addItem(14, "Paste");
            menu.addItem(15, "Delete");
            menu.addSeparator();
            menu.addItem(16, "Select All");
            menu.addItem(17, "Deselect All");
            break;
            
        case 2: // View
            menu.addItem(20, "Zoom In");
            menu.addItem(21, "Zoom Out");
            menu.addItem(22, "Reset Zoom");
            menu.addSeparator();
            menu.addItem(23, "Toggle Auto-follow");
            menu.addItem(24, "Show/Hide Rulers");
            break;
            
        case 3: // Insert
            menu.addItem(30, "Add Movement Clip");
            menu.addItem(31, "Add Action Clip");
            menu.addSeparator();
            menu.addItem(32, "Add New Timeline");
            menu.addItem(33, "Add New Layer");
            break;
    }
    
    return menu;
}

void AnimatorMainView::MainMenuBarModel::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
    owner.handleMenuAction(menuItemID);
}

void AnimatorMainView::handleMenuAction(int menuItemID)
{
    switch (menuItemID)
    {
        case 30: // Add Movement Clip
            addMovementClip();
            break;
        case 31: // Add Action Clip
            addActionClip();
            break;
        case 15: // Delete
        case 16: // Select All
        case 17: // Deselect All
            // These would be handled by the timeline component
            break;
        case 20: // Zoom In
        case 21: // Zoom Out
            // These would adjust timeline zoom
            break;
    }
}

void AnimatorMainView::addMovementClip()
{
    // This would need to interface with the timeline component
    // For now, just a placeholder
}

void AnimatorMainView::addActionClip()
{
    // This would need to interface with the timeline component
    // For now, just a placeholder
}

// ToolbarComponent implementation
AnimatorMainView::ToolbarComponent::ToolbarComponent(AnimatorMainView& ownerRef)
    : owner(ownerRef)
{
    // Create buttons with icons
    addMovementButton = std::make_unique<juce::TextButton>();
    addActionButton = std::make_unique<juce::TextButton>();
    deleteButton = std::make_unique<juce::TextButton>();
    zoomInButton = std::make_unique<juce::TextButton>();
    zoomOutButton = std::make_unique<juce::TextButton>();
    
    // Set button properties
    addMovementButton->setTooltip("Add Movement Clip");
    addActionButton->setTooltip("Add Action Clip");
    deleteButton->setTooltip("Delete Selected");
    zoomInButton->setTooltip("Zoom In");
    zoomOutButton->setTooltip("Zoom Out");
    
    // Connect buttons to actions
    addMovementButton->onClick = [this] { owner.addMovementClip(); };
    addActionButton->onClick = [this] { owner.addActionClip(); };
    deleteButton->onClick = [this] { 
        // Would delete selected clips in timeline
    };
    zoomInButton->onClick = [this] {
        // Would zoom in timeline
    };
    zoomOutButton->onClick = [this] {
        // Would zoom out timeline
    };
    
    addAndMakeVisible(addMovementButton.get());
    addAndMakeVisible(addActionButton.get());
    addAndMakeVisible(deleteButton.get());
    addAndMakeVisible(zoomInButton.get());
    addAndMakeVisible(zoomOutButton.get());
}

void AnimatorMainView::ToolbarComponent::paint(juce::Graphics& g)
{
    // Draw toolbar background
    g.fillAll(juce::Colour(0xff2d2d30));
    
    // Draw subtle border at bottom
    g.setColour(juce::Colours::black.withAlpha(0.3f));
    g.drawLine(0.0f, (float)getHeight(), (float)getWidth(), (float)getHeight(), 1.0f);
    
    // Draw icons on buttons
    auto drawIconOnButton = [&g](juce::Button* button, const juce::Path& icon) {
        auto bounds = button->getLocalBounds().toFloat().reduced(8.0f);
        g.setColour(juce::Colours::white.withAlpha(button->isOver() ? 1.0f : 0.8f));
        g.fillPath(icon, icon.getTransformToScaleToFit(bounds, true));
    };
    
    drawIconOnButton(addMovementButton.get(), createMovementIcon());
    drawIconOnButton(addActionButton.get(), createActionIcon());
    drawIconOnButton(deleteButton.get(), createDeleteIcon());
    drawIconOnButton(zoomInButton.get(), createZoomInIcon());
    drawIconOnButton(zoomOutButton.get(), createZoomOutIcon());
}

void AnimatorMainView::ToolbarComponent::resized()
{
    auto area = getLocalBounds().reduced(5, 5);
    int buttonSize = 30;
    int spacing = 5;
    
    addMovementButton->setBounds(area.removeFromLeft(buttonSize));
    area.removeFromLeft(spacing);
    addActionButton->setBounds(area.removeFromLeft(buttonSize));
    area.removeFromLeft(spacing * 2);
    deleteButton->setBounds(area.removeFromLeft(buttonSize));
    area.removeFromLeft(spacing * 3);
    zoomOutButton->setBounds(area.removeFromLeft(buttonSize));
    area.removeFromLeft(spacing);
    zoomInButton->setBounds(area.removeFromLeft(buttonSize));
}

juce::Path AnimatorMainView::ToolbarComponent::createMovementIcon()
{
    juce::Path path;
    path.addTriangle({0.0f, 1.0f}, {1.0f, 0.5f}, {0.0f, 0.0f});
    return path;
}

juce::Path AnimatorMainView::ToolbarComponent::createActionIcon()
{
    juce::Path path;
    path.addEllipse(0.2f, 0.2f, 0.6f, 0.6f);
    
    for (int i = 0; i < 8; ++i)
    {
        float angle = i * juce::MathConstants<float>::pi / 4.0f;
        juce::Path tooth;
        tooth.addRectangle(0.45f, 0.1f, 0.1f, 0.3f);
        tooth.applyTransform(juce::AffineTransform::rotation(angle, 0.5f, 0.5f));
        path.addPath(tooth);
    }
    return path;
}

juce::Path AnimatorMainView::ToolbarComponent::createDeleteIcon()
{
    juce::Path path;
    path.addRectangle(0.2f, 0.3f, 0.6f, 0.1f);
    path.addRectangle(0.3f, 0.1f, 0.4f, 0.1f);
    path.addRectangle(0.45f, 0.5f, 0.1f, 0.4f);
    path.addRectangle(0.35f, 0.5f, 0.1f, 0.4f);
    path.addRectangle(0.55f, 0.5f, 0.1f, 0.4f);
    return path;
}

juce::Path AnimatorMainView::ToolbarComponent::createZoomInIcon()
{
    juce::Path path;
    path.addEllipse(0.1f, 0.1f, 0.5f, 0.5f);
    path.addLineSegment({0.5f, 0.35f, 0.8f, 0.65f}, 0.1f);
    path.addLineSegment({0.35f, 0.35f, 0.65f, 0.35f}, 0.1f);
    path.addLineSegment({0.5f, 0.2f, 0.5f, 0.5f}, 0.1f);
    return path;
}

juce::Path AnimatorMainView::ToolbarComponent::createZoomOutIcon()
{
    juce::Path path;
    path.addEllipse(0.1f, 0.1f, 0.5f, 0.5f);
    path.addLineSegment({0.5f, 0.35f, 0.8f, 0.65f}, 0.1f);
    path.addLineSegment({0.35f, 0.35f, 0.65f, 0.35f}, 0.1f);
    return path;
}
