//--------------------------------- appwin.cpp ---------------------------------

/***************************************************************************
                            -  Description
                             -------------------
    Begin                : 9 September, 2016
    Copyright            : (C) 2016 by Simo
    E-Mail               : simo@furrydolphin.net
    Version              : 1.2.1
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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "appwin.h"
#include "bandpass.h"
#include "filters.h"
#include "messages.h"
#include "about.h"
#include "splmsgbox.h"
#include "locations.h"
#include "icons.h"

FXDEFMAP(AppWin) CallBacks[]=
{
    FXMAPFUNCS(SEL_UPDATE,  AppWin::ID_NEW,     AppWin::ID_AFLTR,     AppWin::UpdateMainMenu),
    FXMAPFUNCS(SEL_UPDATE,  AppWin::ID_BP_PTC,  AppWin::ID_ACTIVE_HP, AppWin::UpdateOptions),
    FXMAPFUNCS(SEL_UPDATE,  AppWin::ID_FLTR_R,  AppWin::ID_RIPPLE,    AppWin::UpdateLineEdit),
    FXMAPFUNCS(SEL_UPDATE,  AppWin::ID_BUTTER,  AppWin::ID_TIME,      AppWin::UpdateRadioBtn),
    FXMAPFUNCS(SEL_UPDATE,  AppWin::ID_OK,      AppWin::ID_CLR_FLTR,  AppWin::UpdateCmdBtn),
    FXMAPFUNCS(SEL_COMMAND, AppWin::ID_NEW,     AppWin::ID_DOLPHINS,  AppWin::ClickMenuItm),
    FXMAPFUNCS(SEL_COMMAND, AppWin::ID_BP_PTC,  AppWin::ID_ACTIVE_HP, AppWin::ClickOptItm),
    FXMAPFUNCS(SEL_COMMAND, AppWin::ID_BUTTER,  AppWin::ID_TIME,      AppWin::ClickRadioBtn),
    FXMAPFUNCS(SEL_COMMAND, AppWin::ID_OK,      AppWin::ID_CLR_FLTR,  AppWin::ClickCmdBtn),
    FXMAPFUNC(SEL_CLOSE,    AppWin::ID_TITLE,   AppWin::ClickMenuItm),
    FXMAPFUNC(SEL_SIGNAL,   AppWin::ID_EXIT,    AppWin::ClickMenuItm)
};

FXIMPLEMENT(AppWin, FXMainWindow, CallBacks, ARRAYNUMBER(CallBacks))

/*
     Name: AppWin

     Type: Constructor

     Parameters: aApp (Pointer: FXApp class) -- Pointer to the main event loop
*/

