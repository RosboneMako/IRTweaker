/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MakoBiteAudioProcessorEditor::MakoBiteAudioProcessorEditor (MakoBiteAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{    
    
    addMouseListener(this, true);

    //R1,00 get our LOGO image from our resources.
    imgLogo = juce::ImageCache::getFromMemory(BinaryData::irt_back_png, BinaryData::irt_back_pngSize);
    imgSlideOn = juce::ImageCache::getFromMemory(BinaryData::slide_on_png, BinaryData::slide_on_pngSize);
    imgSlideOff = juce::ImageCache::getFromMemory(BinaryData::slide_off_png, BinaryData::slide_off_pngSize);
    
    //R1.00 Start our Timer so we can tell the user they are clipping. Could draw VU Meters here, etc.
    startTimerHz(10);  //R1.00 have our Timer get called 10 times per second.

    //****************************************************************************************
    //R1.00 Add GUI CONTROLS
    //****************************************************************************************
    GUI_Init_Rotary(&sldKnob[e_Gain], *audioProcessor.Setting[e_Gain],0.0f, 1.0f,.01f,"", 2, 0xFFC0C0B0);
    
    GUI_Init_Rotary(&sldKnob[e_EQ1F], *audioProcessor.Setting[e_EQ1F], 20.0f, 1000.0f, .1f, "", 2, 0xFFC0C0B0);
    GUI_Init_Rotary(&sldKnob[e_EQ1A], *audioProcessor.Setting[e_EQ1A], -12.0f, 12.0f, .1f, "", 2, 0xFFC0C0B0);
    GUI_Init_Rotary(&sldKnob[e_EQ1Q], *audioProcessor.Setting[e_EQ1Q], .1f, 2.0f, .1f, "", 2, 0xFFC0C0B0);
    GUI_Init_Rotary(&sldKnob[e_EQ2F], *audioProcessor.Setting[e_EQ2F], 200.0f, 2500.0f, .1f, "", 2, 0xFFC0C0B0);
    GUI_Init_Rotary(&sldKnob[e_EQ2A], *audioProcessor.Setting[e_EQ2A], -12.0f, 12.0f, .1f, "", 2, 0xFFC0C0B0);
    GUI_Init_Rotary(&sldKnob[e_EQ2Q], *audioProcessor.Setting[e_EQ2Q], .1f, 2.0f, .1f, "", 2, 0xFFC0C0B0);
    GUI_Init_Rotary(&sldKnob[e_EQ3F], *audioProcessor.Setting[e_EQ3F], 500.0f, 15000.0f, .1f, "", 2, 0xFFC0C0B0);
    GUI_Init_Rotary(&sldKnob[e_EQ3A], *audioProcessor.Setting[e_EQ3A], -12.0f, 12.0f, .1f, "", 2, 0xFFC0C0B0);
    GUI_Init_Rotary(&sldKnob[e_EQ3Q], *audioProcessor.Setting[e_EQ3Q], .1f, 2.0f, .1f, "", 2, 0xFFC0C0B0);
    
    GUI_Init_Rotary(&sldKnob[e_HighCut], *audioProcessor.Setting[e_HighCut], 1000, 10000, 100, "", 2, 0xFFC0C0B0);
    GUI_Init_Rotary(&sldKnob[e_LowCut], *audioProcessor.Setting[e_LowCut], 20, 300, 10, "", 2, 0xFFC0C0B0);

    GUI_Init_Rotary(&sldKnob[e_IR1off], *audioProcessor.Setting[e_IR1off], -100, 100, 1, "", 2, 0xFFC0C0B0);
    GUI_Init_Rotary(&sldKnob[e_IR2off], *audioProcessor.Setting[e_IR2off], -100, 100, 1, "", 2, 0xFFC0C0B0);
    GUI_Init_Rotary(&sldKnob[e_IR1Vol], *audioProcessor.Setting[e_IR1Vol], 0.0f, 1.0f, .01f, "", 2, 0xFFC0C0B0);
    GUI_Init_Rotary(&sldKnob[e_IR2Vol], *audioProcessor.Setting[e_IR2Vol], 0.0f, 1.0f, .01f, "", 2, 0xFFC0C0B0);
    
    GUI_Init_Slider(&sldKnob[e_IRF1], *audioProcessor.Setting[e_IRF1], -1.0f, 1.0f, .1f, "");
    GUI_Init_Slider(&sldKnob[e_IRF2], *audioProcessor.Setting[e_IRF2], -1.0f, 1.0f, .1f, "");
    GUI_Init_Slider(&sldKnob[e_IRF3], *audioProcessor.Setting[e_IRF3], -1.0f, 1.0f, .1f, "");
    GUI_Init_Slider(&sldKnob[e_IRF4], *audioProcessor.Setting[e_IRF4], -1.0f, 1.0f, .1f, "");
    GUI_Init_Slider(&sldKnob[e_IRF5], *audioProcessor.Setting[e_IRF5], -1.0f, 1.0f, .1f, "");
    GUI_Init_Slider(&sldKnob[e_IRF6], *audioProcessor.Setting[e_IRF6], -1.0f, 1.0f, .1f, "");
    GUI_Init_Slider(&sldKnob[e_IRF7], *audioProcessor.Setting[e_IRF7], -1.0f, 1.0f, .1f, "");
    GUI_Init_Slider(&sldKnob[e_IRF8], *audioProcessor.Setting[e_IRF8], -1.0f, 1.0f, .1f, "");
    GUI_Init_Slider(&sldKnob[e_IRF9], *audioProcessor.Setting[e_IRF9], -1.0f, 1.0f, .1f, "");
    GUI_Init_Slider(&sldKnob[e_IRF10], *audioProcessor.Setting[e_IRF10], -1.0f, 1.0f, .1f, "");
    GUI_Init_Slider(&sldKnob[e_IRF11], *audioProcessor.Setting[e_IRF11], -1.0f, 1.0f, .1f, "");
    GUI_Init_Slider(&sldKnob[e_IRF12], *audioProcessor.Setting[e_IRF12], -1.0f, 1.0f, .1f, "");
    GUI_Init_Slider(&sldKnob[e_IRF13], *audioProcessor.Setting[e_IRF13], -1.0f, 1.0f, .1f, "");
    GUI_Init_Slider(&sldKnob[e_IRF14], *audioProcessor.Setting[e_IRF14], -1.0f, 1.0f, .1f, "");
    GUI_Init_Slider(&sldKnob[e_IRF15], *audioProcessor.Setting[e_IRF15], -1.0f, 1.0f, .1f, "");
    GUI_Init_Slider(&sldKnob[e_IRF16], *audioProcessor.Setting[e_IRF16], -1.0f, 1.0f, .1f, "");
    GUI_Init_Slider(&sldKnob[e_IRF17], *audioProcessor.Setting[e_IRF17], -1.0f, 1.0f, .1f, "");
    GUI_Init_Slider(&sldKnob[e_IRF18], *audioProcessor.Setting[e_IRF18], -1.0f, 1.0f, .1f, "");
    GUI_Init_Slider(&sldKnob[e_IRF19], *audioProcessor.Setting[e_IRF19], -1.0f, 1.0f, .1f, "");
    GUI_Init_Slider(&sldKnob[e_IRF20], *audioProcessor.Setting[e_IRF20], -1.0f, 1.0f, .1f, "");
    GUI_Init_Slider(&sldKnob[e_IRF21], *audioProcessor.Setting[e_IRF21], -1.0f, 1.0f, .1f, "");


    sldKnob[e_Gain].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000000));
    sldKnob[e_LowCut].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000000));
    sldKnob[e_HighCut].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000000));

    sldKnob[e_IR1off].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000000));
    sldKnob[e_IR1Vol].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000000));
    sldKnob[e_IR2Vol].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_IR2off].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));

    sldKnob[e_EQ1F].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_EQ1A].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000000));
    sldKnob[e_EQ1Q].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_EQ2F].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_EQ2A].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000000));
    sldKnob[e_EQ2Q].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_EQ3F].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_EQ3A].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000000));
    sldKnob[e_EQ3Q].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));

    //R1.00 Update the Look and Feel (Global colors) so drop down menu is the correct color. 
    getLookAndFeel().setColour(juce::DocumentWindow::backgroundColourId, juce::Colour(32, 32, 32));
    getLookAndFeel().setColour(juce::DocumentWindow::textColourId, juce::Colour(255, 255, 255));
    getLookAndFeel().setColour(juce::DialogWindow::backgroundColourId, juce::Colour(32, 32, 32));
    getLookAndFeel().setColour(juce::PopupMenu::backgroundColourId, juce::Colour(0, 0, 0));
    getLookAndFeel().setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(192, 0, 0));
    getLookAndFeel().setColour(juce::TextButton::buttonOnColourId, juce::Colour(192, 0, 0));
    getLookAndFeel().setColour(juce::TextButton::buttonColourId, juce::Colour(0, 0, 0));
    getLookAndFeel().setColour(juce::ComboBox::backgroundColourId, juce::Colour(0, 0, 0));
    getLookAndFeel().setColour(juce::ListBox::backgroundColourId, juce::Colour(32, 32, 32));
    getLookAndFeel().setColour(juce::Label::backgroundColourId, juce::Colour(32, 32, 32));


    //R1.00 Define our control positions to make drawing easier.
    KNOB_DefinePosition(e_LowCut, 30, 95, 50, 75, 0, "Lo Cut", "Hz");
    KNOB_DefinePosition(e_EQ1A, 85, 95, 50, 75, 0, "Ampl 1", " dB");
    KNOB_DefinePosition(e_EQ1F, 140, 95, 50, 75, 0, "Freq 1", " Hz");    
    KNOB_DefinePosition(e_EQ1Q, 195, 95, 50, 75, 0, "Q 1", "");
    KNOB_DefinePosition(e_EQ2A, 250, 95, 50, 75, 0, "Ampl 2", " dB");
    KNOB_DefinePosition(e_EQ2F, 305, 95, 50, 75, 0, "Freq 2", " Hz");    
    KNOB_DefinePosition(e_EQ2Q, 360, 95, 50, 75, 0, "Q 2", "");
    KNOB_DefinePosition(e_EQ3A, 415, 95, 50, 75, 0, "Ampl 3", " dB");
    KNOB_DefinePosition(e_EQ3F, 470, 95, 50, 75, 0, "Freq 3", " Hz");    
    KNOB_DefinePosition(e_EQ3Q, 525, 95, 50, 75, 0, "Q 3", "");
    KNOB_DefinePosition(e_HighCut, 580, 95, 50, 75, 0, "Hi Cut", "Hz");
    KNOB_DefinePosition(e_Gain, 635, 95, 50, 75, 0, "Gain", "");

    KNOB_DefinePosition(e_IR1Vol, 30, 390, 50, 80, 0, "IR1 Vol", "");
    KNOB_DefinePosition(e_IR1off, 80, 390, 50, 80, 0, "IR1 Dist", "");
    KNOB_DefinePosition(e_IR2Vol, 130, 390, 50, 80, 0, "IR2 Vol", "");
    KNOB_DefinePosition(e_IR2off, 180, 390, 50, 80, 0, "IR2 Dist", "");
    
    KNOB_DefinePosition(e_IRF1, 45, 185, 30, 190, 2, "50", "");
    KNOB_DefinePosition(e_IRF2, 75, 185, 30, 190, 2, "100", "");
    KNOB_DefinePosition(e_IRF3, 105, 185, 30, 190, 2, "200", "");
    KNOB_DefinePosition(e_IRF4, 135, 185, 30, 190, 2, "300", "");
    KNOB_DefinePosition(e_IRF5, 165, 185, 30, 190, 2, "400", "");
    KNOB_DefinePosition(e_IRF6, 195, 185, 30, 190, 2, "500", "");
    KNOB_DefinePosition(e_IRF7, 225, 185, 30, 190, 2, "600", "");
    KNOB_DefinePosition(e_IRF8, 255, 185, 30, 190, 2, "700", "");
    KNOB_DefinePosition(e_IRF9, 285, 185, 30, 190, 2, "800", "");
    KNOB_DefinePosition(e_IRF10, 315, 185, 30, 190, 2, "900", "");
    KNOB_DefinePosition(e_IRF11, 345, 185, 30, 190, 2, "1k", "");
    KNOB_DefinePosition(e_IRF12, 375, 185, 30, 190, 2, "1.25k", "");
    KNOB_DefinePosition(e_IRF13, 405, 185, 30, 190, 2, "1.5k", "");
    KNOB_DefinePosition(e_IRF14, 435, 185, 30, 190, 2, "2k", "");
    KNOB_DefinePosition(e_IRF15, 465, 185, 30, 190, 2, "3k", "");
    KNOB_DefinePosition(e_IRF16, 495, 185, 30, 190, 2, "4k", "");
    KNOB_DefinePosition(e_IRF17, 525, 185, 30, 190, 2, "5k", "");
    KNOB_DefinePosition(e_IRF18, 555, 185, 30, 190, 2, "6k", "");
    KNOB_DefinePosition(e_IRF19, 585, 185, 30, 190, 2, "7k", "");
    KNOB_DefinePosition(e_IRF20, 615, 185, 30, 190, 2, "8k", "");
    KNOB_DefinePosition(e_IRF21, 645, 185, 30, 190, 2, "12k", "");


    Knob_Cnt = 37;

    //R1.00 HELP label.
    labHelp.setJustificationType(juce::Justification::centredLeft);
    labHelp.setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
    labHelp.setColour(juce::Label::textColourId, juce::Colour(0xFFC0C0C0));
    labHelp.setColour(juce::Label::outlineColourId, juce::Colour(0x00000000));
    addAndMakeVisible(labHelp);
    labHelp.setText("IR Tweaker v1.00 7/2026", juce::dontSendNotification);

    //R1.00 Graphic EQ Adjustment label.
    labGEQ.setJustificationType(juce::Justification::centredRight);
    labGEQ.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    labGEQ.setColour(juce::Label::textColourId, juce::Colour(0xFFA0A0A0));
    labGEQ.setColour(juce::Label::outlineColourId, juce::Colour(0xFF606060));
    addAndMakeVisible(labGEQ);
    labGEQ.setVisible(false);

    //R1.00 Adjustment information label.
    labInfo1.setJustificationType(juce::Justification::centredRight);
    labInfo1.setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
    labInfo1.setColour(juce::Label::textColourId, juce::Colour(0xFFA0A0A0));
    labInfo1.setColour(juce::Label::outlineColourId, juce::Colour(0x00000000));
    addAndMakeVisible(labInfo1);
    
    //R1.00 Clipping label.
    labInfo2.setJustificationType(juce::Justification::centred);
    labInfo2.setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
    labInfo2.setColour(juce::Label::textColourId, juce::Colour(0xFFFF8000));
    labInfo2.setColour(juce::Label::outlineColourId, juce::Colour(0x00000000));
    addAndMakeVisible(labInfo2);

    //R1.00 Version label.
    labInfo4.setJustificationType(juce::Justification::centredLeft);
    labInfo4.setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
    labInfo4.setColour(juce::Label::textColourId, juce::Colour(0xFFA0A0A0));
    labInfo4.setColour(juce::Label::outlineColourId, juce::Colour(0x00000000));
    addAndMakeVisible(labInfo4);
    labInfo4.setText("v1.00", juce::dontSendNotification);
      
    //R1.00 Setup Save IR Pulse button.
    butClick.setButtonText("Save IR");
    butClick.setClickingTogglesState(false);
    butClick.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF202020));
    butClick.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFF2ECE2));
    butClick.addListener(this);
    addAndMakeVisible(butClick);

    //R1.00 Setup Load speaker IR button.
    butLoadIR1.setButtonText("Load IR 1");
    butLoadIR1.setClickingTogglesState(false);
    butLoadIR1.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF202020));
    butLoadIR1.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFF2ECE2));
    butLoadIR1.addListener(this);
    addAndMakeVisible(butLoadIR1);

    //R1.00 Setup Load speaker IR button.
    butLoadIR2.setButtonText("Load IR 2");
    butLoadIR2.setClickingTogglesState(false);
    butLoadIR2.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF202020));
    butLoadIR2.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFF2ECE2));
    butLoadIR2.addListener(this);
    addAndMakeVisible(butLoadIR2);

    //R1.00 Setup Load speaker IR button.
    butEQReset.setButtonText("Reset EQ");
    butEQReset.setClickingTogglesState(false);
    butEQReset.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF202020));
    butEQReset.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFF2ECE2));
    butEQReset.addListener(this);
    addAndMakeVisible(butEQReset);

    //R1.00 Invert the IR.
    butInvIR1.setButtonText("Invert");
    butInvIR1.setClickingTogglesState(false);
    butInvIR1.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF202020));
    butInvIR1.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFF2ECE2));
    butInvIR1.addListener(this);
    addAndMakeVisible(butInvIR1);

    //R1.00 Invert the IR.
    butInvIR2.setButtonText("Invert");
    butInvIR2.setClickingTogglesState(false);
    butInvIR2.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF202020));
    butInvIR2.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFF2ECE2));
    butInvIR2.addListener(this);
    addAndMakeVisible(butInvIR2);

    //R1.00 auto align IR1.
    butAutoIR1.setButtonText("Auto");
    butAutoIR1.setClickingTogglesState(false);
    butAutoIR1.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF202020));
    butAutoIR1.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFF2ECE2));
    butAutoIR1.addListener(this);
    addAndMakeVisible(butAutoIR1);

    //R1.00 auto align IR2.
    butAutoIR2.setButtonText("Auto");
    butAutoIR2.setClickingTogglesState(false);
    butAutoIR2.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF202020));
    butAutoIR2.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFF2ECE2));
    butAutoIR2.addListener(this);
    addAndMakeVisible(butAutoIR2);

    //R1.00 IR1 Path viewer.
    butPathIR1.setButtonText("?");
    butPathIR1.setClickingTogglesState(false);
    butPathIR1.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF202020));
    butPathIR1.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFF2ECE2));
    butPathIR1.addListener(this);
    addAndMakeVisible(butPathIR1);

    //R1.00 IR2 Path viewer.
    butPathIR2.setButtonText("?");
    butPathIR2.setClickingTogglesState(false);
    butPathIR2.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF202020));
    butPathIR2.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFF2ECE2));
    butPathIR2.addListener(this);
    addAndMakeVisible(butPathIR2);

    //R1.00 Create SLIDER ATTACHMENTS so our parameter vars get adjusted automatically for Get/Set states.
    ParAtt[e_Gain] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "gain", sldKnob[e_Gain]);
    ParAtt[e_IR1off] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "ir1off", sldKnob[e_IR1off]);
    ParAtt[e_IR2off] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "ir2off", sldKnob[e_IR2off]);
    ParAtt[e_IR1Vol] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "ir1vol", sldKnob[e_IR1Vol]);
    ParAtt[e_IR2Vol] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "ir2vol", sldKnob[e_IR2Vol]);
    ParAtt[e_EQ1F] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "eq1f", sldKnob[e_EQ1F]);
    ParAtt[e_EQ1A] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "eq1a", sldKnob[e_EQ1A]);
    ParAtt[e_EQ1Q] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "eq1q", sldKnob[e_EQ1Q]);
    ParAtt[e_EQ2F] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "eq2f", sldKnob[e_EQ2F]);
    ParAtt[e_EQ2A] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "eq2a", sldKnob[e_EQ2A]);
    ParAtt[e_EQ2Q] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "eq2q", sldKnob[e_EQ2Q]);
    ParAtt[e_EQ3F] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "eq3f", sldKnob[e_EQ3F]);
    ParAtt[e_EQ3A] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "eq3a", sldKnob[e_EQ3A]);
    ParAtt[e_EQ3Q] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "eq3q", sldKnob[e_EQ3Q]);

    ParAtt[e_HighCut] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "highcut", sldKnob[e_HighCut]);
    ParAtt[e_LowCut] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "lowcut", sldKnob[e_LowCut]);
    
    ParAtt[e_IRF1] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "irf1", sldKnob[e_IRF1]);
    ParAtt[e_IRF2] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "irf2", sldKnob[e_IRF2]);
    ParAtt[e_IRF3] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "irf3", sldKnob[e_IRF3]);
    ParAtt[e_IRF4] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "irf4", sldKnob[e_IRF4]);
    ParAtt[e_IRF5] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "irf5", sldKnob[e_IRF5]);
    ParAtt[e_IRF6] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "irf6", sldKnob[e_IRF6]);
    ParAtt[e_IRF7] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "irf7", sldKnob[e_IRF7]);
    ParAtt[e_IRF8] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "irf8", sldKnob[e_IRF8]);
    ParAtt[e_IRF9] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "irf9", sldKnob[e_IRF9]);
    ParAtt[e_IRF10] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "irf10", sldKnob[e_IRF10]);
    ParAtt[e_IRF11] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "irf11", sldKnob[e_IRF11]);
    ParAtt[e_IRF12] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "irf12", sldKnob[e_IRF12]);
    ParAtt[e_IRF13] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "irf13", sldKnob[e_IRF13]);
    ParAtt[e_IRF14] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "irf14", sldKnob[e_IRF14]);
    ParAtt[e_IRF15] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "irf15", sldKnob[e_IRF15]);
    ParAtt[e_IRF16] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "irf16", sldKnob[e_IRF16]);
    ParAtt[e_IRF17] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "irf17", sldKnob[e_IRF17]);
    ParAtt[e_IRF18] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "irf18", sldKnob[e_IRF18]);
    ParAtt[e_IRF19] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "irf19", sldKnob[e_IRF19]);
    ParAtt[e_IRF20] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "irf20", sldKnob[e_IRF20]);
    ParAtt[e_IRF21] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "irf21", sldKnob[e_IRF21]);

    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    //R1.00 Set the window size.
    setSize(GUI_Width, GUI_Height);
}

