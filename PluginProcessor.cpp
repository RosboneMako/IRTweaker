/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "cmath"              //R1.00 Added library.

//==============================================================================
MakoBiteAudioProcessor::MakoBiteAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),

    //R1.10 Define our VALUE TREE parameter variables. Min val, Max Val, default Val.
    //R1.10 These need to match the values used for the EDITOR controls/sliders.
    parameters(*this, nullptr, "PARAMETERS",
        {
          std::make_unique<juce::AudioParameterFloat>("gain","Gain", .0f, 1.0f, .15f),

          std::make_unique<juce::AudioParameterInt>("ir1off","IR1 off", -100, 100, 0),
          std::make_unique<juce::AudioParameterInt>("ir2off","IR2 off", -100, 100, 0),
          std::make_unique<juce::AudioParameterFloat>("ir1vol","IR1 Vol", .0f, 1.0f, 1.0f),
          std::make_unique<juce::AudioParameterFloat>("ir2vol","IR2 Vol", .0f, 1.0f, 0.0f),

          std::make_unique<juce::AudioParameterInt>("eq1f","EQ 1 F", 20, 1000, 120),
          std::make_unique<juce::AudioParameterFloat>("eq1a","EQ 1 A", -12.0f, 12.0f, 0.0f),
          std::make_unique<juce::AudioParameterFloat>("eq1q","EQ 1 Q", .1f, 4.0f, 0.707f),
          
          std::make_unique<juce::AudioParameterInt>("eq2f","EQ 2 F", 200, 2500, 700),
          std::make_unique<juce::AudioParameterFloat>("eq2a","EQ 2 A", -12.0f, 12.0f, 0.0f),
          std::make_unique<juce::AudioParameterFloat>("eq2q","EQ 2 Q", .1f, 4.0f, 0.35f),

          std::make_unique<juce::AudioParameterInt>("eq3f","EQ 3 F", 500, 15000, 2000),
          std::make_unique<juce::AudioParameterFloat>("eq3a","EQ 3 A", -12.0f, 12.0f, 0.0f),
          std::make_unique<juce::AudioParameterFloat>("eq3q","EQ 3 Q", .1f, 4.0f, 0.35f),

          std::make_unique<juce::AudioParameterInt>("highcut","High Cut", 1000, 10000, 10000),
          std::make_unique<juce::AudioParameterInt>("lowcut","Low Cut", 20, 300, 20),

          std::make_unique<juce::AudioParameterFloat>("irf1","IRF1", -1.0f, 1.0f, .0f),
          std::make_unique<juce::AudioParameterFloat>("irf2","IRF2", -1.0f, 1.0f, .0f),
          std::make_unique<juce::AudioParameterFloat>("irf3","IRF3", -1.0f, 1.0f, .0f),
          std::make_unique<juce::AudioParameterFloat>("irf4","IRF4", -1.0f, 1.0f, .0f),
          std::make_unique<juce::AudioParameterFloat>("irf5","IRF5", -1.0f, 1.0f, .0f),
          std::make_unique<juce::AudioParameterFloat>("irf6","IRF6", -1.0f, 1.0f, .0f),
          std::make_unique<juce::AudioParameterFloat>("irf7","IRF7", -1.0f, 1.0f, .0f),
          std::make_unique<juce::AudioParameterFloat>("irf8","IRF8", -1.0f, 1.0f, .0f),
          std::make_unique<juce::AudioParameterFloat>("irf9","IRF9", -1.0f, 1.0f, .0f),
          std::make_unique<juce::AudioParameterFloat>("irf10","IRF10", -1.0f, 1.0f, .0f),
          std::make_unique<juce::AudioParameterFloat>("irf11","IRF11", -1.0f, 1.0f, .0f),
          std::make_unique<juce::AudioParameterFloat>("irf12","IRF12", -1.0f, 1.0f, .0f),
          std::make_unique<juce::AudioParameterFloat>("irf13","IRF13", -1.0f, 1.0f, .0f),
          std::make_unique<juce::AudioParameterFloat>("irf14","IRF14", -1.0f, 1.0f, .0f),
          std::make_unique<juce::AudioParameterFloat>("irf15","IRF15", -1.0f, 1.0f, .0f),
          std::make_unique<juce::AudioParameterFloat>("irf16","IRF16", -1.0f, 1.0f, .0f),
          std::make_unique<juce::AudioParameterFloat>("irf17","IRF17", -1.0f, 1.0f, .0f),
          std::make_unique<juce::AudioParameterFloat>("irf18","IRF18", -1.0f, 1.0f, .0f),
          std::make_unique<juce::AudioParameterFloat>("irf19","IRF19", -1.0f, 1.0f, .0f),
          std::make_unique<juce::AudioParameterFloat>("irf20","IRF20", -1.0f, 1.0f, .0f),
          std::make_unique<juce::AudioParameterFloat>("irf21","IRF21", -1.0f, 1.0f, .0f),

        }
        )
