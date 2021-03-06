/** \file wxsimagelistdlg.cpp
*
* This file is part of wxSmith plugin for Code::Blocks Studio
* Copyright (C) 2010 Gary Harris
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* wxSmith is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith. If not, see <http://www.gnu.org/licenses/>.
*
* This code was taken from the wxSmithImage plug-in, copyright Ron Collins
* and released under the GPL.
*
*/

//(*InternalHeaders(wxsImageListDlg)
#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/settings.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>
//*)

#include <wx/app.h>
#include <wx/msgdlg.h>
#include <wx/clrpicker.h>
#include <wx/dcmemory.h>
#include <wx/sstream.h>
#include <wx/tokenzr.h>
#include "../wxsitemresdata.h"
#include "../wxstool.h"

#include "wxsimagelistdlg.h"
#include "../wxsitem.h"


// for the base-64 stuff
// we copy the code into this file to avoid any conflict with the SDK routines of the same name
const wxChar fillchar = '=';
static wxString     cvt = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "abcdefghijklmnopqrstuvwxyz"
                             "0123456789+/");


//(*IdInit(wxsImageListDlg)
const long wxsImageListDlg::ID_STATICBOX3 = wxNewId();
const long wxsImageListDlg::ID_STATICBOX2 = wxNewId();
const long wxsImageListDlg::ID_STATICBOX1 = wxNewId();
const long wxsImageListDlg::ID_PANEL1 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT1 = wxNewId();
const long wxsImageListDlg::ID_PANEL2 = wxNewId();
const long wxsImageListDlg::ID_PANEL3 = wxNewId();
const long wxsImageListDlg::ID_PANEL4 = wxNewId();
const long wxsImageListDlg::ID_PANEL5 = wxNewId();
const long wxsImageListDlg::ID_PANEL6 = wxNewId();
const long wxsImageListDlg::ID_PANEL7 = wxNewId();
const long wxsImageListDlg::ID_PANEL8 = wxNewId();
const long wxsImageListDlg::ID_PANEL9 = wxNewId();
const long wxsImageListDlg::ID_PANEL10 = wxNewId();
const long wxsImageListDlg::ID_PANEL11 = wxNewId();
const long wxsImageListDlg::ID_BITMAPBUTTON1 = wxNewId();
const long wxsImageListDlg::ID_BITMAPBUTTON2 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT2 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT3 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT4 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT5 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT6 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT7 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT8 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT9 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT10 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT12 = wxNewId();
const long wxsImageListDlg::ID_BUTTON1 = wxNewId();
const long wxsImageListDlg::ID_BUTTON4 = wxNewId();
const long wxsImageListDlg::ID_BITMAPBUTTON3 = wxNewId();
const long wxsImageListDlg::ID_BITMAPBUTTON4 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT11 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT13 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT14 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT15 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT16 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT17 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT18 = wxNewId();
const long wxsImageListDlg::ID_BITMAPBUTTON5 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT20 = wxNewId();
const long wxsImageListDlg::ID_BUTTON2 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT21 = wxNewId();
const long wxsImageListDlg::ID_PANEL12 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT22 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT23 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT24 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT25 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT19 = wxNewId();
const long wxsImageListDlg::ID_CHECKBOX1 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT26 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT27 = wxNewId();
const long wxsImageListDlg::ID_STATICTEXT28 = wxNewId();
const long wxsImageListDlg::ID_BITMAPBUTTON6 = wxNewId();
const long wxsImageListDlg::ID_BITMAPBUTTON7 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wxsImageListDlg, wxDialog)
    //(*EventTable(wxsImageListDlg)
    //*)
END_EVENT_TABLE()

