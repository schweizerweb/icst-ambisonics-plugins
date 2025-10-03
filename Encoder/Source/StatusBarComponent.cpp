#include "StatusBarComponent.h"

StatusBarComponent::StatusBarComponent(juce::Component& ownerRef)
    : owner(ownerRef)
{
    // Create validation button - use ImageRaw to remove borders
    validationButton = std::make_unique<juce::DrawableButton>("Validation", juce::DrawableButton::ImageRaw);
    
    // Remove all borders and backgrounds from the button
    validationButton->setColour(juce::DrawableButton::backgroundColourId, juce::Colours::transparentBlack);
    validationButton->setColour(juce::DrawableButton::backgroundOnColourId, juce::Colours::transparentBlack);
    
    // Load icons once during construction
    loadAndScaleIcons();
    
    // Set initial state
    setValidationState(true, "No validation performed yet");
    
    // Use lambda for button click
    validationButton->onClick = [this] {
        showValidationDetails();
    };
    
    addAndMakeVisible(validationButton.get());
}

StatusBarComponent::~StatusBarComponent()
{
    // Close any open callout
    if (activeCallout != nullptr)
    {
        activeCallout->dismiss();
    }
}

void StatusBarComponent::loadAndScaleIcons()
{
    const float iconSize = 20.0f; // Square icon (width = height)
    
    // Load and scale OK icon once
    if (BinaryData::ok_icon_svg != nullptr && BinaryData::ok_icon_svgSize > 0)
    {
        auto okIcon = juce::Drawable::createFromImageData(BinaryData::ok_icon_svg, BinaryData::ok_icon_svgSize);
        if (okIcon != nullptr)
        {
            okIcon->replaceColour(juce::Colours::black, juce::Colours::lightgreen);
            scaledOkIcon = okIcon->createCopy();
            scaledOkIcon->setTransformToFit(juce::Rectangle<float>(0, 0, iconSize, iconSize),
                                          juce::RectanglePlacement::centred);
        }
    }
    
    // Load and scale error icon once
    if (BinaryData::error_icon_svg != nullptr && BinaryData::error_icon_svgSize > 0)
    {
        auto errorIcon = juce::Drawable::createFromImageData(BinaryData::error_icon_svg, BinaryData::error_icon_svgSize);
        if (errorIcon != nullptr)
        {
            errorIcon->replaceColour(juce::Colours::black, juce::Colours::orangered);
            scaledErrorIcon = errorIcon->createCopy();
            scaledErrorIcon->setTransformToFit(juce::Rectangle<float>(0, 0, iconSize, iconSize),
                                             juce::RectanglePlacement::centred);
        }
    }
}

void StatusBarComponent::paint(juce::Graphics& g)
{
    // Draw background
    g.fillAll(backgroundColour);
    
    // Draw top border
    g.setColour(borderColour);
    g.drawLine(0.0f, 0.0f, (float)getWidth(), 0.0f, 1.0f);
    
    // Draw message if available - properly vertically centered
    if (hasMessage)
    {
        auto textArea = getLocalBounds();
        
        // Reserve space for icon on the left: 2px left + 20px icon + 5px right = 27px
        textArea.removeFromLeft(27);
        
        // Remove right padding
        textArea.removeFromRight(2);
        
        // Create a copy of the message and set its justification for vertical centering
        juce::AttributedString centeredMessage = currentMessage;
        centeredMessage.setJustification(juce::Justification::centredLeft);
        
        // Draw with vertical centering
        centeredMessage.draw(g, textArea.toFloat());
    }
    else
    {
        // Default status message - vertically centered
        g.setColour(textColour);
        g.setFont(juce::FontOptions(12.0f));
        
        auto textArea = getLocalBounds();
        
        // Reserve space for icon on the left
        textArea.removeFromLeft(27);
        
        // Remove right padding
        textArea.removeFromRight(2);
        
        g.drawText("Ready", textArea, juce::Justification::centredLeft);
    }
}

void StatusBarComponent::resized()
{
    auto area = getLocalBounds();
    
    // Icon on the left side with 2px left margin and 2px top/bottom margin
    int iconSize = 20; // Square icon
    int totalHeight = getHeight();
    int iconTop = (totalHeight - iconSize) / 2; // Center vertically with 2px margins
    
    // Position icon: 2px from left, vertically centered
    auto iconBounds = area.removeFromLeft(iconSize + 2); // 2px left margin
    iconBounds = iconBounds.withTrimmedLeft(2); // Apply the 2px left margin
    iconBounds = iconBounds.withY(iconTop).withHeight(iconSize); // Center vertically
    
    validationButton->setBounds(iconBounds);
    
    // Add 5px space between icon and message text
    area.removeFromLeft(5);
}

void StatusBarComponent::setMessage(const juce::AttributedString& message)
{
    currentMessage = message;
    hasMessage = true;
    repaint();
}

void StatusBarComponent::clearMessage()
{
    hasMessage = false;
    repaint();
}

void StatusBarComponent::setValidationState(bool valid, const juce::String& details)
{
    isValid = valid;
    validationDetails = details;
    
    // Update button icon using pre-scaled icons
    if (isValid && scaledOkIcon != nullptr)
    {
        validationButton->setImages(scaledOkIcon.get());
        validationButton->setTooltip("Validation passed");
    }
    else if (!isValid && scaledErrorIcon != nullptr)
    {
        validationButton->setImages(scaledErrorIcon.get());
        validationButton->setTooltip("Validation failed - Click for details");
    }
    else
    {
        // Fallback if icons failed to load
        validationButton->setButtonText(valid ? "OK" : "ERR");
        validationButton->setTooltip(valid ? "Validation passed" : "Validation failed - Click for details");
    }
    
    repaint();
}

void StatusBarComponent::showValidationDetails()
{
    // Only show details if there are validation issues
    if (isValid) return;
    
    // Close existing callout
    if (activeCallout != nullptr)
    {
        activeCallout->dismiss();
        activeCallout.reset();
        return;
    }
    
    // Create a simple component
    class ValidationDetailsComponent : public juce::Component
    {
    public:
        ValidationDetailsComponent(const juce::String& details)
        {
            // Create a formatted message
            juce::String message;
            message << "Timeline Validation - Status: FAILED\n";
            message << "Last checked: " << juce::Time::getCurrentTime().toString(true, true, false, true) << "\n\n";
            message << details;
            
            editor = std::make_unique<juce::TextEditor>();
            editor->setMultiLine(true);
            editor->setReadOnly(true);
            editor->setCaretVisible(false);
            editor->setScrollbarsShown(true);
            editor->setText(message);
            
            // Set a reasonable size - JUCE will handle the rest
            setSize(400, 300);
            editor->setBounds(getLocalBounds());
            
            addAndMakeVisible(editor.get());
        }
        
        void resized() override
        {
            editor->setBounds(getLocalBounds());
        }
        
    private:
        std::unique_ptr<juce::TextEditor> editor;
    };
    
    // Create and show the callout
    auto content = std::make_unique<ValidationDetailsComponent>(validationDetails);
    juce::CallOutBox::launchAsynchronously(std::move(content),
                                         validationButton->getScreenBounds(),
                                         &owner);
}
