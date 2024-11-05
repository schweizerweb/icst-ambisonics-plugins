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
#include "../../Common/Constants.h"

#define SEPARATOR ";"
#define CSV_IMPORT_FAIL -1
#define CSV_IMPORT_SUCCESS 1

class CsvImportExport
{
private:
    struct CsvDataSet
    {
        double a;
        double e;
        double d;
        String name;
        Colour color;
        double gain;
    };

public:
    static int importFromCsv(AmbiDataSet* pDataSet, bool keepExistingData)
    {
        FileChooser chooser("Import from CSV", File(), "*.csv");
        bool ok = chooser.browseForFileToOpen();
        if (!ok)
        {
            return 0;
        }

        FileInputStream stream(chooser.getResult());
        Array<CsvDataSet> points;
        int indexPlus1 = 1;
        while (!stream.isExhausted())
        {
            String line = stream.readNextLine();
            StringArray arr;
            int nbTokens = arr.addTokens(line, SEPARATOR, String());
            if (nbTokens >= 3)
            {
                CsvDataSet set;
                set.a = Constants::GradToRad(arr[0].getDoubleValue());
                set.e = Constants::GradToRad(arr[1].getDoubleValue());
                set.d = arr[2].getDoubleValue();
                set.name = arr.size() > 3 && arr[3].isNotEmpty() ? arr[3] : String(indexPlus1);
                set.color = arr.size() > 4 && arr[4].isNotEmpty() ? Colour::fromString(arr[4]) : TrackColors::getSpeakerColor();
                set.gain = arr.size() > 5 && arr[5].isNotEmpty() ? Decibels::decibelsToGain(arr[5].getDoubleValue()) : 1.0;
                points.add(set);
                indexPlus1++;
            }
            else if(nbTokens > 0)
            {
                // fail for rows with less than 3 columns, but ignore empty lines
                return CSV_IMPORT_FAIL;
            }
        }

        if (keepExistingData)
        {
            for (int i = 0; i < pDataSet->size(); i++)
            {
                if (points.size() > i)
                {
                    auto p = points[i];
                    pDataSet->setChannelAED(i, p.a, p.e, p.d);
                    pDataSet->setChannelName(i, p.name);
                    pDataSet->setChannelColor(i, p.color);
                    pDataSet->setGain(i, p.gain);
                    pDataSet->setEnabled(i, true);
                }
                else
                {
                    pDataSet->setEnabled(i, false);
                }
            }
        }
        else
        {
            pDataSet->clear();
            for (int i = 0; i < points.size(); i++)
            {
                Point3D<double> p;
                p.setAed(points[i].a, points[i].e, points[i].d);
                pDataSet->addNew(Uuid().toString(), p, points[i].name, points[i].color);
                pDataSet->setGain(i, points[i].gain);
            }
        }
        return !points.isEmpty() ? CSV_IMPORT_SUCCESS : CSV_IMPORT_FAIL;
    }

    static bool exportToCsv(AmbiDataSet* pDataSet)
    {
        FileChooser chooser("Export to CSV", File(), "*.csv");
        bool ok = chooser.browseForFileToSave(true);
        if (!ok)
        {
            return 0;
        }

        if(chooser.getResult().exists())
        {
            chooser.getResult().deleteFile();
        }

        FileOutputStream stream(chooser.getResult());
        for(int i = 0; i < pDataSet->size(); i++)
        {
            AmbiPoint* s = pDataSet->get(i);
            if (s->getEnabled())
            {
                stream.writeText(
                    String(Constants::RadToGrad(s->getRawPoint()->getAzimuth()))
                    + SEPARATOR
                    + String(Constants::RadToGrad(s->getRawPoint()->getElevation()))
                    + SEPARATOR
                    + String(s->getRawPoint()->getDistance())
                    + SEPARATOR
                    + s->getName()
                    + SEPARATOR
                    + s->getColor().toString()
                    + SEPARATOR
                    + String(Decibels::gainToDecibels(s->getGain()))
                    + NewLine::getDefault(), false, false, nullptr);
            }
        }
        
        return CSV_IMPORT_SUCCESS;
    }
};
