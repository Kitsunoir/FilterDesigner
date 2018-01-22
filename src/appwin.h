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

class DataDict;

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
	    ID_ANOTCH,
	    ID_PNOTCH,
	    ID_LOWPASS,
	    ID_HIGHPASS,
	    ID_CIN,
	    ID_LIN,
	    ID_ACTIVE,
	    ID_BUTTERWORTH,
	    ID_BESSEL,
	    ID_CHEV,
	    ID_INV_CHEV,

//              Radio Btn IDs

	    ID_DECIBEL,
	    ID_LINEAR,
	    ID_FREQ,
	    ID_TIME,
	    ID_CHECK_INF,

//              Command Btn IDs

	    ID_OK,
	    ID_CLR,
	    ID_DESIGN,
	    ID_DES_PBPF,
	    ID_DES_ABPF,
	    ID_DES_PFLTR,
	    ID_DES_AFLTR,
	    ID_CLR_BPF,
	    ID_CLR_FLTR,

//              Text Field IDs

	    ID_UPPER,
	    ID_LOWER,
	    ID_FLTR_R,
	    ID_GAIN,
	    ID_C_TRIAL,
	    ID_PB_FREQ,
	    ID_SB_FREQ,
	    ID_ATTN_PASS,
	    ID_ATTN_STOP,
	    ID_ORDER_SPIN,
	    ID_CUTOFF,
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
	    CHAR_PANE= FXSwitcher::ID_OPEN_FIRST,
	    IMPL_PANE
	};

        enum
	{
            PROJ_NONE= 0,
            PROJ_PENDING,
            PROJ_COMPLETE
        };

	enum
	{
	    LINEAR= 0,
	    DECIBEL
	};

        enum
	{
            NOT_SAVED= 0,
            SAVED
	};

        FXSwitcher     *MainPanel, *LPFPanel;
        FXImageView    *MainDisp;
	FXImageFrame   *SchemosLbl;
	FXCanvas       *DataView;
        FXButton       *OKCmdBtn, *XnlCmdBtn, *ClrCmdBtn, *ExitCmdBtn;
        FXSpinner      *OrderSB;
        FXLabel        *StatusLbl, *RtrialLbl;
        FXTextField    *UsrEntry[12];

        FXPopup        *TplPop, *FltrPop, *Topl, *LPFchar;
        FXOptionMenu   *TplOpt, *FltrOpt, *LPFcharOM, *LPGTopOM;
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

        FXApp    *ThisApp;
	DataDict *ProjData;

        FXuint FilterTypeID, FilterCharID, FilterClassID;
        FXuint ProjState;
        FXuint BesselOpt;
        FXuint SaveState;
	FXuint RLinf;
        FXuint Order;
	FXuint Dimensions;

        FXGIFImage *ProjSchemos, *Welcome;
        FXString   *TxtResults;
};

#endif

//---------------------------------- appwin.h ----------------------------------
