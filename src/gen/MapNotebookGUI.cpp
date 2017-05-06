///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  5 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "MapNotebookGUI.h"

///////////////////////////////////////////////////////////////////////////

MapNotebookPanel::MapNotebookPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
    wxBoxSizer* bSizer1;
    bSizer1 = new wxBoxSizer( wxVERTICAL );

    docNotebook = new wxFlatNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFNB_COLORFUL_TABS|wxFNB_DROPDOWN_TABS_LIST|wxFNB_NO_NAV_BUTTONS|wxFNB_NO_X_BUTTON|wxFNB_SMART_TABS|wxFNB_X_ON_TAB);

    docNotebook->SetCustomizeOptions( wxFNB_CUSTOM_ALL );
    docNotebook->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

    startPage = new wxPanel( docNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    docNotebook->AddPage( startPage, wxT("Start"), false );

    bSizer1->Add( docNotebook, 1, wxEXPAND, 0 );


    this->SetSizer( bSizer1 );
    this->Layout();

    // Connect Events
    docNotebook->Connect( wxEVT_COMMAND_FLATNOTEBOOK_PAGE_CLOSED, wxFlatNotebookEventHandler( MapNotebookPanel::OnClosePage ), NULL, this );
}

MapNotebookPanel::~MapNotebookPanel()
{
    // Disconnect Events
    docNotebook->Disconnect( wxEVT_COMMAND_FLATNOTEBOOK_PAGE_CLOSED, wxFlatNotebookEventHandler( MapNotebookPanel::OnClosePage ), NULL, this );

}
