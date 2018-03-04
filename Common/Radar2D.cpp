/*
  ==============================================================================

    Radar2D.cpp
    Created: 22 Jul 2017 1:43:26pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "JuceHeader.h"
#include "Radar2D.h"
#include "TrackColors.h"

//==============================================================================
Radar2D::Radar2D(RadarMode mode, OwnedArray<AmbiPoint>* pEditablePointsArray, OwnedArray<AmbiPoint>* pDisplayOnlyPointsArray, ZoomSettings* pZoomSettings, PointSelection* pPointSelection, RadarOptions* pRadarOptions):
	pEditablePointsArray(pEditablePointsArray),
	pDisplayOnlyPointsArray(pDisplayOnlyPointsArray),
	pZoomSettings(pZoomSettings), 
	radarMode(mode),
	pPointSelection(pPointSelection),
	pRadarOptions(pRadarOptions)
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

float Radar2D::getEditablePointSize(float scaler) const
{
	return radarViewport.getWidth() / 30.0f * scaler;
}

float Radar2D::getDisplayOnlyPointSize(float scaler) const
{
	return radarViewport.getWidth() / 50.0f * scaler;
}

float Radar2D::getFontSize() const
{
	return radarViewport.getWidth() / 25.0f;
}

void Radar2D::paint (Graphics&)
{
}

void Radar2D::createRadarBackground()
{
	radarUpdated = false;

	Rectangle<float> localBounds = radarViewport.toFloat();

	Image img(Image::ARGB, int(localBounds.getWidth()), int(localBounds.getHeight()), true);
	Graphics g(img);
	g.setColour(radarColors->getRadarLineColor());
	g.setFont(getFontSize());
	int numberOfRings = 10;

	float dist = pZoomSettings->getInitialRadius() / float(numberOfRings) * getValueToScreenRatio();
	Point<float> centerPoint = getRelativeScreenPoint(Point<float>(0.0, 0.0));
	for (int i = 0; i < numberOfRings; i++)
	{
		float absDist = (i + 1) * dist;
		Rectangle<float> ellipseRect(centerPoint.getX() - absDist, centerPoint.getY() - absDist, 2 * absDist, 2 * absDist);
		g.drawEllipse(ellipseRect, 1.0f);
		g.drawSingleLineText(String((i + 1) * pZoomSettings->getInitialRadius() / numberOfRings), int(centerPoint.getX()), int(centerPoint.getY() - absDist));
	}
	
	g.drawLine(centerPoint.getX(), 0.0f, centerPoint.getX(), localBounds.getHeight(), 2.0f);
	g.drawLine(0.0f, centerPoint.getY(), localBounds.getWidth(), centerPoint.getY(), 2.0f);
	
	radarBackground = img;
}

float Radar2D::getValueToScreenRatio() const
{
	return radarViewport.getWidth() / (2 * pZoomSettings->getCurrentRadius());
}

float Radar2D::getSelectedPointSize(float scaler) const
{
	return getEditablePointSize(scaler) * 1.5f;
}

void Radar2D::paintPointLabel(Graphics* g, String text, Point<float> screenPt, float offset) const
{
	int y = int(screenPt.getY() + (screenPt.getY() < (offset + getFontSize()) ? 2.0 : -1.0) * offset);
	if(screenPt.getX() > getWidth() / 2
		&& screenPt.getX() > getWidth() - (offset + text.length() * getFontSize() / 2.0f))
	{
		g->drawSingleLineText(text, int(screenPt.getX() - offset), y, Justification::right);
	}
	else
	{
		g->drawSingleLineText(text, int(screenPt.getX() + offset), y);
	}
}

void Radar2D::paintPoint(Graphics* g, AmbiPoint* point, float pointSize, bool select, float selectionSize)
{
	Point<float> screenPt = getAbsoluteScreenPoint(getProjectedPoint(point->getPoint()).toFloat());
	if (select)
	{
		g->setColour(radarColors->getPointSelectionColor());
		Rectangle<float> rect(selectionSize, selectionSize);
		g->fillEllipse(rect.withCentre(screenPt));
	}

	g->setColour(trackColors.getColor(point->getColorIndex()));
	Rectangle<float> rect(pointSize, pointSize);
	g->fillEllipse(rect.withCentre(screenPt));
			
	paintPointLabel(g, point->getName(), screenPt, pointSize * 0.5f);
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

		g.setFont(getFontSize());
		g.setColour(radarColors->getRadarLineColor());
		g.drawRect(radarViewport, 1);   // draw an outline around the component

		if (radarUpdated)
			createRadarBackground();

		g.drawImageAt(radarBackground, radarViewport.getX(), radarViewport.getY());

		if (pEditablePointsArray != nullptr)
		{
			for (int i = 0; i < pEditablePointsArray->size(); i++)
			{
				float scaler = 1.0f + 10.0f * pEditablePointsArray->getUnchecked(i)->getRms();
				paintPoint(&g, pEditablePointsArray->getUnchecked(i), getEditablePointSize(scaler), i == pPointSelection->getSelectedPointIndex(), getSelectedPointSize(scaler));
			}
		}

		if (pDisplayOnlyPointsArray != nullptr)
		{
			for (int i = 0; i < pDisplayOnlyPointsArray->size(); i++)
			{
				float scaler = 1.0f + 10.0f * pDisplayOnlyPointsArray->getUnchecked(i)->getRms();
				paintPoint(&g, pDisplayOnlyPointsArray->getUnchecked(i), getDisplayOnlyPointSize(scaler));
			}
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
		radarViewport = Rectangle<int>(
			int((getBounds().getWidth() - getBounds().getHeight() * wantedRatioWidthToHeight) / 2), 
			0, 
			int(getBounds().getHeight() * wantedRatioWidthToHeight), 
			int(getBounds().getHeight()));
	}
	else
	{
		// add space on top or bottom, depending on radar mode
		radarViewport = Rectangle<int>(
			0,
			(radarMode == XY) ? int(getBounds().getHeight() - getBounds().getWidth() * wantedRatioWidthToHeight) : 0,
			getBounds().getWidth(),
			int(getBounds().getWidth() / wantedRatioWidthToHeight));
	}

	radarUpdated = true;
}

void Radar2D::mouseExit(const MouseEvent&)
{
	infoString = "";
}

double Radar2D::getMaxPointSelectionDist() const
{
	return pZoomSettings->getCurrentRadius()/15.0;
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
		for (int i = 0; i < pEditablePointsArray->size(); i++)
		{
			AmbiPoint* pt = pEditablePointsArray->getUnchecked(i);
			double dist;
			if ((dist = valuePoint.getDistanceFrom(getProjectedPoint(pt->getPoint()).toFloat())) < minDist)
			{
				minDist = dist;
				minDistIndex = i;
			}
		}
		if (minDistIndex >= 0 && minDist < getMaxPointSelectionDist())
		{
			pPointSelection->selectPoint(minDistIndex);
		}
		else
		{
			pPointSelection->unselectPoint();
		}
	}
}

void Radar2D::mouseDrag(const MouseEvent& e)
{
	Point<float> valuePoint = getValuePointFromAbsoluteScreenPoint(e.getPosition().toFloat());
	showCoordinates(valuePoint);

	if(e.mods.isShiftDown())
	{
		Point<float> originPoint = getValuePointFromAbsoluteScreenPoint((e.getPosition() - e.getOffsetFromDragStart()).toFloat());
		setCenterPoint(originPoint - valuePoint);
	}
	else
	{
		int pointSelection = pPointSelection->getSelectedPointIndex();
		if (pointSelection >= 0 && pointSelection < pEditablePointsArray->size())
		{
			switch (radarMode)
			{
			case XY:
				pEditablePointsArray->getUnchecked(pointSelection)->getPoint()->setXY(valuePoint.getY(), valuePoint.getX());
				break;
			case ZY:
				pEditablePointsArray->getUnchecked(pointSelection)->getPoint()->setYZ(valuePoint.getX(), valuePoint.getY());
				break;
			}
			pPointSelection->notifyChange();
		}
	}
}

void Radar2D::setCenterPoint(Point<float> valuePoint) const
{
	if (radarMode == XY)
		pZoomSettings->setCurrentCenterPointXY(valuePoint.getY(), valuePoint.getX());
	else if (radarMode == ZY)
		pZoomSettings->setCurrentCenterPointYZ(valuePoint.getX(), valuePoint.getY());
}

void Radar2D::mouseUp(const MouseEvent& e)
{
	Point<float> valuePoint = getValuePointFromAbsoluteScreenPoint(e.getPosition().toFloat());

	if(e.mods.isShiftDown() && !e.mouseWasDraggedSinceMouseDown())
	{
		setCenterPoint(valuePoint);

		if(e.mods.isLeftButtonDown())
		{
			pZoomSettings->setCurrentRadius(pZoomSettings->getCurrentRadius() * 0.8f);
		}
		else if(e.mods.isRightButtonDown())
		{
			pZoomSettings->setCurrentRadius(pZoomSettings->getCurrentRadius() / 0.8f);
		}
	}

	radarUpdated = true;
}

void Radar2D::mouseDoubleClick(const MouseEvent& e)
{
	Point<float> valuePoint = getValuePointFromAbsoluteScreenPoint(e.getPosition().toFloat());

	if (pRadarOptions->maxNumberEditablePoints > 0 && pEditablePointsArray->size() >= pRadarOptions->maxNumberEditablePoints)
		return;

	// add new point
	ScopedPointer<Uuid> newId = new Uuid();
	switch (radarMode) {
	case XY:
		pEditablePointsArray->add(new AmbiPoint(newId->toString(), Point3D<double>(valuePoint.getY(), valuePoint.getX(), 0.0, pRadarOptions->getAudioParamForIndex(pEditablePointsArray->size())), String(pEditablePointsArray->size()+1), 0));
		break;
	case ZY:
		pEditablePointsArray->add(new AmbiPoint(newId->toString(), Point3D<double>(0.0, valuePoint.getX(), valuePoint.getY(), pRadarOptions->getAudioParamForIndex(pEditablePointsArray->size())), String(pEditablePointsArray->size()+1), 0));
		break;
	}

	// select added point
	pPointSelection->selectPoint(pEditablePointsArray->size() - 1);
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
