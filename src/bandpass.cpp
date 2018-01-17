//-------------------------------- bandpass.cpp --------------------------------

/***************************************************************************
                            -  Description
                             -------------------
    Begin                : 9 September, 2016
    Copyright            : (C) 2016 by Simo
    E-Mail               : simo@furrydolphin.net
    Version              : 1.0.0
 **************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *                                                                         *
 *   If software can be free, why can't dolphins?                          *
 *                                                                         *
 ***************************************************************************/

#ifdef INSTR
#include <stdio.h>
#endif

#include <math.h>
#include "bandpass.h"

//---------------------------- Design Equations  ------------------------------

/*
       Name: Inductor

       Type: Function (FXdouble)

       Parameters:
                   aRio:      The source/load resistance (Ohms)
		   aFreqHigh: Upper limit of passband (Hz)
		   aFreqLow:  Lower   "   "      "    (Hz)
		   aIDCode:   Configuration identifier

       Returns: The inductance of the resonator coils (H)

       Note: All equations have the same parameters.
*/

//                        Passive BPF's

double Bandpass::L_Tune(double aR, double aFh, double aFl, BPuint aIDCode)
{
    double BW, RetVal;

    BW= aFh - aFl;

#ifdef INSTR
    printf("\nEntering function: Bandpass::L_Tune");
    printf("\naR= %E", aR);
    printf("\naFh= %E", aFh);
    printf("\naFl= %E", aFl);
    printf("\naIDCode= %d", aIDCode);
    printf("\nBW= %E", BW);
#endif

    switch (aIDCode)
    {
       case BW_PTC:

       case BW_PTLC: RetVal= (BW * aR)/(TWO_PI * aFl * aFh); break;

       case BW_STC:

       case BW_STLC: RetVal= aR/ANGULAR(BW);
    }

#ifdef INSTR
    printf("\nRetVal= %E", RetVal);
    printf("\nLeaving function: Banspass::L_Tune");
#endif

    return RetVal;
}
//------------------------------------------------------------------------------
/*
       Name: Capacitor

       Type Function (double)

       Returns: Capacitance of the tuning capacitors (F)
*/

double Bandpass::C_Tune(double aR, double aFh, double aFl, BPuint aIDCode)
{
    double BW, RetVal;

    BW= aFh - aFl;

#ifdef INSTR
    printf("\nEntering function: Bandpass::C_Tune");
    printf("\naR= %E", aR);
    printf("\naFh= %E", aFh);
    printf("\naFl= %E", aFl);
    printf("\naIDCode= %d", aIDCode);
    printf("\nBW= %E", BW);
#endif

    switch(aIDCode)
    {
       case BW_PTC:  RetVal= aFl/(TWO_PI * aFh * BW * aR); break;

       case BW_PTLC: RetVal= 1.0/(ANGULAR(BW) * aR); break;

       case BW_STC:  RetVal= BW/(TWO_PI * POW2(aFl) * aR); break;

       case BW_STLC: RetVal= BW/(TWO_PI * aFl * aFh * aR);
    }

#ifdef INSTR
    printf("\nRetVal= %E", RetVal);
    printf("\nLeaving function: Banspass::C_Tune");
#endif

    return RetVal;
}
//------------------------------------------------------------------------------
/*
       Name: CplCapacitor

       Type: Function (double)

       Returns: Capacitance of the coupling capacitor (F). Both resonant and
                non-resonant.
*/

double Bandpass::C_Couple(double aR, double aFh, double aFl, BPuint aIDCode)
{
    double BW, RetVal;

    BW= aFh - aFl;

#ifdef INSTR
    printf("\nEntering function: Bandpass::C_Couple");
    printf("\naR= %E", aR);
    printf("\naFh= %E", aFh);
    printf("\naFl= %E", aFl);
    printf("\naIDCode= %d", aIDCode);
    printf("\nBW= %E", BW);
#endif

    switch (aIDCode)
    {
       case BW_PTC:  RetVal= (aFh + aFl)/(FOUR_PI * aFh * aFl * aR); break;

       case BW_PTLC: RetVal= BW/(FOUR_PI * aFl * aFh * aR); break;

       case BW_STC:  RetVal= 1.0/(PI * aR * (aFh + aFl)); break;

       case BW_STLC: RetVal= 1.0/(PI * BW * aR);
    }

#ifdef INSTR
    printf("\nRetVal= %E", RetVal);
    printf("\nLeaving function: Bandpass::C_Couple");
#endif

    return RetVal;
}
//------------------------------------------------------------------------------
/*
       Name: CplInductor

       Type: Function (double)

       Returns: inductance of coupling inductor (H), resonant.
*/

