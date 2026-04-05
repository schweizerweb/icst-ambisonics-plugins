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
#include <JuceHeader.h>

class HelpOscSyntax  : public Component
{
public:
    HelpOscSyntax ();
    ~HelpOscSyntax() override;

    void showInfo(String info);

    void paint (juce::Graphics& g) override;
    void resized() override;
    void broughtToFront() override;
    void mouseUp (const juce::MouseEvent& e) override;

private:
    std::unique_ptr<XmlElement> treeXml;
    std::unique_ptr<TreeViewItem> rootItem;
    std::unique_ptr<juce::TreeView> treeViewOscSyntax;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HelpOscSyntax)
};
