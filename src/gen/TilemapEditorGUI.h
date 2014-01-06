///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan  4 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __TILEMAPEDITORGUI_H__
#define __TILEMAPEDITORGUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statusbr.h>
#include <wx/docmdi.h>

///////////////////////////////////////////////////////////////////////////

#define ID_IMPORT_TEXT 1000
#define ID_IMPORT_XML 1001
#define ID_IMPORT 1002
#define ID_EXPORT_TEXT 1003
#define ID_EXPORT_XML 1004
#define ID_EXPORT_AS_IMAGE 1005
#define ID_EXPORT_LAYERS_AS_IMAGES 1006
#define ID_EXPORT 1007

///////////////////////////////////////////////////////////////////////////////
/// Class TilemapEditorGUI
///////////////////////////////////////////////////////////////////////////////
class TilemapEditorGUI : public wxDocMDIParentFrame 
{
	private:
	
	protected:
		wxMenuBar* menubar;
		wxMenu* fileMenu;
		wxMenu* menuRecent;
		wxMenu* editMenu;
		wxMenu* helpMenu;
		wxStatusBar* statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnImportText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnImportXml( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnImport( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExportText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExportXml( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExportAsImage( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExportLayersAsImages( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExport( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		TilemapEditorGUI( wxDocManager* manager, wxFrame* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("TilemapEditor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 481,466 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~TilemapEditorGUI();
	
};

#endif //__TILEMAPEDITORGUI_H__
