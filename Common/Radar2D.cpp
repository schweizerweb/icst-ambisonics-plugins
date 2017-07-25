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
Radar2D::Radar2D(RadarMode mode, Array<AmbiPoint>* pAmbiPointArray, ZoomSettings* pZoomSettings, int* pSelectedPointIndex):
	pAmbiPoints(pAmbiPointArray), 
	pZoomSettings(pZoomSettings), 
	radarMode(mode),
	pSelectedPointIndex(pSelectedPointIndex)
{
	openGLContext.setRenderer(this);
	openGLContext.attachTo(*this);
	openGLContext.setContinuousRepainting(true);

	pZoomSettings->addChangeListener(this);

	radarColors = new RadarColors(0);
}

Radar2D::~Radar2D()
{
	openGLContext.detach();

	radarColors = nullptr;
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
	g.setColour(radarColors->getRadarLineColor());
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

float Radar2D::getSelectedPointSize() const
{
	return getPointSize() * 1.5;
}

void Radar2D::renderOpenGL()
{
	jassert(OpenGLHelpers::isContextActive());

	const float desktopScale = (float)openGLContext.getRenderingScale();

	OpenGLHelpers::clear(radarColors->getRadarBackground());
	ScopedPointer<LowLevelGraphicsContext> glRenderer(createOpenGLGraphicsContext(openGLContext,
		roundToInt(desktopScale * getWidth()),
		roundToInt(desktopScale * getHeight())));

	if (glRenderer != nullptr)
	{
		Graphics g(*glRenderer);
		g.addTransform(AffineTransform::scale(desktopScale));

		g.setColour(radarColors->getRadarLineColor());
		g.drawRect(radarViewport, 1);   // draw an outline around the component

		if (radarUpdated)
			createRadarBackground();

		g.drawImageAt(radarBackground, radarViewport.getX(), radarViewport.getY());

		for (int i = 0; i < pAmbiPoints->size(); i++)
		{
			AmbiPoint point = pAmbiPoints->getReference(i);

			Point<float> screenPt = getAbsoluteScreenPoint(getProjectedPoint(point.getPoint()).toFloat());
			if(i == *pSelectedPointIndex)
			{
				g.setColour(radarColors->getPointSelectionColor());
				Rectangle<float> rect(getSelectedPointSize(), getSelectedPointSize());
				g.fillEllipse(rect.withCentre(screenPt));
			}
			g.setColour(trackColors.getColor(point.getColorIndex()));
			Rectangle<float> rect(getPointSize(), getPointSize());
			g.fillEllipse(rect.withCentre(screenPt));
			g.drawSingleLineText(point.getName(), screenPt.getX() + getPointSize(), screenPt.getY() - getPointSize());
		}

		g.setColour(radarColors->getInfoTextColor());
		g.drawText(infoString, radarViewport.getX(), radarViewport.getY(), radarViewport.getWidth(), 30, Justification::topLeft);
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
	if(source == pZoomSettings)
		radarUpdated = true;
}

Point<float> Radar2D::getRelativeScreenPoint(Point<float> valuePoint) const
{
	Rectangle<float> currentViewValueRect = pZoomSettings->getVisibleArea(radarMode == ZY);

	Point<float> convertedPoint(
		float(valuePoint.getX() - currentViewValueRect.getX()) / currentViewValueRect.getWidth() * radarViewport.getWidth(),
		radarViewport.getHeight() - float(valuePoint.getY() - currentViewValueRect.getY()) / currentViewValueRect.getHeight() * radarViewport.getHeight());
	
	return convertedPoint;
}

Point<float> Radar2D::getValuePointFromRelativeScreenPoint(Point<float> relativeScreenPoint) const
{
	Rectangle<float> currentViewValueRect = pZoomSettings->getVisibleArea(radarMode == ZY);

	Point<float> convertedPoint(
		float(relativeScreenPoint.getX() / radarViewport.getWidth() * currentViewValueRect.getWidth()) + currentViewValueRect.getX(),
		float((radarViewport.getHeight() - relativeScreenPoint.getY()) / radarViewport.getHeight() * currentViewValueRect.getHeight()) + currentViewValueRect.getY());

	return convertedPoint;
}

Point<float> Radar2D::getValuePointFromAbsoluteScreenPoint(Point<float> absoluteScreenPoint) const
{
	return getValuePointFromRelativeScreenPoint(absoluteScreenPoint - radarViewport.getTopLeft().toFloat());
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

double Radar2D::getMaxPointSelectionDist() const
{
	return pZoomSettings->getCurrentRadius()/20.0;
}

void Radar2D::mouseDown(const MouseEvent& e)
{
	Point<float> valuePoint = getValuePointFromAbsoluteScreenPoint(e.getPosition().toFloat());

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
		if (minDistIndex >= 0 && minDist < getMaxPointSelectionDist())
		{
			*pSelectedPointIndex = minDistIndex;
		}
		else
		{
			*pSelectedPointIndex = -1;
		}
	}
}

void Radar2D::mouseDrag(const MouseEvent& e)
{
	Point<float> valuePoint = getValuePointFromAbsoluteScreenPoint(e.getPosition().toFloat());
	showCoordinates(valuePoint);

	if(e.mods.isShiftDown())
	{
		
	}
	else
	{
		int pointSelection = *pSelectedPointIndex;
		if (pointSelection >= 0 && pointSelection < pAmbiPoints->size())
		{
			switch (radarMode)
			{
			case XY:
				pAmbiPoints->getReference(pointSelection).getPoint()->setXY(valuePoint.getY(), valuePoint.getX());
				break;
			case ZY:
				pAmbiPoints->getReference(pointSelection).getPoint()->setYZ(valuePoint.getX(), valuePoint.getY());
				break;
			}
		}
	}
}

void Radar2D::mouseUp(const MouseEvent& e)
{
	Point<float> valuePoint = getValuePointFromAbsoluteScreenPoint(e.getPosition().toFloat());

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
	Point<float> valuePoint = getValuePointFromAbsoluteScreenPoint(e.getPosition().toFloat());

	// add new point
	switch (radarMode) {
	case XY:
		pAmbiPoints->add(AmbiPoint(Point3D<double>(valuePoint.getY(), valuePoint.getX(), 0.0), String(pAmbiPoints->size()), pAmbiPoints->size()));
		break;
	case ZY:
		pAmbiPoints->add(AmbiPoint(Point3D<double>(0.0, valuePoint.getX(), valuePoint.getY()), String(pAmbiPoints->size()), pAmbiPoints->size()));
		break;
	}

	// select added point
	*pSelectedPointIndex = pAmbiPoints->size() - 1;
}

void Radar2D::showCoordinates(const Point<float>& point)
{
	String str;
	str << String(point.getX(), 2) << "; " << String(point.getY(), 2);
	infoString = str;
}

void Radar2D::mouseMove(const MouseEvent& e)
{
	showCoordinates(getValuePointFromAbsoluteScreenPoint(e.getPosition().toFloat()));
}
