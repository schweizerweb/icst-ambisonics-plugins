/*
  ==============================================================================

    Radar2D.h
    Created: 22 Jul 2017 1:43:26pm
    Author:  chris

  ==============================================================================
*/

#pragma once

#include "../Decoder/JuceLibraryCode/JuceHeader.h"
#include "AmbiPoint.h"

//==============================================================================
/*
*/
class Radar2D    : public Component
{
public:
	enum RadarMode { XY, ZY };

	Radar2D(RadarMode mode, Array<AmbiPoint>* pAmbiPointArray, float radius);
    ~Radar2D();

	Point<double> getProjectedPoint(Point3D<double>* point3_d) const;
	Point<float> getAbsoluteScreenPoint(Point<float> point) const;
	float getPointSize() const;
    void paint (Graphics&) override;
	void createRadarBackground();
	
	void resized() override;
	void mouseExit(const MouseEvent& e) override;
	double getMinPointSelectionDist() const;
	void mouseDown(const MouseEvent& e) override;
	void mouseDrag(const MouseEvent& e) override;
	void mouseUp(const MouseEvent& e) override;
	void mouseDoubleClick(const MouseEvent& e) override;

private:
	Point<float> getRelativeScreenPoint(Point<float> valuePoint) const;
	Point<float> getValuePointFromRelativeScreenPoint(Point<float> relativeScreenPoint) const;
	float getValueToScreenRatio() const;

private:
	Array<AmbiPoint>* pAmbiPoints;
	Image radarBackground;
	Rectangle<int> radarViewport;
	float initialRadius;
	RadarMode radarMode;
	Point<float> currentViewCenter;
	float currentViewRadius;
	String infoString;
	AmbiPoint* selectedPoint;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Radar2D)
};
