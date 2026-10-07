#pragma once
#include "../../Common/Point3D.h"
#include "TimelineTypes.h"

// Only ever APPEND to this enum - the value is persisted as a raw int in XML, so reordering would
// silently reinterpret existing projects. RandomWalk must stay last (the XML reader clamps against it).
enum class MovementType
{
    MoveToCartesian,
    MoveToPolar,
    Circle,
    Spiral,
    Spline,
    Polygon,
    Lissajous,
    Rose,
    Helix,
    RandomWalk
};

// Single source of truth for the user-visible type name - used by the editor's combo box and the
// timeline's clip tooltip, so they can't drift apart.
inline juce::String movementTypeToString(MovementType type)
{
    switch (type)
    {
        case MovementType::MoveToCartesian: return "MoveTo (Cartesian)";
        case MovementType::MoveToPolar:     return "MoveTo (Polar)";
        case MovementType::Circle:          return "Circle";
        case MovementType::Spiral:          return "Spiral";
        case MovementType::Spline:          return "Spline";
        case MovementType::Polygon:         return "Polygon";
        case MovementType::Lissajous:       return "Lissajous";
        case MovementType::Rose:            return "Rose";
        case MovementType::Helix:           return "Helix";
        case MovementType::RandomWalk:      return "Random Walk";
    }
    return "Unknown";
}

// Types whose targetPointGroup is the CENTRE of a figure rather than a destination to travel to -
// drives both the "Center X/Y/Z" vs "Target X/Y/Z" labelling and which types read startOffset.
inline bool movementTypeUsesTargetAsCentre(MovementType type)
{
    return type == MovementType::Circle || type == MovementType::Spiral
        || type == MovementType::Lissajous || type == MovementType::Rose
        || type == MovementType::Helix || type == MovementType::RandomWalk;
}

// Types driven by the clip's waypoint list rather than by start/target points.
inline bool movementTypeUsesWaypoints(MovementType type)
{
    return type == MovementType::Spline || type == MovementType::Polygon;
}

// Open-ended paths that end somewhere other than where they started, so repeating them without
// Palindrome snaps visibly at every repeat boundary. Circle/Spiral/Lissajous/Rose/Helix are left
// out deliberately: the first four genuinely close at integer parameters, and Spiral has never been
// constrained despite ending at a different radius - staying consistent with that precedent.
inline bool movementTypeRequiresPalindromeForRepeat(MovementType type)
{
    return type == MovementType::MoveToCartesian || type == MovementType::MoveToPolar
        || type == MovementType::Spline || type == MovementType::Polygon
        || type == MovementType::RandomWalk;
}

// One point on a Spline/Polygon path. Deliberately a plain struct rather than Point3D: Point3D's
// operator=/operator== only accept a non-const lvalue (Common/Point3D.h), which is what forces the
// assignment workarounds elsewhere in the Animator - and juce::Array::operator== compares through
// const refs, so a non-const operator== here would reproduce exactly that trap.
struct MovementWaypoint
{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    bool startsNewSegment = false; // begins a new disjoint sub-path, jumped to instantly

    bool operator==(const MovementWaypoint& other) const
    {
        return juce::exactlyEqual(x, other.x)
            && juce::exactlyEqual(y, other.y)
            && juce::exactlyEqual(z, other.z)
            && startsNewSegment == other.startsNewSegment;
    }
    bool operator!=(const MovementWaypoint& other) const { return !(*this == other); }
};

struct Clip
{
    juce::String id = "";
    ms_t start = 0;      // ms
    ms_t length = 250;    // ms
    juce::Colour colour = juce::Colours::cornflowerblue;
    bool muted = false;  // excluded from playback, but keeps its data - toggled via the clip editor or the 'M' key
    bool palindrome = false;  // play the clip's progress curve forward then backward instead of just forward
    int repetitions = 1;      // number of forward (or forward+backward, if palindrome) cycles compressed into this clip's existing length - orthogonal to MovementClip::count

