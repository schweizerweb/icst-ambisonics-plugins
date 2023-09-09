/*
  ==============================================================================

    Radar2D.cpp
    Created: 22 Jul 2017 1:43:26pm
    Author:  Christian Schweizer

  ==============================================================================
*/

#include "JuceHeader.h"
#include "Radar2D.h"

#include "ActivationDialog.h"
#include "MultiActivationDialog.h"
#include "TrackColors.h"
#include "LabelCreator.h"

//==============================================================================
Radar2D::Radar2D(RadarMode mode, AmbiDataSet* _pEditablePoints, AmbiDataSet* _pDisplayOnlyPoints, PointSelection* _pPointSelection, RadarOptions* _pRadarOptions):
	pEditablePoints(_pEditablePoints),
	pDisplayOnlyPoints(_pDisplayOnlyPoints),
	radarMode(mode),
	pPointSelection(_pPointSelection),
	pRadarOptions(_pRadarOptions),
    specialGroupManipulationMode(false),
    border(15)
{
	radarBackground = Image();
	infoImage = Image();
	openGLContext.setRenderer(this);
	openGLContext.attachTo(*this);
	openGLContext.setContinuousRepainting(false);

	setWantsKeyboardFocus(true);

	pRadarOptions->zoomSettings->addChangeListener(this);

	startTimerHz(INACTIVE_REFRESH_RATE);
}

Radar2D::~Radar2D()
{
    pRadarOptions->zoomSettings->removeChangeListener(this);

	stopTimer();

	openGLContext.detach();
}

void Radar2D::setAnchor(AnchorX x, AnchorY y)
{
    anchorX = x;
    anchorY = y;
    border = 15;
}

Point<double> Radar2D::getProjectedPoint(Vector3D<double>* point3_d) const
{
	switch(radarMode)
	{
	case XY:
		return Point<double>(point3_d->x, point3_d->y);
	case XZ_Half:
	case XZ_Full:
		return Point<double>(point3_d->x, point3_d->z);
	}

	return Point<double>();
}

Point<float> Radar2D::getAbsoluteScreenPoint(Point<float> valuePoint) const
{
	return getRelativeScreenPoint(valuePoint) + radarViewport.getTopLeft().toFloat();
}

float Radar2D::getEditablePointSize(float scaler) const
{
	return (float)radarViewport.getWidth() / 30.0f * scaler * float(pRadarOptions->zoomSettings->getPointScaler());
}

float Radar2D::getGroupPointSize(float scaler) const
{
    return (float)radarViewport.getWidth() / 30.0f * scaler * float(pRadarOptions->zoomSettings->getGroupPointScaler());
}

float Radar2D::getDisplayOnlyPointSize(float scaler) const
{
    return (float)radarViewport.getWidth() / 50.0f * scaler * float(pRadarOptions->zoomSettings->getPointScaler());
}

float Radar2D::getFontSize() const
{
	return (float)radarViewport.getWidth() / 25.0f;
}

