/*
  ==============================================================================

    EditableTextCustomComponent.h
    Created: 9 Oct 2017 2:04:27pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#pragma once
#include "SpeakerSettingsComponent.h"
#include "FilterSettingsComponent.h"
#define MIN_FREQUENCY_THUMBNAIL    20
#define FREQUENCY_STEP_THUMBNAIL 1.1

class CheckBoxFilterCustomComponent : public Component, private  ToggleButton::Listener
{
public:
	CheckBoxFilterCustomComponent(SpeakerSettingsComponent& td) : owner(td)
	{
        addAndMakeVisible(toggle);
        toggle.addListener(this);
        toggle.setWantsKeyboardFocus(false);
        
        strokeType.reset(new PathStrokeType(1.0f, PathStrokeType::JointStyle::curved));
        
        // prepare graph arrays
        sampleRate = owner.getFilterSpecification()->sampleRate;
        double currentFrequency = MIN_FREQUENCY_THUMBNAIL;
        while(currentFrequency < sampleRate / 2.0)
        {
            frequencies.add(currentFrequency);
            currentFrequency *= FREQUENCY_STEP_THUMBNAIL;
        }

        for (int i = 0; i < MAX_FILTER_COUNT; i++)
        {
            magnitudes[i] = static_cast<double*>(calloc((size_t)frequencies.size(), sizeof(double)));
        }

        displayRangeX.reset(new Range<double>(log10(20), log10(sampleRate / 2.0)));
        displayRangeY.reset(new Range<double>(-30, 30));
        graphArea.reset(new Rectangle<int>(0, 0, 150, 40));
	}

    ~CheckBoxFilterCustomComponent() override
	{
        for (int i = 0; i < MAX_FILTER_COUNT; i++)
        {
            free(magnitudes[i]);
        }
	}

	void setRowAndColumn(const int newRow, const int newColumn)
	{
		row = newRow;
		columnId = newColumn;
		toggle.setToggleState(owner.getFlag(columnId, row), dontSendNotification);
        
        FilterBankInfo* newFilterInfo = owner.getFilterInfo(newRow);
        if(firstUpdate || !filterInfo.equals(newFilterInfo))
        {
            filterInfo = *newFilterInfo;
            updateFilterPath();
            repaint();
        }

        firstUpdate = false;
	}
    
private:
    void resized() override
    {
        toggle.setBounds(0, 0, getHeight(), getHeight());
        transform = AffineTransform::scale((float(getWidth())-toggle.getWidth()) / float(graphArea->getWidth()), (float)getHeight() / graphArea->getHeight()).followedBy(AffineTransform::translation(float(toggle.getWidth()), 0.0f));
    }
    
    void updateFilterPath()
    {
        thumbnailFilterPath.clear();
        
        // draw curve
        if(!filterInfo.anyActive())
        {
            thumbnailFilterPath.startNewSubPath(mapValues(frequencies.getFirst(), 0.0).toFloat());
            thumbnailFilterPath.lineTo(mapValues(frequencies.getLast(), 0.0).toFloat());
        }
        else
        {
            std::vector<dsp::IIR::Coefficients<float>::Ptr> coeffs;
            filterInfo.getCoefficients(sampleRate, &coeffs);

            int activeFilterCount = int(coeffs.size());
            for (int iCoeff = 0; iCoeff < activeFilterCount && iCoeff < MAX_FILTER_COUNT; iCoeff++)
            {
                coeffs[(size_t)iCoeff]->getMagnitudeForFrequencyArray(frequencies.getRawDataPointer(), magnitudes[iCoeff], (size_t)frequencies.size(), sampleRate);
            }

            for (int i = 0; i < frequencies.size(); i++)
            {
                double magnitudeSum = 1.0;
                for (int iMag = 0; iMag < activeFilterCount; iMag++)
                    magnitudeSum *= magnitudes[iMag][i];

                Point<float> displayPoint = mapValues(frequencies[i], Decibels::gainToDecibels(magnitudeSum)).toFloat();
                
                if (i == 0)
                    thumbnailFilterPath.startNewSubPath(displayPoint);
                else
                    thumbnailFilterPath.lineTo(displayPoint);
            }
        }
    }

    void paint(Graphics &g) override
    {
        g.fillAll(getLookAndFeel().findColour(ListBox::backgroundColourId));
        g.setColour(Colours::lightgrey);
        g.drawLine(float(toggle.getWidth()), getHeight()/2.0f, float(getWidth()), getHeight()/2.0f);
        
        g.setColour(owner.getFlag(columnId, row) ? Colours::lightgreen : Colours::red);
        g.strokePath(thumbnailFilterPath, *strokeType.get(), transform);
    }
    
    void buttonClicked(Button* b) override
    {
        owner.setFlag(columnId, row, b->getToggleState());
    }
   
    void mouseUp(const MouseEvent &event) override
    {
        if(!toggle.contains(event.getPosition()))
        {
            CallOutBox::launchAsynchronously(std::make_unique<FilterSettingsComponent>(owner.getFilterInfo(row), owner.getFilterSpecification(), &owner, owner.getFilterPresetHelper(), row), getScreenBounds().translated(-owner.getScreenX(), -owner.getScreenY()), &owner);
        }
    }
    
    Point<double> mapValues(double x, double y) const
    {
        double mx = graphArea->getX()
            + jmax(0.0, graphArea->getWidth() / displayRangeX->getLength() * ((log10(x)) - displayRangeX->getStart()));
        double my = graphArea->getBottom()
            - graphArea->getHeight() / displayRangeY->getLength() * (y - displayRangeY->getStart());

        return Point<double>(mx, my);
    }
    
private:
	SpeakerSettingsComponent& owner;
	int row, columnId;
    ToggleButton toggle;
    Path thumbnailFilterPath;
    std::unique_ptr<PathStrokeType> strokeType;
    
    FilterBankInfo filterInfo;
    Array<double> frequencies;
    double sampleRate;
    double* magnitudes[MAX_FILTER_COUNT];
    std::unique_ptr<Rectangle<int>> graphArea;
    std::unique_ptr<Range<double>> displayRangeX;
    std::unique_ptr<Range<double>> displayRangeY;
    AffineTransform transform;
    bool firstUpdate = true;
};