    // Speed curve: a cubic Bezier mapping elapsed time to distance covered, so a clip can accelerate
    // out of its start or decelerate into its target instead of running at a constant rate. The
    // defaults are the exact diagonal (y == x), so an untouched clip is a true no-op and every
    // project predating this feature plays exactly as before. See AnimatorMath::EasingCurve, which
    // these mirror, for the guarantees that come from clamping the handles to [0,1].
    bool easingEnabled = false;
    double easeX1 = 1.0 / 3.0, easeY1 = 1.0 / 3.0;
    double easeX2 = 2.0 / 3.0, easeY2 = 2.0 / 3.0;
    // true: the curve shapes each repetition identically. false: it warps time across the whole
    // clip, so the repetitions themselves speed up/slow down. Meaningless at repetitions == 1
    // without palindrome, where the editor hides the control.
    bool easePerRepetition = true;
    // Palindrome only: true shapes each LEG of the out-and-back separately (so both the outward and
    // the return journey ease), false stretches one curve across the whole go-and-back, which moves
    // the turnaround off the temporal midpoint. Ignored when palindrome is off.
    bool easePerDirection = true;

    ms_t end() const { return start + length; }

    // Used by the clip editors' dirty detection (is the clip being edited different from what's
    // actually stored). Subclasses extend this with their own fields - see MovementClip/ActionClip.
    bool operator==(const Clip& other) const
    {
        return id == other.id && start == other.start && length == other.length &&
               colour == other.colour && muted == other.muted &&
               palindrome == other.palindrome && repetitions == other.repetitions &&
               easingEnabled == other.easingEnabled &&
               juce::exactlyEqual(easeX1, other.easeX1) && juce::exactlyEqual(easeY1, other.easeY1) &&
               juce::exactlyEqual(easeX2, other.easeX2) && juce::exactlyEqual(easeY2, other.easeY2) &&
               easePerRepetition == other.easePerRepetition &&
               easePerDirection == other.easePerDirection;
    }
    bool operator!=(const Clip& other) const { return !(*this == other); }
};

// The Clip base's own XML, shared by MovementClip and ActionClip. Deliberately factored out rather
// than copy-pasted into each: the two clip types previously repeated this block verbatim, so adding
// a base field meant editing four places, and missing one would leave (say) action clips silently
// losing it on reload while movement clips kept it.
inline void writeClipBase(juce::XmlElement& x, const Clip& c)
{
    x.setAttribute("id", c.id);
    x.setAttribute("start", juce::String((juce::int64)c.start));
    x.setAttribute("length", juce::String((juce::int64)c.length));
    x.setAttribute("colour", juce::String::toHexString((juce::uint32)c.colour.getARGB()).paddedLeft('0', 8));
    x.setAttribute("muted", c.muted ? 1 : 0);
    x.setAttribute("palindrome", c.palindrome ? 1 : 0);
    x.setAttribute("repetitions", c.repetitions);

    x.setAttribute("easingEnabled", c.easingEnabled ? 1 : 0);
    x.setAttribute("easeX1", c.easeX1);
    x.setAttribute("easeY1", c.easeY1);
    x.setAttribute("easeX2", c.easeX2);
    x.setAttribute("easeY2", c.easeY2);
    x.setAttribute("easePerRepetition", c.easePerRepetition ? 1 : 0);
    x.setAttribute("easePerDirection", c.easePerDirection ? 1 : 0);
}

