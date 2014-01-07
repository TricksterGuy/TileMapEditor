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

#include <iostream>
#include <fstream>
#include <wx/string.h>
#include <wx/msgdlg.h>
#include "GBAMapHandler.hpp"
#include "HandlerUtils.hpp"

///TODO Rewrite this.

GBAMapHandler::GBAMapHandler() : BaseMapHandler("GBA Map Export", "c", "Exports the map in GBA modes (0-2) format", false, false)
{
    /// TODO Not implemented so disable
    readable = false;
    writeable = false;
}

GBAMapHandler::~GBAMapHandler()
{

}

void GBAMapHandler::Save(const std::string& filename, Map& map)
{
    wxString warnings = "";
    if (map.GetTileWidth() != 8 || map.GetTileHeight() != 8)
        warnings.Append("Map Tile Dimensions are incorrect tiles need to be 8x8 pixels\n");
    if (map.GetWidth() > 128 || map.GetWidth() < 16 || map.GetHeight() > 128 || map.GetHeight() < 16)
        warnings.Append("Map Dimensions are incorrect map dimensions needs to be between 256x256 to 128x128");
    if (!warnings.IsEmpty())
        wxMessageBox(warnings, "Warning!", wxOK | wxCANCEL);

    Magick::Image tileset;
    if (HandlerUtils::LoadTileset(map, tileset))
        throw "Failed to load tileset";

    tileset.fx("round(round(r*255)*31/255)/255", Magick::RedChannel);
    tileset.fx("round(round(g*255)*31/255)/255", Magick::GreenChannel);
    tileset.fx("round(round(b*255)*31/255)/255", Magick::BlueChannel);
    tileset.classType(Magick::PseudoClass);
    tileset.type(Magick::PaletteType);
    tileset.syncPixels();

    // Get Palette
    tileset.getConstPixels(0, 0, tileset.columns(), tileset.rows());
    const Magick::IndexPacket* pixels = tileset.getConstIndexes();

    std::vector<Magick::Image> tiles;
    // Get Tiles
    if (HandlerUtils::GetTiles(map, tileset, tiles))
        throw "Failed to get tiles";

    WriteC(filename, map, pixels, tiles);
}

void GBAMapHandler::WriteC(const std::string& filename, Map& map, const Magick::IndexPacket* pixels, std::vector<Magick::Image> tiles)
{
}
