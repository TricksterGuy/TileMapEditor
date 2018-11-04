/******************************************************************************************************
 * Tile Map Editor
 * Copyright (C) 2009-2017 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
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
#include "BinaryMapHandler.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <istream>

#ifdef LINUX
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#ifdef WINDOWS
#include <winsock2.h>
#endif
#endif

#include "AnimatedTile.hpp"
#include "ChunkStream.hpp"
#include "Logger.hpp"
#include "PixelBasedCollisionLayer.hpp"
#include "TileBasedCollisionLayer.hpp"

static const char MAJOR = 2;
static const char MINOR = 0;
static constexpr char magic_str[14] = {0x54, 0x52, 0x49, 0x43, 0x4b, 0x53, 0x54, 0x45, 0x52, 0x47, 0x55, 0x59, 0x38, 0x37};
static std::string MAGIC(magic_str, 14);

namespace
{

void ReadDrawAttributes(ChunkStreamReader& cs, DrawAttributes* attr)
{
    int32_t depth;
    int32_t x, y;
    int32_t origin_x, origin_y;
    float scale_x, scale_y;
    float rotation;
    float opacity;
    uint32_t blend_mode;
    uint32_t blend_color;

    cs >> depth;
    cs >> x;
    cs >> y;
    cs >> origin_x;
    cs >> origin_y;
    cs >> scale_x;
    cs >> scale_y;
    cs >> rotation;
    cs >> opacity;
    cs >> blend_mode;
    cs >> blend_color;

    attr->SetDepth(depth);
    attr->SetPosition(x, y);
    attr->SetOrigin(origin_x, origin_y);
    attr->SetScale(scale_x, scale_y);
    attr->SetRotation(rotation);
    attr->SetOpacity(opacity);
    attr->SetBlendMode(blend_mode);
    attr->SetBlendColor(blend_color);
}

void WriteDrawAttributes(ChunkStreamWriter& cs, const DrawAttributes* attr)
{
    int32_t x, y;
    int32_t origin_x, origin_y;
    float scale_x, scale_y;

    attr->GetPosition(x, y);
    attr->GetOrigin(origin_x, origin_y);
    attr->GetScale(scale_x, scale_y);

    cs << attr->GetDepth();
    cs << x;
    cs << y;
    cs << origin_x;
    cs << origin_y;
    cs << scale_x;
    cs << scale_y;
    cs << attr->GetRotation();
    cs << attr->GetOpacity();
    cs << attr->GetBlendMode();
    cs << attr->GetBlendColor();
}

}

BinaryMapHandler::BinaryMapHandler()
    : BaseMapHandler("Official Map Format", "map", "Basic format this program recognizes")
{
}

void BinaryMapHandler::Load(const std::string& mapfile, Map& map)
{
    std::ifstream file(mapfile.c_str(), std::ios::binary);
    if (!file.good())
        throw "Could not open file";

    Load(file, map);
    file.close();
}

void BinaryMapHandler::Save(const std::string& mapfile, const Map& map)
{
    EventLog l(__func__);

    std::ofstream file(mapfile.c_str(), std::ios::binary);
    if (!file.good())
        throw "Could not open file";

    Save(file, map);
    file.close();
}

void BinaryMapHandler::Load(std::istream& file, Map& map)
{
    EventLog l(__func__);

    while (!file.eof())
    {
        ChunkStreamReader csr(file, 4);
        std::string chunkname = csr.Name();
        uint32_t size = csr.Size();
        VerboseLog("Read chunk name %s size %zd", chunkname.c_str(), size);

        unsigned int start = file.tellg();

        if (chunkname == "HEAD")
            ReadHEAD(csr, map);
        else if (chunkname == "MAPP")
            ReadMAPP(csr, map);
        else if (chunkname == "LYRS")
            ReadLYRS(csr, map);
        else if (chunkname == "BGDS")
            ReadBGDS(csr, map);
        else if (chunkname == "MTCL")
            ReadMTCL(csr, map);
        else if (chunkname == "MDCL")
            ReadMDCL(csr, map);
        else if (chunkname == "MPCL")
            ReadMPCL(csr, map);
        else if (chunkname == "TTCI")
            ReadTTCI(csr, map);
        else if (chunkname == "TDCI")
            ReadTDCI(csr, map);
        else if (chunkname == "TPCI")
            ReadTPCI(csr, map);
        else if (chunkname == "ANIM")
            ReadANIM(csr, map);
        else if (chunkname == std::string("EOM\0", 4))
            break;
        else
        {
            VerboseLog("Unknown Chunk id %s skipping\n", chunkname.c_str());
            file.seekg(size, std::ios_base::cur);
        }

        unsigned int end = file.tellg();

        if (end - start != size)
            VerboseLog("Malformed Chunk or size incorrect id %s size = %d read = %d\n", chunkname.c_str(), size, end - start);
    }
}

void BinaryMapHandler::Save(std::ostream& file, const Map& map)
{
    EventLog l(__func__);
    WriteHEAD(file, map);
    WriteMAPP(file, map);
    WriteLYRS(file, map);
    if (map.GetNumBackgrounds() > 0)
        WriteBGDS(file, map);
    if (map.HasCollisionLayer())
    {
        CollisionLayer* layer = map.GetCollisionLayer();
        switch (layer->GetType())
        {
            case CollisionLayer::TileBased:
                WriteMTCL(file, map);
                break;
            case CollisionLayer::DirectionBased:
                WriteMDCL(file, map);
                break;
            case CollisionLayer::PixelBased:
                WriteMPCL(file, map);
                break;
            default:
                fprintf(stderr, "Unknown Collision Type %d ignoring\n", layer->GetType());
                break;
        }
    }
    // The following chunk's features are not implemented at this time
    // if (writeTTCI(file, map)) return -1;
    // if (writeTDCI(file, map)) return -1;
    // if (writeTPCI(file, map)) return -1;
    if (map.GetTileset().GetAnimatedTiles().size() > 0)
        WriteANIM(file, map);

    // Write EOM chunk
    char eom[4] = {'E', 'O', 'M', 0};
    uint32_t size = 0;

    file.write(eom, sizeof(char) * 4);
    file.write((char*)&size, sizeof(int32_t));
}

void BinaryMapHandler::ReadHEAD(ChunkStreamReader& head, Map& map)
{
    EventLog l(__func__);

    head >> set_flags(ChunkStreamReader::NO_READ_SIZES);

    char major;
    char minor;
    std::string filemagic;

    head >> major;
    head >> minor;
    head >> set_width(14) >> filemagic;

    // We don't support newer formatted files than MAJOR.MINOR
    if (major > MAJOR)
        throw "Incorrect major version";
    if (minor > MINOR && major == MAJOR)
        throw "Incorrect minor version";

    // Check if magic numbers are equal.
    if (MAGIC != filemagic)
        throw "Not a .map file";

    if (!head.Ok())
        throw "Failed to read HEAD chunk";
}

void BinaryMapHandler::WriteHEAD(std::ostream& file, const Map& map)
{
    EventLog l(__func__);
    ChunkStreamWriter head("HEAD", ChunkStreamWriter::NO_WRITE_SIZES);

    head << MAJOR;
    head << MINOR;
    head << MAGIC;

    file << head;

    if (file.fail())
        throw "Failed to write the HEAD chunk";
}

void BinaryMapHandler::ReadMAPP(ChunkStreamReader& mapp, Map& map)
{
    EventLog l(__func__);

    uint32_t tile_width;
    uint32_t tile_height;
    std::string name;
    std::string filename;

    mapp >> name;
    mapp >> filename;
    mapp >> tile_width;
    mapp >> tile_height;

    map.SetName(name);
    map.SetTileset(Tileset(filename, tile_width, tile_height));

    if (!mapp.Ok())
        throw "Failed to read MAPP chunk";
}

void BinaryMapHandler::WriteMAPP(std::ostream& file, const Map& map)
{
    EventLog l(__func__);
    ChunkStreamWriter mapp("MAPP");

    mapp << map.GetName();

    const Tileset& tileset = map.GetTileset();
    uint32_t tile_width, tile_height;
    tileset.GetTileDimensions(tile_width, tile_height);

    mapp << tileset.GetFilename();
    mapp << tile_width;
    mapp << tile_height;

    file << mapp;

    if (file.fail())
        throw "Failed to write the MAPP chunk";
}

void BinaryMapHandler::ReadLYRS(ChunkStreamReader& lyrs, Map& map)
{
    EventLog l(__func__);

    uint32_t num_layers;

    lyrs >> set_flags(ChunkStreamReader::NO_READ_VECTOR_SIZES | ChunkStreamReader::READ_STRING_SIZES);
    lyrs >> num_layers;
    for (uint32_t i = 0; i < num_layers; i++)
    {
        std::string name;
        uint32_t width;
        uint32_t height;
        DrawAttributes attrs;
        std::vector<int32_t> data;

        lyrs >> name;
        lyrs >> width;
        lyrs >> height;
        ReadDrawAttributes(lyrs, &attrs);
        data.resize(width * height);
        lyrs >> data;

        map.Add(Layer(name, width, height, data, attrs));
    }

    if (!lyrs.Ok())
        throw "Failed to read the LYRS chunk";
}

void BinaryMapHandler::WriteLYRS(std::ostream& file, const Map& map)
{
    EventLog l(__func__);
    ChunkStreamWriter lyrs("LYRS", ChunkStreamWriter::NO_WRITE_VECTOR_SIZES | ChunkStreamWriter::WRITE_STRING_SIZES);

    lyrs << map.GetNumLayers();
    for (const auto& layer : map.GetLayers())
    {
        lyrs << layer.GetName();
        lyrs << layer.GetWidth();
        lyrs << layer.GetHeight();
        WriteDrawAttributes(lyrs, &layer);
        lyrs << layer.GetData();
    }

    file << lyrs;

    if (file.fail())
        throw "Failed to write the LYRS chunk";
}

void BinaryMapHandler::ReadBGDS(ChunkStreamReader& bgds, Map& map)
{
    EventLog l(__func__);
    uint32_t num_backgrounds;

    bgds >> num_backgrounds;
    for (uint32_t i = 0; i < num_backgrounds; i++)
    {
        std::string name;
        std::string filename;
        uint32_t mode;
        float x;
        float y;
        DrawAttributes attrs;

        bgds >> name;
        bgds >> filename;
        bgds >> mode;
        bgds >> x;
        bgds >> y;
        ReadDrawAttributes(bgds, &attrs);

        map.Add(Background(name, filename, mode, x, y, attrs));
    }

    if (!bgds.Ok())
        throw "Failed to read the BGDS chunk";
}

void BinaryMapHandler::WriteBGDS(std::ostream& file, const Map& map)
{
    EventLog l(__func__);
    ChunkStreamWriter bgds("BGDS");

    bgds << map.GetNumBackgrounds();
    for (const auto& background : map.GetBackgrounds())
    {
        float speedx, speedy;
        background.GetSpeed(speedx, speedy);

        bgds << background.GetName();
        bgds << background.GetFilename();
        bgds << background.GetMode();
        bgds << speedx;
        bgds << speedy;
        WriteDrawAttributes(bgds, &background);
    }

    file << bgds;

    if (file.fail())
        throw "Failed to write the BGDS chunk";
}

void BinaryMapHandler::ReadMTCL(ChunkStreamReader& mtcl, Map& map)
{
    EventLog l(__func__);
    uint32_t width;
    uint32_t height;
    std::vector<int32_t> data;

    mtcl >> set_flags(ChunkStreamReader::NO_READ_SIZES);

    mtcl >> width;
    mtcl >> height;
    data.resize(width * height);
    mtcl >> data;
    map.SetCollisionLayer(new TileBasedCollisionLayer(width, height, data));

    if (!mtcl.Ok())
        throw "Failed to read the MTCL chunk";
}

void BinaryMapHandler::WriteMTCL(std::ostream& file, const Map& map)
{
    EventLog l(__func__);
    ChunkStreamWriter mtcl("MTCL", ChunkStreamWriter::NO_WRITE_SIZES);

    TileBasedCollisionLayer* layer = dynamic_cast<TileBasedCollisionLayer*>(map.GetCollisionLayer());
    mtcl << layer->GetWidth();
    mtcl << layer->GetHeight();
    mtcl << layer->GetData();

    file << mtcl;

    if (file.fail())
        throw "Failed to write the MTCL chunk";
}

void BinaryMapHandler::ReadMDCL(ChunkStreamReader& mdcl, Map& map)
{
    EventLog l(__func__);
    uint32_t width;
    uint32_t height;
    std::vector<int32_t> data;

    mdcl >> set_flags(ChunkStreamReader::NO_READ_SIZES);

    mdcl >> width;
    mdcl >> height;
    data.resize(width * height);
    mdcl >> data;
    map.SetCollisionLayer(new TileBasedCollisionLayer(width, height, data));

    if (!mdcl.Ok())
        throw "Failed to read the MTCL chunk";
}


void BinaryMapHandler::WriteMDCL(std::ostream& file, const Map& map)
{
    EventLog l(__func__);
    ChunkStreamWriter mdcl("MDCL");

    TileBasedCollisionLayer* layer = dynamic_cast<TileBasedCollisionLayer*>(map.GetCollisionLayer());
    mdcl << layer->GetWidth();
    mdcl << layer->GetHeight();
    mdcl << layer->GetData();

    file << mdcl;

    if (file.fail())
        throw "Failed to write the MDCL chunk";
}

void BinaryMapHandler::ReadMPCL(ChunkStreamReader& mpcl, Map& map)
{
    EventLog l(__func__);
    uint32_t numrects;

    mpcl >> numrects;

    std::vector<Rectangle> rectangles;
    rectangles.reserve(numrects);
    for (uint32_t i = 0; i < numrects; i++)
    {
        int32_t x;
        int32_t y;
        uint32_t width;
        uint32_t height;

        mpcl >> x;
        mpcl >> y;
        mpcl >> width;
        mpcl >> height;

        rectangles.emplace_back(x, y, width, height);
    }

    map.SetCollisionLayer(new PixelBasedCollisionLayer(rectangles));

    if (!mpcl.Ok())
        throw "Failed to read the MPCL chunk";
}

void BinaryMapHandler::WriteMPCL(std::ostream& file, const Map& map)
{
    EventLog l(__func__);
    ChunkStreamWriter mpcl("MPCL");

    PixelBasedCollisionLayer* layer = dynamic_cast<PixelBasedCollisionLayer*>(map.GetCollisionLayer());
    const Region& region = layer->GetData();
    const std::vector<Rectangle> rectangles = region.GetData();

    mpcl << (uint32_t) rectangles.size();
    for (const auto& rectangle : rectangles)
    {
        mpcl << rectangle.x;
        mpcl << rectangle.y;
        mpcl << rectangle.width;
        mpcl << rectangle.height;
    }

    file << mpcl;

    if (file.fail())
        throw "Failed to write the MPCL chunk";
}

void BinaryMapHandler::ReadTTCI(ChunkStreamReader& ttci, Map& map)
{
    EventLog l(__func__);
    throw "Failed to read the TTCI chunk";
}

void BinaryMapHandler::WriteTTCI(std::ostream& file, const Map& map)
{
    EventLog l(__func__);
    throw "Failed to write the TTCI chunk";
}

void BinaryMapHandler::ReadTDCI(ChunkStreamReader& tdci, Map& map)
{
    EventLog l(__func__);
    throw "Failed to read the TDCI chunk";
}

void BinaryMapHandler::WriteTDCI(std::ostream& file, const Map& map)
{
    EventLog l(__func__);
    throw "Failed to write the TDCI chunk";
}

void BinaryMapHandler::ReadTPCI(ChunkStreamReader& tpci, Map& map)
{
    EventLog l(__func__);
    throw "Failed to read the TPCI chunk";
}

void BinaryMapHandler::WriteTPCI(std::ostream& file, const Map& map)
{
    EventLog l(__func__);
    throw "Failed to write the TPCI chunk";
}

void BinaryMapHandler::ReadANIM(ChunkStreamReader& anim, Map& map)
{
    EventLog l(__func__);
    uint32_t num_animations;

    anim >> num_animations;

    std::vector<AnimatedTile> tiles;
    tiles.reserve(num_animations);
    for (uint32_t i = 0; i < num_animations; i++)
    {
        std::string name;
        int32_t delay;
        int32_t type;
        int32_t times;
        std::vector<int32_t> frames;

        anim >> name;
        anim >> delay;
        anim >> type;
        anim >> times;
        anim >> frames;

        tiles.emplace_back(name, delay, static_cast<AnimatedTile::Type>(type), times, frames);
    }
    Tileset& tileset = map.GetTileset();
    tileset.SetAnimatedTiles(tiles);

    if (!anim.Ok())
        throw "Failed to read the ANIM chunk";
}


void BinaryMapHandler::WriteANIM(std::ostream& file, const Map& map)
{
    EventLog l(__func__);
    ChunkStreamWriter anim("ANIM", ChunkStreamWriter::WRITE_SIZES);

    const Tileset& tileset = map.GetTileset();
    const std::vector<AnimatedTile>& animated_tiles = tileset.GetAnimatedTiles();

    anim << (uint32_t) animated_tiles.size();

    for (const auto& tile : animated_tiles)
    {
        anim << tile.GetName();
        anim << tile.GetDelay();
        anim << tile.GetType();
        anim << tile.GetTimes();
        anim << tile.GetFrames();
    }

    file << anim;

    if (file.fail())
        throw "Failed to write the ANIM chunk";
}
