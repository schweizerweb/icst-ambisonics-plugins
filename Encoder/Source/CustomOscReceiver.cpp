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
    hasIndex = false;
    hasName = false;
    xyzMode = false;
    javaScriptMode = false;
    isValid = false;
    
    if(!pInput->commandString.isEmpty())
    {
        // java script mode
        javaScriptMode = true;
        oscPath = pInput->oscString;
        String matchString = oscPath.replace("{}", "*");
        patternToMatch.reset(new OSCAddressPattern(matchString));
        jsExpression = pInput->commandString;
        jsEngine.reset(new JavascriptEngine());
        jsEngine->maximumExecutionTime = RelativeTime::seconds (5);
        jsContext = new JsContext (this);
        jsEngine->registerNativeObject ("p", jsContext);
        
        isValid = true;
        return;
    }
    
    bool ok = setOscPath(pInput->oscString, &errorMessage);
    if(!ok)
    {
        errorMessage = "Error setting OSC path";
        return;
    }
    
    String matchString = getOscPath().replace("{i}", "*").replace("{n}", "*");
    patternToMatch.reset(new OSCAddressPattern(matchString));
    
    for(auto& p : parametersInPath)
    {
        if(p->getType() == UserDefinedParameter::ParameterType::Index)
            hasIndex = true;
        else if(p->getType() == UserDefinedParameter::ParameterType::Name)
            hasName = true;
    }
    
    for(auto& p : realParameters)
    {
        UserDefinedParameter::ParameterType t = p->getType();
        if(t == UserDefinedParameter::ParameterType::Index)
            hasIndex = true;
        else if(t == UserDefinedParameter::ParameterType::Name)
            hasName = true;
        else if(t == UserDefinedParameter::ParameterType::X
                || t == UserDefinedParameter::ParameterType::Y
                || t == UserDefinedParameter::ParameterType::Z)
            xyzMode = true;
    }
    
    if(!hasIndex && !hasName)
    {
        errorMessage = "Either index or name is required in custom input pattern";
        return;
    }
    
    isValid = true;;
}

bool CustomOscReceiver::matchesPattern(OSCAddress* pAddress)
{
    return isValid && patternToMatch->matches(*pAddress);
}

bool CustomOscReceiver::handleMessage(AmbiSourceSet* pSources, const OSCMessage* pMessage)
{
    if(!isValid)
        return false;
    
    if(javaScriptMode)
    {
        jsContext->jsAmbiSourceSet = pSources;
        //jsContext->jsPointIndex = index;
        
        auto ret = jsEngine->execute(jsExpression);
        if(ret.failed())
        {
            String message = ret.getErrorMessage();
            return false;
        }
        else
        {
            // success
        }
        return true;
    }
    
    // check for enough parameters
    if(pMessage->size() < realParameters.size())
        return false;
    
    const double invalidDbl = std::numeric_limits<double>::max();
    
    int index = -1;
    String name = "";
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
        switch(param->getType())
        {
            case UserDefinedParameter::ParameterType::Index:
                if(!param->getValueFromOsc(&index, &oscParam))
                    return false;
                break;
            case UserDefinedParameter::ParameterType::Name:
                if(!param->getValueFromOsc(&name, &oscParam))
                    return false;
                break;
                
            case UserDefinedParameter::ParameterType::Color:
                if(!param->getValueFromOsc(&color, &oscParam))
                    return false;
                break;
                
            case UserDefinedParameter::ParameterType::X:
            case UserDefinedParameter::ParameterType::ScaledX:
            case UserDefinedParameter::ParameterType::DualScaledX:
                if(!param->getValueFromOsc(&x, &oscParam, pScalingInfo->GetScaler()))
                    return false;
                break;
                
            case UserDefinedParameter::ParameterType::Y:
            case UserDefinedParameter::ParameterType::ScaledY:
            case UserDefinedParameter::ParameterType::DualScaledY:
                if(!param->getValueFromOsc(&y, &oscParam, pScalingInfo->GetScaler()))
                    return false;
                break;
                
            case UserDefinedParameter::ParameterType::Z:
            case UserDefinedParameter::ParameterType::ScaledZ:
            case UserDefinedParameter::ParameterType::DualScaledZ:
                if(!param->getValueFromOsc(&z, &oscParam, pScalingInfo->GetScaler()))
                    return false;
                break;
                
            case UserDefinedParameter::ParameterType::A:
            case UserDefinedParameter::ParameterType::ScaledA:
                if(!param->getValueFromOsc(&a, &oscParam, pScalingInfo->GetScaler()))
                    return false;
                break;
                
            case UserDefinedParameter::ParameterType::E:
            case UserDefinedParameter::ParameterType::ScaledE:
            case UserDefinedParameter::ParameterType::DualScaledE:
                if(!param->getValueFromOsc(&e, &oscParam, pScalingInfo->GetScaler()))
                    return false;
                break;
                
            case UserDefinedParameter::ParameterType::D:
            case UserDefinedParameter::ParameterType::ScaledD:
                if(!param->getValueFromOsc(&d, &oscParam, pScalingInfo->GetScaler()))
                    return false;
                break;
         
            case UserDefinedParameter::ParameterType::Gain:
                if(!param->getValueFromOsc(&gain, &oscParam, pScalingInfo->GetScaler()))
                    return false;
                break;
            
            case UserDefinedParameter::ParameterType::ConstInt:
            case UserDefinedParameter::ParameterType::ConstFloat:
            case UserDefinedParameter::ParameterType::ConstString:
                if(!param->checkConst(&oscParam))
                    return false;
                break;
            
            case UserDefinedParameter::ParameterType::Ignore:
                // ignore this argument, whatever it contains
                break;
                
            case UserDefinedParameter::ParameterType::Expression:
                // this type has no effect
                break;
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
        if(x == invalidDbl)
            x = pt->getPoint()->getX();
        if(y == invalidDbl)
            y = pt->getPoint()->getY();
        if(z == invalidDbl)
            z = pt->getPoint()->getZ();
        
        pSources->setChannelXYZ(index, x, y, z);
    }
    else
    {
        // otherwise use aed
        if(a == invalidDbl)
            a = pt->getPoint()->getAzimuth();
        if(e == invalidDbl)
            e = pt->getPoint()->getElevation();
        if(d == invalidDbl)
            d = pt->getPoint()->getDistance();
        
        pSources->setChannelAED(index, a, e, d);
    }
    
    if(gain != invalidDbl)
    {
        pSources->setGain(index, gain);
    }
    
    if(color != "")
    {
        pSources->setChannelColor(index, Colour::fromString(color));
    }
    
    return true;
}
