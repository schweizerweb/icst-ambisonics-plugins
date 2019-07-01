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
#include "LabelCreator.h"

//==============================================================================
Radar2D::Radar2D(RadarMode mode, AmbiDataSet* pEditablePoints, AmbiDataSet* pDisplayOnlyPoints, ZoomSettings* pZoomSettings, PointSelection* pPointSelection, RadarOptions* pRadarOptions):
	pEditablePoints(pEditablePoints),
	pDisplayOnlyPoints(pDisplayOnlyPoints),
	pZoomSettings(pZoomSettings), 
	radarMode(mode),
	pPointSelection(pPointSelection),
	pRadarOptions(pRadarOptions)
{
	radarBackground = new Image();
	infoImage = new Image();
	openGLContext.setRenderer(this);
	openGLContext.attachTo(*this);
	openGLContext.setContinuousRepainting(false);

	pZoomSettings->addChangeListener(this);

	radarColors = new RadarColors();

	startTimerHz(INACTIVE_REFRESH_RATE);
}

Radar2D::~Radar2D()
{
	stopTimer();

	openGLContext.detach();

	infoImage = nullptr;
	radarBackground = nullptr;

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

void Radar2D::drawRadar(Graphics* g) const
{
	const ScopedLock lock(radarBackgroundLock);
	g->drawImageAt(*radarBackground, radarViewport.getX(), radarViewport.getY());
}

void Radar2D::drawInfoLabel(Graphics* g) const
{
	const ScopedLock lock(infoLabelLock);
	g->drawImageAt(*infoImage, radarViewport.getX() + 3, radarViewport.getY());
}

void Radar2D::paint (Graphics&)
{
}

Image* Radar2D::createRadarBackground() const
{
	const MessageManagerLock lock;

	Rectangle<float> localBounds = radarViewport.toFloat();

	Image* img = new Image(Image::ARGB, int(localBounds.getWidth()), int(localBounds.getHeight()), true);
	Graphics g(*img);
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
	
	return img;
}

void Radar2D::updateRadarBackground()
{
	ScopedPointer<Image> img = createRadarBackground();

	const ScopedLock lock(radarBackgroundLock);
	radarBackground.swapWith(img);
}

void Radar2D::updateInfoLabel(String info)
{
	ScopedPointer<Image> img = LabelCreator::createNewLabel(info, radarColors->getInfoTextColor(), INFO_FONT_SIZE);

	const ScopedLock lock(infoLabelLock);
	infoImage.swapWith(img);
}

void Radar2D::timerCallback()
{
	openGLContext.triggerRepaint();
}

float Radar2D::getValueToScreenRatio() const
{
	return radarViewport.getWidth() / (2 * pZoomSettings->getCurrentRadius());
}

float Radar2D::getSelectedPointSize(float scaler) const
{
	return getEditablePointSize(scaler) * 1.5f;
}

void Radar2D::drawSquare(Graphics* g, Point<float>* screenPt, Point3D<double>* pt, float pointSize) const
{
	double angle = (radarMode == XY ? pt->getAzimuth() : atan2(pt->getY(), pt->getZ())) + PI * 0.25;

	Path p;
	p.addPolygon(*screenPt, 4, pointSize, float(angle));
	p.closeSubPath();
	g->fillPath(p);
}

void Radar2D::paintPointLabel(Graphics* g, Image labelImage, Point<float> screenPt, float offset) const
{
	int y = screenPt.getY() > offset + labelImage.getHeight()
		? int(screenPt.getY() - offset - labelImage.getHeight())
		: int(screenPt.getY() + offset);
	if(screenPt.getX() > getWidth() / 2
		&& screenPt.getX() > getWidth() - (offset + labelImage.getWidth()))
	{
		g->drawImageAt(labelImage, int(screenPt.getX() - offset - labelImage.getWidth()), y);
	}
	else
	{
		g->drawImageAt(labelImage, int(screenPt.getX() + offset), y);
	}
}

void Radar2D::paintPoint(Graphics* g, AmbiPoint* point, float pointSize, bool square, bool select, float selectionSize) const
{
	Point3D<double>* pt = point->getPoint();
	Point<float> screenPt = getAbsoluteScreenPoint(getProjectedPoint(pt).toFloat());
	if (select)
	{
		g->setColour(radarColors->getPointSelectionColor());
		if (square)
			drawSquare(g, &screenPt, pt, selectionSize);
		else
		{
			Rectangle<float> rect(selectionSize, selectionSize);
			g->fillEllipse(rect.withCentre(screenPt));
		}
	}

	g->setColour(point->getColor());
	
	if(square)
		drawSquare(g, &screenPt, pt, pointSize);
	else
	{
		Rectangle<float> rect(pointSize, pointSize);
		g->fillEllipse(rect.withCentre(screenPt));
	}
	
	Image* img = point->getLabelImage();
	paintPointLabel(g, *img, screenPt, pointSize * (square ? 0.7f : 0.5f));
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

		drawRadar(&g);
		
		if (pEditablePoints != nullptr && pRadarOptions->showEditablePoints)
		{
			for (int i = 0; i < pEditablePoints->size(); i++)
			{
				AmbiPoint* pt = pEditablePoints->get(i);
				if (pt != nullptr)
				{
					float scaler = 1.0f + 10.0f * pt->getRms();
					paintPoint(&g, pt, getEditablePointSize(scaler), pRadarOptions->editablePointsAsSquare, i == pPointSelection->getSelectedPointIndex(), getSelectedPointSize(scaler));
				}
			}
		}

		int64 referenceTime = Time::currentTimeMillis();
		if (pDisplayOnlyPoints != nullptr && pRadarOptions->showDisplayOnlyPoints)
		{
			for (int i = pDisplayOnlyPoints->size() - 1; i >= 0 ; i--)
			{
				AmbiPoint* pt = pDisplayOnlyPoints->get(i, referenceTime, pRadarOptions->displayTimeout);
				if (pt != nullptr)
				{
					float scaler = 1.0f + 10.0f * pt->getRms();
					paintPoint(&g, pt, getDisplayOnlyPointSize(scaler), false);
				}
			}
		}

		drawInfoLabel(&g);
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
	if (source == pZoomSettings)
		updateRadarBackground();
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

	if (getBounds().getWidth() * getBounds().getHeight() <= 0)
		return;

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

	updateRadarBackground();
}

void Radar2D::mouseExit(const MouseEvent&)
{
	startTimerHz(INACTIVE_REFRESH_RATE);
	updateInfoLabel("");
}

void Radar2D::mouseEnter(const MouseEvent&)
{
	startTimerHz(ACTIVE_REFRESH_RATE);
}

double Radar2D::getMaxPointSelectionDist() const
{
	return pZoomSettings->getCurrentRadius()/15.0;
}

void Radar2D::mouseDown(const MouseEvent& e)
{
	Point<float> valuePoint = getValuePointFromAbsoluteScreenPoint(e.getPosition().toFloat());

	if (!pRadarOptions->showEditablePoints)
		return;

	if (e.mods.isShiftDown())
	{
		
	}
	else
	{
		double minDist = DBL_MAX;
		int minDistIndex = -1;
		for (int i = 0; i < pEditablePoints->size(); i++)
		{
			AmbiPoint* pt = pEditablePoints->get(i);
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
		if (!pRadarOptions->showEditablePoints)
			return;

		int pointSelection = pPointSelection->getSelectedPointIndex();
		switch (radarMode)
		{
		case XY:
			pEditablePoints->setChannelXY(pointSelection, valuePoint.getY(), valuePoint.getX());
			break;
		case ZY:
			pEditablePoints->setChannelYZ(pointSelection, valuePoint.getX(), valuePoint.getY());
			break;
		}
		pPointSelection->notifyChange();
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
}

void Radar2D::mouseDoubleClick(const MouseEvent& e)
{
	Point<float> valuePoint = getValuePointFromAbsoluteScreenPoint(e.getPosition().toFloat());

	if (!pRadarOptions->showEditablePoints)
		return;

	if (pRadarOptions->maxNumberEditablePoints > 0 && pEditablePoints->size() >= pRadarOptions->maxNumberEditablePoints)
		return;

	// add new point
	ScopedPointer<Uuid> newId = new Uuid();
	int index = pEditablePoints->size();
	int indexForName = index + 1;
	switch (radarMode) {
	case XY:
		pEditablePoints->add(new AmbiPoint(newId->toString(), Point3D<double>(valuePoint.getY(), valuePoint.getX(), 0.0, pRadarOptions->getAudioParamForIndex(index)), String(indexForName), TrackColors::getColor(indexForName)));
		break;
	case ZY:
		pEditablePoints->add(new AmbiPoint(newId->toString(), Point3D<double>(0.0, valuePoint.getX(), valuePoint.getY(), pRadarOptions->getAudioParamForIndex(index)), String(indexForName), TrackColors::getColor(indexForName)));
		break;
	}

	// select added point
	pPointSelection->selectPoint(pEditablePoints->size() - 1);
}

void Radar2D::showCoordinates(const Point<float>& point)
{
	String str;
	str << String(point.getX(), 2) << "; " << String(point.getY(), 2);
	updateInfoLabel(str);
}

void Radar2D::mouseMove(const MouseEvent& e)
{
	showCoordinates(getValuePointFromAbsoluteScreenPoint(e.getPosition().toFloat()));
}
