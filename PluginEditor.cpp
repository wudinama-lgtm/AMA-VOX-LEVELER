#include "PluginProcessor.h"
#include "PluginEditor.h"

AmaVocalLevelerAudioProcessorEditor::AmaVocalLevelerAudioProcessorEditor(AmaVocalLevelerAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(560, 390);
    title.setText("AMA VOCAL LEVELER", juce::dontSendNotification);
    title.setFont(juce::Font(juce::FontOptions(27.0f, juce::Font::bold)));
    title.setJustificationType(juce::Justification::centred);
    title.setColour(juce::Label::textColourId, juce::Colour(0xfff3f5f7));
    addAndMakeVisible(title);

    setupSlider(target, targetLabel, "TARGET LEVEL", "target", "-18 dBFS");
    setupSlider(boost, boostLabel, "WEAK VOICE BOOST", "boost", "8 dB");
    setupSlider(cut, cutLabel, "LOUD VOICE REDUCTION", "cut", "8 dB");
    setupSlider(speed, speedLabel, "RIDER SPEED", "speed", "0.45 s");
    setupSlider(ceiling, ceilingLabel, "PEAK CEILING", "ceiling", "-1 dBFS");
    setupSlider(output, outputLabel, "OUTPUT", "output", "0 dB");

    bypass.setButtonText("BYPASS");
    bypass.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    addAndMakeVisible(bypass);
    attachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.apvts, "bypass", bypass));
}

void AmaVocalLevelerAudioProcessorEditor::setupSlider(juce::Slider& s, juce::Label& l, const juce::String& name, const juce::String& id, const juce::String& suffix)
{
    l.setText(name, juce::dontSendNotification);
    l.setColour(juce::Label::textColourId, juce::Colour(0xffb8c4d0));
    l.setFont(juce::Font(juce::FontOptions(11.0f, juce::Font::bold)));
    l.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(l);
    s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    s.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 82, 22);
    s.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff36c9a2));
    s.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    s.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff202a36));
    addAndMakeVisible(s);
    attachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, id, s));
    (void) suffix;
}

void AmaVocalLevelerAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff101720));
    g.setColour(juce::Colour(0xff1b2835));
    g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(12.0f), 16.0f);
    g.setColour(juce::Colour(0xff36c9a2));
    g.fillRoundedRectangle(juce::Rectangle<float>(24, 76, 512, 3), 2.0f);
    g.setColour(juce::Colour(0xff8fa1b2));
    g.setFont(11.0f);
    g.drawFittedText("AUTOMATIC VOCAL LEVELING  •  VST3", 20, 354, 520, 20, juce::Justification::centred, 1);
}

void AmaVocalLevelerAudioProcessorEditor::resized()
{
    title.setBounds(20, 20, 520, 45);
    const int x0 = 34, y0 = 94, cellW = 164, cellH = 116, gapX = 0, gapY = 10;
    juce::Component* sliders[] = { &target, &boost, &cut, &speed, &ceiling, &output };
    juce::Label* labels[] = { &targetLabel, &boostLabel, &cutLabel, &speedLabel, &ceilingLabel, &outputLabel };
    for (int i = 0; i < 6; ++i)
    {
        const int col = i % 3, row = i / 3;
        const int x = x0 + col * (cellW + gapX), y = y0 + row * (cellH + gapY);
        labels[i]->setBounds(x, y, cellW, 20);
        sliders[i]->setBounds(x + 15, y + 19, cellW - 30, 88);
    }
    bypass.setBounds(218, 332, 124, 26);
}
