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
	radarBackground = Image();
	infoImage = Image();
	openGLContext.setRenderer(this);
	openGLContext.attachTo(*this);
	openGLContext.setContinuousRepainting(false);

	setWantsKeyboardFocus(true);

	pZoomSettings->addChangeListener(this);

	startTimerHz(INACTIVE_REFRESH_RATE);
}

Radar2D::~Radar2D()
{
	stopTimer();

	openGLContext.detach();
}

Point<double> Radar2D::getProjectedPoint(Point3D<double>* point3_d) const
{
	switch(radarMode)
	{
	case XY:
		return Point<double>(point3_d->getX(), point3_d->getY());
	case XZ_Half:
	case XZ_Full:
		return Point<double>(point3_d->getX(), point3_d->getZ());
	}

	return Point<double>();
}

Point<float> Radar2D::getAbsoluteScreenPoint(Point<float> valuePoint) const
{
	return getRelativeScreenPoint(valuePoint) + radarViewport.getTopLeft().toFloat();
}

float Radar2D::getEditablePointSize(float scaler) const
{
	return radarViewport.getWidth() / 30.0f * scaler * float(pZoomSettings->getPointScaler());
}

float Radar2D::getDisplayOnlyPointSize(float scaler) const
{
    return radarViewport.getWidth() / 50.0f * scaler * float(pZoomSettings->getPointScaler());
}

float Radar2D::getFontSize() const
{
	return radarViewport.getWidth() / 25.0f;
}

void Radar2D::drawRadar(Graphics* g) const
{
	const ScopedLock lock(radarBackgroundLock);
	g->drawImageAt(radarBackground, radarViewport.getX(), radarViewport.getY());
}

void Radar2D::drawInfoLabel(Graphics* g) const
{
	const ScopedLock lock(infoLabelLock);
	g->drawImageAt(infoImage, radarViewport.getX() + 3, radarViewport.getY());
}

void Radar2D::paint (Graphics&)
{
}

Image Radar2D::createRadarBackground() const
{
	const MessageManagerLock lock;

	Rectangle<float> localBounds = radarViewport.toFloat();

	Image img = Image(Image::ARGB, int(localBounds.getWidth()), int(localBounds.getHeight()), true);
	Graphics g(img);
	g.setColour(radarColors.getRadarLineColor());
	g.setFont(getFontSize());
	int numberOfRings = pZoomSettings->getNumberOfRings();

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
	
    g.setColour(radarColors.getRadarAxisColor());
    g.drawSingleLineText("X", localBounds.getWidth()-12, centerPoint.getY() - 2);
    g.drawSingleLineText(radarMode == XY ? "Y" : "Z", centerPoint.getX() - 10, 15);
    
	return img;
}

void Radar2D::updateRadarBackground()
{
	Image img = createRadarBackground();

	const ScopedLock lock(radarBackgroundLock);
	radarBackground = img;
}

void Radar2D::updateInfoLabel(String info)
{
	Image img = LabelCreator::createNewLabel(info, radarColors.getInfoTextColor(), INFO_FONT_SIZE);

	const ScopedLock lock(infoLabelLock);
	infoImage = img;
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
	double angle = (radarMode == XY ? pt->getAzimuth() : atan2(pt->getX(), pt->getZ())) + PI * 0.25;

	Path p;
	p.addPolygon(*screenPt, 4, pointSize, float(angle));
	p.closeSubPath();
	g->fillPath(p);
}

void Radar2D::drawStar(Graphics* g, Point<float>* screenPt, float pointSize) const
{
	Path p;
	p.addStar(*screenPt, 8, pointSize / 2, pointSize);
	p.closeSubPath();
	g->fillPath(p);
}

void Radar2D::paintPointLabel(Graphics* g, Image labelImage, Point<float> screenPt, float offset) const
{
    double baseScaler = pZoomSettings->getPointScaler();
    int scaledImageWidth = int(labelImage.getWidth() * baseScaler);
    int scaledImageHeight = int(labelImage.getHeight() * baseScaler);
    int y = screenPt.getY() > offset + scaledImageHeight
		? int(screenPt.getY() - offset - scaledImageHeight)
		: int(screenPt.getY() + offset);
	if(screenPt.getX() > getWidth() / 2
		&& screenPt.getX() > getWidth() - (offset + scaledImageWidth))
	{
		g->drawImageWithin(labelImage, int(screenPt.getX() - offset - scaledImageWidth), y, scaledImageWidth, scaledImageHeight, RectanglePlacement::stretchToFit);
	}
	else
	{
		g->drawImageWithin(labelImage, int(screenPt.getX() + offset), y, scaledImageWidth, scaledImageHeight, RectanglePlacement::stretchToFit);
	}
}

