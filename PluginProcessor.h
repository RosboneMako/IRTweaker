/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class MakoBiteAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    MakoBiteAudioProcessor();
    ~MakoBiteAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
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
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    float (MakoBiteAudioProcessor::*fptr_Amp)(float, int);

    //R1.00 Add a Parameters variable.
    juce::AudioProcessorValueTreeState parameters;                           

    //R1.00 Variables you want to access in the EDITOR should be defined in a PUBLIC section here.
    //R1.00 Settings.
    int SettingsChanged = 0;
    int SettingsType = 0;
    std::atomic<float>* Setting[60] = { nullptr };  //R1.00 Multithread safe vars. Point to Knob values/parameters.
    float Setting_Last[60] = {};    
    bool Pedal_Mono = true;
    float SampleRate = 48000.0f;

    //R1.00 The IRs we are using.    
    float IR_Spkr1[2048] = {};  //R1.00 Adjusted IR 1.
    float IR_Final[2048] = {};  //R1.00 The IR we will use in our calculations.
    float IR_Spkr2[2048] = {};  //R1.00 Adjusted IR 2.    

    //R1.00 The EDITOR needs to be able to adjust these, so put them in public.
    float Pedal_Band1 = 0.0f;
    float Pedal_Band2 = 0.0f;
    float Pedal_Band3 = 0.0f;
    float Pedal_Band4 = 0.0f;
    float Pedal_Band5 = 0.0f;
    float Pedal_Thump = 0.0f;

    float Band1_Freq = 150.0f;
    float Band2_Freq = 300.0f;
    float Band3_Freq = 750.0f;
    float Band4_Freq = 1500.0f;
    float Band5_Freq = 3000.0f;
    float Band1_Q = .707f;
    float Band2_Q = 1.414f;
    float Band3_Q = 1.414f;
    float Band4_Q = 1.414f;
    float Band5_Q = 1.414f;
    
    //R1.00 Flag set when clipping, EDITOR watches it in the TIMER section. 
    bool AudioIsClipping = false;

    //R1.00 Flag that we are sending an IR Pulse. 
    bool Pulse_On = false;
    bool Pulse_Complete = false;
    int Pulse_Cnt = 0;

    //R1.00 The new Pulse response audio data. 
    float Wave_Temp[2100] = {};

    //R1.00 Deal with loading an IR from file. 
    juce::String File_IRSpeaker1 = "";
    juce::String FileOnly_IRSpeaker1 = "";
    bool File_LoadNewIR1 = false;
    juce::String File_IRSpeaker2 = "";
    juce::String FileOnly_IRSpeaker2 = "";
    bool File_LoadNewIR2 = false;
    juce::String ERR_Message1 = "";           //R1.00 If this is not empty it gets posted in editor.
    

    //R1.00 FLAGS to let Proc/Editor things need done.
    bool IR_RefreshUI = false;
    bool USE_IR = true;
    bool USE_EQ = true;
    bool USE_GEQ = true;   
    bool MIX_Update = false;
    bool Offset1_Update = false;
    bool Offset2_Update = false;
    bool IR1_Invert = false;
    bool IR2_Invert = false;
    bool IR1_Auto = false;
    bool IR2_Auto = false;
    int IR1_AutoNewPosition = -999;  //R1.00 New position from AUTO that needs to be changed in editor.
    int IR2_AutoNewPosition = -999;  //R1.00 -999 = No change.
    

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MakoBiteAudioProcessor)
  
    std::atomic<float>* Pars[60] = { nullptr };

    //R1.00 These are the indexes into our Settings var.
    enum {
        e_Gain, e_EQ1F, e_EQ1A, e_EQ1Q, e_EQ2F, e_EQ2A, e_EQ2Q, e_EQ3F, e_EQ3A, e_EQ3Q, e_HighCut, e_LowCut, e_IR1off, e_IR2off, e_IR1Vol, e_IR2Vol,
        e_IRF1, e_IRF2, e_IRF3, e_IRF4, e_IRF5, e_IRF6, e_IRF7, e_IRF8, e_IRF9, e_IRF10, e_IRF11, e_IRF12, e_IRF13, e_IRF14, e_IRF15, e_IRF16, e_IRF17, e_IRF18, e_IRF19, e_IRF20, e_IRF21,
    };
    
    //R1.00 Deal with loading an IR from file. 
    void FILE_Load_IR_Wave(juce::String tFile, float* buffer);
    bool Do_Speaker = false;        //R1.00 Flag if IR is loaded.
    float IR_SpkrFile1[2048] = {};  //R1.00 IR 1 from file.
    float IR_SpkrFile2[2048] = {};  //R1.00 IR 2 from file.    

    //R1.00 Maximize our Pulse data.
    void Pulse_Normalize(float* buffer);
    
    //R1.00 Clean up the parameter reading code.
    int makoGetParmValue_int(juce::String Pstring);
    float makoGetParmValue_float(juce::String Pstring);

    //R1.00 Handle parameter changes made in editor.
    void Settings_Update(bool ForceAll);

    //R1.00 Our actual AUDIO adjusting functions.    
    void CheckForClipping(float tSample);

    //R1.00 Amplifier IR BUFFER.
    //float InEQ_AudioBuffer[2][2050] = {  }; //R1.00 we need to buffer incoming audio to do calcs occuring in the past.
    //int InEQB_Ring1[2] = {  };
    

    //R1.00 Some Constants and vars.
    const float pi = 3.14159265f;
    const float pi2 = 6.2831853f;
    const float sqrt2 = 1.4142135f;
        

    //R1.00 OUR FILTER VARIABLES
    struct tp_coeffs {
        float a0;
        float a1;
        float a2;
        float b1;
        float b2;
        float c0;
        float d0;
    };

    struct tp_filter {
        float a0;
        float a1;
        float a2;
        float b1;
        float b2;
        float c0;
        float d0;
        float xn0[2];
        float xn1[2];
        float xn2[2];
        float yn1[2];
        float yn2[2];
        float offset[2];
    };

    float Filter_Apply_EQ(float tSample, int channel);
    float Filter_Apply_GEQ(float tSample, int channel);

    //R1.00 FILTER FUNCTIONS
    float Filter_Calc_BiQuad(float tSample, int channel, tp_filter* fn);
    void Filter_BP_Coeffs(float Gain_dB, float Fc, float Q, tp_filter* fn);
    void Filter_LP_Coeffs(float fc, tp_filter* fn);    
    void Filter_HP_Coeffs(float fc, tp_filter* fn);    
    void Filter_FO_HP_Coeffs(float fc, tp_filter* fn);

    //R1.00 Our pedal filters and function def.
    tp_filter makoF_HighCut = {};   //R1.00 
    tp_filter makoF_LowCut = {};    //R1.00 
    tp_filter makoF_Band1 = {};
    tp_filter makoF_Band2 = {};
    tp_filter makoF_Band3 = {};
    tp_filter makoF_IRF[32] = {};
    
    //R1.00 IR Cab simulator variables.
    float Mako_CabSim(float tSample, int channel);
    float IRB[2][2049] = {};       //R1.00 Our stored audio buffer that gets multiplied by the IR. 
    int IRB_Idx[2] = {};
    void MIX_IRs();
    void IR_Offset(float* BufferIn, float* BufferOut, int SampleOffset);
    void IR_Invert(float* Buffer);
    void IR1_AutoAlign();
    void IR2_AutoAlign();
    
    
};
