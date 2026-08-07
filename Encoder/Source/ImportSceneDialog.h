#pragma once
#include "JuceHeader.h"
#include "../../Common/AdditionalWindow.h"

enum class ImportSceneMode
{
    Ignore,
    Replace,
    InsertAtCursor,
    AppendAsNew,   // new group, clips keep their original timing from the file
    AppendAtCursor // new group, clips shifted so the earliest one starts at the cursor position
};

struct ImportGroupDecision
{
    ImportSceneMode mode = ImportSceneMode::Ignore;
    int targetIndex = -1; // only meaningful for Replace/InsertAtCursor
};

struct ImportSceneResult
{
    juce::Array<ImportGroupDecision> groupDecisions; // one per imported group, same order as the file
};

// Content shown inside ImportSceneDialog: for each group found in an imported scene file, lets
// the user pick an action (ignore / replace / insert at cursor) via one dropdown, and - for
// replace/insert - which group it applies to via a second dropdown that also offers "New Group"
// (i.e. append) alongside the existing groups.
class ImportSceneOptionsComponent : public juce::Component
{
public:
    ImportSceneOptionsComponent(int importedGroupCount, int existingGroupCount, bool cursorIsSet,
                                std::function<void(bool confirmed, ImportSceneResult)> onFinished)
        : numImportedGroups(importedGroupCount), completionCallback(std::move(onFinished))
    {
        headerLabel.setText(juce::String(numImportedGroups) + (numImportedGroups == 1 ? " group found in file."
                                                                                        : " groups found in file."),
                            juce::dontSendNotification);
        addAndMakeVisible(headerLabel);

        for (int i = 0; i < numImportedGroups; ++i)
        {
            auto* label = groupLabels.add(new juce::Label({}, "File Group " + juce::String(i + 1) + ":"));
            addAndMakeVisible(label);

            auto* actionCombo = groupActionCombos.add(new juce::ComboBox());
            actionCombo->addItem("Ignore", ignoreItemId);
            actionCombo->addItem("Replace", replaceItemId);
            actionCombo->addItem("Insert at Cursor", insertItemId);
            actionCombo->setItemEnabled(insertItemId, cursorIsSet); // no cursor position to insert at otherwise
            actionCombo->setSelectedId(replaceItemId, juce::dontSendNotification);
            addAndMakeVisible(actionCombo);

            auto* targetCombo = groupTargetCombos.add(new juce::ComboBox());
            for (int g = 0; g < existingGroupCount; ++g)
                targetCombo->addItem("Group " + juce::String(g + 1), g + 1);
            targetCombo->addItem("New Group", newGroupItemId);

            const int defaultTargetId = existingGroupCount > 0
                ? juce::jlimit(0, existingGroupCount - 1, i) + 1
                : newGroupItemId;
            targetCombo->setSelectedId(defaultTargetId, juce::dontSendNotification);
            addAndMakeVisible(targetCombo);

            targetCombo->setVisible(actionNeedsTarget(actionCombo->getSelectedId()));

            actionCombo->onChange = [actionCombo, targetCombo]
            {
                targetCombo->setVisible(actionNeedsTarget(actionCombo->getSelectedId()));
            };
        }

        okButton.setButtonText("Import");
        okButton.onClick = [this] { finish(true); };
        addAndMakeVisible(okButton);

        cancelButton.setButtonText("Cancel");
        cancelButton.onClick = [this] { finish(false); };
        addAndMakeVisible(cancelButton);

        setSize(480, getTotalRequiredHeight());
    }

    int getTotalRequiredHeight() const
    {
        // Must mirror resized()'s consumption exactly: 2x margin (top+bottom); header with a
        // trailing gap; one row+gap per imported group; and the final ok/cancel row, which has
        // no trailing gap.
        return 2 * margin
             + (rowHeight + rowSpacing) // header
             + numImportedGroups * (rowHeight + rowSpacing)
             + rowHeight;               // ok/cancel row (no trailing gap)
    }

private:
    void paint(juce::Graphics& g) override
    {
        // AdditionalWindow's DialogWindow background is hardcoded white; without this, Labels
        // (which paint a transparent background) use this app's light/white text colour and
        // become invisible. Match MovementClipEditor/ActionClipEditor instead.
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(margin);

        headerLabel.setBounds(area.removeFromTop(rowHeight));
        area.removeFromTop(rowSpacing);

        for (int i = 0; i < numImportedGroups; ++i)
        {
            auto row = area.removeFromTop(rowHeight);
            groupLabels[i]->setBounds(row.removeFromLeft(labelWidth));
            groupActionCombos[i]->setBounds(row.removeFromLeft(actionComboWidth));
            row.removeFromLeft(rowSpacing);
            groupTargetCombos[i]->setBounds(row);
            area.removeFromTop(rowSpacing);
        }

        auto buttonRow = area.removeFromTop(rowHeight);
        cancelButton.setBounds(buttonRow.removeFromRight(90));
        buttonRow.removeFromRight(rowSpacing);
        okButton.setBounds(buttonRow.removeFromRight(90));
    }

