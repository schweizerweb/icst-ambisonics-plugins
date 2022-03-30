/*
  ==============================================================================

    CustomOscReceiver.h
    Created: 17 Jan 2022 10:21:03pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "CustomOscInput.h"
#include "CustomOscBase.h"
#include "../../Common/AmbiSourceSet.h"

class CustomOscReceiver : CustomOscBase
{
public:
    CustomOscReceiver(CustomOscInput* pInput, ScalingInfo* pScaling);
    
public:
    bool matchesPattern(OSCAddress* pAddress);
    bool handleMessage(AmbiSourceSet* pSources, const OSCMessage* pMessage);
    String getErrorMessage();
    
private:
    std::unique_ptr<OSCAddressPattern> patternToMatch;
    bool hasIndex;
    bool hasName;
    bool xyzMode;
    bool javaScriptMode;
    bool isValid;
    
    String errorMessage;
    
private:
    struct JsContext  : public DynamicObject
        {
            JsContext (CustomOscReceiver* demo) : owner (demo)
            {
                setMethod ("setXYZ", setXYZ);
                /*
                setMethod ("x", x);
                setMethod ("y", y);
                setMethod ("z", z);
                setMethod ("a", a);
                setMethod ("e", e);
                setMethod ("d", d);
                setMethod ("gain", gain);
                setMethod ("name", name);
                setMethod ("color", color);
                setMethod ("i", i);
                */
            }
    
            static Identifier getClassName()    { return "s"; }
    
            static var setXYZ (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments == 4)
                    {
                        int index = args.arguments[0];
                        double x = args.arguments[1];
                        double y = args.arguments[2];
                        double z = args.arguments[3];
                    
                        thisObject->jsAmbiSourceSet->setChannelXYZ(index, x, y, z);
                    }
                }
                
                return var::undefined();
            }
            
            static var x (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments == 1)
                    {
                        int index = args.arguments[0];
                        return thisObject->jsAmbiSourceSet->get(index)->getPoint()->getX();
                    }
                }
                
                return var::undefined();
            }
/*
            static var y (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                    return thisObject->jsAmbiPoint->getPoint()->getY();
            
                return var::undefined();
            }
               
            static var z (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                    return thisObject->jsAmbiPoint->getPoint()->getZ();
            
                return var::undefined();
            }
               
            static var a (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                    return thisObject->jsAmbiPoint->getPoint()->getAzimuth();
            
                return var::undefined();
            }
               
            static var e (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                    return thisObject->jsAmbiPoint->getPoint()->getElevation();
            
                return var::undefined();
            }
               
            static var d (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                    return thisObject->jsAmbiPoint->getPoint()->getDistance();
            
                return var::undefined();
            }
               
            static var gain (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                    return thisObject->jsAmbiPoint->getGain();
            
                return var::undefined();
            }
               
            static var name (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                    return thisObject->jsAmbiPoint->getName();
            
                return var::undefined();
            }
               
            static var color (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                    return thisObject->jsAmbiPoint->getColor().toDisplayString(true);
            
                return var::undefined();
            }
              
            static var i (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                    return thisObject->jsPointIndex + 1; // 0-based to 1-based
            
                return var::undefined();
            }
              */
            
            CustomOscReceiver* owner;
            AmbiSourceSet* jsAmbiSourceSet;
            int jsPointIndex;
            
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JsContext)
        };
    
    String jsExpression;
    std::unique_ptr<JavascriptEngine> jsEngine;
    JsContext* jsContext;
};
