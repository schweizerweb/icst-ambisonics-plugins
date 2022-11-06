/*
  =============================================================================

    MultiActivationDialog.
    Created: 5 Nov 2022 9:26:39a
    Author:  Christian Schweize

  =============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "AmbiSourceSet.h"
#include "ScalingInfo.h"
#include "GroupPointsSelectionComponent.h"

enum ShapeId { SHAPE_ID_NONE, SHAPE_ID_CIRCLE, SHAPE_ID_SQUARE, SHAPE_ID_TRIANGLE, SHAPE_ID_STAR };

class MultiActivationDialog : public Component, ComboBox::Listener, Button::Listener, Slider::Listener, public ChangeListener
{
public:
    MultiActivationDialog(AmbiDataSet* pData, Point<float> newPosition, ScalingInfo* pScaling, bool xyRadar) : pData(pData), newPosition(newPosition), xyRadar(xyRadar)
    {
        // copy to local data set
        localDataSet.reset(new AmbiSourceSet(&scalingInfo));
        localDataSet->setGroupModeFlag(true);
        localDataSet->addGroup(Uuid().toString(), Vector3D<double>(newPosition.x, newPosition.y, 0), "Temp", Colours::red);
        for(int i = 0; i < pData->size(); i++)
        {
            auto p = pData->get(i)->getRawPoint();
            Point3D<double> np(p->getX(), p->getY(), p->getZ());
            if(!pData->get(i)->getEnabled())
            {
                // use the ID to identify the original index
                localDataSet->addNew(String(i), np, pData->get(i)->getName(), pData->get(i)->getColor());
            }
        }
        
        labelPointCount.reset(new Label("PointCount"));
        labelPointCount->setText("# Points: ", dontSendNotification);
        addAndMakeVisible(labelPointCount.get());
        
        sliderPointCount.reset(new Slider("PointCount"));
        int max = pData->size() - pData->getEnabledCount();
        sliderPointCount->setRange(Range<double>(0, max), 1.0);
        sliderPointCount->setValue(max, sendNotificationAsync);
        sliderPointCount->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxRight, true, 40, 24);
        sliderPointCount->addListener(this);
        addAndMakeVisible(sliderPointCount.get());
        
        btnSelectPoints.reset(new TextButton("Select"));
        btnSelectPoints->addListener(this);
        addAndMakeVisible(btnSelectPoints.get());
        
        labelShape.reset(new Label("Shape"));
        labelShape->setText("Shape: ", dontSendNotification);
        addAndMakeVisible(labelShape.get());

        comboShape.reset(new ComboBox("Shape"));
        comboShape->addItem("Circle", SHAPE_ID_CIRCLE);
        comboShape->addItem("Square", SHAPE_ID_SQUARE);
        comboShape->addItem("Triangle", SHAPE_ID_TRIANGLE);
        comboShape->addItem("Star", SHAPE_ID_STAR);
        comboShape->addListener(this);
        addAndMakeVisible(comboShape.get());
        
        labelRadius.reset(new Label("Radius"));
        labelRadius->setText("Radius:", dontSendNotification);
        addAndMakeVisible(labelRadius.get());

        sliderRadius.reset(new Slider("Radius"));
        sliderRadius->setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
        sliderRadius->setRange(0.01, pScaling->CartesianMax());
        sliderRadius->setValue(pScaling->CartesianMax() / 5);
        sliderRadius->setPopupDisplayEnabled (true, false, this);
        sliderRadius->addListener(this);
        addAndMakeVisible(sliderRadius.get());

        zoomSettings.reset(new ZoomSettings(&scalingInfo));
        RadarOptions radarOptions;
        radarOptions.zoomSettings = zoomSettings.get();
        
        btn.reset(new TextButton("btn"));
        btn->setButtonText("Activate");
        btn->addListener(this);
        addAndMakeVisible(btn.get());

        comboShape->setSelectedItemIndex(0);

        setSize(350, 120);
    }

    ~MultiActivationDialog() override
    {
        labelPointCount = nullptr;
        sliderPointCount = nullptr;
        btnSelectPoints = nullptr;
        labelShape = nullptr;
        comboShape = nullptr;
        labelRadius = nullptr;
        sliderRadius = nullptr;
        btn = nullptr;
    }

private:
    void resized() override
    {
        labelPointCount->setBounds(0, 0, getWidth() / 3, 24);
        sliderPointCount->setBounds(labelPointCount->getRight(), 0, getWidth() - labelPointCount->getRight() - 70, 24);
        btnSelectPoints->setBounds(getWidth()-70, 0, 70, 24);

        labelShape->setBounds(0, 30, getWidth() / 3, 24);
        comboShape->setBounds(labelShape->getRight(), 30, getWidth() - labelShape->getRight(), 24);

        labelRadius->setBounds(0, 60, getWidth() / 3, 24);
        sliderRadius->setBounds(labelRadius->getRight(), 60, getWidth() - labelRadius->getRight(), 24);
        
        btn->setBounds(0, getHeight()-30, getWidth(), 24);
    }

    void comboBoxChanged(ComboBox*) override
    {
        // prepare preview
        controlDimming();
        buildShape();
    }

    void buttonClicked(Button* b) override
    {
        if(b == btnSelectPoints.get())
        {
            CallOutBox::launchAsynchronously(std::make_unique<GroupPointsSelectionComponent>(localDataSet.get(), 0, this), btnSelectPoints->getScreenBounds(), nullptr);
        }
        else if(b == btn.get())
        {
            if(buildShape() && applyDataSet())
                findParentComponentOfClass<CallOutBox>()->exitModalState(0);
        }
    }
    
    void sliderValueChanged(Slider *slider) override
    {
        // modify point selection
        if(slider == sliderPointCount.get())
        {
            int count = sliderPointCount->getValue();
            for(int i = 0; i < localDataSet->size(); i++)
            {
                if(count > 0)
                {
                    localDataSet->addPointToGroup(0, i);
                    count--;
                }
                else
                {
                    localDataSet->removePointFromGroup(0, i);
                }
            }
        }
        else if(slider == sliderRadius.get())
        {
            
        }
        
        controlDimming();
        buildShape();
    }

    void changeListenerCallback(ChangeBroadcaster *source) override
    {
        int count = 0;
        for(int i = 0; i < localDataSet->size(); i++)
        {
            if(localDataSet->get(i)->getGroup() != nullptr)
                count++;
        }
        
        sliderPointCount->setValue(count, dontSendNotification);
        
        controlDimming();
        buildShape();
    }
    
    void controlDimming()
    {
        btn->setEnabled(sliderPointCount->getValue()>0);
        sliderRadius->setSliderStyle(comboShape->getSelectedId() == SHAPE_ID_STAR ? Slider::TwoValueHorizontal : Slider::LinearHorizontal);
    }
    
private:
    bool applyDataSet()
    {
        if(!localDataSet->getGroup(0)->groupPointCount())
            return false;
        
        // first add a group point
        Vector3D<double> gp = xyRadar
            ? Vector3D<double>(newPosition.x, newPosition.y, 0)
            : Vector3D<double>(newPosition.x, 0, newPosition.y);
        int newGroupIndex = pData->addGroup(Uuid().toString(), gp, "Shape", Colours::red);
        if(newGroupIndex < 0)
            return false;
        
        for(int i = 0; i < localDataSet->size(); i++)
        {
            auto p = localDataSet->get(i);
            if(p->getGroup() == localDataSet->getGroup(0))
            {
                int pointIndex = p->getId().getIntValue();
                pData->addPointToGroup(newGroupIndex, pointIndex);
                pData->get(pointIndex)->setEnabled(true);
                Vector3D<double> pp = xyRadar
                    ? Vector3D<double>(p->getRawPoint()->getX(), p->getRawPoint()->getY(), 0)
                    : Vector3D<double>(p->getRawPoint()->getX(), 0, p->getRawPoint()->getY());
                pData->get(pointIndex)->getRawPoint()->setXYZ(pp.x, pp.y, pp.z);
            }
        }
        return true;
    }
    
    bool buildShape()
    {
        double radius = sliderRadius->getSliderStyle() == Slider::TwoValueHorizontal ? sliderRadius->getMaxValue() : sliderRadius->getValue();
        ShapeId shapeId = (ShapeId)comboShape->getSelectedId();
        int pointCount = localDataSet->getGroup(0)->groupPointCount();
        
        Path p;
        switch(shapeId)
        {
            
            case SHAPE_ID_NONE:
                return false;
            case SHAPE_ID_CIRCLE:
                p.addEllipse(-radius, -radius, 2*radius, 2*radius);
                break;
            case SHAPE_ID_SQUARE:
                p.addRectangle(-radius, -radius, 2*radius, 2*radius);
                break;
            case SHAPE_ID_TRIANGLE:
                p.addTriangle(0.0f, radius, cos(PI/6)*radius, -sin(PI/6)*radius, -cos(PI/6)*radius, -sin(PI/6)*radius);
                break;
            case SHAPE_ID_STAR:
                double innerRadius = sliderRadius->getMinValue();
                p.addStar(Point<float>(0.0f, 0.0f), pointCount/2, innerRadius, radius);
                break;
        }
        
        float totalLength = p.getLength();
        int groupPointIndex = 0;
        for(int i = 0; i < localDataSet->size(); i++)
        {
            if(localDataSet->get(i)->getGroup() == localDataSet->getGroup(0))
            {
                auto pt = p.getPointAlongPath(totalLength / pointCount * groupPointIndex);
                localDataSet->get(i)->getRawPoint()->setXY(pt.getX(), pt.getY());
                groupPointIndex++;
            }
        }
        
        return true;
    }
    
private:
    std::unique_ptr<Label> labelPointCount;
    std::unique_ptr<Slider> sliderPointCount;
    std::unique_ptr<Button> btnSelectPoints;
    
    std::unique_ptr<Label> labelShape;
    std::unique_ptr<ComboBox> comboShape;

    std::unique_ptr<Label> labelRadius;
    std::unique_ptr<Slider> sliderRadius;
    
    std::unique_ptr<TextButton> btn;
   
    
    AmbiDataSet* pData;
    Point<float> newPosition;
    bool xyRadar;
    std::unique_ptr<ZoomSettings> zoomSettings;
    ScalingInfo scalingInfo;
    std::unique_ptr<AmbiDataSet> localDataSet;
};