void Radar2D::drawRadar(Graphics* g) const
{
	const ScopedLock lock(radarBackgroundLock);
	g->drawImageAt(radarBackground, radarViewportWithBorder.getX(), radarViewportWithBorder.getY());
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

    Rectangle<float> localBounds = radarViewport.toFloat().withZeroOrigin();

	Image img = Image(Image::ARGB, int(localBounds.getWidth()), int(localBounds.getHeight()), true);
	Graphics g(img);
	g.setColour(radarColors.getRadarLineColor());
	g.setFont(getFontSize());
    
    float ringResolution = pRadarOptions->zoomSettings->getRingResolution();
    float dist = ringResolution * getValueToScreenRatio();
	Point<float> centerPoint = getRelativeScreenPoint(Point<float>(0.0, 0.0));
    int nbRings = 0;
    bool labelUp = centerPoint.getY() >= localBounds.getHeight()/2;
    
    for(int i = 1; (float)i * ringResolution < pRadarOptions->zoomSettings->getScalingInfo()->DistanceMax(); i++)
    {
        float absDist = (float)i * dist;
        float ringValue = (float)i * ringResolution;
		Rectangle<float> ellipseRect(centerPoint.getX() - absDist, centerPoint.getY() - absDist, 2 * absDist, 2 * absDist);
        
        // check if circle is in visible area
        if(localBounds.intersects(ellipseRect) && !ellipseRect.withSizeKeepingCentre(ellipseRect.getWidth()/2, ellipseRect.getHeight()/2).contains(localBounds))
        {
            g.drawEllipse(ellipseRect, 1.0f);
            if(-20<=centerPoint.getX() && localBounds.getWidth()>= centerPoint.getX())
            {
                g.drawSingleLineText(String(ringValue), int(centerPoint.getX() + 2), int(centerPoint.getY() + (labelUp?-1:1)* absDist));
            }
            
            nbRings++;
        }
        else if(nbRings > 0)
            break;
    }
	
    // axis
    g.drawLine(centerPoint.getX(), 0.0f, centerPoint.getX(), localBounds.getHeight(), 2.0f);
    g.drawLine(0.0f, centerPoint.getY(), localBounds.getWidth(), centerPoint.getY(), 2.0f);
	
    // axis label
    Rectangle<int> label1Rect(int(localBounds.getWidth()-22), int(centerPoint.getY() - (radarMode == XZ_Half ? 22 : 10)), 20, 20);
    g.setColour(radarColors.getRadarBackground());
    g.fillRect(label1Rect);
    g.setColour(radarColors.getRadarAxisColor());
    g.drawRect(label1Rect);
    g.drawFittedText("X", label1Rect, Justification::centred, 1);
    
    Rectangle<int> label2Rect(int(centerPoint.getX() - (radarMode == XZ_Half ? 22 : 10)), int((!labelUp || radarMode == XZ_Half) ? 2 : localBounds.getHeight() -22), 20, 20);
    g.setColour(radarColors.getRadarBackground());
    g.fillRect(label2Rect);
    g.setColour(radarColors.getRadarAxisColor());
    g.drawRect(label2Rect);
    g.drawFittedText(radarMode == XY ? "Y" : "Z", label2Rect, Justification::centred, 1);
    
    if(!pRadarOptions->scalingInfo->IsInfinite())
    {
        // draw cartesian limits
        g.setColour(radarColors.getValidRangeColor());
        float x = centerPoint.getX() + pRadarOptions->scalingInfo->CartesianMin() * getValueToScreenRatio();
        float y = centerPoint.getY() + pRadarOptions->scalingInfo->CartesianMin() * getValueToScreenRatio();
        float squareSize = (pRadarOptions->scalingInfo->CartesianMax() - pRadarOptions->scalingInfo->CartesianMin()) * getValueToScreenRatio();
        g.fillRect(x, y, squareSize, squareSize);
    }
    
    Rectangle<float> localBoundsAll = radarViewportWithBorder.toFloat().withZeroOrigin();
    Image imgAll = Image(Image::ARGB, int(localBoundsAll.getWidth()), int(localBoundsAll.getHeight()), true);
    Graphics gAll(imgAll);
    gAll.drawImage(img, localBounds.withPosition(border, border));
    
    // labels
    gAll.setFont(border);
    gAll.drawText(radarMode == XY ? "Front" : "Top", Rectangle<float>(0, 0, localBoundsAll.getWidth(), border), Justification::centred);
    gAll.drawText(radarMode == XY ? "Back" : "Bottom", Rectangle<float>(0, localBoundsAll.getHeight() - border, localBoundsAll.getWidth(), border), Justification::centred);
    if(radarMode != XZ_Half)
    {
        gAll.addTransform(AffineTransform::rotation(float(PI)/2, localBoundsAll.getWidth()/2, localBoundsAll.getWidth()/2));
        gAll.drawText("Right", Rectangle<float>(0, 0, localBoundsAll.getWidth(), border), Justification::centred);
        gAll.addTransform(AffineTransform::rotation(-float(PI), localBoundsAll.getWidth()/2, localBoundsAll.getWidth()/2));
        gAll.drawText("Left", Rectangle<float>(0, 0, localBoundsAll.getWidth(), border), Justification::centred);
    }
    
	return imgAll;
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
	return (float)radarViewport.getWidth() / (2.0f * pRadarOptions->zoomSettings->getCurrentRadius());
}

float Radar2D::getSelectedPointSize(float scaler) const
{
	return getEditablePointSize(scaler) * 1.5f;
}

float Radar2D::getSelectedGroupPointSize(float scaler) const
{
    return getGroupPointSize(scaler) * 1.5f;
}

Point<float> Radar2D::getSpecialIconPositionForCenter(Point<float> centerPt, SpecialHandlingMode mode) const
{
    float offset = getGroupPointSize(1.0f)*2.1f;
    switch(mode)
    {
        case Stretch:
            return centerPt.translated(-offset, 0.0f);
        case RotateAroundGroupPoint:
            return centerPt.translated(-0.2f * offset, -offset);
        case RotateInAedSpace:
            return centerPt.translated(-0.2f * offset, offset);
        
        case None:
        default:
            return centerPt;
    }
}

void Radar2D::drawSquare(Graphics* g, Point<float>* screenPt, Vector3D<double>* pt, float pointSize) const
{
	double angle = (radarMode == XY ? atan2(pt->x, pt->y) : atan2(pt->x, pt->z)) + PI * 0.25;

	Path p;
	p.addPolygon(*screenPt, 4, pointSize, float(angle));
	p.closeSubPath();
	g->fillPath(p);
}

void Radar2D::drawEmptySquare(Graphics* g, Point<float>* screenPt, Vector3D<double>* pt, float pointSize) const
{
    double angle = (radarMode == XY ? atan2(pt->x, pt->y) : atan2(pt->x, pt->z)) + PI * 0.25;

    Path p;
    p.addPolygon(*screenPt, 4, pointSize, float(angle));
    p.closeSubPath();
    g->strokePath(p, PathStrokeType(2.0));
}

void Radar2D::drawStar(Graphics* g, Point<float>* screenPt, float pointSize) const
{
	Path p;
	p.addStar(*screenPt, 8, pointSize / 2, pointSize);
	p.closeSubPath();
	g->fillPath(p);
}

