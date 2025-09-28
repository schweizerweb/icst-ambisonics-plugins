#pragma once
#include "../../Common/Point3D.h"

typedef int ms_t;

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
    Point3D<double> startPointGroup;
    Point3D<double> endPointGroup;
    bool useStartPoint;
    Array<Point3D<double>> startPointsRel;
    Array<Point3D<double>> endPointsRel;
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
    AbsolutePerSecond
};

struct ActionDefinition
{
    ActionType action;
    TimingType timing;
    double value;
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
            xClip->setAttribute("startPointGroupX", c.startPointGroup.getX());
            xClip->setAttribute("startPointGroupY", c.startPointGroup.getY());
            xClip->setAttribute("startPointGroupZ", c.startPointGroup.getZ());
            xClip->setAttribute("endPointGroupX", c.endPointGroup.getX());
            xClip->setAttribute("endPointGroupY", c.endPointGroup.getY());
            xClip->setAttribute("endPointGroupZ", c.endPointGroup.getZ());
            xClip->setAttribute("useStartPoint", c.useStartPoint ? 1 : 0);
            
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
            
            // Serialize endPointsRel array
            auto* xEndPoints = new juce::XmlElement("EndPointsRel");
            for (const auto& point : c.endPointsRel)
            {
                auto* xPoint = new juce::XmlElement("Point");
                xPoint->setAttribute("x", point.getX());
                xPoint->setAttribute("y", point.getY());
                xPoint->setAttribute("z", point.getZ());
                xEndPoints->addChildElement(xPoint);
            }
            xClip->addChildElement(xEndPoints);
            
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
                xAction->setAttribute("actionType", static_cast<int>(action.action));
                xAction->setAttribute("timingType", static_cast<int>(action.timing));
                xAction->setAttribute("value", action.value);
                xActions->addChildElement(xAction);
            }
            xClip->addChildElement(xActions);
            
            xActionLayer->addChildElement(xClip);
        }
        xml->addChildElement(xActionLayer);
    
        return xml;
    }

    // Fixed XML deserialization
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
                    c.startPointGroup.setXYZ(
                                             xClip->getDoubleAttribute("startPointGroupX", 0.0),
                                             xClip->getDoubleAttribute("startPointGroupY", 0.0),
                                             xClip->getDoubleAttribute("startPointGroupZ", 0.0));
                    c.endPointGroup.setXYZ(
                                           xClip->getDoubleAttribute("endPointGroupX", 0.0),
                                           xClip->getDoubleAttribute("endPointGroupY", 0.0),
                                           xClip->getDoubleAttribute("endPointGroupZ", 0.0));
                    c.useStartPoint = xClip->getBoolAttribute("useStartPoint", false);

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

                    // Deserialize endPointsRel array
                    if (auto* xEndPoints = xClip->getChildByName("EndPointsRel"))
                    {
                        for (auto* xPoint = xEndPoints->getFirstChildElement(); xPoint != nullptr; xPoint = xPoint->getNextElement())
                        {
                            if (xPoint->hasTagName("Point"))
                            {
                                Point3D<double> point(
                                                      xPoint->getDoubleAttribute("x", 0.0),
                                                      xPoint->getDoubleAttribute("y", 0.0),
                                                      xPoint->getDoubleAttribute("z", 0.0));
                                c.endPointsRel.add(point);
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
                                action.action = static_cast<ActionType>(xAction->getIntAttribute("actionType", 0));
                                action.timing = static_cast<TimingType>(xAction->getIntAttribute("timingType", 0));
                                action.value = xAction->getDoubleAttribute("value", 0.0);
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
