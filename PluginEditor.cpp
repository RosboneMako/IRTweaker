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

    //R1.10 Load any UI images from the resources file.
    imgBackground = juce::ImageCache::getFromMemory(BinaryData::irt_back_png, BinaryData::irt_back_pngSize);
    imgGraph = juce::ImageCache::getFromMemory(BinaryData::log_graph_01_png, BinaryData::log_graph_01_pngSize);

    imgUseIRoff = juce::ImageCache::getFromMemory(BinaryData::optIRoff_png, BinaryData::optIRoff_pngSize);
    imgUseIRon = juce::ImageCache::getFromMemory(BinaryData::optIRon_png, BinaryData::optIRon_pngSize);
    imgUseEQoff = juce::ImageCache::getFromMemory(BinaryData::optEQoff_png, BinaryData::optEQoff_pngSize);
    imgUseEQon = juce::ImageCache::getFromMemory(BinaryData::optEQon_png, BinaryData::optEQon_pngSize);
    imgUseGEQoff = juce::ImageCache::getFromMemory(BinaryData::optGEQoff_png, BinaryData::optGEQoff_pngSize);
    imgUseGEQon = juce::ImageCache::getFromMemory(BinaryData::optGEQon_png, BinaryData::optGEQon_pngSize);

    imgopt07off = juce::ImageCache::getFromMemory(BinaryData::opt07off_png, BinaryData::opt07off_pngSize);
    imgopt07on = juce::ImageCache::getFromMemory(BinaryData::opt07on_png, BinaryData::opt07on_pngSize);
    imgopt14off = juce::ImageCache::getFromMemory(BinaryData::opt14off_png, BinaryData::opt14off_pngSize);
    imgopt14on = juce::ImageCache::getFromMemory(BinaryData::opt14on_png, BinaryData::opt14on_pngSize);
    imgopt20off = juce::ImageCache::getFromMemory(BinaryData::opt20off_png, BinaryData::opt20off_pngSize);
    imgopt20on = juce::ImageCache::getFromMemory(BinaryData::opt20on_png, BinaryData::opt20on_pngSize);
    imgopt40off = juce::ImageCache::getFromMemory(BinaryData::opt40off_png, BinaryData::opt40off_pngSize);
    imgopt40on = juce::ImageCache::getFromMemory(BinaryData::opt40on_png, BinaryData::opt40on_pngSize);
    
    imgFFTon = juce::ImageCache::getFromMemory(BinaryData::optFFTon_png, BinaryData::optFFTon_pngSize);
    imgFFToff = juce::ImageCache::getFromMemory(BinaryData::optFFToff_png, BinaryData::optFFToff_pngSize);

    //R1.00 Start our Timer so we can tell the user they are clipping. Could draw VU Meters here, etc.
    startTimerHz(10);  //R1.00 have our Timer get called 10 times per second.

    //****************************************************************************************
    //R1.00 Add GUI CONTROLS
    //****************************************************************************************
    GUI_Init_Rotary(&sldKnob[e_Gain], *audioProcessor.Setting[e_Gain],0.0f, 1.0f,.01f,"", 2, 0xFFC0C0B0);
    
    GUI_Init_Rotary(&sldKnob[e_EQ1F], *audioProcessor.Setting[e_EQ1F], 20.0f, 1000.0f, .1f, "", 2, 0xFFC0C0B0);
    GUI_Init_Rotary(&sldKnob[e_EQ1A], *audioProcessor.Setting[e_EQ1A], -12.0f, 12.0f, .1f, "", 2, 0xFFC0C0B0);
    GUI_Init_Rotary(&sldKnob[e_EQ1Q], *audioProcessor.Setting[e_EQ1Q], .1f, 4.0f, .1f, "", 2, 0xFFC0C0B0);
    GUI_Init_Rotary(&sldKnob[e_EQ2F], *audioProcessor.Setting[e_EQ2F], 200.0f, 2500.0f, .1f, "", 2, 0xFFC0C0B0);
    GUI_Init_Rotary(&sldKnob[e_EQ2A], *audioProcessor.Setting[e_EQ2A], -12.0f, 12.0f, .1f, "", 2, 0xFFC0C0B0);
    GUI_Init_Rotary(&sldKnob[e_EQ2Q], *audioProcessor.Setting[e_EQ2Q], .1f, 4.0f, .1f, "", 2, 0xFFC0C0B0);
    GUI_Init_Rotary(&sldKnob[e_EQ3F], *audioProcessor.Setting[e_EQ3F], 500.0f, 15000.0f, .1f, "", 2, 0xFFC0C0B0);
    GUI_Init_Rotary(&sldKnob[e_EQ3A], *audioProcessor.Setting[e_EQ3A], -12.0f, 12.0f, .1f, "", 2, 0xFFC0C0B0);
    GUI_Init_Rotary(&sldKnob[e_EQ3Q], *audioProcessor.Setting[e_EQ3Q], .1f, 4.0f, .1f, "", 2, 0xFFC0C0B0);
    
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
    sldKnob[e_LowCut].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000002));
    sldKnob[e_HighCut].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000002));

    sldKnob[e_IR1off].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000000));
    sldKnob[e_IR1Vol].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000000));
    sldKnob[e_IR2Vol].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_IR2off].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));

    sldKnob[e_EQ1F].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_EQ1A].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_EQ1Q].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_EQ2F].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000000));
    sldKnob[e_EQ2A].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000000));
    sldKnob[e_EQ2Q].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000000));
    sldKnob[e_EQ3F].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_EQ3A].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
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
    KNOB_DefinePosition(e_EQ1F, 85, 95, 50, 75, 0, "Freq 1", " Hz");
    KNOB_DefinePosition(e_EQ1A, 140, 95, 50, 75, 0, "Ampl 1", " dB");    
    KNOB_DefinePosition(e_EQ1Q, 195, 95, 50, 75, 0, "Q 1", "");
    KNOB_DefinePosition(e_EQ2F, 250, 95, 50, 75, 0, "Freq 2", " Hz");
    KNOB_DefinePosition(e_EQ2A, 305, 95, 50, 75, 0, "Ampl 2", " dB");    
    KNOB_DefinePosition(e_EQ2Q, 360, 95, 50, 75, 0, "Q 2", "");
    KNOB_DefinePosition(e_EQ3F, 415, 95, 50, 75, 0, "Freq 3", " Hz");
    KNOB_DefinePosition(e_EQ3A, 470, 95, 50, 75, 0, "Ampl 3", " dB");    
    KNOB_DefinePosition(e_EQ3Q, 525, 95, 50, 75, 0, "Q 3", "");
    KNOB_DefinePosition(e_HighCut, 580, 95, 50, 75, 0, "Hi Cut", "Hz");
    KNOB_DefinePosition(e_Gain, 635, 95, 50, 75, 0, "Gain", "");

    KNOB_DefinePosition(e_IR1Vol, 30, 390, 50, 80, 0, "IR1 Vol", "");
    KNOB_DefinePosition(e_IR1off, 80, 390, 50, 80, 0, "IR1 Dist", "");
    KNOB_DefinePosition(e_IR2Vol, 130, 390, 50, 80, 0, "IR2 Vol", "");
    KNOB_DefinePosition(e_IR2off, 180, 390, 50, 80, 0, "IR2 Dist", "");
    
    KNOB_DefinePosition(e_IRF1, 58, 185, 30, 190, 2, "50", "");
    KNOB_DefinePosition(e_IRF2, 88, 185, 30, 190, 2, "100", "");
    KNOB_DefinePosition(e_IRF3, 118, 185, 30, 190, 2, "200", "");
    KNOB_DefinePosition(e_IRF4, 148, 185, 30, 190, 2, "300", "");
    KNOB_DefinePosition(e_IRF5, 178, 185, 30, 190, 2, "400", "");
    KNOB_DefinePosition(e_IRF6, 208, 185, 30, 190, 2, "500", "");
    KNOB_DefinePosition(e_IRF7, 238, 185, 30, 190, 2, "600", "");
    KNOB_DefinePosition(e_IRF8, 268, 185, 30, 190, 2, "700", "");
    KNOB_DefinePosition(e_IRF9, 298, 185, 30, 190, 2, "800", "");
    KNOB_DefinePosition(e_IRF10, 328, 185, 30, 190, 2, "900", "");
    KNOB_DefinePosition(e_IRF11, 358, 185, 30, 190, 2, "1k", "");
    KNOB_DefinePosition(e_IRF12, 388, 185, 30, 190, 2, "1.25k", "");
    KNOB_DefinePosition(e_IRF13, 418, 185, 30, 190, 2, "1.5k", "");
    KNOB_DefinePosition(e_IRF14, 448, 185, 30, 190, 2, "2k", "");
    KNOB_DefinePosition(e_IRF15, 478, 185, 30, 190, 2, "3k", "");
    KNOB_DefinePosition(e_IRF16, 508, 185, 30, 190, 2, "4k", "");
    KNOB_DefinePosition(e_IRF17, 538, 185, 30, 190, 2, "5k", "");
    KNOB_DefinePosition(e_IRF18, 568, 185, 30, 190, 2, "6k", "");
    KNOB_DefinePosition(e_IRF19, 598, 185, 30, 190, 2, "7k", "");
    KNOB_DefinePosition(e_IRF20, 628, 185, 30, 190, 2, "8k", "");
    KNOB_DefinePosition(e_IRF21, 658, 185, 30, 190, 2, "12k", "");

    //R1.10 Track how many active knobs we have.
    Knob_Cnt = 37;

    //R1.00 HELP label.
    labHelp.setJustificationType(juce::Justification::centredLeft);
    labHelp.setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
    labHelp.setColour(juce::Label::textColourId, juce::Colour(0xFFC0C0C0));
    labHelp.setColour(juce::Label::outlineColourId, juce::Colour(0x00000000));
    addAndMakeVisible(labHelp);
    labHelp.setText("IR Tweaker v1.10 7/2026", juce::dontSendNotification);

    //R1.00 Graphic EQ Adjustment label.
    labGEQ.setJustificationType(juce::Justification::centredRight);
    labGEQ.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    labGEQ.setColour(juce::Label::textColourId, juce::Colour(0xFFA0A0A0));
    labGEQ.setColour(juce::Label::outlineColourId, juce::Colour(0xFF606060));
    addAndMakeVisible(labGEQ);
    labGEQ.setVisible(false);

    //R1.10 Mouse position.
    labMouse.setJustificationType(juce::Justification::centredRight);
    labMouse.setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
    labMouse.setColour(juce::Label::textColourId, juce::Colour(0xFFA0A0A0));
    labMouse.setColour(juce::Label::outlineColourId, juce::Colour(0x00000000));
    addAndMakeVisible(labMouse);
    labMouse.setVisible(false);

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

    //R1.10 Undo Reset.
    butUndo.setButtonText("Undo Reset");
    butUndo.setClickingTogglesState(false);
    butUndo.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF202020));
    butUndo.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFF2ECE2));
    butUndo.addListener(this);
    addAndMakeVisible(butUndo);

    //R1.00 Invert the IR.
    butInvIR1.setButtonText("Inv");
    butInvIR1.setClickingTogglesState(false);
    butInvIR1.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF202020));
    butInvIR1.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFF2ECE2));
    butInvIR1.addListener(this);
    addAndMakeVisible(butInvIR1);

    //R1.00 Invert the IR.
    butInvIR2.setButtonText("Inv");
    butInvIR2.setClickingTogglesState(false);
    butInvIR2.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF202020));
    butInvIR2.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFF2ECE2));
    butInvIR2.addListener(this);
    addAndMakeVisible(butInvIR2);

    //R1.00 Auto align IR1.
    butAutoIR1.setButtonText("Auto");
    butAutoIR1.setClickingTogglesState(false);
    butAutoIR1.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF202020));
    butAutoIR1.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFF2ECE2));
    butAutoIR1.addListener(this);
    addAndMakeVisible(butAutoIR1);

    //R1.00 Auto align IR2.
    butAutoIR2.setButtonText("Auto");
    butAutoIR2.setClickingTogglesState(false);
    butAutoIR2.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF202020));
    butAutoIR2.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFF2ECE2));
    butAutoIR2.addListener(this);
    addAndMakeVisible(butAutoIR2);

    
    //R1.10 Show the Last FFT.
    butFFT_Last.setButtonText("Trc B");
    butFFT_Last.setClickingTogglesState(false);
    butFFT_Last.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF202020));
    butFFT_Last.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFF2ECE2));
    butFFT_Last.addListener(this);
    addAndMakeVisible(butFFT_Last);
    butFFT_Last.setVisible(false);

    //R1.10 Show the Last FFT.
    butFFT_Calc.setButtonText("Calc");
    butFFT_Calc.setClickingTogglesState(false);
    butFFT_Calc.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFF202020));
    butFFT_Calc.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFFF2ECE2));
    butFFT_Calc.addListener(this);
    addAndMakeVisible(butFFT_Calc);
    butFFT_Calc.setVisible(false);

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


    //R1.10 PreFill the FFT array so it is on the graph.
    for (int F = 0; F < 2048; F++) FFT_TraceY[F] = 360.0f;

    //R1.10 PreCalc the FFT Log graph X coordinates.
    float FreqBin = (audioProcessor.SampleRate / 2.0f) / 1024.0f;
    for (int F = 0; F < 1024; F++)
    {
        //R1.10 Calulate the screen X position based on Frequency.
        //R1.10 Graph starts at screen X coordinate 35.
        //R1.10 35 + (log(Freq) - log(StartFreq)) * Xscalefactor.
        //R1.10 35 + (log(Freq) - 1.301) * 215.666.
        FFT_TraceXFreq[F] = F * FreqBin;
        FFT_TraceX[F] = 35.0f + ((log10f(F * FreqBin) - 1.301f) * 215.666f);
    }

    //R1.10 OPTION PreCalc Log Graph X positions for drawn graph instead of using an Image.
    //for (int t = 0; t < 28; t++)
    //{
    //   FFT_GrafXPos[t] = log10f(FFT_GrafX[t]);
    //   FFT_GrafXPos[t] = 35.0f + (FFT_GrafXPos[t] - 1.301f) * 215.666f;
    //}
    
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
    //R1.10 Watch to see if VST is offline. Basically non-responsive when offline.
    VST_WatchDogTime++;
    if (audioProcessor.VST_Activity != VST_WatchDog)
    {
        VST_WatchDog = audioProcessor.VST_Activity;
        VST_WatchDogTime = 0;
        if (!VST_Active)
        {
            VST_Active = true;
            Buttons_Set(true);
            HELP_Post("VST ONLINE", -1);
        }
    }
    //R1.10 Wait for 1 second of inactivity to declare OFFLINE.
    if (10 < VST_WatchDogTime)
    {
        VST_WatchDogTime = 0;
        VST_Active = false;
        Buttons_Set(false);
        HELP_Post("VST OFFLINE", -1);
    }

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
        case 44100: labInfo4.setText("v1.10  44.1 k", juce::sendNotification); break;
        case 48000: labInfo4.setText("v1.10  48 k", juce::sendNotification); break;
        case 96000: labInfo4.setText("v1.10  96 k", juce::sendNotification); break;
        case 192000: labInfo4.setText("v1.10  192 k", juce::sendNotification); break;
        default: labInfo4.setText("v1.10", juce::sendNotification); break;
        }
    }

    //R1.00 User wants to save the current IR pulse data.
    if (audioProcessor.Pulse_Complete)
    {
        audioProcessor.Pulse_Complete = false;

        //R1.00 We have fresh Pulse data, perform FFT or Save to Wave file.
        if (Do_FFT)
        {   
            FFT_Calc(&audioProcessor.Wave_Temp[0], &FFT_Trace[0]);
            FFT_ScaleToScreen(&FFT_Trace[0], &FFT_TraceY[0]);
            repaint();
        }
        else
        {
            dlgFile = std::make_unique<juce::FileChooser>("Save the IR wave file", SetupPath_Current, "*.wav");
            auto chooserFlags = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles;

            dlgFile->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
                {
                    auto file = fc.getResult();
                    if (file != juce::File{}) FILE_WriteWave(file.getFullPathName());
                    repaint();
                });
        }
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