MakoBiteAudioProcessorEditor::~MakoBiteAudioProcessorEditor()
{
}

//R1.00 This func gets called 10 times a second.
//R1.00 Could be used to draw VU meters, etc.
//R1.00 Refreshing at a slower rate may make the UI feel clunky.
void MakoBiteAudioProcessorEditor::timerCallback()
{

    //R1.00 We have fresh IR data that needs drawn. 
    if (audioProcessor.IR_RefreshUI)
    {
        audioProcessor.IR_RefreshUI = false;

        //R1.00 If we have an error, post it and then clear it.
        if (audioProcessor.ERR_Message1 != "")
        {
            labHelp.setText(audioProcessor.ERR_Message1, juce::sendNotification);
            audioProcessor.ERR_Message1 = "";
        }

        repaint();
    }

    //R1.00 Check processor if audio is clipping.
    //R1.00 Track the Label stats so we are not redrawing the control twice a second.
    if (audioProcessor.AudioIsClipping)
    {
        audioProcessor.AudioIsClipping = false;
        STATE_Clip = true;
        labInfo2.setText("CLIP", juce::sendNotification);        
    }
    else
    {
        if (STATE_Clip) labInfo2.setText("", juce::sendNotification);        
        STATE_Clip = false;
    }

    //R1.00 Show the current sample rate.
    if (SampleRate != audioProcessor.SampleRate)
    {
        SampleRate = audioProcessor.SampleRate;
        switch (int(SampleRate))
        {
        case 44100: labInfo4.setText("v1.00  44.1 k", juce::sendNotification); break;
        case 48000: labInfo4.setText("v1.00  48 k", juce::sendNotification); break;
        case 96000: labInfo4.setText("v1.00  96 k", juce::sendNotification); break;
        case 192000: labInfo4.setText("v1.00  192 k", juce::sendNotification); break;
        default: labInfo4.setText("v1.00", juce::sendNotification); break;
        }
    }

    //R1.00 User wants to save the current IR pulse data.
    if (audioProcessor.Pulse_Complete)
    {
        audioProcessor.Pulse_Complete = false;

        dlgFile = std::make_unique<juce::FileChooser>("Save the IR wave file", SetupPath_Current, "*.wav");
        auto chooserFlags = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles;

        dlgFile->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file != juce::File{}) FILE_WriteWave(file.getFullPathName());
                repaint();
            });        
    }

    //R1.00 See if we have a new IR position from Processor.
    if (-999 < audioProcessor.IR1_AutoNewPosition)
    {
        sldKnob[e_IR1off].setValue(audioProcessor.IR1_AutoNewPosition, juce::sendNotification);
        audioProcessor.IR1_AutoNewPosition = -999;
        repaint();
    }

    //R1.00 See if we have a new IR position from Processor.
    if (-999 < audioProcessor.IR2_AutoNewPosition)
    {
        sldKnob[e_IR2off].setValue(audioProcessor.IR2_AutoNewPosition, juce::sendNotification);
        audioProcessor.IR2_AutoNewPosition = -999;
        repaint();
    }

}

