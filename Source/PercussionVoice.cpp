/*
  ==============================================================================

    PercussionVoice.cpp
    Created: 9 Nov 2016 9:28:45pm
    Author:  F.GK

  ==============================================================================
*/
#include "PercussionVoice.h"
PercussionVoice::PercussionVoice(float stiffness,float damping,float thickness,int material,float volume)
{
    //note = new Plate();
    
    m_stiffness = stiffness;
    m_damping = damping;
    m_thickness = thickness;
    m_material = material;
    m_volume = volume;
}


PercussionVoice::~PercussionVoice()
{

}

bool PercussionVoice::canPlaySound(SynthesiserSound* sound)
{
    return sound != nullptr;
}


void PercussionVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound* , int /*currentPitchWheelPosition*/)
{
    m_sampleRate = getSampleRate();
    
    mapMidiToSizes(midiNoteNumber);
    note = new Plate(m_sampleRate, m_Lx, m_Ly, m_thickness, m_stiffness, m_damping, m_material);
}



void PercussionVoice::renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    const int leftChanIndex = 0;
    const int rightChanIndex = 1;
    
    float* buf = (float *)outputBuffer.getWritePointer(leftChanIndex);

    if (note != nullptr)
    {
        note->renderToBuffer(&buf[startSample], numSamples, isKeyDown() /*|| isSustainPedalDown()*/);
        
        FloatVectorOperations::copy(outputBuffer.getWritePointer(rightChanIndex), buf, numSamples);
        
        if (!note->isAlive())
        {
            clearCurrentNote();
            note = nullptr;
        }
    }

}
