//---------------------------------- appwin.h ----------------------------------

/***************************************************************************
                            -  Description
                             -------------------
    Begin                : 9 Septemver, 2016
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

#ifndef DESWIN_H
#define DESWIN_H

#include <fox-1.6/fx.h>
#include "defs.h"

//              Convenience Macros

#define DISABLE FXWindow::ID_DISABLE
#define ENABLE  FXWindow::ID_ENABLE

#ifndef LAYOUT_FIX
#define LAYOUT_FIX LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT
#endif

#ifndef LAYOUT_CENTER
#define LAYOUT_CENTER LAYOUT_CENTER_X|LAYOUT_CENTER_Y
#endif

//------------------------------------------------------------------------------

/*
       Name: AppWin

       Type: Daughter Class

       Ancestor: FXMainWindow

       Methods: See sdmainwindow.cpp for details

       Note: Main window object
*/

class AppWin : public FXMainWindow
{
    FXDECLARE(AppWin)

//                              Class Methods

    public:

        AppWin(FXApp *aApp);
        virtual ~AppWin(void);
        virtual void create(void);

//                    Event Handlers: Command

	long ClickMenuItm(FXObject*, FXSelector, void*);
	long ClickCmdBtn(FXObject*, FXSelector, void*);
	long ClickOptItm(FXObject*, FXSelector, void*);
	long ClickRadioBtn(FXObject*, FXSelector, void*);

//                    Event Handlers: Update

	long UpdateLineEdit(FXObject*, FXSelector, void*);
	long UpdateCmdBtn(FXObject*, FXSelector, void*);
	long UpdateRadioBtn(FXObject*, FXSelector, void*);
	long UpdateOptions(FXObject*, FXSelector, void*);
	long UpdateMainMenu(FXObject*, FXSelector, void*);

    protected:

        AppWin(void) {}
	FXdouble GetData(FXTextField*);
	FXuint ActiveFltrDesigner(const FXuint, const FXdouble, FXdouble*, FXdouble*);
	FXuint PassiveFltrDesigner(const FXuint, const FXdouble, FXdouble*);
	const FXString& FindBrowser(void);
	const FXString& PBPFDesigner(FXdouble, FXdouble, FXdouble, const FXuint);
	const FXString& ABPFDesigner(FXdouble, FXdouble, FXdouble, const FXuint);
        const FXString& FormatData(FXdouble, const FXchar);
	const FXString& ScaleActive(const FXuint, const FXdouble, const FXdouble, const FXdouble, FXdouble*, FXdouble*);
	const FXString& ScalePassive(const FXuint, const FXdouble, const FXdouble, const FXdouble, FXdouble*);
	void PassiveFltrDesigner(const FXuint, const FXdouble, FXdouble*, const FXuint);
	void WriteOut(const FXString&, const FXuint);
	void OpenDlgs(FXSelector);
	void ProjectSave(void);
	void ProjectPrint(void);

    private:

        AppWin(const AppWin&);
        AppWin& operator=(const AppWin&);


//                   Variables

    public:

        enum
	{
//                Menu Command IDs

            ID_NEW= FXMainWindow::ID_LAST,
	    ID_SAVE,
	    ID_PRINT,
	    ID_XNL,
	    ID_PBPF,
	    ID_ABPF,
	    ID_PFLTR,
	    ID_AFLTR,
	    ID_CNTS,
	    ID_ABOUT,
	    ID_EXIT,
	    ID_DOLPHINS,

//               Option Menu IDs

	    ID_BP_PTC,
	    ID_BP_PTLC,
	    ID_BP_STC,
	    ID_BP_STLC,
	    ID_MULTIFB,
	    ID_STATEVAR,
	    ID_LP_CIN,
	    ID_LP_LIN,
	    ID_HP_CIN,
	    ID_HP_LIN,
	    ID_ACTIVE_LP,
	    ID_ACTIVE_HP,

//              Radio Btn IDs

	    ID_BUTTER,
	    ID_CHEBYCHEV,
	    ID_BESSEL,
	    ID_RL_INF,
	    ID_FREQ,
	    ID_TIME,

//              Command Btn IDs

	    ID_OK,
	    ID_CLR,
	    ID_OK_PBPF,
	    ID_OK_ABPF,
	    ID_OK_PFLTR,
	    ID_OK_AFLTR,
	    ID_CLR_BPF,
	    ID_CLR_FLTR,

//              Text Field IDs

	    ID_FLTR_R,
	    ID_FLTR_RL,
	    ID_GAIN,
	    ID_C_TRIAL,
	    ID_LOWER,
	    ID_UPPER,
	    ID_ORDER,
	    ID_CORNER,
	    ID_RIPPLE,

	    ID_TITLE,
	    ID_LAST
	};

        enum
	{
            MAIN_PANE= FXSwitcher::ID_OPEN_FIRST,
	    BPF_PANE,
            FLTR_PANE
	};

        enum
	{
            PROJ_NONE= 0,
            PROJ_PEND,
            PROJ_COMP
        };

        enum
	{
            NOT_SAVED= 0,
            SAVED
	};

        FXSwitcher     *MainPanel;
        FXImageView    *MainDisp, *SchemosLbl;
        FXButton       *OKCmdBtn, *XnlCmdBtn, *ClrCmdBtn, *ExitCmdBtn;
        FXSpinner      *OrderSB;
        FXLabel        *StatusLbl, *RValueLbl;
        FXTextField    *UsrEntry[8];

        FXPopup        *TplPop, *FltrPop;
        FXOptionMenu   *TplOpt, *FltrOpt;
        FXText         *BPFDesc, *FltrDesc;
        FXStatusBar    *MainSB;
	FXToolBarShell *MenuSh, *ToolSh;
	FXMenuPane     *FileMain, *ProjMain, *HelpMain;

    protected:

        FXIcon *PrintIco, *SaveIco, *NewIco, *CntsIco,  *AboutIco,
	       *ExitIco,  *SmDolphinIco, *XnlIco, *ClrIco, *OKChkIco,
	       *LEDGrayIco, *LEDOnIco, *LEDOffIco, *BPFPTCIco, *BPFPTLCIco,
	       *BPFSTCIco, *BPFSTLCIco, *MFBIco, *SVarIco;

    private:

        FXApp *ThisApp;

        FXuint FilterTypeID, FilterCharID, FilterClassID;
        FXuint ProjState;
        FXuint BesselOpt;
        FXuint SaveState;
	FXuint RLinf;
        FXuint Order;

        FXGIFImage *ProjSchemos, *Welcome;
        FXString   *TxtResults;
};

#endif

//---------------------------------- appwin.h ----------------------------------
