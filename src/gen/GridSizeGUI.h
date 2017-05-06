///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 17 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GridSizeGUI__
#define __GridSizeGUI__

#include <wx/intl.h>

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class GridSizeGUI
///////////////////////////////////////////////////////////////////////////////
class GridSizeGUI : public wxDialog
{
private:

protected:
    wxStaticText* m_staticText1;
    wxSpinCtrl* gridSizeX;
    wxStaticText* m_staticText2;
    wxSpinCtrl* gridSizeY;

    wxButton* m_button1;
    wxButton* m_button2;

public:

    GridSizeGUI( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Grid Size"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 158,114 ), long style = wxDEFAULT_DIALOG_STYLE );
    ~GridSizeGUI();

};

#endif //__GridSizeGUI__
