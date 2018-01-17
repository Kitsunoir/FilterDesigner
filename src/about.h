//--------------------------------- about.h ----------------------------------

/***************************************************************************
                            -  Description
                             -------------------
    Begin                : 25 June, 2014
    Copyright            : (C) 2014 by Simo
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

*/

#ifndef ABOUT_H
#define ABOUT_H

#include <fox-1.6/fx.h>

//------------------------------- Text Macros --------------------------------

#define ABOUT \
"\nFilter Designer:"\
"\n"\
"\nAn electronic project design aid:"\
"\nDesign lowpass, highpass, bandpass filters"\
"\n"\
"\nVersion: "VERSION\
"\n"\
"\nSee the on-line help for further details on using"\
"\nFilter Designer"\

#define AUTHORS \
"\nFilter Designer written by: Simo,"\
"\nwith FOX and XEMACS"\
"\n"\
"\nCopyright(C) 2017 by Simo"\
"\nIF SOFTWARE CAN BE FREE, WHY CAN'T DOLPHINS?"

#define CREDITS \
"\nFilter Designer welcome screen by:"\
"\nSalem Pelz Fuchs"\
"\n"\
"\nCopyright (C) 2004 by Salem Pelz Fuchs"\
"\nTNX Salem"

//------------------------------------------------------------------------------

#define FILE_ERROR \
"Unable to find or load the file: %s."\
"\nCorrupt or lost?\n"

class About : public FXDialogBox
{
    FXDECLARE(About)
    public:
                 About(FXApp *aApp);
                 virtual ~About(void);

    protected:
                 About(void) {}

    private:
                 About(const About&);
                 About& operator=(const About&);

    public:
                 enum
	         {
	             ID_TITLE= FXDialogBox::ID_LAST,
	             ID_LAST
	         };

    protected:
        	 FXIcon  *DolphinLogo, *OKCheck;
                 FXImage *FreeMe;

    private:
		 FXApp *ThisApp;
};

#endif

//--------------------------------- about.h ----------------------------------