inline void readClipBase(Clip& c, const juce::XmlElement& x)
{
    c.id = x.getStringAttribute("id");
    c.start = (ms_t)x.getStringAttribute("start").getLargeIntValue();
    c.length = (ms_t)x.getStringAttribute("length").getLargeIntValue();

    const auto colourHex = x.getStringAttribute("colour");
    c.colour = colourHex.isNotEmpty()
             ? juce::Colour((juce::uint32)colourHex.getHexValue32())
             : juce::Colours::cornflowerblue;
    c.muted = x.getBoolAttribute("muted", false);
    c.palindrome = x.getBoolAttribute("palindrome", false);
    c.repetitions = juce::jmax(1, x.getIntAttribute("repetitions", 1));

    // Missing attributes fall back to the exact-diagonal defaults, so a file written before speed
    // curves existed loads as an unmodified linear clip. The handles are clamped here and not only
    // in the editor: a hand-edited file must not be able to make time run backwards.
    c.easingEnabled = x.getBoolAttribute("easingEnabled", false);
    c.easeX1 = juce::jlimit(0.0, 1.0, x.getDoubleAttribute("easeX1", 1.0 / 3.0));
    c.easeY1 = juce::jlimit(0.0, 1.0, x.getDoubleAttribute("easeY1", 1.0 / 3.0));
    c.easeX2 = juce::jlimit(0.0, 1.0, x.getDoubleAttribute("easeX2", 2.0 / 3.0));
    c.easeY2 = juce::jlimit(0.0, 1.0, x.getDoubleAttribute("easeY2", 2.0 / 3.0));
    c.easePerRepetition = x.getBoolAttribute("easePerRepetition", true);
    c.easePerDirection = x.getBoolAttribute("easePerDirection", true);
}

struct MovementClip : public Clip
{
    MovementType movementType = MovementType::MoveToCartesian;
    Point3D<double> startPointGroup; // is used as start position of the group point, if useStartFlag = true, otherwise just start at the current position (wherever that is)
    Point3D<double> targetPointGroup; // is used depending on the movementType:
        // MoveToCartesian/MoveToPolar: the target position, where the group point shall be at the end of the clip, since the data is always stored as XYZ in the background, there's no difference between the two MoveTo-Types regarding the targetPoint, but the movement between will be different.
        // Circle/Spiral: The center point, around which the group point will rotate.
    
    bool useStartPoint = false;

    double count = 1.0;           // Number of full rounds/cycles - Circle, Spiral, Helix, Lissajous, Rose, RandomWalk
    double radiusChange = 0.0;    // Spiral and Helix: absolute radius change per round

    // Spline/Polygon: the path's own points. Empty for every other type.
    juce::Array<MovementWaypoint> waypoints;

    double tension = 0.0;         // Spline only: 0 = roundest Catmull-Rom, 1 = straight (matches Polygon)
    // Spline/Polygon: join the last waypoint of each sub-path back to its first, so the figure is a
    // closed loop. A closed path ends where it began, which is also what lets it repeat without
    // Palindrome - see movementClipRequiresPalindromeForRepeat().
    bool closedPath = false;
    double heightRise = 0.0;      // Helix only: total Z travel over the clip (a distance - rescaled on import)
    double freqRatioA = 3.0;      // Lissajous: X frequency. Rose: petal count k
    double freqRatioB = 2.0;      // Lissajous only: Y frequency
    double phaseDeg = 0.0;        // Lissajous and Rose: phase offset in degrees
    int randomSeed = 1;           // RandomWalk only: reproducible noise seed

