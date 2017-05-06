///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  5 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __MAPNOTEBOOKGUI_H__
#define __MAPNOTEBOOKGUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/wxFlatNotebook/wxFlatNotebook.h>
#ifdef __VISUALC__
#include <wx/link_additions.h>
#endif //__VISUALC__
#include <wx/sizer.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MapNotebookPanel
///////////////////////////////////////////////////////////////////////////////
class MapNotebookPanel : public wxPanel
{
private:

protected:
    wxFlatNotebook* docNotebook;
    wxPanel* startPage;

    // Virtual event handlers, overide them in your derived class
    virtual void OnClosePage( wxFlatNotebookEvent& event )
    {
        event.Skip();
    }


public:

    MapNotebookPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
    ~MapNotebookPanel();

};

#endif //__MAPNOTEBOOKGUI_H__
