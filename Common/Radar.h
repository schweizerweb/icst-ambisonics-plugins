/*
==============================================================================

Radar.h
Created: 23 Jul 2016 1:49:21pm
Author:  christian.schweizer

==============================================================================
*/

#ifndef RADAR_H_INCLUDED
#define RADAR_H_INCLUDED

#define MAX_DISTANCE_CATCH_POINT 0.2

#include "JuceHeader.h"
#include "AmbiPoint.h"
#include "TrackColors.h"

//==============================================================================
/*
*/
class Radar : public OpenGLAppComponent
{
private:
	Rectangle<int> _xyRadarViewport;
	Rectangle<int> _yzRadarViewport;
	AmbiPoint* _selectedXYPoint;
	AmbiPoint* _selectedYZPoint;
	Array<AmbiPoint>* pAmbiPointArray;
	//GlLabel _labelX;
	//GlLabel _labelY;
	//GlLabel _labelZ;
	//GlLabel _coordinateLabelAed;
	//GlLabel _coordinateLabelXyz;
	//Array<GlLabel> _pointLabels;
	float _desktopScale;
	TrackColors trackColors;

public:
	Radar(Array<AmbiPoint>* pAmbiPointArray)
	{
		// In your constructor, you should add any child components, and
		// initialise any special settings that your component needs.
		_selectedXYPoint = nullptr;
		_selectedYZPoint = nullptr;
		this->pAmbiPointArray = pAmbiPointArray;
	}

	~Radar()
	{
	}

	void initialise() override
	{
		//_coordinateLabelAed.setPosition(Rectangle<double>(0.2, 1.0, 0.8, -0.05));
		//_coordinateLabelAed.setJustification(Justification::right);
		//_coordinateLabelXyz.setJustification(Justification::right);
		//_coordinateLabelXyz.setPosition(Rectangle<double>(0.2, 0.95, 0.8, -0.05));
		//_labelX.setLabel("X");
		//_labelX.setPosition(Rectangle<double>(0, 1.0, 0.1, -0.1));
		//_labelY.setLabel("Y");
		//_labelY.setPosition(Rectangle<double>(0.9, 0.1, 0.1, -0.1));
		//_labelZ.setLabel("Z");
		//_labelZ.setPosition(Rectangle<double>(0, 1.0, 0.1, -0.1));
		
	}

	void shutdown() override
	{
		//_labelX.shutdown();
		//_labelY.shutdown();
		//_labelZ.shutdown();
		//_coordinateLabelAed.shutdown();
		//_coordinateLabelXyz.shutdown();
	}

	void paint(Graphics& g) override
	{
		g.setColour(Colours::grey);
		g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

		g.setColour(Colours::black);
		g.setFont(14.0f);
		g.drawText("Radar", getLocalBounds(),
			Justification::top, true);   // draw some placeholder text
	}

	void resized() override
	{
		// This is called when the MainContentComponent is resized.
		// If you add any child components, this is where you should
		// update their positions.
		Rectangle<int> rect = getLocalBounds();
		setSize(rect.getWidth(), rect.getHeight());
		_desktopScale = (float)openGLContext.getRenderingScale();

		double wantedRelationHeightToWidth = 1.5;
		double effectiveWidth = getHeight() >= wantedRelationHeightToWidth*rect.getWidth() ?
			_desktopScale * rect.getWidth() :
			_desktopScale * rect.getHeight() / wantedRelationHeightToWidth;
		double effectiveHeight = wantedRelationHeightToWidth*effectiveWidth;

		double topLeftX = ((_desktopScale * rect.getWidth() - effectiveWidth) / 2.0);
		double topLeftY1 = ((_desktopScale * rect.getHeight() - effectiveHeight) / 2.0) + effectiveHeight / 3.0;
		double topLeftY2 = ((_desktopScale * rect.getHeight() - effectiveHeight) / 2.0) - effectiveHeight / 3.0;

		_xyRadarViewport = Rectangle<int>(roundToInt(topLeftX), roundToInt(topLeftY1), roundToInt(effectiveWidth), roundToInt(effectiveHeight * 2.0 / 3.0));
		_yzRadarViewport = Rectangle<int>(roundToInt(topLeftX), roundToInt(topLeftY2), roundToInt(effectiveWidth), roundToInt(effectiveHeight * 2.0 / 3.0));
	}

