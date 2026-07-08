/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/

//R1.00 Structure to hold our slider/knob screen positions.
struct t_KnobInfo {
    float x;
    float y;
    float sizex;
    float sizey;
    int DataType;
    juce::String Suffix;
    juce::String Name;
};

//*******************************************************************************************************************
//R1.00 Create a new LOOK AND FEEL class based on Juces LnF class.
//R1.00 We will override the SLIDER drawing routine.
//*******************************************************************************************************************
//R1.00 This lets us modify how objects are drawn to the screen.
class MakoLookAndFeel : public juce::LookAndFeel_V4
{
public:
    //R1.00 Let the user select a knob style.
    float Kpts[32];
    juce::Path pathKnob;

private:
    //R1.00 Ten tick mark angles around a slider.
    float TICK_Angle[11] = { 8.79645920, 8.29380417, 7.79114914, 7.28849411, 6.78583908, 6.28318405, 5.78052902, 5.27787399, 4.77521896, 4.27256393, 3.76 }; //3.76990914
    float TICK_Cos[11] = {};
    float TICK_Sin[11] = {};

    juce::Image imgSlideKnobOff;
    juce::Image imgSlideKnobOn;
    juce::Image imgKnob01;
    juce::Image imgKnob02;
    juce::Image imgKnob03;
    
public:
    MakoLookAndFeel()
    {
        imgSlideKnobOff = juce::ImageCache::getFromMemory(BinaryData::slide01_png, BinaryData::slide01_pngSize);
        imgSlideKnobOn = juce::ImageCache::getFromMemory(BinaryData::slide02_png, BinaryData::slide02_pngSize);
        imgKnob01 = juce::ImageCache::getFromMemory(BinaryData::knob_glass_01sm_png, BinaryData::knob_glass_01sm_pngSize);
        imgKnob02 = juce::ImageCache::getFromMemory(BinaryData::knob_glass_02sm_png, BinaryData::knob_glass_02sm_pngSize);
        imgKnob03 = juce::ImageCache::getFromMemory(BinaryData::knob_glass_03sm_png, BinaryData::knob_glass_03sm_pngSize);

        //R1.00 Do some PRECALC on Sin/Cos since they are expensive on CPU.
        for (int t = 0; t < 11; t++)
        {
            TICK_Cos[t] = std::cosf(TICK_Angle[t]);
            TICK_Sin[t] = std::sinf(TICK_Angle[t]);
        }

        //R1.00 Define the Path points to make a knob (Style 3).
        Kpts[0] = -2.65325243300477f;
        Kpts[1] = 8.60001462363607f;
        Kpts[2] = 0.0f;
        Kpts[3] = 10.0f;
        Kpts[4] = 2.65277678639377f;
        Kpts[5] = 8.60016135439157f;
        Kpts[6] = 7.81826556234706f;
        Kpts[7] = 6.23495979109873f;
        Kpts[8] = 8.3778301945593f;
        Kpts[9] = 3.28815468479365f;
        Kpts[10] = 9.74931428347318f;
        Kpts[11] = -2.22505528067641f;
        Kpts[12] = 7.79431009355225f;
        Kpts[13] = -4.4998589050713f;
        Kpts[14] = 4.3390509473009f;
        Kpts[15] = -9.00958583269659f;
        Kpts[16] = 1.34161181197136f;
        Kpts[17] = -8.89944255254108f;
        Kpts[18] = -4.33855264588318f;
        Kpts[19] = -9.00982579958681f;
        Kpts[20] = -6.12133095297134f;
        Kpts[21] = -6.59767439058605f;
        Kpts[22] = -9.74919120703023f;
        Kpts[23] = -2.22559448434896f;
        Kpts[24] = -8.97486228392824f;
        Kpts[25] = .672195644527914f;
        Kpts[26] = -7.81861038843018f;
        Kpts[27] = 6.23452737534543f;
        Kpts[28] = -5.07025014121689f;
        Kpts[29] = 7.4358969536627f;
        Kpts[30] = -2.65325243300477f;
        Kpts[31] = 8.60001462363607f;

        //R1.00 Create the actual PATH for our KNOB style 3.
        pathKnob.startNewSubPath(Kpts[0], Kpts[1]);
        for (int t = 0; t < 32; t += 2)
        {
            pathKnob.lineTo(Kpts[t], Kpts[t + 1]);
        }
        pathKnob.closeSubPath();

        //R1.00 Recreate our points with smoothed corners.
        //pathKnob = pathKnob.createPathWithRoundedCorners(4.0f);
    }

    
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle, juce::Slider& sld) override
    {   
        int tH = height - 10;

        //R1.00 Draw the knob valley.
        //g.setColour(juce::Colour(0xFF000000));
        //g.fillRect(x + 12, y + 5, 6, tH);
        //g.setColour(juce::Colour(0xFFFFFFFF));
        //g.drawLine(x + 18, y + 5, x + 18, y + 5 + tH);

        //R1.00 Draw the Knob.
        int tY = (y + height - 5) - (tH * (.5f + (sld.getValue() * .5f))) - 16;
        if (sld.getValue() != 0.0f)
            g.drawImageAt(imgSlideKnobOn, x + 4, tY);
        else
            g.drawImageAt(imgSlideKnobOff, x + 4, tY);

    }
    
    //R1.00 Override the Juce SLIDER drawing function so our code gets called instead of Juces code.
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& sld) override
    {
        //R1.00 Most of these are from JUCE demo code. Could be reduced if not used.
        //R1.00 Could PRECALC if they were all the same size control. 
        auto radius = (float)juce::jmin(width / 2, height / 2) - 8.0f;
        auto centreX = (float)x + (float)width * 0.5f;
        auto centreY = (float)y + (float)height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle); //R1.00 Bizarre values here 216(36) to 504(324).

        //R1.00 Mako Var defs.
        float sinA;
        float cosA;
        juce::ColourGradient ColGrad;
        int col;
        juce::Colour C1;

        //R1.00 Used when creating background image.
        //g.setColour(juce::Colour(0xFF000000));
        //g.fillEllipse(rx, ry, rw, rw);
        //return;

        //1.00 Draw the KNOB face.
        //ColGrad = juce::ColourGradient(juce::Colour(0xFF404040), 0.0f, y, juce::Colour(0xFF202020), 0.0f, y + height, false);
        //g.setGradientFill(ColGrad);
        //g.fillEllipse(rx, ry, rw, rw);

        //R1.00 Draw shading around knob face.        
        //ColGrad = juce::ColourGradient(juce::Colour(0xFFF0F0F0), 0.0f, y, juce::Colour(0xFFA0A0A0), 0.0f, y + height, false);
        //g.setGradientFill(ColGrad);
        //g.drawEllipse(rx, ry, rw, rw, 2.0f);

        
        //R4.00 Using the outline color as knob style selector.
        C1 = sld.findColour(juce::Slider::rotarySliderOutlineColourId);
        int Cx = C1.getBlue();
        switch (Cx)
        {
        case 0: //R1.00 Orange Knob
            g.drawImageAt(imgKnob01, rx - 4, ry - 3);
            break;
        case 1: //R1.00 Blue Knob
            g.drawImageAt(imgKnob02, rx - 4, ry - 3);
            break;
        case 2: //R1.00 Gray Knob
            g.drawImageAt(imgKnob03, rx - 4, ry - 3);
            break;
        }

        //R1.00 Dont draw anymore objects if the control is disabled.
        if (sld.isEnabled() == false) return;

        //R1.00 Copy our predefined KNOB PATH, scale it, and then transform it to the centre position.
        //R1.00 The knob SIZE must be performed first. It is then ROTATED around its center. Then moved (TRANSLATED) to the screen knob position.
        //juce::Path pK = pathKnob;
        //pK.applyTransform(juce::AffineTransform::scale(radius / 11.0f).followedBy(juce::AffineTransform::rotation(angle).translated(centreX, centreY)));
        //ColGrad = juce::ColourGradient(juce::Colour(0xFFF0F0F0), 0.0f, y, juce::Colour(0xFF000000), 0.0f, y + height, false);
        //g.setGradientFill(ColGrad);
        //g.strokePath(pK, juce::PathStrokeType(2.0f));

        //R1.00 TICK marks on background.
        //g.setColour(juce::Colour(0xFFA0A0A0));
        //sinA = TICK_Sin[0] * radius; cosA = TICK_Cos[0] * radius; g.drawLine(centreX + (sinA * 1.3f), centreY - (cosA * 1.3f), centreX + sinA * 1.1f, centreY - cosA * 1.1f, 1.0f);
        //g.drawLine(centreX, centreY - (radius * 1.3f), centreX, centreY - (radius * 1.1f), 1.0f);
        //sinA = TICK_Sin[10] * radius; cosA = TICK_Cos[10] * radius; g.drawLine(centreX + (sinA * 1.3f), centreY - (cosA * 1.3f), centreX + sinA * 1.1f, centreY - cosA * 1.1f, 1.0f);
        
        //R1.00 Reference Tick Marks
        //g.setColour(juce::Colour(0xFFFFFFFF));
        //for (int t = 0; t < 11; t++)
        //{
        //    sinA = TICK_Sin[t] * radius;
        //    cosA = TICK_Cos[t] * radius;
        //    g.drawLine(centreX + (sinA * 1.3f), centreY - (cosA * 1.3f), centreX + sinA * 1.1f, centreY - cosA * 1.1f, 1.0f);            
        //}

        //R1.00 Draw finger adjust dent/indicator.
        sinA = std::sinf(angle);
        cosA = std::cosf(angle);        
        g.setColour(juce::Colour(0xFFF0F0F0));    
        g.drawLine(centreX + sinA * radius * .5f, centreY - cosA * radius * .5f, centreX + sinA * radius, centreY - cosA * radius, 4.0f);

        if (sld.hasKeyboardFocus(true))
        {
            g.setColour(juce::Colour(0xFF80FF80));
            g.fillEllipse(centreX - 4.0f, centreY - 4.0f, 8.0f, 8.0f);            
        }
       
    }
};


