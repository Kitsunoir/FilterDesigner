//------------------------------------- filters.cpp  -------------------------------------

/***************************************************************************
                            -  description
                             -------------------
    Begin                : 9 March, 2017
    Copyright            : (C) 2017 by Simo
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

#ifdef INSTR
#include <stdio.h>
#endif

#include <math.h>
#include <string.h>
#include <unistd.h>
#include "filters.h"
#include "besselcons.h"
#include "locations.h"

/*
     Name: LPActiveBW

     Type: Void function

     Parameters:
                 aOrd -- The filter order

		 C_1 (FXdouble pointer) -- The values for C_1 of each
		 first/second order section of an active Butterworth LPF

		 C_2 (FXdouble pointer) -- The values for C_2 of each
		 second order section.

     Returns: FXdouble arrays containing the normalized C values

     Note: Designs a Butterworth LPF
*/

void Filters::LPActiveBW(const FXint aOrd, const FXint aSec, FXdouble *C_1, FXdouble *C_2)
{
#ifdef INSTR
  printf("\nEntering function Filters::LPActiveBW");
#endif

    FXint i, k;
    FXdouble Theta, Sigma, Omega;
    FXdouble temp_1[aSec];
    FXdouble temp_2[aSec];

//                Note: This algorithm produces the C values in reverse order

    i= 0;
    k= 1;

    while (1)
    {
      Theta= ((2.0*k - 1) * PI)/(2.0*aOrd);

      if (Theta > (0.5*PI)) break;

      Sigma= sin(Theta);
      Omega= cos(Theta);
      temp_1[i]= 1.0/Sigma;
      temp_2[i]= (Omega > 1.0E-9) ? 1.0/(temp_1[i]*(POW2(Sigma) + POW2(Omega))) : 0;

#ifdef INSTR
  printf("\n%d) Theta= %f", k, Theta);
  printf("\n%d) Sigma= %f", k, Sigma);
  printf("\n%d) Omega= %f", k, Omega);
  printf("\n%d) C_1= %f", k, temp_1[i]);
  printf("\n%d) C_2= %f", k, temp_2[i]);
#endif

      k++;
      i++;
    }

    k= aSec;

//              Note: Put the return values in correct order

    for (int j= 0; j < aSec; j++)
    {
      k--;
      C_1[j]= temp_1[k];
      C_2[j]= temp_2[k];
    }


#ifdef INSTR
  printf("\nLeaving function: Filters::LPActiveBW");
#endif
}

//------------------------------------------------------------------------------

/*
     Name: LPActiveChev

     Type: Void function

     Parameters:
                 aOrd -- The filter order

		 C_1 (FXdouble pointer) -- The normalized C_1 values of the design

		 C_2 (FXdouble pointer) -- The normalozed C_2 values of the design

     Returns: FXdouble arrays containing the design C values (normalized)

     Note: Designs an active Chebychev LPF
*/

void Filters::LPActiveChev(const FXint aOrd, const FXint aSec, FXdouble aRfac, FXdouble *C_1, FXdouble *C_2)
{
#ifdef INSTR
  printf("\nEntering function: Filters::LPActiveChev");
#endif

    FXint i, k;
    FXdouble A, Theta, Sigma, Omega, Epsilon;
    FXdouble temp_1[aSec];
    FXdouble temp_2[aSec];

    FXdouble temp= pow(10, (aRfac/10.0)) - 1.0;
    Epsilon=       sqrt(temp);
    A=             (1.0/aOrd) * ARCSINH(1.0/Epsilon);

#ifdef INSTR
  printf("\nEpsilon= %f", Epsilon);
  printf("\nA= %f", A);
#endif

//              Note: This algorithm produces its results in reverse order

    i= 0;
    k= 1;

    while (1)
    {
      Theta= ((2.0*k - 1) * PI)/(2.0*aOrd);

      if (Theta > (0.5*PI)) break;

      Sigma= sinh(A) * sin(Theta);
      Omega= cosh(A) * cos(Theta);
      temp_1[i]= 1.0/Sigma;
      temp_2[i]= (Omega > 1.0E-9) ? 1.0/(temp_1[i]*(POW2(Sigma) + POW2(Omega))) : 0;

#ifdef INSTR
  printf("\n%d) Theta= %f", k, Theta);
  printf("\n%d) Sigma= %f", k, Sigma);
  printf("\n%d) Omega= %f", k, Omega);
  printf("\n%d) C_1= %f", k, temp_1[i]);
  printf("\n%d) C_2= %f", k, temp_2[i]);
#endif

      k++;
      i++;
    }

    k= aSec;

//              Note: Put the return values in correct order

    for (int j= 0; j < aSec; j++)
    {
      k--;
      C_1[j]= temp_1[k];
      C_2[j]= temp_2[k];
    }

#ifdef INSTR
    printf("\nLeaving function: Filters::LPActiveChev");
#endif
}