    // Point3D's own operator== is not const-qualified (can't be called on a const Point3D), so its
    // coordinates are compared directly via their (const-qualified) getters instead.
    bool operator==(const MovementClip& other) const
    {
        return static_cast<const Clip&>(*this) == static_cast<const Clip&>(other) &&
               movementType == other.movementType &&
               juce::exactlyEqual(startPointGroup.getX(), other.startPointGroup.getX()) &&
               juce::exactlyEqual(startPointGroup.getY(), other.startPointGroup.getY()) &&
               juce::exactlyEqual(startPointGroup.getZ(), other.startPointGroup.getZ()) &&
               juce::exactlyEqual(targetPointGroup.getX(), other.targetPointGroup.getX()) &&
               juce::exactlyEqual(targetPointGroup.getY(), other.targetPointGroup.getY()) &&
               juce::exactlyEqual(targetPointGroup.getZ(), other.targetPointGroup.getZ()) &&
               useStartPoint == other.useStartPoint &&
               juce::exactlyEqual(count, other.count) &&
               juce::exactlyEqual(radiusChange, other.radiusChange) &&
               waypoints == other.waypoints &&
               juce::exactlyEqual(tension, other.tension) &&
               closedPath == other.closedPath &&
               juce::exactlyEqual(heightRise, other.heightRise) &&
               juce::exactlyEqual(freqRatioA, other.freqRatioA) &&
               juce::exactlyEqual(freqRatioB, other.freqRatioB) &&
               juce::exactlyEqual(phaseDeg, other.phaseDeg) &&
               randomSeed == other.randomSeed;
    }
    bool operator!=(const MovementClip& other) const { return !(*this == other); }
};

// Whether THIS clip would jump at a repeat boundary, as opposed to whether its type generally
// would. A closed Spline/Polygon finishes on the waypoint it started from, so it can repeat
// perfectly well without Palindrome - which the type-only rule above cannot express.
inline bool movementClipRequiresPalindromeForRepeat(const MovementClip& clip)
{
    if (movementTypeUsesWaypoints(clip.movementType) && clip.closedPath)
        return false;

    return movementTypeRequiresPalindromeForRepeat(clip.movementType);
}

struct MovementLayer
{
    Array<MovementClip> clips;
};

enum class ActionType
{
    None,
    RotationX,
    RotationY,
    RotationZ,
    Stretch,
    Jitter
};

enum class TimingType
{
    None,
    AbsoluteTarget,
    RelativeDuringClip,
    ConstantPerSecond
};

class ActionDefinition
{
    ActionType action;
    TimingType timing;
    double value;
    double startValue;
    bool useStartValue;
    double jitterSpeed;

public:
    ActionDefinition(ActionType action_ = ActionType::None,
                    TimingType timing_ = TimingType::None,
                    double value_ = 0.0,
                    double startValue_ = 0.0,
                    bool useStartValue_ = false,
                    double jitterSpeed_ = 1.0)
        : action(action_), timing(timing_), value(value_),
          startValue(startValue_), useStartValue(useStartValue_), jitterSpeed(jitterSpeed_) {}

    // Value equality of every field - used by ActionClip::operator== (via juce::Array<ActionDefinition>'s
    // own element-wise operator==) for clip-editor dirty detection.
    bool operator==(const ActionDefinition& other) const
    {
        return action == other.action && timing == other.timing &&
               juce::exactlyEqual(value, other.value) && juce::exactlyEqual(startValue, other.startValue) &&
               useStartValue == other.useStartValue && juce::exactlyEqual(jitterSpeed, other.jitterSpeed);
    }
    bool operator!=(const ActionDefinition& other) const { return !(*this == other); }

    // 1. Get unit based on action type
    std::string getUnit(bool verbose = false) const
    {
        switch (action)
        {
            case ActionType::RotationX:
            case ActionType::RotationY:
            case ActionType::RotationZ:
                return "°";  // degrees
            case ActionType::Stretch:
                return verbose?"factor":"";     // unitless (ratio or factor)
            case ActionType::Jitter:
            case ActionType::None:
            default:
                return "";   // unitless (same scene-position units used everywhere else, no suffix)
        }
    }

    // 2. Get unit with timing consideration
    std::string getUnitWithTiming(bool verbose = false) const
    {
        // Jitter ignores TimingType entirely (it's continuous for the whole clip, with its own
        // separate Speed parameter) so it never gets the "/s" rate suffix.
        if (action == ActionType::Jitter)
            return getUnit(verbose);

        std::string unit = getUnit(verbose);

        if (timing == TimingType::ConstantPerSecond && !unit.empty())
        {
            return unit + "/s";
        }

        return unit;
    }
    