void Radar2D::drawStrechIcon(Graphics* g, Point<float> screenPt, float pointSize) const
{
    Path p;
    p.addArrow(Line<float>(screenPt.translated(0.0f, -0.5f*pointSize), screenPt.translated(-1.2f*pointSize, -0.5f*pointSize)), pointSize / 6.0f, pointSize, pointSize / 2.0f);
    p.addArrow(Line<float>(screenPt.translated(0.0f, -0.5f*pointSize), screenPt.translated(1.2f*pointSize, -0.5f*pointSize)), pointSize / 6.0f, pointSize, pointSize / 2.0f);
    
    p.addArrow(Line<float>(screenPt.translated(-1.2f*pointSize, 0.5f*pointSize), screenPt.translated(0.0f, 0.5f*pointSize)), pointSize / 6.0f, pointSize, pointSize / 2.0f);
    p.addArrow(Line<float>(screenPt.translated(1.2f*pointSize, 0.5f*pointSize), screenPt.translated(0.0f, 0.5f*pointSize)), pointSize / 6.0f, pointSize, pointSize / 2.0f);
    
    g->fillPath(p);
}

void Radar2D::drawRotateIcon(Graphics* g, Point<float> screenPt, float pointSize, bool centerPoint) const
{
    Path p;
    p.addCentredArc(screenPt.getX(), screenPt.getY(), pointSize, pointSize, 0.0f, float(-PI*0.75), float(PI*0.75), true);
    g->strokePath(p, PathStrokeType(2.0f));
    
    Path p2;
    Point<float> startPoint = screenPt.translated(-pointSize*SIN45, pointSize*SIN45);
    p2.addArrow(Line<float>(startPoint, startPoint.translated(pointSize / 3.0f, pointSize/3.0f)), pointSize / 6.0f, pointSize, pointSize / 2.0f);
    Point<float> endPoint = screenPt.translated(pointSize*SIN45, pointSize*SIN45);
    p2.addArrow(Line<float>(endPoint, endPoint.translated(-pointSize / 3.0f, pointSize/3.0f)), pointSize / 6.0f, pointSize, pointSize / 2.0f);
    g->fillPath(p2);
    
    if(centerPoint)
    {
        drawStar(g, &screenPt, pointSize / 2.0f);
    }
    else
    {
        g->drawHorizontalLine(int(screenPt.getY()), screenPt.getX() - 1.5f * pointSize, screenPt.getX() + 1.5f * pointSize);
        g->drawVerticalLine(int(screenPt.getX()), screenPt.getY() - 1.5f * pointSize, screenPt.getY() + 1.5f * pointSize);
    }
}

void Radar2D::paintPointLabel(Graphics* g, Image labelImage, Point<float> screenPt, float offset, bool groupFlag) const
{
    double baseScaler = groupFlag ? pRadarOptions->zoomSettings->getGroupPointScaler() : pRadarOptions->zoomSettings->getPointScaler();
    int scaledImageWidth = int(labelImage.getWidth() * baseScaler);
    int scaledImageHeight = int(labelImage.getHeight() * baseScaler);
    int y = screenPt.getY() > offset + (float)scaledImageHeight
		? int(screenPt.getY() - offset - (float)scaledImageHeight)
		: int(screenPt.getY() + offset);
	if(screenPt.getX() > (float)getWidth() / 2.0f
		&& screenPt.getX() > ((float)getWidth() - (offset + (float)scaledImageWidth)))
	{
		g->drawImageWithin(labelImage, int(screenPt.getX() - offset - (float)scaledImageWidth), y, scaledImageWidth, scaledImageHeight, RectanglePlacement::stretchToFit);
	}
	else
	{
		g->drawImageWithin(labelImage, int(screenPt.getX() + offset), y, scaledImageWidth, scaledImageHeight, RectanglePlacement::stretchToFit);
	}
}

bool Radar2D::containsIncludingBoder(const Rectangle<int> *rect, Point<int> point) const
{
    return point.x >= rect->getX() && point.y >= rect->getY() && point.x <= rect->getRight() && point.y <= rect->getBottom();
}

