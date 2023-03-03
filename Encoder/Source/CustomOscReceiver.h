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
                
                // custom fail
                setMethod ("reportError", reportError);
                
                // setter
                setMethod ("setXYZ", setXYZ);
                setMethod ("setXYZbyName", setXYZbyName);
                setMethod ("setAED", setAED);
                setMethod ("setAEDbyName", setAEDbyName);
                
                // getter
                setMethod ("getX", getX);
                setMethod ("getY", getY);
                setMethod ("getZ", getZ);
                setMethod ("getA", getA);
                setMethod ("getE", getE);
                setMethod ("getD", getD);
                setMethod ("getName", getName);
                setMethod ("getAbsX", getAbsX);
                setMethod ("getAbsY", getAbsY);
                setMethod ("getAbsZ", getAbsZ);
                setMethod ("getGroupX", getGroupX);
                setMethod ("getGroupY", getGroupY);
                setMethod ("getGroupZ", getGroupZ);
                setMethod ("getGroupA", getGroupA);
                setMethod ("getGroupE", getGroupE);
                setMethod ("getGroupD", getGroupD);
                setMethod ("getGroupName", getGroupName);
                
                // group functions
                setMethod ("rotateGroup", rotateGroup);
                setMethod ("rotateGroupByName", rotateGroupByName);
                setMethod ("rotateGroupAroundOrigin", rotateGroupAroundOrigin);
                setMethod ("rotateGroupAroundOriginByName", rotateGroupAroundOriginByName);
                setMethod ("stretchGroup", stretchGroup);
                setMethod ("stretchGroupByName", stretchGroupByName);
                setMethod ("setGroupXYZ", setGroupXYZ);
                setMethod ("setGroupXYZbyName", setGroupXYZbyName);
                setMethod ("setGroupAED", setGroupAED);
                setMethod ("setGroupAEDbyName", setGroupAEDbyName);
                
                setMethod ("setGroupRotation", setGroupRotation);
                setMethod ("setGroupRotationByName", setGroupRotationByName);
                setMethod ("setGroupStretch", setGroupStretch);
                setMethod ("setGroupStretchByName", setGroupStretchByName);
                
                // per-receiver buffer
                setMethod ("getBufferValue", getBufferValue);
                setMethod ("setBufferValue", setBufferValue);
                
                // OSC message functions
                setMethod ("path", path);
                setMethod ("arg", arg);
            }
    
            static Identifier getClassName()    { return "s"; }
    
            // access to osc path elements
            static var path(const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("path", args, 1))
                {
                    int index = args.arguments[0];
                    if(index > 0 && index <= thisObject->jsPathElements.size())
                    {
                        return thisObject->jsPathElements[index-1]; // make index 0-based
                    }
                    else
                    {
                        thisObject->fail("path: invalid path index " + String(index));
                    }
                }
                
                return var::undefined();
            }
            
            // access to osc arguments
            static var arg(const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("arg", args, 1))
                {
                    int index = args.arguments[0];
                    if(index > 0 && index <= thisObject->jsArguments.size())
                    {
                        return thisObject->jsArguments[index-1]; // make index 0-based
                    }
                    else
                    {
                        thisObject->fail("arg: invalid arg index " + String(index));
                    }
                }
                
                return var::undefined();
            }
            
            // access to buffer
            static var getBufferValue(const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("getBufferValue", args, 1))
                {
                    int index = args.arguments[0];
                    if(index < thisObject->buffer_size && index >= 0)
                    {
                        return thisObject->buffer[index];
                    }
                    else
                    {
                        thisObject->fail("getBufferValue: invalid data index " + String(index));
                    }
                }
                
                return var::undefined();
            }
            
            static var setBufferValue(const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("setBufferValue", args, 2))
                {
                    int index = args.arguments[0];
                    if(index < thisObject->buffer_size && index >= 0)
                    {
                        thisObject->buffer[index] = args.arguments[1];
                    }
                    else
                    {
                        thisObject->fail("setBufferValue: invalid data index " + String(index));
                    }
                }
                
                return var::undefined();
            }
            
            static var setXYZ (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("setXYZ", args, 4))
                {
                    int index = args.arguments[0];
                    double x = args.arguments[1];
                    double y = args.arguments[2];
                    double z = args.arguments[3];
                    
                    if(thisObject->jsAmbiSourceSet->setChannelXYZ(index-1, x, y, z)) // make index 0-based
                    {
                        return true;
                    }
                    else
                    {
                        thisObject->fail("setXYZ: invalid channel index " + String(index));
                    }
                }
                
                return var::undefined();
            }
            
            static var setXYZbyName (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("setXYZbyName", args, 4))
                {
                    String name = args.arguments[0];
                    double x = args.arguments[1];
                    double y = args.arguments[2];
                    double z = args.arguments[3];
                    
                    if(thisObject->jsAmbiSourceSet->setChannelNameXYZ(name, x, y, z))
                    {
                        return true;
                    }
                    else
                    {
                        thisObject->fail("setXYZbyName: channel '" + name + "' not found");
                    }
                }
                
                return var::undefined();
            }
            
            
            static var setAED (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("setAED", args, 4))
                {
                    int index = args.arguments[0];
                    double a = args.arguments[1];
                    double e = args.arguments[2];
                    double d = args.arguments[3];
                    
                    if(thisObject->jsAmbiSourceSet->setChannelAED(index-1, a, e, d))
                    {
                        return true;
                    }
                    else
                    {
                        thisObject->fail("setAED: invalid channel index " + String(index));
                    }
                }
                
                return var::undefined();
            }
            
            static var setAEDbyName (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("setAEDbyName", args, 4))
                {
                    String name = args.arguments[0];
                    double a = args.arguments[1];
                    double e = args.arguments[2];
                    double d = args.arguments[3];
                    
                    if(thisObject->jsAmbiSourceSet->setChannelNameAED(name, a, e, d))
                    {
                        return true;
                    }
                    else
                    {
                        thisObject->fail("setAEDbyName: channel '" + name + "' not found");
                    }
                }
                
                return var::undefined();
            }
            
            static var setGroupXYZ (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("setGroupXYZ", args, 4, 5))
                {
                    int index = args.arguments[0];
                    double x = args.arguments[1];
                    double y = args.arguments[2];
                    double z = args.arguments[3];
                    
                    // optional move sub elements flag
                    bool moveSub = true;
                    if(args.numArguments >= 5)
                        moveSub = args.arguments[4];
                        
                    if(thisObject->jsAmbiSourceSet->setGroupXyz(index-1, x, y, z, moveSub))
                    {
                        return true;
                    }
                    else
                    {
                        thisObject->fail("setGroupXYZ: invalid group index " + String(index));
                    }
                    
                }
                
                return var::undefined();
            }
            
            static var setGroupXYZbyName (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("setGroupXYZbyName", args, 4, 5))
                {
                    String name = args.arguments[0];
                    double x = args.arguments[1];
                    double y = args.arguments[2];
                    double z = args.arguments[3];
                    
                    // optional move sub elements flag
                    bool moveSub = true;
                    if(args.numArguments >= 5)
                        moveSub = args.arguments[4];
                        
                    if(thisObject->jsAmbiSourceSet->setGroupXyz(name, x, y, z, moveSub))
                    {
                        return true;
                    }
                    else
                    {
                        thisObject->fail("setGroupXYZbyName: group '" + name + "' not found");
                    }
                }
                
                return var::undefined();
            }
            
            
            static var setGroupAED (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("setGroupAED", args, 4, 5))
                {
                    int index = args.arguments[0];
                    double a = args.arguments[1];
                    double e = args.arguments[2];
                    double d = args.arguments[3];
                    
                    // optional move sub elements flag
                    bool moveSub = true;
                    if(args.numArguments >= 5)
                        moveSub = args.arguments[4];
                        
                    if(thisObject->jsAmbiSourceSet->setGroupAed(index-1, a, e, d, moveSub))
                    {
                        return true;
                    }
                    else
                    {
                        thisObject->fail("setGroupAED: invalid group index " + String(index));
                    }
                }
                
                return var::undefined();
            }
            
            static var setGroupAEDbyName (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("setGroupAEDbyName", args, 4, 5))
                {
                    String name = args.arguments[0];
                    double a = args.arguments[1];
                    double e = args.arguments[2];
                    double d = args.arguments[3];
                    
                    // optional move sub elements flag
                    bool moveSub = true;
                    if(args.numArguments >= 5)
                        moveSub = args.arguments[4];
                        
                    if(thisObject->jsAmbiSourceSet->setGroupAed(name, a, e, d, moveSub))
                    {
                        return true;
                    }
                    else
                    {
                        thisObject->fail("setGroupAEDbyName: group '" + name + "' not found");
                    }
                }
                
                return var::undefined();
            }
            
            static var getX (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("getX", args, 1))
                {
                    int index = args.arguments[0];
                    auto p = thisObject->jsAmbiSourceSet->get(index - 1); // make 0-based
                    if(p != nullptr)
                    {
                        return p->getRawPoint()->getX();
                    }
                    else
                    {
                        thisObject->fail("getX: invalid source index " + String(index));
                    }
                }
                
                return var::undefined();
            }

            static var getY (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("getY", args, 1))
                {
                    int index = args.arguments[0];
                    auto p = thisObject->jsAmbiSourceSet->get(index - 1); // make 0-based
                    if(p != nullptr)
                    {
                        return p->getRawPoint()->getY();
                    }
                    else
                    {
                        thisObject->fail("getY: invalid source index " + String(index));
                    }
                }
                
                return var::undefined();
            }
               
            static var getZ (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("getZ", args, 1))
                {
                    int index = args.arguments[0];
                    auto p = thisObject->jsAmbiSourceSet->get(index - 1); // make 0-based
                    if(p != nullptr)
                    {
                        return p->getRawPoint()->getZ();
                    }
                    else
                    {
                        thisObject->fail("getZ: invalid source index " + String(index));
                    }
                }
                
                return var::undefined();
            }
               
            static var getAbsX (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("getAbsX", args, 1))
                {
                    int index = args.arguments[0];
                    if(thisObject->jsAmbiSourceSet->get(index-1) != nullptr)
                    {
                        return thisObject->jsAmbiSourceSet->getAbsSourcePoint(index-1).x;
                    }
                    else
                    {
                        thisObject->fail("getAbsX: invalid source index " + String(index));
                    }
                }
                
                return var::undefined();
            }

            static var getAbsY (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("getAbsY", args, 1))
                {
                    int index = args.arguments[0];
                    if(thisObject->jsAmbiSourceSet->get(index-1) != nullptr)
                    {
                        return thisObject->jsAmbiSourceSet->getAbsSourcePoint(index-1).y;
                    }
                    else
                    {
                        thisObject->fail("getAbsY: invalid source index " + String(index));
                    }
                }
                
                return var::undefined();
            }
               
            static var getAbsZ (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("getAbsZ", args, 1))
                {
                    int index = args.arguments[0];
                    if(thisObject->jsAmbiSourceSet->get(index-1) != nullptr)
                    {
                        return thisObject->jsAmbiSourceSet->getAbsSourcePoint(index-1).z;
                    }
                    else
                    {
                        thisObject->fail("getAbsZ: invalid source index " + String(index));
                    }
                }
                
                return var::undefined();
            }
            
            static var getA (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("getA", args, 1))
                {
                    int index = args.arguments[0];
                    auto p = thisObject->jsAmbiSourceSet->get(index - 1); // make 0-based
                    if(p != nullptr)
                    {
                        return p->getRawPoint()->getAzimuth();
                    }
                    else
                    {
                        thisObject->fail("getA: invalid source index " + String(index));
                    }
                }
                
                return var::undefined();
            }
               
            static var getE (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("getE", args, 1))
                {
                    int index = args.arguments[0];
                    auto p = thisObject->jsAmbiSourceSet->get(index - 1); // make 0-based
                    if(p != nullptr)
                    {
                        return p->getRawPoint()->getElevation();
                    }
                    else
                    {
                        thisObject->fail("getE: invalid source index " + String(index));
                    }
                }
                
                return var::undefined();
            }
               
            static var getD (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("getD", args, 1))
                {
                    int index = args.arguments[0];
                    auto p = thisObject->jsAmbiSourceSet->get(index - 1); // make 0-based
                    if(p != nullptr)
                    {
                        return p->getRawPoint()->getDistance();
                    }
                    else
                    {
                        thisObject->fail("getD: invalid source index " + String(index));
                    }
                }
                
                return var::undefined();
            }
               
            static var getName (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("getName", args, 1))
                {
                    int index = args.arguments[0];
                    auto p = thisObject->jsAmbiSourceSet->get(index - 1); // make 0-based
                    if(p != nullptr)
                    {
                        return p->getName();
                    }
                    else
                    {
                        thisObject->fail("getName: invalid source index " + String(index));
                    }
                }
                
                return var::undefined();
            }
            
            static var getGroupX (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("getGroupX", args, 1))
                {
                    int index = args.arguments[0];
                    auto p = thisObject->jsAmbiSourceSet->getGroup(index - 1); // make 0-based
                    if(p != nullptr)
                    {
                        return p->getRawPoint()->getX();
                    }
                    else
                    {
                        thisObject->fail("getGroupX: invalid source index " + String(index));
                    }
                }
                
                return var::undefined();
            }

            static var getGroupY (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("getGroupY", args, 1))
                {
                    int index = args.arguments[0];
                    auto p = thisObject->jsAmbiSourceSet->getGroup(index - 1); // make 0-based
                    if(p != nullptr)
                    {
                        return p->getRawPoint()->getY();
                    }
                    else
                    {
                        thisObject->fail("getGroupY: invalid source index " + String(index));
                    }
                }
                
                return var::undefined();
            }
               
            static var getGroupZ (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("getGroupZ", args, 1))
                {
                    int index = args.arguments[0];
                    auto p = thisObject->jsAmbiSourceSet->getGroup(index - 1); // make 0-based
                    if(p != nullptr)
                    {
                        return p->getRawPoint()->getZ();
                    }
                    else
                    {
                        thisObject->fail("getGroupZ: invalid source index " + String(index));
                    }
                }
                
                return var::undefined();
            }
               
            
            static var getGroupA (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("getGroupA", args, 1))
                {
                    int index = args.arguments[0];
                    auto p = thisObject->jsAmbiSourceSet->getGroup(index - 1); // make 0-based
                    if(p != nullptr)
                    {
                        return p->getRawPoint()->getAzimuth();
                    }
                    else
                    {
                        thisObject->fail("getGroupA: invalid source index " + String(index));
                    }
                }
                
                return var::undefined();
            }
               
            static var getGroupE (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("getGroupE", args, 1))
                {
                    int index = args.arguments[0];
                    auto p = thisObject->jsAmbiSourceSet->getGroup(index - 1); // make 0-based
                    if(p != nullptr)
                    {
                        return p->getRawPoint()->getElevation();
                    }
                    else
                    {
                        thisObject->fail("getGroupE: invalid source index " + String(index));
                    }
                }
                
                return var::undefined();
            }
               
            static var getGroupD (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("getGroupD", args, 1))
                {
                    int index = args.arguments[0];
                    auto p = thisObject->jsAmbiSourceSet->getGroup(index - 1); // make 0-based
                    if(p != nullptr)
                    {
                        return p->getRawPoint()->getDistance();
                    }
                    else
                    {
                        thisObject->fail("getGroupD: invalid source index " + String(index));
                    }
                }
                
                return var::undefined();
            }
               
            static var getGroupName (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("getGroupName", args, 1))
                {
                    int index = args.arguments[0];
                    auto p = thisObject->jsAmbiSourceSet->getGroup(index - 1); // make 0-based
                    if(p != nullptr)
                    {
                        return p->getName();
                    }
                    else
                    {
                        thisObject->fail("getGroupName: invalid source index " + String(index));
                    }
                }
                
                return var::undefined();
            }
            
            static var rotateGroup (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("rotateGroup", args, 4))
                {
                    int index = args.arguments[0];
                    double x = (double)args.arguments[1] / 180.0 * PI;
                    double y = (double)args.arguments[2] / 180.0 * PI;
                    double z = (double)args.arguments[3] / 180.0 * PI;
                    
                    if(thisObject->jsAmbiSourceSet->rotateGroup(index - 1, x, y, z)) // make index 0-based
                    {
                        return true;
                    }
                    else
                    {
                        thisObject->fail("rotateGroup: invalid group index " + String(index));
                    }
                }
                
                return var::undefined();
            }
            
            static var rotateGroupByName (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("rotateGroupByName", args, 4))
                {
                    String name = args.arguments[0];
                    double x = (double)args.arguments[1] / 180.0 * PI;
                    double y = (double)args.arguments[2] / 180.0 * PI;
                    double z = (double)args.arguments[3] / 180.0 * PI;
                    
                    if(thisObject->jsAmbiSourceSet->rotateGroup(name, x, y, z))
                    {
                        return true;
                    }
                    else
                    {
                        thisObject->fail("rotateGroupByName: group '" + name + "' not found");
                    }
                }
                
                return var::undefined();
            }
            
            static var rotateGroupAroundOrigin (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("rotateGroupAroundOrigin", args, 4, 5))
                {
                    int index = args.arguments[0];
                    double x = (double)args.arguments[1] / 180.0 * PI;
                    double y = (double)args.arguments[2] / 180.0 * PI;
                    double z = (double)args.arguments[3] / 180.0 * PI;
                        
                    // optional move sub elements flag
                    bool moveSub = true;
                    if(args.numArguments >= 5)
                        moveSub = args.arguments[4];
                        
                    if(thisObject->jsAmbiSourceSet->rotateGroupAroundOrigin(index - 1, x, y, z, moveSub)) // make index 0-based
                    {
                        return true;
                    }
                    else
                    {
                        thisObject->fail("rotateGroupAroundOrigin: invalid group index " + String(index));
                    }
                }
                
                return var::undefined();
            }
            
            static var rotateGroupAroundOriginByName (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("rotateGroupAroundOriginByName", args, 4, 5))
                {
                    String name = args.arguments[0];
                    double x = (double)args.arguments[1] / 180.0 * PI;
                    double y = (double)args.arguments[2] / 180.0 * PI;
                    double z = (double)args.arguments[3] / 180.0 * PI;
                        
                    // optional move sub elements flag
                    bool moveSub = true;
                    if(args.numArguments >= 5)
                        moveSub = args.arguments[4];
                        
                    if(thisObject->jsAmbiSourceSet->rotateGroupAroundOrigin(name, x, y, z, moveSub))
                    {
                        return true;
                    }
                    else
                    {
                        thisObject->fail("rotateGroupAroundOriginByName: group '" + name + "' not found");
                    }
                }
                
                return var::undefined();
            }
            
            static var stretchGroup (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("stretchGroup", args, 2))
                {
                    int index = args.arguments[0];
                    double stretch = args.arguments[1];
                        
                    if(thisObject->jsAmbiSourceSet->stretchGroup(index - 1, stretch)) // make index 0-based
                    {
                        return true;
                    }
                    else
                    {
                        thisObject->fail("stretchGroup: invalid group index " + String(index));
                    }
                }
                
                return var::undefined();
            }
            
            static var stretchGroupByName (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("stretchGroupByName", args, 2))
                {
                    String name = args.arguments[0];
                    double stretch = args.arguments[1];
                        
                    if(thisObject->jsAmbiSourceSet->stretchGroup(name, stretch))
                    {
                        return true;
                    }
                    else
                    {
                        thisObject->fail("stretchByName: group '" + name + "' not found");
                    }
                }
                
                return var::undefined();
            }
            
            static var setGroupRotation (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("setGroupRotation", args, 5))
                {
                    int index = args.arguments[0];
                    double x = (double)args.arguments[1];
                    double y = (double)args.arguments[2];
                    double z = (double)args.arguments[3];
                    double w = (double)args.arguments[4];
                    
                    if(thisObject->jsAmbiSourceSet->setGroupRotation(index - 1, Quaternion<double>(x, y, z, w))) // make index 0-based
                    {
                        return true;
                    }
                    else
                    {
                        thisObject->fail("setGroupRotation: invalid group index " + String(index));
                    }
                }
                
                return var::undefined();
            }
                        
            static var setGroupRotationByName (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("setGroupRotationByName", args, 5))
                {
                    String name = args.arguments[0];
                    double x = (double)args.arguments[1];
                    double y = (double)args.arguments[2];
                    double z = (double)args.arguments[3];
                    double w = (double)args.arguments[4];
                    
                    if(thisObject->jsAmbiSourceSet->setGroupRotation(name, Quaternion<double>(x, y, z, w)))
                    {
                        return true;
                    }
                    else
                    {
                        thisObject->fail("setGroupRotationByName: group '" + name + "' not found");
                    }
                }
                
                return var::undefined();
            }
            
            static var setGroupStretch (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("setGroupStretch", args, 2))
                {
                    int index = args.arguments[0];
                    double stretch = args.arguments[1];
                        
                    if(thisObject->jsAmbiSourceSet->setGroupStretch(index - 1, stretch)) // make index 0-based
                    {
                        return true;
                    }
                    else
                    {
                        thisObject->fail("setGroupStretch: invalid group index " + String(index));
                    }
                }
                
                return var::undefined();
            }
            
            static var setGroupStretchByName (const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("setGroupStretchByName", args, 2))
                {
                    String name = args.arguments[0];
                    double stretch = args.arguments[1];
                        
                    if(thisObject->jsAmbiSourceSet->setGroupStretch(name, stretch))
                    {
                        return true;
                    }
                    else
                    {
                        thisObject->fail("setGroupStretchByName: group '" + name + "' not found");
                    }
                }
                
                return var::undefined();
            }
            
            static var reportError(const var::NativeFunctionArgs& args)
            {
                if (auto* thisObject = basicCheck("reportError", args, 1))
                {
                    String msg = args.arguments[0];
                    thisObject->fail(msg);
                    return true;
                }
                
                return var::undefined();
            
            }
            
            void fail(String msg)
            {
                errorMsg = msg;
            }
            
            CustomOscReceiver* owner;
            AmbiSourceSet* jsAmbiSourceSet;
            Array<var> jsPathElements;
            Array<var> jsArguments;
            String errorMsg;
            int jsPointIndex;
            static const int buffer_size = 1000;
            float buffer[buffer_size];
            
        private:
            static JsContext* basicCheck(String methodName, const var::NativeFunctionArgs& args, int minArgs, int maxArgs)
            {
                if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                {
                    if(args.numArguments >= minArgs && args.numArguments <= maxArgs)
                    {
                        return thisObject;
                    }
                    else
                    {
                        thisObject->fail(methodName
                                         + ": wrong #args ("
                                         + (minArgs != maxArgs
                                            ? (String(minArgs) + "-" + String(maxArgs))
                                            : String(minArgs))
                                         + " expected, "
                                         + String(args.numArguments)
                                         + " found)");
                    }
                }
                
                return nullptr;
            }
            
            static JsContext* basicCheck(String methodName, const var::NativeFunctionArgs& args, int numArgs)
            {
                return basicCheck(methodName, args, numArgs, numArgs);
            }
            
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JsContext)
        };
    
    String jsExpression;
    std::unique_ptr<JavascriptEngine> jsEngine;
    JsContext* jsContext;
};