//==============================================================================
void MakoBiteAudioProcessorEditor::paint (juce::Graphics& g)
{   
    
            
    //JUCE (Our component is opaque, so we must completely fill the background with a solid colour)
    //JUCE g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    /*
    //R1.00 Draw our GUI.
    //R1.00 Background.
    juce::ColourGradient ColGrad;
    ColGrad = juce::ColourGradient(juce::Colour(0xFF808080), 0.0f, 0.0f, juce::Colour(0xFF404040), 0.0f, GUI_Height, false);
    g.setGradientFill(ColGrad);        
    g.fillRect(0, 0, GUI_Width, 11);
    g.fillRect(0, 10, 15, GUI_Height);
    g.fillRect(GUI_Width - 15, 9, 15, GUI_Height);
    g.fillRect(0, GUI_Height - 10, GUI_Width, 10);

    //R1.00 Blue panel.
    g.setColour(juce::Colour::Colour(0xFFB0B0B0));
    g.fillRect(17.0f, 12.0f, GUI_Width - 33.0f, GUI_Height - 22.0f);

    //R1.00 Draw panel edge.
    ColGrad = juce::ColourGradient(juce::Colour(0xFF202020), 0.0f, 0.0f, juce::Colour(0xFF808080), 0.0f, GUI_Height, false);
    g.setGradientFill(ColGrad);
    g.drawRoundedRectangle(15.0f, 10.0f, GUI_Width - 30.0f, GUI_Height - 20.0f, 4.0f, 2.0f);

    ColGrad = juce::ColourGradient(juce::Colour(0xFFC0C0C0), 0.0f, 0.0f, juce::Colour(0xFF000000), 0.0f, GUI_Height, false);
    g.setGradientFill(ColGrad);
    g.drawRoundedRectangle(17.0f, 12.0f, GUI_Width - 34.0f, GUI_Height - 24.0f, 4.0f, 2.0f);

    //R1.00 Draw EQ Rectangle.        
    ColGrad = juce::ColourGradient(juce::Colour(0xFFE0E0E0), 0.0f, 175.0f, juce::Colour(0xFFA0A0A0), 0.0f, 375.0f, false);
    g.setGradientFill(ColGrad);
    g.fillRoundedRectangle(30.0f, 175.0f, GUI_Width - 60.0f, 200.0f, 4.0f);
        
    g.setColour(juce::Colour::Colour(0xFF404040));
    g.drawLine(60.0f, 280.0f, 660.0f, 280.0f);
    g.setColour(juce::Colour::Colour(0xFF909090));
    g.drawLine(60.0f, 202.0f, 660.0f, 202.0f);
    g.drawLine(60.0f, 240.0f, 660.0f, 240.0f);
    g.drawLine(60.0f, 320.0f, 660.0f, 320.0f);
    g.drawLine(60.0f, 358.0f, 660.0f, 358.0f);        
    */

    //R1.00 Draw LOGO.
    g.drawImageAt(imgLogo, 0, 0);

    //R1.00 DRAW TEXT.
    //R1.00 Most of these could be done on the image to speed up painting.
    //R1.00 But the EQ frequencies need to update. So we are doing all the text
    //R1.00 here so the font matches no matter what in the future.
    /*
    g.setFont(12.0f);
    g.setColour(colBone);
    for (int t = 0; t < e_IRF1; t++)
    {
        g.drawFittedText(Knob_Info[t].Name, Knob_Info[t].x, Knob_Info[t].y - 10, Knob_Info[t].sizex, 15, juce::Justification::centred, 1);
    }
    g.setColour(juce::Colour::Colour(0xFF000000));
    for (int t = e_IRF1; t < Knob_Cnt; t++)
    {
        g.drawFittedText(Knob_Info[t].Name, Knob_Info[t].x, Knob_Info[t].y - 10, Knob_Info[t].sizex, 15, juce::Justification::centred, 1);
    }
    */
       
      

    //R1.00 Draw the On/Off Toggle Switchs.
    g.setColour(colBone);
    //g.drawText("Use IR", 600, 19, 60, 15, juce::Justification::left, 1);
    //g.drawText("Use EQ", 600, 39, 60, 15, juce::Justification::left, 1);
    //g.drawText("Use GEQ", 600, 59, 60, 15, juce::Justification::left, 1);
    if (audioProcessor.USE_IR)
        g.drawImageAt(imgSlideOn, rcKillIR.x1, rcKillIR.y1);
    else
        g.drawImageAt(imgSlideOff, rcKillIR.x1, rcKillIR.y1);
    
    if (audioProcessor.USE_EQ)
        g.drawImageAt(imgSlideOn, rcKillEQ.x1, rcKillEQ.y1);
    else
        g.drawImageAt(imgSlideOff, rcKillEQ.x1, rcKillEQ.y1);

    if (audioProcessor.USE_GEQ)
        g.drawImageAt(imgSlideOn, rcKillGEQ.x1, rcKillGEQ.y1);
    else
        g.drawImageAt(imgSlideOff, rcKillGEQ.x1, rcKillGEQ.y1);


    //R1.00 The IR1 and IR2 indicators.
    g.setColour(juce::Colour(0xFFFF8000)); g.fillRect(342, 383, 5, 15);
    g.setColour(juce::Colour(0xFF0080FF)); g.fillRect(342, 403, 5, 15);

    //R1.00 Show the current IR files we are using.        
    g.setColour(colBone);
    g.drawFittedText(audioProcessor.FileOnly_IRSpeaker1, 375, 382, 650, 15, juce::Justification::left, 1);
    g.drawFittedText(audioProcessor.FileOnly_IRSpeaker2, 375, 402, 650, 15, juce::Justification::left, 1);

    // *********************************************
    //R1.00 Draw the IR traces.
    // *********************************************
    int Xst = 240;
    g.setColour(juce::Colour(0xFF000000));
    g.fillRect(Xst, 430, 200, 40);
    g.fillRect(Xst + 210, 430, 200, 40);

    juce::Path pathTrace;
    //R1.00 Draw IR1.
    g.setColour(juce::Colour(0xFFFF8000));
    pathTrace.startNewSubPath(Xst, 450); for (int t = 0; t < 100; t++)  pathTrace.lineTo(Xst + (t * 2), int(450 - (audioProcessor.IR_Spkr1[t] * 20.0f))); g.strokePath(pathTrace, juce::PathStrokeType(1.0f));
    pathTrace.clear(); 

    //R1.00 Draw IR2.
    g.setColour(juce::Colour(0xFF0080FF));
    pathTrace.startNewSubPath(Xst, 450); for (int t = 0; t < 100; t++)  pathTrace.lineTo(Xst + (t * 2), int(450 - (audioProcessor.IR_Spkr2[t] * 20.0f))); g.strokePath(pathTrace, juce::PathStrokeType(1.0f));
    pathTrace.clear(); 

    //R1.00 Draw IR_Final.
    g.setColour(colBone);
    pathTrace.startNewSubPath(Xst + 210, 450); for (int t = 0; t < 100; t++)  pathTrace.lineTo(Xst + 210 + (t * 2), int(450 - (audioProcessor.IR_Final[t] * 20.0f))); g.strokePath(pathTrace, juce::PathStrokeType(1.0f));
    pathTrace.clear();

}