#endif
{   
    Setting[e_Gain] = parameters.getRawParameterValue("gain");
    Setting[e_IR1off] = parameters.getRawParameterValue("ir1off");
    Setting[e_IR2off] = parameters.getRawParameterValue("ir2off");
    Setting[e_IR1Vol] = parameters.getRawParameterValue("ir1vol");
    Setting[e_IR2Vol] = parameters.getRawParameterValue("ir2vol");

    Setting[e_EQ1F] = parameters.getRawParameterValue("eq1f");
    Setting[e_EQ1A] = parameters.getRawParameterValue("eq1a");
    Setting[e_EQ1Q] = parameters.getRawParameterValue("eq1q");
    Setting[e_EQ2F] = parameters.getRawParameterValue("eq2f");
    Setting[e_EQ2A] = parameters.getRawParameterValue("eq2a");
    Setting[e_EQ2Q] = parameters.getRawParameterValue("eq2q");
    Setting[e_EQ3F] = parameters.getRawParameterValue("eq3f");
    Setting[e_EQ3A] = parameters.getRawParameterValue("eq3a");
    Setting[e_EQ3Q] = parameters.getRawParameterValue("eq3q");

    Setting[e_HighCut] = parameters.getRawParameterValue("highcut");
    Setting[e_LowCut] = parameters.getRawParameterValue("lowcut");

    Setting[e_IRF1] = parameters.getRawParameterValue("irf1");
    Setting[e_IRF2] = parameters.getRawParameterValue("irf2");
    Setting[e_IRF3] = parameters.getRawParameterValue("irf3");
    Setting[e_IRF4] = parameters.getRawParameterValue("irf4");
    Setting[e_IRF5] = parameters.getRawParameterValue("irf5");
    Setting[e_IRF6] = parameters.getRawParameterValue("irf6");
    Setting[e_IRF7] = parameters.getRawParameterValue("irf7");
    Setting[e_IRF8] = parameters.getRawParameterValue("irf8");
    Setting[e_IRF9] = parameters.getRawParameterValue("irf9");
    Setting[e_IRF10] = parameters.getRawParameterValue("irf10");
    Setting[e_IRF11] = parameters.getRawParameterValue("irf11");
    Setting[e_IRF12] = parameters.getRawParameterValue("irf12");
    Setting[e_IRF13] = parameters.getRawParameterValue("irf13");
    Setting[e_IRF14] = parameters.getRawParameterValue("irf14");
    Setting[e_IRF15] = parameters.getRawParameterValue("irf15");
    Setting[e_IRF16] = parameters.getRawParameterValue("irf16");
    Setting[e_IRF17] = parameters.getRawParameterValue("irf17");
    Setting[e_IRF18] = parameters.getRawParameterValue("irf18");
    Setting[e_IRF19] = parameters.getRawParameterValue("irf19");
    Setting[e_IRF20] = parameters.getRawParameterValue("irf20");
    Setting[e_IRF21] = parameters.getRawParameterValue("irf21");

}

MakoBiteAudioProcessor::~MakoBiteAudioProcessor()
{
}

//==============================================================================
const juce::String MakoBiteAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MakoBiteAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MakoBiteAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MakoBiteAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MakoBiteAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MakoBiteAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MakoBiteAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MakoBiteAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MakoBiteAudioProcessor::getProgramName (int index)
{
    return {};
}

void MakoBiteAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MakoBiteAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //R1.10 Get our Sample Rate for filter calculations.
    float tRate = SampleRate;
    SampleRate = MakoBiteAudioProcessor::getSampleRate();
    if (SampleRate < 21000) SampleRate = 48000.0f;
    if (192000 < SampleRate) SampleRate = 48000.0f;

    //R1.10 If our sample rate has changed magically, update our filters, etc.
    if ((tRate != SampleRate) || (!SettingsPrepared))
    {
        SettingsPrepared = true;
        Settings_Update(true);
    }

}

void MakoBiteAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MakoBiteAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//R1.00 This is the JUCE created function that handles audio processing.
void MakoBiteAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    //R1.10 Our defined variables.
    float tS;       //R1.00 Temporary working sample.    

    VST_Activity++;
    if (1000000 < VST_Activity) VST_Activity = 0;

    //R1.00 Handle any changes to our Paramters.
    //R1.00 Handle any settings changes made in Editor. 
    if (0 < SettingsChanged) Settings_Update(false);    

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        //R1.00 Process the AUDIO buffer data.
        if (Pedal_Mono && (channel == 1))
        {
            auto* channel0Data = buffer.getWritePointer(0);

            //R1.00 FORCE MONO - Put CHANNEL 0 data in CHANNEL 1.
            for (int samp = 0; samp < buffer.getNumSamples(); samp++) channelData[samp] = channel0Data[samp];
        }
        else
        {
            //1.00 loop thru the audio data.
            for (int samp = 0; samp < buffer.getNumSamples(); samp++)
            {
                //R1.10 PULSE_ON is true when creating a new IR for FFT Calc and SAVE IR.
                //R1.10 PULSE_ON is false for normal audio operation.
                if (!Pulse_On)
                    tS = buffer.getSample(channel, samp); //R1.00 Get the current sample from the audio buffer and put it in tS. 
                else
                    tS = PULSE_StreamIR();                //R1.10 Create a IR pulse stream.
               
                //R1.10 High Pass / Low Cut.
                if (20.0f < *Setting[e_LowCut]) tS = Filter_Calc_BiQuad(tS, channel, &makoF_LowCut);
                if (*Setting[e_HighCut] < 10000.0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_HighCut);

                //R1.10 Impulse Response (IR). 0 = Off.
                if (Do_Speaker && USE_IR)
                {
                    tS *= (*Setting[e_Gain]);
                    tS = Mako_CabSim(tS, channel);
                }
                else
                    tS *= (*Setting[e_Gain] * 5.0f);

                //R1.00 If doing Speakers, EQ the speaker.
                tS = Filter_Apply_EQ(tS, channel);
                tS = Filter_Apply_GEQ(tS, channel);
                
                //R1.10 Start saving the output waveform from the pulse IR generation.
                if (Pulse_On) if (10 < Pulse_Cnt) Wave_Temp[Pulse_Cnt - 11] = tS;

                //R1.10 See if we are clipping and flag the editor to show UI notice.
                //R1.10 Do not check during IR creation. Peak is normalized later.
                if (!Pulse_On) CheckForClipping(tS);

                //R1.00 Write our modified sample back into the sample buffer.
                channelData[samp] = tS;
            }
        }
    }
}


//R1.10 Create an Impulse in the audio data stream.
float MakoBiteAudioProcessor::PULSE_StreamIR()
{
    float tS = 0.0f;

    //R1.10 Clear the IR and EQ buffers or the new IR will be random garbage.
    if (Pulse_Cnt == 0) Buffers_Clear(0);

    //R1.10 Create an Impulse in the audio data stream.
    //R1.10 Wait 10 safety samples before starting the pulse.
    if (Pulse_Cnt == 11) tS = 1.0f;

    //R1.10 IR should be approx 2048+10 samples.
    Pulse_Cnt++;
    if (2060 < Pulse_Cnt)
    {
        //R1.10 We have completed the fresh IR stream. Return to normal operation.
        Pulse_On = false;
        Pulse_Cnt = 0;
        Pulse_Normalize(&Wave_Temp[0]);
        Pulse_Complete = true; //R1.00 Flag editor that our pulse data is ready to be saved. 
    }

    return tS;
}

//==============================================================================
bool MakoBiteAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MakoBiteAudioProcessor::createEditor()
{
    return new MakoBiteAudioProcessorEditor (*this);
}


//==============================================================================
void MakoBiteAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    //R1.00 Save our parameters to file/DAW.
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);  
    
}

void MakoBiteAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    //R1.00 Read our parameters from file/DAW.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));

    Settings_Update(true);
}

