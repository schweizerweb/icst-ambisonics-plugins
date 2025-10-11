#pragma once
#include "../../Common/Point3D.h"
#include "TimelineTypes.h"

// Add this enum before MovementClip definition
enum class MovementType
{
    MoveToCartesian,
    MoveToPolar,
    Circle,
    Spiral
};

struct Clip
{
    juce::String id = "";
    ms_t start = 0;      // ms
    ms_t length = 250;    // ms
    juce::Colour colour = juce::Colours::cornflowerblue;
    
    ms_t end() const { return start + length; }
};

struct MovementClip : public Clip
{
    MovementType movementType = MovementType::MoveToCartesian;
    Point3D<double> startPointGroup; // is used as start position of the group point, if useStartFlag = true, otherwise just start at the current position (wherever that is)
    Point3D<double> targetPointGroup; // is used depending on the movementType:
        // MoveToCartesian/MoveToPolar: the target position, where the group point shall be at the end of the clip, since the data is always stored as XYZ in the background, there's no difference between the two MoveTo-Types regarding the targetPoint, but the movement between will be different.
        // Circle/Spiral: The center point, around which the group point will rotate.
    
    bool useStartPoint;
    Array<Point3D<double>> startPointsRel; // to be implemented later
    Array<Point3D<double>> targetPointsRel; // to be implemented later
    
    double count = 1.0;           // For Circle and Spiral: number of full rounds
    double radiusChange = 0.0;    // For Spiral only: change of the radius per round, with which the group point will rotate around the target point. for now it's the absolute radius change per round.
};

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
    Stretch
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

public:
    ActionDefinition(ActionType action_ = ActionType::None,
                    TimingType timing_ = TimingType::None,
                    double value_ = 0.0,
                    double startValue_ = 0.0,
                    bool useStartValue_ = false)
        : action(action_), timing(timing_), value(value_),
          startValue(startValue_), useStartValue(useStartValue_) {}
    
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
            case ActionType::None:
            default:
                return "";
        }
    }
    
    // 2. Get unit with timing consideration
    std::string getUnitWithTiming(bool verbose = false) const
    {
        std::string unit = getUnit(verbose);
        
        if (timing == TimingType::ConstantPerSecond && !unit.empty())
        {
            return unit + "/s";
        }
        
        return unit;
    }
    
    // 3. Get descriptive string of all settings
    std::string getDescription() const
    {
        std::string actionStr;
        switch (action)
        {
            case ActionType::None: actionStr = "None"; break;
            case ActionType::RotationX: actionStr = "Rotation X"; break;
            case ActionType::RotationY: actionStr = "Rotation Y"; break;
            case ActionType::RotationZ: actionStr = "Rotation Z"; break;
            case ActionType::Stretch: actionStr = "Stretch"; break;
        }
        
        std::string timingSymbol;
        switch (timing)
        {
            case TimingType::None: timingSymbol = ""; break;
            case TimingType::AbsoluteTarget: timingSymbol = " →"; break;
            case TimingType::RelativeDuringClip: timingSymbol = " |▷|"; break;
            case TimingType::ConstantPerSecond: timingSymbol = ""; break;
        }
        
        std::string unit = getUnitWithTiming(true);
        std::string valueStr = std::to_string(value);
        
        // Remove trailing zeros for cleaner display
        valueStr.erase(valueStr.find_last_not_of('0') + 1, std::string::npos);
        if (valueStr.back() == '.') valueStr.pop_back();
        
        // Add start value info if used
        std::string startInfo = "";
        if (useStartValue)
        {
            std::string startStr = std::to_string(startValue);
            startStr.erase(startStr.find_last_not_of('0') + 1, std::string::npos);
            if (startStr.back() == '.') startStr.pop_back();
            startInfo = " from " + startStr + unit;
        }
        
        return actionStr + timingSymbol + " " + valueStr + (unit.empty() ? "" : " " + unit) + startInfo;
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
    
    // Check if start value controls should be enabled
    bool shouldEnableStartValueControls() const
    {
        return timing == TimingType::AbsoluteTarget || timing == TimingType::RelativeDuringClip;
    }
};

struct ActionClip : public Clip
{
    Array<ActionDefinition> actions;
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
            xClip->setAttribute("id", c.id);
            xClip->setAttribute("start", juce::String((juce::int64)c.start));
            xClip->setAttribute("length", juce::String((juce::int64)c.length));
            xClip->setAttribute("colour", juce::String::toHexString((juce::uint32)c.colour.getARGB()).paddedLeft('0', 8));
            
