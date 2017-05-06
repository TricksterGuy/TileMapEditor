///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 17 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "GridSizeGUI.h"

///////////////////////////////////////////////////////////////////////////

GridSizeGUI::GridSizeGUI( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    wxBoxSizer* bSizer5;
    bSizer5 = new wxBoxSizer( wxVERTICAL );

    wxFlexGridSizer* fgSizer1;
    fgSizer1 = new wxFlexGridSizer( 1, 2, 0, 0 );
    fgSizer1->SetFlexibleDirection( wxBOTH );
    fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    wxBoxSizer* bSizer2;
    bSizer2 = new wxBoxSizer( wxVERTICAL );

    m_staticText1 = new wxStaticText( this, wxID_ANY, _("Width"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText1->Wrap( -1 );
    bSizer2->Add( m_staticText1, 0, wxALL, 5 );

    gridSizeX = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 64,-1 ), wxSP_ARROW_KEYS, 1, 1024, 1 );
    bSizer2->Add( gridSizeX, 0, wxALL, 5 );

    fgSizer1->Add( bSizer2, 1, wxEXPAND, 5 );

    wxBoxSizer* bSizer3;
    bSizer3 = new wxBoxSizer( wxVERTICAL );

    m_staticText2 = new wxStaticText( this, wxID_ANY, _("Height"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText2->Wrap( -1 );
    bSizer3->Add( m_staticText2, 0, wxALL, 5 );

    gridSizeY = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 64,-1 ), wxSP_ARROW_KEYS, 1, 1024, 1 );
    bSizer3->Add( gridSizeY, 0, wxALL, 5 );

    fgSizer1->Add( bSizer3, 1, wxEXPAND, 5 );

    bSizer5->Add( fgSizer1, 1, wxEXPAND, 5 );


    bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );

    wxBoxSizer* bSizer4;
    bSizer4 = new wxBoxSizer( wxHORIZONTAL );

    m_button1 = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxSize( 64,-1 ), 0 );
    bSizer4->Add( m_button1, 0, wxALL, 5 );

    m_button2 = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxSize( 64,-1 ), 0 );
    bSizer4->Add( m_button2, 0, wxALL, 5 );

    bSizer5->Add( bSizer4, 1, wxEXPAND, 5 );

    this->SetSizer( bSizer5 );
    this->Layout();
}

GridSizeGUI::~GridSizeGUI()
{
}