void MakoBiteAudioProcessorEditor::buttonClicked(juce::Button* butt)
{   
    auto buttstate = butt->getState();

    //R1.00 Tell the processor to Start the IR Pulse.
    if (butt == &butClick)
    {
        audioProcessor.Pulse_On = true;
        HELP_Post("PULSE: Send an impulse thru the IRs, filters, and save.",-1);
    }

    //R1.00 Load an Speaker IR.
    if (butt == &butLoadIR1)
    {
        dlgFile = std::make_unique<juce::FileChooser>("Load a speaker IR wave file", SetupPath_Current, "*.wav");
        auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

        dlgFile->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file != juce::File{}) audioProcessor.File_IRSpeaker1 = file.getFullPathName(); audioProcessor.File_LoadNewIR1 = true; audioProcessor.SettingsChanged = true;
                repaint();
            });
    }

    //R1.00 Load an Speaker IR.
    if (butt == &butLoadIR2)
    {
        dlgFile = std::make_unique<juce::FileChooser>("Load a speaker IR wave file", SetupPath_Current, "*.wav");
        auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

        dlgFile->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file != juce::File{}) audioProcessor.File_IRSpeaker2 = file.getFullPathName(); audioProcessor.File_LoadNewIR2 = true; audioProcessor.SettingsChanged = true;
                repaint();
            });
    }

    
    //R1.00 Set all EQs to 0.
    if (butt == &butEQReset)
    {
        sldKnob[e_LowCut].setValue(20.0, juce::sendNotification);
        sldKnob[e_HighCut].setValue(10000.0, juce::sendNotification);
        sldKnob[e_EQ1F].setValue(120.0f, juce::sendNotification);
        sldKnob[e_EQ1A].setValue(0.0f, juce::sendNotification);
        sldKnob[e_EQ1Q].setValue(0.707f, juce::sendNotification);
        sldKnob[e_EQ2F].setValue(700.0f, juce::sendNotification);
        sldKnob[e_EQ2A].setValue(0.0f, juce::sendNotification);
        sldKnob[e_EQ2Q].setValue(0.35f, juce::sendNotification);
        sldKnob[e_EQ3F].setValue(2000.0f, juce::sendNotification);
        sldKnob[e_EQ3A].setValue(0.0f, juce::sendNotification);
        sldKnob[e_EQ3Q].setValue(0.35f, juce::sendNotification);

        for (int t = 0; t < (1 + (e_IRF21 - e_IRF1)); t++) sldKnob[e_IRF1 + t].setValue(0.0, juce::sendNotification);
        audioProcessor.SettingsChanged = true;

        HELP_Post("RESET: Set all EQs to their default states.", -1);
    }

    
    //R1.00 Invert IR 1.
    if (butt == &butInvIR1)
    {
        audioProcessor.IR1_Invert = true;
        audioProcessor.SettingsChanged = true;
        HELP_Post("INVERT: Flip the IR 1 polarity.",-1);
    }
    
    //R1.00 Invert IR 2.
    if (butt == &butInvIR2)
    {
        audioProcessor.IR2_Invert = true;
        audioProcessor.SettingsChanged = true;
        HELP_Post("INVERT: Flip the IR 2 polarity.", -1);
    }

    //R1.00 Auto align IR 1.
    if (butt == &butAutoIR1)
    {
        audioProcessor.IR1_Auto = true;
        audioProcessor.SettingsChanged = true;
        HELP_Post("AUTO DISTANCE ALIGN 1: Force IR 1 to match IR 2.", -1);
    }

    //R1.00 Auto align IR 2.
    if (butt == &butAutoIR2)
    {
        audioProcessor.IR2_Auto = true;
        audioProcessor.SettingsChanged = true;
        HELP_Post("AUTO DISTANCE ALIGN 2: Force IR 2 to match IR 1.", -1);
    }

    //R1.00 View IR 1 path.
    if (butt == &butPathIR1) HELP_Post("IR1 Path: " + audioProcessor.File_IRSpeaker1, -1);
    
    //R1.00 View IR 2 path.
    if (butt == &butPathIR2) HELP_Post("IR2 Path: " + audioProcessor.File_IRSpeaker2, -1);

}