wxsImageListDlg::wxsImageListDlg(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
{
    //(*Initialize(wxsImageListDlg)
    Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    SetClientSize(wxSize(400,317));
    Move(wxDefaultPosition);
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
    wxFont thisFont(8,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    SetFont(thisFont);
    StaticBox3 = new wxStaticBox(this, ID_STATICBOX3, _("Options"), wxPoint(200,104), wxSize(200,160), 0, _T("ID_STATICBOX3"));
    StaticBox2 = new wxStaticBox(this, ID_STATICBOX2, _("Import From File"), wxPoint(0,104), wxSize(200,160), 0, _T("ID_STATICBOX2"));
    StaticBox1 = new wxStaticBox(this, ID_STATICBOX1, _("Image List"), wxPoint(0,0), wxSize(400,104), 0, _T("ID_STATICBOX1"));
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(8,144), wxSize(92,92), wxDOUBLE_BORDER|wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    Panel1->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HOTLIGHT));
    Panel1->SetToolTip(_("Click any spot on the image to select a transparent color"));
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Imported Image"), wxPoint(8,124), wxSize(92,16), wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_STATICTEXT1"));
    Panel2 = new wxPanel(this, ID_PANEL2, wxPoint(40,48), wxSize(32,32), wxDOUBLE_BORDER|wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    Panel2->SetBackgroundColour(wxColour(255,255,255));
    Panel3 = new wxPanel(this, ID_PANEL3, wxPoint(72,48), wxSize(32,32), wxDOUBLE_BORDER|wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    Panel3->SetBackgroundColour(wxColour(255,255,255));
    Panel4 = new wxPanel(this, ID_PANEL4, wxPoint(104,48), wxSize(32,32), wxDOUBLE_BORDER|wxTAB_TRAVERSAL, _T("ID_PANEL4"));
    Panel4->SetBackgroundColour(wxColour(255,255,255));
    Panel5 = new wxPanel(this, ID_PANEL5, wxPoint(136,48), wxSize(32,32), wxDOUBLE_BORDER|wxTAB_TRAVERSAL, _T("ID_PANEL5"));
    Panel5->SetBackgroundColour(wxColour(255,255,255));
    Panel6 = new wxPanel(this, ID_PANEL6, wxPoint(200,48), wxSize(32,32), wxDOUBLE_BORDER|wxTAB_TRAVERSAL, _T("ID_PANEL6"));
    Panel6->SetBackgroundColour(wxColour(255,255,255));
    Panel7 = new wxPanel(this, ID_PANEL7, wxPoint(232,48), wxSize(32,32), wxDOUBLE_BORDER|wxTAB_TRAVERSAL, _T("ID_PANEL7"));
    Panel7->SetBackgroundColour(wxColour(255,255,255));
    Panel8 = new wxPanel(this, ID_PANEL8, wxPoint(168,48), wxSize(32,32), wxDOUBLE_BORDER|wxTAB_TRAVERSAL, _T("ID_PANEL8"));
    Panel8->SetBackgroundColour(wxColour(255,255,255));
    Panel9 = new wxPanel(this, ID_PANEL9, wxPoint(264,48), wxSize(32,32), wxDOUBLE_BORDER|wxTAB_TRAVERSAL, _T("ID_PANEL9"));
    Panel9->SetBackgroundColour(wxColour(255,255,255));
    Panel10 = new wxPanel(this, ID_PANEL10, wxPoint(296,48), wxSize(32,32), wxDOUBLE_BORDER|wxTAB_TRAVERSAL, _T("ID_PANEL10"));
    Panel10->SetBackgroundColour(wxColour(255,255,255));
    Panel11 = new wxPanel(this, ID_PANEL11, wxPoint(328,48), wxSize(32,32), wxDOUBLE_BORDER|wxTAB_TRAVERSAL, _T("ID_PANEL11"));
    Panel11->SetBackgroundColour(wxColour(255,255,255));
    bLeft = new wxBitmapButton(this, ID_BITMAPBUTTON1, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_GO_BACK")),wxART_BUTTON), wxPoint(8,52), wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON1"));
    bLeft->SetDefault();
    bRight = new wxBitmapButton(this, ID_BITMAPBUTTON2, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_GO_FORWARD")),wxART_BUTTON), wxPoint(368,52), wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON2"));
    bRight->SetDefault();
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("1"), wxPoint(40,80), wxSize(32,16), wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_STATICTEXT2"));
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("2"), wxPoint(72,80), wxSize(32,16), wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_STATICTEXT3"));
    StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("3"), wxPoint(104,80), wxSize(32,16), wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_STATICTEXT4"));
    StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("4"), wxPoint(136,80), wxSize(32,16), wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_STATICTEXT5"));
    StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("5"), wxPoint(168,80), wxSize(32,16), wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_STATICTEXT6"));
    StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("6"), wxPoint(200,80), wxSize(32,16), wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_STATICTEXT7"));
    StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("7"), wxPoint(232,80), wxSize(32,16), wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_STATICTEXT8"));
    StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("8"), wxPoint(264,80), wxSize(32,16), wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_STATICTEXT9"));
    StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("9"), wxPoint(296,80), wxSize(32,16), wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_STATICTEXT10"));
    StaticText12 = new wxStaticText(this, ID_STATICTEXT12, _("10"), wxPoint(328,80), wxSize(32,16), wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_STATICTEXT12"));
    bOK = new wxButton(this, ID_BUTTON1, _("OK"), wxPoint(72,280), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    bOK->SetDefault();
    bCancel = new wxButton(this, ID_BUTTON4, _("Cancel"), wxPoint(256,280), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    bAdd = new wxBitmapButton(this, ID_BITMAPBUTTON3, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_ADD_BOOKMARK")),wxART_BUTTON), wxPoint(104,220), wxDefaultSize, wxBU_LEFT|wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON3"));
    bDel = new wxBitmapButton(this, ID_BITMAPBUTTON4, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_DEL_BOOKMARK")),wxART_BUTTON), wxPoint(304,144), wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON4"));
    bDel->SetDefault();
    StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _("Add To List"), wxPoint(136,224), wxDefaultSize, 0, _T("ID_STATICTEXT11"));
    StaticText13 = new wxStaticText(this, ID_STATICTEXT13, _("Delete Image"), wxPoint(336,148), wxDefaultSize, 0, _T("ID_STATICTEXT13"));
    StaticText14 = new wxStaticText(this, ID_STATICTEXT14, _("Name"), wxPoint(8,20), wxDefaultSize, 0, _T("ID_STATICTEXT14"));
    StaticText15 = new wxStaticText(this, ID_STATICTEXT15, _("Label"), wxPoint(40,20), wxSize(96,18), wxST_NO_AUTORESIZE|wxSUNKEN_BORDER, _T("ID_STATICTEXT15"));
    StaticText16 = new wxStaticText(this, ID_STATICTEXT16, _("Image Count"), wxPoint(144,20), wxDefaultSize, 0, _T("ID_STATICTEXT16"));
    StaticText17 = new wxStaticText(this, ID_STATICTEXT17, _("Label"), wxPoint(208,20), wxSize(32,18), wxST_NO_AUTORESIZE|wxALIGN_CENTRE|wxSUNKEN_BORDER, _T("ID_STATICTEXT17"));
    StaticText18 = new wxStaticText(this, ID_STATICTEXT18, _("Image Size"), wxPoint(248,20), wxDefaultSize, 0, _T("ID_STATICTEXT18"));
    bRead = new wxBitmapButton(this, ID_BITMAPBUTTON5, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FILE_OPEN")),wxART_BUTTON), wxPoint(104,144), wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON5"));
    bRead->SetDefault();
    StaticText20 = new wxStaticText(this, ID_STATICTEXT20, _("Read File"), wxPoint(136,148), wxDefaultSize, 0, _T("ID_STATICTEXT20"));
    bColor = new wxButton(this, ID_BUTTON2, _("C"), wxPoint(104,192), wxSize(24,24), 0, wxDefaultValidator, _T("ID_BUTTON2"));
    StaticText21 = new wxStaticText(this, ID_STATICTEXT21, _("Mask Color"), wxPoint(136,200), wxDefaultSize, 0, _T("ID_STATICTEXT21"));
    Panel12 = new wxPanel(this, ID_PANEL12, wxPoint(208,144), wxSize(92,92), wxDOUBLE_BORDER|wxTAB_TRAVERSAL, _T("ID_PANEL12"));
    Panel12->SetBackgroundColour(wxColour(255,255,255));
    StaticText22 = new wxStaticText(this, ID_STATICTEXT22, _("Selected Image"), wxPoint(208,124), wxSize(92,16), wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_STATICTEXT22"));
    StaticText23 = new wxStaticText(this, ID_STATICTEXT23, _("Label"), wxPoint(8,240), wxSize(92,14), wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_STATICTEXT23"));
    StaticText24 = new wxStaticText(this, ID_STATICTEXT24, _("Clear List"), wxPoint(336,178), wxDefaultSize, 0, _T("ID_STATICTEXT24"));
    StaticText25 = new wxStaticText(this, ID_STATICTEXT25, _("Export BMP"), wxPoint(336,208), wxDefaultSize, 0, _T("ID_STATICTEXT25"));
    StaticText19 = new wxStaticText(this, ID_STATICTEXT19, _("x"), wxPoint(344,20), wxDefaultSize, 0, _T("ID_STATICTEXT19"));
    cxTransparent = new wxCheckBox(this, ID_CHECKBOX1, _("Transparent"), wxPoint(108,174), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    cxTransparent->SetValue(false);
    StaticText26 = new wxStaticText(this, ID_STATICTEXT26, _("16"), wxPoint(304,20), wxSize(32,18), wxST_NO_AUTORESIZE|wxALIGN_CENTRE|wxSUNKEN_BORDER, _T("ID_STATICTEXT26"));
    StaticText27 = new wxStaticText(this, ID_STATICTEXT27, _("16"), wxPoint(360,20), wxSize(32,18), wxST_NO_AUTORESIZE|wxALIGN_CENTRE|wxSUNKEN_BORDER, _T("ID_STATICTEXT27"));
    StaticText28 = new wxStaticText(this, ID_STATICTEXT28, _("Label"), wxPoint(208,240), wxSize(88,14), wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_STATICTEXT28"));
    bClear = new wxBitmapButton(this, ID_BITMAPBUTTON6, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_DELETE")),wxART_BUTTON), wxPoint(304,174), wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON6"));
    bClear->SetDefault();
    bSave = new wxBitmapButton(this, ID_BITMAPBUTTON7, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FILE_SAVE")),wxART_BUTTON), wxPoint(304,204), wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON7"));
    bSave->SetDefault();
    ColourDialog1 = new wxColourDialog(this);
    FileDialog1 = new wxFileDialog(this, _("Select output file"), wxEmptyString, wxEmptyString, _("Bitmap Files|*.bmp|All Files|*.*"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));

    Panel1->Connect(wxEVT_PAINT,(wxObjectEventFunction)&wxsImageListDlg::OnPanel1Paint,0,this);
    Panel1->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&wxsImageListDlg::OnPanel1LeftUp,0,this);
    Panel2->Connect(wxEVT_PAINT,(wxObjectEventFunction)&wxsImageListDlg::PaintPreviewPanel,0,this);
    Panel2->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&wxsImageListDlg::OnPanel2LeftUp,0,this);
    Panel3->Connect(wxEVT_PAINT,(wxObjectEventFunction)&wxsImageListDlg::PaintPreviewPanel,0,this);
    Panel3->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&wxsImageListDlg::OnPanel3LeftUp,0,this);
    Panel4->Connect(wxEVT_PAINT,(wxObjectEventFunction)&wxsImageListDlg::PaintPreviewPanel,0,this);
    Panel4->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&wxsImageListDlg::OnPanel4LeftUp,0,this);
    Panel5->Connect(wxEVT_PAINT,(wxObjectEventFunction)&wxsImageListDlg::PaintPreviewPanel,0,this);
    Panel5->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&wxsImageListDlg::OnPanel5LeftUp,0,this);
    Panel6->Connect(wxEVT_PAINT,(wxObjectEventFunction)&wxsImageListDlg::PaintPreviewPanel,0,this);
    Panel6->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&wxsImageListDlg::OnPanel6LeftUp,0,this);
    Panel7->Connect(wxEVT_PAINT,(wxObjectEventFunction)&wxsImageListDlg::PaintPreviewPanel,0,this);
    Panel7->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&wxsImageListDlg::OnPanel7LeftUp,0,this);
    Panel8->Connect(wxEVT_PAINT,(wxObjectEventFunction)&wxsImageListDlg::PaintPreviewPanel,0,this);
    Panel8->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&wxsImageListDlg::OnPanel8LeftUp,0,this);
    Panel9->Connect(wxEVT_PAINT,(wxObjectEventFunction)&wxsImageListDlg::PaintPreviewPanel,0,this);
    Panel9->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&wxsImageListDlg::OnPanel9LeftUp,0,this);
    Panel10->Connect(wxEVT_PAINT,(wxObjectEventFunction)&wxsImageListDlg::PaintPreviewPanel,0,this);
    Panel10->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&wxsImageListDlg::OnPanel10LeftUp,0,this);
    Panel11->Connect(wxEVT_PAINT,(wxObjectEventFunction)&wxsImageListDlg::PaintPreviewPanel,0,this);
    Panel11->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&wxsImageListDlg::OnPanel11LeftUp,0,this);
    Connect(ID_BITMAPBUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageListDlg::OnbLeftClick);
    Connect(ID_BITMAPBUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageListDlg::OnbRightClick);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageListDlg::OnbOKClick);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageListDlg::OnbCancelClick);
    Connect(ID_BITMAPBUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageListDlg::OnbAddClick);
    Connect(ID_BITMAPBUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageListDlg::OnbDelClick);
    Connect(ID_BITMAPBUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageListDlg::OnbReadClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageListDlg::OnbColorClick);
    Panel12->Connect(wxEVT_PAINT,(wxObjectEventFunction)&wxsImageListDlg::OnPanel12Paint,0,this);
    Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&wxsImageListDlg::OncxTransparentClick);
    Connect(ID_BITMAPBUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageListDlg::OnbClearClick);
    Connect(ID_BITMAPBUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageListDlg::OnbSaveClick);
    //*)

    // reading images
    wxInitAllImageHandlers();

    // nothing yet
    m_ImageDialog   = new wxsBitmapIconEditorDlg(0, m_ImageData, _("wxART_OTHER"));
    m_ImportMask    = *wxWHITE;
    m_FirstImage    = 0;
    m_PreviewSelect = -1;

    // preview panels
    m_PreviewPanel[0] = Panel2;
    m_PreviewLabel[0] = StaticText2;
    m_PreviewPanel[1] = Panel3;
    m_PreviewLabel[1] = StaticText3;
    m_PreviewPanel[2] = Panel4;
    m_PreviewLabel[2] = StaticText4;
    m_PreviewPanel[3] = Panel5;
    m_PreviewLabel[3] = StaticText5;
    m_PreviewPanel[4] = Panel8;
    m_PreviewLabel[4] = StaticText6;
    m_PreviewPanel[5] = Panel6;
    m_PreviewLabel[5] = StaticText7;
    m_PreviewPanel[6] = Panel7;
    m_PreviewLabel[6] = StaticText8;
    m_PreviewPanel[7] = Panel9;
    m_PreviewLabel[7] = StaticText9;
    m_PreviewPanel[8] = Panel10;
    m_PreviewLabel[8] = StaticText10;
    m_PreviewPanel[9] = Panel11;
    m_PreviewLabel[9] = StaticText12;

    // change the cursor for the transparency preview panel
    Panel1->SetCursor(*wxCROSS_CURSOR);
}