AppWin::AppWin(FXApp *aApp) :
FXMainWindow(aApp, "FilterDesigner", NULL, NULL, DECOR_ALL, 0, 0, 727, 636),
ThisApp(aApp),
FilterTypeID(0),
FilterCharID(0),
FilterClassID(CLASS_NONE),
ProjState(PROJ_NONE),
BesselOpt(NOOPT),
SaveState(SAVED),
RLinf(0),
ProjSchemos(0),
Welcome(0),
TxtResults(0)
{

//                           Icons

    SmDolphinIco=  new FXGIFIcon(ThisApp, atldolphin_gif);
    NewIco=        new FXGIFIcon(ThisApp, New_gif);
    SaveIco=       new FXGIFIcon(ThisApp, filesaveas_gif);
    PrintIco=      new FXGIFIcon(ThisApp, fileprint_gif);
    CntsIco=       new FXGIFIcon(ThisApp, help_gif);
    AboutIco=      new FXGIFIcon(ThisApp, about_gif);
    XnlIco=        new FXGIFIcon(ThisApp, CancelX_gif);
    ClrIco=        new FXGIFIcon(ThisApp, clear_up_gif);
    OKChkIco=      new FXGIFIcon(ThisApp, OKCheck_gif);
    ExitIco=       new FXGIFIcon(ThisApp, Exit_gif);
    BPFPTCIco=     new FXGIFIcon(ThisApp, PT_BPF_gif, 0, IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    BPFPTLCIco=    new FXGIFIcon(ThisApp, PTLC_BPF_gif, 0, IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    BPFSTCIco=     new FXGIFIcon(ThisApp, ST_BPF_gif, 0, IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    BPFSTLCIco=    new FXGIFIcon(ThisApp, STLC_BPF_gif, 0, IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    MFBIco=        new FXGIFIcon(ThisApp, MultiFB_gif, 0, IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    SVarIco=       new FXGIFIcon(ThisApp, StateVar_gif, 0, IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    LEDGrayIco=    new FXGIFIcon(ThisApp, LedGray_gif, 0, IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    LEDOnIco=      new FXGIFIcon(ThisApp, LedOn_gif, 0, IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    LEDOffIco=     new FXGIFIcon(ThisApp, LedOff_gif, 0, IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);

    LEDGrayIco->create();
    LEDOnIco->create();
    LEDOffIco->create();

//            Set targets and signals: Allow the application to be closed from the title bar

    setTarget(this);
    setSelector(ID_TITLE);

    FXVerticalFrame   *MainLO;
    FXHorizontalFrame *CmdBtnBar, *ImgViewLO;

//                                Create Status Bar

    MainSB= new FXStatusBar(this, LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|STATUSBAR_WITH_DRAGCORNER|FRAME_RAISED);
    new FXButton(MainSB, "\tClick me!\tA special message for you", SmDolphinIco, this,
                 ID_DOLPHINS, LAYOUT_FILL_Y|LAYOUT_RIGHT);
    new FXLabel(MainSB, "Project Status", NULL, JUSTIFY_RIGHT|FRAME_NONE);
    StatusLbl= new FXLabel(MainSB, "No Project", LEDGrayIco, LABEL_NORMAL|FRAME_NORMAL|ICON_BEFORE_TEXT);

//                                Create Main Menu

    FXDockSite *DockUp, *DockDwn, *DockRt, *DockLt;
    FXMenuBar  *MainMenu;
    FXToolBar  *MainTB;

    DockUp=   new FXDockSite(this, LAYOUT_SIDE_TOP|LAYOUT_FILL_X);
    DockDwn=  new FXDockSite(this, LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X);
    DockRt=   new FXDockSite(this, LAYOUT_SIDE_RIGHT|LAYOUT_FILL_Y);
    DockLt=   new FXDockSite(this, LAYOUT_SIDE_LEFT|LAYOUT_FILL_Y);

    MenuSh=   new FXToolBarShell(this, FRAME_RAISED);
    MainMenu= new FXMenuBar(DockUp, MenuSh, LAYOUT_DOCK_NEXT|LAYOUT_SIDE_TOP|LAYOUT_FILL_X|FRAME_RAISED);
              new FXToolBarGrip(MainMenu, MainMenu, FXMenuBar::ID_TOOLBARGRIP, TOOLBARGRIP_DOUBLE);

    FileMain= new FXMenuPane(this);
	      new FXMenuTitle(MainMenu, "&File", NULL, FileMain);
	      new FXMenuCommand(FileMain, "&New Project", NULL, this, ID_NEW);
	      new FXMenuCommand(FileMain, "&Save as...", NULL, this, ID_SAVE);
	      new FXMenuCommand(FileMain, "&Print...", NULL, this, ID_PRINT);
	      new FXMenuSeparator(FileMain);
	      new FXMenuCommand(FileMain, "&Cancel", NULL, this, ID_XNL);
	      new FXMenuCommand(FileMain, "&Exit", NULL, this, ID_EXIT);

    ProjMain= new FXMenuPane(this);
              new FXMenuTitle(MainMenu,  "&Project", NULL, ProjMain);
	      new FXMenuCommand(ProjMain, "&Passive BPF", NULL, this, ID_PBPF);
	      new FXMenuCommand(ProjMain, "&Active BPF", NULL, this, ID_ABPF);
	      new FXMenuCommand(ProjMain, "P&assive Fltr", NULL, this, ID_PFLTR);
	      new FXMenuCommand(ProjMain, "A&ctive Fltr", NULL, this, ID_AFLTR);

    HelpMain= new FXMenuPane(this);
              new FXMenuTitle(MainMenu, "&Help", NULL, HelpMain, LAYOUT_RIGHT);
	      new FXMenuCommand(HelpMain, "Help &Contents", NULL, this, ID_CNTS);
	      new FXMenuCommand(HelpMain, "&About...", NULL, this, ID_ABOUT);
	      new FXMenuSeparator(HelpMain);
	      new FXMenuCommand(HelpMain, "&Dolphins...", NULL, this, ID_DOLPHINS);

//                                Create Tool Bar

    ToolSh= new FXToolBarShell(this, FRAME_RAISED);
    MainTB= new FXToolBar(DockUp, ToolSh, LAYOUT_DOCK_NEXT|LAYOUT_SIDE_TOP|LAYOUT_FILL_X|FRAME_RAISED);
            new FXToolBarGrip(MainTB, MainTB, FXToolBar::ID_TOOLBARGRIP, TOOLBARGRIP_DOUBLE);

    new FXButton(MainTB, "\tNew Project\tStart a new project.", NewIco, this,
		 ID_NEW, ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y);

    new FXButton(MainTB, "\tSave Project\tSave current project to file...", SaveIco, this,
		 ID_SAVE, ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y);

    new FXButton(MainTB, "\tPrint\tPrint out the current project.", PrintIco, this,
		 ID_PRINT, ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y);

    new FXFrame(MainTB, FRAME_SUNKEN|LAYOUT_CENTER_Y|LAYOUT_FIX, 0, 0, 2, 22);

    new FXButton(MainTB, "\tAbout\tShow the \"About\" box.", AboutIco, this, ID_ABOUT,
                 ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y);

    new FXButton(MainTB, "\tHelp\tDisplay the on-line help.", CntsIco, this, ID_CNTS,
                 ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_CENTER_Y|LAYOUT_RIGHT);

    new FXToolTip(ThisApp);

//                                 Create Main Window

    MainLO=     new FXVerticalFrame(this, LAYOUT_FILL|FRAME_NORMAL);
    MainPanel=  new FXSwitcher(MainLO, SWITCHER_VCOLLAPSE|LAYOUT_FILL|FRAME_NONE);
    ImgViewLO=  new FXHorizontalFrame(MainPanel, LAYOUT_FILL|FRAME_NORMAL);
    MainDisp=   new FXImageView(ImgViewLO, NULL, NULL, 0, IMAGEVIEW_NORMAL|LAYOUT_FILL);
    CmdBtnBar=  new FXHorizontalFrame(MainLO, LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|FRAME_NORMAL);
    OKCmdBtn=   new FXButton(CmdBtnBar, "&OK", OKChkIco, this, ID_OK, BUTTON_NORMAL|ICON_BEFORE_TEXT);
    ClrCmdBtn=  new FXButton(CmdBtnBar, "C&lear", ClrIco, this, ID_CLR, BUTTON_NORMAL|ICON_BEFORE_TEXT);
    ExitCmdBtn= new FXButton(CmdBtnBar, "&Exit", ExitIco, this, ID_EXIT, BUTTON_NORMAL|ICON_BEFORE_TEXT|LAYOUT_RIGHT);

//                                 Set Up

    Welcome= new FXGIFImage(ThisApp, NULL, IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    FXFileStream PixStr;

    if (PixStr.open(INTROSCREEN, FXStreamLoad))
    {
      Welcome->loadPixels(PixStr);
      PixStr.close();
      Welcome->create();
      MainDisp->setImage(Welcome);
    }
    else
    {
      fprintf(stderr, "\nCan't open the welcome screen image: %s. Lost? Corrupted?", INTROSCREEN);
      FXFont *SysFont= ThisApp->getNormalFont();
      SysFont->create();
      FXDCWindow Cntx(Welcome);
      Cntx.setFont(SysFont);
      Cntx.setForeground(FXRGB(255, 255, 255));
      Cntx.fillRectangle(0, 0, 690, 447);
      Cntx.setForeground(FXRGB(0, 0, 0));
      Cntx.drawText(200, 345, "(Image display area)");
    }

//                              Create Bandpass Filter Designer Pane

    FXSplitter        *SubLO1;
    FXVerticalFrame   *SubLO2, *SubLO3;
    FXHorizontalFrame *DispLO, *SchemosLO;
    FXGroupBox        *DataGB;
    FXMatrix          *Grid1;

    TplPop= new FXPopup(this);

    new FXOption(TplPop, "Parallel 1", NULL, this, ID_BP_PTC, JUSTIFY_RIGHT|ICON_BEFORE_TEXT);
    new FXOption(TplPop, "Parallel 2", NULL, this, ID_BP_PTLC, JUSTIFY_RIGHT|ICON_BEFORE_TEXT);
    new FXOption(TplPop, "Series 1", NULL, this, ID_BP_STC, JUSTIFY_RIGHT|ICON_BEFORE_TEXT);
    new FXOption(TplPop, "Series 2", NULL, this, ID_BP_STLC, JUSTIFY_RIGHT|ICON_BEFORE_TEXT);
    new FXOption(TplPop, "Multi-FB", NULL, this, ID_MULTIFB, JUSTIFY_RIGHT|ICON_BEFORE_TEXT);
    new FXOption(TplPop, "State Var", NULL, this, ID_STATEVAR, JUSTIFY_RIGHT|ICON_BEFORE_TEXT);

    SubLO1=  new FXSplitter(MainPanel, SPLITTER_HORIZONTAL|SPLITTER_TRACKING|LAYOUT_FILL|FRAME_NORMAL);
    SubLO2=  new FXVerticalFrame(SubLO1, LAYOUT_FILL|FRAME_NORMAL);
    DataGB=  new FXGroupBox(SubLO2, "Topologies", GROUPBOX_TITLE_CENTER|FRAME_GROOVE);
    Grid1=   new FXMatrix(DataGB, 6, MATRIX_BY_ROWS);
             new FXLabel(Grid1, "Topology", NULL, ICON_BEFORE_TEXT|JUSTIFY_RIGHT);
             new FXLabel(Grid1, "Fh (Hz, KHz, MHz)", NULL, ICON_BEFORE_TEXT|JUSTIFY_RIGHT);
             new FXLabel(Grid1, "Fl (Hz, KHz, MHz)", NULL, ICON_BEFORE_TEXT|JUSTIFY_RIGHT);
             new FXLabel(Grid1, "Rs/Rl (R, K)", NULL, ICON_BEFORE_TEXT|JUSTIFY_RIGHT);
             new FXLabel(Grid1, "Gain", NULL, ICON_BEFORE_TEXT|JUSTIFY_RIGHT);
             new FXLabel(Grid1, "Trial C (uF, pF)", NULL, ICON_BEFORE_TEXT|JUSTIFY_RIGHT);

    TplOpt=      new FXOptionMenu(Grid1, TplPop, FRAME_RAISED|FRAME_THICK|JUSTIFY_HZ_APART|ICON_BEFORE_TEXT|LAYOUT_CENTER_X);
    UsrEntry[0]= new FXTextField(Grid1, 16, this, ID_UPPER, TEXTFIELD_LIMITED|TEXTFIELD_NORMAL);
    UsrEntry[1]= new FXTextField(Grid1, 16, this, ID_LOWER, TEXTFIELD_LIMITED|TEXTFIELD_NORMAL);
    UsrEntry[2]= new FXTextField(Grid1, 16, this, ID_FLTR_R, TEXTFIELD_LIMITED|TEXTFIELD_NORMAL);
    UsrEntry[3]= new FXTextField(Grid1, 16, this, ID_GAIN, TEXTFIELD_LIMITED|TEXTFIELD_NORMAL);
    UsrEntry[4]= new FXTextField(Grid1, 16, this, ID_C_TRIAL, TEXTFIELD_LIMITED|TEXTFIELD_NORMAL);

    SubLO3=     new FXVerticalFrame(SubLO1, LAYOUT_FILL|FRAME_NORMAL);
    SchemosLO=  new FXHorizontalFrame(SubLO3, LAYOUT_FILL|FRAME_NORMAL);
    SchemosLbl= new FXImageView(SchemosLO, NULL, NULL, 0, IMAGEVIEW_NORMAL|LAYOUT_FILL);
    DispLO=     new FXHorizontalFrame(SubLO3, LAYOUT_FILL|LAYOUT_CENTER|FRAME_NORMAL);
    BPFDesc=    new FXText(DispLO, NULL, 0, TEXT_READONLY|TEXT_WORDWRAP|LAYOUT_FILL|FRAME_NONE);

    BPFDesc->setMarginTop(10);
    BPFDesc->setMarginBottom(10);
    BPFDesc->setMarginLeft(20);
    BPFDesc->setMarginRight(20);
    BPFDesc->setTabColumns(7);
    BPFDesc->setText("Select topology to begin");

//                       Highpass/Lowpass Filter Designer Pane

    FXSplitter      *SubLO4;
    FXVerticalFrame *SubLO5, *SubLO6;
    FXGroupBox      *CharGB, *ImpGB;
    FXMatrix        *Grid2, *Grid3, *Grid4;

    FltrPop= new FXPopup(this);

    new FXOption(FltrPop, "Active LPF", NULL, this, ID_ACTIVE_LP, JUSTIFY_RIGHT|ICON_BEFORE_TEXT);
    new FXOption(FltrPop, "Active HPF", NULL, this, ID_ACTIVE_HP, JUSTIFY_RIGHT|ICON_BEFORE_TEXT);
    new FXOption(FltrPop, "Passive LP: Cin", NULL, this, ID_LP_CIN, JUSTIFY_RIGHT|ICON_BEFORE_TEXT);
    new FXOption(FltrPop, "Passive LP: Lin", NULL, this, ID_LP_LIN, JUSTIFY_RIGHT|ICON_BEFORE_TEXT);
    new FXOption(FltrPop, "Passive HP: Cin", NULL, this, ID_HP_CIN, JUSTIFY_RIGHT|ICON_BEFORE_TEXT);
    new FXOption(FltrPop, "Passive HP: Lin", NULL, this, ID_HP_LIN, JUSTIFY_RIGHT|ICON_BEFORE_TEXT);

    SubLO4= new FXSplitter(MainPanel, SPLITTER_HORIZONTAL|SPLITTER_TRACKING|LAYOUT_FILL|FRAME_NORMAL);
    SubLO5= new FXVerticalFrame(SubLO4, LAYOUT_FILL|LAYOUT_CENTER_Y|FRAME_NORMAL);
    Grid2=  new FXMatrix(SubLO5, 3, MATRIX_BY_ROWS);
            new FXLabel(Grid2, "Topology", NULL, ICON_BEFORE_TEXT|JUSTIFY_RIGHT);
	    new FXLabel(Grid2, "Order", NULL, ICON_BEFORE_TEXT|JUSTIFY_RIGHT);
	    new FXLabel(Grid2, "Fc (Hz, KHz, MHz)", NULL, ICON_BEFORE_TEXT|JUSTIFY_RIGHT);

    FltrOpt=     new FXOptionMenu(Grid2, FltrPop, FRAME_RAISED|FRAME_THICK|JUSTIFY_HZ_APART|ICON_BEFORE_TEXT|LAYOUT_CENTER_X);
    OrderSB=     new FXSpinner(Grid2, 10, this, ID_ORDER, SPIN_CYCLIC|FRAME_NORMAL|LAYOUT_CENTER_X);
    UsrEntry[5]= new FXTextField(Grid2, 16, this, ID_CORNER, TEXTFIELD_LIMITED|TEXTFIELD_NORMAL);
    CharGB=      new FXGroupBox(SubLO5, "Characteristics", GROUPBOX_TITLE_CENTER|FRAME_GROOVE);
                 new FXRadioButton(CharGB, "Butterworth", this, ID_BUTTER);
                 new FXRadioButton(CharGB, "Chebychev", this, ID_CHEBYCHEV);
                 new FXRadioButton(CharGB, "Bessel", this, ID_BESSEL);
                 new FXRadioButton(CharGB, "Frequency", this, ID_FREQ);
                 new FXRadioButton(CharGB, "Time", this, ID_TIME);
                 new FXRadioButton(CharGB, "R_l/R_src INF", this, ID_RL_INF);
    Grid4=       new FXMatrix(CharGB, 1, MATRIX_BY_ROWS);
                 new FXLabel(Grid4, "PB Ripple (db)", NULL, ICON_BEFORE_TEXT|JUSTIFY_RIGHT);
    UsrEntry[6]= new FXTextField(Grid4, 16, this, ID_RIPPLE, TEXTFIELD_LIMITED|TEXTFIELD_NORMAL);
    ImpGB=       new FXGroupBox(SubLO5, "Impedance", GROUPBOX_TITLE_CENTER|FRAME_GROOVE);
    Grid3=       new FXMatrix(ImpGB, 1, MATRIX_BY_ROWS);
    RValueLbl=   new FXLabel(Grid3, "", NULL, ICON_BEFORE_TEXT|JUSTIFY_RIGHT);
    UsrEntry[7]= new FXTextField(Grid3, 16, this, ID_FLTR_RL, TEXTFIELD_LIMITED|TEXTFIELD_NORMAL);

    SubLO6=   new FXVerticalFrame(SubLO4, LAYOUT_FILL|FRAME_NORMAL);
    FltrDesc= new FXText(SubLO6, NULL, 0, TEXT_READONLY|TEXT_WORDWRAP|LAYOUT_FILL|FRAME_NONE);

    OrderSB->setRange(2, MAXORDER);
    FltrDesc->setMarginTop(10);
    FltrDesc->setMarginBottom(10);
    FltrDesc->setMarginLeft(20);
    FltrDesc->setMarginRight(20);
    FltrDesc->setTabColumns(7);
}

//------------------------------------------------------------------------------

AppWin::~AppWin(void)
{
    delete SmDolphinIco;
    delete NewIco;
    delete SaveIco;
    delete PrintIco;
    delete CntsIco;
    delete AboutIco;
    delete XnlIco;
    delete ClrIco;
    delete OKChkIco;
    delete ExitIco;
    delete BPFPTCIco;
    delete BPFPTLCIco;
    delete BPFSTCIco;
    delete BPFSTLCIco;
    delete MFBIco;
    delete SVarIco;
    delete LEDGrayIco;
    delete LEDOnIco;
    delete LEDOffIco;
    delete TplPop;
    delete FltrPop;
    delete FileMain;
    delete ProjMain;
    delete HelpMain;
    
    if (Welcome) delete Welcome;

    if (ProjSchemos) delete ProjSchemos;

    if (TxtResults) delete TxtResults;
}

//------------------------------------------------------------------------------

void AppWin::create(void)
{
    FXMainWindow::create();
    MenuSh->create();
    ToolSh->create();
    FileMain->create();
    ProjMain->create();
    HelpMain->create();
    BPFPTCIco->create();
    BPFPTLCIco->create();
    BPFSTCIco->create();
    BPFSTLCIco->create();
    MFBIco->create();
    SVarIco->create();
    show(PLACEMENT_SCREEN);
}

//-------------------------- Event Handlers: Command --------------------------

long AppWin::ClickMenuItm(FXObject*, FXSelector CallerID, void*)
{
    FXuint Reply;

    switch (FXSELID(CallerID))
    {
        case ID_XNL: SaveState= SAVED;

        case ID_NEW:

	  if (SaveState == NOT_SAVED)
	  {
	      Reply= FXMessageBox::question(ThisApp, MBOX_YES_NO_CANCEL, "Filter Designer Question",
					    "Current project not saved.\nStart a new one anyway?");
	      if (Reply != MBOX_CLICKED_YES) return 1;
	  }

	  FilterTypeID=  0;
	  FilterCharID=  0;
	  FilterClassID= 0;
	  RLinf=         0;
	  ProjState=     PROJ_NONE;
	  SaveState=     SAVED;
	  BesselOpt=     NOOPT;
	  Order=         0;
	  BPFDesc->setText("");
	  FltrDesc->setText("");
	  RValueLbl->setText("");
	  StatusLbl->setText("No project");
	  StatusLbl->setIcon(LEDGrayIco);
	  OrderSB->setRange(2, MAXORDER);
	  OrderSB->setIncrement(1);
          MainDisp->setImage(Welcome);
	  MainPanel->handle(this, FXSEL(SEL_COMMAND, MAIN_PANE), 0);
	  if (ProjSchemos) delete ProjSchemos;
	  ProjSchemos= 0;
	  if (TxtResults) TxtResults->clear();

	  for (int i= 0; i < 8; i++) UsrEntry[i]->setText("");
	  break;

        case ID_SAVE:

	  ProjectSave();
	  SaveState= SAVED;
	  StatusLbl->setText("Project saved");
	  StatusLbl->setIcon(LEDOnIco);
	  break;

        case ID_PRINT: ProjectPrint(); break;

        case ID_TITLE:

        case ID_EXIT:

	  if (SaveState == NOT_SAVED)
	  {
	      Reply= FXMessageBox::question(ThisApp, MBOX_YES_NO_CANCEL, "Filter Designer Question",
					    "Current project not saved.\nExit anyway?");
	      if (Reply != MBOX_CLICKED_YES) return 1;
	  }

          ThisApp->exit(0);
	  break;

        case ID_PBPF:

	  FilterClassID= PASSIVE;
	  MainPanel->handle(this, FXSEL(SEL_COMMAND, BPF_PANE), 0);
	  OKCmdBtn->setSelector(ID_OK_PBPF);
	  ClrCmdBtn->setSelector(ID_CLR_BPF);
	  break;

        case ID_ABPF:

	  FilterClassID= ACTIVE;
	  MainPanel->handle(this, FXSEL(SEL_COMMAND, BPF_PANE), 0);
	  OKCmdBtn->setSelector(ID_OK_ABPF);
	  ClrCmdBtn->setSelector(ID_CLR_BPF);
	  break;

        case ID_PFLTR:

	  FilterClassID= PASSIVE;
	  RValueLbl->setText("R_src/R_load (R, K, M)");
	  FltrDesc->setText("\n\tSelect a passive filter topology");
	  MainPanel->handle(this, FXSEL(SEL_COMMAND, FLTR_PANE), 0);
	  OKCmdBtn->setSelector(ID_OK_PFLTR);
	  ClrCmdBtn->setSelector(ID_CLR_FLTR);
	  break;

        case ID_AFLTR:

	  FilterClassID= ACTIVE;
	  RValueLbl->setText("Trial R (R, K, M)");
	  FltrDesc->setText("\n\tSelect an active highpass or lowpass filter");
	  MainPanel->handle(this, FXSEL(SEL_COMMAND, FLTR_PANE), 0);
	  OKCmdBtn->setSelector(ID_OK_AFLTR);
	  ClrCmdBtn->setSelector(ID_CLR_FLTR);
	  break;

        case ID_CNTS:

        case ID_ABOUT:

        case ID_DOLPHINS: OpenDlgs(CallerID); break;
    }

    return 1;
}

//------------------------------------------------------------------------------

long AppWin::ClickCmdBtn(FXObject*, FXSelector CallerID, void*)
{
    switch(FXSELID(CallerID))
    {
        case ID_OK_PBPF:
	{
	    FXdouble Fh, Fl, R;
	    FXString DesignData;

	    Fh= GetData(UsrEntry[0]);

	    if (! Fh)
	    {
		FXMessageBox::error(ThisApp, MBOX_OK, "Filter Designer Error",
				    "Bad/missing entry for high PB frequency");
		UsrEntry[0]->setFocus();
		return 1;

	    }

	    Fl= GetData(UsrEntry[1]);

	    if (! Fl)
	    {
		FXMessageBox::error(ThisApp, MBOX_OK, "Filter Designer Error",
				    "Bad/missing entry for low PB frequency");
		UsrEntry[1]->setFocus();
		return 1;

	    }

	    R=  GetData(UsrEntry[2]);

	    if (! R)
	    {
		FXMessageBox::error(ThisApp, MBOX_OK, "Filter Designer Error",
				    "Bad/missing entry of input/output resistance");
		UsrEntry[2]->setFocus();
		return 1;
	    }

	    if (Fh <= Fl)
	    {
		FXMessageBox::error(ThisApp, MBOX_OK, "Filter Designer Error",
				    "The upper and/or lower passband frequencies are incorrect");
		return 1;
	    }

	    DesignData= PBPFDesigner(Fh, Fl, R, FilterTypeID);
	    WriteOut(DesignData, FilterTypeID);
	}
	break;

        case ID_OK_ABPF:
	{
	    FXdouble Fh, Fl, Ctrial;
	    FXString DesignData;

	    Fh= GetData(UsrEntry[0]);

	    if (! Fh)
	    {
		FXMessageBox::error(ThisApp, MBOX_OK, "Filter Designer Error",
				    "Bad/missing entry of upper PB frequency");
		UsrEntry[0]->setFocus();
		return 1;
	    }

	    Fl= GetData(UsrEntry[1]);

	    if (! Fl)
	    {
		FXMessageBox::error(ThisApp, MBOX_OK, "Filter Designer Error",
				    "Bad/missing entry of lower PB frequency");
		UsrEntry[1]->setFocus();
		return 1;
	    }
	    Ctrial=  GetData(UsrEntry[4]);

	    if (! Ctrial)
	    {
		FXMessageBox::error(ThisApp, MBOX_OK, "Filter Designer Error",
				    "Bad/missing Ctrial value");
		UsrEntry[4]->setFocus();
		return 1;
	    }

	    if (Fh <= Fl)
	    {
		FXMessageBox::error(ThisApp, MBOX_OK, "Filter Designer Error",
				    "The upper and/or lower passband frequencies are incorrect");
		return 1;
	    }

	    DesignData= ABPFDesigner(Fh, Fl, Ctrial, FilterTypeID);
	    WriteOut(DesignData, FilterTypeID);
	}
	break;

        case ID_OK_PFLTR:
	{
	    FXdouble CutoffFreq, Rsrc, PBripple= 0;
	    FXuint Order;
	    FXString DesignData;

	    Order=      OrderSB->getValue();
	    CutoffFreq= GetData(UsrEntry[5]);
	    PBripple=   fabs(GetData(UsrEntry[6]));
	    Rsrc=       GetData(UsrEntry[7]);

	    if (! CutoffFreq)
	    {
	      FXMessageBox::error(ThisApp, MBOX_OK, "Filter Designer Error",
				  "The cutoff frequency is missing/incorrect");
	    return 1;
	    }

	    if (! FilterCharID)
	    {
		FXMessageBox::error(ThisApp, MBOX_OK, "Filter Designer Error",
				    "No filter characteristic selected");
		return 1;
	    }

	    if ((FilterCharID == CHEBYCHEV) && (! PBripple || (PBripple > 3.01)))
	    {
	      FXMessageBox::error(ThisApp, MBOX_OK, "Filter Designer Error", PBR_ERROR);
	      return 1;
	    }

	    if (! Rsrc)
	    {
		FXMessageBox::error(ThisApp, MBOX_OK, "Filter Designer Error",
				    "Missing values for Rsrc");
		return 1;
	    }

	    FXdouble LCVals[Order];
	    if (PassiveFltrDesigner(Order, PBripple, LCVals)) return 1;
	    DesignData= ScalePassive(Order, CutoffFreq, PBripple, Rsrc, LCVals);
	    WriteOut(DesignData, FilterTypeID);
	}
	break;

        case ID_OK_AFLTR:
	{
	    FXdouble CutoffFreq, TrialR, PBripple= 0;
	    FXuint Order;
	    FXint Sections;
	    FXString DesignData;

	    CutoffFreq= GetData(UsrEntry[5]);
	    PBripple=   fabs(GetData(UsrEntry[6]));
	    TrialR=     GetData(UsrEntry[7]);
	    Order=      OrderSB->getValue();

	    if (! CutoffFreq)
	    {
	      FXMessageBox::error(ThisApp, MBOX_OK, "Filter Designer Error",
				  "The cutoff frequency is missing/incorrect");
	    return 1;
	    }

	    if (! FilterCharID)
	    {
		FXMessageBox::error(ThisApp, MBOX_OK, "Filter Designer Error",
				    "No filter characteristic selected");
		return 1;
	    }

	    if ((FilterCharID == CHEBYCHEV) && (! PBripple || PBripple > 3.01))
	    {
	      FXMessageBox::error(ThisApp, MBOX_OK, "Filter Designer Error", PBR_ERROR);
	      return 1;
	    }

	    if (! TrialR)
	    {
	      FXMessageBox::error(ThisApp, MBOX_OK, "Filter Designer Error",
				  "The trial/match resistance is missing/incorrect");
	      return 1;
	    }

	    Sections= (Order & 1) ? (Order/2 + 1) : (Order/2);
	    FXdouble C_one[Sections];
	    FXdouble C_two[Sections];
	    if (ActiveFltrDesigner(Order, PBripple, C_one, C_two)) return 1;
	    DesignData= ScaleActive(Sections, CutoffFreq, PBripple, TrialR, C_one, C_two);
	    (Order & 1) ? WriteOut(DesignData, FilterTypeID|1) : WriteOut(DesignData, FilterTypeID);
	}
	break;

        case ID_CLR_BPF:

	  for (int i= 0; i < 5; i++) UsrEntry[i]->setText("");
	  return 1;
	  break;

        case ID_CLR_FLTR:

	  for (int i= 5; i < 8; i++) UsrEntry[i]->setText("");
	  return 1;
	  break;
    }

    MainPanel->handle(this, FXSEL(SEL_COMMAND, MAIN_PANE), 0);
    SaveState= NOT_SAVED;
    ProjState= PROJ_COMP;
    StatusLbl->setText("Project complete; not saved");
    StatusLbl->setIcon(LEDOffIco);

    return 1;
}

//------------------------------------------------------------------------------

long AppWin::ClickOptItm(FXObject*, FXSelector CallerID, void*)
{
    switch(FXSELID(CallerID))
    {
        case ID_BP_PTC:

	  FilterTypeID= BW_PTC;
	  SchemosLbl->setImage(BPFPTCIco);
	  BPFDesc->setText(BPF_PTC_DESC);
	  break;

        case ID_BP_PTLC:

	  FilterTypeID= BW_PTLC;
	  SchemosLbl->setImage(BPFPTLCIco);
	  BPFDesc->setText(BPF_PTLC_DESC);
	  break;

        case ID_BP_STC:

	  FilterTypeID= BW_STC;
	  SchemosLbl->setImage(BPFSTCIco);
	  BPFDesc->setText(BPF_STC_DESC);
	  break;

        case ID_BP_STLC:

	  FilterTypeID= BW_STLC;
	  SchemosLbl->setImage(BPFSTLCIco);
	  BPFDesc->setText(BPF_STLC_DESC);
	  break;

        case ID_MULTIFB:

	  FilterTypeID= MULTI_FB;
	  SchemosLbl->setImage(MFBIco);
	  BPFDesc->setText(MFB_DESC);
	  break;

        case ID_STATEVAR:

	  FilterTypeID= STATE_VAR;
	  SchemosLbl->setImage(SVarIco);
	  BPFDesc->setText(STATEVAR_DESC);
	  break;

        case ID_LP_CIN:

	  FilterTypeID= LP_CIN;
	  FltrDesc->setText(LP_CIN_DESC);
	  break;

        case ID_LP_LIN:

	  FilterTypeID= LP_LIN;
	  FltrDesc->setText(LP_LIN_DESC);
	  break;

        case ID_HP_CIN:

	  FilterTypeID= HP_CIN;
	  FltrDesc->setText(HP_CIN_DESC);
	  break;

        case ID_HP_LIN:

	  FilterTypeID= HP_LIN;
	  FltrDesc->setText(HP_LIN_DESC);
	  break;

        case ID_ACTIVE_LP: FilterTypeID= ACTIVE_LPF; break;

        case ID_ACTIVE_HP: FilterTypeID= ACTIVE_HPF; break;
    }

    ProjState= PROJ_PEND;
    StatusLbl->setText("Project Pending...");

    return 1;
}

//------------------------------------------------------------------------------

long AppWin::ClickRadioBtn(FXObject*, FXSelector CallerID, void*)
{
    switch(FXSELID(CallerID))
    {
        case ID_BUTTER:
	  FilterCharID= BUTTERWORTH;
	  FltrDesc->setText(BUTTERWORTH_DESC);
	  break;

        case ID_CHEBYCHEV:
	  FilterCharID= CHEBYCHEV;
	  FltrDesc->setText(CHEBYCHEV_DESC);
	  break;

        case ID_BESSEL:
	  FilterCharID= BESSEL;
	  FltrDesc->setText(BESSEL_DESC);
	  break;

        case ID_RL_INF: RLinf= 1; break;

        case ID_FREQ: BesselOpt= FREQ; break;

        case ID_TIME: BesselOpt= TIME; break;
    }

  return 1;
}

//--------------------------- Event Handlers: Update ---------------------------

long AppWin::UpdateMainMenu(FXObject *Sender, FXSelector CallerID, void*)
{
  switch (FXSELID(CallerID))
  {
      case ID_NEW:

      case ID_SAVE:

      case ID_PRINT:

          (ProjState == PROJ_COMP) ? Sender->handle(this, FXSEL(SEL_COMMAND, ENABLE), 0) :
	                             Sender->handle(this, FXSEL(SEL_COMMAND, DISABLE), 0);
	  break;

      case ID_XNL:

	  (ProjState == PROJ_PEND) ? Sender->handle(this, FXSEL(SEL_COMMAND, ENABLE), 0) :
				     Sender->handle(this, FXSEL(SEL_COMMAND, DISABLE), 0);
	  break;

      case ID_PBPF:

      case ID_ABPF:

      case ID_PFLTR:

      case ID_AFLTR:

	  (ProjState == PROJ_NONE) ? Sender->handle(this, FXSEL(SEL_COMMAND, ENABLE), 0) :
	                             Sender->handle(this, FXSEL(SEL_COMMAND, DISABLE), 0);
	  break;
  }

  return 1;
}

//------------------------------------------------------------------------------

long AppWin::UpdateOptions(FXObject *Sender, FXSelector Caller, void*)
{
    if (ProjState == PROJ_COMP)
    {
	Sender->handle(this, FXSEL(SEL_COMMAND, DISABLE), 0);
	return 1;
    }

    switch (FXSELID(Caller))
    {
      case ID_BP_PTC:

      case ID_BP_PTLC:

      case ID_BP_STC:

      case ID_BP_STLC:

      case ID_LP_CIN:

      case ID_LP_LIN:

      case ID_HP_CIN:

      case ID_HP_LIN:

	(FilterClassID == PASSIVE) ? Sender->handle(this, FXSEL(SEL_COMMAND, ENABLE), 0) :
	                             Sender->handle(this, FXSEL(SEL_COMMAND, DISABLE), 0);
	break;

      case ID_ACTIVE_LP:

      case ID_ACTIVE_HP:

	(FilterClassID == ACTIVE) ? Sender->handle(this, FXSEL(SEL_COMMAND, ENABLE), 0) :
	                            Sender->handle(this, FXSEL(SEL_COMMAND, DISABLE), 0);
	break;

      case ID_MULTIFB:

      case ID_STATEVAR:

        (FilterClassID == ACTIVE) ? Sender->handle(this, FXSEL(SEL_COMMAND, ENABLE), 0) :
	                            Sender->handle(this, FXSEL(SEL_COMMAND, DISABLE), 0);
        break;
    }

    return 1;
}

//------------------------------------------------------------------------------

long AppWin::UpdateLineEdit(FXObject *Sender, FXSelector CallerID, void*)
{
    if (ProjState == PROJ_COMP)
    {
        Sender->handle(this, FXSEL(SEL_COMMAND, DISABLE), 0);
	return 1;
    }

    switch(FXSELID(CallerID))
    {
        case ID_GAIN:

	  (FilterTypeID == STATE_VAR) ? Sender->handle(this, FXSEL(SEL_COMMAND, ENABLE), 0) :
	                                Sender->handle(this, FXSEL(SEL_COMMAND, DISABLE), 0);
	  break;

        case ID_C_TRIAL:

          (FilterClassID == ACTIVE) ? Sender->handle(this, FXSEL(SEL_COMMAND, ENABLE), 0) :
	                              Sender->handle(this, FXSEL(SEL_COMMAND, DISABLE), 0);
          break;

        case ID_RIPPLE:

	  (FilterCharID == CHEBYCHEV) ? Sender->handle(this, FXSEL(SEL_COMMAND, ENABLE), 0) :
	                                Sender->handle(this, FXSEL(SEL_COMMAND, DISABLE), 0);
	  break;

        case ID_FLTR_R:

	  (FilterClassID == PASSIVE) ? Sender->handle(this, FXSEL(SEL_COMMAND, ENABLE), 0) :
	                               Sender->handle(this, FXSEL(SEL_COMMAND, DISABLE), 0);
	  break;

        default: Sender->handle(this, FXSEL(SEL_COMMAND, ENABLE), 0);
    }

    return 1;
}

//------------------------------------------------------------------------------

long AppWin::UpdateRadioBtn(FXObject *Sender, FXSelector CallerID, void*)
{
    FXRadioButton *Current= (FXRadioButton*)Sender;

    switch (FXSELID(CallerID))
    {
      case ID_BUTTER:

	(FilterCharID == BUTTERWORTH) ? Current->setCheck(TRUE) : Current->setCheck(FALSE); break;

      case ID_CHEBYCHEV:

	(FilterCharID == CHEBYCHEV) ? Current->setCheck(TRUE) : Current->setCheck(FALSE); break;

      case ID_BESSEL:

	(FilterCharID == BESSEL) ? Current->setCheck(TRUE) : Current->setCheck(FALSE); break;

      case ID_TIME:

	if (FilterCharID == BESSEL)
	{
	    Current->handle(this, FXSEL(SEL_COMMAND, ENABLE), 0);
	    Current->setCheck(TRUE);
	}
	else
	{
	    Current->handle(this, FXSEL(SEL_COMMAND, DISABLE), 0);
	    Current->setCheck(FALSE);
	}

	(BesselOpt == TIME) ? Current->setCheck(TRUE) : Current->setCheck(FALSE);
	break;

      case ID_FREQ:

	if (FilterCharID == BESSEL)
	{
	    Current->handle(this, FXSEL(SEL_COMMAND, ENABLE), 0);
	    Current->setCheck(TRUE);
	}
	else
	{
	    Current->handle(this, FXSEL(SEL_COMMAND, DISABLE), 0);
	    Current->setCheck(FALSE);
	}

	(BesselOpt == FREQ) ? Current->setCheck(TRUE) : Current->setCheck(FALSE);
	break;

      case ID_RL_INF:

	if (FilterClassID == ACTIVE)
	{
	    Current->handle(this, FXSEL(SEL_COMMAND, DISABLE), 0);
	    Current->setCheck(FALSE);
	}
	else
	{
	  Current->handle(this, FXSEL(SEL_COMMAND, ENABLE), 0);
	  (RLinf) ? Current->setCheck(TRUE) : Current->setCheck(FALSE);
	}
	break;
    }

  return 1;
}

//------------------------------------------------------------------------------

long AppWin::UpdateCmdBtn(FXObject *Sender, FXSelector CallerID, void*)
{

  (ProjState == PROJ_PEND) ? Sender->handle(this, FXSEL(SEL_COMMAND, ENABLE), 0) :
                             Sender->handle(this, FXSEL(SEL_COMMAND, DISABLE), 0);
  return 1;
}

//------------------------------ Helper Functions ------------------------------

void AppWin::OpenDlgs(FXSelector aCaller)
{
    FXDialogBox *SplMsg= 0;
    FXString UseBrowser, SysArg;

    switch (FXSELID(aCaller))
    {
        case ID_ABOUT:

	  SplMsg= new About(ThisApp);
	  SplMsg->execute(PLACEMENT_SCREEN);
	  break;

	case ID_DOLPHINS:

	  SplMsg= new SplMsgBox(ThisApp);
	  SplMsg->execute(PLACEMENT_SCREEN);
	  break;

        case ID_CNTS:

	  UseBrowser= FindBrowser();

	  if (UseBrowser == "None")
	  {
	      FXMessageBox::error(ThisApp, MBOX_OK, "Filter Designer Error", HELP_ERROR);
	      return;
	  }

	  SysArg= UseBrowser + DOCPATH;

	  if (system(SysArg.text()))
	      FXMessageBox::error(ThisApp, MBOX_OK, "Filter Designer Error",
				  "System failed to open %s", SysArg.text());
    }

    if (SplMsg) delete SplMsg;
}

//------------------------------------------------------------------------------

const FXString& AppWin::FindBrowser(void)
{
/*
    Length: Length of the text in the file
    Buffer: Pointer to character array of file contents
    Head, Tail: Pointers into the buffer for text parse
    RetVal: Either a file name or error advisory
*/

    FXint   Length;
    FXchar *Buffer= 0, *Head= 0, *Tail= 0, C;
    static  FXString RetVal;

    FXString BrowserPaths= FXString(BROWSER_PATHS);
    FXFile   ReadConfig(BrowserPaths, FXFile::Reading);

    RetVal= "None";

    if (! ReadConfig.isOpen())
    {
        fprintf(stderr, "Couldn't open the config file: %s", BrowserPaths.text());
	return RetVal;
    }

    Length= ReadConfig.size();
    FXchar Oneline[Length];
    FXMALLOC(&Buffer, FXchar, CEIL(Length + 1));

    ReadConfig.readBlock(Buffer, Length);

/*
    Parse the file contents to extract the list of possible paths to
    web browsers. If found a file that *is* a file, and is executable,
    we may have a possibility. Return it to the caller.

*/
    Head= Tail= Buffer;

    while ((C= *Head++))
    {
	if (C == '\n')
	{
	    memcpy(&Oneline[0], Tail, Head - Tail);   //  Fill the character array...
	    Oneline[Head - Tail - 1]= '\0';           //  ...Make it a C string; ditch new line

	    if ((Oneline[0] != '#') && (Oneline[0]))  //  Ditch comments; empty lines
	    {
	        if ((FXStat::isFile(Oneline)) && (FXStat::isExecutable(Oneline)))
		{
		  RetVal= Oneline;
		  FXFREE(&Buffer);
		  Buffer= 0;
		  break;
		}
	    }
	    Tail= Head;
	}
    }
    
    if (Buffer) FXFREE(&Buffer);

    return RetVal;
}

//------------------------------------------------------------------------------

FXdouble AppWin::GetData(FXTextField *Current)
{
    FXString TextEntry;
    FXdouble Mantissa, Multiplier;

    TextEntry= Current->getText();

#ifdef INSTR
    printf("\nEntering function AppWin::GetData");
#endif

    if (TextEntry.find('R') > -1)
        Multiplier= 1.0;
    else if (TextEntry.find('K') > -1)
	Multiplier= 1000.0;
    else if (TextEntry.find('M') > -1)
	Multiplier= 1.0E6;
    else if (TextEntry.find('u') > -1)
	Multiplier= 1.0E-6;
    else if (TextEntry.find('n') > -1)
	Multiplier= 1.0E-9;
    else if (TextEntry.find('p') > -1)
	Multiplier= 1.0E-12;
    else Multiplier= 1.0;

    Mantissa= FXDoubleVal(TextEntry);

#ifdef INSTR
  printf("\nIn function: AppWin::GetData: TextEntry= %s", TextEntry.text());
  printf("\nIn function: AppWin::GetData: Mantissa= %E", Mantissa);
  printf("\nIn function: AppWin::GetData: Multiplier= %E", Multiplier);
  printf("\nLeaving function AppWin::GetData");
#endif

    return Mantissa * Multiplier;
}

//------------------------------------------------------------------------------

const FXString& AppWin::PBPFDesigner(FXdouble aFh, FXdouble aFl, FXdouble aR, const FXuint aID)
{
  using namespace Bandpass;

  FXdouble Cres, Ccpl, Lres, Lcpl, BW, GCF;
  static FXString RetVal;

  RetVal.clear();

#ifdef INSTR
  printf("\nEntering function: AppWin::PBPFDesigner");
  printf("\naFh= %E", aFh);
  printf("\naFl= %E", aFl);
  printf("\naR= %E", aR);
#endif

  BW=   aFh - aFl;
  GCF=  sqrt(aFh*aFl);
  Cres= C_Tune(aR, aFh, aFl, aID);
  Lres= L_Tune(aR, aFh, aFl, aID);
  Ccpl= C_Couple(aR, aFh, aFl, aID);
  Lcpl= L_Couple(aR, aFh, aFl, aID);

  RetVal.append(FXStringFormat("Pass Band: %s\n", FormatData(BW, 'f').text()));
  RetVal.append(FXStringFormat("Geometric Center Frequency: %s\n", FormatData(GCF, 'f').text()));
  RetVal.append(FXStringFormat("Resonator C= %s\n", FormatData(Cres, 'c').text()));
  RetVal.append(FXStringFormat("Resonator L= %s\n", FormatData(Lres, 'l').text()));
  RetVal.append(FXStringFormat("Coupling C= %s\n", FormatData(Ccpl, 'c').text()));

  if (Lcpl) RetVal.append(FXStringFormat("Coupling L= %s\n", FormatData(Lcpl, 'l').text()));

#ifdef INSTR
  printf("\nLeaving function: AppWin::PBPFDesigner");
#endif

  return RetVal;
}

//------------------------------------------------------------------------------

const FXString& AppWin::ABPFDesigner(FXdouble aFh, FXdouble aFl, FXdouble aCtrial, const FXuint aID)
{
    using namespace Bandpass;

    static FXString RetVal;

    RetVal.clear();

#ifdef INSTR
    printf("\nEntering function: AppWin::ABPFDesigner");
    printf("\naFh= %E", aFh);
    printf("\naFl= %E", aFl);
    printf("\naCtrial= %E", aCtrial);
    printf("\naID= %d", aID);
#endif

    switch (aID)
    {
      case MULTI_FB:
      {
        FXdouble Kfactor, Qfactor, Fcntr, R_1, R_2, R_3;

	Kfactor= K_factor(aFh, aFl, aCtrial, Qfactor, Fcntr);
	R_1= R_One(Qfactor, Kfactor);
	R_2= R_Two(Qfactor, Kfactor);
	R_3= R_Three(Qfactor, Kfactor);

	RetVal.append(FXStringFormat("Center Frequency: %s\n", FormatData(Fcntr, 'f').text()));
	RetVal.append(FXStringFormat("Q-factor: %f\n", Qfactor));
	RetVal.append(FXStringFormat("C= %s\n", FormatData(aCtrial, 'c').text()));
	RetVal.append(FXStringFormat("R_1= %s\n", FormatData(R_1, 'z').text()));
	RetVal.append(FXStringFormat("R_2= %s\n", FormatData(R_2, 'z').text()));
	RetVal.append(FXStringFormat("R_3= %s\n", FormatData(R_3, 'z').text()));
      }
      break;

      case STATE_VAR:
      {
	FXdouble R_main, R_g, R_df, Fcntr, Q, Av;

	Av= GetData(UsrEntry[3]);

	if (! Av)
	{
	    FXMessageBox::error(ThisApp, MBOX_OK, "Filter Designer Error",
			        "Bad/missing entry of filter gain");
	    UsrEntry[3]->setFocus();
	    return RetVal;
	}

	R_main= MainResistor(aFh, aFl, aCtrial, Fcntr, Q);
	R_g= GainSetResistor(R_main, Av);
	R_df= DFSetResistor(Q, R_main);

	RetVal.append(FXStringFormat("Center Frequency: %s\n", FormatData(Fcntr, 'f').text()));
	RetVal.append(FXStringFormat("Q-factor: %f\n", Q));
	RetVal.append(FXStringFormat("Main Resistors= %s\n", FormatData(R_main, 'z').text()));
	RetVal.append(FXStringFormat("Gain Set Resistor= %s\n", FormatData(R_g, 'z').text()));
	RetVal.append(FXStringFormat("Damping Factor Set Resistor= %s\n", FormatData(R_df, 'z').text()));
      }
      break;
    }

#ifdef INSTR
    printf("Leaving function: AppWin::ABPFDesigner");
#endif

    return RetVal;
}

//------------------------------------------------------------------------------

FXuint AppWin::ActiveFltrDesigner(const FXuint aOrder, const FXdouble aPBR, FXdouble *aCone, FXdouble *aCtwo)
{
#ifdef INSTR
  printf("\nEntering function: AppWin::ActiveFltrDesigner\n");
#endif

    using namespace Filters;

    FXdouble R_1, R_2;
    FXint Sections= (aOrder & 1) ? (aOrder/2 + 1) : (aOrder/2);
    FXuint RetCode= 0;

    switch (FilterCharID)
    {
        case BUTTERWORTH: LPActiveBW(aOrder, Sections, aCone, aCtwo); break;

        case CHEBYCHEV:   LPActiveChev(aOrder, Sections, aPBR, aCone, aCtwo); break;

        case BESSEL:
	    if (! HAVE_MPSOLVE)
	    {
	      FXMessageBox::information(ThisApp, MBOX_OK, "Filter Designer Advisory",
					"MPSolve not installed: cannot complete project");
	      return 1;
	    }
	    RetCode= LPActiveBes(aOrder, aCone, aCtwo, BesselOpt);
	    break;
    }

    if (RetCode)
    {
	FXMessageBox::error(ThisApp, MBOX_OK, "Filter Designer Error",
			    "Failed to correctly parse MPSolve output");
	return 1;
    }

    if (FilterTypeID == ACTIVE_LPF) return 0;  // Note: If LPF: done

//               Adjust immitences for HPF

    for (int i= 0; i < Sections; i++)
    {
        R_1= 1.0/aCone[i];
        R_2= (aCtwo[i]) ? 1.0/aCtwo[i] : 0;

#ifdef INSTR
  printf("i= %d, R_1= %E, R_2= %E\n", i, R_1, R_2);
#endif

	aCone[i]= R_1;
	aCtwo[i]= R_2;
    }
    return 0;
}

//------------------------------------------------------------------------------

FXuint AppWin::PassiveFltrDesigner(const FXuint aOrd, const FXdouble aPBR, FXdouble *aLCVals)
{
    using namespace Filters;

    if (RLinf)
    {
      if ((FilterTypeID == LP_CIN) || (FilterTypeID == LP_LIN))
	FilterTypeID= (aOrd & 1) ? LP_CIN : LP_LIN;
      else if ((FilterTypeID == HP_CIN) || (FilterTypeID == HP_LIN))
	FilterTypeID= (aOrd & 1) ? HP_LIN : HP_CIN;

      FXMessageBox::information(ThisApp, MBOX_OK, "Filter Designer Advisory",
				"Topology automatically selected for R_l= INF");
    }

    switch (FilterCharID)
    {
        case BUTTERWORTH: LPPassiveBW(aOrd, aLCVals, RLinf); break;

        case CHEBYCHEV:
	  if (!((aOrd & 1) || RLinf))
	  {
	      FXMessageBox::error(ThisApp, MBOX_OK, "Filter Designer Error", CHEBYCHEV_ORDER_ERROR);
	      OrderSB->setRange(3, MAXORDER-1);
	      OrderSB->setIncrement(2);
	      return 1;
	  }

          LPPassiveChev(aOrd, aPBR, aLCVals, RLinf);
	  break;

        case BESSEL:
	  if (aOrd == 10)
	  {
	      FXMessageBox::information(ThisApp, MBOX_OK, "Filter Designer Advisory",
					"Orders up to 9 are available at this time");
	      return 1;
	  }

	  LPPassiveBes(aOrd, aLCVals, BesselOpt, RLinf);
	  break;
    }

    return 0;
}

//------------------------------------------------------------------------------

const FXString& AppWin::
ScaleActive(const FXuint aSect, const FXdouble aFreqC, const FXdouble aPBR, const FXdouble aRTrial, FXdouble *aRone, FXdouble *aRtwo)
{
    static FXString RetVal;
    FXdouble Cpp_1, Cpp_2;
    FXint k= 1;

    RetVal.clear();

    switch (FilterCharID)
    {
       case BUTTERWORTH:
	 RetVal.append(FXStringFormat("Characteristic: Butterworth; Cutoff Frequency: %s\n",
				      FormatData(aFreqC, 'f').text()));
         break;

       case CHEBYCHEV:
	 RetVal.append(FXStringFormat("Characteristic: Chebychev; Cutoff Frequency: %s; PB Ripple: %f(db)\n",
				      FormatData(aFreqC, 'f').text(), aPBR));
         break;

       case BESSEL:
       {
	   FXString BessOpt= ( BesselOpt == FREQ) ? "Frequency Optimized" : "Time Delay Optimized";

	   RetVal.append(FXStringFormat("Characteristic: Bessel; Cutoff Frequency = %s; Optimization= %s\n",
					FormatData(aFreqC, 'f').text(), BessOpt.text()));
       }
       break;
    }

    if (FilterTypeID == ACTIVE_LPF)
    {
      for (FXuint i= 0; i < aSect; i++)
      {
	  Cpp_1= aRone[i]/(ANGULAR(aFreqC) * aRTrial);
	  Cpp_2= aRtwo[i]/(ANGULAR(aFreqC) * aRTrial);

	  if (Cpp_2)
	  {
	      RetVal.append(FXStringFormat("\n%d) Second Order Section\n", k++));
	      RetVal.append(FXStringFormat("C_1= %s\n", FormatData(Cpp_1, 'c').text()));
	      RetVal.append(FXStringFormat("C_2= %s\n", FormatData(Cpp_2, 'c').text()));
	      RetVal.append(FXStringFormat("R= %s\n", FormatData(aRTrial, 'z').text()));
	  }
	  else
	  {
	      RetVal.append(FXStringFormat("\n%d) First Order Section\n", k++));
	      RetVal.append(FXStringFormat("C= %s\n", FormatData(Cpp_1*10.0, 'c').text()));
	      RetVal.append(FXStringFormat("R= %s\n", FormatData(aRTrial/10.0, 'z').text()));
	  }
      }

      return RetVal;
    }

    for (FXuint i= 0; i < aSect; i++)
    {
	Cpp_1= 1.0/(ANGULAR(aFreqC) * aRTrial);

	if (aRtwo[i])
	{
	    RetVal.append(FXStringFormat("\n%d) Second Order Section\n", k++));
	    RetVal.append(FXStringFormat("R_1= %s\n", FormatData(aRone[i]*aRTrial, 'z').text()));
	    RetVal.append(FXStringFormat("R_2= %s\n", FormatData(aRtwo[i]*aRTrial, 'z').text()));
	    RetVal.append(FXStringFormat("C= %s\n", FormatData(Cpp_1, 'c').text()));
	}
	else
	{
	    RetVal.append(FXStringFormat("\n%d) First Order Section\n", k++));
	    RetVal.append(FXStringFormat("R= %s\n", FormatData(aRone[i]*aRTrial/10.0, 'z').text()));
	    RetVal.append(FXStringFormat("C= %s\n", FormatData(Cpp_1*10.0, 'c').text()));
	}
    }

    return RetVal;
}
//------------------------------------------------------------------------------

const FXString& AppWin::
ScalePassive(const FXuint aOrder, const FXdouble aFc, const FXdouble aPBR, const FXdouble aRsrc, FXdouble *aLCVals)
{
    static FXString RetVal;
    FXdouble temp, newC, newL;

    RetVal.clear();

    switch (FilterCharID)
    {
       case BUTTERWORTH:
	   RetVal.append(FXStringFormat("Characteristic: Butterworth; Cutoff Frequency= %s\n", FormatData(aFc, 'f').text()));
	   break;

       case CHEBYCHEV:
	   RetVal.append(FXStringFormat("Characteristic: Chebychev; Cutoff Frequency= %s; Passband Ripple= %f(db)\n",
					FormatData(aFc, 'f').text(), aPBR));
	   break;

       case BESSEL:
       {
	   FXString BessOpt= ( BesselOpt == FREQ) ? "Frequency Optimized" : "Time Delay Optimized";

	   RetVal.append(FXStringFormat("Characteristic: Bessel; Cutoff Frequency = %s; Optimization= %s\n",
					FormatData(aFc, 'f').text(), BessOpt.text()));
       }
       break;
    }

    RetVal.append(FXStringFormat("R_src= %s\n", FormatData(aRsrc, 'z').text()));
    (RLinf) ? RetVal.append("R_load= INF\n") : RetVal.append(FXStringFormat("R_load= %s\n", FormatData(aRsrc, 'z').text()));
    RetVal.append('\n');

      for (int i= 0; i < aOrder; i++)
      {
	switch (FilterTypeID)
	{
	    case LP_CIN:
	      temp= aLCVals[i];

	      if (! (i & 1))
	      {
		newC= temp/(aRsrc * ANGULAR(aFc));
		RetVal.append(FXStringFormat("C_%d= %s\n", i+1, FormatData(newC, 'c').text()));
	      }
	      else
	      {
		newL= (temp * aRsrc)/ANGULAR(aFc);
		RetVal.append(FXStringFormat("L_%d= %s\n", i+1, FormatData(newL, 'l').text()));
	      }
	      break;

	    case LP_LIN:
	      temp= aLCVals[i];

	      if (! (i & 1))
	      {
		newL= (temp * aRsrc)/ANGULAR(aFc);
		RetVal.append(FXStringFormat("L_%d= %s\n", i+1, FormatData(newL, 'l').text()));
	      }
	      else
	      {
		newC= temp/(aRsrc * ANGULAR(aFc));
		RetVal.append(FXStringFormat("C_%d= %s\n", i+1, FormatData(newC, 'c').text()));
	      }
	      break;

	   case HP_CIN:
	      temp= 1.0/aLCVals[i];

	      if (! (i & 1))
	      {
		newC= temp/(aRsrc * ANGULAR(aFc));
		RetVal.append(FXStringFormat("C_%d= %s\n", i+1, FormatData(newC, 'c').text()));
	      }
	      else
	      {
		newL= (temp * aRsrc)/ANGULAR(aFc);
		RetVal.append(FXStringFormat("L_%d= %s\n", i+1, FormatData(newL, 'l').text()));
	      }
	      break;

	   case HP_LIN:
	      temp= 1.0/aLCVals[i];

	      if (! (i & 1))
	      {
		newL= (temp * aRsrc)/ANGULAR(aFc);
		RetVal.append(FXStringFormat("L_%d= %s\n", i+1, FormatData(newL, 'l').text()));
	      }
	      else
	      {
		newC= temp/(aRsrc * ANGULAR(aFc));
		RetVal.append(FXStringFormat("C_%d= %s\n", i+1, FormatData(newC, 'c').text()));
	      }
	      break;
	}
      }

    return RetVal;
}

//------------------------------------------------------------------------------

const FXString& AppWin::FormatData(FXdouble Current, FXchar IDCode)
{
    static FXString RetVal;

#ifdef INSTR
    printf("\nEntering function AppWin::FormatData");
#endif

    switch(IDCode)
    {
      case 'c':

	if (Current * 1.0E12 <= 6.8E3) RetVal= FXStringFormat("%fpF", Current * 1.0E12);
	else RetVal= FXStringFormat("%fuF", Current * 1.0E6);
	break;

      case 'f':

	if (Current / 1.0E6 >= 1.0) RetVal= FXStringFormat("%fMHz", Current/1.0E6);
	else if (Current / 1.0E3 >= 1.0) RetVal= FXStringFormat("%fKHz", Current/1.0E3);
	else RetVal= FXStringFormat("%fHz", Current);
	break;

      case 'l':

	if (Current >= 1.0) RetVal=  FXStringFormat("%fH", Current);
	else if (Current * 1000.0 >= 1.0) RetVal= FXStringFormat("%fmH", Current * 1000.0);
	else if (Current * 1.0E6 >= 1.0) RetVal= FXStringFormat("%fuH", Current * 1.0E6);
	else RetVal= FXStringFormat("%fnH", Current * 1.0E9);
	break;

      case 'z':

	if (Current / 1.0E6 >= 1.0) RetVal= FXStringFormat("%fMEG", Current/1.0E6);
	else if (Current / 1.0E3 >= 1.0) RetVal= FXStringFormat("%fK", Current/1.0E3);
	else RetVal= FXStringFormat("%fR", Current);
    }
#ifdef INSTR
    printf("\nCurrent= %E", Current);
    printf("\nRetVal= %s", RetVal.text());
    printf("\nLeaving function AppWin::FormatData");
#endif

    return RetVal;
}

//------------------------------------------------------------------------------

void AppWin::WriteOut(const FXString &TxtDesc, const FXuint aFID)
{
/*
    Text Metrics:
        Lines: Number of lines to write to image
        Length: Text length

    Font Metrics:
        FontHeight: the height of each line
        MarginX: Left margin of text in image (fixed)
	MarginY: Y co-ordinate of text

    Image Metrics:
        DiagramH: Height of circuit diagram
        ImgHeight: Total height of generated image
	ImgWidth: Total width of image (fixed).

    Images:
        CircuitSchemo: Circuit diagram image loaded from file
	ProjSchemos: Generated image displaying text, incorporating CircuitDiagram
*/

    FXint Length;
    FXint MarginY, FontHeight, CenterX, DiagramH;
    FXint MarginX= 30;
    FXint SchemoH= 200;
    FXint SchemoW= 500;
    const FXint OffsetX=   10;
    const FXint OffsetY=   20;
    const FXint ImgWidth=  690;
    const FXint ImgHeight= 447;
    FXchar *Buffer= 0, *Head= 0, *Tail= 0, C;

    FXString FileName;

    FXGIFImage *CircuitSchemo= 0;

    switch(aFID & -2)
    {
        case BW_PTC:
	    FileName= PTC_PIC;
            break;

        case BW_PTLC:
	    FileName= PTLC_PIC;
	    break;

        case BW_STC:
	    FileName= STC_PIC;
	    break;

        case BW_STLC:
	    FileName= STLC_PIC;
	    break;

        case MULTI_FB:
	    FileName= MULTI_FB_PIC;
	    break;

        case STATE_VAR:
	    FileName= STATE_VAR_PIC;
	    SchemoH= 284;
	    break;

        case ACTIVE_LPF:
	    FileName= (aFID & 1) ? ACTIVE_LPO_PIC : ACTIVE_LPE_PIC;
	    break;

        case ACTIVE_HPF:
	    FileName= (aFID & 1) ? ACTIVE_HPO_PIC : ACTIVE_HPE_PIC;
	    break;

        case LP_CIN:
	    FileName= LPF_CIN_PIC;
	    break;

        case LP_LIN:
	    FileName= LPF_LIN_PIC;
	    break;

        case HP_CIN:
	    FileName= HPF_CIN_PIC;
	    break;

        case HP_LIN:
	    FileName= HPF_LIN_PIC;
	    break;
    }

    FXFont *SysFont= ThisApp->getNormalFont();
    SysFont->create();
    FontHeight= SysFont->getFontHeight();

    CircuitSchemo= new FXGIFImage(ThisApp, NULL, IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    FXFileStream PixStream;

    if (PixStream.open(FileName, FXStreamLoad))
    {
	CircuitSchemo->loadPixels(PixStream);
	PixStream.close();
	CircuitSchemo->create();
	CenterX= (ImgWidth - SchemoW)/2;
	DiagramH= SchemoH;
    }
    else
    {
	FXMessageBox::information(ThisApp, MBOX_OK, "Filter Designer Advisory",
				  "Failed to open schemo image file: %s", FileName.text());
	delete CircuitSchemo;
	CircuitSchemo= 0;
	DiagramH= 0;
	CenterX= 0;
    }

//                           Set up for text write

    ProjSchemos= new FXGIFImage(ThisApp, NULL, IMAGE_SHMI|IMAGE_SHMP, ImgWidth, ImgHeight);
    ProjSchemos->create();

    FXDCWindow DevCntx(ProjSchemos);
    DevCntx.setFont(SysFont);
    DevCntx.setForeground(FXRGB(255, 255, 255));
    DevCntx.fillRectangle(0, 0, ImgWidth, ImgHeight);

    if (CircuitSchemo) DevCntx.drawImage(CircuitSchemo, CenterX, 0);

    DevCntx.setForeground(FXRGB(0, 0, 0));
    DevCntx.drawLine(OffsetX, DiagramH+OffsetY, ImgWidth-OffsetX, DiagramH+OffsetY);
    MarginY= DiagramH + 2*OffsetY;
    Length= TxtDesc.length();
    FXchar Oneline[Length];

    Buffer= (FXchar*)(TxtDesc.text());
    Head= Tail= Buffer;

    while((C= *Head++))
    {
	if (C == '\n')
	{
	    memcpy(&Oneline[0], Tail, Head-Tail);
	    Oneline[Head-Tail-1]= '\0';

	    if (Oneline[0]) DevCntx.drawText(MarginX, MarginY, Oneline, Head-Tail-1);

	    MarginY+= FontHeight;
	    Tail= Head;

	    if (MarginY >= (ImgHeight - OffsetY))
	    {
	      MarginX+= 228;
	      MarginY=  DiagramH + 2*OffsetY + 4*FontHeight;
	    }
	}
    }

    MainDisp->setImage(ProjSchemos);
    if (CircuitSchemo) delete CircuitSchemo;
    TxtResults= new FXString(TxtDesc);
}

//------------------------------------------------------------------------------

void AppWin::ProjectSave(void)
{
    FXuint   Code;
    FXString FileName;

    FXFileDialog SaveDlg(ThisApp, "Save Project to File");
    SaveDlg.setSelectMode(SELECTFILE_ANY);
    SaveDlg.setPatternList("Project (*.gif)");
    SaveDlg.setCurrentPattern(0);

    if (! SaveDlg.execute(PLACEMENT_OWNER))
    {
        FXMessageBox::information(ThisApp, MBOX_OK, "Filter Designer Advisory",
				  "File Save Cancelled");
        return;
    }

    FileName= SaveDlg.getFilename();

    if (FXStat::exists(FileName))
      Code= FXMessageBox::question(ThisApp, MBOX_YES_NO, "Filter Designer Question",
				   "Overwrite File: %s?", FileName.text());

    if (Code == MBOX_CLICKED_NO) return;

    FXFile WriteFile(FileName, FXFile::Writing);
    FXFileStream PixStream;
    ProjSchemos->restore();
    PixStream.open(FileName, FXStreamSave);
    ProjSchemos->savePixels(PixStream);
    PixStream.close();
}

//------------------------------------------------------------------------------

void AppWin::ProjectPrint(void)
{
    FXint Length;                                    //  Length of the text buffer
    const FXint MarginX=  60;                        //  Page metrics: fix the left margin
    FXint MarginY=        60, LineHeight;            //  Page metrics: start of top margin and line spacing
    const FXchar *Buffer= 0, *Head= 0, *Tail= 0;
    FXchar C;
    FXint i= 1;
    FXdouble PageHeight;

    FXPrinter MyPrinter;  //  Declair a print job
/*
          Invoke print dialog box to get the system printer
	  and set it up for printing
*/
    FXPrintDialog PDlgBox(this, "Print Current Project");
    if (PDlgBox.execute(PLACEMENT_OWNER))  //  Start if #1
    {
      PDlgBox.getPrinter(MyPrinter);
      FXDCPrint Pdc(ThisApp);  
      if (! Pdc.beginPrint(MyPrinter))
      {
        FXMessageBox::error(this, MBOX_OK, "Filter Designer Error", "Print Fault");
        return;
      }

      Length=          TxtResults->length();
      FXFont *SysFont= ThisApp->getNormalFont();
      SysFont->create();
      LineHeight=      SysFont->getFontHeight();
      PageHeight=      MyPrinter.mediaheight;

      FXchar OneLine[Length];
      Pdc.setFont(SysFont);
      Pdc.beginPage(i);
      TxtResults->append('\n');
      Buffer= TxtResults->text();
      Head=   Tail= Buffer;
/*
          Note: Parse the text buffer into a new string at the newline
	        breaks. Otherwise, printer treats the buffer as one
		long string. FOX bug or ideosyncracy?

          1)  Parse the string to newline (character copy)
	  2)  Append C string terminator (ASCII Null)
	  3)  Draw text into image and print.
	  4)  Update the vertical margin, and reset to grab the next line.
*/
      while ((C= *Tail++))
      {
	  if (C == '\n')
	  {
	      memcpy(&OneLine[0], Head, Tail-Head);
	      OneLine[Tail-Head-1]= '\0';
	      Pdc.drawText(MarginX, MarginY, OneLine, Tail-Head);
	      MarginY+= LineHeight;
	      Head= Tail;

	      if (PageHeight - MarginY < 145)
	      {
		  Pdc.endPage();
		  MarginY= 60;
		  Pdc.beginPage(i++);
	      }
	  }
      }

      Pdc.endPage();
      Pdc.endPrint();
    }

    else FXMessageBox::information(this, MBOX_OK, "Filter Designer Advisory", "Print job cancelled");
}

//------------------------------------------------------------------------------

