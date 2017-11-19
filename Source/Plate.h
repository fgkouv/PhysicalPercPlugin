/*==============================================================================

    Plate.h
    Created: 9 Nov 2016 9:29:05pm
    Author:  F.GK

  ============================================================================= */

#ifndef PLATE_H_INCLUDED
#define PLATE_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include <numeric>

class Plate
{
public:
    Plate  (double newSampleRate, float newLx, float newLy, float newThickness, float newStiffness, float newDamping, int newMaterialChoice);
    
    ~Plate ();
    
    void reset(double newSampleRate, float Lx, float Ly, float thickness, float stiffness, float newDamping, int materialChoice);
    
    void getPercussionNote(float* buffer,int numSamples);
    
    void renderToBuffer(float* outbuf, int samples, bool isDown);
    
    bool isAlive()
    {
        return (counter < totalDuration);
    }
    
private:
    void setVectorCoefficients();
    
    double sampleRate;
    float Lx, Ly,thickness, stiffness,damping, gamma;
    
    int materialChoice;
    int forcingDuration;
    int counter;
    int modes;
    int modesSquared;
    int totalDuration {44100};
    
    std::vector<float> nextModes;
    std::vector<float> spatial;
    
    float* currentModes;
    float* previousModes;
    float* currentCoeff;
    float* previousCoeff;
    float* forcing;
    float* forcingCoeff;
    float* modeVectorX;
    float* modeVectorY;
    float* currentDisplacement;
    float* previousDisplacement;
    float* calculationBuffer;
};

#endif  // PLATE_H_INCLUDED
