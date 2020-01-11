/*
  ==============================================================================

    SimpleGraph.cpp
    Created: 13 Nov 2019 10:34:34am
    Author:  chris

  ==============================================================================
*/

#include "JuceHeader.h"
#include "SimpleGraph.h"

//==============================================================================
SimpleGraph::SimpleGraph(): fullGridFlag(false)
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.
	scalingModeX = Linear;
	scalingModeY = Linear;
}

SimpleGraph::~SimpleGraph()
{
	displayRangeX = nullptr;
	displayRangeY = nullptr;
}

void SimpleGraph::drawYAxis(Graphics& g) const
{
	double stepSize;
	String annotationFormat;
	getStepSize(displayRangeY->getStart(), displayRangeY->getEnd(), 10, &stepSize, &annotationFormat, 1.0);

	double upperLim = displayRangeY->getEnd() + 0.01*stepSize;
	double currentStep = stepSize * int(displayRangeY->getStart() / stepSize - 1);
	while (currentStep < upperLim) {
		if (currentStep >= displayRangeY->getStart()) {
			Point<float> xy = mapValues(displayRangeX->getStart(), currentStep).toFloat();
			g.drawLine(xy.getX() - 3, xy.getY(), fullGridFlag ? graphArea->getRight() : xy.getX() + 3, xy.getY(), 1.0);
			String buffer = String::formatted(annotationFormat, currentStep);
			g.drawSingleLineText(buffer, int(xy.getX()) - 4, int(xy.getY()) + 4, Justification::right);
		}
		currentStep += stepSize;
	}
}

void SimpleGraph::drawXAxis(Graphics& g) const
{
	double stepSize;
	String annotationFormat;
	getStepSize(displayRangeX->getStart(), displayRangeX->getEnd(), 8, &stepSize, &annotationFormat, 1.0);

	double upperLim = displayRangeX->getEnd() + 0.01*stepSize;
	double currentStep = stepSize * int(displayRangeX->getStart() / stepSize - 1);
	while (currentStep < upperLim) {
		if (currentStep >= displayRangeX->getStart()) {
			double realValue = scalingModeX == Logarithmic ? pow(10.0, currentStep) : currentStep;
			Point<float> xy = mapValues(realValue, displayRangeY->getStart()).toFloat();
			g.drawLine(xy.getX(), xy.getY() + 3, xy.getX(), fullGridFlag ? graphArea->getY() : xy.getY() - 3, 1.0f);
			//String buffer = "10^" + String::formatted(annotationFormat, currentStep);
			String buffer = String::formatted(scalingModeX == Logarithmic ? "%.0f" : annotationFormat, realValue);
			g.drawSingleLineText(buffer, int(xy.getX()), int(xy.getY()) + 14, Justification::horizontallyCentred);
		}
		currentStep += stepSize;
	}
}

void SimpleGraph::paint (Graphics& g)
{
	g.setImageResamplingQuality(Graphics::mediumResamplingQuality);

	// draw graph basics
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

	paintData(g);

	g.setColour(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
	g.fillRect(0, 0, graphArea->getX(), getHeight());
	g.fillRect(0, graphArea->getBottom(), getWidth(), getBottom() - graphArea->getBottom());

	g.setColour(Colours::grey);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

	// draw axes
	g.drawArrow(Line<int>(graphArea->getBottomLeft(), graphArea->getTopLeft().translated(0, -10)).toFloat(), 2, 8, 7);
	g.drawSingleLineText(labelAxisY, graphArea->getX() + 7, graphArea->getY() - 5, Justification::left);
	drawYAxis(g);

	g.drawArrow(Line<int>(graphArea->getBottomLeft(), graphArea->getBottomRight().translated(10, 0)).toFloat(), 2, 8, 7);
	g.drawSingleLineText(labelAxisX, graphArea->getRight(), graphArea->getBottom() - 8, Justification::right);
	drawXAxis(g);
}

void SimpleGraph::resized()
{
	graphArea.reset(new Rectangle<int>(40, 30, getWidth() - 60, getHeight() - 55));
	repaint();
}

void SimpleGraph::getStepSize(double minValue, double maxValue, int maxNbOfSteps, double* pStepSize, String* pAnnotationFormat, double axisScaler) const
{
	Array<double>		stepReference = {1.0, 2.0, 2.5, 5.0, 10.0};
	String		localAnnotation;

	if (maxNbOfSteps < 1)
		maxNbOfSteps = 2;
	double firstGuess = (maxValue - minValue) / maxNbOfSteps;
	if (firstGuess <= 0.0)
		firstGuess = jmax(fabs(minValue), fabs(maxValue)) / 2.0;

	// map the first guess to the range between 1 .. 10
	int logTen = int(log10(firstGuess));

	if (firstGuess < 1.0)
		logTen -= 1;
	double scaler = pow(10.0, logTen);

	// find the worker to select the correct range
	double worker = firstGuess / scaler;

	// look out for the matching step
	int iRef = 0;
	while (stepReference[iRef] < worker) {
		iRef++;
		if (iRef >= stepReference.size()) {
			iRef = stepReference.size() - 1;
			break;
		}
	}

	// calculate the step
	firstGuess = stepReference[iRef] * scaler;

	// build the format reference
	if (iRef == 2)
		logTen--;
	int logTenAnno = int(logTen + log10(axisScaler));
	switch (logTenAnno) {
	case -2:	localAnnotation = "%.2f";	break;
	case -1:	localAnnotation = "%.1f";	break;
	case 0:		localAnnotation = "%.0f";	break;
	case 1:		localAnnotation = "%.0f";	break;
	case 2:		localAnnotation = "%.0f";	break;
	default:	localAnnotation = "%2.3G";
	}

	// finished
	*pStepSize = firstGuess;
	*pAnnotationFormat = localAnnotation;
}


Point<double> SimpleGraph::mapValues(double x, double y) const
{
	double mx = graphArea->getX() 
		+ jmax(0.0, graphArea->getWidth() / displayRangeX->getLength() * ((scalingModeX == Logarithmic ? log10(x) : x) - displayRangeX->getStart()));
	double my = graphArea->getBottom() 
		- graphArea->getHeight() / displayRangeY->getLength() * ((scalingModeY == Logarithmic ? log10(y) : y) - displayRangeY->getStart());

	return Point<double>(mx, my);
}

void SimpleGraph::setDisplayRange(ScalingMode modeX, Range<double> xRange, ScalingMode modeY, Range<double> yRange)
{
	scalingModeX = modeX;
	scalingModeY = modeY;

	displayRangeX.reset(modeX == Linear ? new Range<double>(xRange) : new Range<double>(log10(xRange.getStart()), log10(xRange.getEnd())));
	displayRangeY.reset(modeY == Linear ? new Range<double>(yRange) : new Range<double>(log10(yRange.getStart()), log10(yRange.getEnd())));
}