//R1.10 Enable/Disable all controls.
void MakoBiteAudioProcessorEditor::Buttons_Set(bool buttstate)
{
    butLoadIR1.setEnabled(buttstate);
    butLoadIR1.setEnabled(buttstate);
    butLoadIR2.setEnabled(buttstate);
    butClick.setEnabled(buttstate);
    butEQReset.setEnabled(buttstate);
    butUndo.setEnabled(buttstate);
    butInvIR1.setEnabled(buttstate);
    butInvIR2.setEnabled(buttstate);
    butAutoIR1.setEnabled(buttstate);
    butAutoIR2.setEnabled(buttstate);    
    butPathIR1.setEnabled(buttstate);
    butPathIR2.setEnabled(buttstate);
    butFFT_Calc.setEnabled(buttstate);
    butFFT_Last.setEnabled(buttstate);

    sldKnob[e_LowCut].setEnabled(buttstate);
    sldKnob[e_HighCut].setEnabled(buttstate);
    sldKnob[e_Gain].setEnabled(buttstate);
    sldKnob[e_IR1Vol].setEnabled(buttstate);
    sldKnob[e_IR1off].setEnabled(buttstate);
    sldKnob[e_IR2Vol].setEnabled(buttstate);
    sldKnob[e_IR2off].setEnabled(buttstate);
    for (int t = e_EQ1F; t < (e_EQ3Q + 1); t++) sldKnob[t].setEnabled(buttstate);
    for (int t = e_IRF1;  t < (e_IRF21 + 1); t++) sldKnob[t].setEnabled(buttstate);

}

