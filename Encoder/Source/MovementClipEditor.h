#pragma once

#include <JuceHeader.h>
#include "CommonClipSettings.h"
#include "ClipPreviewComponent.h"
#include "ClipEditorCloseGuard.h"
#include "../../Common/AmbiSourceSet.h"
#include "../../Common/PointSelection.h"
#include "../../Common/TableColumnCallback.h"
#include "../../Common/NumericColumnCustomComponent.h"
#include "../../Common/CheckBoxCustomComponent.h"
#include "InfoIconButton.h"

class TimelineComponent;

class PrecisionSlider : public juce::Slider
{
public:
    PrecisionSlider()
    {
        // juce::Slider has no direct API to right-align its value text box, so this goes through
        // a LookAndFeel override instead. Scoped to a dedicated shared instance (not the app's
        // default LookAndFeel), so it only affects PrecisionSliders, not every slider in the app.
        setLookAndFeel(&getRightAlignedLookAndFeel());
    }

    ~PrecisionSlider() override
    {
        setLookAndFeel(nullptr);
    }

    double getPreciseValue()
    {
        return normalizeNearZero(Slider::getValue());
    }

    double getValue() const
    {
        return normalizeNearZero(Slider::getValue());
    }

    void setPrecisionThreshold(double newThreshold) { threshold = newThreshold; }
    double getPrecisionThreshold() const { return threshold; }

private:
    class RightAlignedTextBoxLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        juce::Label* createSliderTextBox(juce::Slider& slider) override
        {
            auto* label = juce::LookAndFeel_V4::createSliderTextBox(slider);
            label->setJustificationType(juce::Justification::centredRight);
            return label;
        }
    };

    static RightAlignedTextBoxLookAndFeel& getRightAlignedLookAndFeel()
    {
        static RightAlignedTextBoxLookAndFeel lookAndFeel;
        return lookAndFeel;
    }

    double threshold = 0.001;

    double normalizeNearZero(double value) const
    {
        if (std::abs(value) < threshold)
            return 0.0;
        return value;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PrecisionSlider)
};

// A coordinate field that behaves as a draggable slider for bounded (finite-scaled) values, or
// as an IncDecButtons "updown" spinner (same style as the Count/Radius change fields) for
// unbounded ones - a draggable slider whose range spans an "infinite" scaler makes little sense,
// since almost its entire travel would represent values near zero. The updown style has no drag
// track to misrepresent, so it stays usable no matter how large the range is.
class CoordinateValueControl : public juce::Component
{
public:
    CoordinateValueControl()
    {
        addAndMakeVisible(slider);
        applyStyle();
    }

    void setRange(double min, double max, double step)
    {
        slider.setRange(min, max, step);
    }

    void setValue(double value)
    {
        slider.setValue(value, juce::dontSendNotification);
    }

    double getPreciseValue() const
    {
        return slider.getValue();
    }

    double getValue() const
    {
        return getPreciseValue();
    }

    void setUpDownStyle(bool shouldUseUpDown)
    {
        if (useUpDown == shouldUseUpDown)
            return;

        useUpDown = shouldUseUpDown;
        applyStyle();
        resized();
    }

private:
    void applyStyle()
    {
        if (useUpDown)
        {
            slider.setSliderStyle(juce::Slider::IncDecButtons);
            slider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, textBoxWidth, 22);
        }
        else
        {
            slider.setSliderStyle(juce::Slider::LinearHorizontal);
            slider.setTextBoxStyle(juce::Slider::TextBoxRight, false, textBoxWidth, 22);
        }
    }

    void resized() override
    {
        if (useUpDown)
        {
            // The +/- buttons fill whatever's left of the slider's own bounds once the text box
            // is subtracted, so without an explicit width they stretch to fill the whole row.
            // Cap the total at the same width as the text box, matching the draggable slider's
            // footprint, and anchor it to the right of whatever space is available so it lines up
            // under the label column's right edge rather than trailing off with empty space after it.
            slider.setBounds(getLocalBounds().removeFromRight(textBoxWidth * 2));
        }
        else
        {
            slider.setBounds(getLocalBounds());
        }
    }

    static constexpr int textBoxWidth = 70;
    bool useUpDown = false;
    PrecisionSlider slider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CoordinateValueControl)
};

// Inline editor for a Spline/Polygon clip's waypoint list - X/Y/Z typed per row, plus a "Break"
// flag marking where a new disjoint sub-path starts. Built on the same TableColumnCallback +
// NumericColumnCustomComponent/CheckBoxCustomComponent pieces the source and group tables use, so
// cell editing behaves identically to the rest of the app.
class WaypointTableListModel : public juce::TableListBoxModel, public TableColumnCallback
{
public:
    enum ColumnIds { ColumnIndex = 1, ColumnX, ColumnY, ColumnZ, ColumnBreak };

