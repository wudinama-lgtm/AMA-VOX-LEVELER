#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class AmaVocalLevelerAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit AmaVocalLevelerAudioProcessorEditor(AmaVocalLevelerAudioProcessor&);
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    void setupSlider(juce::Slider&, juce::Label&, const juce::String&, const juce::String&, const juce::String&);
    AmaVocalLevelerAudioProcessor& processor;
    juce::Label title, targetLabel, boostLabel, cutLabel, speedLabel, ceilingLabel, outputLabel;
    juce::Slider target, boost, cut, speed, ceiling, output;
    juce::ToggleButton bypass;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> attachments;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AmaVocalLevelerAudioProcessorEditor)
};
