/* ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ============================================================================== */
#include "PluginProcessor.h"
#include "PluginEditor.h"
#define DEFAULT_SAMPLE_RATE 44100
//==============================================================================
class PercSound : public SynthesiserSound
{
public:
    PercSound() {}
    
    bool appliesToNote      (int /*midiNoteNumber*/)    override  { return true; }
    bool appliesToChannel   (int /*midiChannel*/)       override  { return true; }
};
//==============================================================================
PhysicalPercPluginAudioProcessor::PhysicalPercPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                     #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                     #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
        
    plate = new Plate(DEFAULT_SAMPLE_RATE, Lx, Ly, thickness, stiffness, damping, materialChoice);
      
    for (int i = numVoices; --i >= 0;)
    {
        synth.addVoice (new PercussionVoice(stiffness, damping, thickness, materialChoice, volume));
    }
    
    synth.clearSounds();
    synth.addSound (new PercSound());
    
    /*
    percussionVoice = new PercussionVoice();
    inputDeviceManager.addMidiInputCallback(String::empty, &midiCollector);
    inputDeviceManager.setMidiInputEnabled(MidiInput::getDevices()[0], true);
    midiCollector.reset(DEFAULT_SAMPLE_RATE);
    synth.setCurrentPlaybackSampleRate(DEFAULT_SAMPLE_RATE);
    synth.addVoice(new PercussionVoice());
    synth.clearSounds();
    synth.addSound(new percSound());
    */
}

PhysicalPercPluginAudioProcessor::~PhysicalPercPluginAudioProcessor()
{
    inputDeviceManager.removeMidiInputCallback(String::empty, &midiCollector);
}

//==============================================================================
const String PhysicalPercPluginAudioProcessor::getName() const          {return JucePlugin_Name;}

bool PhysicalPercPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PhysicalPercPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}
double PhysicalPercPluginAudioProcessor::getTailLengthSeconds() const       {return 0.0;}

int PhysicalPercPluginAudioProcessor::getNumPrograms()                      {return 1;}

int PhysicalPercPluginAudioProcessor::getCurrentProgram()                   {return 0;}

void PhysicalPercPluginAudioProcessor::setCurrentProgram (int index)        {}

const String PhysicalPercPluginAudioProcessor::getProgramName (int index)   {return String();}

void PhysicalPercPluginAudioProcessor::changeProgramName (int index, const String& newName){}

//==============================================================================

//====================================
//~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|

void PhysicalPercPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    midiCollector.reset(sampleRate);
    synth.setCurrentPlaybackSampleRate(sampleRate);
}

void PhysicalPercPluginAudioProcessor::releaseResources()
{

}

void PhysicalPercPluginAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int numSamples = buffer.getNumSamples();
    keyboardState.processNextMidiBuffer (midiMessages, 0, numSamples, true);
    synth.renderNextBlock (buffer, midiMessages, 0, numSamples);
    
    //MidiBuffer::Iterator midiBufferIterator(midiMessages);
    
    MidiBuffer midiBuffer;
    MidiMessage midiMessage;
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
    
    /*
    int sample_number;
    MidiBuffer::Iterator midi_buffer_iterator(midiMessages);
    MidiMessage midi_message;

    while (midi_buffer_iterator.getNextEvent(midi_message,sample_number))
    {
        
     if (midi_message.isNoteOn())
     {
        setDimensions(midi_message.getNoteNumber());
        plate->reset(getSampleRate(), Lx, Ly, thickness, stiffness, damping, materialChoice);
     
        for (int chan=0; chan<buffer.getNumChannels(); ++chan)
        {
            float* channelData = buffer.getWritePointer(chan);
            plate->getPercussionNote(channelData,buffer.getNumSamples());
        }
        
    }
    */
        
        
        
        /*
        for (int chan=0; chan<buffer.getNumChannels(); ++chan)
        {
            float* channelData = buffer.getWritePointer(chan);
            for (int i=0; i<buffer.getNumSamples(); ++i)
            {
                float gain = dB2Amp(volume);
                channelData[i] = gain * plate->getPercussionNote();
                //std::cout << channelData[i] << std::endl;
            }
        }
       
        
    }
    
 */

    /*
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    MidiBuffer incomingMidi;
    midiCollector.removeNextBlockOfMessages(incomingMidi, buffer.getNumSamples());
    synth.renderNextBlock(buffer, incomingMidi, 0, buffer.getNumSamples());
     */
}

//~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|~|
//====================================
//==============================================================================
#ifndef JucePlugin_PreferredChannelConfigurations
bool PhysicalPercPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
#else

    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;
    
    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif
    
    return true;
#endif
}
#endif

bool PhysicalPercPluginAudioProcessor::hasEditor() const{return true;}

AudioProcessorEditor* PhysicalPercPluginAudioProcessor::createEditor(){return new PhysicalPercPluginAudioProcessorEditor (*this);}
//==============================================================================
void PhysicalPercPluginAudioProcessor::getStateInformation (MemoryBlock& destData)              {}

void PhysicalPercPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)  {}
//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PhysicalPercPluginAudioProcessor();
}
