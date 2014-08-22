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

#include <fstream>
#include <algorithm>
#include <wx/msgdlg.h>
#include <wx/string.h>
#include "Scanner.hpp"
#include "TextMapHandler.hpp"
#include "TileBasedCollisionLayer.hpp"
#include "Logger.hpp"


TextMapHandler::TextMapHandler() : BaseMapHandler("Text Format", "txt", "Export the map as a text file")
{
}

TextMapHandler::~TextMapHandler()
{

}

void TextMapHandler::Load(std::istream& file, Map& map)
{
    std::string line;
    std::getline(file, line);

    DebugLog("%s Read line %s", __func__, line.c_str());
    if (line == "Properties")
        ReadProperties(file, map);
    else
        throw "Properties must come first in txt file";

    while (std::getline(file, line))
    {
        DebugLog("%s Read line %s", __func__, line.c_str());
        if (line == "Layers")
            ReadLayers(file, map);
        else if (line == "Backgrounds")
            ReadBackgrounds(file, map);
        else if (line == "Collision")
            ReadCollision(file, map);
        else if (line == "Animations")
            ReadAnimations(file, map);
        else if (!line.empty())
            throw "Unknown type found in file line: " + line;
    }
    DebugLog("Done Loading");
}

void TextMapHandler::ReadProperties(std::istream& file, Map& map)
{
    DebugLog("Reading Properties");
    std::string line;
    std::string name;
    std::string tileset;
    uint32_t tile_width = 8, tile_height = 8;

    std::getline(file, line);
    while (!line.empty())
    {
        DebugLog("%s Read line %s", __func__, line.c_str());
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
        else
        {
            throw "Unexpected token " + property;
        }
        std::getline(file, line);
    }

    tile_width = std::min(MAX_TILE_SIZE, std::max(MIN_TILE_SIZE, tile_width));
    tile_height = std::min(MAX_TILE_SIZE, std::max(MIN_TILE_SIZE, tile_height));

    map.SetTileDimensions(tile_width, tile_height);
    map.SetName(name);
    map.SetFilename(tileset);
    DebugLog("Done Reading Properties");
}

