# IRTweaker
* JUCE 8 VST Impulse Response tweaker.
* Tested on Windows only.
* Written in Visual C++ 2022.
* Rosbone DSP
* Author: Mako
* Version: 1.10
* Posted: July 7, 2026

![Demo Image](Docs/irt_demo_01.jpg)
![Demo Image](Docs/irt_demo_02.jpg)

VERSION
------------------------------------------------------------------
VERSION 1.00
* Initial release.<br/>  

VERSION 1.10
* Fixed IR bug where filter buffers were not cleared.
* Added simple FFT graph viewer with trace store and data stats.
* Added Bandwidth options for Graphic Eq.
* Added not in use watchdog type detection.
* Disabled all controls when VST is not in use.
* Clicking the IR name shows path(left click) or filename (right click).

NOTE:<br/>
The VST/EXE use the static runtime library option in JUCE for maximum
compatibility.

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

NOTE: This program does not do any adjustments for sample rate! 

WHY USE IR TWEAKER
------------------------------------------------------------------
For most mid to high gain amplifier situations, the bulk of the final sound comes from the IR.
Best results for high gain amps comes from dialing in the correct IR and EQ. IRT lets this
happen in a CPU hungry way and then save that custom IR. Freeing up CPU cycles in the device/DAW
you using the IRs in.

HOW IT WORKS
------------------------------------------------------------------
IR Tweaker (IRT) is designed to create custom Impulse Responses (IR). It accomplishes this
by applying simple EQ methods to an existing IR. EQ options are 3 band Studio EQ and a 21 band Graphic EQ.
IRT can load two IRs and mix them for even more control. 

The program is designed for IRs that are 2048 samples in length. Longer and Shorter files will be saved as 2048.

STEPS
1. Start the program in a DAW or Stand alone.
2. If in a DAW, place the guitar processor before IRT. 
3. Load an IR by clicking on LOAD IR 1.
4. If desired, load a second IR by clicking on LOAD IR 2.
5. Adjust the EQ and/or Graphic EQ.
6. Select SAVE IR.

MIXING IRS<br/>
IRs loaded into IRT may sound best when they are in phase of each other. IRT lets the user adjust the IR
phase/sample position by adjusting IR1 DIST or IR 2 DIST. The analog equivalent of moving a microphone closer
or farther from the source.

Adjust IR 1 VOL and IR2 VOL to set the proper mix.

NOTE: The resulting IR is always normalized/maximized and will be max volume!

TWEAKING OUT<br/>
Note that when EQing, louder always sounds better to your ear. Adjust volumes as needed when editing.

You should EQ at a good volume to avoid Fletcher-Munson curve issues.<br/>
https://en.wikipedia.org/wiki/Equal-loudness_contour

SAVING<br/>
IRT does not do any magical math to create the final IR. It simply passes a pulse thru the IR, EQ, and GEQ then saves it.

3 BAND STUDIO EQUALIZER<br/>
The EQ lets you create very specific bandpass/bandstop filters. This includes controls for amplitude (boost/cut 12dB), Frequency, and bandwidth (Q).
Set a frequency and Q, then dial the amplitude up or down to modify the sound.

21 BAND GRAPHIC EQ<br/>
The GEQ has 21 fixed frequency filters (+/-12dB) that let you dial in specfic frequencies of the IR. The filter bandwidths are selectable at
Q values of .7, 1.4, 2.0, and 4.0.

FFT FREQUENCY RESPONSE GRAPH
Click on the Show FFT button converts the GEQ section to a frequency graph.
Click on CALC to see the FFT of the current setup. 
Click on TRC B (Trace B) to store the current trace for comparison viewing.
Main trace (A) is white and the secondary trace (B) is orange.
