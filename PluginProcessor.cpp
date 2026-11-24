#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace {
float dbToGain(float db) { return juce::Decibels::decibelsToGain(db); }
float gainToDb(float g) { return juce::Decibels::gainToDecibels(g, -100.0f); }
}

AmaVocalLevelerAudioProcessor::AmaVocalLevelerAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout
AmaVocalLevelerAudioProcessor::createParameterLayout()
{
    using P = juce::AudioParameterFloat;
    juce::AudioProcessorValueTreeState::ParameterLayout p;
    p.add(std::make_unique<P>("target", "Target level", juce::NormalisableRange<float>(-30.0f, -10.0f, 0.1f), -18.0f, " dBFS"));
    p.add(std::make_unique<P>("boost", "Maximum boost", juce::NormalisableRange<float>(0.0f, 18.0f, 0.1f), 8.0f, " dB"));
    p.add(std::make_unique<P>("cut", "Maximum reduction", juce::NormalisableRange<float>(0.0f, 18.0f, 0.1f), 8.0f, " dB"));
    p.add(std::make_unique<P>("speed", "Rider speed", juce::NormalisableRange<float>(0.05f, 2.0f, 0.01f), 0.45f, " s"));
    p.add(std::make_unique<P>("ceiling", "Peak ceiling", juce::NormalisableRange<float>(-6.0f, -0.1f, 0.1f), -1.0f, " dBFS"));
    p.add(std::make_unique<P>("output", "Output", juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f, " dB"));
    p.add(std::make_unique<juce::AudioParameterBool>("bypass", "Bypass", false));
    return p;
}

void AmaVocalLevelerAudioProcessor::prepareToPlay(double sr, int)
{
    sampleRate = sr > 0.0 ? sr : 44100.0;
    rmsEnvelope = 1.0e-5f;
    riderGain = 1.0f;
    peakGain = 1.0f;
    const float attack = std::exp(-1.0f / (0.012f * (float) sampleRate));
    const float release = std::exp(-1.0f / (0.180f * (float) sampleRate));
    rmsAttackCoeff = attack;
    rmsReleaseCoeff = release;
    gainSmoothCoeff = std::exp(-1.0f / (0.080f * (float) sampleRate));
    peakReleaseCoeff = std::exp(-1.0f / (0.100f * (float) sampleRate));
}

void AmaVocalLevelerAudioProcessor::releaseResources() {}

bool AmaVocalLevelerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto in = layouts.getMainInputChannelSet();
    const auto out = layouts.getMainOutputChannelSet();
    return in == out && (in == juce::AudioChannelSet::mono() || in == juce::AudioChannelSet::stereo());
}

void AmaVocalLevelerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const int channels = getTotalNumInputChannels();
    const int samples = buffer.getNumSamples();
    for (int ch = getTotalNumInputChannels(); ch < getTotalNumOutputChannels(); ++ch)
        buffer.clear(ch, 0, samples);

    if (*apvts.getRawParameterValue("bypass") > 0.5f)
        return;

    const float target = dbToGain(apvts.getRawParameterValue("target")->load());
    const float maxBoost = apvts.getRawParameterValue("boost")->load();
    const float maxCut = apvts.getRawParameterValue("cut")->load();
    const float speed = apvts.getRawParameterValue("speed")->load();
    const float ceiling = dbToGain(apvts.getRawParameterValue("ceiling")->load());
    const float outputGain = dbToGain(apvts.getRawParameterValue("output")->load());

    // Lissage du niveau cible : le temps de réponse règle la vitesse du vocal rider.
    const float riderCoeff = std::exp(-1.0f / (juce::jmax(0.05f, speed) * (float) sampleRate));

    for (int i = 0; i < samples; ++i)
    {
        float detector = 0.0f;
        for (int ch = 0; ch < channels; ++ch)
            detector = juce::jmax(detector, std::abs(buffer.getSample(ch, i)));

        // Détecteur RMS approché (moyenne quadratique mono liée).
        const float square = detector * detector;
        const float coeff = square > rmsEnvelope ? rmsAttackCoeff : rmsReleaseCoeff;
        rmsEnvelope = coeff * rmsEnvelope + (1.0f - coeff) * square;
        const float measured = std::sqrt(juce::jmax(rmsEnvelope, 1.0e-10f));

        float wantedDb = juce::jlimit(-maxCut, maxBoost,
            gainToDb(target) - gainToDb(measured));
        const float wantedGain = dbToGain(wantedDb);
        riderGain = riderCoeff * riderGain + (1.0f - riderCoeff) * wantedGain;

        // Réduction rapide des crêtes, avec relâchement doux.
        const float afterRiderPeak = detector * riderGain * outputGain;
        const float wantedPeakGain = afterRiderPeak > ceiling ? ceiling / afterRiderPeak : 1.0f;
        if (wantedPeakGain < peakGain)
            peakGain = wantedPeakGain;
        else
            peakGain = peakReleaseCoeff * peakGain + (1.0f - peakReleaseCoeff) * wantedPeakGain;

        const float totalGain = riderGain * peakGain * outputGain;
        for (int ch = 0; ch < channels; ++ch)
            buffer.setSample(ch, i, buffer.getSample(ch, i) * totalGain);
    }
}

juce::AudioProcessorEditor* AmaVocalLevelerAudioProcessor::createEditor()
{
    return new AmaVocalLevelerAudioProcessorEditor(*this);
}

void AmaVocalLevelerAudioProcessor::getStateInformation(juce::MemoryBlock& dest)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, dest);
}

void AmaVocalLevelerAudioProcessor::setStateInformation(const void* data, int size)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, size));
    if (xml && xml->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmaVocalLevelerAudioProcessor();
}
