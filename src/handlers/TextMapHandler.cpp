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
#include <algorithm>
#include <wx/msgdlg.h>
#include <wx/string.h>
#include <wx/tokenzr.h>
#include "TextMapHandler.hpp"
#include "TileBasedCollisionLayer.hpp"

TextMapHandler::TextMapHandler() : BaseMapHandler("Text Format", "txt", "Export the map as a text file")
{
}

TextMapHandler::~TextMapHandler()
{

}

void TextMapHandler::Load(const std::string& filename, Map& map)
{
    std::ifstream file(filename.c_str());
    if (!file.good())
        throw "Could not open file for reading";

    std::string line;

    std::getline(file, line);

    if (line == "Properties")
        ReadProperties(file, map);
    else
        throw "Properties must come first in txt file";

    while (std::getline(file, line))
    {
        if (line == "Layers")
            ReadLayers(file, map);
        else if (line == "Backgrounds")
            ReadBackgrounds(file, map);
        else if (line == "Collision")
            ReadCollision(file, map);
        else if (!line.empty())
            throw "Unknown type found in file";
    }
}

void TextMapHandler::ReadProperties(std::ifstream& file, Map& map)
{
    std::string mapdim, tiledim, name, filename;
    int width, height, tile_width, tile_height;
    wxStringTokenizer scanner;

    std::getline(file, mapdim);
    std::getline(file, tiledim);
    std::getline(file, name);
    std::getline(file, filename);

    scanner.SetString(mapdim);
    width = std::min(1024, std::max(1, wxAtoi(scanner.GetNextToken())));
    height = std::min(1024, std::max(1, wxAtoi(scanner.GetNextToken())));

    scanner.SetString(tiledim);
    tile_width = std::min(1024, std::max(1, wxAtoi(scanner.GetNextToken())));
    tile_height = std::min(1024, std::max(1, wxAtoi(scanner.GetNextToken())));

    map.Resize(width, height);
    map.SetTileDimensions(tile_width, tile_height);
    map.SetName(name);
    map.SetFilename(filename);
}

void TextMapHandler::ReadLayers(std::ifstream& file, Map& map)
{
    while (1)
    {
        std::string name;
        std::string buffer;
        std::string line;

        std::getline(file, name);
        if (name.empty()) break;

        for (unsigned int i = 0; i < map.GetHeight(); i++)
        {
            std::string line;
            std::getline(file, line);
            buffer += line;
            buffer += "\n";
        }

        wxStringTokenizer scanner(buffer);

        std::vector<int32_t> data;
        data.resize(map.GetWidth() * map.GetHeight());
        unsigned int i = 0;
        while (scanner.HasMoreTokens())
        {
            wxString token = scanner.GetNextToken();
            int tileid = wxAtoi(token);
            if (i > map.GetWidth() * map.GetHeight()) break;
            data[i] = tileid;
            i++;
        }

        if (i != map.GetWidth() * map.GetHeight())
            throw "Incorrect number of tile entries for layer";

        Layer layer(name, map.GetWidth(), map.GetHeight(), data);
        map.Add(layer);

        std::getline(file, line);
    }
}

void TextMapHandler::ReadBackgrounds(std::ifstream& file, Map& map)
{
    while (1)
    {
        std::string name;
        std::string filename;
        std::string type;
        std::string speeds;
        int mode;
        float speedx, speedy;

        std::getline(file, name);
        if (name.empty()) break;

        std::getline(file, filename);
        std::getline(file, type);
        std::getline(file, speeds);

        mode = wxAtoi(type);

        wxStringTokenizer scanner(speeds);
        speedx = wxAtof(scanner.GetNextToken());
        speedy = wxAtof(scanner.GetNextToken());

        Background back(name, filename, mode, speedx, speedy);
        map.Add(back);
    }
}

void TextMapHandler::ReadCollision(std::ifstream& file, Map& map)
{
    std::string type;
    std::string buffer;
    Collision::Type ctype;

    std::getline(file, type);

    ctype = (Collision::Type) wxAtoi(type);

    for (unsigned int i = 0; i < map.GetHeight(); i++)
    {
        std::string line;
        std::getline(file, line);
        buffer += line;
        buffer += "\n";
    }

    wxStringTokenizer scanner(buffer);

    /// TODO this is correct for now
    // Later when PixelBased and others are implemented change this.
    std::vector<int32_t> data;
    data.resize(map.GetWidth() * map.GetHeight());
    unsigned int i = 0;
    while (scanner.HasMoreTokens())
    {
        wxString token = scanner.GetNextToken();
        int tileid = wxAtoi(token);
        if (i > map.GetWidth() * map.GetHeight()) break;
        data[i] = tileid;
        i++;
    }

    if (i != map.GetWidth() * map.GetHeight())
        throw "Incorrect number of tile entries for layer";

    CollisionLayer* layer = new TileBasedCollisionLayer(map.GetWidth(), map.GetHeight(), data);

    map.SetCollisionLayer(layer);
}

void TextMapHandler::Save(const std::string& filename, Map& map)
{
    // Checking to see if the file can be saved to.
    std::ofstream file(filename.c_str());
    if (!file.good())
        throw "Could not open file for writing";

    // Takes the map dimensions, tile dimensions, name, filename, and layer count and writes it to the file.
    file << "Properties\n";
    file << map.GetWidth() << " " << map.GetHeight() << "\n";
    file << map.GetTileWidth() << " " << map.GetTileHeight() << "\n";
    file << map.GetName() << "\n";
    file << map.GetFilename() << "\n\n";

    // Sets all the layer data for each layer.
    file << "Layers\n";
    for (unsigned int k = 0; k < map.GetNumLayers(); k++)
    {
        Layer& layer = map.GetLayer(k);
        file << layer.GetName() << "\n";
        for (unsigned int i = 0; i < map.GetHeight(); i++)
        {
            file << "\t";
            for (unsigned int j = 0; j < map.GetWidth(); j++)
            {
                int tile = layer[i * map.GetWidth() + j];
                file << tile << " ";
            }
            file << "\n";
        }
        file << "\n";
    }

    // Sets all the background data for each background.
    if (map.GetNumBackgrounds() > 0) file << "Backgrounds\n";
    for (unsigned int k = 0; k < map.GetNumBackgrounds(); k++)
    {
        Background& background = map.GetBackground(k);
        float x, y;
        background.GetSpeed(x, y);
        file << background.GetName() << "\n";
        file << background.GetFilename() << "\n";
        file << background.GetMode() << "\n";
        file << x << " " << y << "\n\n";
    }

    // If the map has a collision layer, it will be written to the file as well.
    if (map.HasCollisionLayer())
    {
        file << "Collision\n";
        TileBasedCollisionLayer* collLayer = dynamic_cast<TileBasedCollisionLayer*>(map.GetCollisionLayer());
        std::vector<int32_t>& collMap = collLayer->GetData();
        file << collLayer->GetType() << "\n";
        for (unsigned int i = 0; i < map.GetHeight(); i++)
        {
            file << "\t";
            for (unsigned int j = 0; j < map.GetWidth(); j++)
            {
                int tile = collMap[i * map.GetWidth() + j];
                file << tile << " ";
            }
            file << "\n";
        }
        file << "\n";
    }

    file.close();
}
