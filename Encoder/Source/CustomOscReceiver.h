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
                // set buffer values to 0
                memset(buffer, 0, sizeof(buffer));
                
                // setter
                setMethod ("setXYZ", setXYZ);
                setMethod ("setXYZbyName", setXYZbyName);
                setMethod ("setAED", setAED);
                setMethod ("setAEDbyName", setAEDbyName);
                
                // getter
                setMethod ("getX", x);
                setMethod ("getY", y);
                setMethod ("getZ", z);
                setMethod ("getA", a);
                setMethod ("getE", e);
                setMethod ("getD", d);
                setMethod ("getName", name);
                
                // group functions
                setMethod ("rotateGroup", rotateGroup);
                setMethod ("rotateGroupByName", rotateGroupByName);
                setMethod ("rotateGroupAroundOrigin", rotateOrigin);
                setMethod ("rotateGroupAroundOriginByName", rotateOriginByName);
                setMethod ("stretchGroup", stretch);
                setMethod ("stretchGroupByName", stretchByName);
                setMethod ("setGroupXYZ", setGroupXYZ);
                setMethod ("setGroupXYZbyName", setGroupXYZbyName);
                setMethod ("setGroupAED", setGroupAED);
                setMethod ("setGroupAEDbyName", setGroupAEDbyName);
                
                // per-receiver buffer
                setMethod ("getBufferValue", getBuf);
                setMethod ("setBufferValue", setBuf);
                
                // OSC message functions
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
            
            // access to buffer
            static var getBuf(const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments == 1)
                    {
                        int index = args.arguments[0];
                        if(index < thisObject->buffer_size && index >= 0)
                        {
                            return thisObject->buffer[index];
                        }
                    }
                }
                
                return var::undefined();
            }
            static var setBuf(const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments == 2)
                    {
                        int index = args.arguments[0];
                        if(index < thisObject->buffer_size && index >= 0)
                        {
                            thisObject->buffer[index] = args.arguments[1];
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
            
            static var setGroupXYZ (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments >= 4)
                    {
                        int index = args.arguments[0];
                        index--;    // make 0-based
                        double x = args.arguments[1];
                        double y = args.arguments[2];
                        double z = args.arguments[3];
                    
                        // optional move sub elements flag
                        bool moveSub = false;
                        if(args.numArguments >= 5)
                            moveSub = args.arguments[4];
                        
                        thisObject->jsAmbiSourceSet->setGroupXyz(index, x, y, z, moveSub);
                    }
                }
                
                return var::undefined();
            }
            
            static var setGroupXYZbyName (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments >= 4)
                    {
                        String name = args.arguments[0];
                        double x = args.arguments[1];
                        double y = args.arguments[2];
                        double z = args.arguments[3];
                    
                        // optional move sub elements flag
                        bool moveSub = false;
                        if(args.numArguments >= 5)
                            moveSub = args.arguments[4];
                        
                        thisObject->jsAmbiSourceSet->setGroupXyz(name, x, y, z, moveSub);
                    }
                }
                
                return var::undefined();
            }
            
            
            static var setGroupAED (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments >= 4)
                    {
                        int index = args.arguments[0];
                        index--;    // make 0-based
                        double a = args.arguments[1];
                        double e = args.arguments[2];
                        double d = args.arguments[3];
                    
                        // optional move sub elements flag
                        bool moveSub = false;
                        if(args.numArguments >= 5)
                            moveSub = args.arguments[4];
                        
                        thisObject->jsAmbiSourceSet->setGroupAed(index, a, e, d, moveSub);
                    }
                }
                
                return var::undefined();
            }
            
            static var setGroupAEDbyName (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments >= 4)
                    {
                        String name = args.arguments[0];
                        double a = args.arguments[1];
                        double e = args.arguments[2];
                        double d = args.arguments[3];
                    
                        // optional move sub elements flag
                        bool moveSub = false;
                        if(args.numArguments >= 5)
                            moveSub = args.arguments[4];
                        
                        thisObject->jsAmbiSourceSet->setGroupAed(name, a, e, d, moveSub);
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
            
            static var rotateGroup (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments == 4)
                    {
                        int index = args.arguments[0];
                        double x = (double)args.arguments[1] / 180.0 * PI;
                        double y = (double)args.arguments[2] / 180.0 * PI;
                        double z = (double)args.arguments[3] / 180.0 * PI;
                    
                        if(index > 0)
                            thisObject->jsAmbiSourceSet->rotateGroup(index - 1, x, y, z); // make index 0-based
                    }
                }
                
                return var::undefined();
            }
            
            static var rotateGroupByName (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments == 4)
                    {
                        String name = args.arguments[0];
                        double x = (double)args.arguments[1] / 180.0 * PI;
                        double y = (double)args.arguments[2] / 180.0 * PI;
                        double z = (double)args.arguments[3] / 180.0 * PI;
                    
                        thisObject->jsAmbiSourceSet->rotateGroup(name, x, y, z);
                    }
                }
                
                return var::undefined();
            }
            
            static var rotateOrigin (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments >= 4)
                    {
                        int index = args.arguments[0];
                        double x = (double)args.arguments[1] / 180.0 * PI;
                        double y = (double)args.arguments[2] / 180.0 * PI;
                        double z = (double)args.arguments[3] / 180.0 * PI;
                        
                        // optional move sub elements flag
                        bool moveSub = false;
                        if(args.numArguments >= 5)
                            moveSub = args.arguments[4];
                        
                        if(index > 0)
                            thisObject->jsAmbiSourceSet->rotateGroupAroundOrigin(index - 1, x, y, z, moveSub); // make index 0-based
                    }
                }
                
                return var::undefined();
            }
            
            static var rotateOriginByName (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments >= 4)
                    {
                        String name = args.arguments[0];
                        double x = (double)args.arguments[1] / 180.0 * PI;
                        double y = (double)args.arguments[2] / 180.0 * PI;
                        double z = (double)args.arguments[3] / 180.0 * PI;
                        
                        // optional move sub elements flag
                        bool moveSub = false;
                        if(args.numArguments >= 5)
                            moveSub = args.arguments[4];
                        
                        thisObject->jsAmbiSourceSet->rotateGroupAroundOrigin(name, x, y, z, moveSub);
                    }
                }
                
                return var::undefined();
            }
            
            static var stretch (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments == 2)
                    {
                        int index = args.arguments[0];
                        double stretch = args.arguments[1];
                        
                        if(index > 0)
                            thisObject->jsAmbiSourceSet->stretchGroup(index - 1, stretch); // make index 0-based
                    }
                }
                
                return var::undefined();
            }
            
            static var stretchByName (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments == 2)
                    {
                        String name = args.arguments[0];
                        double stretch = args.arguments[1];
                        
                        thisObject->jsAmbiSourceSet->stretchGroup(name, stretch);
                    }
                }
                
                return var::undefined();
            }
            
            CustomOscReceiver* owner;
            AmbiSourceSet* jsAmbiSourceSet;
            Array<var> jsPathElements;
            Array<var> jsArguments;
            int jsPointIndex;
            static const int buffer_size = 1000;
            float buffer[buffer_size];
            
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JsContext)
        };
    
    String jsExpression;
    std::unique_ptr<JavascriptEngine> jsEngine;
    JsContext* jsContext;
};
