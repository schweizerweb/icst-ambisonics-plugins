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
                setMethod ("setXYZbyName", setXYZbyName);
                setMethod ("setAED", setAED);
                setMethod ("setAEDbyName", setAEDbyName);
                setMethod ("x", x);
                setMethod ("y", y);
                setMethod ("z", z);
                setMethod ("a", a);
                setMethod ("e", e);
                setMethod ("d", d);
                setMethod ("name", name);
                
                setMethod ("path", path);
                setMethod ("arg", arg);
            }
    
            static Identifier getClassName()    { return "s"; }
    
            // access to osc path elements
            static var path(const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments == 1)
                    {
                        int index = args.arguments[0];
                        index--;    // make 0-based
                        if(index < thisObject->jsPathElements.size())
                        {
                            return thisObject->jsPathElements[index];
                        }
                    }
                }
                
                return var::undefined();
            }
            
            // access to osc arguments
            static var arg(const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments == 1)
                    {
                        int index = args.arguments[0];
                        index--;    // make 0-based
                        if(index < thisObject->jsArguments.size())
                        {
                            return thisObject->jsArguments[index];
                        }
                    }
                }
                
                return var::undefined();
            }
            
            static var setXYZ (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments == 4)
                    {
                        int index = args.arguments[0];
                        index--;    // make 0-based
                        double x = args.arguments[1];
                        double y = args.arguments[2];
                        double z = args.arguments[3];
                    
                        thisObject->jsAmbiSourceSet->setChannelXYZ(index, x, y, z);
                    }
                }
                
                return var::undefined();
            }
            
            static var setXYZbyName (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments == 4)
                    {
                        String name = args.arguments[0];
                        double x = args.arguments[1];
                        double y = args.arguments[2];
                        double z = args.arguments[3];
                    
                        thisObject->jsAmbiSourceSet->setChannelNameXYZ(name, x, y, z);
                    }
                }
                
                return var::undefined();
            }
            
            
            static var setAED (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments == 4)
                    {
                        int index = args.arguments[0];
                        index--;    // make 0-based
                        double a = args.arguments[1];
                        double e = args.arguments[2];
                        double d = args.arguments[3];
                    
                        thisObject->jsAmbiSourceSet->setChannelAED(index, a, e, d);
                    }
                }
                
                return var::undefined();
            }
            
            static var setAEDbyName (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments == 4)
                    {
                        String name = args.arguments[0];
                        double a = args.arguments[1];
                        double e = args.arguments[2];
                        double d = args.arguments[3];
                    
                        thisObject->jsAmbiSourceSet->setChannelNameAED(name, a, e, d);
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
                        auto p = thisObject->jsAmbiSourceSet->get(index - 1); // make 0-based
                        if(p != nullptr)
                            return p->getPoint()->getX();
                    }
                }
                
                return var::undefined();
            }

            static var y (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments == 1)
                    {
                        int index = args.arguments[0];
                        auto p = thisObject->jsAmbiSourceSet->get(index - 1); // make 0-based
                        if(p != nullptr)
                            return p->getPoint()->getY();
                    }
                }
                
                return var::undefined();
            }
               
            static var z (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments == 1)
                    {
                        int index = args.arguments[0];
                        auto p = thisObject->jsAmbiSourceSet->get(index - 1); // make 0-based
                        if(p != nullptr)
                            return p->getPoint()->getZ();
                    }
                }
                
                return var::undefined();
            }
               
            static var a (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments == 1)
                    {
                        int index = args.arguments[0];
                        auto p = thisObject->jsAmbiSourceSet->get(index - 1); // make 0-based
                        if(p != nullptr)
                            return p->getPoint()->getAzimuth();
                    }
                }
                
                return var::undefined();
            }
               
            static var e (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments == 1)
                    {
                        int index = args.arguments[0];
                        auto p = thisObject->jsAmbiSourceSet->get(index - 1); // make 0-based
                        if(p != nullptr)
                            return p->getPoint()->getElevation();
                    }
                }
                
                return var::undefined();
            }
               
            static var d (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments == 1)
                    {
                        int index = args.arguments[0];
                        auto p = thisObject->jsAmbiSourceSet->get(index - 1); // make 0-based
                        if(p != nullptr)
                            return p->getPoint()->getDistance();
                    }
                }
                
                return var::undefined();
            }
               
            static var name (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments == 1)
                    {
                        int index = args.arguments[0];
                        auto p = thisObject->jsAmbiSourceSet->get(index - 1); // make 0-based
                        if(p != nullptr)
                            return p->getName();
                    }
                }
                
                return var::undefined();
            }
            
            CustomOscReceiver* owner;
            AmbiSourceSet* jsAmbiSourceSet;
            Array<var> jsPathElements;
            Array<var> jsArguments;
            int jsPointIndex;
            
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JsContext)
        };
    
    String jsExpression;
    std::unique_ptr<JavascriptEngine> jsEngine;
    JsContext* jsContext;
};