void MakoBiteAudioProcessor::FILE_Load_IR_Wave(juce::String tFile, float* buffer)
{
    float max = 0.0f;    
    int LIS = 0;

    //R1.00 Setup JUCE variables to read the WAVE file.
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::File jFile = juce::File::File(tFile);
    formatManager.registerBasicFormats();
    auto fileBuffer = juce::AudioBuffer < float >::AudioBuffer();
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(jFile));

    //R1.00 Read the WAVE file.
    if (reader.get() != nullptr)
    {
        //R1.00 Left here in case we want to check duration.
        //auto duration = (float)reader->lengthInSamples / reader->sampleRate;              
        //if (duration < 1)

        //R1.10 Added larger file accomodation.
        auto SampCount = (float)reader->lengthInSamples;
        if ((SampCount < 100000) && (reader->sampleRate < 50000))
        {
            //R1.10 Added post for Large files.
            if (4000 < SampCount)
            {
                ERR_Message1 = "NOTE: Speaker IR file is very large. All IRs are treated as 2048 samples.";
                IR_RefreshUI = true;
            }

            fileBuffer.setSize((int)reader->numChannels, (int)reader->lengthInSamples);
            reader->read(&fileBuffer, 0, (int)reader->lengthInSamples, 0, true, false);

            //R1.00 Get number of samples but limit to 2048.
            //R1.10 Could loop thru data and look for start of IR then add 2048 from there.
            LIS = (int)reader->lengthInSamples;
            if (2048 < LIS) LIS = 2048;           

            //R1.00 Get the max val in case we need to normalize the IR to 1.0 max.
            float tV;
            float VScale = 1.0f;
            for (int t = 0; t < LIS; t++)
            {
                tV = fileBuffer.getSample(0, t);                
                if (max < abs(tV)) max = abs(tV);
            }

            //R1.00 Check for IR scale requirements.
            if (1.0f < max)  VScale = 1.0f / max;

            //R1.00 Store the WAVE file data into our Temporary IR buffer.
            for (int t = 0; t < LIS; t++) buffer[t] = VScale * fileBuffer.getSample(0, t);
            for (int t = LIS; t < 2048; t++) buffer[t] = 0.0f;
        }
        else
        {
            //R1.10 Handle File errors. Set string and force editor update.
            if (SampCount < 100000) ERR_Message1 = "ERROR: Speaker IR file is invalid (2048 sample files max).";
            if (reader->sampleRate < 50000) ERR_Message1 = "ERROR: Sample rate exceeds 48 kHz.";
            IR_RefreshUI = true;
        }

    }
    else
    {
        //R1.00 Handle File errors. Set string and force editor update.
        ERR_Message1 = "ERROR: Speaker IR file is missing or invalid.";
        IR_RefreshUI = true;
    }
}


void MakoBiteAudioProcessor::Pulse_Normalize(float* buffer)
{
    float max = 0.0f;
    float VScale = 0.0f;

    for (int t = 0; t < 2080; t++)
    {
        if (max < abs(buffer[t])) max = abs(buffer[t]);
    }

    VScale = 1.0f / max;
    for (int t = 0; t < 2048; t++)
        buffer[t] *= VScale;

}

//R1.00 Parameter reading helper function.
int MakoBiteAudioProcessor::makoGetParmValue_int(juce::String Pstring)
{
    auto parm = parameters.getRawParameterValue(Pstring);
    if (parm != NULL)
        return int(parm->load());
    else
        return 0;
}

//R1.00 Parameter reading helper function.
float MakoBiteAudioProcessor::makoGetParmValue_float(juce::String Pstring)
{
    auto parm = parameters.getRawParameterValue(Pstring);
    if (parm != NULL)
        return float(parm->load());
    else
        return 0.0f;
}



//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MakoBiteAudioProcessor();
}

float MakoBiteAudioProcessor::Filter_Apply_EQ(float tSample, int channel)
{
    float tS = tSample;

    //R1.00 Apply EQ. Dont calc unless needed to save CPU cycles.    
    //R1.00 This will cause glitches if automating since buffers will not fill while at 0 (off).
    if (USE_EQ)
    {
        if (*Setting[e_EQ1A] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_Band1);
        if (*Setting[e_EQ2A] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_Band2);
        if (*Setting[e_EQ3A] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_Band3);        
    }

    return tS;
}

float MakoBiteAudioProcessor::Filter_Apply_GEQ(float tSample, int channel)
{
    float tS = tSample;

    //R1.00 Apply Graphic EQ. Dont calc unless needed to save CPU cycles.    
    //R1.00 This will cause glitches if automating since buffers will not fill while at 0 (off).
    if (USE_GEQ)
    {
        if (*Setting[e_IRF1] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_IRF[1]);
        if (*Setting[e_IRF2] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_IRF[2]);
        if (*Setting[e_IRF3] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_IRF[3]);
        if (*Setting[e_IRF4] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_IRF[4]);
        if (*Setting[e_IRF5] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_IRF[5]);
        if (*Setting[e_IRF6] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_IRF[6]);
        if (*Setting[e_IRF7] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_IRF[7]);
        if (*Setting[e_IRF8] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_IRF[8]);
        if (*Setting[e_IRF9] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_IRF[9]);
        if (*Setting[e_IRF10] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_IRF[10]);
        if (*Setting[e_IRF11] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_IRF[11]);
        if (*Setting[e_IRF12] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_IRF[12]);
        if (*Setting[e_IRF13] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_IRF[13]);
        if (*Setting[e_IRF14] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_IRF[14]);
        if (*Setting[e_IRF15] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_IRF[15]);
        if (*Setting[e_IRF16] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_IRF[16]);
        if (*Setting[e_IRF17] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_IRF[17]);
        if (*Setting[e_IRF18] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_IRF[18]);
        if (*Setting[e_IRF19] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_IRF[19]);
        if (*Setting[e_IRF20] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_IRF[20]);
        if (*Setting[e_IRF21] != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_IRF[21]);
    }

    return tS;
}