void Radar2D::paintPoint(Graphics* g, AmbiPoint* point, float pointSize, Shape shape, bool select, float selectionSize) const
{
	Point3D<double>* pt = point->getPoint();
	Point<float> screenPt = getAbsoluteScreenPoint(getProjectedPoint(pt).toFloat());
	if (select)
	{
		g->setColour(radarColors.getPointSelectionColor());
		if (shape == Square)
			drawSquare(g, &screenPt, pt, selectionSize);
		else if (shape == Star)
			drawStar(g, &screenPt, selectionSize);
		else
		{
			Rectangle<float> rect(selectionSize, selectionSize);
			g->fillEllipse(rect.withCentre(screenPt));
		}
	}

	g->setColour(point->getColor());
	
	if(shape == Square)
		drawSquare(g, &screenPt, pt, pointSize);
	else if (shape == Star)
		drawStar(g, &screenPt, pointSize);
	else
	{
		Rectangle<float> rect(pointSize, pointSize);
		g->fillEllipse(rect.withCentre(screenPt));
	}
	
	Image* img = point->getLabelImage();
	paintPointLabel(g, *img, screenPt, pointSize * (shape == Square ? 0.7f : 0.5f));
}

void Radar2D::paintConnection(Graphics* g, AmbiGroup* group, AmbiPoint* point) const
{
	const float dashLengths[2] = { 4, 4 };
	Point<float> screenPtA = getAbsoluteScreenPoint(getProjectedPoint(group->getPoint()).toFloat());
	Point<float> screenPtB = getAbsoluteScreenPoint(getProjectedPoint(point->getPoint()).toFloat());

	g->setColour(group->getColor());
	g->drawDashedLine(Line<float>(screenPtA, screenPtB), dashLengths, 2);
}

