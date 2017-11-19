/*
  ==============================================================================

    PercussionVoice.h
    Created: 9 Nov 2016 9:28:37pm
    Author:  F.GK

  ==============================================================================
*/

#ifndef PERCUSSIONVOICE_H_INCLUDED
#define PERCUSSIONVOICE_H_INCLUDED
#include "Plate.h"

class PercussionVoice :  public SynthesiserVoice
{
public:
    PercussionVoice(float tension,float damping,float thickness,int material,float volume);
    ~PercussionVoice();
    
    bool canPlaySound(SynthesiserSound* sound) override;
    
    void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int /*currentPitchWheelPosition*/) override;
    
    void stopNote(float /*velocity*/, bool /*allowTailOff*/) override {};
    
    void pitchWheelMoved(int /*newValue*/) override {};
    
    void controllerMoved(int /*controllerNumber*/, int /*newValue*/) override {};
    
    void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override ;
    
    void setParameters();

    ScopedPointer<Plate> note;
    
private:
    double m_sampleRate;
    
    void mapMidiToSizes(int midi)
    {
        m_Lx = (LxMax-LxMin) * (midi-midiMin) / (midiMax - midiMin) + LxMin;
        m_Lx = LxMax - m_Lx + LxMin;
        m_Ly = m_Lx;
    }
    
    float m_Lx, m_Ly;
    float m_stiffness, m_damping, m_thickness, m_material, m_volume;

    const int midiMax {108};
    const int midiMin {21};
    const float LxMax {1.6f};
    const float LxMin {0.22f};
    

    
};


#endif  // PERCUSSIONVOICE_H_INCLUDED