void Radar2D::paintPoint(Graphics* g, Vector3D<double> absPoint, AmbiPoint* point, float pointSize, Shape shape, bool select, float selectionSize, bool extendedHandles, bool groupFlag) const
{
	Point<float> screenPt = getAbsoluteScreenPoint(getProjectedPoint(&absPoint).toFloat());
    
    if(!containsIncludingBoder(&radarViewport, screenPt.toInt()))
    {
        return;
    }

	if (select)
	{
		g->setColour(radarColors.getPointSelectionColor());
		if (shape == Square)
        {
			drawSquare(g, &screenPt, &absPoint, selectionSize);
        }
		else if (shape == Star)
        {
			drawStar(g, &screenPt, selectionSize);
        }
		else
		{
			Rectangle<float> rect(selectionSize, selectionSize);
			g->fillEllipse(rect.withCentre(screenPt));
		}
	}

    Colour c = point->getMute() ? point->getColor().withAlpha(0.2f) : point->getColor();
	g->setColour(c);

	if(shape == Square)
    {
		drawSquare(g, &screenPt, &absPoint, pointSize);
    }
	else if (shape == Star)
    {
		drawStar(g, &screenPt, pointSize);
    }
	else
	{
		Rectangle<float> rect(pointSize, pointSize);
		g->fillEllipse(rect.withCentre(screenPt));
	}
    
    if(point->getSolo())
    {
        g->setColour(Colours::red);
        drawEmptySquare(g, &screenPt, &absPoint, pointSize * 1.2f);
    }
    
    if(extendedHandles)
    {
        g->setColour(select && currentSpecialHandlingMode == Stretch ? radarColors.getExtendedHandleSelectionColor() : radarColors.getExtendedHandleColor());
        drawStrechIcon(g, getSpecialIconPositionForCenter(screenPt, Stretch), pointSize);
        
        g->setColour(select && currentSpecialHandlingMode == RotateAroundGroupPoint ? radarColors.getExtendedHandleSelectionColor() : radarColors.getExtendedHandleColor());
        drawRotateIcon(g, getSpecialIconPositionForCenter(screenPt, RotateAroundGroupPoint), pointSize, true);
        
        g->setColour(select && currentSpecialHandlingMode == RotateInAedSpace ? radarColors.getExtendedHandleSelectionColor() : radarColors.getExtendedHandleColor());
        drawRotateIcon(g, getSpecialIconPositionForCenter(screenPt, RotateInAedSpace), pointSize, false);
    }
	
	Image* img = point->getLabelImage();
	paintPointLabel(g, *img, screenPt, pointSize * (shape == Square ? 0.7f : 0.5f), groupFlag);
}

void Radar2D::paintConnection(Graphics* g, AmbiGroup* group, Vector3D<double> absSourcePoint) const
{
	const float dashLengths[2] = { 4, 4 };
    Vector3D<double> groupPos = group->getVector3D();
    Point<float> screenPtA = getAbsoluteScreenPoint(getProjectedPoint(&groupPos).toFloat());
	Point<float> screenPtB = getAbsoluteScreenPoint(getProjectedPoint(&absSourcePoint).toFloat());

	g->setColour(group->getColor());
	g->drawDashedLine(Line<float>(screenPtA, screenPtB), dashLengths, 2);
}

void Radar2D::renderOpenGL()
{
	jassert(OpenGLHelpers::isContextActive());

	const float desktopScale = (float)openGLContext.getRenderingScale();

    OpenGLHelpers::clear(radarColors.getRadarBackground());
    
	std::unique_ptr<LowLevelGraphicsContext> glRenderer(createOpenGLGraphicsContext(openGLContext,
	                                                                                roundToInt(desktopScale * (float)getWidth()),
	                                                                                roundToInt(desktopScale * (float)getHeight())));
	if (glRenderer != nullptr)
	{
        if(!approximatelyEqual(lastCartesianLimit, pRadarOptions->scalingInfo->CartesianMax()))
        {
            updateRadarBackground();
            lastCartesianLimit = pRadarOptions->scalingInfo->CartesianMax();
        }
        
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
                Vector3D<double> absPoint = pEditablePoints->getAbsSourcePoint(i);
				if (pt != nullptr && pt->getEnabled())
				{
					float scaler = pt->getDisplayScaler();
					paintPoint(&g, absPoint, pt, getEditablePointSize(scaler), pRadarOptions->editablePointsAsSquare ? Square : Circle, pPointSelection->isPointSelected(i), getSelectedPointSize(scaler));
                    AmbiGroup* grp = pt->getGroup();
                    if(grp != nullptr)
                    {
                        if(grp->getEnabled()) {
                            paintConnection(&g, grp, absPoint);
                        }
                    }
				}
			}

			// paint groups
			for(int i = 0; i < pEditablePoints->groupCount(); i++)
			{
				AmbiGroup* gPt = pEditablePoints->getGroup(i);
                Vector3D<double> absGrpPoint = gPt->getVector3D();
				if(gPt != nullptr && gPt->getEnabled())
				{
					float scaler = gPt->getDisplayScaler();
					
					paintPoint(&g, absGrpPoint, gPt, getGroupPointSize(scaler), Shape::Star, pPointSelection->isGroupSelected(i), getSelectedGroupPointSize(scaler), specialGroupManipulationMode, true);
				}
			}
		}

		int64 referenceTime = Time::currentTimeMillis();
		if (pDisplayOnlyPoints != nullptr && pRadarOptions->showDisplayOnlyPoints)
		{
			for (int i = pDisplayOnlyPoints->size() - 1; i >= 0 ; i--)
			{
				AmbiPoint* pt = pDisplayOnlyPoints->get(i, referenceTime, pRadarOptions->displayTimeout);
                if (pt != nullptr && pt->getEnabled())
				{
                    Vector3D<double> absPt = pDisplayOnlyPoints->getAbsSourcePoint(i);
					float scaler = pt->getDisplayScaler();
					paintPoint(&g, absPt, pt, getDisplayOnlyPointSize(scaler), Circle);
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
	if (source == pRadarOptions->zoomSettings)
		updateRadarBackground();
}

bool Radar2D::keyPressed(const KeyPress& key)
{
	if(key.isKeyCode(KeyPress::homeKey))
	{
		// reset zoom
        pRadarOptions->zoomSettings->Reset(pEditablePoints);
		return true;
	}

	if (pPointSelection->getSelectionMode() == PointSelection::None || pPointSelection->getSelectedIndices().size() == 0)
		return false;

	Array<int> selection = pPointSelection->getSelectedIndices();
	bool keyHandled = false;
	double dx = 0.0, dy = 0.0, dz = 0.0;

    if(pPointSelection->getSelectionMode() == PointSelection::Point
       && key.getModifiers().isCtrlDown()
       && key.getModifiers().isShiftDown())
    {
        if(key.isKeyCode(77)) // 'm'
        {
            bool anyNotMuted = false;
            for(auto i : selection)
            {
                if(!pEditablePoints->get(i)->getMute())
                {
                    anyNotMuted = true;
                    break;
                }
            }
            
            for(auto i : selection)
                pEditablePoints->setMute(i, anyNotMuted);
        }
        else if(key.isKeyCode(83)) // 's'
        {
            bool anyNotSolo = false;
            for(auto i : selection)
            {
                if(!pEditablePoints->get(i)->getSolo())
                {
                    anyNotSolo = true;
                    break;
                }
            }
            
            for(auto i : selection)
                pEditablePoints->setSolo(i, anyNotSolo);
        }
        else if(key.isKeyCode(KeyPress::backspaceKey))
        {
            for(auto i : selection)
            {
                pEditablePoints->setEnabled(i, false);
            }
        }
        
        return true;
    }
    if(pPointSelection->getSelectionMode() == PointSelection::Group
       && key.getModifiers().isCtrlDown()
       && key.getModifiers().isShiftDown()
       && key.isKeyCode(KeyPress::backspaceKey))
    {
        {
            pPointSelection->unselectPoint();
         
            for(auto i : selection)
            {
                pEditablePoints->removeGroup(i);
            }
        }
        
        return true;
    }
    
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
			pEditablePoints->moveGroupXyz(i, dx, dy, dz, !key.getModifiers().isShiftDown());
		}
	}
	else if(pPointSelection->getSelectionMode() == PointSelection::Point)
	{
		for (int i : selection)
		{
			Vector3D<double> p = pEditablePoints->getAbsSourcePoint(i);
            pEditablePoints->setAbsSourcePoint(i, Vector3D<double>(p.x + dx, p.y + dy, p.z + dz));
		}
	}

	pPointSelection->notifyChange();

	return keyHandled;
}

