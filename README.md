# IRTweaker
* JUCE 8 VST Impulse Response tweaker.
* Tested on Windows only.
* Written in Visual C++ 2022.
* Rosbone DSP
* Author: Mako
* Version: 1.00
* Posted: July 2, 2026

![Demo Image](docs/irt_demo_01.jpg)

VERSION
------------------------------------------------------------------
VERSION 1.00
* Initial release.<br/>  

NOTE:<br/>
Version 1.00s-5.06s files use the static runtime library option in JUCE.

SUMMARY
------------------------------------------------------------------
A Juce/C++ VST3 guitar processor. This app is a simple tool to load 
a Wave file implulse response file (2048 samples) and apply simple EQ
tweaks to it. 

THINGS TO KNOW
------------------------------------------------------------------
VST LIMITATIONS<br/>
VST's do not save string variables. This means things like 
WAVE files used in the VST will NOT BE AVAILABLE
from within the DAW at startup or its presets. 


