/*
  ==============================================================================

    Plate.cpp
    Created: 9 Nov 2016 9:29:09pm
    Author:  F.GK

  ==============================================================================
*/

#include "Plate.h"
#define MAX_MODES 50
#define MAX_MODES_SQUARED 2500

Plate::Plate
(double newSampleRate,float newLx, float newLy, float newThickness, float newStiffness,float newDamping, int newMaterialChoice)
{
    sampleRate = newSampleRate;
    Lx = newLx;
    Ly = newLy;
    thickness = newThickness;
    stiffness = newStiffness;
    materialChoice = newMaterialChoice;
    
    nextModes.resize(MAX_MODES_SQUARED);
    spatial.resize  (MAX_MODES_SQUARED);
    
    currentModes      = new float[MAX_MODES_SQUARED];
    previousModes     = new float[MAX_MODES_SQUARED];
    currentCoeff      = new float[MAX_MODES_SQUARED];
    previousCoeff     = new float[MAX_MODES_SQUARED];
    modeVectorX       = new float[MAX_MODES_SQUARED];
    modeVectorY       = new float[MAX_MODES_SQUARED];
    forcingCoeff      = new float[MAX_MODES_SQUARED];
    calculationBuffer = new float[MAX_MODES_SQUARED];
    
    currentDisplacement  = new float[MAX_MODES_SQUARED];
    previousDisplacement = new float[MAX_MODES_SQUARED];
    
    forcingDuration = floor(0.005*sampleRate);
    forcing         = new float[forcingDuration]; // forcing happens only during the first ~5msec
    
    setVectorCoefficients();
}

Plate::~Plate()
{
    
}

