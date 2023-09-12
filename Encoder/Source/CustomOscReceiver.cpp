/*
  ==============================================================================

    CustomOscReceiver.cpp
    Created: 17 Jan 2022 10:21:03pm
    Author:  Schweizer Christian

  ==============================================================================
*/

#include "CustomOscReceiver.h"

CustomOscReceiver::CustomOscReceiver(CustomOscInput* pInput, ScalingInfo* pScaling) : CustomOscBase(pScaling)
{
    bool hasXyz = false;
    bool hasAed = false;
    xyzMode = false;
    javaScriptMode = false;
    isValid = false;
    lastRxTimestamp = 0;
    lastSuccessfulRxTimestamp = 0;
    
    if(!pInput->commandString.isEmpty())
    {
        // java script mode
        javaScriptMode = true;
        oscPath = pInput->oscString;
        try
        {
            patternToMatch.reset(new OSCAddressPattern(oscPath));
        }
        catch(OSCException& e)
        {
            errorMessage = "Invalid OSC path";
            return;
        }

        jsExpression = pInput->commandString;
        jsEngine.reset(new JavascriptEngine());
        jsEngine->maximumExecutionTime = RelativeTime::seconds (5);
        jsContext = new JsContext (this);
        jsEngine->registerNativeObject ("s", jsContext);
        
        isValid = true;
        return;
    }
    
    bool ok = setOscPath(pInput->oscString, &errorMessage);
    if(!ok)
    {
        errorMessage = "Error setting OSC path" + (errorMessage.isEmpty() ? "" : (": " + errorMessage));
        return;
    }
    
    if(channelSelectionMode == ChannelSelectionMode::Undefined)
    {
        errorMessage = "Invalid channel selection - Either index or name is required in custom input pattern (constant or dynamic)";
        return;
    }
    
    String matchString = getOscPath().replace("{i}", "*").replace("{n}", "*");
    try
    {
        patternToMatch.reset(new OSCAddressPattern(matchString));
    }
    catch (OSCException& e)
    {
        errorMessage = "Invalid OSC path";
        return;
    }
    
    for(auto& p : realParameters)
    {
        UserDefinedParameter::ParameterType t = p->getType();
        if(t == UserDefinedParameter::ParameterType::X
                || t == UserDefinedParameter::ParameterType::Y
                || t == UserDefinedParameter::ParameterType::Z)
            hasXyz = true;
        else if(t == UserDefinedParameter::ParameterType::A
                || t == UserDefinedParameter::ParameterType::E
                || t == UserDefinedParameter::ParameterType::D)
            hasAed = true;
    }
    
    if(hasAed && hasXyz)
    {
        errorMessage = "XYZ and AED cannot be mixed";
        return;
    }
    xyzMode = hasXyz;
    
    isValid = true;
}

bool CustomOscReceiver::getInitFlag()
{
    return isValid;
}

uint32 CustomOscReceiver::getLastRxTimestamp()
{
    return lastRxTimestamp;
}

uint32 CustomOscReceiver::getLastSuccessfulRxTimestamp()
{
    return lastSuccessfulRxTimestamp;
}

String CustomOscReceiver::getErrorMessage()
{
    return errorMessage;
}

bool CustomOscReceiver::matchesPattern(OSCAddress* pAddress)
{
    return isValid && patternToMatch->matches(*pAddress);
}

