/*
================================================================================
    This file is part of the ICST AmbiPlugins.

    ICST AmbiPlugins are free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICST AmbiPlugins are distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ICSTAmbiPlugins.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
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
        
        OSCArgument getOSCArgument(Vector3D<double> absPt, AmbiPoint* pt, double scaler, int index);
        String getString(Vector3D<double> absPt, AmbiPoint* pt, double scaler, int index);
        String getOriginalString();
        ParameterType getType() {return type;}
        
        bool getValueFromOsc(int* pInt, OSCArgument* pArgument);
        bool getValueFromOsc(String* pString, OSCArgument* pArgument);
        bool getValueFromOsc(double* pDouble, OSCArgument* pArgument, double scaler);
        bool checkConst(OSCArgument* pArgument, bool* pDataTypeOk);
        
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
                    setMethod ("getAbsX", absX);
                    setMethod ("getAbsY", absY);
                    setMethod ("getAbsZ", absZ);
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
                
                static var absX (const var::NativeFunctionArgs& args)
                {
                    if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                        return thisObject->jsAbsPos.x;
        
                    return var::undefined();
                }

                static var absY (const var::NativeFunctionArgs& args)
                {
                    if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                        return thisObject->jsAbsPos.y;
                
                    return var::undefined();
                }
                   
                static var absZ (const var::NativeFunctionArgs& args)
                {
                    if (auto* thisObject = dynamic_cast<JsContext*> (args.thisObject.getObject()))
                        return thisObject->jsAbsPos.z;
                
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
                Vector3D<double> jsAbsPos; // TODO
                int jsPointIndex;
                
                JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JsContext)
            };
        
        float getValue(Vector3D<double> absPt, AmbiPoint* pt, double scaler);
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
    
    
