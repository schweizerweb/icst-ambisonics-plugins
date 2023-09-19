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

//==============================================================================
/*
*/
class SimpleGraph    : public Component
{
public:
	enum ScalingMode { Linear, Logarithmic, LogarithmicFrequency };

    SimpleGraph();
	~SimpleGraph() override;

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
	std::unique_ptr<Range<double>> displayRangeX;
	std::unique_ptr<Range<double>> displayRangeY;
    bool fullGridFlag;
	String labelAxisX;
	String labelAxisY;

private:
	ScalingMode scalingModeX;
	ScalingMode scalingModeY;
};
