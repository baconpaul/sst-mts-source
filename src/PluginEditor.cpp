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

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "sst/jucegui/style/StyleSheet.h"
#include "sst/jucegui/components/NamedPanel.h"
#include "sst/jucegui/components/WindowPanel.h"

namespace jcmp = sst::jucegui::components;

struct SSTMTSWindow : jcmp::WindowPanel
{
    SSTMTSWindow()
    {
        sst::jucegui::style::StyleSheet::initializeStyleSheets([]() {});
        const auto &base = sst::jucegui::style::StyleSheet::getBuiltInStyleSheet(sst::jucegui::style::StyleSheet::DARK);
        base->setColour(jcmp::WindowPanel::Styles::styleClass, jcmp::WindowPanel::Styles::backgroundgradstart, juce::Colour(60,60,70));
        base->setColour(jcmp::WindowPanel::Styles::styleClass, jcmp::WindowPanel::Styles::backgroundgradend, juce::Colour(20,20,30));
        base->setColour(jcmp::BaseStyles::styleClass, jcmp::BaseStyles::regionBorder, juce::Colour(90,90,100));
        setStyle(base);


        morphPanel = std::make_unique<jcmp::NamedPanel>("Morph");
        addAndMakeVisible(*morphPanel);

        tabPanel = std::make_unique<jcmp::NamedPanel>("Tabs");
        tabPanel->isTabbed = true;
        tabPanel->tabNames = {"Scale 1", "Scale 2", "Scale 3", "Scale 4"};
        tabPanel->resetTabState();
        addAndMakeVisible(*tabPanel);
    }

    void resized()
    {
        auto mpWidth = 250;
        morphPanel->setBounds(getLocalBounds().withWidth(mpWidth));
        tabPanel->setBounds(getLocalBounds().withTrimmedLeft(mpWidth));
    }

    std::unique_ptr<jcmp::NamedPanel> tabPanel;
    std::unique_ptr<jcmp::NamedPanel> morphPanel;
};

//==============================================================================
SSTMTSSourceEditor::SSTMTSSourceEditor(SSTMTSSourceProcessor &p)
    : juce::AudioProcessorEditor(&p), processor(p)
{

    mainPanel = std::make_unique<SSTMTSWindow>();
    addAndMakeVisible(*mainPanel);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(900, 600);
    setResizable(true, true);
}

SSTMTSSourceEditor::~SSTMTSSourceEditor() {}

//==============================================================================
void SSTMTSSourceEditor::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::black);
}

void SSTMTSSourceEditor::idle() {}

void SSTMTSSourceEditor::resized() {
    mainPanel->setBounds(getLocalBounds());
}