wxsImageListDlg::~wxsImageListDlg()
{
    //(*Destroy(wxsImageListDlg)
    //*)

    if(m_ImageDialog != NULL) delete m_ImageDialog;

}

/*! \brief Run the dialogue.
 *
 * \param inName wxString&
 * \param aImageData wxArrayString&
 * \return bool
 *
 */
bool wxsImageListDlg::Execute(wxString &inName, wxArrayString &aImageData)
{
    int         n;
    int         w, h;
    wxString    ss;

	// need default sizes?
    if(aImageData.GetCount() < 2){
        aImageData.Clear();
        aImageData.Add(_("16"));
        aImageData.Add(_("16"));
    }

	// read image data into list
    ArrayToImageList(aImageData, m_ImageList);

	// some presets
    m_ImportMask    = *wxWHITE;
    m_FirstImage    = 0;
    m_PreviewSelect = -1;

	// the name
    StaticText15->SetLabel(inName);

	// each image size
    m_ImageList.GetSize(0, w, h);
    ss.Printf(_T("%d"), w);
    StaticText26->SetLabel(ss);
    ss.Printf(_T("%d"), h);
    StaticText27->SetLabel(ss);

	// and the current count
    n = m_ImageList.GetImageCount();
    ss.Printf(_T("%d"), n);
    StaticText17->SetLabel(ss);

	// update buttons
    UpdateEnabled();

	// show it
    n = ShowModal();

	// save new data?
    if(n == wxID_OK){
    	ImageListToArray(m_ImageList, aImageData);
    }

	// done
    return (n == wxID_OK);
}

