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

extern const std::string& RESOURCE_POOL;

/** GBAMapHandler
  *
  * Constructor
  */
GBAMapHandler::GBAMapHandler() : BaseMapHandler("GBA Map Export", "c", "Exports the map in GBA modes (0-2) format")
{
    // Not implemented so disable
    readable = false;
    writeable = false;
}

/** ~GBAMapHandler
  *
  * Destructor
  */
GBAMapHandler::~GBAMapHandler()
{

}

/** load
  *
  * Not supported
  */
int GBAMapHandler::load(const std::string& filename, Map& map)
{
    return -1;
}

/** save
  *
  * Saves map to GBA mode (0-2) format
  */
int GBAMapHandler::save(const std::string& filename, Map& map)
{
    wxString warnings = "";
    if (map.getTileWidth() != 8 || map.getTileHeight() != 8)
        warnings.Append("Map Tile Dimensions are incorrect tiles need to be 8x8 pixels\n");
    if (map.getWidth() > 128 || map.getWidth() < 16 || map.getHeight() > 128 || map.getHeight() < 16)
        warnings.Append("Map Dimensions are incorrect map dimensions needs to be between 256x256 to 128x128");
    if (!warnings.IsEmpty())
        wxMessageBox(warnings, "Warning!", wxOK | wxCANCEL);

    Magick::Image tileset;
    if (HandlerUtils::loadTileset(map, tileset))
        return -1;

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
    if (HandlerUtils::getTiles(map, tileset, tiles))
        return -1;

    writeC(filename, map, pixels, tiles);

    return 0;
}

/** WriteC
  *
  * Writes Map out to file
  */
int GBAMapHandler::writeC(const std::string& filename, Map& map, const Magick::IndexPacket* pixels, std::vector<Magick::Image> tiles)
{
//    ofstream file_c, file_h;
//    std::string name = filename.substr(0, filename.size() - 2);
//    std::string filename_c = name + ".c";
//    std::string filename_h = name + ".h";
//    name = name.substr(filename.rfind('/') + 1);
//    std::string name_cap = name;
//
//    transform(name_cap.begin(), name_cap.end(), name_cap.begin(), (int(*)(int)) std::toupper);
//    char buffer[7];
//    int spacecounter = 0;
//
//    file_c.open(filename_c.c_str());
//    file_h.open(filename_h.c_str());
//
//    if (!file_c.good() || !file_h.good())
//        return -1;
//
//    int num_pixels = image.rows() * image.columns();
//    int size = (num_pixels / 2) + ((num_pixels % 2) != 0);
//    int num_colors = image.colorMapSize();
//
//    file_c << "/*\n * Exported with TileMapEditor V1.0\n * Enjoy your day\n */\n";
//    file_c << "const unsigned short " << name << "_palette" << "[" << num_colors << "] =\n{\n\t";
//    for (int i = 0; i < num_colors; i++)
//    {
//        // PACKING
//        // pixels[i] = r pixels[i+1] = g pixels[i+2] = b
//        // Convert to gba format here
//        const Color& color = image.colorMap(i);
//        short byte = color.redQuantum() | (color.greenQuantum() << 5) | (color.blueQuantum() << 10);
//        sprintf(buffer, "0x%04x", byte);
//        file_c << buffer;
//        if (i != num_colors - 1)
//        {
//            file_c << ",";
//            spacecounter++;
//            if (spacecounter == 10)
//            {
//                file_c << "\n\t";
//                spacecounter = 0;
//            }
//        }
//    }
//    file_c << "\n};\n\n";
//
//    file_c << "const unsigned short " << name << "[" << size << "] =\n{\n\t";
//
//    spacecounter = 0;
//    for (int i = 0; i < size << 1; i += 2)
//    {
//        // PACKING
//        // pixels[i] = r pixels[i+1] = g pixels[i+2] = b
//        // Convert to gba format here
//        //short byte = pixels[i] | (pixels[i+1] << 5) | (pixels[i+2] << 10);
//        unsigned short byte = pixels[i] | (pixels[i+1] << 8);
//        sprintf(buffer, "0x%04x", byte);
//        file_c << buffer;
//        if (i != (size - 1) << 1)
//        {
//            file_c << ",";
//            spacecounter++;
//            if (spacecounter == 10)
//            {
//                file_c << "\n\t";
//                spacecounter = 0;
//            }
//        }
//    }
//    file_c << "\n};\n";
//
//    file_h << "#ifndef " << name_cap << "_BITMAP_H\n";
//    file_h << "#define " << name_cap << "_BITMAP_H\n\n";
//    file_h << "extern const unsigned short " << name << "[" << size << "];\n";
//    file_h << "extern const unsigned short " << name << "_palette[" << num_colors << "];\n";
//    file_h << "#define " << name_cap << "_WIDTH " << image.columns() << "\n";
//    file_h << "#define " << name_cap << "_HEIGHT " << image.rows() << "\n";
//    file_h << "#define " << name_cap << "_PALETTE_SIZE " << num_colors << "\n\n";
//    file_h << "#endif";
//
//    file_c.close();
//    file_h.close();
//    return 0;
    return 0;
}