	Point<double> getGLPoint(Point<int> windowPoint, Rectangle<int> viewPort, bool bound = false)
	{
		Point<int> trVpPoint = windowPoint.translated(-viewPort.getTopLeft().x, -viewPort.getTopLeft().y);
		Point<double> glPoint = Point<double>(trVpPoint.x *2.0 / viewPort.getWidth() - 1.0, trVpPoint.y * 2.0 / viewPort.getHeight() - 1.0);
		if (bound)
		{
			glPoint.x = std::max(std::min(glPoint.x, 1.0), -1.0);
			glPoint.y = std::max(std::min(glPoint.y, 1.0), -1.0);
		}
		return glPoint;
	}

	void mouseDoubleClick(const MouseEvent& e) override
	{
		Point<int> trPoint = Point<int>(e.x, getHeight() - e.y);
		trPoint *= _desktopScale;
		Point<double> glPoint = Point<double>(0.0, 0.0);


		pAmbiPointArray->getLock().enter();
		if (_xyRadarViewport.contains(trPoint)) {
			glPoint = getGLPoint(trPoint, _xyRadarViewport);
			pAmbiPointArray->add(AmbiPoint(Point3D<double>(glPoint.getY(), glPoint.getX(), 0.0)));
		}
		else if (_yzRadarViewport.contains(trPoint)) {
			glPoint = getGLPoint(trPoint, _yzRadarViewport);
			pAmbiPointArray->add(AmbiPoint(Point3D<double>(0.0, glPoint.getX(), glPoint.getY())));
		}
		pAmbiPointArray->getLock().exit();
	}
	void mouseDown(const MouseEvent& e) override
	{	
		Point<int> trPoint = Point<int>(e.x, getHeight() - e.y);
		trPoint *= _desktopScale;

		if (pAmbiPointArray->size() <= 0)
			return;

		// xy-radar
		if (_xyRadarViewport.contains(trPoint))
		{
			Point<double> glPoint = getGLPoint(trPoint, _xyRadarViewport);
			int nearestPtIndex = 0;
			double nearestDist = Point<double>(pAmbiPointArray->getReference(nearestPtIndex).getPoint()->getY(), pAmbiPointArray->getReference(nearestPtIndex).getPoint()->getX()).getDistanceFrom(glPoint);
			for (int i = 1; i < pAmbiPointArray->size(); i++) {
				double newDist = Point<double>(pAmbiPointArray->getReference(i).getPoint()->getY(), pAmbiPointArray->getReference(i).getPoint()->getX()).getDistanceFrom(glPoint);
				if (newDist < nearestDist)
				{
					nearestPtIndex = i;
					nearestDist = newDist;
				}
			}
			if (nearestDist < MAX_DISTANCE_CATCH_POINT)
				_selectedXYPoint = &(pAmbiPointArray->getReference(nearestPtIndex));
		}
		// z-radar
		if (_yzRadarViewport.contains(trPoint))
		{
			Point<double> glPoint = getGLPoint(trPoint, _yzRadarViewport);
			int nearestPtIndex = 0;
			double nearestDist = Point<double>(pAmbiPointArray->getReference(nearestPtIndex).getPoint()->getY(), pAmbiPointArray->getReference(nearestPtIndex).getPoint()->getZ()).getDistanceFrom(glPoint);
			for (int i = 1; i < pAmbiPointArray->size(); i++) {
				double newDist = Point<double>(pAmbiPointArray->getReference(i).getPoint()->getY(), pAmbiPointArray->getReference(i).getPoint()->getZ()).getDistanceFrom(glPoint);
				if (newDist < nearestDist)
				{
					nearestPtIndex = i;
					nearestDist = newDist;
				}
			}
			if (nearestDist < MAX_DISTANCE_CATCH_POINT)
				_selectedYZPoint = &(pAmbiPointArray->getReference(nearestPtIndex));
		}
	}
	
	void mouseDrag(const MouseEvent& e) override
	{
		Point<int> trPos = Point<int>(e.x, getHeight() - e.y);
		trPos *= _desktopScale;

		if (_selectedXYPoint != nullptr) {
			Point<double> glPoint = getGLPoint(trPos, _xyRadarViewport, true);
			_selectedXYPoint->getPoint()->setXY(glPoint.y, glPoint.x);
			//_coordinateLabelAed.setLabel(_selectedXYPoint->getPoint()->toStringAed());
			//_coordinateLabelXyz.setLabel(_selectedXYPoint->getPoint()->toString());
			repaint();
		}
		if (_selectedYZPoint != nullptr) {	
			Point<double> glPoint = getGLPoint(trPos, _yzRadarViewport, true);
			_selectedYZPoint->getPoint()->setYZ(glPoint.x, glPoint.y);
			//_coordinateLabelAed.setLabel(_selectedYZPoint->getPoint()->toStringAed());
			//_coordinateLabelXyz.setLabel(_selectedYZPoint->getPoint()->toString());
			repaint();
		}
	}