void Radar2D::modifierKeysChanged(const juce::ModifierKeys &modifiers) {
    specialGroupManipulationMode = modifiers.isAltDown();
    currentSpecialHandlingMode = None;
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
	Rectangle<float> currentViewValueRect = pRadarOptions->zoomSettings->getVisibleArea(radarMode != XY, radarMode != XZ_Half);

	Point<float> convertedPoint(
		float(valuePoint.getX() - currentViewValueRect.getX()) / currentViewValueRect.getWidth() * (float)radarViewport.getWidth(),
		(float)radarViewport.getHeight() - float(valuePoint.getY() - currentViewValueRect.getY()) / currentViewValueRect.getHeight() * (float)radarViewport.getHeight());
	
	return convertedPoint;
}

Point<float> Radar2D::getValuePointFromRelativeScreenPoint(Point<float> relativeScreenPoint) const
{
	Rectangle<float> currentViewValueRect = pRadarOptions->zoomSettings->getVisibleArea(radarMode != XY, radarMode != XZ_Half);

	Point<float> convertedPoint(
		float(relativeScreenPoint.getX() / (float)radarViewport.getWidth() * currentViewValueRect.getWidth()) + currentViewValueRect.getX(),
		float(((float)radarViewport.getHeight() - relativeScreenPoint.getY()) / (float)radarViewport.getHeight() * currentViewValueRect.getHeight()) + currentViewValueRect.getY());

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

	if (getBounds().getWidth() * getBounds().getHeight() <= 0)
		return;

    double wantedRatioWidthToHeight = (radarMode == XZ_Half) ? 2 : 1;

    auto bounds = getBounds();
    double fullSize = jmin((double)bounds.getWidth(), bounds.getHeight() * wantedRatioWidthToHeight) - 2 * border;
    
    double x, y, w, h;
    w = fullSize;
    h = fullSize / wantedRatioWidthToHeight;
    switch (anchorX) {
        case X_Left:
            x = border;
            break;
        case X_Right:
            x = bounds.getWidth() - fullSize - border;
            break;
        case X_Center:
        default:
            x = (bounds.getWidth() - fullSize) / 2.0;
            break;
    }
    
    switch (anchorY) {
        case Y_Top:
            y = border;
            break;
        case Y_Bottom:
            y = bounds.getHeight() - fullSize / wantedRatioWidthToHeight - border;
            break;
        default:
        case Y_Center:
            y = (bounds.getHeight() - fullSize / wantedRatioWidthToHeight) / 2.0;
            break;
    }
    
    radarViewport = Rectangle<double>(x, y, w, h).toNearestInt();
    radarViewportWithBorder = Rectangle<double>(x-border, y-border, w+2*border, h+2*border).toNearestInt();
	updateRadarBackground();
}