/*! \brief Add an external file.
 *
 * \param event wxCommandEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnbAddClick(wxCommandEvent &event)
{
    int         n;
    int         iw, ih;
    int         bw, bh;
    wxBitmap    *bmp;
    wxString    ss;

	// no image?
    if(!m_ImportImage.IsOk()){
    	return;
    }

	// compare sizes
    m_ImageList.GetSize(0, iw, ih);
    bw = m_ImportImage.GetWidth();
    bh = m_ImportImage.GetHeight();

    if((bw > iw) || (bh > ih)){
        n = ::wxMessageBox(_("The image is larger than the size allowed for the list;\nDivide the image into multiple entries?"), _("Add Image"), wxYES_NO);
        if(n != wxYES){
        	return;
        }
    }

	// add the bitmap
    bmp = new wxBitmap(m_ImportImage);
    if(cxTransparent->GetValue()){
    	m_ImageList.Add(*bmp, m_ImportMask);
    }
    else{
    	m_ImageList.Add(*bmp);
    }
    delete bmp;

	// update the count
    n = m_ImageList.GetImageCount();
    ss.Printf(_T("%d"), n);
    StaticText17->SetLabel(ss);

	// re-draw the previews
    PreviewList();
    UpdateEnabled();
}

/*! \brief Add an external file.
 *
 * \param event wxCommandEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnbReadClick(wxCommandEvent &event)
{
    int         n;
    wxBitmap    bmp;

	// show the image select dialog
    n = m_ImageDialog->ShowModal();
    if(n != wxID_OK){
    	return;
    }

	// get what the user selected
    bmp = m_ImageData.GetPreview(wxDefaultSize);
    m_ImportImage = bmp.ConvertToImage();

	// a default mask color
    m_ImportMask = *wxWHITE;

	// show it
    PreviewImport();
}

/*! \brief Refresh the imported image preview.
 *
 * \param void
 * \return void
 *
 */
void wxsImageListDlg::PreviewImport(void)
{
    Panel1->Refresh();
}

/*! \brief Refresh the imported image list preview.
 *
 * \param void
 * \return void
 *
 */
void wxsImageListDlg::PreviewList(void)
{
    int     i;

    for(i = 0;i < 10;i++){
    	m_PreviewPanel[i]->Refresh();
    }
}