void Radar2D::renderOpenGL()
{
	jassert(OpenGLHelpers::isContextActive());

	const float desktopScale = (float)openGLContext.getRenderingScale();

	OpenGLHelpers::clear(radarColors.getRadarBackground());
	std::unique_ptr<LowLevelGraphicsContext> glRenderer(createOpenGLGraphicsContext(openGLContext,
	                                                                                roundToInt(desktopScale * getWidth()),
	                                                                                roundToInt(desktopScale * getHeight())));

	if (glRenderer != nullptr)
	{
		Graphics g(*glRenderer);
		g.addTransform(AffineTransform::scale(desktopScale));
		g.setColour(radarColors.getRadarLineColor());
		g.drawRect(radarViewport, 1);   // draw an outline around the component

		drawRadar(&g);
		
		if (pEditablePoints != nullptr && pRadarOptions->showEditablePoints)
		{
			for (int i = 0; i < pEditablePoints->size(); i++)
			{
				AmbiPoint* pt = pEditablePoints->get(i);
				if (pt != nullptr)
				{
					float scaler = pt->getDisplayScaler();
					paintPoint(&g, pt, getEditablePointSize(scaler), pRadarOptions->editablePointsAsSquare ? Square : Circle, pPointSelection->isPointSelected(i), getSelectedPointSize(scaler));
				}
			}

			// paint groups
			for(int i = 0; i < pEditablePoints->groupCount(); i++)
			{
				AmbiGroup* gPt = pEditablePoints->getGroup(i);
				if(gPt != nullptr)
				{
					float scaler = gPt->getDisplayScaler();
					for(int iSub = 0; iSub < gPt->groupPoints.size(); iSub++)
					{
						paintConnection(&g, gPt, gPt->groupPoints[iSub]);
					}

					paintPoint(&g, gPt, getEditablePointSize(scaler), Shape::Star, pPointSelection->isGroupSelected(i), getSelectedPointSize(scaler));
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
					float scaler = pt->getDisplayScaler();
					paintPoint(&g, pt, getDisplayOnlyPointSize(scaler), Circle);
				}
			}
		}

		if(selectionRectangleActive)
		{
			g.setColour(radarColors.getPointSelectionColor());
			Rectangle<float> rectangle(getAbsoluteScreenPoint(selectionRectangleStart), getAbsoluteScreenPoint(selectionRectangleEnd));
			g.drawRoundedRectangle(rectangle, 5.0f, 3.0f);
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

bool Radar2D::keyPressed(const KeyPress& key)
{
	if(key.isKeyCode(KeyPress::homeKey))
	{
		// reset zoom
		pZoomSettings->setCurrentCenterPointXY(0.0f, 0.0f);
		pZoomSettings->setCurrentCenterPointXZ(0.0f, 0.0f);
		pZoomSettings->setCurrentRadius(1.0f);
		return true;
	}

	if (pPointSelection->getSelectionMode() == PointSelection::None || pPointSelection->getSelectedIndices().size() == 0)
		return false;

	Array<int> selection = pPointSelection->getSelectedIndices();
	bool keyHandled = false;
	double dx = 0.0, dy = 0.0, dz = 0.0;

	if (key.isKeyCode(KeyPress::upKey))
	{
		if (radarMode == XY)
		{
			dy = NUDGE_VALUE;
		}
		else
		{
			dz = NUDGE_VALUE;
		}

		keyHandled = true;
	}

	else if (key.isKeyCode(KeyPress::downKey))
	{
		if (radarMode == XY)
		{
			dy = - NUDGE_VALUE;
		}
		else
		{
			dz = - NUDGE_VALUE;
		}

		keyHandled = true;
	}

	else if (key.isKeyCode(KeyPress::leftKey))
	{
		dx = - NUDGE_VALUE;
		keyHandled = true;
	}

	else if (key.isKeyCode(KeyPress::rightKey))
	{
		dx = NUDGE_VALUE;
		keyHandled = true;
	}

	if(pPointSelection->getSelectionMode() == PointSelection::Group)
	{
		for (int i : selection)
		{
			pEditablePoints->getGroup(i)->moveXYZ(dx, dy, dz, !key.getModifiers().isShiftDown());
		}
	}
	else if(pPointSelection->getSelectionMode() == PointSelection::Point)
	{
		for (int i : selection)
		{
			Point3D<double>* p = pEditablePoints->get(i)->getPoint();
			p->setXYZ(p->getX() + dx, p->getY() + dy, p->getZ() + dz);
		}
	}

	pPointSelection->notifyChange();

	return keyHandled;
}

void Radar2D::setRadarMode(RadarMode mode)
{
	if (mode == XY)
		return;

	this->radarMode = mode;
	resized();
}

Point<float> Radar2D::getRelativeScreenPoint(Point<float> valuePoint) const
{
	Rectangle<float> currentViewValueRect = pZoomSettings->getVisibleArea(radarMode != XY, radarMode != XZ_Half);

	Point<float> convertedPoint(
		float(valuePoint.getX() - currentViewValueRect.getX()) / currentViewValueRect.getWidth() * radarViewport.getWidth(),
		radarViewport.getHeight() - float(valuePoint.getY() - currentViewValueRect.getY()) / currentViewValueRect.getHeight() * radarViewport.getHeight());
	
	return convertedPoint;
}

Point<float> Radar2D::getValuePointFromRelativeScreenPoint(Point<float> relativeScreenPoint) const
{
	Rectangle<float> currentViewValueRect = pZoomSettings->getVisibleArea(radarMode != XY, radarMode != XZ_Half);

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

	double wantedRatioWidthToHeight = (radarMode == XZ_Half) ? 2 : 1;

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
			(radarMode != XZ_Half) ? int(getBounds().getHeight() - getBounds().getWidth() * wantedRatioWidthToHeight) : 0,
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

    if(e.mods.isAltDown())
    {
        lastRadarMovePoint = e.getPosition().toFloat();
        return;
    }
    
	double minDist = std::numeric_limits<double>::max();
	int minDistIndex = -1;
	bool isGroup = false;
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

	// look for group points
	for(int i = 0; i < pEditablePoints->groupCount(); i++)
	{
		AmbiPoint* pt = pEditablePoints->getGroup(i);
		double dist;
		if ((dist = valuePoint.getDistanceFrom(getProjectedPoint(pt->getPoint()).toFloat())) < minDist)
		{
			minDist = dist;
			minDistIndex = i;
			isGroup = true;
		}
	}
	if (minDistIndex >= 0 && minDist < getMaxPointSelectionDist())
	{
		if (isGroup)
		{
			if (e.mods.isShiftDown() && pPointSelection->getSelectionMode() == PointSelection::Group && !pPointSelection->isGroupSelected(minDistIndex))
			{
				// do not allow groups with common points
				for (int i : pPointSelection->getSelectedIndices())
				{
					for (int iNewPt = 0; iNewPt < pEditablePoints->getGroup(minDistIndex)->groupPoints.size(); iNewPt++)
					{
						if (pEditablePoints->getGroup(i)->groupPoints.contains(pEditablePoints->getGroup(minDistIndex)->groupPoints[iNewPt]))
							return;
					}
				}
			}

			pPointSelection->selectGroup(minDistIndex, e.mods.isShiftDown());
		}
		else
			pPointSelection->selectPoint(minDistIndex, e.mods.isShiftDown());
	}
	else
	{
		pPointSelection->unselectPoint();
		selectionRectangleStart = valuePoint;
		selectionRectangleEnd = valuePoint;
		selectionRectangleActive = true;
	}
}

void Radar2D::mouseDrag(const MouseEvent& e)
{
	Point<float> valuePoint = getValuePointFromAbsoluteScreenPoint(e.getPosition().toFloat());
	showCoordinates(valuePoint);

	if(e.mods.isAltDown())
	{
        Point<float> move = lastRadarMovePoint - e.getPosition().toFloat();
            
        move.setX(move.getX() / radarViewport.getWidth() * 2.0f);
        move.setY(move.getY() / radarViewport.getHeight() * ((radarMode == XZ_Half) ? 1.0f : 2.0f));
            
        switch (radarMode)
        {
        case XY:
            pZoomSettings->setCurrentCenterPointXY(pZoomSettings->getCurrentCenterPoint().getX() + move.getX(), pZoomSettings->getCurrentCenterPoint().getY() - move.getY());
            break;
        case XZ_Half:
        case XZ_Full:
            pZoomSettings->setCurrentCenterPointXZ(pZoomSettings->getCurrentCenterPoint().getX() + move.getX(), pZoomSettings->getCurrentCenterPoint().getZ() - move.getY());
            break;
        }
            
        lastRadarMovePoint = e.getPosition().toFloat();
	}
	else
	{
		if (!pRadarOptions->showEditablePoints)
			return;

		Array<int> selection = pPointSelection->getSelectedIndices();

		if (pPointSelection->getSelectionMode() == PointSelection::Point)
		{
		    Point3D<double>* ref = pEditablePoints->get(pPointSelection->getMainSelectedPointIndex())->getPoint();
            std::unique_ptr<AmbiGroup> localGroup(new AmbiGroup("TEMP", Point3D<double>(ref->getX(), ref->getY(), ref->getZ()), "temp", Colours::transparentBlack));
            for (int i : selection)
            {
                localGroup->groupPoints.add(pEditablePoints->get(i));
            }

			double dx = valuePoint.getX() - ref->getX();
			double dy = radarMode == XY ? valuePoint.getY() - ref->getY() : 0.0;
			double dz = radarMode == XY ? 0.0 : valuePoint.getY() - ref->getZ();

            localGroup->moveXYZ(dx, dy, dz, true);
		}
		else if(pPointSelection->getSelectionMode() == PointSelection::Group)
		{
			// keep relative positions
			int mainGroupIndex = pPointSelection->getMainSelectedPointIndex();
			Point3D<double> referencePoint = *pEditablePoints->getGroup(mainGroupIndex)->getPoint();

			Array<Point3D<double>> relativePositions;
			Array<int> selectedIndices;
			for (int i : selection)
			{
				if (i != pPointSelection->getMainSelectedPointIndex())
				{
					selectedIndices.add(i);
					relativePositions.add(*pEditablePoints->getGroup(i)->getPoint() - referencePoint);
				}
			}

			std::unique_ptr<Point3D<double>> newPoint = radarMode == XY
				? std::make_unique<Point3D<double>>(valuePoint.getX(), valuePoint.getY(), referencePoint.getZ())
				: std::make_unique<Point3D<double>>(valuePoint.getX(), referencePoint.getY(), valuePoint.getY());

			if(e.mods.isCtrlDown())
				pEditablePoints->setGroupAed(mainGroupIndex, newPoint->getAzimuth(), newPoint->getElevation(), newPoint->getDistance(), !e.mods.isShiftDown());
			else
				pEditablePoints->setGroupXyz(mainGroupIndex, newPoint->getX(), newPoint->getY(), newPoint->getZ(), !e.mods.isShiftDown());

			// other groups
			Point3D<double> mainPt= *pEditablePoints->getGroup(mainGroupIndex)->getPoint();
			for(int i = 0; i < selectedIndices.size(); i++)
			{
				pEditablePoints->setGroupXyz(selectedIndices[i], 
					mainPt.getX() + relativePositions[i].getX(), 
					mainPt.getY() + relativePositions[i].getY(), 
					mainPt.getZ() + relativePositions[i].getZ(), !e.mods.isShiftDown());
			}
		}
		else
		{
			selectionRectangleEnd = valuePoint;
		}
		pPointSelection->notifyChange();
	}
}

void Radar2D::setCenterPoint(Point<float> valuePoint) const
{
	switch (radarMode)
	{
	case XY:
		pZoomSettings->setCurrentCenterPointXY(valuePoint.getX(), valuePoint.getY());
		break;
	case XZ_Half:
	case XZ_Full:
		pZoomSettings->setCurrentCenterPointXZ(valuePoint.getX(), valuePoint.getY());
		break;
	}
}

void Radar2D::mouseUp(const MouseEvent& e)
{
	Point<float> valuePoint = getValuePointFromAbsoluteScreenPoint(e.getPosition().toFloat());

	if(e.mods.isAltDown() && !e.mouseWasDraggedSinceMouseDown())
	{
		setCenterPoint(valuePoint);

		if (e.mods.isShiftDown() || e.mods.isRightButtonDown())
		{
			pZoomSettings->setCurrentRadius(pZoomSettings->getCurrentRadius() / 0.8f);
		}
		else
		{
			pZoomSettings->setCurrentRadius(pZoomSettings->getCurrentRadius() * 0.8f);
		}
	}

	if(e.mouseWasDraggedSinceMouseDown() && selectionRectangleActive)
	{
		// select all points withing rectangle
		Rectangle<float> rectangle(selectionRectangleStart, selectionRectangleEnd);
		for(int i = 0; i < pEditablePoints->size(); i++)
		{
			Point3D<double>* p = pEditablePoints->get(i)->getPoint();
			Point<float> realCoords;
			realCoords.x = float(radarMode == XY ? p->getX() : p->getX());
			realCoords.y = float(radarMode == XY ? p->getY() : p->getZ());
			if(rectangle.contains(realCoords))
			{
				pPointSelection->selectPoint(i, true);
			}
		}
	}

	selectionRectangleActive = false;
}

void Radar2D::mouseDoubleClick(const MouseEvent& e)
{
    if(e.mods.isAltDown())
    {
        return;
    }
    
	Point<float> valuePoint = getValuePointFromAbsoluteScreenPoint(e.getPosition().toFloat());

	if (!pRadarOptions->showEditablePoints)
		return;

	if (pRadarOptions->maxNumberEditablePoints > 0 && pEditablePoints->size() >= pRadarOptions->maxNumberEditablePoints)
		return;

	// add new point
	Uuid newId = Uuid();
	int index = pEditablePoints->size();
	switch (radarMode) {
	case XY:
		pEditablePoints->addNew(newId.toString(), Point3D<double>(valuePoint.getX(), valuePoint.getY(), 0.0, pRadarOptions->getAudioParamForIndex(index)), pEditablePoints->getNewUniqueName(), TrackColors::getColor(index + 1));
		break;
	case XZ_Half:
	case XZ_Full:
		pEditablePoints->addNew(newId.toString(), Point3D<double>(valuePoint.getX(), 0.0, valuePoint.getY(), pRadarOptions->getAudioParamForIndex(index)), pEditablePoints->getNewUniqueName(), TrackColors::getColor(index + 1));
		break;
	}

	// select added point
	pPointSelection->selectPoint(pEditablePoints->size() - 1);
}

void Radar2D::showCoordinates(const Point<float>& point)
{
	String str;
	if (radarMode == XY)
	{
		str << "X: " << String(point.getX(), 2) << "; Y: " << String(point.getY(), 2);
	}
	else
	{
		str << "X: " << String(point.getX(), 2) << "; Z: " << String(point.getY(), 2);
	}

	updateInfoLabel(str);
}

void Radar2D::mouseMove(const MouseEvent& e)
{
	showCoordinates(getValuePointFromAbsoluteScreenPoint(e.getPosition().toFloat()));
}
