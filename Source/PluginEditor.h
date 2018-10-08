#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include <unordered_map>

class ControllerGroup : public Component
{
public:
	ControllerGroup(AudioProcessorValueTreeState& state, std::string heading, Colour colour);
	
    /** Adds a control to the group for each element in the vector.
        This does not check if parameter exists or has previously been added
     */
    void addControlsForParameters(std::vector<std::string> params);

	void paint(Graphics& g) override;
	void resized() override;

private:
    /** Component which holds and positions a slider and label.
     */
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

/**
*/
class ChordialSynthDemoAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    ChordialSynthDemoAudioProcessorEditor (ChordialSynthDemoAudioProcessor&);
    ~ChordialSynthDemoAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
	enum class groupNames { oscillator, filter, adsr1, adsr2};

	using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
	
	std::unordered_map<groupNames, std::unique_ptr<ControllerGroup>> groups;
    ChordialSynthDemoAudioProcessor& processor;
    MidiKeyboardComponent keyboard;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChordialSynthDemoAudioProcessorEditor)
};