/*! \brief The OnPaint event.
 *
 * \param event wxPaintEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnPanel1Paint(wxPaintEvent &event)
{
    int         iw, ih;
    wxPaintDC   dc(Panel1);
    wxString    ss;

	// no image
    if(!m_ImportImage.IsOk()){
    	return;
    }

	// make a working copy of the original image
    wxImage img(m_ImportImage);

	// apply the mask color
    if(cxTransparent->GetValue()){
    	img.SetMaskColour(m_ImportMask.Red(), m_ImportMask.Green(), m_ImportMask.Blue());
    }

	// and then a bitmap to draw
    wxBitmap bmp(img);

	// image info
    iw = bmp.GetWidth();
    ih = bmp.GetHeight();
    ss.Printf(_T("%d x %d"), iw, ih);
    StaticText23->SetLabel(ss);

	// draw it
    PaintPanel(dc, Panel1, bmp);
}

/*! \brief The mask colour button was clicked.
 *
 * \param event wxCommandEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnbColorClick(wxCommandEvent &event)
{
    int             n;
    wxColourData    cd;
    wxColour        cc;

	// not allowed?
    if(!cxTransparent->GetValue()){
    	return;
    }

	// show the dialog
    n = ColourDialog1->ShowModal();
    if(n != wxID_OK){
    	return;
    }

	// get color data
    cd = ColourDialog1->GetColourData();
    cc = cd.GetColour();

	// save the mask colour
    m_ImportMask = cc;

	// show the colour on the button
    bColor->SetBackgroundColour(cc);

	// redraw the bitmap
    Panel1->Refresh();
}

/*! \brief Set a new mask colour.
 *
 * \param event wxMouseEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnPanel1LeftUp(wxMouseEvent &event)
{
    int         mx, my;
    wxClientDC  dc(Panel1);
    wxColour    cc;

	// not allowed?
    if(!cxTransparent->GetValue()){
    	return;
    }

	// where was the mouse?
    event.GetPosition(&mx, &my);

	// get the colour at that pixel
    dc.GetPixel(mx, my, &cc);

	// save it
    m_ImportMask = cc;

	// show the colour on the button
    bColor->SetBackgroundColour(cc);

	// redraw the bitmap
    Panel1->Refresh();
}

/*! \brief Update which buttons are enabled or disabled.
 *
 * \param void
 * \return void
 *
 */
void wxsImageListDlg::UpdateEnabled(void)
{
    int         n;
    bool        b;
    wxString    ss;

	// can the user select a transparent color?
    b = cxTransparent->GetValue();
    bColor->Enable(b);
    StaticText21->Enable(b);

	// this is also a good place to update image count
    n = m_ImageList.GetImageCount();
    ss.Printf(_T("%d"), n);
    StaticText17->SetLabel(ss);
}

/*! \brief The "transparent" checkbox was clicked.
 *
 * \param event wxCommandEvent&
 * \return void
 *
 */
void wxsImageListDlg::OncxTransparentClick(wxCommandEvent &event)
{
    UpdateEnabled();
    PreviewImport();
}

/*! \brief The "OK" button was clicked.
 *
 * \param event wxCommandEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnbOKClick(wxCommandEvent &event)
{
    EndModal(wxID_OK);
}

/*! \brief The "Cancel" button was clicked.
 *
 * \param event wxCommandEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnbCancelClick(wxCommandEvent &event)
{
    EndModal(wxID_CANCEL);
}

/*! \brief Paint the preview panel.
 *
 * \param event wxPaintEvent&
 * \return void
 *
 */
void wxsImageListDlg::PaintPreviewPanel(wxPaintEvent &event)
{
    int         i;
    int         ip, ix;
    wxPanel     *pnl;
    wxBitmap    bmp;
    wxString    ss, tt;
    wxPaintDC   dc((wxWindow *) event.GetEventObject());

	// which panel?
    ip = 0;
    pnl = (wxPanel *) event.GetEventObject();
    for(i = 0; i < 10; i++){
        if(pnl == m_PreviewPanel[i]){
        	ip = i;
        }
    };

	// which bitmap from the list?
    ix = m_FirstImage + ip;

	// the bitmap
    bmp = m_ImageList.GetBitmap(ix);

	// the label
    ss.Printf(_T("%d"), ix);
    m_PreviewLabel[ip]->SetLabel(ss);

	// draw it
    PaintPanel(dc, pnl, bmp, (ix == m_PreviewSelect));
}

/*! \brief The left mouse button was clicked.
 *
 * \param event wxCommandEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnbLeftClick(wxCommandEvent &event)
{

    m_FirstImage -= 1;
    if(m_FirstImage < 0) m_FirstImage = 0;

    PreviewList();
}

/*! \brief The right mouse button was clicked.
 *
 * \param event wxCommandEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnbRightClick(wxCommandEvent &event)
{
    int     n;

    m_FirstImage += 1;
    n = m_ImageList.GetImageCount();
    if(m_FirstImage >= n) m_FirstImage = n - 1;
    if(m_FirstImage  < 0) m_FirstImage = 0;

    PreviewList();
}

/*! \brief Preview panel 0 was selected.
 *
 * \param event wxMouseEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnPanel2LeftUp(wxMouseEvent &event)
{
    SelectPreviewPanel(0);
}

/*! \brief Preview panel 1 was selected.
 *
 * \param event wxMouseEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnPanel3LeftUp(wxMouseEvent &event)
{
    SelectPreviewPanel(1);
}

/*! \brief Preview panel 2 was selected.
 *
 * \param event wxMouseEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnPanel4LeftUp(wxMouseEvent &event)
{
    SelectPreviewPanel(2);
}

/*! \brief Preview panel 3 was selected.
 *
 * \param event wxMouseEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnPanel5LeftUp(wxMouseEvent &event)
{
    SelectPreviewPanel(3);
}

/*! \brief Preview panel 4 was selected.
 *
 * \param event wxMouseEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnPanel8LeftUp(wxMouseEvent &event)
{
    SelectPreviewPanel(4);
}

/*! \brief Preview panel 5 was selected.
 *
 * \param event wxMouseEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnPanel6LeftUp(wxMouseEvent &event)
{
    SelectPreviewPanel(5);
}

/*! \brief Preview panel 6 was selected.
 *
 * \param event wxMouseEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnPanel7LeftUp(wxMouseEvent &event)
{
    SelectPreviewPanel(6);
}

/*! \brief Preview panel 7 was selected.
 *
 * \param event wxMouseEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnPanel9LeftUp(wxMouseEvent &event)
{
    SelectPreviewPanel(7);
}

/*! \brief Preview panel 8 was selected.
 *
 * \param event wxMouseEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnPanel10LeftUp(wxMouseEvent &event)
{
    SelectPreviewPanel(8);
}

/*! \brief Preview panel 9 was selected.
 *
 * \param event wxMouseEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnPanel11LeftUp(wxMouseEvent &event)
{
    SelectPreviewPanel(9);
}

/*! \brief Select a preview panel by index.
 *
 * \param event wxMouseEvent&
 * \return void
 *
 */
