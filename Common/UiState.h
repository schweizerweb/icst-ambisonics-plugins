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

// Small user-interface conveniences that should outlive a single plugin instance: which folder a
// file dialog last used, and where the user left a dialog window. Deliberately NOT part of the
// plugin's own state (EncoderSettings / the DAW session) - these belong to the person at the
// machine, not to the project, so they persist across instances, across projects and across hosts.
//
// The backing PropertiesFile is opened, used and closed within each call, and deliberately NOT
// cached in a static. juce::PropertiesFile privately inherits juce::Timer, every Timer holds a
// SharedResourcePointer<TimerThread>, and the last Timer destroyed tears that thread down - which
// asserts and crashes if the MessageManager has already gone ("a timer has outlived the platform
// event system", juce_Timer.cpp). A static here would be destroyed at process teardown, long after
// the message system, and did exactly that. Keeping the whole lifetime inside one UI action keeps
// it safely inside the message loop.
//
// The file is a handful of keys, so the per-call read/parse is irrelevant next to opening a dialog.
//
// Message thread only - every caller is a UI action.
class UiState
{
public:
    // Keys for the two kinds of memory. Named constants rather than bare literals at the call
    // sites, because a typo'd key is silent - it just reads back empty forever.
    struct Folders
    {
        static constexpr const char* animatorScene     = "folder.animatorScene";
        static constexpr const char* presetImportExport = "folder.presetImportExport";
        static constexpr const char* pointsCsv          = "folder.pointsCsv";
        static constexpr const char* pointsTxt          = "folder.pointsTxt";
        static constexpr const char* speakerBackup      = "folder.speakerBackup";
    };

    struct Windows
    {
        static constexpr const char* animatorClipEditor = "window.animatorClipEditor";
        static constexpr const char* animatorActionEdit = "window.animatorActionEdit";
        static constexpr const char* animatorTimeline   = "window.animatorTimeline";
        static constexpr const char* animatorImportScene = "window.animatorImportScene";
        static constexpr const char* preferences        = "window.preferences";
    };

    // ---- last-used folders ---------------------------------------------------------------------

    // Where a FileChooser for this purpose should open. Pass suggestedFileName for a save dialog so
    // the name is pre-filled; fallbackFolder is used only when nothing has been remembered yet.
    //
    // A remembered folder that no longer exists (removed drive, deleted project folder) is ignored
    // rather than handed to the chooser, which would otherwise open somewhere arbitrary.
    static juce::File startingFile(const juce::String& key,
                                   const juce::String& suggestedFileName = {},
                                   const juce::File& fallbackFolder = {})
    {
        auto folder = juce::File(properties().getValue(key));

        if (!folder.isDirectory())
            folder = fallbackFolder;

        if (!folder.isDirectory())
            return suggestedFileName.isEmpty() ? juce::File()
                                               : juce::File::getSpecialLocation(juce::File::userHomeDirectory)
                                                     .getChildFile(suggestedFileName);

        return suggestedFileName.isEmpty() ? folder : folder.getChildFile(suggestedFileName);
    }

    // Call with whatever the user actually picked - a file or a directory, either works. Takes the
    // containing folder for a file so that the next dialog opens alongside it.
    static void rememberFolder(const juce::String& key, const juce::File& chosen)
    {
        const auto folder = chosen.isDirectory() ? chosen : chosen.getParentDirectory();

        if (!folder.isDirectory())
            return;

        properties().setValue(key, folder.getFullPathName());
        properties().saveIfNeeded();
    }

    // ---- dialog positions ----------------------------------------------------------------------

    // Restores only the POSITION the user left this dialog at, keeping whatever size the dialog has
    // already given itself - these dialogs size themselves from their content (and some, like
    // MovementClipEditor, resize as the user switches movement type), so restoring a stale saved
    // size would fight with that.
    //
    // Falls back to centring when there's nothing remembered. Off-screen recovery comes free:
    // ResizableWindow::restoreWindowStateFromString clips the bounds against every display and
    // relocates the window when less than 32x32 px of it would be visible, which covers the
    // second-monitor-unplugged case without us re-deriving any display geometry.
    // Returns true if a remembered position was actually applied.
    //
    // centreIfUnknown = false when the caller has already placed the window somewhere sensible of
    // its own (centred on its parent, say) and only wants that overridden by a real saved position.
    //
    // restoreSize = true only for windows the user can actually resize. For the fixed-size dialogs
    // it must stay false: their size comes from their content, so a stale saved size would either
    // clip the content or leave dead space.
    static bool restorePosition(juce::ResizableWindow& window, const juce::String& key,
                                bool centreIfUnknown = true, bool restoreSize = false)
    {
        const auto saved = properties().getValue(key);

        juce::StringArray tokens;
        tokens.addTokens(saved, false);
        tokens.removeEmptyStrings();

        const bool haveSize = restoreSize && tokens.size() >= 4
                              && tokens[2].getIntValue() > 0 && tokens[3].getIntValue() > 0;

        if (tokens.size() >= 2)
        {
            // Feed back the size the window will actually occupy together with the saved top-left,
            // so JUCE validates the real rectangle.
            const juce::String state = tokens[0] + " " + tokens[1]
                                     + " " + (haveSize ? tokens[2] : juce::String(window.getWidth()))
                                     + " " + (haveSize ? tokens[3] : juce::String(window.getHeight()));

            if (window.restoreWindowStateFromString(state))
                return true;
        }

        if (centreIfUnknown)
            window.centreWithSize(window.getWidth(), window.getHeight());

        return false;
    }

    static void rememberPosition(const juce::ResizableWindow& window, const juce::String& key)
    {
        const auto bounds = window.getBounds();

        // A window that was never shown, or already torn down, reports an empty/degenerate rect -
        // storing that would make the next open restore nonsense.
        if (bounds.getWidth() <= 0 || bounds.getHeight() <= 0)
            return;

        properties().setValue(key, juce::String(bounds.getX()) + " " + juce::String(bounds.getY())
                                   + " " + juce::String(bounds.getWidth())
                                   + " " + juce::String(bounds.getHeight()));
        properties().saveIfNeeded();
    }

    // Keeps a window's top-left where it is but pulls it back on-screen if its new size pushed it
    // off. For dialogs that resize themselves after construction, where re-centring would yank the
    // window out from under the user's cursor mid-edit.
    static void constrainToDisplay(juce::ResizableWindow& window)
    {
        const auto bounds = window.getBounds();
        window.restoreWindowStateFromString(juce::String(bounds.getX()) + " " + juce::String(bounds.getY())
                                            + " " + juce::String(bounds.getWidth())
                                            + " " + juce::String(bounds.getHeight()));
    }

private:
    static juce::PropertiesFile& properties()
    {
        static std::unique_ptr<juce::PropertiesFile> file = [
        ]
        {
            juce::PropertiesFile::Options options;
            options.applicationName     = "ICST AmbiPlugins";
            options.filenameSuffix      = "settings";
            options.folderName          = "ICST AmbiPlugins";
            options.osxLibrarySubFolder = "Application Support";
            options.commonToAllUsers    = false;

            return std::make_unique<juce::PropertiesFile>(options);
        }();

        return *file;
    }
};
