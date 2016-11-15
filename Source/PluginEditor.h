/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class PhysicalPercPluginAudioProcessorEditor  : public AudioProcessorEditor , public Slider::Listener , public ComboBox::Listener
{
public:
    PhysicalPercPluginAudioProcessorEditor (PhysicalPercPluginAudioProcessor&);
    ~PhysicalPercPluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged (Slider *slider) override;
    void comboBoxChanged    (ComboBox *box) override ;

private:

    PhysicalPercPluginAudioProcessor& processor;
    //MidiKeyboardComponent keyboard;
    MidiKeyboardState keyboardState;
    
    Slider volumeSlider;
    Slider dampingSlider;
    Slider thickSlider;
    Slider stiffnessSlider;
    
    Label volumeLabel;
    Label dampingLabel;
    Label thickLabel;
    Label stiffnessLabel;
    
    float volume,stiffness,thickness,damping;
    
    ComboBox materials;
   
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhysicalPercPluginAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
