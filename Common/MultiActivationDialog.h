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
#include "AmbiSourceSet.h"
#include "ScalingInfo.h"
#include "GroupPointsSelectionComponent.h"
#include "ColorSelectionComponent.h"
#include "ColorDefinition.h"

enum ShapeId { SHAPE_ID_NONE, SHAPE_ID_CIRCLE, SHAPE_ID_SQUARE, SHAPE_ID_TRIANGLE, SHAPE_ID_STAR, SHAPE_ID_LINE, SHAPE_ID_ARC };

class MultiActivationDialog : public Component, ComboBox::Listener, Button::Listener, Slider::Listener, public ChangeListener
{
public:
    MultiActivationDialog(AmbiDataSet* _pData, Point<float> _newPosition, RadarOptions* _pRadarOptions, bool _xyRadar) : pData(_pData), newPosition(_newPosition), xyRadar(_xyRadar)
    {
        // copy to local data set
        color = COLOR_DEFINITION_GROUP_DEFAULT;
        defaultRadius = _pRadarOptions->scalingInfo->IsInfinite() ? 5.0 : _pRadarOptions->scalingInfo->CartesianMax() / 5.0;
        localDataSet.reset(new AmbiSourceSet(&scalingInfo));
        localDataSet->addGroup(Uuid().toString(), Vector3D<double>(newPosition.x, newPosition.y, 0), "Temp", color);
        for(int i = 0; i < pData->size() && i < _pRadarOptions->maxNumberEditablePoints; i++)
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
        int max = _pRadarOptions->maxNumberEditablePoints - pData->getEnabledCount();
        sliderPointCount->setRange(Range<double>(0, max), 1.0);
        sliderPointCount->setValue(max, sendNotificationAsync);
        sliderPointCount->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxRight, false, 40, 24);
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
        comboShape->addItem("Line", SHAPE_ID_LINE);
        comboShape->addItem("Arc", SHAPE_ID_ARC);
        comboShape->addListener(this);
        addAndMakeVisible(comboShape.get());
        
        labelRadius.reset(new Label("Radius"));
        labelRadius->setText("Radius:", dontSendNotification);
        addAndMakeVisible(labelRadius.get());

        sliderRadius.reset(new Slider("Radius"));
        sliderRadius->setTextBoxStyle (Slider::TextEntryBoxPosition::TextBoxRight, false, 50, 24);
        sliderRadius->setRange(0.01, _pRadarOptions->scalingInfo->CartesianMax());
        sliderRadius->setValue(defaultRadius);
        sliderRadius->setNumDecimalPlacesToDisplay(2);
        sliderRadius->setPopupDisplayEnabled (true, false, this);
        sliderRadius->addListener(this);
        addAndMakeVisible(sliderRadius.get());
        
        labelAngle.reset(new Label("Angle"));
        labelAngle->setText("Angle:", dontSendNotification);
        addAndMakeVisible(labelAngle.get());

        sliderAngle.reset(new Slider("Angle"));
        sliderAngle->setTextBoxStyle (Slider::TextEntryBoxPosition::NoTextBox, false, 50, 24);
        sliderAngle->setRange(-360.0, 360.0);
        sliderAngle->setSliderStyle(Slider::TwoValueHorizontal); 
        sliderAngle->setMinValue(-90);
        sliderAngle->setMaxValue(90);
        sliderAngle->setNumDecimalPlacesToDisplay(0);
        sliderAngle->setPopupDisplayEnabled (true, false, this);
        sliderAngle->addListener(this);
        addAndMakeVisible(sliderAngle.get());
        
        labelGroupName.reset(new Label("GroupName"));
        labelGroupName->setText("Group Name", dontSendNotification);
        addAndMakeVisible(labelGroupName.get());
        
        textGroupName.reset(new TextEditor("GroupName"));
        textGroupName->setText("Shape");
        addAndMakeVisible(textGroupName.get());
        
        toggleApplyName.reset(new ToggleButton("ApplyName"));
        toggleApplyName->setButtonText("Apply Name to Sources");
        toggleApplyName->setToggleState(false, dontSendNotification);
        addAndMakeVisible(toggleApplyName.get());
        
        labelColor.reset(new Label("Color"));
        labelColor->setText("Color", dontSendNotification);
        addAndMakeVisible(labelColor.get());
        
        btnColor.reset(new TextButton("Color"));
        btnColor->setButtonText("");
        btnColor->setColour(TextButton::ColourIds::buttonColourId, color);
        btnColor->addListener(this);
        addAndMakeVisible(btnColor.get());
        
        btnApply.reset(new TextButton("btn"));
        btnApply->setButtonText("Activate");
        btnApply->addListener(this);
        addAndMakeVisible(btnApply.get());

        comboShape->setSelectedItemIndex(0);