//R1.00 Apply filter fn to a sample.
float MakoBiteAudioProcessor::Filter_Calc_BiQuad(float tSample, int channel, tp_filter* fn)
{
    float tS = tSample;

    fn->xn0[channel] = tS;
    tS = fn->a0 * fn->xn0[channel] + fn->a1 * fn->xn1[channel] + fn->a2 * fn->xn2[channel] - fn->b1 * fn->yn1[channel] - fn->b2 * fn->yn2[channel];
    fn->xn2[channel] = fn->xn1[channel]; fn->xn1[channel] = fn->xn0[channel]; fn->yn2[channel] = fn->yn1[channel]; fn->yn1[channel] = tS;

    return tS;
}

void MakoBiteAudioProcessor::Filter_Calc_BiQuad_Clear(int channel, tp_filter* fn)
{
    fn->xn2[channel] = 0.0f;
    fn->xn1[channel] = 0.0f;
    fn->yn2[channel] = 0.0f;
    fn->yn1[channel] = 0.0f;
}

//R1.00 Second order parametric/peaking boost filter with constant-Q
void MakoBiteAudioProcessor::Filter_BP_Coeffs(float Gain_dB, float Fc, float Q, tp_filter* fn)
{    
    float K = pi2 * (Fc * .5f) / SampleRate;
    float K2 = K * K;
    float V0 = pow(10.0, Gain_dB / 20.0);

    float a = 1.0f + (V0 * K) / Q + K2;
    float b = 2.0f * (K2 - 1.0f);
    float g = 1.0f - (V0 * K) / Q + K2;
    float d = 1.0f - K / Q + K2;
    float dd = 1.0f / (1.0f + K / Q + K2);

    fn->a0 = a * dd;
    fn->a1 = b * dd;
    fn->a2 = g * dd;
    fn->b1 = b * dd;
    fn->b2 = d * dd;
    fn->c0 = 1.0f;
    fn->d0 = 0.0f;
}

//R1.00 Second order LOW PASS filter. 
void MakoBiteAudioProcessor::Filter_LP_Coeffs(float fc, tp_filter* fn)
{    
    float c = 1.0f / (tanf(pi * fc / SampleRate));
    fn->a0 = 1.0f / (1.0f + sqrt2 * c + (c * c));
    fn->a1 = 2.0f * fn->a0;
    fn->a2 = fn->a0;
    fn->b1 = 2.0f * fn->a0 * (1.0f - (c * c));
    fn->b2 = fn->a0 * (1.0f - sqrt2 * c + (c * c));
}

//F1.00 Second order HIGH PASS filter.
void MakoBiteAudioProcessor::Filter_HP_Coeffs(float fc, tp_filter* fn)
{    
    float c = tanf(pi * fc / SampleRate);
    fn->a0 = 1.0f / (1.0f + sqrt2 * c + (c * c));
    fn->a1 = -2.0f * fn->a0;
    fn->a2 = fn->a0;
    fn->b1 = 2.0f * fn->a0 * ((c * c) - 1.0f);
    fn->b2 = fn->a0 * (1.0f - sqrt2 * c + (c * c));
}

//R4.00 First Order HIGH PASS filter.
void MakoBiteAudioProcessor::Filter_FO_HP_Coeffs(float fc, tp_filter* fn)
{
    float th = 2.0 * pi * fc / SampleRate;
    float g = cos(th) / (1.0 + sin(th));
    fn->a0 = (1.0 + g) / 2.0;
    fn->a1 = -((1.0 + g) / 2.0);
    fn->a2 = 0.0f;
    fn->b1 = -g;
    fn->b2 = 0.0f;
    fn->c0 = 0.0f;
    fn->d0 = 0.0f;
}


