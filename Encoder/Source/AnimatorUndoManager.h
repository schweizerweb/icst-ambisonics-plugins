#pragma once
#include "JuceHeader.h"
#include "TimelineModel.h"

// Undo/redo for the animator, by snapshotting the whole timeline set rather than by recording
// individual edits.
//
// That choice is deliberate. Clips are mutated from roughly twenty places - the timeline's own
// add/delete/duplicate/paste, drag and resize gestures, both clip editors' Apply, timeline add and
// remove, and scene import - several of which rewrite a clip wholesale. A per-edit UndoableAction
// for each would be a large amount of code whose correctness depends on every future mutation
// remembering to participate; a snapshot is correct for all of them by construction, including ones
// added later, and the state involved is small (a few KB of XML for a full scene).
//
// The snapshot is TimelineModel's own toXml/fromXml, so undo automatically covers every field those
// cover - there is no second serialisation to keep in step.
//
// Scope: one history per animator window session. The window owns this, so closing the animator
// clears the history; the clips themselves live on in the processor.
//
// Message thread only - every caller is a UI action.
class AnimatorUndoManager
{
public:
    // Explicit, because JUCE_DECLARE_NON_COPYABLE below declares a (deleted) copy constructor, and
    // any user-declared constructor suppresses the implicit default one.
    AnimatorUndoManager() = default;

    // Call BEFORE mutating, with the state as it is *now*: that pre-mutation state is what undo
    // returns to. Discards the redo stack, since history has branched.
    void pushStep(const juce::String& name, const juce::OwnedArray<TimelineModel>& timelines)
    {
        if (suppressed)
            return;

        undoStack.add({ name, serialise(timelines) });
        trim();
        redoStack.clearQuick();
    }

    // For compound operations built out of smaller ones that each push a step of their own - scene
    // import runs insertTimelineAtCursor() once per imported group, and without this a single import
    // would leave one history entry per group instead of one for the import. Push the step for the
    // whole operation first, then suppress for its duration.
    class ScopedSuppressor
    {
    public:
        explicit ScopedSuppressor(AnimatorUndoManager& m) : manager(m), wasSuppressed(m.suppressed)
        {
            manager.suppressed = true;
        }

        ~ScopedSuppressor() { manager.suppressed = wasSuppressed; }

    private:
        AnimatorUndoManager& manager;
        bool wasSuppressed;

        JUCE_DECLARE_NON_COPYABLE(ScopedSuppressor)
    };

    // Gestures (dragging or resizing a clip) call beginPending() when the gesture starts and
    // commitPendingIfChanged() when it ends, so one drag is one undo step - and a click that merely
    // selects a clip, or a drag that returns it to where it began, adds no step at all.
    void beginPending(const juce::String& name, const juce::OwnedArray<TimelineModel>& timelines)
    {
        if (suppressed)
            return;

        pendingName = name;
        pendingState = serialise(timelines);
        hasPending = true;
    }

    void commitPendingIfChanged(const juce::OwnedArray<TimelineModel>& timelines)
    {
        if (!hasPending)
            return;

        hasPending = false;

        if (serialise(timelines) == pendingState)
            return; // nothing actually moved

        undoStack.add({ pendingName, pendingState });
        trim();
        redoStack.clearQuick();
    }

    void abandonPending() { hasPending = false; }

    bool canUndo() const { return !undoStack.isEmpty(); }
    bool canRedo() const { return !redoStack.isEmpty(); }

    // For the menu items, so they can read "Undo Move Clip" rather than a bare "Undo".
    juce::String getUndoName() const { return canUndo() ? undoStack.getLast().name : juce::String(); }
    juce::String getRedoName() const { return canRedo() ? redoStack.getLast().name : juce::String(); }

    bool undo(juce::OwnedArray<TimelineModel>& timelines)
    {
        if (!canUndo())
            return false;

        auto step = undoStack.getLast();
        undoStack.removeLast();

        // The state being replaced becomes the redo entry, keeping the step's name attached to the
        // edit it describes in both directions.
        redoStack.add({ step.name, serialise(timelines) });
        trim();

        return deserialise(step.state, timelines);
    }

    bool redo(juce::OwnedArray<TimelineModel>& timelines)
    {
        if (!canRedo())
            return false;

        auto step = redoStack.getLast();
        redoStack.removeLast();

        undoStack.add({ step.name, serialise(timelines) });
        trim();

        return deserialise(step.state, timelines);
    }

    void clear()
    {
        undoStack.clearQuick();
        redoStack.clearQuick();
        hasPending = false;
    }

private:
    struct Step
    {
        juce::String name;
        juce::String state;
    };

    // Bounded so a long editing session can't grow without limit. 64 steps is far more than the
    // "oops, undo that" this exists for, and a scene's XML is small.
    static constexpr int maxSteps = 64;

    void trim()
    {
        while (undoStack.size() > maxSteps) undoStack.remove(0);
        while (redoStack.size() > maxSteps) redoStack.remove(0);
    }

    static juce::String serialise(const juce::OwnedArray<TimelineModel>& timelines)
    {
        juce::XmlElement root("AnimatorUndoState");

        for (auto* t : timelines)
            if (t != nullptr)
                root.addChildElement(t->toXml().release());

        return root.toString(juce::XmlElement::TextFormat().singleLine().withoutHeader());
    }

    static bool deserialise(const juce::String& state, juce::OwnedArray<TimelineModel>& timelines)
    {
        auto xml = juce::XmlDocument::parse(state);
        if (xml == nullptr || !xml->hasTagName("AnimatorUndoState"))
            return false;

        // Built completely before anything is destroyed, so a malformed snapshot leaves the existing
        // timelines untouched rather than half-replaced.
        juce::OwnedArray<TimelineModel> restored;

        for (auto* xTimeline : xml->getChildWithTagNameIterator("Timeline"))
        {
            auto model = std::make_unique<TimelineModel>();
            if (!model->fromXml(*xTimeline))
                return false;

            restored.add(model.release());
        }

        timelines.clear(true);
        while (!restored.isEmpty())
            timelines.add(restored.removeAndReturn(0));

        return true;
    }

    juce::Array<Step> undoStack, redoStack;

    juce::String pendingName, pendingState;
    bool hasPending = false;
    bool suppressed = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnimatorUndoManager)
};
