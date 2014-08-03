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

#include <iostream>
#include <fstream>
#include <algorithm>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include <wx/string.h>
#include "Scanner.hpp"
#include "TextMapHandler.hpp"
#include "TileBasedCollisionLayer.hpp"

TextMapHandler::TextMapHandler() : BaseMapHandler("Text Format", "txt", "Export the map as a text file")
{
    wxLogDebug("Init TextMapHandler");
}

TextMapHandler::~TextMapHandler()
{

}

void TextMapHandler::Load(const std::string& filename, Map& map)
{
    wxLogDebug("Loading %s", filename.c_str());
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
            throw "Unknown type found in file line: " + line;
    }
}

void TextMapHandler::ReadProperties(std::ifstream& file, Map& map)
{
    wxLogDebug("Reading Properties");
    std::string line;
    std::string name;
    std::string tileset;
    uint32_t tile_width = 8, tile_height = 8;

    std::getline(file, line);
    while (!line.empty())
    {
        wxLogDebug("Read line %s", line.c_str());
        std::string property;
        Scanner scanner(line);

        if (!scanner.Next(property))
            throw "Could not parse line: " + line;

        if (property == "name:")
        {
            if (!scanner.Next(name))
                throw "Could not parse name";
        }
        else if (property == "tileset:")
        {
            if (!scanner.Next(tileset))
                throw "Could not parse tileset";
        }
        else if (property == "tile_dimensions:")
        {
            if (!scanner.Next(tile_width))
                throw "Could not parse tile_width";
            if (!scanner.Next(tile_height))
                throw "Could not parse tile_height";
        }
        std::getline(file, line);
    }

    tile_width = std::min(MAX_TILE_SIZE, std::max(MIN_TILE_SIZE, tile_width));
    tile_height = std::min(MAX_TILE_SIZE, std::max(MIN_TILE_SIZE, tile_height));

    map.SetTileDimensions(tile_width, tile_height);
    map.SetName(name);
    map.SetFilename(tileset);
}

void TextMapHandler::ReadLayers(std::ifstream& file, Map& map)
{
    wxLogDebug("Reading Layers");
    std::string line;
    std::getline(file, line);
    while (!line.empty())
    {
        wxLogDebug("Reading a layer");
        wxLogDebug("Read line %s", line.c_str());
        std::string name;
        DrawAttributes attr;
        uint32_t width = 0, height = 0;
        std::vector<int32_t> data;

        while(!line.empty())
        {
            wxLogDebug("Read line %s", line.c_str());
            std::string property;
            Scanner scanner(line);

            if (!scanner.Next(property))
                throw "Could not parse line: " + line;

            if (property == "name:")
            {
                if (!scanner.Next(name))
                    throw "Could not parse name";
            }
            else if (property == "position:")
            {
                int32_t x, y;
                if (!scanner.Next(x))
                    throw "Could not parse position";
                if (!scanner.Next(y))
                    throw "Could not parse position";
                attr.SetPosition(x, y);
            }
            else if (property == "origin:")
            {
                int32_t x, y;
                if (!scanner.Next(x))
                    throw "Could not parse origin";
                if (!scanner.Next(y))
                    throw "Could not parse origin";
                attr.SetOrigin(x, y);
            }
            else if (property == "scale:")
            {
                float x, y;
                if (!scanner.Next(x))
                    throw "Could not parse scale";
                if (!scanner.Next(y))
                    throw "Could not parse scale";
                attr.SetScale(x, y);
            }
            else if (property == "rotation:")
            {
                float rotation;
                if (!scanner.Next(rotation))
                    throw "Could not parse rotation";
                attr.SetRotation(rotation);
            }
            else if (property == "opacity:")
            {
                float opacity;
                if (!scanner.Next(opacity))
                    throw "Could not parse opacity";
                attr.SetOpacity(opacity);
            }
            else if (property == "blend_mode:")
            {
                uint32_t mode;
                if (!scanner.Next(mode))
                    throw "Could not parse blend mode";
                attr.SetBlendMode(mode);
            }
            else if (property == "blend_color:")
            {
                uint32_t color;
                if (!scanner.Next(color, 16))
                    throw "Could not parse blend color";
                attr.SetBlendColor(color);
            }
            else if (property == "dimensions:")
            {
                if (!scanner.Next(width))
                    throw "Could not parse width";
                if (!scanner.Next(height))
                    throw "Could not parse height";
                data.reserve(width * height);
            }
            else if (property == "data:")
            {
                while (scanner.HasMoreTokens())
                {
                    int32_t element;
                    if (!scanner.Next(element))
                        throw "Could not parse data";
                    data.push_back(element);
                }
            }
            std::getline(file, line);
        }

        if (data.size() != width * height)
            throw "Incorrect number of tile entries for layer";

        Layer layer(name, width, height, data, attr);
        map.Add(layer);

        std::getline(file, line);
    }
}