//R1.00 We do changes here so we know the vars are not in use while we change them.
//R1.00 EDITOR sets SETTING flags and we make changes here.
void MakoBiteAudioProcessor::Settings_Update(bool ForceAll)
{    
    bool Force = ForceAll;

    //R1.00 Deal with IR based changes.
    if (File_LoadNewIR1)
    {
        File_LoadNewIR1 = false;
        FileOnly_IRSpeaker1 = File_IRSpeaker1.fromLastOccurrenceOf("\\", false, true);
        FILE_Load_IR_Wave(File_IRSpeaker1, &IR_SpkrFile1[0]);
        IR_Offset(&IR_SpkrFile1[0], &IR_Spkr1[0], *Setting[e_IR1off]);
        MIX_IRs();
        Do_Speaker = true;
        IR_RefreshUI = true;
    }
    if (File_LoadNewIR2)
    {
        File_LoadNewIR2 = false;
        FileOnly_IRSpeaker2 = File_IRSpeaker2.fromLastOccurrenceOf("\\", false, true);
        FILE_Load_IR_Wave(File_IRSpeaker2, &IR_SpkrFile2[0]);
        IR_Offset(&IR_SpkrFile2[0], &IR_Spkr2[0], *Setting[e_IR2off]);
        MIX_IRs();
        Do_Speaker = true;
        IR_RefreshUI = true;
    }
    if (Offset1_Update)
    {
        Offset1_Update = false;
        IR_Offset(&IR_SpkrFile1[0], &IR_Spkr1[0], *Setting[e_IR1off]);
        MIX_IRs();
        IR_RefreshUI = true;
    }
    if (Offset2_Update)
    {
        Offset2_Update = false;
        IR_Offset(&IR_SpkrFile2[0], &IR_Spkr2[0], *Setting[e_IR2off]);
        MIX_IRs();
        IR_RefreshUI = true;
    }
    if (MIX_Update)
    {
        MIX_Update = false;
        MIX_IRs();
        IR_RefreshUI = true;
    }
    if (IR1_Invert)
    {
        IR1_Invert = false;
        IR_Invert(&IR_SpkrFile1[0]);
        IR_Offset(&IR_SpkrFile1[0], &IR_Spkr1[0], *Setting[e_IR1off]);
        MIX_IRs();
        IR_RefreshUI = true;
    }
    if (IR2_Invert)
    {
        IR2_Invert = false;
        IR_Invert(&IR_SpkrFile2[0]);
        IR_Offset(&IR_SpkrFile2[0], &IR_Spkr2[0], *Setting[e_IR2off]);
        MIX_IRs();
        IR_RefreshUI = true;
    }
    if (IR1_Auto)
    {   
        IR1_Auto = false;
        if ((File_IRSpeaker1 != "") && (File_IRSpeaker2 != ""))
        {
            //R1.00 Align forces a change in Editor, updates cascade from there.            
            IR1_AutoAlign();
            IR_Offset(&IR_SpkrFile1[0], &IR_Spkr1[0], IR1_AutoNewPosition);
            MIX_IRs();
            IR_RefreshUI = true;
        }
    }
    if (IR2_Auto)
    {
        IR2_Auto = false;
        if ((File_IRSpeaker1 != "") && (File_IRSpeaker2 != ""))
        {
            //R1.00 Align forces a change in Editor, updates cascade from there.            
            IR2_AutoAlign();
            IR_Offset(&IR_SpkrFile2[0], &IR_Spkr2[0], IR2_AutoNewPosition);
            MIX_IRs();
            IR_RefreshUI = true;
        }
    }


    //R1.10 High and Low Cut. 
    Filter_LP_Coeffs(*Setting[e_HighCut], &makoF_HighCut);
    Filter_HP_Coeffs(*Setting[e_LowCut], &makoF_LowCut);

    //R1.10 Update our EQ Filters.
    Filter_BP_Coeffs(*Setting[e_EQ1A], *Setting[e_EQ1F], *Setting[e_EQ1Q], &makoF_Band1);
    Filter_BP_Coeffs(*Setting[e_EQ2A], *Setting[e_EQ2F], *Setting[e_EQ2Q], &makoF_Band2);
    Filter_BP_Coeffs(*Setting[e_EQ3A], *Setting[e_EQ3F], *Setting[e_EQ3Q], &makoF_Band3);
    
    //R1.10 Setup the Graphic EQ filters. The default Q is 2.0 (mode 2).
    float Q = 2.0f;
    if (GEQ_BW_Mode == 0) Q = .7f;
    if (GEQ_BW_Mode == 1) Q = 1.414f;
    if (GEQ_BW_Mode == 3) Q = 4.0f;
    Filter_BP_Coeffs(*Setting[e_IRF1] * 12.0f, 50.0f, Q, &makoF_IRF[1]);
    Filter_BP_Coeffs(*Setting[e_IRF2] * 12.0f, 100.0f, Q, &makoF_IRF[2]);
    Filter_BP_Coeffs(*Setting[e_IRF3] * 12.0f, 200.0f, Q, &makoF_IRF[3]);
    Filter_BP_Coeffs(*Setting[e_IRF4] * 12.0f, 300.0f, Q, &makoF_IRF[4]);
    Filter_BP_Coeffs(*Setting[e_IRF5] * 12.0f, 400.0f, Q, &makoF_IRF[5]);
    Filter_BP_Coeffs(*Setting[e_IRF6] * 12.0f, 500.0f, Q, &makoF_IRF[6]);
    Filter_BP_Coeffs(*Setting[e_IRF7] * 12.0f, 600.0f, Q, &makoF_IRF[7]);
    Filter_BP_Coeffs(*Setting[e_IRF8] * 12.0f, 700.0f, Q, &makoF_IRF[8]);
    Filter_BP_Coeffs(*Setting[e_IRF9] * 12.0f, 800.0f, Q, &makoF_IRF[9]);
    Filter_BP_Coeffs(*Setting[e_IRF10] * 12.0f, 900.0f, Q, &makoF_IRF[10]);
    Filter_BP_Coeffs(*Setting[e_IRF11] * 12.0f, 1000.0f, Q, &makoF_IRF[11]);
    Filter_BP_Coeffs(*Setting[e_IRF12] * 12.0f, 1250.0f, Q, &makoF_IRF[12]);
    Filter_BP_Coeffs(*Setting[e_IRF13] * 12.0f, 1500.0f, Q, &makoF_IRF[13]);
    Filter_BP_Coeffs(*Setting[e_IRF14] * 12.0f, 2000.0f, Q, &makoF_IRF[14]);
    Filter_BP_Coeffs(*Setting[e_IRF15] * 12.0f, 3000.0f, Q, &makoF_IRF[15]);
    Filter_BP_Coeffs(*Setting[e_IRF16] * 12.0f, 4000.0f, Q, &makoF_IRF[16]);
    Filter_BP_Coeffs(*Setting[e_IRF17] * 12.0f, 5000.0f, Q, &makoF_IRF[17]);
    Filter_BP_Coeffs(*Setting[e_IRF18] * 12.0f, 6000.0f, Q, &makoF_IRF[18]);
    Filter_BP_Coeffs(*Setting[e_IRF19] * 12.0f, 7000.0f, Q, &makoF_IRF[19]);
    Filter_BP_Coeffs(*Setting[e_IRF20] * 12.0f, 8000.0f, Q, &makoF_IRF[20]);
    Filter_BP_Coeffs(*Setting[e_IRF21] * 12.0f, 12000.0f, .35f, &makoF_IRF[21]);

    //R1.00 RESET our settings flags.
    SettingsType = 0;
    SettingsChanged = false;
}

