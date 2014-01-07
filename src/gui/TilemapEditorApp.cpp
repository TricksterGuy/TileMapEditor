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

#include "TilemapEditorApp.hpp"
#include <wx/image.h>

#include "Handlers.hpp"
#include "TilemapEditorFrame.hpp"

IMPLEMENT_APP(TilemapEditorApp);

bool TilemapEditorApp::OnInit()
{
    wxInitAllImageHandlers();

    MapHandlerManager().Add(new BinaryMapHandler());
    MapHandlerManager().Add(new TextMapHandler());
    MapHandlerManager().Add(new ImageMapHandler());
    MapHandlerManager().Add(new GBAImageHandler());
    MapHandlerManager().Add(new GBAMapHandler());
    MapHandlerManager().Add(new XmlMapHandler());

    // Fill in the application information fields before creating wxConfig.
    SetVendorName("wxWidgets");
    SetAppName("wx_docview_sample");
    SetAppDisplayName("wxWidgets DocView Sample");

    frame = new TilemapEditorFrame(NULL);
    SetTopWindow(frame);
    frame->Centre();
    frame->Show();

    return true;
}

wxFrame* TilemapEditorApp::CreateChildFrame(wxView* view)
{
    return frame->CreateChildFrame(view);
}
