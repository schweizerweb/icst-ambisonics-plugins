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

#include "OSCLogComponent.h"

OSCLogComponent::OSCLogComponent (StatusMessageHandler* _pStatusHandler)
    : pStatusHandler(_pStatusHandler)
{
    textLog.reset (new juce::TextEditor ("textLog"));
    addAndMakeVisible (textLog.get());
    textLog->setMultiLine (true);
    textLog->setReturnKeyStartsNewLine (true);
    textLog->setReadOnly (false);
    textLog->setScrollbarsShown (true);
    textLog->setCaretVisible (true);
    textLog->setPopupMenuEnabled (true);
    textLog->setText (juce::String());

    toggleOn.reset (new juce::ToggleButton ("toggleOn"));
    addAndMakeVisible (toggleOn.get());
    toggleOn->setButtonText (TRANS("On"));
    toggleOn->addListener (this);
    toggleOn->setToggleState (true, juce::dontSendNotification);

    setSize (600, 400);

    textLog->setTextToShowWhenEmpty("no incoming OSC", Colours::lightcoral);
    pStatusHandler->registerDetailLog(this);
}

OSCLogComponent::~OSCLogComponent()
{
    pStatusHandler->unregisterDetailLog(this);

    textLog = nullptr;
    toggleOn = nullptr;
}

void OSCLogComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff323e44));
}

void OSCLogComponent::resized()
{
    textLog->setBounds (8, 8, getWidth() - 16, getHeight() - 16);
    toggleOn->setBounds (getWidth() - 8 - 56, 16, 56, 24);
}

void OSCLogComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == toggleOn.get())
    {
        if(toggleOn->getToggleState())
            pStatusHandler->registerDetailLog(this);
        else
            pStatusHandler->unregisterDetailLog(this);
    }
}

void OSCLogComponent::notify(StatusMessage msg)
{
    textLog->setColour(TextEditor::textColourId, msg.messageStyle == StatusMessage::Error ? Colours::red : Colours::limegreen);

    textLog->moveCaretToEnd();
    String timeString = msg.timestamp.toString(false, true, true, true);
    textLog->insertTextAtCaret(timeString + "\t " + msg.message + NewLine::getDefault());

    // truncate periodically
    if(textLog->getText().length() > 20000)
    {
        textLog->setText(textLog->getText().getLastCharacters(10000));
    }
}

void OSCLogComponent::notifyOverflow(int discardedCount)
{
    textLog->setColour(TextEditor::textColourId, Colours::red);
    textLog->moveCaretToEnd();
    textLog->insertTextAtCaret("Too many messages to display, " + String(discardedCount) + " elements discarded");
}
