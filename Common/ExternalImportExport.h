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

#define EXT_IMPORT_FAIL -1
#define EXT_IMPORT_SUCCESS 1

class ExternalImportExport
{
private:
    struct ExternalDataSet
    {
        int index;
        double a;
        double e;
        double d;
        String name;
    };

public:
    static int importFromFile(AmbiDataSet* pDataSet, bool keepExistingData)
    {
        FileChooser chooser("Import from TXT", File(), "*.txt");
        bool ok = chooser.browseForFileToOpen();
        if (!ok)
        {
            return 0;
        }

        FileInputStream stream(chooser.getResult());
        Array<ExternalDataSet> points;
        int indexPlus1 = 1;
        while (!stream.isExhausted())
        {
            String line = stream.readNextLine();
            StringArray arr1;
            int nbTokens = arr1.addTokens(line, ",", String());
            if (nbTokens == 2)
            {
                auto indexString = arr1[0];
                
                StringArray arr2;
                auto info = arr1[1].trim();
                nbTokens = arr2.addTokens(info, " ", String());

                auto shouldBeAed = arr2[0];
                auto shouldBeIndex = arr2[1];

                if (shouldBeAed != "aed")
                    return EXT_IMPORT_FAIL;

                if (shouldBeIndex != indexString)
                    return EXT_IMPORT_FAIL;

                ExternalDataSet set;
                set.index = indexString.getIntValue();
                set.a = Constants::GradToRad(arr2[2].getDoubleValue());
                set.e = Constants::GradToRad(arr2[3].getDoubleValue());
                set.d = arr2[4].getDoubleValue();
                // for now, ignore the last value
                set.name = String(indexString);
                points.add(set);
                indexPlus1++;
            }
            else if(nbTokens > 0)
            {
                // fail for rows with less than 3 columns, but ignore empty lines
                return EXT_IMPORT_FAIL;
            }
        }

        if (keepExistingData)
        {
            for (int i = 0; i < pDataSet->size(); i++)
            {
                bool found = false;

                for (int iSet = 0; iSet < points.size(); iSet++)
                {
                    if (points[iSet].index == i + 1)
                    {
                        auto p = points[iSet];
                        pDataSet->setChannelAED(i, p.a, p.e, p.d);
                        pDataSet->setChannelName(i, p.name);
                        pDataSet->setChannelColor(i, TrackColors::getSpeakerColor());
                        pDataSet->setGain(i, 1.0);
                        pDataSet->setEnabled(i, true);
                        found = true;
                        break;
                    }
                }

                if (!found)
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
                pDataSet->addNew(Uuid().toString(), p, points[i].name, TrackColors::getSpeakerColor());
                pDataSet->setGain(i, 1.0);
            }
        }
        return !points.isEmpty() ? EXT_IMPORT_SUCCESS : EXT_IMPORT_FAIL;
    }

    static int exportToFile(AmbiDataSet* pDataSet)
    {
        FileChooser chooser("Export to file", File(), "*.txt");
        bool ok = chooser.browseForFileToSave(true);
        if (!ok)
        {
            return 0;
        }

        if(chooser.getResult().exists())
        {
            chooser.getResult().deleteFile();
        }

        float maxDist = jmax(pDataSet->getMaxDistance(), 1.0f);

        FileOutputStream stream(chooser.getResult());
        for(int i = 0; i < pDataSet->size(); i++)
        {
            AmbiPoint* s = pDataSet->get(i);
            if (s->getEnabled())
            {
                stream.writeText(
                    String(i + 1)
                    + ", aed "
                    + String(i + 1)
                    + " "
                    + String(Constants::RadToGrad(s->getRawPoint()->getAzimuth()))
                    + " "
                    + String(Constants::RadToGrad(s->getRawPoint()->getElevation()))
                    + " "
                    + String(s->getRawPoint()->getDistance() / maxDist)
                    + " "
                    + "1;"
                    + NewLine::getDefault(), false, false, nullptr);
            }
        }
        
        return EXT_IMPORT_SUCCESS;
    }
};