//------------------------------------------------------------------------------

/*
     Name: LPActiveBes

     Type: Integer function

     Parameters:
                 aOrd (integer) -- the filter order
		 *C_1 (real pointer) -- Array of the normalized C_1 values
		 *C_2 (real pointer) -- Array of the normalized C_2 values
		 aOptID (integer) -- Filter optimization code

     Returns: Error code
*/

FXuint Filters::LPActiveBes(const FXint aOrd, FXdouble *C_1, FXdouble *C_2, const FXuint aOptID)
{
#ifdef INSTR
  printf("\nEntering function: Filters::LPActiveBes\n");
#endif

    FXString FmtStr;
    FXdouble Real[aOrd];
    FXdouble Imag[aOrd];
    FXdouble Omega, Sigma;
    FXuint   RetCode;
    FXint    j= 0;

    FmtStr= FXStringFormat("dri\n0\n%d", aOrd);

    for (int i= 0; i <= aOrd; i++) FmtStr.append(FXStringFormat("\n%d", BesselCoeffs[aOrd-2][i]));

    FmtStr.append('\n');

#ifdef INSTR
  printf("\nFmtStr= %s", FmtStr.text());
#endif

    if ((RetCode= GetPoles(FmtStr, Real, Imag))) return RetCode;

    for (int i= 0; i < aOrd; i++)  //  Note: Results are in the correct order
    {
      if (Imag[i] >= 0)
      {
	Sigma= (aOptID == FREQ) ? -Real[i]/BesselFreqScale[aOrd-2] : -Real[i];
	Omega= (aOptID == FREQ) ? Imag[i]/BesselFreqScale[aOrd-2] : Imag[i];
	C_1[j]= 1.0/Sigma;
	C_2[j]= (Omega > 1.0E-9) ? 1.0/(C_1[j] * (POW2(Sigma) + POW2(Omega))) : 0;

#ifdef INSTR
  printf("\nC_1[%d]= %E", j, C_1[j]);
  printf("\nC_2[%d]= %E", j, C_2[j]);
#endif
	j++;
      }
    }
#ifdef INSTR
    printf("\nLeaving function: Filters::LPActiveBes");
#endif

    return RetCode;
}

//------------------------------------------------------------------------------

void Filters::LPPassiveBW(const FXint aOrd, FXdouble *aLCVals, FXuint aRinf)
{
#ifdef INSTR
  printf("Entering function: Filters::LPPassiveBW\n");
#endif

  FXdouble a, b;
  FXint k;
  FXdouble temp[aOrd];

  if (! aRinf)  //  Note: LC Values correct for Rl = Rs
  {

#ifdef INSTR
  printf("Case Rs = Rl\n");
#endif

//                Note: This algorithm produces the values in reverse order

    for (FXint i= 1; i <= aOrd; i++)
    {
      a= ((2*i - 1)*PI)/(2.0 * aOrd);
      temp[i-1]= 2.0 * sin(a);

#ifdef INSTR
  printf("LCVals[%d]= %f\n", i-1, temp[i-1]);
#endif

    }
  }
  else  //  Note: LC Values correct for Rl >> Rs
  {

#ifdef INSTR
  printf("Case: Rl >= 10Rs\n");
#endif

    FXdouble Ar[2];
    FXdouble Br[2];

    Ar[0]=   sin(PI/(2.0*aOrd));
    Br[0]=   1.0;
    temp[0]= Ar[0];

//               Note: This algorithm produces the LC values in reverse order

    for (FXint R= 2; R <= aOrd; R++)
    {
      a=     ((2.0*R - 1)*PI)/(2.0*aOrd);
      b=     ((R - 1)*PI)/(2.0*aOrd);
      Ar[1]= sin(a);
      Br[1]= POW2(cos(b));

#ifdef INSTR
  printf("A_%d= %f\n", R, Ar[1]);
  printf("B_%d= %f\n", R, Br[1]);
  printf("LCVals_%d= %f\n", R-1, temp[R-2]);
#endif

      temp[R-1]= (Ar[1]*Ar[0])/(Br[1]*temp[R-2]);
      memmove(Ar, Ar+1, sizeof(FXdouble));
      memmove(Br, Br+1, sizeof(FXdouble));

#ifdef INSTR
      printf("Ar[0]= %f\n", Ar[0]);
      printf("Ar[1]= %f\n", Ar[1]);
      printf("Br[0]= %f\n", Br[0]);
      printf("Br[1]= %f\n", Br[1]);
#endif

    }
  }

//               Note: Put the return values in the correct order

    k= aOrd;

    for (int j= 0; j < aOrd; j++)
    {
      aLCVals[j]= temp[--k];
    }

#ifdef INSTR
  for (FXint i= 0; i < aOrd; i++)
  {
      printf("LCVal_%d= %f\n", i+1, aLCVals[i]);
  }
#endif

#ifdef INSTR
  printf("Leaving function: Filters::LPPassiveBW\n");
#endif
}