void MakoBiteAudioProcessorEditor::mouseDown(const juce::MouseEvent& e)
{

    //R2.00 This gets called twice for every click. 
    //R2.00 Do not use the 2nd call or toggles will not work.
    if (MOUSEHACK_IsDown)
    {
        MOUSEHACK_IsDown = false;
        return;
    }
    MOUSEHACK_IsDown = true;

    std::string tMess = "";

    int mButton = 2;
    if (e.mods.isLeftButtonDown()) mButton = 1;
    if (e.mods.isMiddleButtonDown()) mButton = 3;
    int posX = e.getPosition().x;
    int posY = e.getPosition().y;

    MOUSE_Down = mButton;

    //R3.10 Check valid click.
    if (e.eventComponent->getBoundsInParent().getWidth() < GUI_Width) return;

    if ((rcKillIR.x1 < posX) && (posX < rcKillIR.x2))
    {
        if ((rcKillIR.y1 < posY) && (posY < rcKillIR.y2))
        {
            if (audioProcessor.USE_IR)
                audioProcessor.USE_IR = false;            
            else
                audioProcessor.USE_IR = true;
            
            HELP_Post("Toggle the IR on/off.", -1);
        }
        if ((rcKillEQ.y1 < posY) && (posY < rcKillEQ.y2))
        {
            if (audioProcessor.USE_EQ)
                audioProcessor.USE_EQ = false;
            else
                audioProcessor.USE_EQ = true;

            HELP_Post("Toggle the EQ on/off.", -1);
        }
        if ((rcKillGEQ.y1 < posY) && (posY < rcKillGEQ.y2))
        {
            if (audioProcessor.USE_GEQ)
                audioProcessor.USE_GEQ = false;
            else
                audioProcessor.USE_GEQ = true;

            HELP_Post("Toggle the Graphic EQ on/off.", -1);
        }
        repaint();
    }

}