void wxsImageListDlg::SelectPreviewPanel(int aIndex)
{
    m_PreviewSelect = aIndex + m_FirstImage;
    PreviewList();
    PreviewSelected();
}

/*! \brief Paint the selected image preview.
 *
 * \param event wxPaintEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnPanel12Paint(wxPaintEvent &event)
{
    wxPaintDC   dc(Panel12);
    wxBitmap    bmp;
    wxString    ss, tt;

	// a copy of the selected bitmap
    bmp = m_ImageList.GetBitmap(m_PreviewSelect);

	// the label
    ss.Printf(_("%d"), m_PreviewSelect);
    StaticText28->SetLabel(ss);

	// draw it
    PaintPanel(dc, Panel12, bmp);
}

/*! \brief The selected image preview was selected.
 *
 * \param void
 * \return void
 *
 */
void wxsImageListDlg::PreviewSelected(void)
{
    Panel12->Refresh();
}

/*! \brief The "Delete Image" button was clicked.
 *
 * \param event wxCommandEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnbDelClick(wxCommandEvent &event)
{
    int         n;
    wxString    ss;

	// anything to delete?
    if(m_PreviewSelect < 0) return;

	// are you sure?
    ss.Printf(_("Delete Image %d; Are You Sure?"), m_PreviewSelect);
    n = ::wxMessageBox(ss, _("Delete Image"), wxYES_NO);
    if(n != wxYES){
    	return;
    }

	// do it
    m_ImageList.Remove(m_PreviewSelect);

	// refresh
    m_PreviewSelect = -1;
    PreviewList();
    PreviewSelected();
    UpdateEnabled();
}

/*! \brief The "Clear List" button was clicked.
 *
 * \param event wxCommandEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnbClearClick(wxCommandEvent &event)
{
    int         n;
    wxString    ss;

	// are you sure?
    ss.Printf(_("Delete All Images. Are You Sure?"));
    n = ::wxMessageBox(ss, _("Delete Image"), wxYES_NO);
    if(n != wxYES){
    	return;
    }

	// do it
    m_ImageList.RemoveAll();

	// refresh
    m_PreviewSelect = -1;
    PreviewList();
    PreviewSelected();
    UpdateEnabled();
}

/*! \brief The "Export BMP" button was clicked.
 *
 * \param event wxCommandEvent&
 * \return void
 *
 */
void wxsImageListDlg::OnbSaveClick(wxCommandEvent &event)
{
    int         i, n;
    int         w, h;
    wxMemoryDC  dc;
    wxBitmap    *bmp;
    wxString    ss;

	// size of each image
    m_ImageList.GetSize(0, w, h);
    n = m_ImageList.GetImageCount();

	// anything to save
    if(n == 0){
    	return;
    }

	// make a bitmap and a drawing context
    bmp = new wxBitmap(n * w, h);
    dc.SelectObject(*bmp);

	// default background
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();

	// draw each object into the bitmap
    for(i = 0; i < n; i++){
    	m_ImageList.Draw(i, dc, i * w, 0, wxIMAGELIST_DRAW_NORMAL, true);
    }

	// save it
    n = FileDialog1->ShowModal();
    if(n == wxID_OK){
        ss = FileDialog1->GetPath();
        bmp->SaveFile(ss, wxBITMAP_TYPE_BMP);
    }

	// done
    delete bmp;
}

/*! \brief Paint a panel.
 *
 * \param aDC wxPaintDC&
 * \param aPanel wxPanel*
 * \param aBitmap wxBitmap&
 * \param aHot bool
 * \return void
 *
 */
void wxsImageListDlg::PaintPanel(wxPaintDC &aDC, wxPanel *aPanel, wxBitmap &aBitmap, bool aHot)
{
    int         iw, ih;
    int         pw, ph;
    double      sw, sh;
    int         x, y;
    wxBitmap    bmp;
    wxString    ss, tt;

	// nothing to do?
    if(aPanel == NULL){
    	return;
    }

	// default background
    if(aHot){
    	aDC.SetBackground(*wxBLUE_BRUSH);
    }
    else{
    	aDC.SetBackground(*wxWHITE_BRUSH);
    }

    aDC.Clear();

	// image and panel sizes
    iw = aBitmap.GetWidth();
    ih = aBitmap.GetHeight();
    if((iw < 0) || (ih < 0)){
    	return;
    }

    aPanel->GetClientSize(&pw, &ph);

	// set scale to fit in panel
    sw = 1.0;
    sh = 1.0;
    while(((((int)(sw * iw)) > pw) || (((int)(sh * ih)) > ph)) && (sw > 0.01) && (sh > 0.01)){
        sw -= 0.01;
        sh -= 0.01;
    };

    while(((int)(sw * iw)) < 8){
    	sw += 0.01;
    }
    while(((int)(sh * ih)) < 8){
    	sh += 0.01;
    }

    aDC.SetUserScale(sw, sh);

	// center it in the panel and draw it
    x = (pw - ((int)(sw * iw))) / 2;
    y = (ph - ((int)(sh * ih))) / 2;

    aDC.DrawBitmap(aBitmap, x, y, true);

	// reset the scale
    aDC.SetUserScale(1.0, 1.0);
}

/*! \brief Convert an wxImage to an XPM.
 *
 * \param inImage wxImage&
 * \param outArray wxArrayString&
 * \return void
 *
 */
