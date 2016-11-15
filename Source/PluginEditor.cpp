/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
PhysicalPercPluginAudioProcessorEditor::PhysicalPercPluginAudioProcessorEditor (PhysicalPercPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p) //, keyboard(keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    //setSize(900, 300); // with the keyboard
    // midi keyboard
    /*
    addAndMakeVisible (keyboard);
    keyboard.setAvailableRange(31, 110);
     */
    
    setSize(900,200);
    // volume slider
    addAndMakeVisible (volumeSlider);
    addAndMakeVisible (volumeLabel);
    volumeLabel.setText ("master volume", dontSendNotification);
    volumeLabel.attachToComponent (&volumeSlider, false);
    volumeSlider.setRange (-36.0, 36.0,0.1);
    volumeSlider.setValue(0.0);
    volumeSlider.setSliderStyle(juce::Slider::LinearVertical);
    volumeSlider.addListener (this);
    
    addAndMakeVisible (dampingSlider);
    addAndMakeVisible (dampingLabel);
    dampingLabel.setText ("Damping", dontSendNotification);
    dampingLabel.attachToComponent (&dampingSlider, true);
    dampingSlider.setRange (0.0, 0.5);
    dampingSlider.setComponentID ("damping");
    dampingSlider.addListener (this);
    
    addAndMakeVisible (stiffnessSlider);
    addAndMakeVisible (stiffnessLabel);
    stiffnessLabel.setText ("Stiffness", dontSendNotification);
    stiffnessLabel.attachToComponent (&stiffnessSlider, true);
    stiffnessSlider.setRange (0.0, 1.0);
    stiffnessSlider.setComponentID ("stiffness");  // to recognise your slider in the callback
    stiffnessSlider.addListener (this);
    
    addAndMakeVisible (thickSlider);
    addAndMakeVisible (thickLabel);
    thickLabel.setText ("Thickness", dontSendNotification);
    thickLabel.attachToComponent (&thickSlider, true);
    thickSlider.setRange (0.05, 0.1);
    thickSlider.setComponentID ("thickness");  // to recognise your slider in the callback
    thickSlider.addListener (this);
    
    // materials list
    addAndMakeVisible( materials);
    materials.addItem("Bright metallic",1);
    materials.addItem("Dark woody",2);
    materials.setText("Select plate 'material'");
    materials.addListener(this);
}

PhysicalPercPluginAudioProcessorEditor::~PhysicalPercPluginAudioProcessorEditor()
{
}

//==============================================================================
void PhysicalPercPluginAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll(Colours::slategrey);
    g.setFont(18.0f);
    g.drawText("Press on the keys to strike a plate!", 50, 90, 400, 120, Justification::centred, true);
    g.setFont(11.0f);
    g.drawText("lower octaves~bigger plates // higher octaves~smaller plates", 0, 110, 500, 120, Justification::centred, true);
}

void PhysicalPercPluginAudioProcessorEditor::resized()
{
    const int sliderLeft = 120;
    volumeSlider.setBounds(750,40,getWidth()/3 - sliderLeft - 50,120);
    dampingSlider.setBounds (sliderLeft, 40, getWidth()/3 - sliderLeft - 10, 40);
    stiffnessSlider.setBounds (sliderLeft, 60, getWidth()/3 - sliderLeft - 10, 60);
    thickSlider.setBounds (4*sliderLeft, 40, getWidth()/3 + sliderLeft - 300, 40);
    materials.setBounds (4*sliderLeft, 90, 1.5*sliderLeft, 50);
    
    /*
    int w = (int)keyboard.getKeyWidth() * (7 * 7 + 3); int h = 80;
    keyboard.setSize(w, h);
    keyboard.setCentrePosition(getWidth() / 2, getHeight() - getHeight() / 6);
     */
    
}

void PhysicalPercPluginAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &volumeSlider)
        processor.setVolume(slider->getValue());
    if (slider == &thickSlider)
        processor.setThickness(slider->getValue());
    if (slider == &dampingSlider)
        processor.setDamping(slider->getValue());
    if (slider == &stiffnessSlider)
        processor.setStiffness(slider->getValue());
}

void PhysicalPercPluginAudioProcessorEditor::comboBoxChanged(ComboBox* box)
{
    processor.setMaterial(box->getSelectedId());
}