        setSize(350, 250);
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
        labelAngle = nullptr;
        sliderAngle = nullptr;
        labelGroupName = nullptr;
        textGroupName = nullptr;
        toggleApplyName = nullptr;
        labelColor = nullptr;
        btnColor = nullptr;
        btnApply = nullptr;
    }

private:
    void resized() override
    {
        int controlHeight = 24;
        int y = 0;
        int border = 6;

        labelPointCount->setBounds(0, y, getWidth() / 3, controlHeight);
        sliderPointCount->setBounds(labelPointCount->getRight(), labelPointCount->getY(), getWidth() - labelPointCount->getRight() - 70, controlHeight);
        btnSelectPoints->setBounds(getWidth()-70, labelPointCount->getY(), 70, controlHeight);
        y += controlHeight + border;

        labelShape->setBounds(0, y, getWidth() / 3, controlHeight);
        comboShape->setBounds(labelShape->getRight(), labelShape->getY(), getWidth() - labelShape->getRight(), controlHeight);
        y += controlHeight + border;

        labelRadius->setBounds(0, y, getWidth() / 3, controlHeight);
        sliderRadius->setBounds(labelRadius->getRight(), labelRadius->getY(), getWidth() - labelRadius->getRight(), controlHeight);
        y += controlHeight + border;

        labelAngle->setBounds(0, y, getWidth() / 3, controlHeight);
        sliderAngle->setBounds(labelAngle->getRight(), labelAngle->getY(), getWidth() - labelAngle->getRight(), controlHeight);
        y += controlHeight + border;

        labelGroupName->setBounds(0, y, getWidth() / 3, controlHeight);
        textGroupName->setBounds(labelGroupName->getRight(), labelGroupName->getY(), getWidth() - labelGroupName->getRight(), controlHeight);
        toggleApplyName->setBounds(labelGroupName->getRight(), labelGroupName->getY()+controlHeight, getWidth() - labelGroupName->getRight(), controlHeight);
        y += 2 * controlHeight + border;

        labelColor->setBounds(0, y, getWidth() / 3, controlHeight);
        btnColor->setBounds(labelColor->getRight(), labelColor->getY(), getWidth() - labelColor->getRight(), jmin(controlHeight, getHeight() - (y + controlHeight + border)));
        
        btnApply->setBounds(0, getHeight()-controlHeight-border, getWidth(), controlHeight);
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
        else if(b == btnApply.get())
        {
            if(buildShape() && applyDataSet())
                findParentComponentOfClass<CallOutBox>()->exitModalState(0);
        }
        else if(b == btnColor.get())
        {
            CallOutBox::launchAsynchronously(std::make_unique<ColorSelectionComponent>(color, this, nullptr, false), getScreenBounds(), nullptr);
        }
    }
    
    void sliderValueChanged(Slider *slider) override
    {
        // modify point selection
        if(slider == sliderPointCount.get())
        {
            int count = (int)sliderPointCount->getValue();
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
            // nothing to do
        }
        
        controlDimming();
        buildShape();
    }

    void changeListenerCallback(ChangeBroadcaster *source) override
    {
        ColourSelector* selector = dynamic_cast<ColourSelector*> (source);
        if (selector != nullptr)
        {
            color = selector->getCurrentColour();
            btnColor->setColour(TextButton::ColourIds::buttonColourId, color);
        }
        else
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
    }
    
    void controlDimming()
    {
        btnApply->setEnabled(sliderPointCount->getValue()>0);
        
        if(comboShape->getSelectedId() == SHAPE_ID_STAR && sliderRadius->getSliderStyle() != Slider::TwoValueHorizontal)
        {
            defaultRadius = sliderRadius->getValue();
            sliderRadius->setSliderStyle(Slider::TwoValueHorizontal);
            sliderRadius->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 50, 24);
            sliderRadius->setMaxValue(defaultRadius, dontSendNotification);
            sliderRadius->setMinValue(defaultRadius * 0.6, dontSendNotification);
        }
        else if(comboShape->getSelectedId() != SHAPE_ID_STAR && sliderRadius->getSliderStyle() != Slider::LinearHorizontal)
        {
            defaultRadius = sliderRadius->getMaxValue();
            sliderRadius->setSliderStyle(Slider::LinearHorizontal);
            sliderRadius->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxRight, false, 50, 24);
            sliderRadius->setValue(defaultRadius);
        }

        sliderAngle->setEnabled(comboShape->getSelectedId() == SHAPE_ID_ARC);
    }
    