    // 3. Get descriptive string of all settings, as a natural-language sentence rather than a
    // symbol-and-unit soup - e.g. "Increase stretch factor by 0.3 per second, starting at 1".
    std::string getDescription() const
    {
        if (action == ActionType::None)
            return "No action";

        auto fmt = [](double v)
        {
            std::string s = std::to_string(v);
            s.erase(s.find_last_not_of('0') + 1, std::string::npos);
            if (!s.empty() && s.back() == '.') s.pop_back();
            return s;
        };

        // Jitter ignores TimingType/startValue entirely - it's continuous random wobble for the
        // whole clip, described purely by its own Intensity/Speed pair.
        if (action == ActionType::Jitter)
            return "Jitter \xC2\xB1" + fmt(value) + " at speed " + fmt(jitterSpeed);

        const bool isRotation = (action == ActionType::RotationX || action == ActionType::RotationY || action == ActionType::RotationZ);
        const std::string axis = (action == ActionType::RotationX) ? "X" : (action == ActionType::RotationY) ? "Y" : "Z";

        // Degrees matter for rotation and are shown on every value; a stretch factor is already
        // named by the noun below, so repeating "factor" after each number would just be noise.
        const std::string unit = isRotation ? "\xC2\xB0" : "";
        const std::string noun = isRotation ? (axis + " rotation") : "stretch factor";
        const std::string toVerb = isRotation ? ("Rotate " + axis + " to") : "Set stretch factor to";

        auto withUnit = [&](double v) { return fmt(v) + unit; };

        switch (timing)
        {
            case TimingType::None:
                return noun + ": inactive (no timing selected)";

            case TimingType::AbsoluteTarget:
            {
                std::string result = toVerb + " " + withUnit(value);
                if (useStartValue)
                    result += ", starting from " + withUnit(startValue);
                return result;
            }

            case TimingType::RelativeDuringClip:
            {
                const bool increase = value >= 0.0;
                std::string result = std::string(increase ? "Increase " : "Decrease ") + noun + " by " + withUnit(std::abs(value)) + " over the clip";
                if (useStartValue)
                    result += ", starting at " + withUnit(startValue);
                return result;
            }

            case TimingType::ConstantPerSecond:
            {
                const bool increase = value >= 0.0;
                std::string result = std::string(increase ? "Increase " : "Decrease ") + noun + " by " + withUnit(std::abs(value)) + " per second";
                if (useStartValue)
                    result += ", starting at " + withUnit(startValue);
                return result;
            }
        }

        return noun;
    }
    
    // Getters and setters
    ActionType getAction() const { return action; }
    void setAction(ActionType newAction) { action = newAction; }
    
    TimingType getTiming() const { return timing; }
    void setTiming(TimingType newTiming) { timing = newTiming; }
    
    double getValue() const { return value; }
    void setValue(double newValue) { value = newValue; }
    
    double getStartValue() const { return startValue; }
    void setStartValue(double newStartValue) { startValue = newStartValue; }
    
    bool getUseStartValue() const { return useStartValue; }
    void setUseStartValue(bool newUseStartValue) { useStartValue = newUseStartValue; }

    double getJitterSpeed() const { return jitterSpeed; }
    void setJitterSpeed(double newJitterSpeed) { jitterSpeed = newJitterSpeed; }

    // Check if start value controls should be enabled
    bool shouldEnableStartValueControls() const
    {
        if (action == ActionType::Jitter)
            return false; // Jitter wobbles around its live position, there's no start value concept

        return timing == TimingType::AbsoluteTarget || timing == TimingType::RelativeDuringClip
            || timing == TimingType::ConstantPerSecond;
    }
};

struct ActionClip : public Clip
{
    Array<ActionDefinition> actions;

    bool operator==(const ActionClip& other) const
    {
        return static_cast<const Clip&>(*this) == static_cast<const Clip&>(other) &&
               actions == other.actions;
    }
    bool operator!=(const ActionClip& other) const { return !(*this == other); }
};

