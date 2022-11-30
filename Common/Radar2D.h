/*
  ==============================================================================

    Radar2D.h
    Created: 22 Jul 2017 1:43:26pm
    Author:  Christian Schweizer

  ==============================================================================
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
	Radar2D(RadarMode mode, AmbiDataSet* pEditablePoints, AmbiDataSet* pDisplayOnlyPoints, ZoomSettings* pZoomSettings, PointSelection* pPointSelection, RadarOptions* pRadarOptions);
    ~Radar2D();

	Point<double> getProjectedPoint(Point3D<double>* point3_d) const;
	Point<float> getAbsoluteScreenPoint(Point<float> point) const;
	void paint (Graphics&) override;
	
	void resized() override;
	void mouseExit(const MouseEvent& e) override;
	void mouseEnter(const MouseEvent& e) override;
	double getMaxPointSelectionDist() const;
	void mouseDown(const MouseEvent& e) override;
    std::unique_ptr<Point3D<double> > extracted(Point3D<double> &referencePoint, const Point<float> &valuePoint);
    
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
	Point<float> getSpecialIconPositionForCenter(Point<float> centerPt, SpecialHandlingMode mode) const;
    void drawSquare(Graphics* g, Point<float>* screenPt, Point3D<double>* pt, float pointSize) const;
	void drawEmptySquare(Graphics* g, Point<float>* screenPt, Point3D<double>* pt, float pointSize) const;
    void drawStar(Graphics* g, Point<float>* screenPt, float pointSize) const;
    void drawStrechIcon(Graphics* g, Point<float> screenPt, float pointSize) const;
    void drawRotateIcon(Graphics* g, Point<float> screenPt, float pointSize, bool centerPoint) const;
	void paintPoint(Graphics* g, AmbiPoint* point, float pointSize, Shape shape, bool select = false, float selectionSize = 0.0, bool extendedHandles = false) const;
	void paintConnection(Graphics* g, AmbiGroup* group, AmbiPoint* point) const;

	void paintPointLabel(Graphics* g, Image labelImage, Point<float> screenPt, float offset) const;
	float getEditablePointSize(float scaler) const;
	float getDisplayOnlyPointSize(float scaler) const;
	float getFontSize() const;
	void drawRadar(Graphics* g) const;
	void drawInfoLabel(Graphics* g) const;
	void renderOpenGL() override;
	Image createRadarBackground() const;
	void updateRadarBackground();
	void updateInfoLabel(String info);
	void timerCallback() override;
    bool checkMouseActionMode(const ModifierKeys modifiers, MouseActionMode mode);
    void calculateRotationAroundReference(Point<int> currentMousePosition, Point3D<double> &referencePoint, double* rotationY, double* rotationZ);
    bool containsIncludingBoder(const Rectangle<int>* rect, Point<int> point) const;
    
    
private:
	OpenGLContext openGLContext;

	AmbiDataSet* pEditablePoints;
	AmbiDataSet* pDisplayOnlyPoints;
	Image radarBackground;
	Image infoImage;
	Rectangle<int> radarViewport;
	ZoomSettings* pZoomSettings;
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

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Radar2D)
};