void TextMapHandler::ReadBackgrounds(std::ifstream& file, Map& map)
{
    wxLogDebug("Reading Backgrounds");
    std::string line;
    std::getline(file, line);
    while (!line.empty())
    {
        wxLogDebug("Reading a background");
        wxLogDebug("Read line %s", line.c_str());
        std::string name;
        std::string filename;
        int32_t mode = 0;
        int32_t speedx = 0, speedy = 0;
        DrawAttributes attr;

        while(!line.empty())
        {
            wxLogDebug("Read line %s", line.c_str());
            std::string property;
            Scanner scanner(line);

            if (!scanner.Next(property))
                throw "Could not parse line: " + line;

            if (property == "name:")
            {
                if (!scanner.Next(name))
                    throw "Could not parse name";
            }
            else if (property == "filename:")
            {
                if (!scanner.Next(filename))
                    throw "Could not parse filename";
            }
            else if (property == "mode:")
            {
                if (!scanner.Next(mode))
                    throw "Could not parse mode";
            }
            else if (property == "speed:")
            {
                if (!scanner.Next(speedx))
                    throw "Could not parse speed";
                if (!scanner.Next(speedy))
                    throw "Could not parse speed";
            }
            else if (property == "position:")
            {
                int32_t x, y;
                if (!scanner.Next(x))
                    throw "Could not parse position";
                if (!scanner.Next(y))
                    throw "Could not parse position";
                attr.SetPosition(x, y);
            }
            else if (property == "origin:")
            {
                int32_t x, y;
                if (!scanner.Next(x))
                    throw "Could not parse origin";
                if (!scanner.Next(y))
                    throw "Could not parse origin";
                attr.SetOrigin(x, y);
            }
            else if (property == "scale:")
            {
                float x, y;
                if (!scanner.Next(x))
                    throw "Could not parse scale";
                if (!scanner.Next(y))
                    throw "Could not parse scale";
                attr.SetScale(x, y);
            }
            else if (property == "rotation:")
            {
                float rotation;
                if (!scanner.Next(rotation))
                    throw "Could not parse rotation";
                attr.SetRotation(rotation);
            }
            else if (property == "opacity:")
            {
                float opacity;
                if (!scanner.Next(opacity))
                    throw "Could not parse opacity";
                attr.SetOpacity(opacity);
            }
            else if (property == "blend_mode:")
            {
                uint32_t mode;
                if (!scanner.Next(mode))
                    throw "Could not parse blend mode";
                attr.SetBlendMode(mode);
            }
            else if (property == "blend_color:")
            {
                uint32_t color;
                if (!scanner.Next(color, 16))
                    throw "Could not parse blend color";
                attr.SetBlendColor(color);
            }
            std::getline(file, line);
        }

        Background back(name, filename, mode, speedx, speedy, attr);
        map.Add(back);

        std::getline(file, line);
    }
}

void TextMapHandler::ReadCollision(std::ifstream& file, Map& map)
{
    wxLogDebug("Reading Collision Layer");
    std::string line;
    std::getline(file, line);

    int32_t type = 0;
    uint32_t width = 0, height = 0;
    std::vector<int32_t> data;

    while(!line.empty())
    {
        wxLogDebug("Read line %s", line.c_str());
        std::string property;
        Scanner scanner(line);

        if (!scanner.Next(property))
            throw "Could not parse line: " + line;

        if (property == "type:")
        {
            if (!scanner.Next(type))
                throw "Could not parse type";
        }
        else if (property == "dimensions:")
        {
            if (!scanner.Next(width))
                throw "Could not parse width";
            if (!scanner.Next(height))
                throw "Could not parse height";
            data.reserve(width * height);
        }
        else if (property == "data:")
        {
            while (scanner.HasMoreTokens())
            {
                int32_t element;
                if (!scanner.Next(element))
                    throw "Could not parse data";
                data.push_back(element);
            }
        }
        std::getline(file, line);
    }

    if (data.size() != width * height)
        throw "Incorrect number of tile entries for collision layer";

    CollisionLayer* layer = new TileBasedCollisionLayer(width, height, data);
    map.SetCollisionLayer(layer);

    std::getline(file, line);
}

