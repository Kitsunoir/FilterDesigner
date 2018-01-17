//------------------------------- sdsplmsgbox.h  -------------------------------

/***************************************************************************
                            -  Description
                             -------------------
    Begin                : 6 December, 2006
    Copyright            : (C) 2006 by Simo
    E-Mail               : simo (at) furrydolphin (dot) net
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
 ***************************************************************************/

#ifndef SMB_H
#define SMB_H
#include <fox-1.6/fx.h>

/*
          Name: SplMsgBox

	  Type: Daughter Class

	  Ancestor: FXDialogBox (FOX Class library)

	  Note: Creates a message display dialog box. Use is
                advisory only; text is not editable.
*/

class SplMsgBox : public FXDialogBox
{
    FXDECLARE(SplMsgBox)  //  FOX "Boilerplate" Code Macro

    public:
            SplMsgBox(FXApp *aApp);
            virtual ~SplMsgBox(void);

	    enum
	    {
		ID_CLOSE= FXDialogBox::ID_LAST,
		ID_LAST
	    };

    protected:
	       FXIcon *DolphinIco, *ExitIco;

    private:
	       //  FOX Maintenance: Default and copy constructors declared here to
	       //                   prevent compiler from supplying public ones.

               SplMsgBox(void) {}
               SplMsgBox(const SplMsgBox&);
};
#endif

//------------------------------- sdsplmsgbox.h  -------------------------------
