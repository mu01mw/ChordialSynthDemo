#include "PluginProcessor.h"
#include "PluginEditor.h"

ControllerGroup::ControllerGroup(AudioProcessorValueTreeState & state, std::string title, Colour colour) : parameters(state), groupColour(colour)
{
    heading.setText(title, NotificationType::dontSendNotification);
    addAndMakeVisible(heading);
}

void ControllerGroup::addControlsForParameters(std::vector<std::string> params)
{
    for (auto& p : params)
    {
        auto name = parameters.getParameter(StringRef(p))->name;
        
        sliders.push_back(std::make_unique<SliderAndLabel>()); // in C++17 change to emplace_back to get reference to slider
        
        auto& sliderAndLabel = sliders.back();
#if JUCE_IOS || JUCE_ANDROID
        sliderAndLabel->slider.setSliderStyle(Slider::SliderStyle::LinearVertical);
#else
        sliderAndLabel->slider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
#endif
        sliderAndLabel->slider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
        
        sliderAndLabel->slider.setColour(Slider::ColourIds::rotarySliderFillColourId, groupColour);
        sliderAndLabel->slider.setColour(Slider::ColourIds::trackColourId, groupColour);
        
        sliderAndLabel->slider.setColour(Slider::ColourIds::thumbColourId, Colours::white.withAlpha(0.0f));
        sliderAndLabel->label.setText(name, NotificationType::dontSendNotification);
        sliderAndLabel->label.setJustificationType(Justification::centred);
        addAndMakeVisible(sliderAndLabel.get());
        sliderAttachments.push_back(std::make_unique<SliderAttachment>(parameters, p, sliderAndLabel->slider));
    }
}

void ControllerGroup::paint(Graphics & g)
{
    g.setColour(Colours::grey.withAlpha(0.2f));
    g.drawRoundedRectangle(getLocalBounds().reduced(2).toFloat(), 10.0f, 2.0f);
}

void ControllerGroup::resized()
{
    auto r = getLocalBounds().reduced(10);
    heading.setBounds(r.removeFromTop(20));
    using Track = Grid::TrackInfo;
    Grid grid;
    
    Array<Track> tracks{ Track(1_fr), Track(1_fr) };
    grid.templateColumns = tracks;
    grid.templateRows = tracks;
    grid.columnGap = 10_px;
    grid.rowGap = 10_px;
    grid.autoFlow = Grid::AutoFlow::row;
    
    for (const auto& slider : sliders)
        grid.items.add(GridItem(slider.get()));
    
    grid.performLayout(r);
}

ChordialSynthDemoAudioProcessorEditor::ChordialSynthDemoAudioProcessorEditor (ChordialSynthDemoAudioProcessor& p) : AudioProcessorEditor (&p), processor (p), keyboard(p.getKeyboardState(), MidiKeyboardComponent::Orientation::horizontalKeyboard)
{
    auto colour = Colours::cornflowerblue;
    groups[groupNames::oscillator] = std::make_unique<ControllerGroup>(p.getSynthState(), "Oscillator", colour);
    groups[groupNames::filter] = std::make_unique<ControllerGroup>(p.getSynthState(), "Filter", colour.withRotatedHue(0.25));
    groups[groupNames::adsr1] = std::make_unique<ControllerGroup>(p.getSynthState(), "Envelope 1", colour.withRotatedHue(0.5));
    groups[groupNames::adsr2] = std::make_unique<ControllerGroup>(p.getSynthState(), "Envelope 2", colour.withRotatedHue(0.75));
    
    groups[groupNames::oscillator]->addControlsForParameters({ DETUNE_AMT_PARAM, SPREAD_PARAM, LFO_AMT_PARAM, LFO_FREQ_PARAM});
    groups[groupNames::filter]->addControlsForParameters({ FILTER_CUTOFF_PARAM, FILTER_RESONANCE_PARAM, FILTER_CUTOFF_MOD_DEPTH_PARAM});
    groups[groupNames::adsr1]->addControlsForParameters({ ADSR1_ATTACK_PARAM, ADSR1_DECAY_PARAM, ADSR1_SUSTAIN_PARAM, ADSR1_RELEASE_PARAM});
    groups[groupNames::adsr2]->addControlsForParameters({ ADSR2_ATTACK_PARAM, ADSR2_DECAY_PARAM, ADSR2_SUSTAIN_PARAM, ADSR2_RELEASE_PARAM});
    
    addAndMakeVisible(groups[groupNames::oscillator].get());
    addAndMakeVisible(groups[groupNames::filter].get());
    addAndMakeVisible(groups[groupNames::adsr1].get());
    addAndMakeVisible(groups[groupNames::adsr2].get());

    keyboard.setAvailableRange(36, 84);
    addAndMakeVisible(keyboard);
    
    setSize (800, 800);
}

ChordialSynthDemoAudioProcessorEditor::~ChordialSynthDemoAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

void ChordialSynthDemoAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void ChordialSynthDemoAudioProcessorEditor::resized()
{
    using Track = Grid::TrackInfo;
    Grid grid;
    Array<Track> tracks{ Track(1_fr), Track(1_fr)};
    grid.templateColumns = tracks;
    grid.templateRows = tracks;
    grid.columnGap = 10_px;
    grid.rowGap = 10_px;
    
    grid.items = {
        GridItem(groups[groupNames::oscillator].get()), GridItem(groups[groupNames::adsr1].get()),
        GridItem(groups[groupNames::filter].get()),     GridItem(groups[groupNames::adsr2].get())
    };
    
    auto bounds = getLocalBounds().reduced(10);
    
    keyboard.setBounds(bounds.removeFromBottom(100));
    keyboard.setKeyWidth(30.0f);
    bounds.removeFromBottom(10);
    
    grid.performLayout(bounds);
}

ControllerGroup::SliderAndLabel::SliderAndLabel()
{
    addAndMakeVisible(slider);
    addAndMakeVisible(label);
}

void ControllerGroup::SliderAndLabel::resized()
{
    auto r = getLocalBounds();
    label.setBounds(r.removeFromBottom(20));
    slider.setBounds(r);
}