double Bandpass::L_Couple(double aR, double aFh, double aFl, BPuint aIDCode)
{
    double BW, RetVal;

    BW= aFh - aFl;

#ifdef INSTR
    printf("\nEntering function: Bandpass::L_Couple");
    printf("\naR= %E", aR);
    printf("\naFh= %E", aFh);
    printf("\naFl= %E", aFl);
    printf("\naIDCode= %d", aIDCode);
    printf("\nBW= %E", BW);
#endif

    switch (aIDCode)
    {
       case BW_STC:  RetVal= 0; break;

       case BW_PTC:  RetVal= 0; break;

       case BW_PTLC: RetVal= aR/(PI * BW); break;

       case BW_STLC: RetVal= (BW * aR)/(FOUR_PI * aFl * aFh);
    }

#ifdef INSTR
    printf("\nRetVal= %E", RetVal);
    printf("\nLeaving function: Bandpass::L_Couple");
#endif

    return RetVal;
}

//-------------------------------- Active BPF's --------------------------------

//                              Multi-Feedback BPF
/*
       Name: kFactor

       Type: Function (double)

       Parameters:
                   aFc (double) -- The filter center frequency (Hz)
		   aCtrial (double) -- Tenative value for the filter capacitors (Farads)

       Returns: The "k" Factor (A/V) of the design.
*/

double Bandpass::K_factor(double aFh, double aFl, double aCtr, double &aQl, double &aFc)
{
    double RetVal;

#ifdef INSTR
    printf("\nEntering function: Bandpass::K_factor");
    printf("\naFh= %E", aFh);
    printf("\naFl= %E", aFl);
    printf("\naCtr= %E", aCtr);
#endif

    aFc=    (aFh + aFl)/2.0;
    aQl=    aFc/(aFh - aFl);
    RetVal= (ANGULAR(aFc) * aCtr);

#ifdef INSTR
    printf("\naFc= %E", aFc);
    printf("\naQl= %f", aQl);
    printf("\nRetVal= %f", RetVal);
    printf("\nLeaving function: Bandpass::K_factor");
#endif

    return RetVal;
}
//------------------------------------------------------------------------------
/*
       Name: ROne

       Type: Function (double)

       Parameters:
                   aQF (double) -- The filter loaded design Q Factor
		   aK (double) --  The design "k" Factor.

       Returns: The design value for R(1)
*/

double Bandpass::R_One(double aQ, double aKfactor)
{
    return (aQ / aKfactor);
}
//------------------------------------------------------------------------------
/*
       Name: RTwo

       Type: Function (double)

       Parameters:
                   aQF (double) -- The design Q Factor
		   aK (double) --  The design "k" Factor

       Returns: The design value for R(2)
*/

double Bandpass::R_Two(double aQ, double aKfactor)
{
    return (1 / ((2 * aQ - (1/aQ)) * aKfactor));
}
//------------------------------------------------------------------------------
/*
       Name: RThree

       Type: Function (double)

       Parameters:
                   aQF (double) -- The design Q Factor
		   aK (double) --  The design "k" Factor

       Returns: The design value for R(3)
*/

double Bandpass::R_Three(double aQ, double aKfactor)
{
    return ((2 * aQ)/aKfactor);
}

//------------------------------------------------------------------------------

//                              State Variable

 double Bandpass::MainResistor(double aFh, double aFl, double aCtr, double &aFcntr, double &aQfct)
{
    aFcntr= (aFh + aFl)/2.0;
    aQfct= aFcntr/(aFh - aFl);
    return (1 / (ANGULAR(aFcntr) * aCtr));
}

//------------------------------------------------------------------------------

double Bandpass::GainSetResistor(double aR, double Av)
{
    return (aR / Av);
}

//------------------------------------------------------------------------------

double Bandpass::DFSetResistor(double aQ, double aR)
{
    return (aR / aQ);
}

//--------------------------- Support Equations  -----------------------------
/*
       Name: REsonantFreq

       Type: Function (double)

       Parameters:
                   aL - Inductance (H)
		   aC - Capacitance (F)

       Returns: Resonant frequency of LC circuit (Hz)

       Note: Support function: determines the resonant frequency of filter
             resonant branches.
*/

double Bandpass::ResonantFreq(double aL, double aC)
{
    double Denom, RetVal;

    Denom= aL * aC;

#ifdef INSTR
  printf("In function Bandpass::ResonantFreq\n");
  printf("Input Values: aL= %f aC= %f\n", aL, aC);
  printf("Denom= %f\n", Denom);
#endif

    if (! Denom) return 0;
    RetVal= pow(Denom, -0.5);
    return CYCLIC(RetVal);
}

//-------------------------------- bandpass.cpp --------------------------------