void wxsImageListDlg::ImageToArray(wxImage &inImage, wxArrayString &outArray)
{
    wxString                ss, tt;
    wxStringOutputStream    os;
    wxStringTokenizer       tkz;

	// clear old junk
	outArray.Clear();

	// write as XPM data
    SaveXPM(&inImage, os);

    ss = os.GetString();

	// convert to an array; skip empty lines and comment lines
	// XPM images typically have a lot of black spaces, but the XML parser that
	// stores this data in the *.wxs file compresses white space
	// to preserve white space with a minimum of expanded data size, we convert
	// all data lines (all except the first line of the array) to base-64
    tkz.SetString(ss, _T("\r\n"));
    while(tkz.HasMoreTokens()) {
        tt = tkz.GetNextToken();
        tt.Trim(true);
        tt.Trim(false);
        if(tt.Length() == 0) continue;
        if(tt.StartsWith(_T("/*"))) continue;
        if(tt.StartsWith(_T("//"))) continue;

        outArray.Add(tt);
    };
}

/*! \brief Convert an XPM to a wxImage.
 *
 * \param inArray wxArrayString&
 * \param outImage wxImage&
 * \return void
 *
 */
void wxsImageListDlg::ArrayToImage(wxArrayString &inArray, wxImage &outImage)
{
    int                     i, n;
    wxString                ss, tt;
    wxStringInputStream     *ns;

	// make an input stream
    n = CalcArraySize(inArray);
    tt = _("");
    tt.Alloc(n + 64);

    for(i = 0; i < (int)inArray.GetCount(); i++){
        ss = inArray.Item(i);
        n = ss.Length();

        if(n == 0){
        	continue;
        }

        tt += ss;
        tt += _T("\n");
    }

    ns = new wxStringInputStream(tt);

	// use it to read an image
    outImage.LoadFile(*ns, wxBITMAP_TYPE_XPM);

	// done with input

    delete ns;
}

/*! \brief Convert an wxBitmap to an XPM.
 *
 * \param inBitmap wxBitmap&
 * \param outArray wxArrayString&
 * \return void
 *
 */
void wxsImageListDlg::BitmapToArray(wxBitmap &inBitmap, wxArrayString &outArray)
{
    wxImage     img;

    img = inBitmap.ConvertToImage();
    ImageToArray(img, outArray);
}

/*! \brief Convert an XPM to a wxBitmap.
 *
 * \param inArray wxArrayString&
 * \param outBitmap wxBitmap&
 * \return void
 *
 */
void wxsImageListDlg::ArrayToBitmap(wxArrayString &inArray, wxBitmap &outBitmap)
{
    wxImage     img;
    wxBitmap    *bmp;

    ArrayToImage(inArray, img);
    bmp = new wxBitmap(img);
    outBitmap = (*bmp);
}

/*! \brief Convert an image list to an array of XPMs.
 *
 * \param inList wxImageList&
 * \param outArray wxArrayString&
 * \return void
 *
 */
void wxsImageListDlg::ImageListToArray(wxImageList &inList, wxArrayString &outArray)
{
    int             i, j, k, n;
    wxBitmap        bmp;
    wxArrayString   aa;
    wxString        ss, tt;

	// clear old junk
    outArray.Clear();

	// the first 2 items are the width and height
    inList.GetSize(0, i, n);
    ss.Printf(_T("%d"), i);
    outArray.Add(ss);
    ss.Printf(_T("%d"), n);
    outArray.Add(ss);

	// get each bitmap from the list and add to outArray
    n = inList.GetImageCount();
    for(i = 0; i < n; i++){
        bmp = inList.GetBitmap(i);
        BitmapToArray(bmp, aa);

        k = aa.GetCount();
        for(j = 0; j < k; j++){
            ss = aa.Item(j);
            outArray.Add(ss);
        }
    }
}

/*! \brief Convert an array of XPMs to an image list.
 *
 * \param inArray wxArrayString&
 * \param outList wxImageList&
 * \return void
 *
 */
void wxsImageListDlg::ArrayToImageList(wxArrayString &inArray, wxImageList &outList)
{
    int             i, n;
    int             w, h;
    long            ll;
    wxBitmap        bmp;
    wxArrayString   aa;
    wxString        ss, tt;
    wxImageList     *list;

	// get the size of the image list
    w = 16;
    if(inArray.GetCount() >= 1){
        ss = inArray.Item(0);
        if(ss.ToLong(&ll)){
        	w = ll;
        }
    }

    h = 16;
    if(inArray.GetCount() >= 2){
        ss = inArray.Item(1);
        if(ss.ToLong(&ll)){
        	h = ll;
        }
    }

	// make a temp list of correct size
    list = new wxImageList(w, h, 128);

	// extract sub-arrays from the big array and make pictures out of them
    aa.Clear();
    i = 2;
    while(i < (int)inArray.GetCount()){
        ss = inArray.Item(i);
        i += 1;
        ss.Trim(true);
        ss.Trim(false);

        if(ss.Length() == 0){                      // empty string
            // ignore this line
        }
        else if(ss.Find(_T("xpm_data")) < 0){       // not the end of previous XPM data block
            aa.Add(ss);
        }
        else if(aa.GetCount() == 0){               // previous data block was empty
            // do nothing
        }
        else{                                      // end of XPM data; start next block of data
            ArrayToBitmap(aa, bmp);
            n = list->Add(bmp);
            aa.Clear();
            aa.Add(ss);
        }
    }

	// any left-over data?
    if(aa.GetCount() > 0){
        ArrayToBitmap(aa, bmp);
        list->Add(bmp);
        aa.Clear();
    }

	// return the image list, and decrement usage count for our local copy
    outList = (*list);
}

/*! \brief Copy an image list.
 *
 * \param inList wxImageList&
 * \param outList wxImageList&
 * \return void
 *
 */
void wxsImageListDlg::CopyImageList(wxImageList &inList, wxImageList &outList)
{
    int         i, n;
    int         w, h;
    wxBitmap    bmp;
    wxImageList *list;

	// size of old list
    inList.GetSize(0, w, h);
    n = inList.GetImageCount();

	// make a new list
    list = new wxImageList(w, h, n);

	// copy it
    for(i = 0; i < n; i++){
        bmp = inList.GetBitmap(i);
        list->Add(bmp);
    }

	// copy the reference back to the user
    outList = (*list);
}