void MakoBiteAudioProcessorEditor::FILE_WriteWave(juce::String tFile)
{
    juce::File file(tFile);
    juce::WavAudioFormat format;
    std::unique_ptr<juce::AudioFormatWriter> writer;

    //R1.00 Delete the file if it exists.
    if (file.exists()) file.deleteFile();

    //R1.00 Create the file Writer.
    writer.reset(format.createWriterFor(new juce::FileOutputStream(file), 48000.0, 1, 24, {}, 0));

    //R1.00 Create an audio buffer to store the audio in temporarily.
    juce::AudioBuffer<float> buffer = juce::AudioBuffer<float>::AudioBuffer(1, 2080);

    //R1.00 Write our audio data into a temp buffer so it is the right type/format.
    auto* channelData = buffer.getWritePointer(0);
    for (int t = 0; t < 2080; t++) channelData[t] = audioProcessor.Wave_Temp[t];

    //R1.00 Write our buffer data into the file.
    if (writer != nullptr) writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());

    //R1.00 Close the file?
    writer.reset();    
    //writer.release();

    return;
}


void MakoBiteAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    //R1.00 Use RETURN here to have a blank window drawn when creating a GUI image.
    //R1.00 The controls are only visible if their BOUNDS are defined.
    //return;

    //R1.00 Draw all of the defined KNOBS.
    for (int t = 0; t < Knob_Cnt; t++) sldKnob[t].setBounds(Knob_Info[t].x, Knob_Info[t].y, Knob_Info[t].sizex, Knob_Info[t].sizey);

    //R1.00 Information labels. 
    labInfo4.setBounds(183, 47, 80, 15);    
    labInfo2.setBounds(415, 47, 40, 15);   //R1.00 Clipping
    labInfo1.setBounds(423, 47, 105, 15);

    butLoadIR1.setBounds(30, 18, 65, 20);
    butLoadIR2.setBounds(30, 44, 65, 20);
    butClick.setBounds(  102, 44, 65, 20);    
    butEQReset.setBounds(102, 18, 65, 20);
    
    butInvIR1.setBounds(240, 380, 40, 20);
    butInvIR2.setBounds(240, 400, 40, 20);

    butAutoIR1.setBounds(282, 380, 40, 20);
    butAutoIR2.setBounds(282, 400, 40, 20);

    butPathIR1.setBounds(324, 380, 15, 20);
    butPathIR2.setBounds(324, 400, 15, 20);

    labHelp.setBounds(30, 475, 660, 17);
    
}


