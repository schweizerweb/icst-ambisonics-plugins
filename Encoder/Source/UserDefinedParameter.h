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
        
        void getValueFromOsc(int* pInt, OSCArgument* pArgument);
        void getValueFromOsc(String* pString, OSCArgument* pArgument);
        void getValueFromOsc(double* pDouble, OSCArgument* pArgument, double scaler);
        bool checkConst(OSCArgument* pArgument);
        
    private:
        struct JsContext  : public DynamicObject
            {
                JsContext (UserDefinedParameter* demo) : owner (demo)
                {
                    setMethod ("x", x);
                    setMethod ("y", y);
                    setMethod ("z", z);
                    setMethod ("a", a);
                    setMethod ("e", e);
                    setMethod ("d", d);
                    setMethod ("gain", gain);
                    setMethod ("name", name);
                    setMethod ("color", color);
                }
        
                static Identifier getClassName()    { return "p"; }
        
                static var x (const var::NativeFunctionArgs& args)
                {
                    if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                        return thisObject->jsAmbiPoint->getPoint()->getX();
        
                    return var::undefined();
                }

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
                  
                
                UserDefinedParameter* owner;
                AmbiPoint* jsAmbiPoint;
        
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
    
    
