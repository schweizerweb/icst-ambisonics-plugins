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
#define INFO_FONT_SIZE	20

//==============================================================================
/*
*/
class Radar2D    : public Component, OpenGLRenderer, ChangeListener
{
public:
	enum RadarMode { XY, ZY };

	Radar2D(RadarMode mode, AmbiDataSet* pEditablePoints, AmbiDataSet* pDisplayOnlyPoints, ZoomSettings* pZoomSettings, PointSelection* pPointSelection, RadarOptions* pRadarOptions);
    ~Radar2D();

	Point<double> getProjectedPoint(Point3D<double>* point3_d) const;
	Point<float> getAbsoluteScreenPoint(Point<float> point) const;
	void paint (Graphics&) override;
	
	void resized() override;
	void mouseExit(const MouseEvent& e) override;
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

private:
	Point<float> getRelativeScreenPoint(Point<float> valuePoint) const;
	Point<float> getValuePointFromRelativeScreenPoint(Point<float> relativeScreenPoint) const;
	Point<float> getValuePointFromAbsoluteScreenPoint(Point<float> absoluteScreenPoint) const;
	float getValueToScreenRatio() const;
	float getSelectedPointSize(float scaler) const;
	void drawSquare(Graphics* g, Point<float>* screenPt, Point3D<double>* pt, float pointSize) const;
	void paintPoint(Graphics* g, AmbiPoint* point, float pointSize, bool square, bool select = false, float selectionSize = 0.0) const;
	
	void paintPointLabel(Graphics* g, Image labelImage, Point<float> screenPt, float offset) const;
	float getEditablePointSize(float scaler) const;
	float getDisplayOnlyPointSize(float scaler) const;
	float getFontSize() const;
	void drawRadar(Graphics* g) const;
	void drawInfoLabel(Graphics* g) const;
	void renderOpenGL() override;
	Image* createRadarBackground() const;
	void updateRadarBackground();
	void updateInfoLabel(String info);

private:
	OpenGLContext openGLContext;

	AmbiDataSet* pEditablePoints;
	AmbiDataSet* pDisplayOnlyPoints;
	ScopedPointer<Image> radarBackground;
	ScopedPointer<Image> infoImage;
	Rectangle<int> radarViewport;
	ZoomSettings* pZoomSettings;
	RadarMode radarMode;
	PointSelection* pPointSelection;
	ScopedPointer<RadarColors> radarColors;
	RadarOptions* pRadarOptions;
	CriticalSection radarBackgroundLock;
	CriticalSection infoLabelLock;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Radar2D)
};