struct ActionLayer
{
    String name;
    Array<ActionClip> clips;
};

struct TimelineModel
{
    MovementLayer movement;
    ActionLayer actions;

    // Helper methods for compatibility with existing UI code
    int getNumLayers() const { return 2; } // Movement layer + action layers
    
    juce::String getLayerName(int layerIndex) const
    {
        if (layerIndex == 0) return "Movement";
        if (layerIndex == 1) return "Actions";
        return "Invalid";
    }
    
    int getNumClips(int layerIndex) const
    {
        if (layerIndex == 0) return movement.clips.size();
        if (layerIndex == 1) return actions.clips.size();
        return 0;
    }
    
    const Clip* getClip(int layerIndex, int clipIndex) const
    {
        if (layerIndex == 0) {
            if (clipIndex < movement.clips.size())
                return static_cast<const Clip*>(&movement.clips.getReference(clipIndex));
        } else if (layerIndex == 1) {
            if (clipIndex < actions.clips.size())
                return static_cast<const Clip*>(&actions.clips.getReference(clipIndex));
        }
        return nullptr;
    }
    
    Clip* getClip(int layerIndex, int clipIndex)
    {
        if (layerIndex == 0) {
            if (clipIndex < movement.clips.size())
                return static_cast<Clip*>(&movement.clips.getReference(clipIndex));
        } else if (layerIndex == 1) {
            if (clipIndex < actions.clips.size())
                return static_cast<Clip*>(&actions.clips.getReference(clipIndex));
        }
        return nullptr;
    }
    
    int findClipAt(int layerIndex, ms_t t) const
    {
        if (layerIndex == 0) {
            for (int i = 0; i < movement.clips.size(); ++i)
                if (t >= movement.clips[i].start && t <= movement.clips[i].end())
                    return i;
        } else if (layerIndex == 1) {
            for (int i = 0; i < actions.clips.size(); ++i)
                if (t >= actions.clips[i].start && t <= actions.clips[i].end())
                    return i;
        }
        return -1;
    }
    
