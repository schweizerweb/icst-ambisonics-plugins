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

class CheckBoxCustomComponent : public Component, private  ToggleButton::Listener
{
public:
	CheckBoxCustomComponent(SpeakerSettingsComponent& td) : owner(td)
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
        
        magnitudes = static_cast<double*>(calloc(frequencies.size(), sizeof(double)));
        displayRangeX.reset(new Range<double>(log10(20), log10(sampleRate / 2.0)));
        displayRangeY.reset(new Range<double>(-20, 20));
        graphArea.reset(new Rectangle<int>(0, 0, 150, 40));
	}

	void setRowAndColumn(const int newRow, const int newColumn)
	{
		row = newRow;
		columnId = newColumn;
		toggle.setToggleState(owner.getFlag(columnId, row), dontSendNotification);
        
        FilterInfo* newFilterInfo = owner.getFilterInfo(newRow);
        if(!filterInfo.equals(newFilterInfo))
        {
            filterInfo = *newFilterInfo;
            updateFilterPath();
            repaint();
        }
	}
    
private:
    void resized() override
    {
        toggle.setBounds(0, 0, getHeight(), getHeight());
        transform = AffineTransform::scale(((float)getWidth()-toggle.getWidth()) / graphArea->getWidth(), (float)getHeight() / graphArea->getHeight()).followedBy(AffineTransform::translation(toggle.getWidth(), 0.0f));
    }
    
    void updateFilterPath()
    {
        thumbnailFilterPath.clear();
        
        // draw curve
        dsp::IIR::Coefficients<float>::Ptr coeff = filterInfo.getCoefficients(sampleRate);
        
        if(coeff == nullptr)
        {
            thumbnailFilterPath.startNewSubPath(mapValues(frequencies.getFirst(), 0.0).toFloat());
            thumbnailFilterPath.lineTo(mapValues(frequencies.getLast(), 0.0).toFloat());
        }
        else
        {
            coeff->getMagnitudeForFrequencyArray(frequencies.getRawDataPointer(), magnitudes, frequencies.size(), sampleRate);

            for (int i = 0; i < frequencies.size(); i++)
            {
                Point<float> displayPoint = mapValues(frequencies[i], Decibels::gainToDecibels(magnitudes[i])).toFloat();
                
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
        g.drawLine(toggle.getWidth(), getHeight()/2, getWidth(), getHeight()/2);
        
        g.setColour(owner.getFlag(columnId, row) ? Colours::lightgreen : Colours::red);
        g.strokePath(thumbnailFilterPath, *strokeType.get(), transform);
    }
    
    void buttonClicked(Button* b) override
    {
        owner.setFlag(columnId, row, b->getToggleState());
    }
   
    //void mouseDoubleClick(const MouseEvent &event) override
    void mouseUp(const MouseEvent &event) override
    {
        if(!toggle.contains(event.getPosition()))
        {
            CallOutBox::launchAsynchronously(std::make_unique<FilterSettingsComponent>(owner.getFilterInfo(row), owner.getFilterSpecification(), &owner, owner.getFilterPresetHelper()), getScreenBounds().translated(-owner.getScreenX(), -owner.getScreenY()), &owner);
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
    
    FilterInfo filterInfo;
    Array<double> frequencies;
    double sampleRate;
    double* magnitudes;
    std::unique_ptr<Rectangle<int>> graphArea;
    std::unique_ptr<Range<double>> displayRangeX;
    std::unique_ptr<Range<double>> displayRangeY;
    AffineTransform transform;
    
};
