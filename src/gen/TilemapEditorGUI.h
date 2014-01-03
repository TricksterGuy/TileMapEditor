///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __TILEMAPEDITORGUI_H__
#define __TILEMAPEDITORGUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
class MapCanvas;

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define idMenuQuit 1000
#define idMenuAbout 1001

///////////////////////////////////////////////////////////////////////////////
/// Class TilemapEditorGUI
///////////////////////////////////////////////////////////////////////////////
class TilemapEditorGUI : public wxFrame
{
	private:

	protected:
		wxMenuBar* menubar;
		wxMenu* fileMenu;
		wxMenu* helpMenu;
		MapCanvas* mapCanvas;
		wxStatusBar* statusBar;

		// Virtual event handlers, overide them in your derived class
		virtual void onClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void onClose( wxCommandEvent& event ) { event.Skip(); }
		virtual void onAbout( wxCommandEvent& event ) { event.Skip(); }


	public:

		TilemapEditorGUI( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("TileMapEditor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 481,466 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~TilemapEditorGUI();

};

#endif //__TILEMAPEDITORGUI_H__