float MakoBiteAudioProcessor::Mako_CabSim(float tSample, int channel)
{
    int T1;
    float V = 0.0f;
    
    //R1.00 Get the next buffer position Index and store the new incoming Sample.
    T1 = IRB_Idx[channel];
    IRB[channel][T1] = tSample;

    //R1.00 Calculate the IR response by multiplying every IR sample by our audio buffer samples.
    //R1.00 Effectively it is a DELAY(comb filter) pedal with 2048 repeats in a very short time.
    //R1.00 The repeats will add and zero out signals due to phase which creates an EQ filter.
    //R1.00 The IR acts as both a delay and filter combined.
    //R1.00 For absolute best sound an IR should be 2048 samples. But the IR calc is very heavy on CPU usage.
    //R1.10 IR Length should always be a power of 2 so bitmasking will work.
    for (int t = 0; t < 2048; t++)
    {
        V += (IR_Final[t] * IRB[channel][T1]);

        //R1.00 Increment index, mask off bits past 1023 to keep our index between 0-1023.
        //R1.00 IR length must be a power of 2 for masking to work 1024(3FF), 2048(7FF) are standard sizes.
        T1 = (T1 + 1) & 0x7FF;
    }
    
    //R1.00 Decrement our buffer index and loop around at 0. 
    IRB_Idx[channel]--;
    if (IRB_Idx[channel] < 0) IRB_Idx[channel] = 2047;

    //R1.00 We usually gain volume here so reduce it.
    return V;
}

//R1.10 Before creating a new IR pulse, all buffers need to be zeroed
//R1.10 or the random data in the buffers will become part of the new IR.
void MakoBiteAudioProcessor::Buffers_Clear(int channel)
{
    for (int t = 0; t < 2048; t++)
        IRB[channel][t] = 0.0f;

    Filter_Calc_BiQuad_Clear(0, &makoF_Band1);
    Filter_Calc_BiQuad_Clear(0, &makoF_Band2);
    Filter_Calc_BiQuad_Clear(0, &makoF_Band3);

    Filter_Calc_BiQuad_Clear(0, &makoF_LowCut);
    Filter_Calc_BiQuad_Clear(0, &makoF_HighCut);    

    for (int t=1;t<22;t++) Filter_Calc_BiQuad_Clear(0, &makoF_IRF[t]);
}

//R1.00 Move an IR Left or Right in time.
void MakoBiteAudioProcessor::IR_Offset(float* BufferIn, float* BufferOut, int SampleOffset)
{
    float Max = 0.0f;
    int Off1;    
    float Val1;
    
    for (int t = 0; t < 2048; t++)
    {
        //R1.00 Adjust sample by its time offset.
        Off1 = t - SampleOffset;
        if (Off1 < 0)
            Val1 = 0.0f;
        else if (2047 < Off1)
            Val1 = 0.0f;
        else
            Val1 = BufferIn[Off1];

        BufferOut[t] = Val1;
    }
}

