/*
  ==============================================================================

    Radar2D.cpp
    Created: 22 Jul 2017 1:43:26pm
    Author:  chris

  ==============================================================================
*/

#include "../Decoder/JuceLibraryCode/JuceHeader.h"
#include "Radar2D.h"
#include "TrackColors.h"

//==============================================================================
Radar2D::Radar2D(RadarMode mode, Array<AmbiPoint>* pAmbiPointArray, ZoomSettings* pZoomSettings):
	pAmbiPoints(pAmbiPointArray), 
	pZoomSettings(pZoomSettings), 
	radarMode(mode),
	selectedPoint(nullptr)
{
	openGLContext.setRenderer(this);
	openGLContext.attachTo(*this);
	openGLContext.setContinuousRepainting(true);

	pZoomSettings->addChangeListener(this);
}

Radar2D::~Radar2D()
{
	openGLContext.detach();
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
}

void Radar2D::createRadarBackground()
{
	radarUpdated = false;

	Rectangle<int> localBounds = radarViewport;

	Image img(Image::ARGB, localBounds.getWidth(), localBounds.getHeight(), true);
	Graphics g(img);
	g.setColour(Colours::lightgrey);
	g.setFont(16);
	int numberOfRings = 10;

	float dist = pZoomSettings->getInitialRadius() / float(numberOfRings) * getValueToScreenRatio();
	Point<float> centerPoint = getRelativeScreenPoint(Point<float>(0.0, 0.0));
	for (int i = 0; i < numberOfRings; i++)
	{
		float absDist = (i + 1) * dist;
		Rectangle<float> ellipseRect(centerPoint.getX() - absDist, centerPoint.getY() - absDist, 2 * absDist, 2 * absDist);
		g.drawEllipse(ellipseRect, 1);
		g.drawSingleLineText(String((i + 1) * pZoomSettings->getInitialRadius() / numberOfRings), centerPoint.getX(), centerPoint.getY() - absDist);
	}
	
	g.drawLine(centerPoint.getX(), 0, centerPoint.getX(), localBounds.getHeight(), 2);
	g.drawLine(0, centerPoint.getY(), localBounds.getWidth(), centerPoint.getY(), 2);
	
	radarBackground = img;
}

float Radar2D::getValueToScreenRatio() const
{
	return radarViewport.getWidth() / (2 * pZoomSettings->getCurrentRadius());
}

void Radar2D::renderOpenGL()
{
	jassert(OpenGLHelpers::isContextActive());

	const float desktopScale = (float)openGLContext.getRenderingScale();

	OpenGLHelpers::clear(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

	ScopedPointer<LowLevelGraphicsContext> glRenderer(createOpenGLGraphicsContext(openGLContext,
		roundToInt(desktopScale * getWidth()),
		roundToInt(desktopScale * getHeight())));

	if (glRenderer != nullptr)
	{
		Graphics g(*glRenderer);
		g.addTransform(AffineTransform::scale(desktopScale));


		g.setColour(Colours::grey);
		g.drawRect(radarViewport, 1);   // draw an outline around the component

		g.setColour(Colours::white);
		g.setFont(14.0f);

		if (radarUpdated)
			createRadarBackground();

		g.drawImageAt(radarBackground, radarViewport.getX(), radarViewport.getY());

		for (AmbiPoint point : *pAmbiPoints)
		{
			g.setColour(trackColors.getColor(point.getColorIndex()));
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
}

void Radar2D::newOpenGLContextCreated()
{
}

void Radar2D::openGLContextClosing()
{
}

void Radar2D::changeListenerCallback(ChangeBroadcaster* source)
{
	radarUpdated = true;
}

Point<float> Radar2D::getRelativeScreenPoint(Point<float> valuePoint) const
{
	Rectangle<float> currentViewValueRect = pZoomSettings->getVisibleArea(radarMode == ZY);

	Point<float> convertedPoint(
		float(valuePoint.getX() - currentViewValueRect.getX()) / currentViewValueRect.getWidth() * radarViewport.getWidth(),
		float(valuePoint.getY() - currentViewValueRect.getY()) / currentViewValueRect.getHeight() * radarViewport.getHeight());
	
	return convertedPoint;
}

Point<float> Radar2D::getValuePointFromRelativeScreenPoint(Point<float> relativeScreenPoint) const
{
	Rectangle<float> currentViewValueRect = pZoomSettings->getVisibleArea(radarMode == ZY);

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
		//radarViewport = Rectangle<int>(20, 20, getBounds().getWidth(), getBounds().getHeight());
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

	radarUpdated = true;
}

void Radar2D::mouseExit(const MouseEvent& e)
{
	infoString = "";
}

double Radar2D::getMinPointSelectionDist() const
{
	return 20.0;
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
}

void Radar2D::mouseDrag(const MouseEvent& e)
{
	Point<float> valuePoint = getValuePointFromRelativeScreenPoint(e.getPosition().toFloat() - radarViewport.getTopLeft().toFloat());

	if(e.mods.isShiftDown())
	{
		
	}
	else
	{
		if (selectedPoint != nullptr)
		{
			switch (radarMode)
			{
			case XY:
				selectedPoint->getPoint()->setXY(valuePoint.getY(), valuePoint.getX());
				break;
			case ZY:
				selectedPoint->getPoint()->setYZ(valuePoint.getX(), valuePoint.getY());
				break;
			}
		}
	}
}

void Radar2D::mouseUp(const MouseEvent& e)
{
	selectedPoint = nullptr;

	Point<float> valuePoint = getValuePointFromRelativeScreenPoint(e.getPosition().toFloat() - radarViewport.getTopLeft().toFloat());

	if(e.mods.isShiftDown() && !e.mouseWasDraggedSinceMouseDown())
	{
		if (radarMode == XY)
			pZoomSettings->setCurrentCenterPointXY(valuePoint.getY(), valuePoint.getX());
		else if (radarMode == ZY)
			pZoomSettings->setCurrentCenterPointYZ(valuePoint.getX(), valuePoint.getY());

		if(e.mods.isLeftButtonDown())
		{
			pZoomSettings->setCurrentRadius(pZoomSettings->getCurrentRadius() * 0.8);
		}
		else if(e.mods.isRightButtonDown())
		{
			pZoomSettings->setCurrentRadius(pZoomSettings->getCurrentRadius() / 0.8);
		}
	}

	radarUpdated = true;
}

void Radar2D::mouseDoubleClick(const MouseEvent& e)
{
	Point<float> valuePoint = getValuePointFromRelativeScreenPoint(e.getPosition().toFloat() - radarViewport.getTopLeft().toFloat());

	// add new point
	switch (radarMode) {
	case XY:
		pAmbiPoints->add(AmbiPoint(Point3D<double>(valuePoint.getY(), valuePoint.getX(), 0.0), pAmbiPoints->size()));
		break;
	case ZY:
		pAmbiPoints->add(AmbiPoint(Point3D<double>(0.0, valuePoint.getX(), valuePoint.getY()), pAmbiPoints->size()));
		break;
	}
}
