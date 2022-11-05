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
#include "Radar2D.h"
#include "GroupPointsSelectionComponent.h"

#define SHAPE_ID_CIRCLE 1

class MultiActivationDialog : public Component, ComboBox::Listener, Button::Listener, Slider::Listener, public ChangeListener
{
public:
    MultiActivationDialog(AmbiDataSet* pData, Point<float> newPosition, bool xyRadar) : pData(pData), newPosition(newPosition), xyRadar(xyRadar)
    {
        // copy to local data set
        localDataSet.reset(new AmbiSourceSet(&scalingInfo));
        for(int i = 0; i < pData->size(); i++)
            localDataSet->addNew(pData->get(i)->getId(), Point3D<double>(), pData->get(i)->getName(), pData->get(i)->getColor());
        
        labelPointCount.reset(new Label("PointCount"));
        labelPointCount->setText("# Points: ", dontSendNotification);
        addAndMakeVisible(labelPointCount.get());
        
        sliderPointCount.reset(new Slider("PointCount"));
        sliderPointCount->setRange(Range<double>(1, pData->size() - pData->getEnabledCount()), 1.0);
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
        comboShape->addListener(this);
        addAndMakeVisible(comboShape.get());
        
        labelRadius.reset(new Label("Radius"));
        labelRadius->setText("Radius:", dontSendNotification);
        addAndMakeVisible(labelRadius.get());

        sliderRadius.reset(new Slider("Radius"));
        sliderRadius->setRange(0.01, 100.0); // TODO
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
    }

    void buttonClicked(Button* b) override
    {
        if(b == btnSelectPoints.get())
        {
            CallOutBox::launchAsynchronously(std::make_unique<GroupPointsSelectionComponent>(localDataSet.get(), 0, this), btnSelectPoints->getScreenBounds(), nullptr);
        }
        else if(b == btn.get())
        {
           /*
            AmbiPoint* p = pData->get( combo-getSelectedId() -+ 1);
           p->setEnabled(true);
            p->setName(text->getText());
            p->getRawPoint()->setXYZ(newPosition.getX(), xyRadar ? newPosition.getY() : 0.0, xyRadar ? 0.0 : newPosition.getY());
             */
            findParentComponentOfClass<CallOutBox>()->exitModalState(0);
        }
    }
    
    void sliderValueChanged(Slider *slider) override
    {
       // modify point selection

       controlDimming();
    }

    void changeListenerCallback(ChangeBroadcaster *source) override
    {
        
    }
    
    void controlDimming()
    {
       
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