/*! \brief Find a pointer to a wxsTool given its name.
 *
 * \param inItem wxsItem*
 * \param inName wxString
 * \return wxsItem    *
 *
 */
wxsItem     *wxsImageListDlg::FindTool(wxsItem *inItem, wxString inName)
{
    int             i, n;
    wxsItemResData  *res;
    wxsTool         *tool;
    wxsTool         *found;
    wxString        ss, tt;

    static wxsItem  *last_item;

	// keep track of last item used, for those cases where we cannot pass in an item
    if(inItem != NULL){
    	last_item = inItem;
    }

	// still nothing?
    if(last_item == NULL){
    	return NULL;
    }

	// nothing yet
    found = NULL;

	// pointer to resource data
    res = last_item->GetResourceData();

	// scan all the tools and check the names
    n = res->GetToolsCount();
    for(i = 0; i < n; i++){
        tool = res->GetTool(i);
        ss = tool->GetVarName();
        if(ss == inName){
        	found = tool;
        }
    }

	// done
    return found;
}

/*! \brief Estimate the amount of space is needed for a single string to hold an array.
 *
 * \param inArray wxArrayString&
 * \return int
 *
 */
int     wxsImageListDlg::CalcArraySize(wxArrayString &inArray)
{
    int     i, n;

    n = 0;
    for(i = 0; i < (int)inArray.GetCount(); i++){
    	n += inArray.Item(i).Length() + 1;
    }

    return n;
}

/*! \brief Save a wxImage as an XPM stream.
 *
 * \param image wxImage*
 * \param stream wxOutputStream&
 * \return bool
 *
 * We can not use the normal XPM savefile routine in wxImage because it uses
 * spaces " " and percents "%" as part of the encoding.
 * Spaces are compressed by the TinyXml library (which reads and writes the
 * wxSmith *.wxs file) and percents are used by Codef() for argument expansion
 * otherwise, this save routine is identical to the one normally used by wxImage
 */
bool wxsImageListDlg::SaveXPM(wxImage *image, wxOutputStream &stream)
{
    // 1. count colours:
#define MaxCixels  90
    static const char Cixel[MaxCixels+1] =
        "_.XoO+@#$&*=-;:>,<1234567890qwertyuipasdfghjk"
        "lzxcvbnmMNBVCZASDFGHJKLPIUYTREWQ!~^/()`'][{}|";
    int i, j, k;

    wxImageHistogram histogram;
    int cols = int(image->ComputeHistogram(histogram));

    int chars_per_pixel = 1;
    for(k = MaxCixels; cols > k; k *= MaxCixels){
        chars_per_pixel++;
    }

    // 2. write the header:
    wxString sName;
    if(image->HasOption(wxIMAGE_OPTION_FILENAME))
    {
        wxSplitPath(image->GetOption(wxIMAGE_OPTION_FILENAME),
                    NULL, &sName, NULL);
        sName << wxT("_xpm");
    }

    if(!sName.empty()){
        sName = wxString(wxT("/* XPM */\nstatic const char *")) + sName;
    }
    else{
        sName = wxT("/* XPM */\nstatic const char *xpm_data");
    }
    stream.Write((const char *) sName.ToAscii(), sName.Len());

    char tmpbuf[200];
    // VS: 200b is safe upper bound for anything produced by sprintf below
    //     (<101 bytes the string, neither %i can expand into more than 10 chars)
    sprintf(tmpbuf,
            "[] = {\n"
            "/* columns rows colors chars-per-pixel */\n"
            "\"%i %i %i %i\",\n",
            image->GetWidth(), image->GetHeight(), cols, chars_per_pixel);
    stream.Write(tmpbuf, strlen(tmpbuf));

    // 3. create color symbols table:
    char *symbols_data = new char[cols * (chars_per_pixel+1)];
    char **symbols = new char*[cols];

    // 2a. find mask colour:
    unsigned long mask_key = 0x1000000 /*invalid RGB value*/;
    if(image->HasMask())
        mask_key = (image->GetMaskRed() << 16) |
                   (image->GetMaskGreen() << 8) | image->GetMaskBlue();

    // 2b. generate colour table:
    for(wxImageHistogram::iterator entry = histogram.begin();
            entry != histogram.end(); ++entry)
    {
        unsigned long index = entry->second.index;
        symbols[index] = symbols_data + index * (chars_per_pixel + 1);
        char *sym = symbols[index];

        for(j = 0; j < chars_per_pixel; j++)
        {
            sym[j] = Cixel[index % MaxCixels];
            index /= MaxCixels;
        }
        sym[j] = '\0';

        unsigned long key = entry->first;

        if(key == 0)
            sprintf(tmpbuf, "\"%s c Black\",\n", sym);
        else if(key == mask_key)
            sprintf(tmpbuf, "\"%s c None\",\n", sym);
        else
        {
            wxByte r = wxByte(key >> 16);
            wxByte g = wxByte(key >> 8);
            wxByte b = wxByte(key);
            sprintf(tmpbuf, "\"%s c #%02X%02X%02X\",\n", sym, r, g, b);
        }
        stream.Write(tmpbuf, strlen(tmpbuf));
    }

    stream.Write("/* pixels */\n", 13);

    unsigned char *data = image->GetData();
    for(j = 0; j < image->GetHeight(); j++)
    {
        char tmp_c;
        tmp_c = '\"';
        stream.Write(&tmp_c, 1);
        for(i = 0; i < image->GetWidth(); i++, data += 3)
        {
            unsigned long key = (data[0] << 16) | (data[1] << 8) | (data[2]);
            stream.Write(symbols[histogram[key].index], chars_per_pixel);
        }
        tmp_c = '\"';
        stream.Write(&tmp_c, 1);
        if(j + 1 < image->GetHeight())
        {
            tmp_c = ',';
            stream.Write(&tmp_c, 1);
        }
        tmp_c = '\n';
        stream.Write(&tmp_c, 1);
    }
    stream.Write("};\n", 3);

    // Clean up:
    delete[] symbols;
    delete[] symbols_data;

    return true;
}
