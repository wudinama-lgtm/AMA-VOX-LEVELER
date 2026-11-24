#pragma once
#include <JuceHeader.h>

class AmaVocalLevelerAudioProcessor : public juce::AudioProcessor
{
public:
    AmaVocalLevelerAudioProcessor();
    ~AmaVocalLevelerAudioProcessor() override = default;

    void prepareToPlay(double, int) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;

    const juce::String getName() const override { return "AMA VOCAL LEVELER"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    bool hasEditor() const override { return true; }
    bool isBusesLayoutSupported(const BusesLayout&) const override;

    juce::AudioProcessorValueTreeState apvts;

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    double sampleRate = 44100.0;
    float rmsEnvelope = 1.0e-5f, riderGain = 1.0f, peakGain = 1.0f;
    float rmsAttackCoeff = 0.0f, rmsReleaseCoeff = 0.0f;
    float gainSmoothCoeff = 0.0f, peakReleaseCoeff = 0.0f;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AmaVocalLevelerAudioProcessor)
};
