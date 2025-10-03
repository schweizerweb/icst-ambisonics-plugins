#pragma once

#include <JuceHeader.h>

class StatusBarComponent : public juce::Component
{
public:
    StatusBarComponent(juce::Component& owner);
    ~StatusBarComponent() override;

    // Message interface
    void setMessage(const juce::AttributedString& message);
    void clearMessage();
    
    // Validation interface
    void setValidationState(bool isValid, const juce::String& details);
    
    // Customization
    void setBackgroundColour(juce::Colour colour) { backgroundColour = colour; repaint(); }
    void setTextColour(juce::Colour colour) { textColour = colour; repaint(); }
    void setBorderColour(juce::Colour colour) { borderColour = colour; repaint(); }

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::Component& owner;
    
    // Validation button
    std::unique_ptr<juce::DrawableButton> validationButton;
    
    // Pre-loaded and scaled icons (loaded once)
    std::unique_ptr<juce::Drawable> scaledOkIcon;
    std::unique_ptr<juce::Drawable> scaledErrorIcon;
    
    // Message display
    juce::AttributedString currentMessage;
    bool hasMessage = false;
    
    // Validation state
    bool isValid = true;
    juce::String validationDetails;
    
    // Callout box management
    std::unique_ptr<juce::CallOutBox> activeCallout;
    
    // Customization
    juce::Colour backgroundColour = juce::Colour(0xff252526);
    juce::Colour textColour = juce::Colours::lightgrey;
    juce::Colour borderColour = juce::Colours::grey.withAlpha(0.3f);
    
    void showValidationDetails();
    void loadAndScaleIcons();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StatusBarComponent)
};