            // Serialize MovementClip specific data
            xClip->setAttribute("movementType", static_cast<int>(c.movementType));  // Add this
            xClip->setAttribute("startPointGroupX", c.startPointGroup.getX());
            xClip->setAttribute("startPointGroupY", c.startPointGroup.getY());
            xClip->setAttribute("startPointGroupZ", c.startPointGroup.getZ());
            xClip->setAttribute("targetPointGroupX", c.targetPointGroup.getX());
            xClip->setAttribute("targetPointGroupY", c.targetPointGroup.getY());
            xClip->setAttribute("targetPointGroupZ", c.targetPointGroup.getZ());
            xClip->setAttribute("useStartPoint", c.useStartPoint ? 1 : 0);
            xClip->setAttribute("count", c.count);
            xClip->setAttribute("radiusChange", c.radiusChange);
            
            // Serialize startPointsRel array
            auto* xStartPoints = new juce::XmlElement("StartPointsRel");
            for (const auto& point : c.startPointsRel)
            {
                auto* xPoint = new juce::XmlElement("Point");
                xPoint->setAttribute("x", point.getX());
                xPoint->setAttribute("y", point.getY());
                xPoint->setAttribute("z", point.getZ());
                xStartPoints->addChildElement(xPoint);
            }
            xClip->addChildElement(xStartPoints);
            
            // Serialize targetPointsRel array
            auto* xTargetPoints = new juce::XmlElement("TargetPointsRel");
            for (const auto& point : c.targetPointsRel)
            {
                auto* xPoint = new juce::XmlElement("Point");
                xPoint->setAttribute("x", point.getX());
                xPoint->setAttribute("y", point.getY());
                xPoint->setAttribute("z", point.getZ());
                xTargetPoints->addChildElement(xPoint);
            }
            xClip->addChildElement(xTargetPoints);
            
            xMovement->addChildElement(xClip);
        }
        xml->addChildElement(xMovement);

        // Serialize action layer
        auto* xActionLayer = new juce::XmlElement("ActionLayer");
        xActionLayer->setAttribute("name", actions.name);
        
        for (const auto& c : actions.clips)
        {
            auto* xClip = new juce::XmlElement("ActionClip");
            xClip->setAttribute("id", c.id);
            xClip->setAttribute("start", juce::String((juce::int64)c.start));
            xClip->setAttribute("length", juce::String((juce::int64)c.length));
            xClip->setAttribute("colour", juce::String::toHexString((juce::uint32)c.colour.getARGB()).paddedLeft('0', 8));
            
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
                    c.id = xClip->getStringAttribute("id");
                    c.start = (ms_t)xClip->getStringAttribute("start").getLargeIntValue();
                    c.length = (ms_t)xClip->getStringAttribute("length").getLargeIntValue();

                    const auto colourHex = xClip->getStringAttribute("colour");
                    c.colour = colourHex.isNotEmpty()
                             ? juce::Colour((juce::uint32)colourHex.getHexValue32())
                             : juce::Colours::cornflowerblue;

                    // Deserialize MovementClip specific data
                    c.movementType = static_cast<MovementType>(xClip->getIntAttribute("movementType", 0));  // Add this
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

                    // Deserialize startPointsRel array
                    if (auto* xStartPoints = xClip->getChildByName("StartPointsRel"))
                    {
                        for (auto* xPoint = xStartPoints->getFirstChildElement(); xPoint != nullptr; xPoint = xPoint->getNextElement())
                        {
                            if (xPoint->hasTagName("Point"))
                            {
                                Point3D<double> point(
                                                      xPoint->getDoubleAttribute("x", 0.0),
                                                      xPoint->getDoubleAttribute("y", 0.0),
                                                      xPoint->getDoubleAttribute("z", 0.0));
                                c.startPointsRel.add(point);
                            }
                        }
                    }

                    // Deserialize targetPointsRel array
                    if (auto* xTargetPoints = xClip->getChildByName("TargetPointsRel"))
                    {
                        for (auto* xPoint = xTargetPoints->getFirstChildElement(); xPoint != nullptr; xPoint = xPoint->getNextElement())
                        {
                            if (xPoint->hasTagName("Point"))
                            {
                                Point3D<double> point(
                                                      xPoint->getDoubleAttribute("x", 0.0),
                                                      xPoint->getDoubleAttribute("y", 0.0),
                                                      xPoint->getDoubleAttribute("z", 0.0));
                                c.targetPointsRel.add(point);
                            }
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
                    c.id = xClip->getStringAttribute("id");
                    c.start = (ms_t)xClip->getStringAttribute("start").getLargeIntValue();
                    c.length = (ms_t)xClip->getStringAttribute("length").getLargeIntValue();

                    const auto colourHex = xClip->getStringAttribute("colour");
                    c.colour = colourHex.isNotEmpty()
                             ? juce::Colour((juce::uint32)colourHex.getHexValue32())
                             : juce::Colours::cornflowerblue;

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
