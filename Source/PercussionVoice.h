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
    
    PercussionVoice();
    ~PercussionVoice();
    
    bool canPlaySound(SynthesiserSound* sound) override;
    
    void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int /*currentPitchWheelPosition*/) override;
    
    void stopNote(float /*velocity*/, bool /*allowTailOff*/) override {};
    
    void pitchWheelMoved(int /*newValue*/) override {};
    
    void controllerMoved(int /*controllerNumber*/, int /*newValue*/) override {};
    
    void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override ;

    ScopedPointer<Plate> note;
    
private:
    
};


#endif  // PERCUSSIONVOICE_H_INCLUDED