    static bool actionNeedsTarget(int actionId)
    {
        return actionId == replaceItemId || actionId == insertItemId;
    }

    static ImportGroupDecision decodeSelection(int actionId, int targetId)
    {
        if (actionId != replaceItemId && actionId != insertItemId)
            return { ImportSceneMode::Ignore, -1 }; // ignoreItemId, or no selection (id 0)

        // Targeting "New Group" still means append, but which action was picked determines the
        // clip timing: Replace keeps the file's original timing, Insert at Cursor shifts it.
        if (targetId == newGroupItemId)
            return { actionId == insertItemId ? ImportSceneMode::AppendAtCursor : ImportSceneMode::AppendAsNew, -1 };

        const auto mode = (actionId == replaceItemId) ? ImportSceneMode::Replace : ImportSceneMode::InsertAtCursor;
        return { mode, targetId - 1 };
    }

    void finish(bool confirmed)
    {
        ImportSceneResult result;

        if (confirmed)
            for (int i = 0; i < numImportedGroups; ++i)
                result.groupDecisions.add(decodeSelection(groupActionCombos[i]->getSelectedId(), groupTargetCombos[i]->getSelectedId()));

        // completionCallback closes (deletes) this component's own window. Deleting it here,
        // synchronously, would destroy okButton/cancelButton while their own click-dispatch is
        // still unwinding on this exact call stack - a use-after-free. Defer to the next message
        // loop turn instead, so the button's click handling has fully returned first.
        if (completionCallback)
        {
            auto callback = completionCallback;
            juce::MessageManager::callAsync([callback, confirmed, result] { callback(confirmed, result); });
        }
    }

    static constexpr int ignoreItemId = 1;
    static constexpr int replaceItemId = 2;
    static constexpr int insertItemId = 3;
    static constexpr int newGroupItemId = 100000; // comfortably beyond any realistic existing-group id (1..N)

    static constexpr int rowHeight = 26;
    static constexpr int rowSpacing = 6;
    static constexpr int margin = 10;
    static constexpr int labelWidth = 100;
    static constexpr int actionComboWidth = 150;

    int numImportedGroups;
    std::function<void(bool confirmed, ImportSceneResult)> completionCallback;

    juce::Label headerLabel;
    juce::OwnedArray<juce::Label> groupLabels;
    juce::OwnedArray<juce::ComboBox> groupActionCombos;
    juce::OwnedArray<juce::ComboBox> groupTargetCombos;
    juce::TextButton okButton, cancelButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ImportSceneOptionsComponent)
};

// Modal window hosting ImportSceneOptionsComponent. The X/close button behaves like Cancel.
class ImportSceneDialog : public AdditionalWindow
{
public:
    ImportSceneDialog(std::unique_ptr<juce::Component> content, std::function<void()> onCancelRequested)
        : AdditionalWindow("Import Scene", content.get()), cancelCallback(std::move(onCancelRequested))
    {
        const int w = content->getWidth();
        const int h = content->getHeight();

        setAlwaysOnTop(true);
        setContentOwned(content.release(), true);
        setResizable(false, false);
        setUsingNativeTitleBar(false);

        // getTitleBarHeight() reflects the custom (non-native) title bar only once
        // setUsingNativeTitleBar(false) has already taken effect - must be queried after it.
        const int titleBarHeight = getTitleBarHeight();
        setSize(w, h + titleBarHeight);
        centreWithSize(getWidth(), getHeight());
    }

    void closeButtonPressed() override
    {
        // Deferred for the same reason as ImportSceneOptionsComponent::finish(): cancelCallback
        // deletes this window, which must not happen synchronously from within its own
        // close-button dispatch.
        if (cancelCallback)
        {
            auto callback = cancelCallback;
            juce::MessageManager::callAsync([callback] { callback(); });
        }
    }

private:
    std::function<void()> cancelCallback;
};