void TextMapHandler::ReadLayers(std::istream& file, Map& map)
{
    DebugLog("Reading Layers");
    std::string line;
    std::getline(file, line);
    while (!line.empty())
    {
        DebugLog("Reading a layer");
        std::string name;
        DrawAttributes attr;
        uint32_t width = 0, height = 0;
        std::vector<int32_t> data;

        while(!line.empty())
        {
            DebugLog("%s Read line %s", __func__, line.c_str());
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
            else if (property == "priority:")
            {
                int32_t priority;
                if (!scanner.Next(priority))
                    throw "Could not parser priority";
                attr.SetDepth(priority);
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
            else
            {
                throw "Unexpected token " + property;
            }
            std::getline(file, line);
        }

        if (data.size() != width * height)
            throw "Incorrect number of tile entries for layer";

        Layer layer(name, width, height, data, attr);
        map.Add(layer);

        std::getline(file, line);
    }
    DebugLog("Done Reading Layers");
}

void TextMapHandler::ReadBackgrounds(std::istream& file, Map& map)
{
    DebugLog("Reading Backgrounds");
    std::string line;
    std::getline(file, line);
    while (!line.empty())
    {
        DebugLog("Reading a background");
        std::string name;
        std::string filename;
        int32_t mode = 0;
        int32_t speedx = 0, speedy = 0;
        DrawAttributes attr;

        while(!line.empty())
        {
            DebugLog("%s Read line %s", __func__, line.c_str());
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
            else if (property == "priority:")
            {
                int32_t priority;
                if (!scanner.Next(priority))
                    throw "Could not parser priority";
                attr.SetDepth(priority);
            }
            else
            {
                throw "Unexpected token " + property;
            }
            std::getline(file, line);
        }

        Background back(name, filename, mode, speedx, speedy, attr);
        map.Add(back);

        std::getline(file, line);
    }
    DebugLog("Done Reading Backgrounds");
}

void TextMapHandler::ReadAnimations(std::istream& file, Map& map)
{
    DebugLog("Reading Animations");
    std::string line;
    std::getline(file, line);
    while (!line.empty())
    {
        DebugLog("Reading an Animation");
		std::string name;
		int32_t delay;
		int32_t type;
		int32_t times;
		std::vector<int32_t> frames;

        while(!line.empty())
        {
            DebugLog("%s Read line %s", __func__, line.c_str());
            std::string property;
            Scanner scanner(line);

            if (!scanner.Next(property))
                throw "Could not parse line: " + line;

            if (property == "name:")
            {
                if (!scanner.Next(name))
                    throw "Could not parse name";
            }
            else if (property == "delay:")
            {
                if (!scanner.Next(delay))
                    throw "Could not parse delay";
            }
            else if (property == "type:")
            {
                if (!scanner.Next(type))
                    throw "Could not parse type";
            }
            else if (property == "times:")
            {
                if (!scanner.Next(times))
                    throw "Could not parse times";
            }
            else if (property == "frames:")
            {
                while (scanner.HasMoreTokens())
                {
                    int32_t element;
                    if (!scanner.Next(element))
                        throw "Could not parse frames";
                    frames.push_back(element);
                }
            }
            else
            {
                throw "Unexpected token " + property;
            }
            std::getline(file, line);
        }
        AnimatedTile tile(name, delay, (Animation::Type)type, times, frames);
        map.Add(tile);

        std::getline(file, line);
    }
    DebugLog("Done Reading Animations");
}

void TextMapHandler::ReadCollision(std::istream& file, Map& map)
{
    DebugLog("Reading Collision Layer");

    int32_t type = 0;
    uint32_t width = 0, height = 0;
    std::vector<int32_t> data;

    std::string line;
    std::getline(file, line);

    while(!line.empty())
    {
        DebugLog("%s Read line %s", __func__, line.c_str());
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
        else
        {
            throw "Unexpected token " + property;
        }
        std::getline(file, line);
    }

    if (data.size() != width * height)
        throw "Incorrect number of tile entries for collision layer";

    CollisionLayer* layer = new TileBasedCollisionLayer(width, height, data);
    map.SetCollisionLayer(layer);

    std::getline(file, line);
    DebugLog("Done Reading Collision Layer");
}

void TextMapHandler::Save(std::ostream& file, const Map& map)
{
    // Takes the map dimensions, tile dimensions, name, filename, and layer count and writes it to the file.
    file << "Properties\n";
    file << "name: " << map.GetName() << "\n";
    file << "tileset: " << map.GetFilename() << "\n";
    file << "tile_dimensions: " << map.GetTileWidth() << " " << map.GetTileHeight() << "\n\n";

    // Sets all the layer data for each layer.
    file << "Layers\n";
    for (unsigned int k = 0; k < map.GetNumLayers(); k++)
    {
        const Layer& layer = map.GetLayer(k);
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
        file << "blend_color: " << std::uppercase << std::hex << layer.GetBlendColor() << std::dec << std::nouppercase << "\n";
        file << "priority: " << layer.GetDepth() << "\n";

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
    file << "\n";
    // Sets all the background data for each background.
    if (map.GetNumBackgrounds() > 0) file << "Backgrounds\n";
    for (unsigned int k = 0; k < map.GetNumBackgrounds(); k++)
    {
        const Background& background = map.GetBackground(k);
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
        file << "blend_color: " << std::uppercase << std::hex << background.GetBlendColor() << std::dec << std::nouppercase << "\n";
        file << "priority: " << background.GetDepth() << "\n\n";
    }
    file << "\n";

    if (map.GetNumAnimatedTiles() > 0) file << "Animations\n";
    for (unsigned int k = 0; k < map.GetNumAnimatedTiles(); k++)
    {
        const AnimatedTile& tile = map.GetAnimatedTile(k);
        const std::vector<int32_t>& frames = tile.GetFrames();
        file << "name: " << tile.GetName() << "\n";
        file << "delay: " << tile.GetDelay() << "\n";
        file << "type: " << tile.GetType() << "\n";
        file << "times: " << tile.GetTimes() << "\n";
        file << "frames: ";
        for (int32_t element : frames)
        {
            file << element << " ";
        }
        file << "\n\n";
    }
    file << "\n";

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
    file << "\n";
}
