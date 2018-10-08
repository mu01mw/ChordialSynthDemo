#include "PluginProcessor.h"
#include "PluginEditor.h"

ChordialSynthDemoAudioProcessor::ChordialSynthDemoAudioProcessor()
    : AudioProcessor (BusesProperties().withOutput ("Output", AudioChannelSet::stereo(), true)),
    apvtState(*this, &undoMan), synth(apvtState)
{
	apvtState.state = ValueTree("DemoSynth");
	synth.setNumberOfVoices(4);
}

ChordialSynthDemoAudioProcessor::~ChordialSynthDemoAudioProcessor()
{
}

const String ChordialSynthDemoAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ChordialSynthDemoAudioProcessor::acceptsMidi() const
{
	return true;
}

bool ChordialSynthDemoAudioProcessor::producesMidi() const
{
	return false;
}

bool ChordialSynthDemoAudioProcessor::isMidiEffect() const
{
	return false;
}

double ChordialSynthDemoAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ChordialSynthDemoAudioProcessor::getNumPrograms()
{
    return 1;
}

int ChordialSynthDemoAudioProcessor::getCurrentProgram()
{
    return 0;
}

const String ChordialSynthDemoAudioProcessor::getProgramName (int index)
{
    return {};
}

void ChordialSynthDemoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	keyboardState.reset();
	synth.prepareToPlay(sampleRate, samplesPerBlock);
}

bool ChordialSynthDemoAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void ChordialSynthDemoAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

	for (auto i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
	synth.processBlock(buffer, midiMessages);   
}

bool ChordialSynthDemoAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* ChordialSynthDemoAudioProcessor::createEditor()
{
    return new ChordialSynthDemoAudioProcessorEditor (*this);
}

void ChordialSynthDemoAudioProcessor::getStateInformation (MemoryBlock& destData)
{
}

void ChordialSynthDemoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChordialSynthDemoAudioProcessor();
}
