//---------------------------- bandpass.h ----------------------------

/***************************************************************************
                            -  description
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
 ***************************************************************************

*/

#ifndef BPF_H
#define BPF_H

#include "defs.h"

/*
        Name: Bandpass

	Type: Namespace

	Note: Collects all the design equations for four configurations
              of passive, bandpass filters and two active bandpass
              designs: the MFB and State Variable.
*/

typedef unsigned int BPuint;

namespace Bandpass
{

/*
                                   Passive BPFs

    Note: This section designs passive bandpass filters derived from
          Butterworth LPF's. Four varieties are presented here.
*/

    double L_Tune(double, double, double, BPuint);
    double C_Tune(double, double, double, BPuint);
    double C_Couple(double, double, double, BPuint);
    double L_Couple(double, double, double, BPuint);

/*
                                   Active BPF's

    Note: Design of two active BPF's: Multi-feedback and State Variable
*/

    double K_factor(double, double, double, double&, double&);
    double R_One(double, double);
    double R_Two(double, double);
    double R_Three(double, double);
    double MainResistor(double, double, double, double&, double&);
    double GainSetResistor(double, double);
    double DFSetResistor(double, double);

//---------------------------- Functions: Support ------------------------------

    double ResonantFreq(double, double);
}

#endif

//------------------------------- sdpassivebpf.h -------------------------------