bool CustomOscReceiver::handleMessage(AmbiSourceSet* pSources, const OSCMessage* pMessage)
{
    if(!isValid)
        return false;
 
    // reset error message
    errorMessage = "";
    lastRxTimestamp = Time::getMillisecondCounter();
    
    if(javaScriptMode)
    {
        jsContext->jsAmbiSourceSet = pSources;
        
        // address path elements
        jsContext->jsPathElements.clear();
        jsContext->jsArguments.clear();
        jsContext->errorMsg = "";
        String address = pMessage->getAddressPattern().toString();
        StringArray tokens;
        tokens.addTokens (address, "/", "\"");
        tokens.removeEmptyStrings();
        for(auto& t : tokens)
        {
            if(t.containsOnly("0123456789"))
                jsContext->jsPathElements.add(t.getIntValue());
            else if(t.containsOnly("0123456789."))
                jsContext->jsPathElements.add(t.getFloatValue());
            else
                jsContext->jsPathElements.add(t);
        }
        for(auto& a : *pMessage)
        {
            if(a.isInt32())
                jsContext->jsArguments.add(a.getInt32());
            else if(a.isFloat32())
                jsContext->jsArguments.add(a.getFloat32());
            else
                jsContext->jsArguments.add(a.getString());
        }
        
        auto ret = jsEngine->execute(jsExpression);
        if(ret.failed())
        {
            String message = ret.getErrorMessage();
            errorMessage = message;
            return false;
        }
        else if(!jsContext->errorMsg.isEmpty())
        {
            // user defined function failed
            String message = jsContext->errorMsg;
            errorMessage = message;
            return false;
        }
        
        lastSuccessfulRxTimestamp = Time::getMillisecondCounter();
        return true;
    }
    
    // check for enough parameters
    if(pMessage->size() < realParameters.size())
    {
        errorMessage = "too many arguments";
        return false;
    }
    
    const double invalidDbl = std::numeric_limits<double>::max();
    
    int index = channelSelectionMode == ChannelSelectionMode::ConstantIndex ? constantChannelIndex : -1;
    String name = channelSelectionMode == ChannelSelectionMode::ConstantName ? constantChannelName : "";
    String color = "";
    double x = invalidDbl;
    double y = invalidDbl;
    double z = invalidDbl;
    double a = invalidDbl;
    double e = invalidDbl;
    double d = invalidDbl;
    double gain = invalidDbl;
    
    for(auto& p : parametersInPath)
    {
        int i = oscPath.indexOf(p->getOriginalString());
        String interestingPart = pMessage->getAddressPattern().toString().substring(i);
        
        if(p->getType() == UserDefinedParameter::ParameterType::Index)
        {
            index = std::stoi(interestingPart.toStdString());
        }
        else if(p->getType() == UserDefinedParameter::ParameterType::Name)
        {
            name = interestingPart.upToFirstOccurrenceOf("/", false, true);
        }
    }
    
    for(int i = 0; i < realParameters.size(); i++)
    {
        auto oscParam = (*pMessage)[i];
        auto param = realParameters.getUnchecked(i);
        bool dataTypeError = false;
        switch(param->getType())
        {
            case UserDefinedParameter::ParameterType::Index:
                if(!param->getValueFromOsc(&index, &oscParam))
                    dataTypeError = true;
                break;
            case UserDefinedParameter::ParameterType::Name:
                if(!param->getValueFromOsc(&name, &oscParam))
                    dataTypeError = true;
                break;
                
            case UserDefinedParameter::ParameterType::Color:
                if(!param->getValueFromOsc(&color, &oscParam))
                    dataTypeError = true;
                break;
                
            case UserDefinedParameter::ParameterType::X:
            case UserDefinedParameter::ParameterType::ScaledX:
            case UserDefinedParameter::ParameterType::DualScaledX:
                if(!param->getValueFromOsc(&x, &oscParam, pScalingInfo->GetScaler()))
                    dataTypeError = true;
                break;
                
            case UserDefinedParameter::ParameterType::Y:
            case UserDefinedParameter::ParameterType::ScaledY:
            case UserDefinedParameter::ParameterType::DualScaledY:
                if(!param->getValueFromOsc(&y, &oscParam, pScalingInfo->GetScaler()))
                    dataTypeError = true;
                break;
                
            case UserDefinedParameter::ParameterType::Z:
            case UserDefinedParameter::ParameterType::ScaledZ:
            case UserDefinedParameter::ParameterType::DualScaledZ:
                if(!param->getValueFromOsc(&z, &oscParam, pScalingInfo->GetScaler()))
                    dataTypeError = true;
                break;
                
            case UserDefinedParameter::ParameterType::A:
            case UserDefinedParameter::ParameterType::ScaledA:
                if(!param->getValueFromOsc(&a, &oscParam, pScalingInfo->GetScaler()))
                    dataTypeError = true;
                break;
                
            case UserDefinedParameter::ParameterType::E:
            case UserDefinedParameter::ParameterType::ScaledE:
            case UserDefinedParameter::ParameterType::DualScaledE:
                if(!param->getValueFromOsc(&e, &oscParam, pScalingInfo->GetScaler()))
                    dataTypeError = true;
                break;
                
            case UserDefinedParameter::ParameterType::D:
            case UserDefinedParameter::ParameterType::ScaledD:
                if(!param->getValueFromOsc(&d, &oscParam, pScalingInfo->GetScaler()))
                    dataTypeError = true;
                break;
         
            case UserDefinedParameter::ParameterType::Gain:
                if(!param->getValueFromOsc(&gain, &oscParam, pScalingInfo->GetScaler()))
                    dataTypeError = true;
                break;
            
            case UserDefinedParameter::ParameterType::ConstInt:
            case UserDefinedParameter::ParameterType::ConstFloat:
            case UserDefinedParameter::ParameterType::ConstString:
                bool dataTypeOk;
                if(!param->checkConst(&oscParam, &dataTypeOk))
                {
                    if(!dataTypeOk)
                        dataTypeError = true;
                    else
                    {
                        errorMessage = "";
                        return false; // simply ignore
                    }
                }
                break;
            
            case UserDefinedParameter::ParameterType::Ignore:
                // ignore this argument, whatever it contains
                break;
                
            case UserDefinedParameter::ParameterType::Expression:
                // this type has no effect
                break;
        }
        if(dataTypeError)
        {
            errorMessage = "wrong datatype at parameter " + param->getOriginalString();
            return false;
        }
    }
    
    // get index from name if needed
    if(index == -1 && name != "")
    {
        for(int i = 0; i < pSources->size(); i++) {
            if(pSources->get(i)->getName() == name) {
                index = i + 1; // make 1-based
                break;
            }
        }
    }
    
    if(index < 1)
    {
        // no reference to point --> error
        return false;
    }
    
    index--; // make 0-based
    
    auto pt = pSources->get(index);
    // use only xyz OR aed, xyz has priority
    if(xyzMode)
    {
        if(approximatelyEqual(x, invalidDbl))
            x = pt->getRawPoint()->getX();
        if(approximatelyEqual(y, invalidDbl))
            y = pt->getRawPoint()->getY();
        if(approximatelyEqual(z, invalidDbl))
            z = pt->getRawPoint()->getZ();
        
        pSources->setChannelXYZ(index, x, y, z);
    }
    else
    {
        // otherwise use aed
        if(approximatelyEqual(a, invalidDbl))
            a = pt->getRawPoint()->getAzimuth();
        if(approximatelyEqual(e, invalidDbl))
            e = pt->getRawPoint()->getElevation();
        if(approximatelyEqual(d, invalidDbl))
            d = pt->getRawPoint()->getDistance();
        
        pSources->setChannelAED(index, a, e, d);
    }
    
    if(approximatelyEqual(gain, invalidDbl))
    {
        pSources->setGain(index, gain);
    }
    
    if(color != "")
    {
        pSources->setChannelColor(index, Colour::fromString(color));
    }
    
    lastSuccessfulRxTimestamp = Time::getMillisecondCounter();
    return true;
}