//------------------------------------------------------------------------------

void Filters::LPPassiveChev(const FXint aOrd, FXdouble aPBR, FXdouble *aLCVals, const FXuint aRinf)
{
#ifdef INSTR
  printf("\nEntering function: Filters::LPPassiveChev");
#endif

    FXdouble a, e, SF;
    FXdouble temp[aOrd];
    FXint k;

    e= sqrt(pow(10, (0.1*aPBR)) - 1);

#ifdef INSTR
  printf("\nInitial Values:");
  printf("\ne= %f", e);
#endif

    if (! aRinf)
    {
      FXdouble Ar, Br_0;
      FXdouble Br[2];

      Ar= 2.0 * sin(PI/(2*aOrd));
      Br_0= Br[0]= sinh((1.0/aOrd) * ARCSINH(1.0/e));
      temp[0]= Ar/Br_0;

#ifdef INSTR
  printf("\nAr= %f", Ar);
  printf("\nBr_0= %f", Br_0);
  printf("\ntemp[0]= %f", temp[0]);
#endif

      for (int R= 2; R <= aOrd; R++)  //  Note: This algorithm produces the LC Values in reverse order
      {
	  a= (2.0*R - 1)/(2.0*aOrd);
	  Ar= 2.0 * sin(a);
	  Br[1]= (1.0/Br[0]) * (POW2(Br_0) + POW2(sin(a)));
	  temp[R-1]= Ar/Br[1];
	  memmove(Br, Br+1, sizeof(FXdouble));

#ifdef INSTR
  printf("\ntemp[%d]= %f", R-1, temp[R-1]);
  printf("\nAr= %f", Ar);
  printf("\nBr[0]= %f (after memmove)", Br[0]);
  printf("\nBr[1]= %f", Br[1]);
#endif

      }

      k= aOrd;

      for (int j= 0; j < aOrd; j++)
      {
        aLCVals[j]= temp[--k];

#ifdef INSTR
	printf("\naLCVals[%d]= %f", j, aLCVals[j]);
#endif
      }

    }
    else
    {
      FXdouble b, beta, gamma;
      FXdouble A_r[2], B_r[2];

      beta=  2.0 * ARCSINH(1.0/e);
      gamma= sinh(beta/(2.0*aOrd));
      SF=    cosh((1.0/aOrd) * ARCCOSH(1.0/e));

#ifdef INSTR
  printf("\nIn case: Rl >> Rs");  //  Note: Case OK
#endif

      A_r[0]= sin(PI/(2.0*aOrd));
      B_r[0]= (POW2(gamma) + POW2(sin(PI/(2.0*aOrd)))) * POW2(cos(b));
      temp[0]= A_r[0]/gamma;

#ifdef INSTR
  printf("\nA_r[0]= %f (initial value)", A_r[0]);
  printf("\nB_r[0]= %f (initial value)", B_r[0]);
#endif

      for(FXint R= 2; R <= aOrd; R++)
      {
	a= ((2.0*R - 1)*PI)/(2.0*aOrd);
	b= (R * PI)/(2.0 * aOrd);
	A_r[1]= sin(a);
	B_r[1]= (POW2(gamma) + POW2(sin(b))) * POW2(cos(b));
	temp[R-1]= (A_r[1] * A_r[0])/(B_r[0] * temp[R-2]);
	memmove(A_r, A_r+1, sizeof(FXdouble));
	memmove(B_r, B_r+1, sizeof(FXdouble));

#ifdef INSTR
  printf("\ntemp[%d]= %f", R-1, temp[R-1]);
  printf("\nA_r[0]= %f (after memmove)", A_r[0]);
  printf("\nA_r[1]= %f", A_r[1]);
  printf("\nB_r[0]= %f (after memmove)", B_r[0]);
  printf("\nB_r[1]= %f", B_r[1]);
  printf("\ntemp[%d]= %f", R-1, temp[R-1]);
#endif

      }

      k= aOrd;

      for (int j= 0; j < aOrd; j++)
      {
        aLCVals[j]= SF * temp[--k];

#ifdef INSTR
  printf("\naLCVals[%d]= %f", j-1, aLCVals[j-1]);
#endif
      }
    }
#ifdef INSTR
  printf("\nLeaving function Filters::LPPassiveChev");
#endif
}

