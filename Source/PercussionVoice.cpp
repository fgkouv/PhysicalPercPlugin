/*
  ==============================================================================

    PercussionVoice.cpp
    Created: 9 Nov 2016 9:28:45pm
    Author:  F.GK

  ==============================================================================
*/

#include "PercussionVoice.h"
PercussionVoice::PercussionVoice()
{
}


PercussionVoice::~PercussionVoice()
{

}

bool PercussionVoice::canPlaySound(SynthesiserSound* sound)
{
    return sound != nullptr;
}


void PercussionVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int /*currentPitchWheelPosition*/)
{
   //note = new PERC(midiNoteNumber, velocity, (float)getSampleRate(),Tens,Damp,Thik,Mtrl,Vol);
    std::cout << "note = " << midiNoteNumber << std::endl;
}



void PercussionVoice::renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    
 //   float* buf = (float *)outputBuffer.getWritePointer(0);
 //   if (note != nullptr) {
//        note->renderToBuffer(&buf[startSample], numSamples, isKeyDown() /*|| isSustainPedalDown()*/);
  //      if (!note->is_alive()) {
    //        clearCurrentNote();
  //          note = nullptr;}
 //   }

}