private:
    bool applyDataSet()
    {
        if(!localDataSet->getGroup(0)->groupPointCount())
            return false;
        
        // first add a group point
        String groupName = textGroupName->getText();
        Vector3D<double> gp = xyRadar
            ? Vector3D<double>(newPosition.x, newPosition.y, 0)
            : Vector3D<double>(newPosition.x, 0, newPosition.y);
        int newGroupIndex = pData->addGroup(Uuid().toString(), gp, groupName, color);
        if(newGroupIndex < 0)
            return false;
        
        pData->getGroup(newGroupIndex)->setName(groupName);
        pData->getGroup(newGroupIndex)->setColor(color);
        for(int i = 0; i < localDataSet->size(); i++)
        {
            auto p = localDataSet->get(i);
            if(p->getGroup() == localDataSet->getGroup(0))
            {
                int pointIndex = p->getId().getIntValue();
                pData->addPointToGroup(newGroupIndex, pointIndex);
                pData->get(pointIndex)->setEnabled(true);
                auto ptToCopy = pData->getGroupModeFlag() ? p->getVector3D() : localDataSet->getAbsSourcePoint(i);
                Vector3D<double> pp = xyRadar
                    ? Vector3D<double>(ptToCopy.x, ptToCopy.y, 0)
                    : Vector3D<double>(ptToCopy.x, 0, ptToCopy.y);
                pData->get(pointIndex)->getRawPoint()->setXYZ(pp.x, pp.y, pp.z);
                pData->get(pointIndex)->setColor(color);
                if(toggleApplyName->getToggleState())
                {
                    pData->get(pointIndex)->setName(groupName + "_" + String(i + 1));
                }
                else
                {
                    pData->get(pointIndex)->setName(String(pointIndex + 1));
                }
            }
        }
        return true;
    }
    
    bool buildShape()
    {
        float radius = float(sliderRadius->getSliderStyle() == Slider::TwoValueHorizontal ? sliderRadius->getMaxValue() : sliderRadius->getValue());
        ShapeId shapeId = (ShapeId)comboShape->getSelectedId();
        bool isClosed = true;
        int pointCount = localDataSet->getGroup(0)->groupPointCount();
        if (pointCount < 1)
        {
            return false;
        }

        
        Path p;
        switch(shapeId)
        {
            
            case SHAPE_ID_NONE:
                return false;
            case SHAPE_ID_CIRCLE:
                {   
                    float startAngle = (pointCount % 2 == 0 ? (float)(-PI/(double)pointCount) : 0.0f);
                    p.startNewSubPath(radius*sinf(startAngle), radius*cosf(startAngle));
                    for(int i = 1; i < pointCount+1; i++)
                    {
                        float angle = startAngle + (float)((double)i*2.0*PI/(double)pointCount);
                        p.lineTo(radius*sinf(angle), radius*cosf(angle));
                    }
                }
                break;
            case SHAPE_ID_SQUARE:
                p.addRectangle(-radius, -radius, 2*radius, 2*radius);
                break;
            case SHAPE_ID_TRIANGLE:
                p.addTriangle(0.0f, radius, (float)cos(PI/6.0)*radius, (float)-sin(PI/6.0)*radius, (float)-cos(PI/6.0)*radius, (float)-sin(PI/6.0)*radius);
                break;
            case SHAPE_ID_STAR:
                p.addStar(Point<float>(0.0f, 0.0f), pointCount/2, float(sliderRadius->getMinValue()), radius);
                break;
            case SHAPE_ID_LINE:
                {
                    p.startNewSubPath(-radius, 0.0f);
                    p.lineTo(radius, 0.0f);
                    isClosed = false;
                }
                break;
            case SHAPE_ID_ARC:
                {
                    float startAngle = (float)Constants::GradToRad(sliderAngle->getMinValue());
                    float totalAngle = (float)Constants::GradToRad(sliderAngle->getMaxValue() - sliderAngle->getMinValue());
                    p.startNewSubPath(radius * sinf(startAngle), radius * cosf(startAngle));
                    for (int i = 1; i < pointCount + 1; i++)
                    {
                        float angle = startAngle + (float)((double)i * totalAngle / (double)pointCount);
                        p.lineTo(radius * sinf(angle), radius * cosf(angle));
                    }
                    isClosed = false;
                }
                break;
        }
        
        float totalLength = p.getLength();
        int groupPointIndex = 0;
        for(int i = 0; i < localDataSet->size(); i++)
        {
            if(localDataSet->get(i)->getGroup() == localDataSet->getGroup(0))
            {
                auto pt = p.getPointAlongPath(totalLength / float(isClosed ? pointCount : pointCount - 1) * float(groupPointIndex));
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
    
    std::unique_ptr<Label> labelAngle;
    std::unique_ptr<Slider> sliderAngle;
    
    std::unique_ptr<Label> labelGroupName;
    std::unique_ptr<TextEditor> textGroupName;
    std::unique_ptr<ToggleButton> toggleApplyName;
    
    std::unique_ptr<Label> labelColor;
    std::unique_ptr<TextButton> btnColor;
    
    std::unique_ptr<TextButton> btnApply;
   
    
    AmbiDataSet* pData;
    Point<float> newPosition;
    bool xyRadar;
    ScalingInfo scalingInfo;
    std::unique_ptr<AmbiDataSet> localDataSet;
    double defaultRadius;
    Colour color;
};
