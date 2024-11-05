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

class ImportExport
{
public:
    static bool importExport(AmbiDataSet* pDataSet, Component* pBtn)
    {
        PopupMenu m;
        m.addItem(1, "Import from CSV");
        m.addItem(2, "Export to CSV");
        m.addItem(3, "CSV help");
        m.addSeparator();
        m.addItem(4, "Import from AmbiExternal");
        m.addItem(5, "Export to AmbiExternal");
        m.addItem(6, "AmbiExternal help");
        const int result = m.show();
        if (result == 1)
        {
            auto ret = CsvImportExport::importFromCsv(pDataSet);
            if(ret == CSV_IMPORT_SUCCESS)
            {
                AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "CSV Import", "Data import successful");
                return true;
            }
            else if(ret == CSV_IMPORT_FAIL)
            {
                AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "CSV Import", "Data import failed, please refer to file specification...");
                return true;
            }
        }
        else if (result == 2)
        {
            auto ret = CsvImportExport::exportToCsv(pDataSet);
            if(ret == CSV_IMPORT_SUCCESS)
            {
                AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "CSV Export", "Export completed successfully");
            }
            else if(ret == CSV_IMPORT_FAIL)
            {
                AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "CSV Export", "Export failed!");
            }
        }
        else if (result == 3)
        {
            std::unique_ptr<Label> label = std::make_unique<Label>();
            label->setSize(600, 140);
            label->setText("Required CSV format:\nAzimuth[Degrees];Elevation[Degrees];Distance[m];{Name};{Color[ARGB Hex]};{Gain[dB]}\nParameters in {} are optional, semicolons can be omitted after last used parameter.\nIf parameter 'Name' is not specified, the channel number will be used as name.\n\nExample:\n315;0;10\n45;0;9.5\n10;-5;4;Subwoofer;ffff0000;-6", dontSendNotification);
            label->setJustificationType(Justification::left);
            CallOutBox::launchAsynchronously(std::move(label), pBtn->getBounds(), nullptr);
        }
        else if (result == 4)
        {
            auto ret = ExternalImportExport::importFromFile(pDataSet);
            if(ret == EXT_IMPORT_SUCCESS)
            {
                AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "AmbiExternal format import", "Data import successful");
                return true;
            }
            else if(ret == EXT_IMPORT_FAIL)
            {
                AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "AmbiExternal format import", "Data import failed, please refer to file specification...");
            }
        }
        else if (result == 5)
        {
            auto ret = ExternalImportExport::exportToFile(pDataSet);
            if(ret == EXT_IMPORT_SUCCESS)
            {
                AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "AmbiExternal format export", "Export completed successfully");
            }
            else if(ret == EXT_IMPORT_FAIL)
            {
                AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "AmbiExternal format export", "Export failed!");
            }
        }
        else if (result == 6)
        {
            std::unique_ptr<Label> label = std::make_unique<Label>();
            label->setSize(600, 140);
            label->setText("ICST AmbiExternals (Max MSP) format:\n{index}, aed {index} {azimuth[degrees]} {elevation[degrees]} {distance[m]} {status};\n\nThe {status} parameter is currently not used. It is ignored at import and set to 1 at export.\n\nExample:\n1, aed 1 315 0 1 1;", dontSendNotification);
            label->setJustificationType(Justification::left);
            CallOutBox::launchAsynchronously(std::move(label), pBtn->getBounds(), nullptr);
        }

        return false;
    }
};
