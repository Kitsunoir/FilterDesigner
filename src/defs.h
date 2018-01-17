//----------------------------------- defs.h -----------------------------------

/***************************************************************************
                            -  Description
                             -------------------
    Begin                : 9 September, 2016
    Copyright            : (C) 2016 by Simo
    E-Mail               : simo@furrydolphin.net
    Version              : 1.1.0
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

     Note: This file contains preprocessor macros that are related to the
           The details of the network designer. Should there be a code revision,
           these values may be altered to accomodate the updates.

*/

#ifndef DEFS_H
#define DEFS_H

//          Filter ID Codes

//     Bandpass Filters


#define BW_PTC    2
#define BW_PTLC   4
#define BW_STC    6
#define BW_STLC   8
#define MULTI_FB  10
#define STATE_VAR 12

//     Passive Filters

#define LP_CIN 14
#define LP_LIN 16
#define HP_CIN 18
#define HP_LIN 20

//     Active Filters

#define ACTIVE_LPF 22
#define ACTIVE_HPF 24

//     Filter Class

#define CLASS_NONE 0
#define ACTIVE     1
#define PASSIVE    2

//     Filter Characteristics

#define CHAR_NONE   0
#define BUTTERWORTH 1
#define CHEBYCHEV   2
#define BESSEL      3

//     Bessel Optimizations

#define NOOPT 0
#define FREQ  1
#define TIME  2

//     Constants

#ifndef PI
#define PI 3.141592653589793116
#endif

#define MAXORDER 10
#define TWO_PI   6.283185307179586232
#define FOUR_PI  12.56637061435917246

#define REX_PAT "\\(([^,]*),([^,]*)\\)"

//                               Macro Functions

/*
        Name: ANGULAR

	Type: Real Macro function

	Parameters: Cyclic frequency (Hz)

	Returns: Angular frequency (rads/sec)
*/

#define ANGULAR(f) (6.283185307179586232*(f))

/*
        Name: CYCLIC

	Type: Real Macro Function

	Parameters: Angular frequency (rad/sec)

	Returns: Cyclic frequency (Hz)
*/

#define CYCLIC(w) ((w)/6.283185307179586232)

/*
        Name: POW2

	Type: Real/Integer Macro function

	Parameters: Real or Integer

	Returns: Square of arguement
*/

#define POW2(x) ((x)*(x))

/*
       Name: CEIL

       Type: Integer Macro Function

       Parameters: x (int) -- Length of string for which dynamic memory is
                   allocated.

       Returns: The next closest value that'll accomodate this string length.

       Note: Force carry by adding 15 to x. Next, bitwise AND with -16 to determine
             where the carry bit was set. Allocate sufficient memory for a string
	     that insures against malloc arena corruption.
*/

#define CEIL(x) (((x)+15) & -16)

/*
     Name: ARCSINH

     Type: Real macro function

     Parameters: x (double)

     Returns: The inverse of the hyperbolic sine function
*/

#define ARCSINH(x) (log((x)+sqrt(1.0+((x)*(x)))))

/*
     Name: ARCCOSH

     Type: Real macro function

     Parameters: x (double)

     Returns: The inverse of the hyperbolic cosine function
*/

#define ARCCOSH(x) (log((x)+sqrt((x)+1)*sqrt((x)-1)))

#endif

//----------------------------------- defs.h -----------------------------------
