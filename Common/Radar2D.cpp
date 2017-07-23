/*
  ==============================================================================

    Radar2D.cpp
    Created: 22 Jul 2017 1:43:26pm
    Author:  chris

  ==============================================================================
*/

#include "../Decoder/JuceLibraryCode/JuceHeader.h"
#include "Radar2D.h"

//==============================================================================
Radar2D::Radar2D(RadarMode mode, Array<AmbiPoint>* pAmbiPointArray, float radius):
	pAmbiPoints(pAmbiPointArray), 
	initialRadius(radius), 
	radarMode(mode),
	selectedPoint(nullptr),
	currentViewRadius(radius),
	currentViewCenter(Point<float>(0.0, 0.0))
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.
}

Radar2D::~Radar2D()
{
}

Point<double> Radar2D::getProjectedPoint(Point3D<double>* point3_d) const
{
	switch(radarMode)
	{
	case XY:
		return Point<double>(point3_d->getY(), point3_d->getX());
	case ZY:
		return Point<double>(point3_d->getY(), point3_d->getZ());
	}

	return Point<double>();
}

Point<float> Radar2D::getAbsoluteScreenPoint(Point<float> valuePoint) const
{
	return getRelativeScreenPoint(valuePoint) + radarViewport.getTopLeft().toFloat();
}

float Radar2D::getPointSize() const
{
	return 20;
}

void Radar2D::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

	
    g.setColour (Colours::white);
    g.setFont (14.0f);
    //g.drawText ("Radar2D", getLocalBounds(),
    //            Justification::centred, true);   // draw some placeholder text
	g.drawImageAt(radarBackground, radarViewport.getX(), radarViewport.getY());

	for (AmbiPoint point : *pAmbiPoints)
	{
		Point<float> screenPt = getAbsoluteScreenPoint(getProjectedPoint(point.getPoint()).toFloat());
		Rectangle<float> rect(
			screenPt.getX() - getPointSize() / 2,
			screenPt.getY() - getPointSize() / 2,
			getPointSize(),
			getPointSize());
		g.fillEllipse(rect);
	}

	g.drawText(infoString, 0, 0, 100, 30, Justification::topLeft);
}

void Radar2D::createRadarBackground()
{
	Rectangle<int> localBounds = radarViewport;

	Image img(Image::ARGB, localBounds.getWidth(), localBounds.getHeight(), true);
	Graphics g(img);
	g.setColour(Colours::lightgrey);

	int dist = currentViewRadius / 10 * getValueToScreenRatio();
	Point<float> centerPoint = getRelativeScreenPoint(Point<float>(0.0, 0.0));
	for (int i = 0; i < 10; i++)
	{
		int absDist = i * dist;
		Rectangle<float> ellipseRect(centerPoint.getX() - absDist, centerPoint.getY() - absDist, 2 * absDist, 2 * absDist);
		g.drawEllipse(ellipseRect, 1);
	}
	
	radarBackground = img;
}

float Radar2D::getValueToScreenRatio() const
{
	return radarViewport.getWidth() / (2 * currentViewRadius);
}

Point<float> Radar2D::getRelativeScreenPoint(Point<float> valuePoint) const
{
	Rectangle<float> currentViewValueRect(currentViewCenter.getX() - currentViewRadius, currentViewCenter.getY() - currentViewRadius, 2 * currentViewRadius, radarMode == XY ? 2 * currentViewRadius : currentViewRadius);

	Point<float> convertedPoint(
		float(valuePoint.getX() - currentViewValueRect.getX()) / currentViewValueRect.getWidth() * radarViewport.getWidth(),
		float(valuePoint.getY() - currentViewValueRect.getY()) / currentViewValueRect.getHeight() * radarViewport.getHeight());
	
	return convertedPoint;
}

Point<float> Radar2D::getValuePointFromRelativeScreenPoint(Point<float> relativeScreenPoint) const
{
	Rectangle<float> currentViewValueRect(currentViewCenter.getX() - currentViewRadius, currentViewCenter.getY() - currentViewRadius, 2 * currentViewRadius, radarMode == XY ? 2 * currentViewRadius : currentViewRadius);

	Point<float> convertedPoint(
		float(relativeScreenPoint.getX() / radarViewport.getWidth() * currentViewValueRect.getWidth()) + currentViewValueRect.getX(),
		float(relativeScreenPoint.getY() / radarViewport.getHeight() * currentViewValueRect.getHeight()) + currentViewValueRect.getY());

	return convertedPoint;
}