    std::unique_ptr<juce::XmlElement> toXml() const
    {
        auto xml = std::make_unique<juce::XmlElement>("Timeline");

        // Serialize movement layer
        auto* xMovement = new juce::XmlElement("MovementLayer");
        for (const auto& c : movement.clips)
        {
            auto* xClip = new juce::XmlElement("MovementClip");
            writeClipBase(*xClip, c);

            // Serialize MovementClip specific data
            xClip->setAttribute("movementType", static_cast<int>(c.movementType));
            xClip->setAttribute("startPointGroupX", c.startPointGroup.getX());
            xClip->setAttribute("startPointGroupY", c.startPointGroup.getY());
            xClip->setAttribute("startPointGroupZ", c.startPointGroup.getZ());
            xClip->setAttribute("targetPointGroupX", c.targetPointGroup.getX());
            xClip->setAttribute("targetPointGroupY", c.targetPointGroup.getY());
            xClip->setAttribute("targetPointGroupZ", c.targetPointGroup.getZ());
            xClip->setAttribute("useStartPoint", c.useStartPoint ? 1 : 0);
            xClip->setAttribute("count", c.count);
            xClip->setAttribute("radiusChange", c.radiusChange);
            xClip->setAttribute("tension", c.tension);
            xClip->setAttribute("closedPath", c.closedPath ? 1 : 0);
            xClip->setAttribute("heightRise", c.heightRise);
            xClip->setAttribute("freqRatioA", c.freqRatioA);
            xClip->setAttribute("freqRatioB", c.freqRatioB);
            xClip->setAttribute("phaseDeg", c.phaseDeg);
            xClip->setAttribute("randomSeed", c.randomSeed);

            // Spline/Polygon waypoints - same child-element pattern as ActionClip's <Actions>, and
            // additive-safe for the same reason: an older file simply has no <Waypoints> child and
            // loads with an empty list.
            if (!c.waypoints.isEmpty())
            {
                auto* xWaypoints = new juce::XmlElement("Waypoints");
                for (const auto& wp : c.waypoints)
                {
                    auto* xWaypoint = new juce::XmlElement("Waypoint");
                    xWaypoint->setAttribute("x", wp.x);
                    xWaypoint->setAttribute("y", wp.y);
                    xWaypoint->setAttribute("z", wp.z);
                    xWaypoint->setAttribute("newSegment", wp.startsNewSegment ? 1 : 0);
                    xWaypoints->addChildElement(xWaypoint);
                }
                xClip->addChildElement(xWaypoints);
            }

            xMovement->addChildElement(xClip);
        }
        xml->addChildElement(xMovement);

        // Serialize action layer
        auto* xActionLayer = new juce::XmlElement("ActionLayer");
        xActionLayer->setAttribute("name", actions.name);
        
        for (const auto& c : actions.clips)
        {
            auto* xClip = new juce::XmlElement("ActionClip");
            writeClipBase(*xClip, c);

            // Serialize ActionClip actions
            auto* xActions = new juce::XmlElement("Actions");
            for (const auto& action : c.actions)
            {
                auto* xAction = new juce::XmlElement("Action");
                xAction->setAttribute("actionType", static_cast<int>(action.getAction()));
                xAction->setAttribute("timingType", static_cast<int>(action.getTiming()));
                xAction->setAttribute("value", action.getValue());
                xAction->setAttribute("startValue", action.getStartValue());
                xAction->setAttribute("useStartValue", action.getUseStartValue() ? 1 : 0);
                xAction->setAttribute("jitterSpeed", action.getJitterSpeed());
                xActions->addChildElement(xAction);
            }
            xClip->addChildElement(xActions);
            
            xActionLayer->addChildElement(xClip);
        }
        xml->addChildElement(xActionLayer);
    
        return xml;
    }

