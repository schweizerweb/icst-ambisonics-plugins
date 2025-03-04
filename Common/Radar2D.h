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
#include "AmbiDataSet.h"
#include "ZoomSettings.h"
#include "RadarColors.h"
#include "PointSelection.h"
#include "RadarOptions.h"

#define INFO_FONT_SIZE	15
#define ACTIVE_REFRESH_RATE 25
#define INACTIVE_REFRESH_RATE 10
#define NUDGE_VALUE 0.01
#define SIN45 0.70710678118f

//==============================================================================
/*
*/
class Radar2D    : public Component, OpenGLRenderer, ChangeListener, Timer
{
public:
	enum RadarMode { XY, XZ_Half, XZ_Full };
	enum Shape { Circle, Square, Star };
    enum MouseActionMode { Standard, RadarMove, RadarZoomOut, MoveGroupPointOnly };
    enum SpecialHandlingMode { None, Stretch, RotateAroundGroupPoint, RotateInAedSpace };
    enum AnchorX { X_Left, X_Center, X_Right };
    enum AnchorY { Y_Top, Y_Center, Y_Bottom };
	Radar2D(RadarMode mode, AmbiDataSet* pEditablePoints, AmbiDataSet* pDisplayOnlyPoints, PointSelection* pPointSelection, RadarOptions* pRadarOptions);
    ~Radar2D() override;

	Point<double> getProjectedPoint(Vector3D<double>* point3_d) const;
	Point<float> getAbsoluteScreenPoint(Point<float> point) const;
	void paint (Graphics&) override;
	
	void resized() override;
	void mouseExit(const MouseEvent& e) override;
	void mouseEnter(const MouseEvent& e) override;
	double getMaxPointSelectionDist() const;
	void mouseDown(const MouseEvent& e) override;
    
    void mouseDrag(const MouseEvent& e) override;
	void setCenterPoint(Point<float> valuePoint) const;
	void mouseUp(const MouseEvent& e) override;
	void mouseDoubleClick(const MouseEvent& e) override;
	void showCoordinates(const Point<float>& point);
	void mouseMove(const MouseEvent& e) override;
	void newOpenGLContextCreated() override;
	void openGLContextClosing() override;
	void changeListenerCallback(ChangeBroadcaster* source) override;
	bool keyPressed(const KeyPress& key) override;
    void modifierKeysChanged(const ModifierKeys &modifiers) override;
	void setRadarMode(RadarMode radarMode);
    void mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) override;
    void setAnchor(AnchorX x, AnchorY y);
    void setRefreshRate(int rateHz);
    
private:
	Point<float> getRelativeScreenPoint(Point<float> valuePoint) const;
	Point<float> getValuePointFromRelativeScreenPoint(Point<float> relativeScreenPoint) const;
	Point<float> getValuePointFromAbsoluteScreenPoint(Point<float> absoluteScreenPoint) const;
	float getValueToScreenRatio() const;
	float getSelectedPointSize(float scaler) const;
    float getSelectedGroupPointSize(float scaler) const;
	Point<float> getSpecialIconPositionForCenter(Point<float> centerPt, SpecialHandlingMode mode) const;
    void drawSquare(Graphics* g, Point<float>* screenPt, Vector3D<double>* pt, float pointSize) const;
	void drawEmptySquare(Graphics* g, Point<float>* screenPt, Vector3D<double>* pt, float pointSize) const;
    void drawStar(Graphics* g, Point<float>* screenPt, float pointSize) const;
    void drawStrechIcon(Graphics* g, Point<float> screenPt, float pointSize) const;
    void drawRotateIcon(Graphics* g, Point<float> screenPt, float pointSize, bool centerPoint) const;
	void paintPoint(Graphics* g, Vector3D<double> absPoint, AmbiPoint* point, float pointSize, Shape shape, bool select = false, float selectionSize = 0.0, bool extendedHandles = false, bool groupFlag = false) const;
	void paintConnection(Graphics* g, AmbiGroup* group, Vector3D<double> absSourcePoint) const;

	void paintPointLabel(Graphics* g, Image labelImage, Point<float> screenPt, float pointSize, float offsetFactor, bool groupFlag) const;
	float getEditablePointSize(float scaler) const;
	float getDisplayOnlyPointSize(float scaler) const;
    float getGroupPointSize(float scaler) const;
	float getFontSize() const;
	void drawRadar(Graphics* g) const;
	void drawInfoLabel(Graphics* g) const;
	void renderOpenGL() override;
	Image createRadarBackground() const;
	void updateRadarBackground();
	void updateInfoLabel(String info);
	void timerCallback() override;
    bool checkMouseActionMode(const ModifierKeys modifiers, MouseActionMode mode);
    void calculateRotationAroundReference(Point<int> currentMousePosition, Vector3D<double> &referencePoint, double* rotationY, double* rotationZ);
    bool containsIncludingBoder(const Rectangle<int>* rect, Point<int> point) const;
    
    
private:
	OpenGLContext openGLContext;

	AmbiDataSet* pEditablePoints;
	AmbiDataSet* pDisplayOnlyPoints;
	Image radarBackground;
	Image infoImage;
	Rectangle<int> radarViewport;
    Rectangle<int> radarViewportWithBorder;
	RadarMode radarMode;
	PointSelection* pPointSelection;
	RadarColors radarColors;
	RadarOptions* pRadarOptions;
	CriticalSection radarBackgroundLock;
	CriticalSection infoLabelLock;
	Point<float> selectionRectangleEnd;
	Point<float> selectionRectangleStart;
    Point<float> lastRadarMovePoint;
	bool selectionRectangleActive;
    bool specialGroupManipulationMode;
    SpecialHandlingMode currentSpecialHandlingMode;
    Point<float> specialHandlingOffset;
    Point<int> lastSpecialModePosition;
    float lastCartesianLimit;
    AnchorX anchorX;
    AnchorY anchorY;
    float border;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Radar2D)
};