void Plate::reset(double newSampleRate, float newLx, float newLy, float newThickness, float newStiffness,float newDamping, int newMaterial)
{
    sampleRate= newSampleRate;
    Lx        = newLx;
    Ly        = newLy;
    thickness = newThickness;
    stiffness = newStiffness;
    damping   = newDamping;
    materialChoice = newMaterial;
    
    setVectorCoefficients();
}
//==================================
//~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|
void Plate::setVectorCoefficients()
{
    //=== Material Option <-> volume density
    float volDens;
    if (materialChoice == 1) volDens = 8050;
    else                     volDens = 10050;
    
    stiffness = (1e11-1e09)*stiffness + 1e09;   // map map (0.0,1.0) to physical Yougng's Modulus values: (1e09,1e11)
    
    //=== Calculate intermediate quantities and calculate the #modes
    const float nu = 0.3;
    const float tension = 100.0f;
    float flexuralRigidness = (thickness*thickness*thickness*stiffness) / ((12*(1-nu*nu))); // D term!
    float mu = thickness * volDens;
    float sigma0 = damping;
    float sigma1 = 0.1f * damping;
    double Gamma = -4.0f * sampleRate * sampleRate;
    double temp1 = double_Pi / Lx;
    double temp2 = double_Pi / Ly;
    double Alpha = double(flexuralRigidness/mu)*(temp1*temp1*temp1 + (2 * double_Pi*double_Pi*double_Pi*double_Pi)/(Lx*Lx*Ly*Ly)+temp2*temp2*temp2*temp2);
    double Beta = -double(tension/mu)*double(pow(double_Pi/Lx,2) + pow(double_Pi/Ly,2));
    double Delta= Beta*Beta - 4 * Alpha * Gamma;
    gamma = 5 * sigma0 + 5; // gammaMin = 5, gammaMax = 10 -> map sigma0 to a gamma range
    
    modes = 0.5 * abs( (int) sqrt((sqrt(Delta) -  Beta) / (2*Alpha))  );
    if (modes > MAX_MODES) modes = MAX_MODES;
    modesSquared = modes * modes;
    
    /*
    std::cout << "Lx = " << Lx << " , Ly = " << Ly << " , thick = " << thickness << " , stiff = " << stiffness << std::endl;
    std::cout << "modes = " << modes <<   " , D = " << flexuralRigidness << " , mu = " << mu << " , sigma0 = " << sigma0 <<  std::endl;
    */
    
    spatial.resize(modesSquared);
    nextModes.resize(modesSquared);
    
    float strikeX = 0.2;
    float strikeY = 0.7;
    double samplePeriod = 1 / sampleRate;
    
    for (int i=0; i < modes; i++)
    {
        for (int j=0; j < modes; j++)
        {
            modeVectorX[i*modes+j] = i+1;
            modeVectorY[i*modes+j] = j+1;
        }
    }
    
    
    for (int i=0; i<modesSquared; ++i)
    {
        if (i < forcingDuration)
        {
            forcing[i] = 5 * (1-std::cos((float_Pi/forcingDuration) * ( double(i) / sampleRate) ));// Raised cosine excitation signal
        }
        
        calculationBuffer[i]  = pow( (float_Pi * modeVectorX[i]) / Lx ,2) + pow((float_Pi*modeVectorY[i]) / Ly ,2) ;
        
        spatial[i] = sin(float_Pi*double(modeVectorX[i])*(strikeX/Lx)) * sin(float_Pi*double(modeVectorY[i])*(strikeY/Ly));
        
        // Current displacement
        currentDisplacement[i] = (2.0 - double(5.1419e-10) * double( ( tension * calculationBuffer[i] + flexuralRigidness * calculationBuffer[i] * calculationBuffer[i] ) / mu)) / (1 + double(samplePeriod * (sigma0 + sigma1 * calculationBuffer[i])));
        
        // Previous displacement
        previousDisplacement[i] = double(( -1 + samplePeriod * (sigma0 + sigma1 * calculationBuffer[i] ) ) / (1 + samplePeriod * (sigma0 + sigma1 * calculationBuffer[i])));
        
        // Current forcing
        forcingCoeff[i] = double(( ((4 * double(5.1419e-10))/(mu*Lx*Ly)) * sin(float_Pi*modeVectorX[i]*(strikeX/Lx)) * sin(float_Pi*modeVectorY[i]*(strikeY/Ly)) ) / (1 + samplePeriod * (sigma0 + sigma1 * calculationBuffer[i])));

        nextModes[i] = 0 ; currentModes[i] = 0; previousModes[i] = 0;

    }
    
    counter = 0;
    totalDuration =  (int) 10 * sampleRate;
    
}
//==================================
//~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|
void Plate::getPercussionNote(float* buffer,int numSamples)
{
    
    for (int n=0; n<numSamples; ++n)
    {
        double sum = 0;
        if (counter > 2 && counter < totalDuration)
        {
            if (counter < forcingDuration)  //~~~~~~~~~~~ Here, we will include the forcing samples RC.
                for (int i=0; i < modesSquared; i++)
                    nextModes[i] = currentDisplacement[i] * currentModes[i] + previousDisplacement[i]*previousModes[i] + forcingCoeff[i]*forcing[counter];
            
            else                //~~~~~~~~~~~ Here, the forcing is over. Only previous states affect the vibration
                for (int i=0; i < modesSquared; i++)
                    nextModes[i] = currentDisplacement[i] * currentModes[i] + previousDisplacement[i]*previousModes[i];
        }
    

        for (int i=0; i<modesSquared; ++i)
        {
            sum += double(exp(-gamma*counter/44100) ) * double( (1 / 0.3) * 1e15 * spatial[i] * nextModes[i]); // manual inner product
            //sum = double(exp(-gamma*counter/sampleRate)) * double( 3e15 * std::inner_product(begin(spatial), end(spatial), begin(nextModes), 0.0));
            // Updates
            previousModes[i] = currentModes[i];
            currentModes[i] = nextModes[i];
        }
        buffer[n] = sum;
        counter++;
    }
    
}