    bool fromXml(const juce::XmlElement& xml)
    {
        if (!xml.hasTagName("Timeline"))
            return false;

        movement.clips.clear();
        actions.clips.clear();

        for (auto* xLayer = xml.getFirstChildElement(); xLayer != nullptr; xLayer = xLayer->getNextElement())
        {
            if (xLayer->hasTagName("MovementLayer"))
            {
                for (auto* xClip = xLayer->getFirstChildElement(); xClip != nullptr; xClip = xClip->getNextElement())
                {
                    if (!xClip->hasTagName("MovementClip")) continue;

                    MovementClip c;
                    readClipBase(c, *xClip);

                    // Deserialize MovementClip specific data. The type is clamped to the range this
                    // build knows: a file written by a newer version would otherwise produce an
                    // invalid enum, which falls through calculatePosition()'s default case and
                    // teleports the group to the origin for the clip's whole duration.
                    c.movementType = static_cast<MovementType>(
                        juce::jlimit(0, static_cast<int>(MovementType::RandomWalk),
                                     xClip->getIntAttribute("movementType", 0)));
                    c.startPointGroup.setXYZ(
                                             xClip->getDoubleAttribute("startPointGroupX", 0.0),
                                             xClip->getDoubleAttribute("startPointGroupY", 0.0),
                                             xClip->getDoubleAttribute("startPointGroupZ", 0.0));
                    c.targetPointGroup.setXYZ(
                                           xClip->getDoubleAttribute("targetPointGroupX", 0.0),
                                           xClip->getDoubleAttribute("targetPointGroupY", 0.0),
                                           xClip->getDoubleAttribute("targetPointGroupZ", 0.0));
                    c.useStartPoint = xClip->getBoolAttribute("useStartPoint", false);
                    c.count = xClip->getDoubleAttribute("count", 1.0);
                    c.radiusChange = xClip->getDoubleAttribute("radiusChange", 0.0);
                    c.tension = juce::jlimit(0.0, 1.0, xClip->getDoubleAttribute("tension", 0.0));
                    c.closedPath = xClip->getBoolAttribute("closedPath", false);
                    c.heightRise = xClip->getDoubleAttribute("heightRise", 0.0);
                    c.freqRatioA = xClip->getDoubleAttribute("freqRatioA", 3.0);
                    c.freqRatioB = xClip->getDoubleAttribute("freqRatioB", 2.0);
                    c.phaseDeg = xClip->getDoubleAttribute("phaseDeg", 0.0);
                    c.randomSeed = xClip->getIntAttribute("randomSeed", 1);

                    if (auto* xWaypoints = xClip->getChildByName("Waypoints"))
                    {
                        for (auto* xWaypoint = xWaypoints->getFirstChildElement(); xWaypoint != nullptr; xWaypoint = xWaypoint->getNextElement())
                        {
                            if (!xWaypoint->hasTagName("Waypoint")) continue;

                            MovementWaypoint wp;
                            wp.x = xWaypoint->getDoubleAttribute("x", 0.0);
                            wp.y = xWaypoint->getDoubleAttribute("y", 0.0);
                            wp.z = xWaypoint->getDoubleAttribute("z", 0.0);
                            wp.startsNewSegment = xWaypoint->getBoolAttribute("newSegment", false);

                            // A hand-edited or corrupted file could carry inf/NaN, which would
                            // poison the arc-length table and every position derived from it.
                            if (std::isfinite(wp.x) && std::isfinite(wp.y) && std::isfinite(wp.z))
                                c.waypoints.add(wp);
                        }
                    }

                    movement.clips.add(c);
                }
            }
            else if (xLayer->hasTagName("ActionLayer"))
            {
                for (auto* xClip = xLayer->getFirstChildElement(); xClip != nullptr; xClip = xClip->getNextElement())
                {
                    if (!xClip->hasTagName("ActionClip")) continue;

                    ActionClip c;
                    readClipBase(c, *xClip);

                    // Deserialize ActionClip actions
                    if (auto* xActions = xClip->getChildByName("Actions"))
                    {
                        for (auto* xAction = xActions->getFirstChildElement(); xAction != nullptr; xAction = xAction->getNextElement())
                        {
                            if (xAction->hasTagName("Action"))
                            {
                                ActionDefinition action;
                                action.setAction(static_cast<ActionType>(xAction->getIntAttribute("actionType", 0)));
                                action.setTiming(static_cast<TimingType>(xAction->getIntAttribute("timingType", 0)));
                                action.setValue(xAction->getDoubleAttribute("value", 0.0));
                                action.setStartValue(xAction->getDoubleAttribute("startValue", 0.0));
                                action.setUseStartValue(xAction->getBoolAttribute("useStartValue", false));
                                action.setJitterSpeed(xAction->getDoubleAttribute("jitterSpeed", 1.0));
                                c.actions.add(action);
                            }
                        }
                    }

                    actions.clips.add(c);
                }
            }
        }

        return true;
    }
};

struct SelectedClip
{
    int timelineIndex = -1;
    int layerIndex = -1;
    int clipIndex = -1;
    bool isMovementClip = false;
    
    bool isValid() const { return timelineIndex >= 0 && layerIndex >= 0 && clipIndex >= 0; }
    bool equals(int t, int l, int c, bool m) const {
        return timelineIndex == t && layerIndex == l && clipIndex == c && isMovementClip == m;
    }
    bool equals(const SelectedClip& other) const {
        return equals(other.timelineIndex, other.layerIndex, other.clipIndex, other.isMovementClip);
    }
    void reset() { timelineIndex = -1; layerIndex = -1; clipIndex = -1; }
};

// Clipboard for cut/copy/paste
struct ClipboardData
{
    juce::OwnedArray<TimelineModel> timelineData;
    bool hasData = false;
    
    void clear()
    {
        timelineData.clear();
        hasData = false;
    }
};
