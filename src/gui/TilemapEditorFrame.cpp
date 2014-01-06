/******************************************************************************************************
 * Tile Map Editor
 * Copyright (C) 2009-2013 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * excluding commercial applications, and to alter it and redistribute it freely,
 * subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented;
 *    you must not claim that you wrote the original software.
 *    An acknowledgement in your documentation and link to the original version is required.
 *
 * 2. Altered source versions must be plainly marked as such,
 *    and must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 ******************************************************************************************************/

#include "TilemapEditorFrame.hpp"
#include "MapDocument.hpp"
#include "MapView.hpp"
#include <wx/config.h>
#include <wx/cmdproc.h>
#include <wx/docview.h>
#include <wx/docmdi.h>
#include <wx/msgdlg.h>

TilemapEditorFrame::TilemapEditorFrame(wxFrame* window) : wxDocManager(), TilemapEditorGUI(this, window)
{
    new wxDocTemplate(this, "Map", "*.map", ".", "map", "Map Doc", "Map Canvas",
                      CLASSINFO(MapDocument), CLASSINFO(MapView));

    FileHistoryLoad(*wxConfig::Get());
    FileHistoryUseMenu(menuRecent);
    FileHistoryAddFilesToMenu();

    //clock.add(mapCanvas);
    clock.setFramerate(60);
    clock.run();

    statusBar->SetStatusText("Hello Code::Blocks user!", 0);
    statusBar->SetStatusText("", 1);
}


TilemapEditorFrame::~TilemapEditorFrame()
{
    FileHistorySave(*wxConfig::Get());
}

wxFrame* TilemapEditorFrame::CreateChildFrame(wxView* view)
{
    // create a child frame of appropriate class for the current mode
    wxFrame *subframe;
    wxDocument *doc = view->GetDocument();
    subframe = new wxDocMDIChildFrame(doc, view, this, wxID_ANY, "Child Frame", wxDefaultPosition, wxSize(300, 300));

    clock.add(view);
    //doc->GetCommandProcessor()->SetEditMenu(editMenu);
    //doc->GetCommandProcessor()->Initialize();

    return subframe;
}

void TilemapEditorFrame::OnImportText(wxCommandEvent& event)
{

}

void TilemapEditorFrame::OnImportXml(wxCommandEvent& event)
{

}

void TilemapEditorFrame::OnImport(wxCommandEvent& event)
{

}

void TilemapEditorFrame::OnExportText(wxCommandEvent& event)
{

}

void TilemapEditorFrame::OnExportXml(wxCommandEvent& event)
{

}

void TilemapEditorFrame::OnExportAsImage(wxCommandEvent& event)
{

}

void TilemapEditorFrame::OnExportLayersAsImages(wxCommandEvent& event)
{

}

void TilemapEditorFrame::OnExport(wxCommandEvent& event)
{

}

void TilemapEditorFrame::OnClose(wxCloseEvent& event)
{
    Destroy();
}

void TilemapEditorFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = "";
    wxMessageBox(msg, "Welcome to...");
}