//==============================================================================
void MakoBiteAudioProcessorEditor::paint (juce::Graphics& g)
{   
    int Samp = 0;
            
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
    g.drawImageAt(imgBackground, 0, 0);
    
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
        g.drawImageAt(imgUseIRon, rcUseIR.x1, rcUseIR.y1);
    else
        g.drawImageAt(imgUseIRoff, rcUseIR.x1, rcUseIR.y1);
    
    if (audioProcessor.USE_EQ)
        g.drawImageAt(imgUseEQon, rcUseEQ.x1, rcUseEQ.y1);
    else
        g.drawImageAt(imgUseEQoff, rcUseEQ.x1, rcUseEQ.y1);

    if (audioProcessor.USE_GEQ)
        g.drawImageAt(imgUseGEQon, rcUseGEQ.x1, rcUseGEQ.y1);
    else
        g.drawImageAt(imgUseGEQoff, rcUseGEQ.x1, rcUseGEQ.y1);


    //R1.00 The IR1 and IR2 indicators.
    //g.setColour(juce::Colour(0xFFFF8000)); g.fillRect(315, 383, 25, 15);
    //g.setColour(juce::Colour(0xFF0080FF)); g.fillRect(315, 403, 25, 15);

    //R1.00 Show the current IR files we are using.        
    g.setColour(colBone);
    g.drawFittedText(audioProcessor.FileOnly_IRSpeaker1, 342, 382, 345, 15, juce::Justification::left, 1);
    g.drawFittedText(audioProcessor.FileOnly_IRSpeaker2, 342, 402, 345, 15, juce::Justification::left, 1);

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

    
    //R1.10 Draw FFT Frequency Response graph or BW options based on editMode.
    if (editMode == 1)
    {
        //R1.10 Draw the empty FFT graph image.
        g.drawImageAt(imgGraph, 30, 174);
        g.drawImageAt(imgFFTon, rcFFT.x1, rcFFT.y1);

        //R1.00 Drawn graph option.
        //g.setColour(juce::Colour(0xFFC00080));
        //for (int t = 0; t < 28; t++) g.drawLine(FFT_GrafXPos[t], 178.0f, FFT_GrafXPos[t], 361.0f);
        //for (int t = 0; t < 8; t++) g.drawLine(35, 178.0f + (t * 26.143f), 683, 178.0f + (t * 26.143f));

        //R1.10 Draw the LAST FFT Freq Response up to 20 kHz (683px on screen).
        if (FFT_Last_Valid)
        {
            g.setColour(juce::Colour(0xFFA06000));
            pathTrace.startNewSubPath(35.0f, FFT_TraceY_Last[0]);
            Samp = 0;
            while ((FFT_TraceX[Samp] < 683.0f) && (Samp < 1024))
            {
                Samp++;
                pathTrace.lineTo(FFT_TraceX[Samp], FFT_TraceY_Last[Samp]);
            }
            g.strokePath(pathTrace, juce::PathStrokeType(2.0f));
            pathTrace.clear();
        }

        //R1.10 Draw the FFT Freq Response up to 20 kHz (683px on screen). 
        g.setColour(juce::Colour(0xFFFFFFFF));
        pathTrace.startNewSubPath(35.0f, FFT_TraceY[0]);
        Samp = 0;
        while ((FFT_TraceX[Samp] < 683.0f) && (Samp < 1024))
        {
            Samp++;
            pathTrace.lineTo(FFT_TraceX[Samp], FFT_TraceY[Samp]);
        }
        g.strokePath(pathTrace, juce::PathStrokeType(2.0f));
        pathTrace.clear();
    }
    else
    {
        //R1.10 Normal Mode view.
        //R1.10 Draw the Graphic EQ Bandwidth(Q) options.
        if (audioProcessor.GEQ_BW_Mode == 0)
          g.drawImageAt(imgopt07on, 35, 245);
        else
            g.drawImageAt(imgopt07off, 35, 245);

        if (audioProcessor.GEQ_BW_Mode == 1)
            g.drawImageAt(imgopt14on, 35, 265);
        else
            g.drawImageAt(imgopt14off, 35, 265);

        if (audioProcessor.GEQ_BW_Mode == 2)
            g.drawImageAt(imgopt20on, 35, 285);
        else
            g.drawImageAt(imgopt20off, 35, 285);

        if (audioProcessor.GEQ_BW_Mode == 3)
            g.drawImageAt(imgopt40on, 35, 305);
        else
            g.drawImageAt(imgopt40off, 35, 305);

        g.drawImageAt(imgFFToff, rcFFT.x1, rcFFT.y1);
    }

}

