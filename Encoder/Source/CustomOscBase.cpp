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
    channelSelectionMode = ChannelSelectionMode::Undefined;
}

bool CustomOscBase::setOscPath(String path, String* pErrorMessage)
{
    int index;

    channelSelectionMode = ChannelSelectionMode::Undefined;
    parametersInPath.clear();
    realParameters.clear();
    oscPath = "";
    
    std::regex rFixedChannelIndex("[ ]*[cC][iI][ ]*([0-9]+)[ ]*[:](.*)");
    std::regex rFixedChannelName("[ ]*[cC][nN][ ]*(.+)[ ]*[:](.*)");
    std::smatch parameterMatch;
    
    std::string pathStr = path.toStdString();
    if(regex_search(pathStr, parameterMatch, rFixedChannelIndex))
    {
        auto chStr = parameterMatch[1].str();
        path = parameterMatch[2].str();
        constantChannelIndex = String(chStr).getIntValue();
        if(constantChannelIndex < 1)
        {
            *pErrorMessage = "Invalid channel index " + String(constantChannelIndex);
            return false;
        }
        channelSelectionMode = ChannelSelectionMode::ConstantIndex;
    }
    else if(regex_search(pathStr, parameterMatch, rFixedChannelName))
    {
        auto chStr = parameterMatch[1].str();
        path = parameterMatch[2].str();
        constantChannelName = chStr;
        channelSelectionMode = ChannelSelectionMode::ConstantName;
    }
    
    if(!path.startsWith("/"))
    {
        *pErrorMessage = "Invalid OSC address - missing '/' or channel selection at the beginning";
        return false;
    }
    
    index = 0;
    while(index < path.length())
    {
        // termination
        if(path[index] == ' ')
            break;
        
        if(path[index] == '{')
        {
            UserDefinedParameter* param = analyzeEscapedString(path, &index, pErrorMessage);
            if(param != nullptr)
            {
                // check for channel or name
                if(!checkChannelSelection(param))
                {
                    *pErrorMessage = "Redefinition of channel selection at parameter " + param->getOriginalString();
                    return false;
                }
                
                parametersInPath.add(param);
                oscPath += param->getOriginalString();
            }
            else
            {
                return false;
            }
        }
        else
        {
            oscPath += path[index];
        }
        
        index++;
    }
    
    // arguments
    while(index < path.length())
    {
        if(path[index] == ' ')
            index++;
        else if(path[index] == '{')
        {
            UserDefinedParameter* param = analyzeEscapedString(path, &index, pErrorMessage);
            if(param != nullptr)
            {
                // check for channel or name
                if(!checkChannelSelection(param))
                {
                    *pErrorMessage = "Redefinition of channel selection at parameter " + param->getOriginalString();
                    return false;
                }
                
                realParameters.add(param);
                index++;
            }
            else
            {
                return false;
            }
        }
        else
        {
            *pErrorMessage = "Invalid character " + String(path[index]) + " at position " + String(index);
            return false;
        }
    }
    
    return true;
}

bool CustomOscBase::checkChannelSelection(UserDefinedParameter* pParam)
{
    if(pParam->getType() == UserDefinedParameter::ParameterType::Index
    || pParam->getType() == UserDefinedParameter::ParameterType::Name
    )
    {
        if(channelSelectionMode != ChannelSelectionMode::Undefined)
            return false;
        else
        {
            channelSelectionMode = ChannelSelectionMode::Parameter;
        }
    }
    
    return true;
}

