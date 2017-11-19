/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Plate.h"
#include "PercussionVoice.h"

//==============================================================================
/**
*/
class PhysicalPercPluginAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    PhysicalPercPluginAudioProcessor();
    ~PhysicalPercPluginAudioProcessor();
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif
    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    //==============================================================================
    const String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;
    //==============================================
    //~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|
    void setThickness   (float newThickness)
    {
        thickness = newThickness;
        plate->reset(getSampleRate(), Lx, Ly, thickness, stiffness, damping, materialChoice);
        synth.addVoice (new PercussionVoice(stiffness, damping, thickness, materialChoice, volume));

    }
    
    void setDamping     (float newDamping)
    {
        damping = newDamping;
        plate->reset(getSampleRate(), Lx, Ly, thickness, stiffness, damping, materialChoice);
        synth.addVoice (new PercussionVoice(stiffness, damping, thickness, materialChoice, volume));
    }
    
    void setVolume      (float newVolume)
    {
        volume = newVolume;
        plate->reset(getSampleRate(), Lx, Ly, thickness, stiffness, damping, materialChoice);
        synth.addVoice (new PercussionVoice(stiffness, damping, thickness, materialChoice, volume));
    }
    
    void setStiffness   (float newStiffness)
    {
        stiffness = newStiffness;
        plate->reset(getSampleRate(), Lx, Ly, thickness, stiffness, damping, materialChoice);
        synth.addVoice (new PercussionVoice(stiffness, damping, thickness, materialChoice, volume));
    }
    
    void setMaterial    (int newMaterial)
    {
        materialChoice = newMaterial;
        plate->reset(getSampleRate(), Lx, Ly, thickness, stiffness, damping, materialChoice);
        synth.addVoice (new PercussionVoice(stiffness, damping, thickness, materialChoice, volume));
    }
    
    void setDimensions (int midiNote)
    {
        float midiMax = 108, midiMin = 21, Lx_max = 2.0, Lx_min = 0.2;
        
        Lx = (Lx_max-Lx_min) * (midiNote-midiMin) / (midiMax - midiMin) + Lx_min;
        Lx = Lx_max - Lx + Lx_min;
        Ly = Lx;
    }
    
    
    float dB2Amp (float dB)
    {
        return powf(10, 0.05*dB);
    }
    //~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|
    //==============================================
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;
    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
private:
    //==============================================================================
    int materialChoice  { 1 };
    
    float thickness     { 0.05f};
    float damping       { 0.0f };
    float volume        { 0.0f };
    float stiffness     { 0.0f };
    float Lx            { 1.0f };
    float Ly            { 1.0f };
    
    const int numVoices {1};
    MidiKeyboardState keyboardState;
    MidiMessageCollector midiCollector;
    AudioDeviceManager inputDeviceManager;
    AudioDeviceManager outputDeviceManager;
    Synthesiser synth;
    ScopedPointer<PercussionVoice> percussionVoice;
    AudioSourcePlayer audioSourcePlayer;
    
    struct percSound : public SynthesiserSound
    {
        percSound() {}
        bool appliesToNote   (int /*midiNoteNumber*/) override { return true; }
        bool appliesToChannel(int /*midiChannel*/)   override  { return true; }
    };
    
    ScopedPointer<Plate> plate;
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhysicalPercPluginAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