void MakoBiteAudioProcessorEditor::buttonClicked(juce::Button* butt)
{   
    auto buttstate = butt->getState();

    //R1.00 Tell the processor to Start the IR Pulse.
    if (butt == &butClick)
    {
        Do_FFT = false;                  //R1.10 Turn off any FFT actions.
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
                if (file != juce::File{})
                {
                    audioProcessor.File_IRSpeaker1 = file.getFullPathName();
                    audioProcessor.File_LoadNewIR1 = true;
                    audioProcessor.SettingsChanged = true;
                    sldKnob[e_IR1off].setValue(0.0f, juce::sendNotification); //R1.10 Reset Dist/offset.
                }
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
                if (file != juce::File{})
                {
                    audioProcessor.File_IRSpeaker2 = file.getFullPathName();
                    audioProcessor.File_LoadNewIR2 = true;
                    audioProcessor.SettingsChanged = true;
                    sldKnob[e_IR2off].setValue(0.0f, juce::sendNotification); //R1.10 Reset Dist/offset.
                }
                repaint();
            });
    }

    
    //R1.00 Set all EQs to 0.
    if (butt == &butEQReset)
    {
        //R1.10 Needs modal set and/or callbacks to work.
        //int Ass = juce::NativeMessageBox::showYesNoBox(juce::MessageBoxIconType::QuestionIcon, "Reset all EQs?", "Selecting YES will reset both the Studio EQ and Graphic EQ to their default states.", nullptr , nullptr);

        for (int t = 0; t < e_End; t++) sldKnob_Undo[t] = sldKnob[t].getValue();

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

    //R1.10 Undo the last EQ RESET.
    if (butt == &butUndo)
    {
        //R1.10 Verify we have valid data to Reset to.
        if (0.0f < sldKnob_Undo[e_LowCut])
        {
            sldKnob[e_LowCut].setValue(sldKnob_Undo[e_LowCut], juce::sendNotification);
            sldKnob[e_HighCut].setValue(sldKnob_Undo[e_HighCut], juce::sendNotification);
            sldKnob[e_EQ1F].setValue(sldKnob_Undo[e_EQ1F], juce::sendNotification);
            sldKnob[e_EQ1A].setValue(sldKnob_Undo[e_EQ1A], juce::sendNotification);
            sldKnob[e_EQ1Q].setValue(sldKnob_Undo[e_EQ1Q], juce::sendNotification);
            sldKnob[e_EQ2F].setValue(sldKnob_Undo[e_EQ2F], juce::sendNotification);
            sldKnob[e_EQ2A].setValue(sldKnob_Undo[e_EQ2A], juce::sendNotification);
            sldKnob[e_EQ2Q].setValue(sldKnob_Undo[e_EQ2Q], juce::sendNotification);
            sldKnob[e_EQ3F].setValue(sldKnob_Undo[e_EQ3F], juce::sendNotification);
            sldKnob[e_EQ3A].setValue(sldKnob_Undo[e_EQ3A], juce::sendNotification);
            sldKnob[e_EQ3Q].setValue(sldKnob_Undo[e_EQ3Q], juce::sendNotification);

            for (int t = 0; t < (1 + (e_IRF21 - e_IRF1)); t++) sldKnob[e_IRF1 + t].setValue(sldKnob_Undo[e_IRF1 + t], juce::sendNotification);
        }

        HELP_Post("UNDO RESET: Return EQs to the settings when RESET was selected.", -1);
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

    
    //R1.10 Send the Impulse and then Calculate the FFT.
    //R1.10 Pulse_On creates the pulse in Processor.
    //R1.10 Do_FFT tells the editor to deal with the data when processor is done.
    if (butt == &butFFT_Calc)
    {
        if (editMode == 1)
        {
            audioProcessor.Pulse_On = true;
            Do_FFT = true;            
        }
        HELP_Post("CALC: Update the Frequency Response graph (FFT).", -1);
        repaint();
    }

    //R1.10 store the current FFT trace for comparisons.
    if (butt == &butFFT_Last)
    {
        FFT_Last_Valid = true;
        FFT_StoreLast();
        HELP_Post("STORE TRACE: Store the current FFT trace in memory for comparison view.", -1);
    }

    //R1.00 View IR 1 path.
    if (butt == &butPathIR1) HELP_Post("IR1 Path: " + audioProcessor.File_IRSpeaker1, -1);
    
    //R1.00 View IR 2 path.
    if (butt == &butPathIR2) HELP_Post("IR2 Path: " + audioProcessor.File_IRSpeaker2, -1);

}

//R1.10 Mouse click handling.
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

    if (!VST_Active) return;

    //R1.10 Get some mouse information.
    int mButton = 2;
    if (e.mods.isLeftButtonDown()) mButton = 1;
    if (e.mods.isMiddleButtonDown()) mButton = 3;
    int posX = e.getPosition().x;
    int posY = e.getPosition().y;

    MOUSE_Down = mButton;

    //R3.10 Check for valid click.
    if (e.eventComponent->getBoundsInParent().getWidth() < GUI_Width) return;

    //R1.10 Check to see if user clicked on a USE selector button.
    if ((rcUseIR.x1 < posX) && (posX < rcUseIR.x2))
    {
        if ((rcUseIR.y1 < posY) && (posY < rcUseIR.y2))
        {
            if (audioProcessor.USE_IR)
                audioProcessor.USE_IR = false;            
            else
                audioProcessor.USE_IR = true;
            
            HELP_Post("Toggle the IR on/off.", -1);
        }
        if ((rcUseEQ.y1 < posY) && (posY < rcUseEQ.y2))
        {
            if (audioProcessor.USE_EQ)
                audioProcessor.USE_EQ = false;
            else
                audioProcessor.USE_EQ = true;

            HELP_Post("Toggle the EQ on/off.", -1);
        }
        if ((rcUseGEQ.y1 < posY) && (posY < rcUseGEQ.y2))
        {
            if (audioProcessor.USE_GEQ)
                audioProcessor.USE_GEQ = false;
            else
                audioProcessor.USE_GEQ = true;

            HELP_Post("Toggle the Graphic EQ on/off.", -1);
        }
        repaint();
    }

    //R1.10 Check for IR Filename selection.
    if ((315 < posX) && (posX < 690))
    {
        if (mButton == 1)
        {
            if ((380 < posY) && (posY < 400)) HELP_Post(audioProcessor.File_IRSpeaker1, -1);
            if ((400 < posY) && (posY < 420)) HELP_Post(audioProcessor.File_IRSpeaker2, -1);
        }
        else
        {
            if ((380 < posY) && (posY < 400)) HELP_Post(audioProcessor.FileOnly_IRSpeaker1, -1);
            if ((400 < posY) && (posY < 420)) HELP_Post(audioProcessor.FileOnly_IRSpeaker2, -1);
        }
    }

    //R1.10 Check for Graphic EQ bandwidth changes.
    if (!editMode)
    {
        if ((245 < posY) && (posY < 325))
            if ((35 < posX) && (posX < 55)) GEQ_OptionCheck(posX, posY);
    }

    //R1.10 Check to see if user clicked on a USE selector button.
    if ((rcFFT.x1 < posX) && (posX < rcFFT.x2))
        if ((rcFFT.y1 < posY) && (posY < rcFFT.y2)) FFT_OptionCheck();        
    

}

