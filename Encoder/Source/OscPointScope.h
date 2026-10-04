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

// Which points an OSC target (standard or custom) sends. Shared by StandardOscTarget and
// CustomOscTarget so both get the same "Select..." behaviour in the targets grid.
enum class OscPointScope
{
    AllSources = 0,
    AllGroups,
    AllSourcesAndGroups,
    Select
};

inline String getOscPointScopeDisplayName(OscPointScope scope)
{
    switch (scope)
    {
        case OscPointScope::AllSources:          return "All Sources";
        case OscPointScope::AllGroups:            return "All Groups";
        case OscPointScope::AllSourcesAndGroups:  return "All Sources & Groups";
        case OscPointScope::Select:                return "Select...";
        default:                                   return "";
    }
}

// Selected source/group indices, used only when scope == Select. Indices are positions into the
// AmbiSourceSet's sources/groups collections - they're not stable across reordering, adding, or
// removing sources/groups (there's no persistent cross-session point identity in this codebase to
// key off instead), which is why the selection dialog carries an explicit warning about this.
struct OscPointSelection
{
    Array<int> sourceIndices;
    Array<int> groupIndices;

    static String joinIndices(const Array<int>& indices)
    {
        StringArray tokens;
        for (auto i : indices)
            tokens.add(String(i));
        return tokens.joinIntoString(",");
    }

    static Array<int> parseIndices(const String& text)
    {
        Array<int> result;
        for (auto& token : StringArray::fromTokens(text, ",", ""))
            if (token.trim().isNotEmpty())
                result.add(token.trim().getIntValue());
        return result;
    }
};