//*******************************************************************************************************************
//R1.00 Add SLIDER listener. BUTTON or TIMER listeners also go here if needed. Must add ValueChanged overrides!
//*******************************************************************************************************************
class MakoBiteAudioProcessorEditor  : public juce::AudioProcessorEditor , public juce::Slider::Listener, public juce::Timer, public juce::Button::Listener //public juce::Timer
{
public:
    MakoBiteAudioProcessorEditor (MakoBiteAudioProcessor&);
    ~MakoBiteAudioProcessorEditor() override;

    //R1.00 OUR override functions.
    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragStarted(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;

    void HELP_Post(juce::String Message, int HelpIndex);

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button* butt) override;
    void mouseDown(const juce::MouseEvent&) override;
    void mouseMove(const juce::MouseEvent&) override;
    bool MOUSEHACK_IsDown = false;
    int MOUSE_Down = 0;

    //R1.00 Override the TIMER so we can capture it and executes our UI code.
    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MakoBiteAudioProcessor& audioProcessor;

    MakoLookAndFeel myLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MakoBiteAudioProcessorEditor);


    void FILE_WriteWave(juce::String tFile);
    //R4.00 Some file work vars.
    juce::String SetupName_Current = "";
    juce::String SetupPath_Current = "";
    std::unique_ptr<juce::FileChooser> dlgFile;

    juce::Colour colBone = juce::Colour(0xFFF2ECE2);

    juce::TextButton butClick;       //R1.00 Force IR Pulse and save.
    juce::TextButton butLoadIR1;      //R1.00 Load a speaker IR.
    juce::TextButton butInvIR1;
    juce::TextButton butAutoIR1;
    juce::TextButton butPathIR1;
    juce::TextButton butLoadIR2;      //R1.00 Load a speaker IR.
    juce::TextButton butInvIR2;
    juce::TextButton butAutoIR2;
    juce::TextButton butPathIR2;
    juce::TextButton butEQReset;    
    //juce::TextButton butFFT;
    juce::TextButton butFFT_Last;
    juce::TextButton butFFT_Calc;
    juce::TextButton butUndo;
    //juce::TextButton butQ07;
    //juce::TextButton butQ14;
    //juce::TextButton butQ20;
    //juce::TextButton butQ40;
    juce::Image imgBackground;
    juce::Image imgSlideOn;
    juce::Image imgSlideOff;
    juce::Image imgGraph;
    juce::Image imgopt07off; 
    juce::Image imgopt07on;
    juce::Image imgopt14off;
    juce::Image imgopt14on;
    juce::Image imgopt20off;
    juce::Image imgopt20on;
    juce::Image imgopt40off;
    juce::Image imgopt40on;
    juce::Image imgUseIRon;
    juce::Image imgUseIRoff;
    juce::Image imgUseEQon;
    juce::Image imgUseEQoff;
    juce::Image imgUseGEQon;
    juce::Image imgUseGEQoff;
    juce::Image imgFFTon;
    juce::Image imgFFToff;
    juce::Label labInfo1;
    juce::Label labInfo2;    
    juce::Label labInfo4;
    juce::Label labGEQ;
    juce::Label labHelp; 
    juce::Label labMouse;
    int ctrlHelp = 0;
    int ctrlHelpLast = -1;

    int editMode = 0; //R1.00 0-Normal, 1-FFT

    void FFT_Calc(float* buffer, float* result);
    void FFT_ScaleToScreen(float* buffer, float* result);
    void FFT_StoreLast();
    bool Do_FFT = false;
    bool FFT_Last_Draw = false;
    bool FFT_Last_Valid = false;
    
    float FFT_Trace[2049] = {};        //R1.10 FFT Actual stored amplitudes.
    float FFT_TraceY[2049] = {};       //R1.10 FFT amplitudes scaled to the screen.
    float FFT_TraceX[2049] = {};       //R1.10 FFT frequency bins scaled to the screen.
    float FFT_TraceXFreq[2049] = {};
    float FFT_TraceY_Last[2049] = {};  //R1.10 Stored trace for FFT comparison.  
    float FFT_GrafX[29] = {20,30,40,50,60,70,80,90,100,200,300,400,500,600,700,800,900,1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,20000};
    float FFT_GrafXPos[29] = {};
    void FFT_ShowMouseData(int posX, int posY);

    void GEQ_LabelUpdate(int idx);

    float SampleRate = 0.0f;         //R1.00 Added must be 0.

    //R1.00 Track our Clipping state so we dont redraw the screen every timer tick.
    bool STATE_Clip = false;
    long VST_WatchDog = 0;
    long VST_WatchDogTime = 0;
    bool VST_Active = true;
    void Buttons_Set(bool buttstate);

    int GUI_Width = 718;  
    int GUI_Height = 510; 

    void GUI_Init_Rotary(juce::Slider* slider, float Val, float Vmin, float Vmax, float Vinterval, juce::String Suffix, int TickStyle, int ThumbColor);
    void GUI_Init_Slider(juce::Slider* slider, float Val, float Vmin, float Vmax, float Vinterval, juce::String Suffix);
    
    //R1.00 Define our UI Juce Slider controls.
    int Knob_Cnt = 0;
    juce::Slider sldKnob[60];
    juce::Slider jsP1_Mono;
    float sldKnob_Undo[60] = {};

    //R1.00 Define the coords and text for our knobs. Not JUCE related. 
    t_KnobInfo Knob_Info[60] = {};    
    void KNOB_DefinePosition(int t, float x, float y, float sizex, float sizey, int datatype, juce::String name, juce::String suffix);
    
    //R4.00 Struct and position defs for mouse click checks in signal path.
    struct makoRect
    {
        int x1;
        int y1;
        int x2;
        int y2;
    };

    //R1.00 Set some screen positions for ON/OFF sliders.
    makoRect rcUseIR =  { 620,  17, 690,  32 }; 
    makoRect rcUseEQ =  { 620,  37, 690,  52 };
    makoRect rcUseGEQ = { 620,  57, 690,  72 };
    makoRect rcFFT    = { 655, 430, 695, 470 };

    void FFT_OptionCheck();
    void GEQ_OptionCheck(int x, int y);

    //R1.00 These are the indexes into our Settings var. Must be recreated in Processor. 
    enum { e_Gain, e_EQ1F, e_EQ1A, e_EQ1Q, e_EQ2F, e_EQ2A, e_EQ2Q, e_EQ3F, e_EQ3A, e_EQ3Q, e_HighCut, e_LowCut, e_IR1off, e_IR2off, e_IR1Vol, e_IR2Vol,
           e_IRF1, e_IRF2, e_IRF3, e_IRF4, e_IRF5, e_IRF6, e_IRF7, e_IRF8, e_IRF9, e_IRF10, e_IRF11, e_IRF12, e_IRF13, e_IRF14, e_IRF15, e_IRF16, e_IRF17, e_IRF18, e_IRF19, e_IRF20, e_IRF21,
           e_End
    };

    std::string LangStringHelp[60] =
    {   
        //R1.00 Normal Help.
        "GAIN: Set the output volume. Does not affect final IR.",
        "EQ FREQ 1: Set the bandpass center frequency.",
        "EQ AMPL 1: How much to add or lose this frequency.",
        "EQ Q 1: set the filter width. How many freqs to modify.",
        "EQ FREQ 2: Set the bandpass center frequency.",
        "EQ AMPL 2: How much to add or lose this frequency.",
        "EQ Q 2: set the filter width. How many freqs to modify.",
        "EQ FREQ 3: Set the bandpass center frequency.",
        "EQ AMPL 3: How much to add or lose this frequency.",
        "EQ Q 3: set the filter width. How many freqs to modify.",
        "HIGH CUT: Set when to start reducing high frequencies.",
        "LOW CUT: Set when to start reducing low frequencies.",
        "IR1 DISTANCE: Adjust the IR1 start to align with the IR2 phasing (mic distance).",
        "IR2 DISTANCE: Adjust the IR2 start to align with the IR1 phasing (mic distance).",
        "IR1 VOLUME: Adjust how loud IR1 will be when mixed with IR2.",
        "IR2 VOLUME: Adjust how loud IR2 will be when mixed with IR1.",
        
        //R1.00 Graphic EQ.
        "GRAPHIC EQ: Add or Remove this frequency.",
        "GRAPHIC EQ: Add or Remove this frequency.",
        "GRAPHIC EQ: Add or Remove this frequency.",
        "GRAPHIC EQ: Add or Remove this frequency.",
        "GRAPHIC EQ: Add or Remove this frequency.",
        "GRAPHIC EQ: Add or Remove this frequency.",
        "GRAPHIC EQ: Add or Remove this frequency.",
        "GRAPHIC EQ: Add or Remove this frequency.",
        "GRAPHIC EQ: Add or Remove this frequency.",
        "GRAPHIC EQ: Add or Remove this frequency.",
        "GRAPHIC EQ: Add or Remove this frequency.",
        "GRAPHIC EQ: Add or Remove this frequency.",
        "GRAPHIC EQ: Add or Remove this frequency.",
        "GRAPHIC EQ: Add or Remove this frequency.",
        "GRAPHIC EQ: Add or Remove this frequency.",
        "GRAPHIC EQ: Add or Remove this frequency.",
        "GRAPHIC EQ: Add or Remove this frequency.",
        "GRAPHIC EQ: Add or Remove this frequency.",
        "GRAPHIC EQ: Add or Remove this frequency.",
        "GRAPHIC EQ: Add or Remove this frequency.",
        "GRAPHIC EQ: Add or Remove this frequency.",
    };

public:
    
    //R1.00 Define our SLIDER attachment variables.
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt[60];
    

};
