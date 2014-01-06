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
#include <wx/arrstr.h>
#include <wx/choicdlg.h>
#include "GBAImageHandler.hpp"
#include "HandlerUtils.hpp"

/// TODO Rewrite this class hint brandontools

GBAImageHandler::GBAImageHandler() : BaseMapHandler("GBA Image Export", "c", "Exports the map's image in GBA format modes (3-5)", false)
{
}

GBAImageHandler::~GBAImageHandler()
{
}

void GBAImageHandler::save(const std::string& filename, Map& map)
{
    int mode;
    wxArrayString choices;
    choices.Add("Mode 3");
    choices.Add("Mode 4");
    mode = wxGetSingleChoiceIndex("Which mode do you want the image to be usable for?", "GBA Image Export", choices, 0);

    if (mode == -1)
        throw "No GBA Mode Selected";

    mode += 3;


    Magick::Image image;

    if (HandlerUtils::mapToImage(map, image))
        throw "Could not convert map to image";

    // GBA colors are 16bit ubbbbbgggggrrrrr
    // u = unused
    image.fx("round(round(b*255)*31/255)/255", Magick::RedChannel);
    image.fx("round(round(g*255)*31/255)/255", Magick::GreenChannel);
    image.fx("round(round(r*255)*31/255)/255", Magick::BlueChannel);
    // If we are in mode 4 mode then we need to generate a palette
    if (mode == 4)
    {
        image.classType(Magick::PseudoClass);
        image.type(Magick::PaletteType);
        image.syncPixels();
    }

    if (mode != 4)
        writeCMode3(filename, image);
    else
        writeCMode4(filename, image);
}

void GBAImageHandler::writeCMode3(const std::string& filename, Magick::Image& image)
{
    /// TODO update this code.
    std::ofstream file_c, file_h;

    std::string name = filename.substr(0, filename.size() - 2);
    std::string filename_c = name + ".c";
    std::string filename_h = name + ".h";
    name = name.substr(filename.rfind('/') + 1);
    std::string name_cap = name;

    transform(name_cap.begin(), name_cap.end(), name_cap.begin(), (int(*)(int)) std::toupper);

    file_c.open(filename_c.c_str());
    file_h.open(filename_h.c_str());

    if (!file_c.good() || !file_h.good())
        throw "Could not open files";

    int num_pixels = image.rows() * image.columns();

    file_c << "/*\n * Exported with TileMapEditor V1.0\n * Enjoy your day\n */\n";
    file_c << "const unsigned short " << name << "[" << num_pixels << "] =\n{\n\t";

    char* pixels = new char[num_pixels * sizeof(char) * 3];
    image.write(0, 0, image.columns(), image.rows(), "RGB", Magick::CharPixel, pixels);
    int size = 3 * num_pixels;
    char buffer[7];
    int spacecounter = 0;
    for (int i = 0; i < size; i += 3)
    {
        // PACKING
        // pixels[i] = r pixels[i+1] = g pixels[i+2] = b
        // Convert to gba format here
        unsigned short byte = pixels[i] | (pixels[i+1] << 5) | (pixels[i+2] << 10);
        sprintf(buffer, "0x%04x", byte);
        file_c << buffer;
        if (i != size - 3)
        {
            file_c << ",";
            spacecounter++;
            if (spacecounter == 10)
            {
                file_c << "\n\t";
                spacecounter = 0;
            }
        }
    }
    file_c << "\n};\n";

    file_h << "#ifndef " << name_cap << "_BITMAP_H\n";
    file_h << "#define " << name_cap << "_BITMAP_H\n\n";
    file_h << "extern const unsigned short " << name << "[" << num_pixels << "];\n";
    file_h << "#define " << name_cap << "_WIDTH " << image.columns() << "\n";
    file_h << "#define " << name_cap << "_HEIGHT " << image.rows() << "\n\n";
    file_h << "#endif";

    file_c.close();
    file_h.close();

    delete[] pixels;
}

void GBAImageHandler::writeCMode4(const std::string& filename, Magick::Image& image)
{
    /// TODO update this too.
    std::ofstream file_c, file_h;
    std::string name = filename.substr(0, filename.size() - 2);
    std::string filename_c = name + ".c";
    std::string filename_h = name + ".h";
    name = name.substr(filename.rfind('/') + 1);
    std::string name_cap = name;

    transform(name_cap.begin(), name_cap.end(), name_cap.begin(), (int(*)(int)) std::toupper);
    char buffer[7];
    int spacecounter = 0;

    file_c.open(filename_c.c_str());
    file_h.open(filename_h.c_str());

    if (!file_c.good() || !file_h.good())
        throw "Could not open files for writing";

    int num_pixels = image.rows() * image.columns();
    int size = (num_pixels / 2) + ((num_pixels % 2) != 0);
    int num_colors = image.colorMapSize();

    file_c << "/*\n * Exported with TileMapEditor V1.0\n * Enjoy your day\n */\n";
    file_c << "const unsigned short " << name << "_palette" << "[" << num_colors << "] =\n{\n\t";
    for (int i = 0; i < num_colors; i++)
    {
        // PACKING
        // pixels[i] = r pixels[i+1] = g pixels[i+2] = b
        // Convert to gba format here
        const Magick::Color& color = image.colorMap(i);
        short byte = color.redQuantum() | (color.greenQuantum() << 5) | (color.blueQuantum() << 10);
        sprintf(buffer, "0x%04x", byte);
        file_c << buffer;
        if (i != num_colors - 1)
        {
            file_c << ",";
            spacecounter++;
            if (spacecounter == 10)
            {
                file_c << "\n\t";
                spacecounter = 0;
            }
        }
    }
    file_c << "\n};\n\n";

    file_c << "const unsigned short " << name << "[" << size << "] =\n{\n\t";


    image.getConstPixels(0, 0, image.columns(), image.rows());
    const Magick::IndexPacket* pixels = image.getConstIndexes();

    spacecounter = 0;
    for (int i = 0; i < size << 1; i += 2)
    {
        // PACKING
        // pixels[i] = r pixels[i+1] = g pixels[i+2] = b
        // Convert to gba format here
        //short byte = pixels[i] | (pixels[i+1] << 5) | (pixels[i+2] << 10);
        unsigned short byte = pixels[i] | (pixels[i+1] << 8);
        sprintf(buffer, "0x%04x", byte);
        file_c << buffer;
        if (i != (size - 1) << 1)
        {
            file_c << ",";
            spacecounter++;
            if (spacecounter == 10)
            {
                file_c << "\n\t";
                spacecounter = 0;
            }
        }
    }
    file_c << "\n};\n";

    file_h << "#ifndef " << name_cap << "_BITMAP_H\n";
    file_h << "#define " << name_cap << "_BITMAP_H\n\n";
    file_h << "extern const unsigned short " << name << "[" << size << "];\n";
    file_h << "extern const unsigned short " << name << "_palette[" << num_colors << "];\n";
    file_h << "#define " << name_cap << "_WIDTH " << image.columns() << "\n";
    file_h << "#define " << name_cap << "_HEIGHT " << image.rows() << "\n";
    file_h << "#define " << name_cap << "_PALETTE_SIZE " << num_colors << "\n\n";
    file_h << "#endif";

    file_c.close();
    file_h.close();
}