void Radar2D::mouseExit(const MouseEvent&)
{
	updateInfoLabel("");
    specialGroupManipulationMode = false;
    currentSpecialHandlingMode = None;
}

void Radar2D::mouseEnter(const MouseEvent&)
{
}

double Radar2D::getMaxPointSelectionDist() const
{
	return pRadarOptions->zoomSettings->getCurrentRadius()/15.0;
}

void Radar2D::mouseWheelMove(const MouseEvent& /*event*/, const MouseWheelDetails& wheel)
{
    if(wheel.deltaY != 0.0f)
    {
        if(specialGroupManipulationMode && pPointSelection->getSelectionMode() == PointSelection::Group)
        {
            pEditablePoints->stretchGroup(pPointSelection->getMainSelectedPointIndex(), pRadarOptions->zoomSettings->getCurrentRadius() * wheel.deltaY);
        }
        else
        {
            pRadarOptions->zoomSettings->setCurrentRadius(pRadarOptions->zoomSettings->getCurrentRadius() * (1 + wheel.deltaY));
        }
    }
    
    return;
}

void Radar2D::mouseDown(const MouseEvent& e)
{
	Point<float> valuePoint = getValuePointFromAbsoluteScreenPoint(e.getPosition().toFloat());

	if (!pRadarOptions->showEditablePoints)
		return;

    if(checkMouseActionMode(e.mods, RadarMove))
    {
        lastRadarMovePoint = e.getPosition().toFloat();
        return;
    }
    
	double minDist = std::numeric_limits<double>::max();
	int minDistIndex = -1;
	bool isGroup = false;
    SpecialHandlingMode specialHandlingMode = None;
    
	for (int i = 0; i < pEditablePoints->size(); i++)
	{
		if (!pEditablePoints->get(i)->getEnabled())
			continue;
		double dist;
        Vector3D<double> absPoint = pEditablePoints->getAbsSourcePoint(i);
		if ((dist = valuePoint.getDistanceFrom(getProjectedPoint(&absPoint).toFloat())) < minDist)
		{
			minDist = dist;
			minDistIndex = i;
		}
	}

	// look for group points
	for(int i = 0; i < pEditablePoints->groupCount(); i++)
	{
		AmbiPoint* pt = pEditablePoints->getGroup(i);
		if (!pt->getEnabled())
			continue;
	    double dist;
        Vector3D<double> absPoint = pt->getVector3D();
        if ((dist = valuePoint.getDistanceFrom(getProjectedPoint(&absPoint).toFloat())) < minDist)
		{
			minDist = dist;
			minDistIndex = i;
			isGroup = true;
            specialHandlingMode = None;
		}
        if(specialGroupManipulationMode)
        {
            Point<float> groupScreenPoint = getAbsoluteScreenPoint(getProjectedPoint(&absPoint).toFloat());
            SpecialHandlingMode specialHandleModes[3];
            specialHandleModes[0] = Stretch;
            specialHandleModes[1] = RotateAroundGroupPoint;
            specialHandleModes[2] = RotateInAedSpace;
            for(int si = 0; si < 3; si++)
            {
                Point<float> p = getSpecialIconPositionForCenter(groupScreenPoint, specialHandleModes[si]);
                if((dist = valuePoint.getDistanceFrom(getValuePointFromAbsoluteScreenPoint(p))) < minDist)
                {
                    minDist = dist;
                    minDistIndex = i;
                    isGroup = true;
                    specialHandlingMode = specialHandleModes[si];
                }
            }
        }
	}
	if (minDistIndex >= 0 && minDist < getMaxPointSelectionDist())
	{
		if (isGroup)
		{
            pPointSelection->selectGroup(minDistIndex, checkMouseActionMode(e.mods, MoveGroupPointOnly));
            currentSpecialHandlingMode = specialHandlingMode;
            lastSpecialModePosition = e.getPosition();
            Vector3D<double> absPoint = pEditablePoints->getGroup(minDistIndex)->getVector3D();
            Point<float> originalGroupPointPosition = getProjectedPoint(&absPoint).toFloat();
            specialHandlingOffset = getSpecialIconPositionForCenter(originalGroupPointPosition, specialHandlingMode) - originalGroupPointPosition;
		}
		else
			pPointSelection->selectPoint(minDistIndex, checkMouseActionMode(e.mods, MoveGroupPointOnly));
	}
	else
	{
		pPointSelection->unselectPoint();
		selectionRectangleStart = valuePoint;
		selectionRectangleEnd = valuePoint;
		selectionRectangleActive = true;
	}
}

void Radar2D::calculateRotationAroundReference(Point<int> currentMousePosition, Vector3D<double> &referencePoint, double* rotationY, double* rotationZ) {
    Point<float> referencePointScreenCoordinates = getAbsoluteScreenPoint(getProjectedPoint(&referencePoint).toFloat());
    Point<float> relativeOld = lastSpecialModePosition.toFloat() - referencePointScreenCoordinates;
    Point<float> relativeNew = currentMousePosition.toFloat() - referencePointScreenCoordinates;
    
    double angleOld = atan2(relativeOld.getX(), relativeOld.getY());
    double angleNew = atan2(relativeNew.getX(), relativeNew.getY());
    double angle = angleOld - angleNew;
    *rotationZ = radarMode == XY ? angle : 0.0;
    *rotationY = radarMode != XY ? angle : 0.0;
}

