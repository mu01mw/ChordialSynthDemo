/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include <unordered_map>

class ControllerGroup : public Component
{
public:
	ControllerGroup(AudioProcessorValueTreeState& state, std::string heading, Colour colour);
	void addControlsForParameters(std::vector<std::string> params);

	void paint(Graphics& g) override;
	void resized() override;

private:
	struct SliderAndLabel : Component
	{
		SliderAndLabel();
		void resized() override;

		Slider slider;
		Label label;
	};

	Label heading;
	AudioProcessorValueTreeState& parameters;
	using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
	std::vector<std::unique_ptr<SliderAndLabel>> sliders;
	std::vector<std::unique_ptr<SliderAttachment>> sliderAttachments;
	Colour groupColour;
};

//==============================================================================
/**
*/
class ChordialSynthDemoAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    ChordialSynthDemoAudioProcessorEditor (ChordialSynthDemoAudioProcessor&);
    ~ChordialSynthDemoAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
	enum class groupNames { oscillator, filter, adsr1, adsr2};

	using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
	
	std::unordered_map<groupNames, std::unique_ptr<ControllerGroup>> groups;
	MidiKeyboardComponent keyboard;
    ChordialSynthDemoAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChordialSynthDemoAudioProcessorEditor)
};
