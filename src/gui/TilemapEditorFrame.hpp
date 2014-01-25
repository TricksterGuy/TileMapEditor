/******************************************************************************************************
 * Tile Map Editor
 * Copyright (C) 2009-2014 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
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

#ifndef TILEMAPEDITORFRAME_HPP
#define TILEMAPEDITORFRAME_HPP

#include <wx/docview.h>

#include "TilemapEditorFrame.hpp"
#include "TilemapEditorGUI.h"
#include "Clock.hpp"
#include "MapCanvas.hpp"

/**
  *  Represents the main class of the application
  */
class TilemapEditorFrame : public wxDocManager, public TilemapEditorGUI
{
    public:
        TilemapEditorFrame(wxFrame *frame);
        ~TilemapEditorFrame();
        wxFrame* CreateChildFrame(wxView* doc);
    private:
        // File Menu
		virtual void OnImportText(wxCommandEvent& event);
		virtual void OnImportXml(wxCommandEvent& event);
		virtual void OnImport(wxCommandEvent& event);
		virtual void OnExportText(wxCommandEvent& event);
		virtual void OnExportXml(wxCommandEvent& event);
		virtual void OnExportAsImage(wxCommandEvent& event);
		virtual void OnExportLayersAsImages(wxCommandEvent& event);
		virtual void OnExport(wxCommandEvent& event);
		virtual void OnClose(wxCloseEvent& event);
		// Help menu
        virtual void OnAbout(wxCommandEvent& event);
        Clock clock;
};

#endif
