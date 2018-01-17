//--------------------------------- about.cpp ----------------------------------

/***************************************************************************
                            -  Description
                             -------------------
    Begin                : 25 June, 2014
    Copyright            : (C) 2017 by Simo
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

#include "about.h"
#include "locations.h"
#include "icons.h"

//     Make the FOX Message/Target Callback Map

FXDEFMAP(About) Callbacks[]=
{
    FXMAPFUNC(SEL_CLOSE, About::ID_TITLE, FXDialogBox::onCmdAccept)
};

FXIMPLEMENT(About, FXDialogBox, Callbacks, ARRAYNUMBER(Callbacks))

/*
       Name: About

       Type: Subroutine (Constructor)

       Parameters: aApp (Pointer: FXApp) -- Pointer to the main event loop

       Note: Create the "About" Box GUI. No other functionality to implement.
*/

About::About(FXApp *aApp) :
FXDialogBox(aApp, "About Filter Designer", DECOR_ALL, 0, 0, 660, 360),
ThisApp(aApp)

{
//        Declare Layout Managers

    FXVerticalFrame   *MainLO;
    FXHorizontalFrame *TabItem0, *TabItem1, *TabItem2, *CmdBtnGrp;
    FXSplitter *SubLO;
    FXTabBook *Tabber;

//        Initialize Icons

    DolphinLogo= new FXGIFIcon(ThisApp, atldolphin_gif);
    OKCheck=     new FXGIFIcon(ThisApp, OKCheck_gif);
    FreeMe=      new FXGIFImage(ThisApp, NULL, IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);

    FXString ImgFile(ETC_DIR);
    ImgFile+= "FreeMe.gif";

    FXFileStream ImageStr;

    if (ImageStr.open(ImgFile.text(), FXStreamLoad))
    {
	FreeMe->loadPixels(ImageStr);
	ImageStr.close();
	FreeMe->create();
    }
    else fprintf(stderr, FILE_ERROR, ImgFile.text());

    setMiniIcon(DolphinLogo);
    setTarget(this);
    setSelector(ID_TITLE);

    MainLO=     new FXVerticalFrame(this, LAYOUT_SIDE_TOP|LAYOUT_FILL|FRAME_NORMAL);
    SubLO=      new FXSplitter(MainLO, SPLITTER_HORIZONTAL|SPLITTER_TRACKING|LAYOUT_FILL|FRAME_NORMAL);
                new FXImageView(SubLO, FreeMe, NULL, 0, IMAGEVIEW_NORMAL|LAYOUT_FILL|FRAME_GROOVE, 0, 0, 270, 270);
    Tabber=     new FXTabBook(SubLO, NULL, 0, TABBOOK_BOTTOMTABS|LAYOUT_FILL|FRAME_NORMAL);
                new FXTabItem(Tabber, "About", NULL, TAB_BOTTOM_NORMAL);
    TabItem0=   new FXHorizontalFrame(Tabber, LAYOUT_FILL|FRAME_GROOVE);
                new FXLabel(TabItem0, ABOUT, NULL, JUSTIFY_LEFT|ICON_BEFORE_TEXT);
		new FXTabItem(Tabber, "Author", NULL, TAB_BOTTOM_NORMAL);
    TabItem1=   new FXHorizontalFrame(Tabber, LAYOUT_FILL|FRAME_GROOVE);
                new FXLabel(TabItem1, AUTHORS, NULL, JUSTIFY_LEFT|ICON_BEFORE_TEXT);
                new FXTabItem(Tabber, "Credits", NULL, TAB_BOTTOM_NORMAL);
    TabItem2=   new FXHorizontalFrame(Tabber, LAYOUT_FILL|FRAME_GROOVE);
                new FXLabel(TabItem2, CREDITS, NULL, JUSTIFY_LEFT|ICON_BEFORE_TEXT);

    CmdBtnGrp=  new FXHorizontalFrame(MainLO, LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|FRAME_NORMAL);
                new FXButton(CmdBtnGrp, "&OK", OKCheck, this, FXDialogBox::ID_ACCEPT,
			     BUTTON_NORMAL|LAYOUT_RIGHT);
}
//------------------------------------------------------------------------------
/*
       Name: ~About

       Type: Subroutine (Destructor)

       Parameters: (none)

       Note: Clean up objects FOX doesn't clean up for us
*/

About::~About(void)
{
    delete DolphinLogo;
    delete OKCheck;
    delete FreeMe;
}

//-------------------------------- tabout.cpp ----------------------------------
