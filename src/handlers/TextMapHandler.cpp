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
#include <wx/msgdlg.h>
#include <wx/string.h>
#include "Scanner.hpp"
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
            throw "Unknown type found in file line: " + line;
    }
}

void TextMapHandler::ReadProperties(std::ifstream& file, Map& map)
{
    std::string line;
    std::string name;
    std::string tileset;
    uint32_t tile_width, tile_height;

    std::getline(file, line);
    while (!line.empty())
    {
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
    map.SetFilename(filename);
}

void TextMapHandler::ReadLayers(std::ifstream& file, Map& map)
{
    std::getline(file, line);
    while (!line.empty())
    {
        std::string name;
        DrawAttributes attr;
        uint32_t width = 0, height = 0;
        std::vector<int32_t> data;

        std::getline(file, line);
        while(!line.empty())
        {
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
            else if (property == "scale:")
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
            }
        }
    }

    while (1)
    {
        std::string name;
        std::string line;
        std::string layerdim;

        std::getline(file, name);
        if (name.empty()) break;

        DrawAttributes attr;
        ReadAttributes(file, attr);

        wxStringTokenizer scanner;
        std::getline(file, layerdim);

        scanner.SetString(layerdim);
        uint32_t width = std::min(1024, std::max(1, wxAtoi(scanner.GetNextToken())));
        uint32_t height = std::min(1024, std::max(1, wxAtoi(scanner.GetNextToken())));
        std::vector<int32_t> data;
        data.reserve(width * height);

        for (unsigned int i = 0; i < height; i++)
        {
            std::string line;
            std::getline(file, line);
            printf("line: %s\n", line.c_str());
            if (line.empty()) break;
            scanner.SetString(line);
            while (scanner.HasMoreTokens())
            {
                wxString token = scanner.GetNextToken();
                int tileid = wxAtoi(token);
                data.push_back(tileid);
            }
        }

        printf("%d\n", data.size());
        if (data.size() != width * height)
            throw "Incorrect number of tile entries for layer";

        Layer layer(name, width, height, data, attr);
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

        DrawAttributes attr;
        ReadAttributes(file, attr);

        Background back(name, filename, mode, speedx, speedy, attr);
        map.Add(back);
    }
}

void TextMapHandler::ReadCollision(std::ifstream& file, Map& map)
{
    std::string type;
    std::getline(file, type);
    Collision::Type ctype = (Collision::Type) wxAtoi(type);

    /// TODO this is correct for now
    // Later when PixelBased and others are implemented change this.
    std::string layerdim;
    std::getline(file, layerdim);
    wxStringTokenizer scanner(layerdim);

    uint32_t width = std::min(1024, std::max(1, wxAtoi(scanner.GetNextToken())));
    uint32_t height = std::min(1024, std::max(1, wxAtoi(scanner.GetNextToken())));
    std::vector<int32_t> data;
    data.reserve(width * height);

    for (unsigned int i = 0; i < height; i++)
    {
        std::string line;
        std::getline(file, line);
        if (line.empty()) break;
        scanner.SetString(line);
        while (scanner.HasMoreTokens())
        {
            wxString token = scanner.GetNextToken();
            int tileid = wxAtoi(token);
            data.push_back(tileid);
        }
    }

    printf("%d\n", data.size());
    if (data.size() != width * height)
        throw "Incorrect number of tile entries for collision layer";

    CollisionLayer* layer = new TileBasedCollisionLayer(width, height, data);

    map.SetCollisionLayer(layer);
}

void TextMapHandler::ReadAttributes(std::ifstream& file, DrawAttributes& attr)
{
    std::string pos;
    std::string origin;
    std::string scale;
    std::string rotation;
    std::string opacity;
    std::string blend_mode;
    std::string blend_color;

    std::getline(file, pos);
    std::getline(file, origin);
    std::getline(file, scale);
    std::getline(file, rotation);
    std::getline(file, opacity);
    std::getline(file, blend_mode);
    std::getline(file, blend_color);

    wxStringTokenizer posScan(pos);
    wxStringTokenizer originScan(origin);
    wxStringTokenizer scaleScan(scale);

    attr.SetPosition(wxAtoi(posScan.GetNextToken()), wxAtoi(posScan.GetNextToken()));
    attr.SetOrigin(wxAtoi(originScan.GetNextToken()), wxAtoi(originScan.GetNextToken()));
    attr.SetScale(wxAtof(scaleScan.GetNextToken()), wxAtof(scaleScan.GetNextToken()));
    attr.SetRotation(wxAtof(rotation));
    attr.SetOpacity(wxAtof(opacity));
    attr.SetBlendMode(wxAtoi(blend_mode));
    unsigned long color;
    if (!wxString(blend_color).ToULong(&color, 16))
        throw "Failed to parse blend color";
    attr.SetBlendColor(color);
}

void TextMapHandler::Save(const std::string& filename, Map& map)
{
    // Checking to see if the file can be saved to.
    std::ofstream file(filename.c_str());
    if (!file.good())
        throw "Could not open file for writing";

    // Takes the map dimensions, tile dimensions, name, filename, and layer count and writes it to the file.
    file << "Properties\n";
    file << map.GetTileWidth() << " " << map.GetTileHeight() << "\n";
    file << map.GetName() << "\n";
    file << map.GetFilename() << "\n\n";

    // Sets all the layer data for each layer.
    file << "Layers\n";
    for (unsigned int k = 0; k < map.GetNumLayers(); k++)
    {
        Layer& layer = map.GetLayer(k);
        file << layer.GetName() << "\n";

        int32_t x, y;
        layer.GetPosition(x, y);
        file << x << " " << y << "\n";

        int32_t ox, oy;
        layer.GetOrigin(ox, oy);
        file << ox << " " << oy << "\n";

        float sx, sy;
        layer.GetScale(sx, sy);
        file << sx << " " << sy << "\n";

        file << layer.GetRotation() << "\n";
        file << layer.GetOpacity() << "\n";
        file << layer.GetBlendMode() << "\n";
        file << layer.GetBlendColor() << "\n";

        file << layer.GetWidth() << " " << layer.GetHeight() << "\n";
        for (unsigned int i = 0; i < layer.GetHeight(); i++)
        {
            file << "\t";
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
        file << background.GetName() << "\n";
        file << background.GetFilename() << "\n";
        file << background.GetMode() << "\n";
        file << spx << " " << spy << "\n";

        int32_t x, y;
        background.GetPosition(x, y);
        file << x << " " << y << "\n";

        int32_t ox, oy;
        background.GetOrigin(ox, oy);
        file << ox << " " << oy << "\n";

        float sx, sy;
        background.GetScale(sx, sy);
        file << sx << " " << sy << "\n";

        file << background.GetRotation() << "\n";
        file << background.GetOpacity() << "\n";
        file << background.GetBlendMode() << "\n";
        file << background.GetBlendColor() << "\n\n";
    }

    if (map.HasCollisionLayer())
    {
        file << "Collision\n";
        TileBasedCollisionLayer* collLayer = dynamic_cast<TileBasedCollisionLayer*>(map.GetCollisionLayer());
        std::vector<int32_t>& collMap = collLayer->GetData();
        file << collLayer->GetType() << "\n";
        file << collLayer->GetWidth() << " " << collLayer->GetHeight() << "\n";
        for (unsigned int i = 0; i < collLayer->GetHeight(); i++)
        {
            file << "\t";
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
