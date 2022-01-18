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
    setOscPath(pInput->oscString);
    String matchString = getOscPath().replace("{i}", "#");
    addressToMatch.reset(new OSCAddress(matchString));
    
    hasIndex = false;
    hasName = false;
    xyzMode = false;
    isValid = true;
    
    for(auto& p : parametersInPath)
    {
        if(p.getType() == UserDefinedParameter::ParameterType::Index)
            hasIndex = true;
        else if(p.getType() == UserDefinedParameter::ParameterType::Name)
            hasName = true;
        else
            isValid = false;
    }
    
    for(auto& p : realParameters)
    {
        UserDefinedParameter::ParameterType t = p.getType();
        if(t == UserDefinedParameter::ParameterType::Index)
            hasIndex = true;
        else if(t == UserDefinedParameter::ParameterType::Name)
            hasName = true;
        else if(t == UserDefinedParameter::ParameterType::X
                || t == UserDefinedParameter::ParameterType::Y
                || t == UserDefinedParameter::ParameterType::Z)
            xyzMode = true;
    }
    
    isValid &= (hasIndex || hasName);
}

bool CustomOscReceiver::matchesPattern(OSCAddressPattern* pPattern)
{
    return isValid && pPattern->matches(*addressToMatch);
}

bool CustomOscReceiver::handleMessage(AmbiSourceSet* pSources, const OSCMessage* pMessage)
{
    if(!isValid)
        return false;
    
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
        int i = oscPath.indexOf(p.getOriginalString());
        String interestingPart = pMessage->getAddressPattern().toString().substring(i);
        
        if(p.getType() == UserDefinedParameter::ParameterType::Index)
        {
            index = std::stoi(interestingPart.toStdString());
        }
        else if(p.getType() == UserDefinedParameter::ParameterType::Name)
        {
            name = interestingPart.upToFirstOccurrenceOf("/", false, true);
        }
    }
    
    for(int i = 0; i < realParameters.size(); i++)
    {
        auto oscParam = (*pMessage)[i];
        auto& param = realParameters.getReference(i);
        switch(param.getType())
        {
            case UserDefinedParameter::ParameterType::Index:
                param.getValueFromOsc(&index, &oscParam);
                break;
            case UserDefinedParameter::ParameterType::Name:
                param.getValueFromOsc(&name, &oscParam);
                break;
                
            case UserDefinedParameter::ParameterType::Color:
                param.getValueFromOsc(&color, &oscParam);
                break;
                
            case UserDefinedParameter::ParameterType::X:
            case UserDefinedParameter::ParameterType::ScaledX:
            case UserDefinedParameter::ParameterType::DualScaledX:
                param.getValueFromOsc(&x, &oscParam, pScalingInfo->GetScaler());
                break;
                
            case UserDefinedParameter::ParameterType::Y:
            case UserDefinedParameter::ParameterType::ScaledY:
            case UserDefinedParameter::ParameterType::DualScaledY:
                param.getValueFromOsc(&y, &oscParam, pScalingInfo->GetScaler());
                break;
                
            case UserDefinedParameter::ParameterType::Z:
            case UserDefinedParameter::ParameterType::ScaledZ:
            case UserDefinedParameter::ParameterType::DualScaledZ:
                param.getValueFromOsc(&z, &oscParam, pScalingInfo->GetScaler());
                break;
                
            case UserDefinedParameter::ParameterType::A:
            case UserDefinedParameter::ParameterType::ScaledA:
                param.getValueFromOsc(&a, &oscParam, pScalingInfo->GetScaler());
                break;
                
            case UserDefinedParameter::ParameterType::E:
            case UserDefinedParameter::ParameterType::ScaledE:
            case UserDefinedParameter::ParameterType::DualScaledE:
                param.getValueFromOsc(&e, &oscParam, pScalingInfo->GetScaler());
                break;
                
            case UserDefinedParameter::ParameterType::D:
            case UserDefinedParameter::ParameterType::ScaledD:
                param.getValueFromOsc(&d, &oscParam, pScalingInfo->GetScaler());
                break;
         
            case UserDefinedParameter::ParameterType::Gain:
                param.getValueFromOsc(&gain, &oscParam, pScalingInfo->GetScaler());
                break;
            
            case UserDefinedParameter::ParameterType::ConstInt:
            case UserDefinedParameter::ParameterType::ConstFloat:
            case UserDefinedParameter::ParameterType::ConstString:
                if(!param.checkConst(&oscParam))
                    return false;
            
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
                index = i;
                break;
            }
        }
    }
    
    if(index < 0)
    {
        // no reference to point --> error
        return false;
    }
    
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
