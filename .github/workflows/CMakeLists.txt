cmake_minimum_required(VERSION 3.22)
project(AMA_VOCAL_LEVELER VERSION 1.0.0)

# JUCE doit être installé séparément. Passez -DJUCE_DIR=chemin/vers/JUCE à CMake.
if(NOT DEFINED JUCE_DIR)
    message(FATAL_ERROR "Définissez JUCE_DIR vers le dossier JUCE (ex: C:/JUCE).")
endif()
add_subdirectory("${JUCE_DIR}" JUCE)

juce_add_plugin(AMA_VOCAL_LEVELER
    COMPANY_NAME "AMA Audio"
    BUNDLE_ID "com.amaaudio.vocalleveler"
    PLUGIN_MANUFACTURER_CODE AmAu
    PLUGIN_CODE AmVl
    FORMATS VST3
    PRODUCT_NAME "AMA VOCAL LEVELER"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
)

target_sources(AMA_VOCAL_LEVELER PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)
target_compile_features(AMA_VOCAL_LEVELER PRIVATE cxx_std_17)
target_compile_definitions(AMA_VOCAL_LEVELER PUBLIC
    JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0
)
target_link_libraries(AMA_VOCAL_LEVELER PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    PUBLIC juce::juce_recommended_config_flags
           juce::juce_recommended_lto_flags
           juce::juce_recommended_warning_flags
)