void Radar2D::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

	double wantedRatioWidthToHeight = (radarMode == XY) ? 1 : 2;

	if(getBounds().getAspectRatio() >= wantedRatioWidthToHeight)
	{
		// add additional space left and right
		radarViewport = Rectangle<int>((getBounds().getWidth() - getBounds().getHeight() * wantedRatioWidthToHeight) / 2, 0, getBounds().getHeight() * wantedRatioWidthToHeight, getBounds().getHeight());
	}
	else
	{
		// add space on top or bottom, depending on radar mode
		radarViewport = Rectangle<int>(
			0,
			(radarMode == XY) ? int(getBounds().getHeight() - getBounds().getWidth() * wantedRatioWidthToHeight) : 0,
			getBounds().getWidth(),
			getBounds().getWidth() / wantedRatioWidthToHeight);
	}

	createRadarBackground();
	repaint();
}

void Radar2D::mouseExit(const MouseEvent& e)
{
	infoString = "exit";
	repaint();
}

double Radar2D::getMinPointSelectionDist() const
{
	return currentViewRadius / 20.0;
}

void Radar2D::mouseDown(const MouseEvent& e)
{
	Point<float> valuePoint = getValuePointFromRelativeScreenPoint(e.getPosition().toFloat() - radarViewport.getTopLeft().toFloat());

	if (e.mods.isShiftDown())
	{
		
	}
	else
	{
		double minDist = DBL_MAX;
		int minDistIndex = -1;
		for (int i = 0; i < pAmbiPoints->size(); i++)
		{
			AmbiPoint pt = pAmbiPoints->getReference(i);
			double dist;
			if ((dist = valuePoint.getDistanceFrom(getProjectedPoint(pt.getPoint()).toFloat())) < minDist)
			{
				minDist = dist;
				minDistIndex = i;
			}
		}
		if (minDistIndex >= 0 && minDist < getMinPointSelectionDist())
		{
			selectedPoint = &pAmbiPoints->getReference(minDistIndex);
		}
	}

	repaint();
}

void Radar2D::mouseDrag(const MouseEvent& e)
{
	Point<float> valuePoint = getValuePointFromRelativeScreenPoint(e.getPosition().toFloat() - radarViewport.getTopLeft().toFloat());

	if(selectedPoint != nullptr)
	{
		switch(radarMode)
		{
		case XY:
			selectedPoint->getPoint()->setXY(valuePoint.getY(), valuePoint.getX());
			break;
		case ZY:
			selectedPoint->getPoint()->setYZ(valuePoint.getX(), valuePoint.getY());
			break;
		}
		
	}
	repaint();
}

void Radar2D::mouseUp(const MouseEvent& e)
{
	selectedPoint = nullptr;

	Point<float> valuePoint = getValuePointFromRelativeScreenPoint(e.getPosition().toFloat() - radarViewport.getTopLeft().toFloat());

	if(e.mods.isShiftDown())
	{
		currentViewCenter = valuePoint;

		if(e.mods.isLeftButtonDown())
		{
			currentViewRadius *= 0.8;
		}
		else if(e.mods.isRightButtonDown())
		{
			currentViewRadius /= 0.8;
		}
	}

	createRadarBackground();
	repaint();
}

void Radar2D::mouseDoubleClick(const MouseEvent& e)
{
	Point<float> valuePoint = getValuePointFromRelativeScreenPoint(e.getPosition().toFloat() - radarViewport.getTopLeft().toFloat());

	// add new point
	switch (radarMode) {
	case XY:
		pAmbiPoints->add(AmbiPoint(Point3D<double>(valuePoint.getY(), valuePoint.getX(), 0.0)));
		break;
	case ZY:
		pAmbiPoints->add(AmbiPoint(Point3D<double>(0.0, valuePoint.getX(), valuePoint.getY())));
		break;
	}

	repaint();
}