//R1.10 Mouse movement tracking.
void MakoBiteAudioProcessorEditor::mouseMove(const juce::MouseEvent& e)
{
    int posX = e.getPosition().x;
    int posY = e.getPosition().y;

    //R1.10 If viewing FFT graph, display frequency and amplitude.
    if (editMode == 1)
    {
        if ((180 < posY) && (posY < 360))
        {
            if ((35 < posX) && (posX < 683)) FFT_ShowMouseData(posX, posY);
            return;
        }
    }
}

//R1.10 Put the FFT data (at the mouse coordinates) in the Help Label.
void MakoBiteAudioProcessorEditor::FFT_ShowMouseData(int posX, int posY)
{
    //R1.10 Calculate the FFT data from the mouse position.
    //186px = 100Hz, 
    //FFT_TraceX[F] = 35.0f + ((log10f(F * FreqBin) - 1.301f) * 215.666f);
    //186-35=151/215.666=.70001+1.301=2  10^2=100
    
    //R1.10 Calculate the frequency, which gives the amplitude array index.
    float f = powf(10.0f, ((posX - 35) / 215.666f) + 1.301f);
    float FreqBin = (audioProcessor.SampleRate / 2.0f) / 1024.0f;
    int idx = f / FreqBin;

    if ((- 1 < idx) && (idx < 1024)) labMouse.setText(std::to_string(int(f)) + " Hz, " + std::to_string(int(FFT_Trace[idx])) + " dB", juce::dontSendNotification);
}