void Radar2D::mouseDrag(const MouseEvent& e)
{
    if(!e.mouseWasDraggedSinceMouseDown())
    {
        return;
    }

	if(checkMouseActionMode(e.mods, RadarMove))
	{
        Point<float> move = lastRadarMovePoint - e.getPosition().toFloat();
            
        move.setX(move.getX() / (float)radarViewport.getWidth() * pRadarOptions->zoomSettings->getCurrentRadius() * 2.0f);
        move.setY(move.getY() / (float)radarViewport.getHeight() * pRadarOptions->zoomSettings->getCurrentRadius() * ((radarMode == XZ_Half) ? 1.0f : 2.0f));
            
        switch (radarMode)
        {
        case XY:
            pRadarOptions->zoomSettings->setCurrentCenterPointXY(pRadarOptions->zoomSettings->getCurrentCenterPoint().getX() + move.getX(), pRadarOptions->zoomSettings->getCurrentCenterPoint().getY() - move.getY());
            break;
        case XZ_Half:
        case XZ_Full:
            pRadarOptions->zoomSettings->setCurrentCenterPointXZ(pRadarOptions->zoomSettings->getCurrentCenterPoint().getX() + move.getX(), pRadarOptions->zoomSettings->getCurrentCenterPoint().getZ() - move.getY());
            break;
        }
            
        lastRadarMovePoint = e.getPosition().toFloat();
	}
	else
	{
		if (!pRadarOptions->showEditablePoints)
			return;

        Point<float> valuePoint;
        if(specialGroupManipulationMode && currentSpecialHandlingMode != None)
            valuePoint = getValuePointFromAbsoluteScreenPoint(e.getPosition().toFloat() - specialHandlingOffset);
        else
            valuePoint = getValuePointFromAbsoluteScreenPoint(e.getPosition().toFloat());
        
        showCoordinates(valuePoint);
        
		Array<int> selection = pPointSelection->getSelectedIndices();

		if (pPointSelection->getSelectionMode() == PointSelection::Point)
		{
		    Vector3D<double> ref = pEditablePoints->getAbsSourcePoint(pPointSelection->getMainSelectedPointIndex());
            
			double dx = valuePoint.getX() - ref.x;
			double dy = radarMode == XY ? valuePoint.getY() - ref.y : 0.0;
			double dz = radarMode == XY ? 0.0 : valuePoint.getY() - ref.z;

            for (int i : selection)
            {
                Vector3D<double> p = pEditablePoints->getAbsSourcePoint(i);
                pEditablePoints->setAbsSourcePoint(i, Vector3D<double>(p.x + dx, p.y + dy, p.z + dz));
            }
		}
		else if(pPointSelection->getSelectionMode() == PointSelection::Group)
		{
			// keep relative positions
			int mainGroupIndex = pPointSelection->getMainSelectedPointIndex();
			Vector3D<double> referencePoint = pEditablePoints->getGroup(mainGroupIndex)->getVector3D();
			Vector3D<double> newPoint = radarMode == XY
				? Vector3D<double>(valuePoint.getX(), valuePoint.getY(), referencePoint.z)
				: Vector3D<double>(valuePoint.getX(), referencePoint.y, valuePoint.getY());

            if(currentSpecialHandlingMode == Stretch)
            {
                double stretchFactor = (float)(lastSpecialModePosition.getY() - e.getPosition().getY()) * pRadarOptions->zoomSettings->getCurrentRadius() * 0.01f;
                for(int i : selection)
                    pEditablePoints->stretchGroup(i, stretchFactor);
                
                lastSpecialModePosition = e.getPosition();
            }
            else if(currentSpecialHandlingMode == RotateAroundGroupPoint)
            {
                double rotationZ;
                double rotationY;
                calculateRotationAroundReference(e.getPosition(), referencePoint, &rotationY, &rotationZ);
                
                for(int i : selection)
                    pEditablePoints->rotateGroup(i, 0.0, rotationY, rotationZ);
                
                lastSpecialModePosition = e.getPosition();
            }
            else if(currentSpecialHandlingMode == RotateInAedSpace)
            {
                Vector3D<double> zeroPoint(0.0, 0.0, 0.0);
                double rotationZ;
                double rotationY;
                calculateRotationAroundReference(e.getPosition(), zeroPoint, &rotationY, &rotationZ);
                    
                for (int i : selection)
                    pEditablePoints->rotateGroupAroundOrigin(i, 0.0, rotationY, rotationZ, !checkMouseActionMode(e.mods, MoveGroupPointOnly));
                
                lastSpecialModePosition = e.getPosition();
            }
            else
            {
                // move
                Array<Vector3D<double>> relativePositions;
                Array<int> selectedIndices;
                for (int i : selection)
                {
                    if (i != mainGroupIndex)
                    {
                        selectedIndices.add(i);
                        relativePositions.add(pEditablePoints->getGroup(i)->getVector3D() - referencePoint);
                    }
                }

                pEditablePoints->setGroupXyz(mainGroupIndex, newPoint.x, newPoint.y, newPoint.z, !checkMouseActionMode(e.mods, MoveGroupPointOnly));
                
                // other groups
                Point3D<double> mainPt= *pEditablePoints->getGroup(mainGroupIndex)->getRawPoint();
                for(int i = 0; i < selectedIndices.size(); i++)
                {
                    pEditablePoints->setGroupXyz(selectedIndices[i],
                                                 mainPt.getX() + relativePositions[i].x,
                                                 mainPt.getY() + relativePositions[i].y,
                                                 mainPt.getZ() + relativePositions[i].z,
                                                 !checkMouseActionMode(e.mods, MoveGroupPointOnly));
                }
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
        pRadarOptions->zoomSettings->setCurrentCenterPointXY(valuePoint.getX(), valuePoint.getY());
		break;
	case XZ_Half:
	case XZ_Full:
        pRadarOptions->zoomSettings->setCurrentCenterPointXZ(valuePoint.getX(), valuePoint.getY());
		break;
	}
}

void Radar2D::mouseUp(const MouseEvent& e)
{
	Point<float> valuePoint = getValuePointFromAbsoluteScreenPoint(e.getPosition().toFloat());

	if(checkMouseActionMode(e.mods, RadarMove) && !e.mouseWasDraggedSinceMouseDown())
	{
		setCenterPoint(valuePoint);

		if (checkMouseActionMode(e.mods, RadarZoomOut))
		{
            pRadarOptions->zoomSettings->setCurrentRadius(pRadarOptions->zoomSettings->getCurrentRadius() / 0.8f);
		}
		else
		{
            pRadarOptions->zoomSettings->setCurrentRadius(pRadarOptions->zoomSettings->getCurrentRadius() * 0.8f);
		}
	}

	if(e.mouseWasDraggedSinceMouseDown() && selectionRectangleActive)
	{
		// select all points withing rectangle
		Rectangle<float> rectangle(selectionRectangleStart, selectionRectangleEnd);
		for(int i = 0; i < pEditablePoints->size(); i++)
		{
			if (!pEditablePoints->get(i)->getEnabled())
				continue;
			Vector3D<double> p = pEditablePoints->getAbsSourcePoint(i);
			Point<float> realCoords;
			realCoords.x = float(radarMode == XY ? p.x : p.x);
			realCoords.y = float(radarMode == XY ? p.y : p.z);
			if(rectangle.contains(realCoords))
			{
				pPointSelection->selectPoint(i, true);
			}
		}
	}

	selectionRectangleActive = false;
    currentSpecialHandlingMode = None;
}

void Radar2D::mouseDoubleClick(const MouseEvent& e)
{
    if(checkMouseActionMode(e.mods, RadarMove))
    {
        return;
    }
    
	Point<float> valuePoint = getValuePointFromAbsoluteScreenPoint(e.getPosition().toFloat());

	if (!pRadarOptions->showEditablePoints)
		return;

	if (pRadarOptions->maxNumberEditablePoints > 0 && pEditablePoints->getEnabledCount() >= pRadarOptions->maxNumberEditablePoints)
		return;

	if (pEditablePoints->size() < pRadarOptions->maxNumberEditablePoints)
	{
		// add new point if capacity allows it
		Uuid newId = Uuid();
		int index = pEditablePoints->size();
		switch (radarMode) {
		case XY:
			pEditablePoints->addNew(newId.toString(), Point3D<double>(valuePoint.getX(), valuePoint.getY(), 0.0, pRadarOptions->getAudioParamForIndex(index, false)), pEditablePoints->getNewUniqueName(), TrackColors::getColor(index + 1));
			break;
		case XZ_Half:
		case XZ_Full:
			pEditablePoints->addNew(newId.toString(), Point3D<double>(valuePoint.getX(), 0.0, valuePoint.getY(), pRadarOptions->getAudioParamForIndex(index, false)), pEditablePoints->getNewUniqueName(), TrackColors::getColor(index + 1));
			break;
		}

		// select point
		pPointSelection->selectPoint(pEditablePoints->size() - 1);
	}
	else 
	{
        if(e.mods.isShiftDown())
        {
            CallOutBox::launchAsynchronously(std::make_unique<MultiActivationDialog>(pEditablePoints, valuePoint, pRadarOptions, radarMode == XY), Rectangle<int>(e.getPosition().translated(3, 3), e.getPosition().translated(3, 3)), this);
        }
        else {
            // otherwise enable a disabled point
            CallOutBox::launchAsynchronously(std::make_unique<ActivationDialog>(pEditablePoints, valuePoint, radarMode == XY), Rectangle<int>(e.getPosition(), e.getPosition().translated(3, 3)), this);
        }
	}
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

bool Radar2D::checkMouseActionMode(const ModifierKeys modifiers, MouseActionMode mode)
{
    if(modifiers.isCtrlDown() || modifiers.isRightButtonDown())
        return (modifiers.isShiftDown() && mode == RadarZoomOut) || (mode == RadarMove);
    
    if(modifiers.isShiftDown())
        return mode == MoveGroupPointOnly;

    return mode == Standard;
}

void Radar2D::setRefreshRate(int rateHz)
{
    stopTimer();
    startTimerHz(rateHz);
}
