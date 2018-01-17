//------------------------------ sdsplmsgbox.cpp  ------------------------------

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

#include "splmsgbox.h"
#include "freedolphins.h"
#include "icons.h"

//       Make a subroutine/message map

FXDEFMAP(SplMsgBox) SMBMethodMap[]=
{
    FXMAPFUNC(SEL_CLOSE, SplMsgBox::ID_CLOSE, FXDialogBox::onCmdAccept)
};

FXIMPLEMENT(SplMsgBox, FXDialogBox, SMBMethodMap, ARRAYNUMBER(SMBMethodMap))

/*
       Name: SplMsgBox

       Type: Subroutine (Constructor)

       Parameters: aOwner (Pointer: FXWindow) - Pointer to parent window whence called

       Note: Construct a message display dialog -- pass title and geometry info to
             base class constructor
*/

SplMsgBox::SplMsgBox(FXApp *aApp) :
FXDialogBox(aApp, "A Special Message for You", DECOR_TITLE|DECOR_BORDER|DECOR_RESIZE|DECOR_CLOSE,
            0, 0, 485, 515, 6, 6, 6, 6, 4, 4)
{
    FXVerticalFrame   *MainLO;
    FXHorizontalFrame *CmdBtnLO;
    FXText            *Display;
    FXButton          *CloseBtn;

    DolphinIco= new FXGIFIcon(aApp, atldolphin_gif);
    ExitIco=    new FXGIFIcon(aApp, Exit_gif);

    setMiniIcon(DolphinIco);
    setTarget(this);
    setSelector(ID_CLOSE);

/*
                           Create Main Window

			   I) Main Layout (Vertical Frame)
			      A) Multi-line Text Field: Read only
			      B) Command Button Group
			         1) One Command Button
*/

    MainLO=   new FXVerticalFrame(this, LAYOUT_SIDE_TOP|LAYOUT_FILL|FRAME_SUNKEN|FRAME_THICK);
    Display=  new FXText(MainLO, NULL, 0, TEXT_READONLY|TEXT_WORDWRAP|LAYOUT_FILL);
    CmdBtnLO= new FXHorizontalFrame(MainLO, LAYOUT_SIDE_BOTTOM|FRAME_GROOVE|LAYOUT_FILL_X);
    CloseBtn= new FXButton(CmdBtnLO, "&Close", ExitIco, this, FXDialogBox::ID_ACCEPT,
			   BUTTON_INITIAL|BUTTON_DEFAULT|ICON_BEFORE_TEXT|LAYOUT_RIGHT|
			   FRAME_RAISED|FRAME_THICK, 0, 0, 0, 0, 20, 10, 5, 5);
    Display->setVisibleRows(35);
    Display->setVisibleColumns(95);
    Display->setTabColumns(TABCOLS);
    Display->setText(FXString(SPECIAL_MESSAGE));
    CloseBtn->setFocus();
}

//------------------------------------------------------------------------------
/*
       Name: ~SplMsgBox

       Type: Subroutine (Destructor)

       Parameters: (none)

       Note: Delete the title bar icon, let FOX clean up everything else
*/

SplMsgBox::~SplMsgBox(void)
{
    delete DolphinIco;
    delete ExitIco;
}

//--------------------------- tspecialmsgbox.cpp -----------------------------