void MakoBiteAudioProcessorEditor::FILE_WriteWave(juce::String tFile)
{
    //R1.10 Added simple check for locked file errors.
    juce::File file(tFile);
    juce::WavAudioFormat format;
    std::unique_ptr<juce::AudioFormatWriter> writer;
    bool FileValid = false;

    //R1.00 Delete the file if it exists.
    if (file.exists()) file.deleteFile();

    //R1.00 Create the file Writer.
    writer.reset(format.createWriterFor(new juce::FileOutputStream(file), 48000.0, 1, 24, {}, 0));

    //R1.00 Create an audio buffer to store the audio in temporarily.
    juce::AudioBuffer<float> buffer = juce::AudioBuffer<float>::AudioBuffer(1, 2080);

    //R1.00 Write our audio data into a temp buffer so it is the right type/format.
    auto* channelData = buffer.getWritePointer(0);
    for (int t = 0; t < 2080; t++) channelData[t] = audioProcessor.Wave_Temp[t];

    //R1.10 Write our buffer data into the file.
    if (writer != nullptr) 
        FileValid = writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());

    //R1.10 Check for a locked file. 
    if (!FileValid) HELP_Post("FILE ERROR: Unable to write to the selected file.", -1);

    //R1.00 Close the file? This causes a Juce Assert when errors occur above. 
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

    //R1.10 Information labels. 
    labInfo4.setBounds(183, 47, 80, 15);    
    labInfo2.setBounds(415, 47, 40, 15);   //R1.00 Clipping
    labInfo1.setBounds(423, 47, 105, 15);

    butLoadIR1.setBounds(30, 18, 65, 20);
    butLoadIR2.setBounds(30, 39, 65, 20);
    butClick.setBounds(  30, 60, 65, 20);    

    butEQReset.setBounds(102, 18, 65, 20);
    butUndo.setBounds(102, 60, 65, 20);
    
    butInvIR1.setBounds(240, 380, 35, 20);
    butInvIR2.setBounds(240, 400, 35, 20);
    butAutoIR1.setBounds(277, 380, 35, 20);
    butAutoIR2.setBounds(277, 400, 35, 20);

    //butPathIR1.setBounds(324, 380, 15, 20);
    //butPathIR2.setBounds(324, 400, 15, 20);

    butFFT_Calc.setBounds(38, 180, 35, 15);
    butFFT_Last.setBounds(38, 200, 35, 15);
    labMouse.setBounds(75, 180, 120, 15);

    labHelp.setBounds(30, 475, 655, 17);
    
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