UserDefinedParameter* CustomOscBase::analyzeEscapedString(String fullPath, int* pIndex, String* pErrorMessage)
{
    String fullString;
    String commandString;
    String paramString;
    
    if(fullPath[*pIndex] != '{')
    {
        *pErrorMessage = "Programm error, parameter should start with '{'";
        return nullptr;
    }
    
    fullString = "{";
    
    while(++(*pIndex) < fullPath.length())
    {
        fullString += fullPath[*pIndex];
        
        // ignore whitespace
        if(fullPath[*pIndex] == ' ')
            continue;
    
        // comma delimiter to separate command and user parameters
        if(fullPath[*pIndex] == ',')
            break;
        
        // end of parameter (command only)
        if(fullPath[*pIndex] == '}')
            break;
        
        commandString += fullPath[*pIndex];
    }
    
    if(*pIndex >= fullPath.length())
    {
        *pErrorMessage = "Closing '}' missing";
        return nullptr;
    }
    else if(fullPath[*pIndex] == '}')
    {
        // means, it's a single command without params
        commandString = commandString.toLowerCase();
        if(commandString == "a")
            return new UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::A);
        else if(commandString == "e")
            return new UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::E);
        else if(commandString == "d")
            return new UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::D);
        else if(commandString == "x")
            return new UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::X);
        else if(commandString == "y")
            return new UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::Y);
        else if(commandString == "z")
            return new UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::Z);
        else if(commandString == "i")
            return new UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::Index);
        else if(commandString == "n")
            return new UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::Name);
        else if(commandString == "c")
            return new UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::Color);
        else if(commandString == "g")
            return new UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::Gain);
        else if(commandString == "sa")
            return new UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::ScaledA);
        else if(commandString == "se")
            return new UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::ScaledE);
        else if(commandString == "sd")
            return new UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::ScaledD);
        else if(commandString == "sx")
            return new UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::ScaledX);
        else if(commandString == "sy")
            return new UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::ScaledY);
        else if(commandString == "sz")
            return new UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::ScaledZ);
        else if(commandString == "")
            return new UserDefinedParameter(fullString, UserDefinedParameter::ParameterType::Ignore);
        else
        {
            *pErrorMessage = "Unknown command: " + commandString;
            return nullptr;
        }
    }
    else if(fullPath[*pIndex] == ',')
    {
        // rest is command params
        int nestedBracketLevel = 0;
        while(++(*pIndex) < fullPath.length())
        {
            fullString += fullPath[*pIndex];
            paramString += fullPath[*pIndex];
            
            if(fullPath[*pIndex] == '{')
                nestedBracketLevel++;
            
            if(fullPath[*pIndex] == '}')
            {
                if(nestedBracketLevel > 0)
                    nestedBracketLevel--;
                else
                    break;
            }
        }
        if(*pIndex >= fullPath.length())
        {
            *pErrorMessage = "Closing '}' missing";
            return nullptr;
        }
        
        // build parameter
        if(commandString == "expr")
        {
            auto p = new UserDefinedParameter(fullString, UserDefinedParameter::Expression);
            // TODO: try evaluate
            return p;
        }
        else if(commandString == "ci")
        {
            return new UserDefinedParameter(fullString, UserDefinedParameter::ConstInt);
        }
        else if(commandString == "cf")
        {
            return new UserDefinedParameter(fullString, UserDefinedParameter::ConstFloat);
        }
        else if(commandString == "cs")
        {
            return new UserDefinedParameter(fullString, UserDefinedParameter::ConstString);
        }
        else if(commandString.matchesWildcard("s?", true))
        {
            std::regex rTwoValues("s([xyzaed])[ ]*,[ ]*([-+]?[0-9]*\\.?[0-9]+|[0-9]+)[ ]*,[ ]*([-+]?[0-9]*\\.?[0-9]+|[0-9]+)");
            std::regex rThreeValues("s([xyze])[ ]*,[ ]*([-+]?[0-9]*\\.?[0-9]+|[0-9]+)[ ]*,[ ]*([-+]?[0-9]*\\.?[0-9]+|[0-9]+)[ ]*,[ ]*([-+]?[0-9]*\\.?[0-9]+|[0-9]+)");
            std::smatch sm;
            std::string userString = commandString.toStdString() + "," + paramString.toStdString();
            if(regex_search(userString, sm, rThreeValues))
            {
                double lo = std::stod(sm[2]);
                double zero = std::stod(sm[3]);
                double hi = std::stod(sm[4]);
                if(approximatelyEqual(lo, hi) && approximatelyEqual(lo, zero))
                {
                    *pErrorMessage = "Invalid arguments for command " + commandString;
                    return nullptr;
                }
                        
                return new UserDefinedParameter(fullString, sm[1].str(), lo, hi, zero);
            }
            else if(regex_search(userString, sm, rTwoValues))
            {
                double lo = std::stod(sm[2]);
                double hi = std::stod(sm[3]);
                if(approximatelyEqual(lo, hi))
                {
                    *pErrorMessage = "Invalid arguments for command " + commandString;
                    return nullptr;
                }
                    
                return new UserDefinedParameter(fullString, sm[1].str(), lo, hi);
            }
            else
            {
                *pErrorMessage = "Invalid number of arguments for command " + commandString;
                return nullptr;
            }
            /*
            if(paramString.matchesWildcard("*,*,*", true))
            {
                String first = paramString
                    .upToFirstOccurrenceOf(",", false, true)
                    .trimCharactersAtStart(" ")
                    .trimCharactersAtEnd(" ");
                String second = paramString
                    .fromFirstOccurrenceOf(",", false, true)
                    .upToFirstOccurrenceOf(",", false, true)
                    .trimCharactersAtStart(" ")
                    .trimCharactersAtEnd(" ");
                String third = paramString
            }
            else if(paramString.matchesWildcard("*,*", true))
            {
                
            }
            else
            {
                *pErrorMessage = "Wrong number of arguments for command " + commandString;
                return errorParam;
            }
             */
        }
        else
        {
            *pErrorMessage = "Unknown command with parameters: " + commandString;
            return nullptr;
        }
    }
    
    
    *pErrorMessage = "Program error: unhandled error in custom OSC target";
    return nullptr;
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
                    if(approximatelyEqual(lo, hi) && approximatelyEqual(lo, zero))
                        return false;
                            
                    pArray->add(UserDefinedParameter(fullString, sm[1].str(), lo, hi, zero));
                }
                else if(regex_search(userString, sm, rTwoValues))
                {
                    double lo = std::stod(sm[2]);
                    double hi = std::stod(sm[3]);
                    if(approximatelyEqual(lo, hi))
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

