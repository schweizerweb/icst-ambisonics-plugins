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
#include "CsvImportExport.h"
#include "ExternalImportExport.h"

#define MENU_IMPORTEXPORT_CSV_IMPORT    1
#define MENU_IMPORTEXPORT_CSV_EXPORT    2
#define MENU_IMPORTEXPORT_CSV_HELP      3
#define MENU_IMPORTEXPORT_EXT_IMPORT    4
#define MENU_IMPORTEXPORT_EXT_EXPORT    5
#define MENU_IMPORTEXPORT_EXT_HELP      6

class ImportExport
{
public:
    static int idCount()
    {
        return 6;
    }

    static void appendSubMenu(PopupMenu* pMenu, int menuIdOffset = 0)
    {
        PopupMenu mCsv;
        mCsv.addItem(menuIdOffset + MENU_IMPORTEXPORT_CSV_IMPORT, "Import");
        mCsv.addItem(menuIdOffset + MENU_IMPORTEXPORT_CSV_EXPORT, "Export");
        mCsv.addItem(menuIdOffset + MENU_IMPORTEXPORT_CSV_HELP, "Help");
        pMenu->addSubMenu("CSV", mCsv);

        PopupMenu mExt;
        mExt.addItem(menuIdOffset + MENU_IMPORTEXPORT_EXT_IMPORT, "Import");
        mExt.addItem(menuIdOffset + MENU_IMPORTEXPORT_EXT_EXPORT, "Export");
        mExt.addItem(menuIdOffset + MENU_IMPORTEXPORT_EXT_HELP, "Help");
        pMenu->addSubMenu("ICST Ambisonics Externals (Max)", mExt);
    }

    static bool handleImportExport(int menuId, int menuIdOffset, AmbiDataSet* pDataSet, Component* pBtn, bool keepExistingData)
    {
        int result = menuId - menuIdOffset;

        switch (result)
        {
        case MENU_IMPORTEXPORT_CSV_IMPORT:
            {
                auto ret = CsvImportExport::importFromCsv(pDataSet, keepExistingData);
                if (ret == CSV_IMPORT_SUCCESS)
                {
                    return true;
                }
                else if (ret == CSV_IMPORT_FAIL)
                {
                    AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "CSV Import", "Data import failed, please refer to file specification...");
                    return true;
                }
            }
            break;
        case MENU_IMPORTEXPORT_CSV_EXPORT:
            {
                auto ret = CsvImportExport::exportToCsv(pDataSet);
                if (ret == CSV_IMPORT_SUCCESS)
                {
                    AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "CSV Export", "Export completed successfully");
                }
                else if (ret == CSV_IMPORT_FAIL)
                {
                    AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "CSV Export", "Export failed!");
                }
            }
            break;
        case MENU_IMPORTEXPORT_CSV_HELP:
            {
                std::unique_ptr<Label> label = std::make_unique<Label>();
                label->setSize(600, 140);
                label->setText("Required CSV format:\nAzimuth[Degrees];Elevation[Degrees];Distance[m];{Name};{Color[ARGB Hex]};{Gain[dB]}\nParameters in {} are optional, semicolons can be omitted after last used parameter.\nIf parameter 'Name' is not specified, the channel number will be used as name.\n\nExample:\n315;0;10\n45;0;9.5\n10;-5;4;Subwoofer;ffff0000;-6", dontSendNotification);
                label->setJustificationType(Justification::left);
                CallOutBox::launchAsynchronously(std::move(label), pBtn->getBounds(), pBtn);
            }
            break;
        case MENU_IMPORTEXPORT_EXT_IMPORT:
            {
                auto ret = ExternalImportExport::importFromFile(pDataSet, keepExistingData);
                if (ret == EXT_IMPORT_SUCCESS)
                {
                    return true;
                }
                else if (ret == EXT_IMPORT_FAIL)
                {
                    AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "AmbiExternal format import", "Data import failed, please refer to file specification...");
                }
            }
            break;
        case MENU_IMPORTEXPORT_EXT_EXPORT:
            {
                auto ret = ExternalImportExport::exportToFile(pDataSet);
                if (ret == EXT_IMPORT_SUCCESS)
                {
                    AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "AmbiExternal format export", "Export completed successfully");
                }
                else if (ret == EXT_IMPORT_FAIL)
                {
                    AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "AmbiExternal format export", "Export failed!");
                }
            }
            break;
        case MENU_IMPORTEXPORT_EXT_HELP:
            {
                std::unique_ptr<Label> label = std::make_unique<Label>();
                label->setSize(600, 140);
                label->setText("ICST AmbiExternals (Max MSP) format:\n{index}, aed {index} {azimuth[degrees]} {elevation[degrees]} {distance[m]} {status};\n\nThe {status} parameter is currently not used. It is ignored at import and set to 1 at export.\n\nExample:\n1, aed 1 315 0 1 1;", dontSendNotification);
                label->setJustificationType(Justification::left);
                CallOutBox::launchAsynchronously(std::move(label), pBtn->getBounds(), pBtn);
            }
            break;
        }

        return false;
    }
};
