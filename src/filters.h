//--------------------------------- filters.h  ---------------------------------

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

#ifndef FLTRS_H
#define FLTRS_H

#include <fox-1.6/fx.h>
#include "defs.h"

/*
     Name: Filters

     Type: Namespace

     Note: Functions for the design of lowpass/highpass filters.
*/

namespace Filters
{
  void LPActiveBW(const FXint aOrd, const FXint aSec, FXdouble *C_1, FXdouble *C_2);
  void LPActiveChev(const FXint aOrd, const FXint aSec, FXdouble aRfac, FXdouble *C_1, FXdouble *C_2);
  FXuint LPActiveBes(const FXint aOrd, FXdouble *C_1, FXdouble *C_2, const FXuint aOptID=FREQ);
  void LPPassiveBW(const FXint aOrd, FXdouble *aLCVals, FXuint aRinf);
  void LPPassiveChev(const FXint aOrd, FXdouble aPBR, FXdouble *aLCVals, const FXuint aRinf);
  void LPPassiveBes(const FXint aOrd, FXdouble *aLCVals, const FXuint aOptID, const FXuint aRinf);
  FXuint GetPoles(const FXString&, FXdouble*, FXdouble*);
}

#endif

//--------------------------------- filters.h  ---------------------------------
