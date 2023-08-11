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
#include "libMTSMaster.h"

//==============================================================================
SSTMTSSourceProcessor::SSTMTSSourceProcessor()
    : juce::AudioProcessor(juce::AudioProcessor::BusesProperties().withOutput(
          "Output", juce::AudioChannelSet::stereo(), true))
{
    auto initS = Tunings::evenTemperament12NoteScale();
    setSCL(initS.rawText, false);
    setKBM("", false);
    retune();

    if (false && MTS_CanRegisterMaster())
    {
        MTS_RegisterMaster();
        registeredMTS = true;
        repushTuning = true;
    }
}

SSTMTSSourceProcessor::~SSTMTSSourceProcessor()
{
    if (registeredMTS)
        MTS_DeregisterMaster();
}

//==============================================================================
const juce::String SSTMTSSourceProcessor::getName() const { return JucePlugin_Name; }

bool SSTMTSSourceProcessor::acceptsMidi() const { return true; }

bool SSTMTSSourceProcessor::producesMidi() const { return false; }

bool SSTMTSSourceProcessor::isMidiEffect() const { return false; }

double SSTMTSSourceProcessor::getTailLengthSeconds() const { return 0.0; }

int SSTMTSSourceProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int SSTMTSSourceProcessor::getCurrentProgram() { return 0; }

void SSTMTSSourceProcessor::setCurrentProgram(int index) {}

const juce::String SSTMTSSourceProcessor::getProgramName(int index) { return {}; }

void SSTMTSSourceProcessor::changeProgramName(int index, const juce::String &newName) {}

//==============================================================================
void SSTMTSSourceProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {}

void SSTMTSSourceProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool SSTMTSSourceProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
    return true;
}

void SSTMTSSourceProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                         juce::MidiBuffer &midiMessages)
{
    if (!registeredMTS)
        return;

    numClients = MTS_GetNumClients();
}

//==============================================================================
bool SSTMTSSourceProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *SSTMTSSourceProcessor::createEditor()
{
    return new SSTMTSSourceEditor(*this);
}

//==============================================================================
void SSTMTSSourceProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    auto xml = std::make_unique<juce::XmlElement>("state");

    xml->deleteAllChildElementsWithTagName("tuningState");
    auto txml = xml->createNewChildElement("tuningState");
    auto sclx = txml->createNewChildElement("scl");
    sclx->addTextElement(currentSCLString);
    auto kbmx = txml->createNewChildElement("kbm");
    kbmx->addTextElement(currentKBMString);

    copyXmlToBinary(*xml, destData);
}

void SSTMTSSourceProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
    {
        auto txml = xmlState->getChildByName("tuningState");
        if (txml)
        {
            bool needsRT = false;

            auto sclx = txml->getChildByName("scl");
            auto kbmx = txml->getChildByName("kbm");
            std::string s = "";

            if (sclx && sclx->getFirstChildElement() &&
                sclx->getFirstChildElement()->isTextElement())
            {
                s = sclx->getFirstChildElement()->getText().toStdString();
                if (s.size() > 1)
                {
                    needsRT = true;
                    setSCL(s, false);
                }
            }

            std::string k = "";
            if (kbmx && kbmx->getFirstChildElement() &&
                kbmx->getFirstChildElement()->isTextElement())
            {
                k = kbmx->getFirstChildElement()->getText().toStdString();
                if (k.size() > 1)
                {
                    needsRT = true;
                    setKBM(k, false);
                }
            }

            if (needsRT)
            {
                retune();
            }
        }
    }
}

void SSTMTSSourceProcessor::setSCL(juce::String SCL, bool dretune)
{
    currentSCLString = SCL;
    if (dretune)
        retune();
}

void SSTMTSSourceProcessor::setKBM(juce::String KBM, bool dretune)
{
    currentKBMString = KBM;
    if (dretune)
        retune();
}

void SSTMTSSourceProcessor::retune()
{
    try
    {
        if (currentKBMString.isEmpty())
            currentKBMString = Tunings::tuneA69To(440).rawText;
        tuning = Tunings::Tuning(Tunings::parseSCLData(currentSCLString.toStdString()),
                                 Tunings::parseKBMData(currentKBMString.toStdString()));
        repushTuning = true;

        repushTuning = false;
        for (int i = 0; i < 128; ++i)
        {
            MTS_SetNoteTuning(tuning.frequencyForMidiNote(i), i);
        }
        MTS_SetScaleName(tuning.scale.name.c_str());
    }
    catch (const Tunings::TuningError &e)
    {
        // This is just sample code
        std::cout << "TUNING ERROR " << e.what() << std::endl;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() { return new SSTMTSSourceProcessor(); }