//R1.00 Setup the KNOB control edit values, Text Suffix (if any), UI tick marks, and Indicator Color.
void MakoBiteAudioProcessorEditor::GUI_Init_Rotary(juce::Slider* slider, float Val, float Vmin, float Vmax, float Vinterval, juce::String Suffix, int TickStyle, int ThumbColor)
{
    //R1.00 Setup the slider edit parameters.
    slider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    slider->setTextValueSuffix(Suffix);
    slider->setRange(Vmin, Vmax, Vinterval);
    slider->setValue(Val);
    slider->addListener(this);
    addAndMakeVisible(slider);

    //R1.00 Override the default Juce drawing routines and use ours.
    slider->setLookAndFeel(&myLookAndFeel);

    //R1.00 Setup the type and colors for the sliders.
    slider->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider->setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xFFE0E0E0));
    slider->setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xFF000000));
    slider->setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xFF000000));
    slider->setColour(juce::Slider::textBoxHighlightColourId, juce::Colour(0xFF804000));
    slider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0x00000000));    //R1.00 Make this SEE THRU. Alpha=0.
    slider->setColour(juce::Slider::thumbColourId, juce::Colour(ThumbColor));

    //R1.00 Cheat: We are using this color as a Tick Mark style selector in our drawing function.
    slider->setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(TickStyle));
}

//R1.00 Setup the SLIDER control edit values, Text Suffix (if any), UI tick marks, and Indicator Color.
void MakoBiteAudioProcessorEditor::GUI_Init_Slider(juce::Slider* slider, float Val, float Vmin, float Vmax, float Vinterval, juce::String Suffix)
{
    //R1.00 Setup the slider edit parameters.
    slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 40);
    slider->setRange(Vmin, Vmax, Vinterval);
    slider->setTextValueSuffix(Suffix);
    slider->setValue(Val);
    slider->addListener(this);
    addAndMakeVisible(slider);

    //R1.00 Setup the type and colors for the sliders.
    slider->setSliderStyle(juce::Slider::LinearVertical);
    slider->setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xFFA0A0A0));
    slider->setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xFF202020));
    slider->setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xFF000000));
    slider->setColour(juce::Slider::textBoxHighlightColourId, juce::Colour(0xFF000000));
    slider->setColour(juce::Slider::trackColourId, juce::Colour(0xFFC06000));
    slider->setColour(juce::Slider::backgroundColourId, juce::Colour(0xFF000000));
    slider->setColour(juce::Slider::thumbColourId, juce::Colour(0xFFC0C0B0)); //E02020

    //R1.00 Override the default Juce drawing routines and use ours.
    slider->setLookAndFeel(&myLookAndFeel);

}