//------------------------------------------------------------------------------

void Filters::LPPassiveBes(const FXint aOrd, FXdouble *aLCVals, const FXuint aOptID, const FXuint aRinf)
{
#ifdef INSTR
  printf("\nEntering function: Filters::LPPassiveBes\n");
#endif

    if (aRinf)
    {
      for (int i= 0; i < aOrd; i++)
      {
	  aLCVals[i]= (aOptID == FREQ) ? BesselFreqLC_Rinf[aOrd-2][i] : BesselTimeLC_Rinf[aOrd-2][i];
      }
    }
    else
    {
        for (int i= 0; i < aOrd; i++)
        {
          aLCVals[i]= (aOptID == FREQ) ? BesselFreqLC[aOrd-2][i] :
	                             BesselTimeLC[aOrd-2][i];
        }
    }
#ifdef INSTR
  for (int i= 0; i < aOrd; i++)
  {
    printf("\naLCVals[%d]= %12.10f", i, aLCVals[i]);
  }
  printf("\nLeaving function: Filters::LPPassiveBes\n");
#endif
}

//------------------------------------------------------------------------------
/*
     Name: GetPoles

     Type: Integer function

     Params: aFmt (FXString reference) -- The format file MPSolve reads
             aReal (double pointer) -- The real part of a 2nd order poly
	     aImag (double pointer) -- The imaginary part

     Returns: aReal and aImag real arrays via pointer
              An error code: 0) No error
                             1) Failed to parse MPSolve output
*/

FXuint Filters::GetPoles(const FXString &aFmt, FXdouble *aReal, FXdouble *aImag)
{
#ifdef INSTR
  printf("\nEntering function: Filters::GetPoles\n");
#endif

    FXString FmtFileName= FXStringFormat("/tmp/poly-%s.mps", VERSION);
    FXString Sysarg, ComplexStr;
    FXchar   Buffer[BUFSIZ];
    FXchar   OneLine[BUFSIZ];
    FXchar  *Head, *Tail, C;
    FXint    Bytesread, i= 0;
    FXint    Beg[3], End[3];
    FILE     *MPSResults;

    FXFile FileWriter(FmtFileName, FXFile::Writing);
    FileWriter.writeBlock(aFmt.text(), aFmt.length());
    Sysarg= "unisolve " + FmtFileName;
    memset(Buffer, '\0', sizeof(Buffer));
    MPSResults= popen(Sysarg.text(), "r");
    ComplexStr.clear();

    if (! MPSResults)
    {
	pclose(MPSResults);
	FileWriter.close();
	FileWriter.remove(FmtFileName);
    }

    do { Bytesread= fread(Buffer, sizeof(Buffer), BUFSIZ, MPSResults); } while (Bytesread > 0);

#ifdef INSTR
    printf("\nBuffer= %s", Buffer);
#endif

    pclose(MPSResults);
    FileWriter.close();
    FileWriter.remove(FmtFileName);
    FXRex Matcher(REX_PAT, REX_CAPTURE);
    Head= Tail= Buffer;

    while ((C= *Head++))
    {
	if (C == '\n')
	{
	    memcpy(&OneLine[0], Tail, Head-Tail);
	    OneLine[Head-Tail-1]= '\0';
	    ComplexStr= OneLine;

#ifdef INSTR
  printf("\nOneLine= %s", OneLine);
#endif

	    if (Matcher.match(ComplexStr, Beg, End, REX_FORWARD, 3))
	    {
	      aReal[i]= FXDoubleVal(ComplexStr.mid(Beg[1], End[1]-Beg[1]));
	      aImag[i++]= FXDoubleVal(ComplexStr.mid(Beg[2], End[2]-Beg[2]));
#ifdef INSTR
  printf("\naReal[%d]= %f", i-1, aReal[i-1]);
  printf("\naImag[%d]= %f", i-1, aImag[i-1]);
#endif
	    }
	    else return 1;

	    Tail= Head;
	}
    }
#ifdef INSTR
    printf("\nLeaving function: Filters::GetPoles");
#endif
    return 0;
}
