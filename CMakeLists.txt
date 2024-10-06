cmake_minimum_required(VERSION 3.10)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.14 CACHE STRING "Build for 10.11")

project(sst-mts-source VERSION 0.9.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(libs/JUCE)
add_subdirectory(libs/clap-juce-extensions EXCLUDE_FROM_ALL)
add_subdirectory(libs/sst/sst-jucegui)
add_subdirectory(libs/sst/tuning-library)


juce_add_plugin(${PROJECT_NAME}
    PRODUCT_NAME "Surge Synth Team MTS Source"
    COMPANY_NAME "Surge Synth Team"  
    BUNDLE_ID "org.surge-synth-team.mts-source"
    PLUGIN_MANUFACTURER_CODE VmbA
    PLUGIN_CODE sMTS

    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    
    FORMATS AU VST3 Standalone

    COPY_PLUGIN_AFTER_BUILD FALSE
)

clap_juce_extensions_plugin(TARGET ${PROJECT_NAME}
        CLAP_ID "org.surge-synth-team.mts-source.clap"
        CLAP_FEATURES instrument "tuning source" microtonal)


target_include_directories(${PROJECT_NAME}
  PRIVATE
  src/
)

add_library(oddsound-mts-source libs/MTS-ESP/Master/libMTSMaster.cpp)
target_include_directories(oddsound-mts-source PUBLIC libs/MTS-ESP/master)

target_sources(${PROJECT_NAME} PRIVATE src/PluginEditor.cpp src/PluginProcessor.cpp)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    JUCE_ALLOW_STATIC_NULL_VARIABLES=0
    JUCE_STRICT_REFCOUNTEDPOINTER=1

    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_USE_CURL=0
    JUCE_WEB_BROWSER=0

    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_REPORT_APP_USAGE=0

    JUCE_ALSA=0
    JUCE_JACK=0
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        juce::juce_graphics
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_audio_devices
        tuning-library
        oddsound-mts-source
        sst-jucegui
        )