    WaypointTableListModel(juce::Array<MovementWaypoint>& pointsRef, juce::TableListBox& tableRef)
        : points(pointsRef), table(tableRef) {}

    // Fired after any cell edit, so the host can refresh the preview immediately rather than
    // waiting for its 150ms poll.
    std::function<void()> onWaypointEdited;

    void setScalingInfo(ScalingInfo* scaling) { pScalingInfo = scaling; }

    int getNumRows() override { return points.size(); }

    void paintRowBackground(juce::Graphics& g, int rowNumber, int, int, bool rowIsSelected) override
    {
        auto& lf = table.getLookAndFeel();
        auto base = lf.findColour(juce::ListBox::backgroundColourId);
        g.fillAll(rowIsSelected ? base.brighter(0.3f)
                                : (rowNumber % 2 ? base : base.brighter(0.04f)));
    }

    void paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool) override
    {
        if (columnId != ColumnIndex || rowNumber >= points.size())
            return;

        // A segment break is also drawn as a line above the row, so the sub-path grouping is
        // readable at a glance without reading the checkbox column.
        if (points.getReference(rowNumber).startsNewSegment && rowNumber > 0)
        {
            g.setColour(juce::Colours::orange.withAlpha(0.8f));
            g.fillRect(0, 0, width, 2);
        }

        g.setColour(table.getLookAndFeel().findColour(juce::ListBox::textColourId));
        g.setFont(juce::FontOptions(13.0f));
        g.drawText(juce::String(rowNumber + 1), 4, 0, width - 6, height, juce::Justification::centredLeft);
    }

    juce::Component* refreshComponentForCell(int rowNumber, int columnId, bool,
                                             juce::Component* existingComponentToUpdate) override
    {
        if (columnId == ColumnBreak)
        {
            auto* box = static_cast<CheckBoxCustomComponent*>(existingComponentToUpdate);
            if (box == nullptr) box = new CheckBoxCustomComponent(*this);
            box->setRowAndColumn(rowNumber, columnId);
            return box;
        }

        if (columnId == ColumnX || columnId == ColumnY || columnId == ColumnZ)
        {
            auto* cell = static_cast<NumericColumnCustomComponent*>(existingComponentToUpdate);
            if (cell == nullptr) cell = new NumericColumnCustomComponent(*this);
            cell->setRowAndColumn(rowNumber, columnId);
            return cell;
        }

        delete existingComponentToUpdate;
        return nullptr;
    }

    double getValue(int columnId, int rowNumber) override
    {
        if (rowNumber < 0 || rowNumber >= points.size()) return 0.0;
        const auto& wp = points.getReference(rowNumber);

        switch (columnId)
        {
            case ColumnX:     return wp.x;
            case ColumnY:     return wp.y;
            case ColumnZ:     return wp.z;
            case ColumnBreak: return wp.startsNewSegment ? 1.0 : 0.0;
            default:          return 0.0;
        }
    }

    void setValue(int columnId, int rowNumber, double newValue) override
    {
        if (rowNumber < 0 || rowNumber >= points.size()) return;
        auto& wp = points.getReference(rowNumber);

        switch (columnId)
        {
            case ColumnX:     wp.x = newValue; break;
            case ColumnY:     wp.y = newValue; break;
            case ColumnZ:     wp.z = newValue; break;
            case ColumnBreak: wp.startsNewSegment = !juce::exactlyEqual(newValue, 0.0); break;
            default: return;
        }

        table.repaint(); // the index column draws the break marker
        if (onWaypointEdited) onWaypointEdited();
    }

    SliderRange getSliderRange(int) override
    {
        // CartesianMin/Max already substitute a large-but-finite bound in "infinite" mode, so this
        // must not gate on IsInfinite() - the same rule the coordinate sliders follow.
        if (pScalingInfo != nullptr)
            return SliderRange(pScalingInfo->CartesianMin(), pScalingInfo->CartesianMax(), 0.001);
        return SliderRange(-10.0, 10.0, 0.001);
    }

    juce::TableListBox* getTable() override { return &table; }
    juce::String getTableText(const int, const int) override { return {}; }
    void setTableText(const int, const int, const juce::String&) override {}

    // The first waypoint always begins the first sub-path, so its own Break flag would be
    // meaningless - greyed out rather than hidden, so the column stays readable.
    bool getEnabled(const int columnId, const int rowNumber) override
    {
        return !(columnId == ColumnBreak && rowNumber == 0);
    }

