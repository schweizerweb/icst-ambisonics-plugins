/*
  ==============================================================================

    SimpleGraph.h
    Created: 13 Nov 2019 10:34:34am
    Author:  chris

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

//==============================================================================
/*
*/
class SimpleGraph    : public Component
{
public:
	enum ScalingMode { Linear, Logarithmic };

    SimpleGraph();
	virtual ~SimpleGraph();

	void paint (Graphics&) override;
	virtual void paintData(Graphics&) = 0;
    void resized() override;

protected:
	void getStepSize(double minValue, double maxValue, int maxNbOfSteps, double* pStepSize, String* pAnnotationFormat, double axisScaler) const;
	Point<double> mapValues(double x, double y) const;
	void setDisplayRange(ScalingMode modeX, Range<double> xRange, ScalingMode modeY, Range<double> yRange);
	void drawYAxis(Graphics&) const;
	void drawXAxis(Graphics&) const;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleGraph)

protected:
	std::unique_ptr<Rectangle<int>> graphArea;
	bool fullGridFlag;
	String labelAxisX;
	String labelAxisY;

private:
	std::unique_ptr<Range<double>> displayRangeX;
	std::unique_ptr<Range<double>> displayRangeY;
	ScalingMode scalingModeX;
	ScalingMode scalingModeY;
};
