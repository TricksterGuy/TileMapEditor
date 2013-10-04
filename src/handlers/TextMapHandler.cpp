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

int TextMapHandler::load(const std::string& filename, Map& map)
{
    std::ifstream file(filename.c_str());
    if (!file.good()) return -1;

    std::string line;

    std::getline(file, line);

    if (line == "Properties")
    {
        if (readProperties(file, map)) return -1;
    }
    else
    {
        return -1;
    }

    while (std::getline(file, line))
    {
        if (line == "Layers")
        {
            if (readLayers(file, map)) return -1;
        }
        else if (line == "Backgrounds")
        {
            if (readBackgrounds(file, map)) return -1;
        }
        else if (line == "Collision")
        {
            if (readCollision(file, map)) return -1;
        }
        else if (!line.empty())
        {
            return -1;
        }
    }

    return 0;
}

int TextMapHandler::readProperties(std::ifstream& file, Map& map)
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

    map.resize(width, height);
    map.setTileDimensions(tile_width, tile_height);
    map.setName(name);
    map.setFilename(filename);

    return 0;
}

int TextMapHandler::readLayers(std::ifstream& file, Map& map)
{
    while (1)
    {
        std::string name;
        std::string buffer;
        std::string line;

        std::getline(file, name);
        if (name.empty()) break;

        for (unsigned int i = 0; i < map.getHeight(); i++)
        {
            std::string line;
            std::getline(file, line);
            buffer += line;
            buffer += "\n";
        }

        wxStringTokenizer scanner(buffer);

        std::vector<int32_t> data;
        data.resize(map.getWidth() * map.getHeight());
        unsigned int i = 0;
        while (scanner.HasMoreTokens())
        {
            wxString token = scanner.GetNextToken();
            int tileid = wxAtoi(token);
            if (i > map.getWidth() * map.getHeight()) break;
            data[i] = tileid;
            i++;
        }

        if (i != map.getWidth() * map.getHeight())
        {
            wxMessageBox("Not enough / Too Many tile entries for layer", "Error");
            return -1;
        }

        Layer layer(name, map.getWidth(), map.getHeight(), data);
        map.add(layer);

        std::getline(file, line);
    }

    return 0;
}

int TextMapHandler::readBackgrounds(std::ifstream& file, Map& map)
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
        map.add(back);
    }

    return 0;
}

int TextMapHandler::readCollision(std::ifstream& file, Map& map)
{
    std::string type;
    std::string buffer;
    Collision::Type ctype;

    std::getline(file, type);

    ctype = (Collision::Type) wxAtoi(type);

    for (unsigned int i = 0; i < map.getHeight(); i++)
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
    data.resize(map.getWidth() * map.getHeight());
    unsigned int i = 0;
    while (scanner.HasMoreTokens())
    {
        wxString token = scanner.GetNextToken();
        int tileid = wxAtoi(token);
        if (i > map.getWidth() * map.getHeight()) break;
        data[i] = tileid;
        i++;
    }

    if (i != map.getWidth() * map.getHeight())
    {
        wxMessageBox("Not enough / Too Many tile entries for layer", "Error");
        return -1;
    }

    CollisionLayer* layer = new TileBasedCollisionLayer(map.getWidth(), map.getHeight(), data);

    map.setCollisionLayer(layer);

    return 0;
}

int TextMapHandler::save(const std::string& filename, Map& map)
{
    // Checking to see if the file can be saved to.
    std::ofstream file(filename.c_str());
    if (!file.good()) return -1;

    // Takes the map dimensions, tile dimensions, name, filename, and layer count and writes it to the file.
    file << "Properties\n";
    file << map.getWidth() << " " << map.getHeight() << "\n";
    file << map.getTileWidth() << " " << map.getTileHeight() << "\n";
    file << map.getName() << "\n";
    file << map.getFilename() << "\n\n";

    // Sets all the layer data for each layer.
    file << "Layers\n";
    for (unsigned int k = 0; k < map.getNumLayers(); k++)
    {
        Layer& layer = map.getLayer(k);
        file << layer.getName() << "\n";
        for (unsigned int i = 0; i < map.getHeight(); i++)
        {
            file << "\t";
            for (unsigned int j = 0; j < map.getWidth(); j++)
            {
                int tile = layer[i * map.getWidth() + j];
                file << tile << " ";
            }
            file << "\n";
        }
        file << "\n";
    }

    // Sets all the background data for each background.
    if (map.getNumBackgrounds() > 0) file << "Backgrounds\n";
    for (unsigned int k = 0; k < map.getNumBackgrounds(); k++)
    {
        Background& background = map.getBackground(k);
        float x, y;
        background.getSpeed(x, y);
        file << background.getName() << "\n";
        file << background.getFilename() << "\n";
        file << background.getMode() << "\n";
        file << x << " " << y << "\n\n";
    }

    // If the map has a collision layer, it will be written to the file as well.
    if (map.hasCollisionLayer())
    {
        file << "Collision\n";
        TileBasedCollisionLayer* collLayer = dynamic_cast<TileBasedCollisionLayer*>(map.getCollisionLayer());
        std::vector<int32_t>& collMap = collLayer->getData();
        file << collLayer->getType() << "\n";
        for (unsigned int i = 0; i < map.getHeight(); i++)
        {
            file << "\t";
            for (unsigned int j = 0; j < map.getWidth(); j++)
            {
                int tile = collMap[i * map.getWidth() + j];
                file << tile << " ";
            }
            file << "\n";
        }
        file << "\n";
    }

    file.close();

    return 0;
}