//R1.10 Swap the polarity of the passed in IR data.
void MakoBiteAudioProcessor::IR_Invert(float* Buffer)
{
    for (int t = 0; t < 2048; t++) Buffer[t] *= -1.0f;
}

//R1.00 Auto align IR1 to IR2.
void MakoBiteAudioProcessor::IR1_AutoAlign()
{
    int Diff = 0;

    //R1.00 Find peaks in 1 and 2. Move 1s peak to 2s.
    float Max1 = 0.0f; int Max1Pos = 0; bool Max1Pol = true;
    float Max2 = 0.0f; int Max2Pos = 0; bool Max2Pol = true;

    for (int t = 0; t < 2048; t++)
    {
        if (Max1 < abs(IR_SpkrFile1[t])) 
        {
            Max1 = abs(IR_SpkrFile1[t]);
            if (IR_SpkrFile1[t] < 0.0f)
                Max1Pol = false;
            else
                Max1Pol = true;
            Max1Pos = t;
        }

        if (Max2 < abs(IR_Spkr2[t]))
        {
            Max2 = abs(IR_Spkr2[t]);
            if (IR_Spkr2[t] < 0.0f)
                Max2Pol = false;
            else
                Max2Pol = true;
            Max2Pos = t;
        }
    }

    if (Max1Pol != Max2Pol) IR_Invert(&IR_SpkrFile1[0]);

    //R1.00 Calc new IR1 offset position.
    //R1.00 New pos var is watched in Editor Timer and update occurs there.
    Diff = Max2Pos - Max1Pos;
    if (100 < abs(Diff)) Diff = 0; //R1.00 Limit to +/- 100.
    IR1_AutoNewPosition = Diff;

}

//R1.00 Auto align IR2 to IR1.
void MakoBiteAudioProcessor::IR2_AutoAlign()
{
    int Diff = 0;

    //R1.00 Find peaks in 1 and 2. Move 1s peak to 2s.
    float Max1 = 0.0f; int Max1Pos = 0; bool Max1Pol = true;
    float Max2 = 0.0f; int Max2Pos = 0; bool Max2Pol = true;

    for (int t = 0; t < 2048; t++)
    {
        if (Max1 < abs(IR_Spkr1[t]))
        {
            Max1 = abs(IR_Spkr1[t]);
            if (IR_Spkr1[t] < 0.0f)
                Max1Pol = false;
            else
                Max1Pol = true;
            Max1Pos = t;
        }

        if (Max2 < abs(IR_SpkrFile2[t]))
        {
            Max2 = abs(IR_SpkrFile2[t]);
            if (IR_SpkrFile2[t] < 0.0f)
                Max2Pol = false;
            else
                Max2Pol = true;
            Max2Pos = t;
        }
    }

    if (Max1Pol != Max2Pol) IR_Invert(&IR_SpkrFile2[0]);

    //R1.00 Calc new IR1 offset position.
    //R1.00 New pos var is watched in Editor Timer and update occurs there.
    Diff = Max1Pos - Max2Pos;
    if (100 < abs(Diff)) Diff = 0; //R1.00 Limit to +/- 100.
    IR2_AutoNewPosition = Diff;

}


//R1.10 Mix the two defined IRs (IR_Spkr1, IR_Spkr2).
void MakoBiteAudioProcessor::MIX_IRs()
{
    float Max = 0.0f;
    int Off1;
    int Off2;
    float Val1;
    float Val2;

    //R1.1.0 Mix the IRs and detect max sample.
    for (int t = 0; t < 2048; t++)
    {
        IR_Final[t] = (IR_Spkr1[t] * *Setting[e_IR1Vol]) + (IR_Spkr2[t] * *Setting[e_IR2Vol]);
        if (Max < abs(IR_Final[t])) Max = abs(IR_Final[t]);
    }

    //R1.00 Normalize the output to 1.0f.
    if (0.0f < Max)
    {
        for (int t = 0; t < 2048; t++) IR_Final[t] *= (1.0f / Max);
    }

}


void MakoBiteAudioProcessor::CheckForClipping(float tSample)
{
    //R1.00 Clip the signal to just below -1/1 so the audio engine does not crash. 
    //R1.00 Need a var here to let user know they are clipping. This is read in the
    //R1.00 EDITOR Timer code to show clipping on the UI.
    if (.9999f < tSample)
    {
        tSample = .9999f;
        AudioIsClipping = true;
    }
    else if (tSample < -.9999f)
    {
        tSample = -.9999f;
        AudioIsClipping = true;
    }    
}

