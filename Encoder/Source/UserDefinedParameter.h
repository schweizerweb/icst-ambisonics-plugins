/*
  ==============================================================================

    UserDefinedParameter.h
    Created: 17 Jan 2022 10:50:22pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/AmbiPoint.h"

    class UserDefinedParameter
    {
    public:
        enum ParameterType { Index, Name, Color, A, E, D, X, Y, Z, ScaledA, ScaledE, ScaledD, ScaledX, ScaledY, ScaledZ, Gain, DualScaledE, DualScaledX, DualScaledY, DualScaledZ, Expression, ConstInt, ConstFloat, ConstString, Ignore };

        UserDefinedParameter(String originalString, std::string typeString, double lo, double hi);
        UserDefinedParameter(String originalString, std::string typeString, double lo, double hi, double zero);
        UserDefinedParameter(String originalString, ParameterType t);
        
        OSCArgument getOSCArgument(AmbiPoint* pt, double scaler, int index);
        String getString(AmbiPoint* pt, double scaler, int index);
        String getOriginalString();
        ParameterType getType() {return type;}
        
        bool getValueFromOsc(int* pInt, OSCArgument* pArgument);
        bool getValueFromOsc(String* pString, OSCArgument* pArgument);
        bool getValueFromOsc(double* pDouble, OSCArgument* pArgument, double scaler);
        bool checkConst(OSCArgument* pArgument);
        
    private:
        struct JsContext  : public DynamicObject
            {
                JsContext (UserDefinedParameter* demo) : owner (demo)
                {
                    setMethod ("getX", x);
                    setMethod ("getY", y);
                    setMethod ("getZ", z);
                    setMethod ("getA", a);
                    setMethod ("getE", e);
                    setMethod ("getD", d);
                    setMethod ("getGain", gain);
                    setMethod ("getName", name);
                    setMethod ("getColor", color);
                    setMethod ("getIndex", i);
                }
        
                static Identifier getClassName()    { return "p"; }
        
                static var x (const var::NativeFunctionArgs& args)
                {
                    if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                        return thisObject->jsAmbiPoint->getRawPoint()->getX();
        
                    return var::undefined();
                }

                static var y (const var::NativeFunctionArgs& args)
                {
                    if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                        return thisObject->jsAmbiPoint->getRawPoint()->getY();
                
                    return var::undefined();
                }
                   
                static var z (const var::NativeFunctionArgs& args)
                {
                    if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                        return thisObject->jsAmbiPoint->getRawPoint()->getZ();
                
                    return var::undefined();
                }
                   
                static var a (const var::NativeFunctionArgs& args)
                {
                    if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                        return thisObject->jsAmbiPoint->getRawPoint()->getAzimuth();
                
                    return var::undefined();
                }
                   
                static var e (const var::NativeFunctionArgs& args)
                {
                    if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                        return thisObject->jsAmbiPoint->getRawPoint()->getElevation();
                
                    return var::undefined();
                }
                   
                static var d (const var::NativeFunctionArgs& args)
                {
                    if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                        return thisObject->jsAmbiPoint->getRawPoint()->getDistance();
                
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
                  
                
                UserDefinedParameter* owner;
                AmbiPoint* jsAmbiPoint;
                int jsPointIndex;
                
                JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JsContext)
            };
        
        float getValue(AmbiPoint* pt, double scaler);
        float dualMap(double value, double maxValue);
        float inverseDualMap(double value, double maxValue);
        
        String originalString;
        ParameterType type;
        double loLim;
        double hiLim;
        double zero;
        String jsExpression;
        std::unique_ptr<JavascriptEngine> jsEngine;
        JsContext* jsContext;
        
        int constInt;
        float constFloat;
        String constString;
};
    
    
