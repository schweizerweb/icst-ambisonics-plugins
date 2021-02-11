/*
  ==============================================================================

    CsvImportExport.h
    Created: 7 Feb 2021 10:36:36pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../../Common/Constants.h"
#define SEPARATOR ";"

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
    static bool importFromCsv(AmbiSettings* pAmbiSettings, AmbiSpeakerSet* pSpeakerSet)
    {
        FileChooser chooser("Import from CSV", File(), "*.csv");
        bool ok = chooser.browseForFileToOpen();
        if (!ok)
        {
            return false;
        }

        FileInputStream stream(chooser.getResult());
        Array<CsvDataSet> points;
        int indexPlus1 = 1;
        double maxDistance = 0.0;
        while (!stream.isExhausted())
        {
            String line = stream.readNextLine();
            StringArray arr;
            if (arr.addTokens(line, SEPARATOR, String()) >= 3)
            {
                CsvDataSet set;
                set.a = Constants::GradToRad(arr[0].getDoubleValue());
                set.e = Constants::GradToRad(arr[1].getDoubleValue());
                set.d = arr[2].getDoubleValue();
                set.name = arr.size() > 3 && arr[3].isNotEmpty() ? arr[3] : String(indexPlus1);
                set.color = arr.size() > 4 && arr[4].isNotEmpty() ? Colour::fromString(arr[4]) : TrackColors::getSpeakerColor();
                set.gain = arr.size() > 5 && arr[5].isNotEmpty() ? Decibels::decibelsToGain(arr[5].getDoubleValue()) : 1.0;
                points.add(set);
                maxDistance = jmax(maxDistance, set.d);
                indexPlus1++;
            }
        }

        maxDistance = jmax(1.0, maxDistance);
        pAmbiSettings->setDistanceScaler(maxDistance);
        pSpeakerSet->clear();
        for (int i = 0; i < points.size(); i++)
        {
            Point3D<double> p;
            p.setAed(points[i].a, points[i].e, points[i].d / maxDistance);
            pSpeakerSet->addNew(Uuid().toString(), p, points[i].name, points[i].color);
            pSpeakerSet->setGain(i, points[i].gain);
        }

        return !points.isEmpty();
    }

    static bool exportToCsv(AmbiSpeakerSet* pSpeakerSet)
    {
        FileChooser chooser("Import from CSV", File(), "*.csv");
        bool ok = chooser.browseForFileToSave(true);
        if (!ok)
        {
            return false;
        }

        if(chooser.getResult().exists())
        {
            chooser.getResult().deleteFile();
        }

        FileOutputStream stream(chooser.getResult());
        for(int i = 0; i < pSpeakerSet->size(); i++)
        {
            AmbiSpeaker* s = pSpeakerSet->get(i);
            stream.writeText(
                String(Constants::RadToGrad(s->getPoint()->getAzimuth()))
                + SEPARATOR
                + String(Constants::RadToGrad(s->getPoint()->getElevation()))
                + SEPARATOR
                + String(s->getPoint()->getDistance())
                + SEPARATOR
                + s->getName()
                + SEPARATOR
                + s->getColor().toString()
                + SEPARATOR
                + String(Decibels::gainToDecibels(s->getGain()))
                + NewLine::getDefault(), false, false, nullptr);
        }
        
        return true;
    }
};