	void mouseUp(const MouseEvent&) override
	{
		// button toggles state on mouse over.
		_selectedXYPoint = nullptr;
		_selectedYZPoint = nullptr;
		//_coordinateLabelAed.setLabel("");
		//_coordinateLabelXyz.setLabel("");
	}

	void render() override
	{
		jassert(OpenGLHelpers::isContextActive());

		// initialize GL
		OpenGLHelpers::clear(Colour::greyLevel(0.9f));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// XY-Radar
		glColor3f(0.2f, 0.3f, 0.8f);
		glViewport(_xyRadarViewport.getTopLeft().getX(), _xyRadarViewport.getTopLeft().getY(), _xyRadarViewport.getWidth(), _xyRadarViewport.getHeight());
		paintRadar();

		glEnable(GL_TEXTURE_2D);
		//_coordinateLabelAed.paint();
		//_coordinateLabelXyz.paint();
		//updatePointLabels();
		//for (int l = 0; l < _pointLabels.size(); l++)
		//	_pointLabels.getReference(l).paint();
		glDisable(GL_TEXTURE_2D);

		// Z-Radar
		glColor3f(0.2f, 0.3f, 0.8f);
		glViewport(_yzRadarViewport.getTopLeft().getX(), _yzRadarViewport.getTopLeft().getY(), _yzRadarViewport.getWidth(), _yzRadarViewport.getHeight());
		paintRadar(true);
		glFlush();
	}

private:
	/*void updatePointLabels() 
	{
		if (_pointLabels.size() != pAmbiPointArray->size()) {
			clearTextures();
			_pointLabels.resize(pAmbiPointArray->size());
		}
		for (int i = 0; i < _pointLabels.size(); i++) 
		{
			_pointLabels.getReference(i).setLabel(pAmbiPointArray->getReference(i).getLabel()->getLabel());
			_pointLabels.getReference(i).setPosition(Rectangle<double>(pAmbiPointArray->getReference(i).getPoint()->getY() - 0.05, pAmbiPointArray->getReference(i).getPoint()->getX() + 0.15, 0.1, -0.1));
		}
	}*/

	void paintRadar(bool isZRadar = false)
	{
		double startAngle = 0.0;
		double stopAngle = isZRadar ? PI : 2 * PI;
		double angleResolution = PI / 90.0;

		//Draw Circles
		for (int i = 0; i <= 10; i++) {
			float radius = i*0.1f;
			drawCircle(0.0, 0.0, radius, startAngle, stopAngle, angleResolution, false);
		}

		// Draw Lines
		// horizontal
		glBegin(GL_LINE_STRIP);
		glVertex3d(-1.0, 0.0, 0.0);
		glVertex3d(1.0, 0.0, 0.0);
		glEnd();
		// vertical
		glBegin(GL_LINE_STRIP);
		glVertex3d(0.0, isZRadar ? 0.0 : -1.0, 0.0);
		glVertex3d(0.0, 1.0, 0.0);
		glEnd();

		//// right label
		//_labelY.paint();
		//// top label
		//isZRadar ? _labelZ.paint() : _labelX.paint();
		//
		pAmbiPointArray->getLock().enter();
		// Draw Points
		for (int i = 0; i < (*pAmbiPointArray).size(); i++)
		{
			AmbiPoint* ambiPoint = &(pAmbiPointArray->getReference(i));
			double localX = ambiPoint->getPoint()->getY();
			double localY = isZRadar ? ambiPoint->getPoint()->getZ() : ambiPoint->getPoint()->getX();
			Colour color = trackColors.getColor(ambiPoint->getColorIndex());
			glColor3f(color.getRed() / 255.0, color.getGreen() / 255.0, color.getBlue() / 255.0);
			drawCircle(localX, localY, 0.03, 0.0, 2 * PI, PI / 90, true);
		}

		pAmbiPointArray->getLock().exit();
	}

	void drawCircle(double centerX, double centerY, double radius, double startAngle, double stopAngle, double angleResolution, bool filled)
	{
		glBegin(filled?GL_POLYGON:GL_LINE_STRIP);
		for (double d = startAngle; d <= stopAngle; d += angleResolution)
			glVertex3d((cos(d) * radius) + centerX, (sin(d) * radius) + centerY, 0.0f);
		glEnd();
	}

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Radar)
};


#endif  // RADAR_H_INCLUDED
