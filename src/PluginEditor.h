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

#ifndef SSTMTSSOURCE_SRC_PLUGINEDITOR_H
#define SSTMTSSOURCE_SRC_PLUGINEDITOR_H

#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"
#include "sst/jucegui/components/WindowPanel.h"


//==============================================================================
/**
 */
class SSTMTSSourceEditor : public juce::AudioProcessorEditor
{
  public:
    SSTMTSSourceEditor(SSTMTSSourceProcessor &);
    ~SSTMTSSourceEditor();

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

    // Do this later
    std::unique_ptr<juce::Timer> idleTimer;
    void idle();

    std::unique_ptr<sst::jucegui::components::WindowPanel> mainPanel;

  private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SSTMTSSourceProcessor &processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SSTMTSSourceEditor)
};

#endif // SSTMTSSOURCE_SRC_PLUGINEDITOR_H
