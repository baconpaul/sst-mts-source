/*
 * SST MTS Source - an editable morphable tuning source
 * for the ODDSound MTS-ESP protocol.
 *
 * Learn more at https://surge-synth-team.org/
 *
 * Copyright 2023, various authors, as described in the GitHub
 * transaction log.
 *
 * SST MTS Source is released under the GNU General Public Licence v3
 * or later (GPL-3.0-or-later). The license is found in the "LICENSE"
 * file in the root of this repository, or at
 * https://www.gnu.org/licenses/gpl-3.0.en.html
 *
 * All source for SST MTS Source is available at
 * https://github.com/surge-synthesizer/sst-mts-source
 */

#ifndef SSTMTSSOURCE_SRC_PLUGINPROCESSOR_H
#define SSTMTSSOURCE_SRC_PLUGINPROCESSOR_H

#include <juce_audio_processors/juce_audio_processors.h>
#include "Tunings.h"
#include <set>

class SSTMTSSourceProcessor : public juce::AudioProcessor
{
  public:
    //==============================================================================
    SSTMTSSourceProcessor();
    ~SSTMTSSourceProcessor();

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String &newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

    // == tuning support ==
    void setSCL(juce::String SCL, bool retune = true);
    void setKBM(juce::String KBM, bool retune = true);
    void resetSCLToStandard()
    {
        auto s = Tunings::evenTemperament12NoteScale();
        setSCL(s.rawText);
    }
    void resetKBMToStandard() { setKBM(""); }

    void retune();

    Tunings::Tuning tuning;
    juce::String currentSCLString = "", currentKBMString = "";

    std::atomic<int> numClients{0};

  private:
    bool registeredMTS{false};
    std::atomic<bool> repushTuning{false};
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SSTMTSSourceProcessor)
};

#endif // SSTMTSSOURCE_SRC_PLUGINPROCESSOR_H