//R1.10 User is toggling between FFT GRAPH and GRAPHIC EQ modes.
void MakoBiteAudioProcessorEditor::FFT_OptionCheck()
{
    if (editMode == 1)
    {
        //R1.10 Set the screen to Graphic EQ Mode.
        editMode = 0;
        Do_FFT = false;
        for (int t = e_IRF1; t < (e_IRF21 + 1); t++) sldKnob[t].setVisible(true);
        //butFFT.setButtonText("FFT");
        butFFT_Calc.setVisible(false);
        butFFT_Last.setVisible(false);
        labMouse.setVisible(false);
        HELP_Post("FFT: Toggle between FFT and GEW view.", -1);
    }
    else
    {
        //R1.10 Set the screen to FFT Graph Mode.
        editMode = 1;
        for (int t = e_IRF1; t < (e_IRF21 + 1); t++) sldKnob[t].setVisible(false);
        //butFFT.setButtonText("GEQ");
        butFFT_Calc.setVisible(true);
        butFFT_Last.setVisible(true);
        labMouse.setVisible(true);
        HELP_Post("GEQ: Toggle between FFT and GEW view.", -1);
    }
    repaint();
}

//R1.10 Check the mouse clicks for Graphic EQ filter bandwidth options.
void MakoBiteAudioProcessorEditor::GEQ_OptionCheck(int x, int y)
{
    int tMode = audioProcessor.GEQ_BW_Mode;

    if ((245 < y) && (y < 260)) audioProcessor.GEQ_BW_Mode = 0;
    if ((265 < y) && (y < 280)) audioProcessor.GEQ_BW_Mode = 1;
    if ((285 < y) && (y < 300)) audioProcessor.GEQ_BW_Mode = 2;
    if ((305 < y) && (y < 320)) audioProcessor.GEQ_BW_Mode = 3;

    //R1.10 Update our GEQ filters in processor if our setting changed.
    if (tMode != audioProcessor.GEQ_BW_Mode)
    {
        audioProcessor.SettingsChanged += 1;
        HELP_Post("BANDWIDTH (Q): Set the GEQ filter bandwidth. Smaller Q affects more freqs.", -1);
    }

    repaint();
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

void MakoBiteAudioProcessorEditor::FFT_Calc(float* buffer, float* result)
{
    int Nsamples = 2048;
    float Nsamplesf = 2048.0f;
    float PI = 3.141592654f;
    float INVLOG10 = 1.0f / 2.302585f;
    float Xscale = 2048.0f / (Nsamples / 2.0f);
    float AScale = 0.00048828125; //1.0f / Nsamples;
    float Rsum;
    float Isum;
    float PI2;
    float R;
    float I;
    float C;

    //R1.10 Reduce our 2048 time data to 1024 freq data.
    for (int F = 0; F < (Nsamples / 2); F++)
    {
        //R1.10 FFT Sumation formula.
        //R1.10 Loop thru our data looking for a specific frequency.
        Rsum = 0.0f;
        Isum = 0.0f;
        PI2 = float(F) / Nsamplesf;
        PI2 = 2.0f * PI * (PI2);
        for (int K = 0; K < Nsamples; K++)
        {
            R = buffer[K] * cosf(PI2 * K);
            I = buffer[K] * sinf(PI2 * K);
            Rsum = Rsum + R;
            Isum = Isum + I;
        }

        //R1.10 Scale the magnitude.
        C = AScale * sqrt((Rsum * Rsum) + (Isum * Isum));

        //R1.10 Convert to dB, and scale to screen.
        if (C == 0.0f) C = 0.0000001f;
        C = 20.0f * log10f(C);
        
        //R1.10 save.
        result[F] = C;        
    }

}

//R1.10 Scale the current FFT data to screen coordinates.
void MakoBiteAudioProcessorEditor::FFT_ScaleToScreen(float* buffer, float* scaledBuffer)
{
    float C;

    //R1.10 Loop thru the Time data (1024 samples) and scale to the screen graph area.
    for (int F = 0; F < 1024; F++)
    {
        //R1.10 Scale the result to the screen.
        //R1.10 70dB = 184px 184/70 = 2.62857 px/dB
        C = 125.0f - (buffer[F] * 2.62857f);

        //R1.10 Clip to screen graph area.
        if (360.0f < C) C = 360.0f;
        if (C < 178.0f) C = 178.0f;

        scaledBuffer[F] = C;
    }
}

//R1.10 Place the current FFT trace into memory for comparing traces.
void MakoBiteAudioProcessorEditor::FFT_StoreLast()
{
    for (int t = 0; t < 1024; t++)  FFT_TraceY_Last[t] = FFT_TraceY[t];
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