private:
    juce::Array<MovementWaypoint>& points;
    juce::TableListBox& table;
    ScalingInfo* pScalingInfo = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaypointTableListModel)
};

class MovementClipEditor : public juce::Component, public juce::ChangeListener,
                            public ClipEditorCloseGuard, private juce::Timer
{
public:
    MovementClipEditor(TimelineComponent& timeline, int timelineIdx, int clipIdx);
    ~MovementClipEditor() override;

    void resized() override;
    void paint(juce::Graphics& g) override;

    int getTotalRequiredHeight() const;
    int getTotalRequiredWidth() const;
    bool applyChanges();

    void changeListenerCallback(ChangeBroadcaster* source) override;

    // True if the live UI state differs from dirtyBaseline (see its own comment below).
    bool isDirty();
    bool confirmDiscardIfDirty() override;

private:
    AmbiSourceSet* pSourceSet;
    PointSelection* pPointSelection;
    TimelineComponent& timelineComp;
    int timelineIndex, clipIndex;
    MovementClip currentClip;
    // What buildClipFromControls() produced right after the controls were first populated from
    // currentClip, before any user interaction - NOT simply a copy of currentClip itself. The
    // coordinate sliders snap to their configured interval (0.01 for X/Y/Z, 0.1 for Azimuth/
    // Elevation - see juce::Slider::setValue()/NormalisableRange::snapToLegalValue()), so loading
    // currentClip's full-precision values into them already quantizes away anything finer than that
    // grid. Comparing a later buildClipFromControls() against raw currentClip would then see that
    // quantization itself as a "change" and report dirty on a freshly opened, untouched dialog -
    // comparing against this already-quantized baseline instead makes it an apples-to-apples
    // comparison, so only a genuine edit moves it off the grid point it started on.
    MovementClip dirtyBaseline;
    
    juce::Vector3D<double> currentPosition;
    bool currentPositionValid = false;
    
    CommonClipSettings commonSettings;
    ClipPreviewComponent preview;

    juce::GroupComponent clipGroup{"Clip", "Clip Properties"};
    juce::GroupComponent movementGroup{"Movement", "Movement Properties"};
    juce::GroupComponent previewGroup{"Preview", "Preview"};
    
    juce::TextButton applyButton{"Apply"}, cancelButton{"Cancel"};
    juce::TextButton applyCurrentStartButton, applyCurrentTargetButton;
    
    juce::ToggleButton usePolarDisplay;
    juce::ToggleButton useStartPosition;
    
    // Movement type selector
    juce::ComboBox movementTypeCombo;
    juce::Label movementTypeLabel;

    CoordinateValueControl startXSlider, startYSlider, startZSlider;
    CoordinateValueControl targetXSlider, targetYSlider, targetZSlider;
    juce::Label startXLabel, startYLabel, startZLabel;
    juce::Label targetXLabel, targetYLabel, targetZLabel;

    // New properties
    CoordinateValueControl countSlider;
    juce::Label countLabel;
    CoordinateValueControl radiusChangeSlider;
    juce::Label radiusChangeLabel;

    // Per-type parameters - each row is only laid out (and only counted in
    // getMovementControlsHeight()) for the types that actually use it, see the show*() predicates.
    // Spline/Polygon: joins each sub-path's last waypoint back to its first.
    juce::ToggleButton closedPathToggle;
    InfoIconButton closedPathInfo { "Closed path: each sub-path runs from its last waypoint back to "
                                    "its first, so the figure is a loop rather than an open line.\n\n"
                                    "Because a closed path finishes where it started, it can also be "
                                    "repeated without Palindrome - an open one would snap back to its "
                                    "beginning at every repeat." };

    CoordinateValueControl tensionSlider;     // Spline
    juce::Label tensionLabel;
    // "Tension (0=round, 1=straight):" didn't fit the label column, so the range explanation moved
    // behind an icon - the same treatment as Muted/Palindrome in CommonClipSettings.
    InfoIconButton tensionInfo { "Tension controls how round the spline is between its waypoints.\n\n"
                                 "0 = roundest (a Catmull-Rom curve through the points).\n"
                                 "1 = completely straight, which looks identical to a Polygon." };
    CoordinateValueControl heightRiseSlider;  // Helix
    juce::Label heightRiseLabel;
    CoordinateValueControl freqASlider;       // Lissajous (X frequency) / Rose (petal count)
    juce::Label freqALabel;
    CoordinateValueControl freqBSlider;       // Lissajous (Y frequency)
    juce::Label freqBLabel;
    CoordinateValueControl phaseSlider;       // Lissajous / Rose
    juce::Label phaseLabel;
    CoordinateValueControl randomSeedSlider;  // Random Walk
    juce::Label randomSeedLabel;

    // Spline/Polygon waypoint list. Edits mutate currentClip.waypoints directly - the same live
    // mutation ActionClipEditor does with currentClip.actions, and safe for the same reason: dirty
    // detection compares against the separate dirtyBaseline snapshot, not against currentClip.
    juce::TableListBox waypointTable;
    std::unique_ptr<WaypointTableListModel> waypointModel;
    juce::TextButton addWaypointButton{"Add"}, removeWaypointButton{"Remove"};

    int getWaypointTableHeight() const { return 132; }
    void addWaypoint();
    void removeSelectedWaypoint();
    void refreshWaypointTable();
    void ensureWaypointsSeeded();

    // Which optional rows the currently selected type shows. getMovementControlsHeight() and
    // layoutMovementControls() both derive from these, so the reserved height and the laid-out rows
    // can never drift apart - and the dialog has exactly zero spare vertical space, so a mismatch
    // would clip the last row immediately.
    MovementType getSelectedMovementType() const;
    bool showsPolarToggle() const;
    bool showsTargetRows() const;
    bool showsWaypointTable() const;
    bool showsCount() const;
    bool showsRadiusChange() const;
    bool showsTension() const;
    bool showsHeightRise() const;
    bool showsFreqA() const;
    bool showsFreqB() const;
    bool showsPhase() const;
    bool showsRandomSeed() const;

    void createControls();
    void createCoordinateSlider(CoordinateValueControl& slider, juce::Label& label, const juce::String& name,
                               double min, double max, double defaultValue);
    void createStandardSlider(CoordinateValueControl& slider, juce::Label& label, const juce::String& name, double defaultValue);
    void createApplyCurrentPositionButton(juce::TextButton& button, CoordinateValueControl& xSlider, CoordinateValueControl& ySlider, CoordinateValueControl& zSlider);
    void updateApplyCurrentPositionButtonText(juce::TextButton& button, const juce::Vector3D<double>& vector, bool isValid);
    void updateCurrentPosition(bool force = false);
    int getMovementControlsHeight() const;
    // Preview sits beside (not below) the Clip Properties group, in portrait orientation (two
    // square panels stacked) - narrower but taller than the old side-by-side layout.
    int getClipPropertiesWidth() const { return 300; }
    // Wider than it was, and paired with the fixed column height below this gives the two radar
    // panels roughly double the area they had - the preview is the thing you actually read while
    // designing a movement.
    int getPreviewWidth() const { return 280; }
    // Narrowed to pay for the preview. Still wide enough for the waypoint table's columns plus the
    // widest label ("Radius change / rotation:") and its value control.
    int getMovementPropertiesWidth() const { return 370; }

    // ONE height for every movement type, rather than one that grows and shrinks as the type combo
    // changes. Sized for the tallest type (Lissajous) with room to spare, so the dialog never jumps
    // and the Clip Properties column is tall enough to hold the speed curve editor inline. Columns
    // that need less simply have slack - except the waypoint table, which stretches into it.
    int getFixedColumnHeight() const { return 610; }

    // Movement Properties stops this far short of the other two columns, and Apply/Cancel sit in the
    // gap - so the buttons tuck into the layout instead of adding a band of their own underneath.
    int getButtonInsetHeight() const { return 36; } // 28px buttons + 8px gap
    void layoutMovementControls(juce::Rectangle<int> area);
    void updateControlVisibility();
    // Whether THIS clip would jump at a repeat boundary - depends on the closed flag, not just
    // the movement type, so it is recomputed whenever either changes.
    void refreshPalindromeConstraint();
    void updateCoordinateSystem();
    void updateSliderLabelsAndRanges();  // New method to update UI based on coordinate system
    juce::Vector3D<double> getCurrentPositionInSelectedSystem() const;  // New method to get position in current coordinate system
    // Shared Cartesian->Polar conversion used by both getCurrentPositionInSelectedSystem() and the
    // preview's onPointDragged callback - see MovementClipEditor.cpp.
    juce::Vector3D<double> convertCartesianToSelectedSystem(juce::Vector3D<double> cartesian) const;
    juce::String getCoordinateDisplayText(const juce::Vector3D<double>& vector, bool isValid) const;  // New method for coordinate display
    void onMovementTypeChanged();

    // Clip editors have no per-control change notification, so the preview is kept live by
    // polling the controls instead of wiring ~12 individual callbacks - see ClipPreviewComponent.
    MovementClip buildClipFromControls();
    void timerCallback() override { preview.setMovementClip(buildClipFromControls()); }
};
