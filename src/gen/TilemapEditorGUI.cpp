///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan  4 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "TilemapEditorGUI.h"

///////////////////////////////////////////////////////////////////////////

TilemapEditorGUI::TilemapEditorGUI( wxDocManager* manager, wxFrame* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDocMDIParentFrame( manager, parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	menubar = new wxMenuBar( 0 );
	fileMenu = new wxMenu();
	wxMenuItem* fileNew;
	fileNew = new wxMenuItem( fileMenu, wxID_NEW, wxString( wxT("&New") ) + wxT('\t') + wxT("Ctrl+N"), wxT("Creates a new map"), wxITEM_NORMAL );
	fileMenu->Append( fileNew );

	wxMenuItem* open;
	open = new wxMenuItem( fileMenu, wxID_OPEN, wxString( wxT("&Open") ) + wxT('\t') + wxT("Ctrl+O"), wxT("Opens a map from a file"), wxITEM_NORMAL );
	fileMenu->Append( open );

	wxMenuItem* save;
	save = new wxMenuItem( fileMenu, wxID_SAVE, wxString( wxT("&Save") ) + wxT('\t') + wxT("Ctrl+S"), wxT("Saves a map to a file"), wxITEM_NORMAL );
	fileMenu->Append( save );

	wxMenuItem* saveAs;
	saveAs = new wxMenuItem( fileMenu, wxID_SAVEAS, wxString( wxT("Save &As") ) + wxT('\t') + wxT("F12"), wxT("Saves map to a new file"), wxITEM_NORMAL );
	fileMenu->Append( saveAs );

	fileMenu->AppendSeparator();

	wxMenuItem* importText;
	importText = new wxMenuItem( fileMenu, ID_IMPORT_TEXT, wxString( wxT("Import Text") ) , wxT("Imports a map from text"), wxITEM_NORMAL );
	fileMenu->Append( importText );

	wxMenuItem* importXml;
	importXml = new wxMenuItem( fileMenu, ID_IMPORT_XML, wxString( wxT("Import XML") ) , wxT("Imports a map from xml"), wxITEM_NORMAL );
	fileMenu->Append( importXml );

	wxMenuItem* import;
	import = new wxMenuItem( fileMenu, ID_IMPORT, wxString( wxT("&Import...") ) , wxT("Imports a map from other format"), wxITEM_NORMAL );
	fileMenu->Append( import );

	fileMenu->AppendSeparator();

	wxMenuItem* exportText;
	exportText = new wxMenuItem( fileMenu, ID_EXPORT_TEXT, wxString( wxT("Export Text") ) , wxT("Exports a map to text"), wxITEM_NORMAL );
	fileMenu->Append( exportText );

	wxMenuItem* exportXml;
	exportXml = new wxMenuItem( fileMenu, ID_EXPORT_XML, wxString( wxT("Export XML") ) , wxT("Exports a map to xml"), wxITEM_NORMAL );
	fileMenu->Append( exportXml );

	wxMenuItem* exportAsImage;
	exportAsImage = new wxMenuItem( fileMenu, ID_EXPORT_AS_IMAGE, wxString( wxT("Export As Image") ) , wxT("Exports the map as an image"), wxITEM_NORMAL );
	fileMenu->Append( exportAsImage );

	wxMenuItem* exportLayersAsImages;
	exportLayersAsImages = new wxMenuItem( fileMenu, ID_EXPORT_LAYERS_AS_IMAGES, wxString( wxT("Export Layers As Images") ) , wxT("Exports each layer as an image"), wxITEM_NORMAL );
	fileMenu->Append( exportLayersAsImages );

	wxMenuItem* fileExport;
	fileExport = new wxMenuItem( fileMenu, ID_EXPORT, wxString( wxT("&Export...") ) , wxT("Exports a map to other format"), wxITEM_NORMAL );
	fileMenu->Append( fileExport );

	fileMenu->AppendSeparator();

	menuRecent = new wxMenu();
	fileMenu->Append( -1, wxT("Recent"), menuRecent );

	fileMenu->AppendSeparator();

	wxMenuItem* menuFileQuit;
	menuFileQuit = new wxMenuItem( fileMenu, wxID_CLOSE, wxString( wxT("&Quit") ) + wxT('\t') + wxT("Alt+F4"), wxT("Quit the application"), wxITEM_NORMAL );
	fileMenu->Append( menuFileQuit );

	menubar->Append( fileMenu, wxT("&File") );

	editMenu = new wxMenu();
	wxMenuItem* undo;
	undo = new wxMenuItem( editMenu, wxID_UNDO, wxString( wxT("&Undo") ) + wxT('\t') + wxT("Ctrl+Z"), wxT("Undos last command"), wxITEM_NORMAL );
	editMenu->Append( undo );

	wxMenuItem* redo;
	redo = new wxMenuItem( editMenu, wxID_REDO, wxString( wxT("&Redo") ) + wxT('\t') + wxT("Ctrl+Y"), wxT("Redos last undone command"), wxITEM_NORMAL );
	editMenu->Append( redo );

	menubar->Append( editMenu, wxT("&Edit") );

	helpMenu = new wxMenu();
	wxMenuItem* menuHelpAbout;
	menuHelpAbout = new wxMenuItem( helpMenu, wxID_HELP, wxString( wxT("&About") ) + wxT('\t') + wxT("F1"), wxT("Show info about this application"), wxITEM_NORMAL );
	helpMenu->Append( menuHelpAbout );

	menubar->Append( helpMenu, wxT("&Help") );

	this->SetMenuBar( menubar );

	statusBar = this->CreateStatusBar( 2, wxST_SIZEGRIP, wxID_ANY );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( TilemapEditorGUI::OnClose ) );
	this->Connect( importText->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TilemapEditorGUI::OnImportText ) );
	this->Connect( importXml->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TilemapEditorGUI::OnImportXml ) );
	this->Connect( import->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TilemapEditorGUI::OnImport ) );
	this->Connect( exportText->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TilemapEditorGUI::OnExportText ) );
	this->Connect( exportXml->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TilemapEditorGUI::OnExportXml ) );
	this->Connect( exportAsImage->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TilemapEditorGUI::OnExportAsImage ) );
	this->Connect( exportLayersAsImages->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TilemapEditorGUI::OnExportLayersAsImages ) );
	this->Connect( fileExport->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TilemapEditorGUI::OnExport ) );
	this->Connect( menuHelpAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TilemapEditorGUI::OnAbout ) );
}

TilemapEditorGUI::~TilemapEditorGUI()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( TilemapEditorGUI::OnClose ) );
	this->Disconnect( ID_IMPORT_TEXT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TilemapEditorGUI::OnImportText ) );
	this->Disconnect( ID_IMPORT_XML, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TilemapEditorGUI::OnImportXml ) );
	this->Disconnect( ID_IMPORT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TilemapEditorGUI::OnImport ) );
	this->Disconnect( ID_EXPORT_TEXT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TilemapEditorGUI::OnExportText ) );
	this->Disconnect( ID_EXPORT_XML, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TilemapEditorGUI::OnExportXml ) );
	this->Disconnect( ID_EXPORT_AS_IMAGE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TilemapEditorGUI::OnExportAsImage ) );
	this->Disconnect( ID_EXPORT_LAYERS_AS_IMAGES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TilemapEditorGUI::OnExportLayersAsImages ) );
	this->Disconnect( ID_EXPORT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TilemapEditorGUI::OnExport ) );
	this->Disconnect( wxID_HELP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TilemapEditorGUI::OnAbout ) );

}