//R1.00 Function to make declarations easier.
void MakoBiteAudioProcessorEditor::KNOB_DefinePosition(int idx,float x, float y, float sizex, float sizey, int datatype, juce::String name, juce::String suffix)
{
    Knob_Info[idx].x = x;
    Knob_Info[idx].y = y;
    Knob_Info[idx].sizex = sizex;
    Knob_Info[idx].sizey = sizey;
    Knob_Info[idx].DataType = datatype;
    Knob_Info[idx].Suffix = suffix;
    Knob_Info[idx].Name = name;
}

//R1.00 Function to format our SLIDER/KNOB settings to print on the UI (labInfo1).
void MakoBiteAudioProcessorEditor::KNOB_ShowValue(int t)
{
    char Mess[40];
    juce::String Mess2;
    
    switch (Knob_Info[t].DataType)
    {
      case 0: //R1.00 Generic FLOAT value.
        sprintf(Mess, "%1.2f", float(sldKnob[t].getValue()));
        labInfo1.setText(Mess + Knob_Info[t].Suffix, juce::dontSendNotification);
        break;

      case 1: //R1.00 Generic INT value.
         Mess2 = std::to_string(int(sldKnob[t].getValue()));
         labInfo1.setText(Mess2 + Knob_Info[t].Suffix, juce::sendNotification);
         break;

      case 2:  //R1.00 Print IRF value with 1 decimal place.
        sprintf(Mess, "%2.1f dB", float(sldKnob[t].getValue() * 12.0f));
        labInfo1.setText(Mess + Knob_Info[t].Suffix, juce::dontSendNotification);
        break;
    
      case 3: //R1.00 Special formatting for HIGH CUT. 
          if (sldKnob[t].getValue() < 8000.0f)
          {
              Mess2 = std::to_string(int(sldKnob[t].getValue()));
              labInfo1.setText(Mess2 + Knob_Info[t].Suffix, juce::sendNotification);
          }
          else
              labInfo1.setText("HC off", juce::sendNotification);
          break;
          
      case 4: //R1.00
          
          break;

      
      
      case 9: //R1.00 Special formatting for LOW CUT. 
          if (20.0f < sldKnob[t].getValue())
          {
              Mess2 = std::to_string(int(sldKnob[t].getValue()));
              labInfo1.setText(Mess2 + Knob_Info[t].Suffix, juce::sendNotification);
          }
          else
              labInfo1.setText("LC off", juce::sendNotification);
          break;
    }
}

//R1.00 Override SLIDER control to capture setting changes.
void MakoBiteAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{       
    float x;
    float y;
    float tVal;
    char Mess[40];
       
    //R1.00 When a slider is adjusted, this func gets called. Capture the new edits
    //R1.00  and flag the processor when it needs to recalc things.
    //R1.00 Check which slider has been adjusted.
    for (int t = 0; t < Knob_Cnt; t++)
    {
        if (slider == &sldKnob[t])
        {   
            //R1.00 We need to update settings in processor.
            //R1.00 Increment changed var to be sure every change gets made. Changed var is decremented in processor.
            audioProcessor.SettingsChanged += 1;

            //R1.00 Print the Graphic EQ values to the UI.
            if ((e_IRF1 <= t) && (t <= e_IRF21)) GEQ_LabelUpdate(t);

            //R1.00 We have IR modifier changes needed.
            if ((t == e_IR1Vol) || (t == e_IR2Vol)) audioProcessor.MIX_Update = true;
            if (t == e_IR1off) audioProcessor.Offset1_Update = true;
            if (t == e_IR2off) audioProcessor.Offset2_Update = true;

            //R1.00 Post help for this Slider.
            HELP_Post(LangStringHelp[t], t);
            
            //R1.00 We have captured the correct slider change, exit this function.
            return;
        }
    }
    
    return;
}

void MakoBiteAudioProcessorEditor::GEQ_LabelUpdate(int idx)
{
    float x;
    float y;
    float tVal;
    char Mess[40];

    x = sldKnob[idx].getPosition().x;
    y = 269 - sldKnob[idx].getValue() * 80;
    tVal = float(sldKnob[idx].getValue());

    sprintf(Mess, "%1.2f dB", float(sldKnob[idx].getValue()) * 12.0f);
    labInfo1.setText(Mess, juce::dontSendNotification);
    labGEQ.setText(Mess, juce::dontSendNotification);
    if (idx < e_IRF19)
        labGEQ.setBounds(x + 30, y, 60, 15);
    else
        labGEQ.setBounds(x - 58, y, 60, 15);
}


void MakoBiteAudioProcessorEditor::sliderDragStarted(juce::Slider* slider)
{
    //R1.00 If dragging a GEQ knob, show the value next to the knob.
    for (int t = e_IRF1; t < (e_IRF21+1); t++)
    {
        if (slider == &sldKnob[t])
        {
            GEQ_LabelUpdate(t);
            labGEQ.setVisible(true);
        }
    }
}

void MakoBiteAudioProcessorEditor::sliderDragEnded(juce::Slider* slider)
{
    //R1.00 Graphic EQ label is the only object we are changing during drags.
    labGEQ.setVisible(false);
}

void MakoBiteAudioProcessorEditor::HELP_Post(juce::String Message, int HelpIndex)
{
    //R1.00 Post a Help message. Index = -1 = Force post.
    if (HelpIndex < 0)
    {        
        labHelp.setText(Message, juce::sendNotification);
    }
    else if (HelpIndex != ctrlHelpLast)
    {
        ctrlHelpLast = HelpIndex;
        labHelp.setText(Message, juce::sendNotification);
    }

    //R1.00 Track the message so we dont repost over and over.
    ctrlHelpLast = HelpIndex;
}

