/*
  ==============================================================================

    CustomOscBase.cpp
    Created: 18 Jan 2022 9:55:33am
    Author:  Schweizer Christian

  ==============================================================================
*/

#include "CustomOscBase.h"
#include <regex>

CustomOscBase::CustomOscBase(ScalingInfo* pScaling) : pScalingInfo(pScaling)
{
    
}

bool CustomOscBase::setOscPath(String path)
{
    oscPath = path.upToFirstOccurrenceOf(" ", false, false);
    
    std::string oscPathString = oscPath.toStdString();
    std::string parameterString = path.substring(oscPath.length()).toStdString();
    
    return analyzeString(oscPathString, &parametersInPath) && analyzeString(parameterString, &realParameters);
}

bool CustomOscBase::analyzeString(std::string parameterString, Array<UserDefinedParameter> *pArray)
{
    // regex patterns
    std::regex rParameter("[ ]*\\{[ ]*(.*?)[ ]*\\}[ ]*");
    std::regex rTwoValues("s([xyzaed])[ ]*,[ ]*([-+]?[0-9]*\\.?[0-9]+|[0-9]+)[ ]*,[ ]*([-+]?[0-9]*\\.?[0-9]+|[0-9]+)");
    std::regex rThreeValues("s([xyze])[ ]*,[ ]*([-+]?[0-9]*\\.?[0-9]+|[0-9]+)[ ]*,[ ]*([-+]?[0-9]*\\.?[0-9]+|[0-9]+)[ ]*,[ ]*([-+]?[0-9]*\\.?[0-9]+|[0-9]+)");
    std::smatch parameterMatch;
    
    pArray->clear();

    while(regex_search(parameterString, parameterMatch, rParameter))
    {
        auto fullString = parameterMatch[0].str();
        auto userString = parameterMatch[1].str();
        if(userString.length() == 1)
        {
            switch(userString.substr()[0])
            {
                case 'a': pArray->add(UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::A)); break;
                case 'e': pArray->add(UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::E)); break;
                case 'd': pArray->add(UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::D)); break;
                case 'x': pArray->add(UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::X)); break;
                case 'y': pArray->add(UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::Y)); break;
                case 'z': pArray->add(UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::Z)); break;
                case 'i': pArray->add(UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::Index)); break;
                case 'n': pArray->add(UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::Name)); break;
                case 'c': pArray->add(UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::Color)); break;
                case 'g': pArray->add(UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::Gain)); break;
            }
        }
        else if(userString.length() > 0 && userString.substr()[0]== 's')
        {
            if(userString.length() == 2)
            {
                switch(userString.substr()[1])
                {
                    case 'a': pArray->add(UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::ScaledA)); break;
                    case 'e': pArray->add(UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::ScaledE)); break;
                    case 'd': pArray->add(UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::ScaledD)); break;
                    case 'x': pArray->add(UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::ScaledX)); break;
                    case 'y': pArray->add(UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::ScaledY)); break;
                    case 'z': pArray->add(UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::ScaledZ)); break;
                        default: return false;
                }
            }
            else
            {
                std::smatch sm;
                        
                if(regex_search(userString, sm, rThreeValues))
                {
                    double lo = std::stod(sm[2]);
                    double zero = std::stod(sm[3]);
                    double hi = std::stod(sm[4]);
                    if(lo == hi && lo == zero)
                        return false;
                            
                    pArray->add(UserDefinedParameter(fullString, sm[1].str(), lo, hi, zero));
                }
                else if(regex_search(userString, sm, rTwoValues))
                {
                    double lo = std::stod(sm[2]);
                    double hi = std::stod(sm[3]);
                    if(lo == hi)
                        return false;
                        
                    pArray->add(UserDefinedParameter(fullString, sm[1].str(), lo, hi));
                }
                else
                {
                    return false;
                }
            }
        }
        else if(String(userString).startsWith("ci"))
        {
            pArray->add(UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::ConstInt));
        }
        else if(String(userString).startsWith("cf"))
        {
            pArray->add(UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::ConstFloat));
        }
        else if(String(userString).startsWith("cs"))
        {
            pArray->add(UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::ConstString));
        }
        else if(String(userString).length() == 0)
        {
            pArray->add(UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::Ignore));
        }
        else if(String(userString).startsWith("ex"))
        {
            pArray->add(UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::Expression));
        }
        else
        {
            return false;
        }
        
        parameterString = parameterMatch.suffix();
    }

    return true;
}

String CustomOscBase::getOscPath()
{
    return oscPath;
}

