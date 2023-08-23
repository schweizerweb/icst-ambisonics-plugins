/*
  ==============================================================================

    UserDefinedParameter.cpp
    Created: 17 Jan 2022 10:50:22pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#include "UserDefinedParameter.h"
#include <string.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"

UserDefinedParameter::UserDefinedParameter(String _originalString, std::string typeString, double lo, double hi) : originalString(_originalString), loLim(lo), hiLim(hi)
{
    switch(typeString.substr()[0])
    {
        case 'x': type = ParameterType::ScaledX; break;
        case 'y': type = ParameterType::ScaledY; break;
        case 'z': type = ParameterType::ScaledZ; break;
        case 'a': type = ParameterType::ScaledA; break;
        case 'e': type = ParameterType::ScaledE; break;
        case 'd': type = ParameterType::ScaledD; break;
    }
}

UserDefinedParameter::UserDefinedParameter(String _originalString, std::string typeString, double lo, double hi, double _zero) : originalString(_originalString), loLim(lo), hiLim(hi), zero(_zero)
{
    switch(typeString.substr()[0])
    {
        case 'x': type = ParameterType::DualScaledX; break;
        case 'y': type = ParameterType::DualScaledY; break;
        case 'z': type = ParameterType::DualScaledZ; break;
        case 'e': type = ParameterType::DualScaledE; break;
    }
}

UserDefinedParameter::UserDefinedParameter(String _originalString, ParameterType t): originalString(_originalString)
{
    type = t;
    switch(t)
    {
        case ScaledA:
        case ScaledE:
        case ScaledD:
        case ScaledX:
        case ScaledY:
        case ScaledZ:
            loLim = 0.0; hiLim = 1.0; break;

        case ConstInt:
            constInt = std::stoi(originalString.fromFirstOccurrenceOf(",", false, true).toStdString());
            break;
            
        case ConstFloat:
            constFloat = std::stof(originalString.fromFirstOccurrenceOf(",", false, true).toStdString());
            break;
            
        case ConstString:
            constString = originalString.fromFirstOccurrenceOf(",", false, true).upToLastOccurrenceOf("}", false, true).trimCharactersAtStart(" ");
            break;
            
        case Expression:
            jsExpression = originalString.fromFirstOccurrenceOf(",", false, true).upToLastOccurrenceOf("}", false, true);
            jsEngine.reset(new JavascriptEngine());
            jsEngine->maximumExecutionTime = RelativeTime::seconds (5);
            jsContext = new JsContext (this);
            jsEngine->registerNativeObject ("p", jsContext);
            break;

        default: break;
    }
}

OSCArgument UserDefinedParameter::getOSCArgument(Vector3D<double> absPt, AmbiPoint* pt, double scaler, int index)
{
    switch(type)
    {
        case Index:
            return OSCArgument(index + 1);
            
        case Name:
        case Color:
            return OSCArgument(getString(absPt, pt, scaler, index));
    
        case A:
        case E:
        case D:
        case X:
        case Y:
        case Z:
        case ScaledX:
        case ScaledY:
        case ScaledZ:
        case ScaledA:
        case ScaledE:
        case ScaledD:
        case Gain:
        case DualScaledX:
        case DualScaledY:
        case DualScaledZ:
        case DualScaledE:
            return OSCArgument(getValue(absPt, pt, scaler));
            
        case ConstInt:
            return OSCArgument(constInt);
            
        case ConstFloat:
            return OSCArgument(constFloat);
            
        case ConstString:
            return OSCArgument(constString);
            
        case Ignore:
            return OSCArgument("");
            
        case Expression:
        {
            jsContext->jsAmbiPoint = pt;
            jsContext->jsAbsPos = absPt;
            jsContext->jsPointIndex = index;
            
            auto ret = jsEngine->evaluate(jsExpression);
            if(ret.isUndefined())
                return OSCArgument("error");
            else
            {
                // return OSC argument that fits the return type of the java script code
                if(ret.isInt() || ret.isBool())
                    return OSCArgument((int)ret);
                else if(ret.isDouble())
                    return OSCArgument((float)ret);
                return OSCArgument(ret.toString());
            }
        }
            
        default:
            return OSCArgument("error");
    }
}

String UserDefinedParameter::getString(Vector3D<double> absPt, AmbiPoint* pt, double scaler, int index)
{
    switch(type)
    {
        case Index: return String(index + 1);
        case Name: return pt->getName();
        case Color: return pt->getColor().toDisplayString(true);
            
        case A:
        case E:
        case D:
        case X:
        case Y:
        case Z:
        case ScaledX:
        case ScaledY:
        case ScaledZ:
        case ScaledA:
        case ScaledE:
        case ScaledD:
        case Gain:
        case DualScaledX:
        case DualScaledY:
        case DualScaledZ:
        case DualScaledE:
            return String(getValue(absPt, pt, scaler));
            
        case ConstInt: return String(constInt);
        case ConstFloat: return String(constFloat);
        case ConstString: return constString;
        case Ignore: return "";
        case Expression:
        {
            jsContext->jsAbsPos = absPt;
            jsContext->jsAmbiPoint = pt;
            jsContext->jsPointIndex = index;
            
            Result errorMessage(Result::ok());
            auto ret = jsEngine->evaluate(jsExpression, &errorMessage);
            if(ret.isUndefined())
                return "error";
            else
            {
                return ret.toString();
            }
        }
        default: return "error";
    }
}

String UserDefinedParameter::getOriginalString()
{
    return originalString;
}

float UserDefinedParameter::getValue(Vector3D<double> absPt, AmbiPoint* pt, double scaler)
{
    switch(type)
    {
    case A: return float(Constants::RadToGrad(Point3D<double>(absPt.x, absPt.y, absPt.z).getAzimuth()));
    case E: return float(Constants::RadToGrad(Point3D<double>(absPt.x, absPt.y, absPt.z).getElevation()));
    case D: return float(absPt.length());
    case X: return float(absPt.x);
    case Y: return float(absPt.y);
    case Z: return float(absPt.z);
    case ScaledX: return float(jmap(absPt.x, -scaler, scaler, loLim, hiLim));
    case ScaledY: return float(jmap(absPt.y, -scaler, scaler, loLim, hiLim));
    case ScaledZ: return float(jmap(absPt.z, -scaler, scaler, loLim, hiLim));
    case ScaledA: return float(jmap(Point3D<double>(absPt.x, absPt.y, absPt.z).getAzimuth(), double(Constants::AzimuthRadMin), double(Constants::AzimuthRadMax), loLim, hiLim));
    case ScaledE: return float(jmap(Point3D<double>(absPt.x, absPt.y, absPt.z).getElevation(), double(Constants::ElevationRadMin), double(Constants::ElevationRadMin), loLim, hiLim));
    case ScaledD: return float(jmap(absPt.length(), -MathConstants<double>::sqrt2 * scaler, MathConstants<double>::sqrt2 * scaler, loLim, hiLim));
    case Gain: return float(pt->getGain());
    case DualScaledX: return dualMap(absPt.x, scaler);
    case DualScaledY: return dualMap(absPt.y, scaler);
    case DualScaledZ: return dualMap(absPt.z, scaler);
    case DualScaledE: return dualMap(Point3D<double>(absPt.x, absPt.y, absPt.z).getElevation(), Constants::ElevationRadMax);
    
    case ConstInt: return float(constInt);
    case ConstFloat: return constFloat;
            
    case Index:
    case Name:
    case Color:
    case ConstString:
    case Ignore:
    default:
        throw;
    }
}

float UserDefinedParameter::dualMap(double value, double maxValue)
{
    return value <= 0.0
    ? float(jmap(value, -maxValue, 0.0, loLim, zero))
    : float(jmap(value, 0.0, maxValue, zero, hiLim));
}

float UserDefinedParameter::inverseDualMap(double value, double maxValue)
{
    return value <= 0.0
    ? float(jmap(value, loLim, zero, -maxValue, 0.0))
    : float(jmap(value, zero, hiLim, 0.0, maxValue));
}

bool UserDefinedParameter::getValueFromOsc(int* pInt, OSCArgument* pArgument)
{
    switch(type)
    {
        case Index:
            if(pArgument->isInt32())
            {
                *pInt = pArgument->getInt32();
                return true;
            }
            if(pArgument->isFloat32())
            {
                *pInt = (int)pArgument->getFloat32();
                return true;
            }
            break;
        default:
            break;
    }
    
    return false;
}

bool UserDefinedParameter::getValueFromOsc(double* pDouble, OSCArgument* pArgument, double scaler)
{
    if(!pArgument->isFloat32())
        return false;
    
    double value = pArgument->getFloat32();
    
    switch(type)
    {
        case A:
        case E:
        case D:
        case X:
        case Y:
        case Z:
        case Gain:
            *pDouble = value;
            return true;
            
        case ScaledX:
        case ScaledY:
        case ScaledZ:
            *pDouble = jmap(value, loLim, hiLim, -scaler, scaler);
            return true;
            
        case ScaledA:
            *pDouble = jmap(value, loLim, hiLim, double(Constants::AzimuthRadMin), double(Constants::AzimuthRadMax));
            return true;
            
        case ScaledE:
            *pDouble = jmap(value, loLim, hiLim, double(Constants::ElevationRadMin), double(Constants::ElevationRadMin));
            return true;
            
        case ScaledD:
            *pDouble = jmap(value, loLim, hiLim, -MathConstants<double>::sqrt2 * scaler, MathConstants<double>::sqrt2 * scaler);
            return true;
            
        case DualScaledX:
        case DualScaledY:
        case DualScaledZ:
            *pDouble = inverseDualMap(value, scaler);
            return true;
            
        case DualScaledE:
            *pDouble = inverseDualMap(value, Constants::ElevationRadMax);
            return true;
            
        default:
            return false;
    }
}

bool UserDefinedParameter::getValueFromOsc(String *pString, OSCArgument *pArgument)
{
    switch (type) {
        case Name:
            if(pArgument->isString())
            {
                *pString = pArgument->getString();
                return true;
            }
            break;
        default:
            break;
    }
    
    return false;
}

bool UserDefinedParameter::checkConst(OSCArgument* pArgument, bool* pDataTypeOk)
{
    *pDataTypeOk = false;
    
    switch (type) {
        case ConstInt: return (*pDataTypeOk = pArgument->isInt32()) && pArgument->getInt32() == constInt;
        case ConstFloat: return (*pDataTypeOk = pArgument->isFloat32()) && approximatelyEqual(pArgument->getFloat32(), constFloat);
        case ConstString: return (*pDataTypeOk = pArgument->isString()) && pArgument->getString() == constString;
            
        default:
            return false;
    }
}