void TextMapHandler::Save(const std::string& filename, Map& map)
{
    // Checking to see if the file can be saved to.
    std::ofstream file(filename.c_str());
    if (!file.good())
        throw "Could not open file for writing";

    // Takes the map dimensions, tile dimensions, name, filename, and layer count and writes it to the file.
    file << "Properties\n";
    file << "name: " << map.GetName() << "\n";
    file << "tileset: " << map.GetFilename() << "\n";
    file << "tile_dimensions: " << map.GetTileWidth() << " " << map.GetTileHeight() << "\n\n";

    // Sets all the layer data for each layer.
    file << "Layers\n";
    for (unsigned int k = 0; k < map.GetNumLayers(); k++)
    {
        Layer& layer = map.GetLayer(k);
        file << "name: " << layer.GetName() << "\n";

        int32_t x, y;
        layer.GetPosition(x, y);
        file << "position: " << x << " " << y << "\n";

        int32_t ox, oy;
        layer.GetOrigin(ox, oy);
        file << "origin: " << ox << " " << oy << "\n";

        float sx, sy;
        layer.GetScale(sx, sy);
        file << "scale: " << sx << " " << sy << "\n";

        file << "rotation: " << layer.GetRotation() << "\n";
        file << "opacity: " << layer.GetOpacity() << "\n";
        file << "blend_mode: " << layer.GetBlendMode() << "\n";
        file << "blend_color: " << layer.GetBlendColor() << "\n";

        file << "dimensions: " << layer.GetWidth() << " " << layer.GetHeight() << "\n";
        for (unsigned int i = 0; i < layer.GetHeight(); i++)
        {
            file << "data: ";
            for (unsigned int j = 0; j < layer.GetWidth(); j++)
            {
                file << layer[i * layer.GetWidth() + j] << " ";
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
        int32_t spx, spy;
        background.GetSpeed(spx, spy);
        file << "name: " << background.GetName() << "\n";
        file << "filename: " << background.GetFilename() << "\n";
        file << "mode: " << background.GetMode() << "\n";
        file << "speed: " << spx << " " << spy << "\n";

        int32_t x, y;
        background.GetPosition(x, y);
        file << "position: " << x << " " << y << "\n";

        int32_t ox, oy;
        background.GetOrigin(ox, oy);
        file << "origin: " << ox << " " << oy << "\n";

        float sx, sy;
        background.GetScale(sx, sy);
        file << "scale: " << sx << " " << sy << "\n";

        file << "rotation: " << background.GetRotation() << "\n";
        file << "opacity: " << background.GetOpacity() << "\n";
        file << "blend_mode: " << background.GetBlendMode() << "\n";
        file << "blend_color: " << std::hex << background.GetBlendColor() << std::dec << "\n\n";
    }

    if (map.HasCollisionLayer())
    {
        file << "Collision\n";
        TileBasedCollisionLayer* collLayer = dynamic_cast<TileBasedCollisionLayer*>(map.GetCollisionLayer());
        std::vector<int32_t>& collMap = collLayer->GetData();
        file << "type: " << collLayer->GetType() << "\n";
        file << "dimensions: " << collLayer->GetWidth() << " " << collLayer->GetHeight() << "\n";
        for (unsigned int i = 0; i < collLayer->GetHeight(); i++)
        {
            file << "data: ";
            for (unsigned int j = 0; j < collLayer->GetWidth(); j++)
            {
                int32_t tile = collMap[i * collLayer->GetWidth() + j];
                file << tile << " ";
            }
            file << "\n";
        }
        file << "\n";
    }

    file.close();
}
