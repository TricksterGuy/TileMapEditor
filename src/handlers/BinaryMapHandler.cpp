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
#include <cstring>
#include <algorithm>

#ifdef LINUX
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#ifdef WINDOWS
#include <winsock2.h>
#endif
#endif

#include "BinaryMapHandler.hpp"
#include "TileBasedCollisionLayer.hpp"
#include "PixelBasedCollisionLayer.hpp"
#include "AnimatedTile.hpp"

#define MAJOR 1
#define MINOR 0

const char magic[14] = {0x54, 0x52, 0x49, 0x43, 0x4b, 0x53, 0x54, 0x45, 0x52, 0x47, 0x55, 0x59, 0x38, 0x37};

BinaryMapHandler::BinaryMapHandler() : BaseMapHandler("Official Map Format V1.0", "map", "Basic format this program recognizes")
{
}

BinaryMapHandler::~BinaryMapHandler()
{
}

int BinaryMapHandler::load(const std::string& mapfile, Map& map)
{
    int32_t num_layers = -1;
    int32_t num_backgrounds = -1;

    std::ifstream file(mapfile.c_str(), std::ios::binary);
    bool error = false;

    if (!file.good()) throw "Could not open file";

    while (!file.eof() || !error)
    {
        std::string chunkname;
        uint32_t size;
        if (readChunkName(file, chunkname, size)) error = true;

        unsigned int start = file.tellg();

        if (chunkname == "HEAD")
        {
            if (readHEAD(file, map)) error = true;
        }
        else if (chunkname == "MAPP")
        {
            if (readMAPP(file, map, num_layers, num_backgrounds)) error = true;
        }
        else if (chunkname == "LYRS")
        {
            if (readLYRS(file, map, num_layers)) error = true;
        }
        else if (chunkname == "BGDS")
        {
            if (readBGDS(file, map, num_backgrounds)) error = true;
        }
        else if (chunkname == "MTCL")
        {
            if (readMTCL(file, map)) error = true;
        }
        else if (chunkname == "MDCL")
        {
            if (readMDCL(file, map)) error = true;
        }
        else if (chunkname == "MPCL")
        {
            if (readMPCL(file, map)) error = true;
        }
        else if (chunkname == "TTCI")
        {
            if (readTTCI(file, map)) error = true;
        }
        else if (chunkname == "TDCI")
        {
            if (readTDCI(file, map)) error = true;
        }
        else if (chunkname == "TPCI")
        {
            if (readTPCI(file, map)) error = true;
        }
        else if (chunkname == "ANIM")
        {
            if (readANIM(file, map)) error = true;
        }
        else if (chunkname == "EOM")
        {
            break;
        }
        else
        {
            fprintf(stderr, "Unknown Chunk id %s skipping\n", chunkname.c_str());
            file.seekg(size, std::ios_base::cur);
        }

        unsigned int end = file.tellg();

        if (end - start != size)
        {
            fprintf(stderr, "Malformed Chunk or size incorrect id %s size = %d read = %d\n", chunkname.c_str(), size, end - start);
            error = true;
        }
    }

    file.close();

    return error ? -1 : 0;
}

int BinaryMapHandler::save(const std::string& mapfile, Map& map)
{
    std::ofstream file(mapfile.c_str(), std::ios::binary);

    if (!file.good()) throw "Could not open file";

    if (writeHEAD(file, map)) return -1;
    if (writeMAPP(file, map)) return -1;
    if (writeLYRS(file, map)) return -1;
    if (map.getNumBackgrounds() > 0) {if (writeBGDS(file, map)) return -1;}
    if (map.hasCollisionLayer())
    {
        CollisionLayer* layer = map.getCollisionLayer();
        switch(layer->getType())
        {
            case Collision::TileBased:
                if (writeMTCL(file, map)) return -1;
                break;
            case Collision::DirectionBased:
                if (writeMDCL(file, map)) return -1;
                break;
            case Collision::PixelBased:
                if (writeMPCL(file, map)) return -1;
                break;
            default:
                fprintf(stderr, "Unknown Collision Type %d ignoring\n", layer->getType());
                break;
        }
    }
    // The folowing chunk's features are not implemented at this time
    //if (writeTTCI(file, map)) return -1;
    //if (writeTDCI(file, map)) return -1;
    //if (writeTPCI(file, map)) return -1;
    if (map.getNumAnimatedTiles() > 0) {if (writeANIM(file, map)) return -1;}

    // Write EOM chunk
    char eom[4] = "EOM";
    int32_t size = htonl(0);

    file.write(eom, sizeof(char) * 4);
    file.write((char*) &size, sizeof(int32_t));

    file.close();
    return 0;
}

int BinaryMapHandler::readChunkName(std::ifstream& file, std::string& name, uint32_t& size)
{
    char chunk[5];

    file.read(chunk, sizeof(char) * 4);
    chunk[4] = 0;

    file.read((char*) &size, sizeof(uint32_t));
    size = ntohl(size);

    name = chunk;

    if (file.fail()) throw "Failed to read chunk name";

    return 0;
}

int BinaryMapHandler::readHEAD(std::ifstream& file, Map& map)
{
    char major;
    char minor;
    char filemagic[14];

    file.read(&major, sizeof(char));
    file.read(&minor, sizeof(char));
    file.read(filemagic, sizeof(char) * 14);

    // We don't support newer formatted files than MAJOR.MINOR
    if (major > MAJOR) throw "Major version not accepted";
    if (minor > MINOR && major == MAJOR) throw "Minor version not accepted";

    // Check if magic numbers are equal.
    if (memcmp(magic, filemagic, sizeof(char) * 14) != 0) throw "Not a .map file";

    if (file.fail()) throw "Failed to read HEAD chunk";

    return 0;
}

int BinaryMapHandler::readMAPP(std::ifstream& file, Map& map, int32_t& num_layers, int32_t& num_backgrounds)
{
    uint32_t tile_width;
    uint32_t tile_height;
    uint32_t width;
    uint32_t height;
    std::string name;
    std::string filename;
    uint32_t texture_size;

    file.read((char*)(&width), sizeof(uint32_t));
    file.read((char*)(&height), sizeof(uint32_t));
    file.read((char*)(&tile_width), sizeof(uint32_t));
    file.read((char*)(&tile_height), sizeof(uint32_t));
    file.read((char*)(&num_layers), sizeof(uint32_t));
    file.read((char*)(&num_backgrounds), sizeof(uint32_t));

    tile_width = ntohl(tile_width);
    tile_height = ntohl(tile_height);
    width = ntohl(width);
    height = ntohl(height);
    num_layers = ntohl(num_layers);
    num_backgrounds = ntohl(num_backgrounds);

    file.read((char*)(&texture_size), sizeof(uint32_t));
    texture_size = ntohl(texture_size);
    char* temp = new char[texture_size];
    file.read(temp, sizeof(char) * texture_size);
    name = temp;
    delete[] temp;

    file.read((char*)(&texture_size), sizeof(uint32_t));
    texture_size = ntohl(texture_size);
    temp = new char[texture_size];
    file.read(temp, sizeof(char) * texture_size);
    filename = temp;
    delete[] temp;

    if (file.fail()) throw "Failed to read the MAPP chunk";
    if (num_layers <= -1 || num_backgrounds <= -1) throw "Invalid number of layers or backgrounds";

    map.setTileDimensions(tile_width, tile_height);
    map.resize(width, height);
    map.setName(name);
    map.setFilename(filename);

    if (file.fail()) throw "Failed to read the MAPP chunk";

    return 0;
}

int BinaryMapHandler::readLYRS(std::ifstream& file, Map& map, int32_t& num_layers)
{
    uint32_t texture_size;
    char* temp;
    uint32_t width = map.getWidth();
    uint32_t height = map.getHeight();
    int32_t* data = new int32_t[width * height];

    for (int i = 0; i < num_layers; i++)
    {
        file.read((char*)(&texture_size), sizeof(uint32_t));
        texture_size = ntohl(texture_size);
        temp = new char[texture_size];
        file.read(temp, sizeof(char) * texture_size);


        file.read((char*)(data), sizeof(uint32_t) * width * height);
        for (uint32_t j = 0; j < width * height; j++) data[j] = ntohl(data[j]);
        Layer layer(temp, width, height, data);
        map.add(layer);

        delete[] temp;
    }

    delete[] data;
    if (file.fail()) throw "Failed to read the LYRS chunk";

    return 0;
}

int BinaryMapHandler::readBGDS(std::ifstream& file, Map& map, int32_t& num_backgrounds)
{
    uint32_t texture_size;
    char* temp;

    union {float f; int32_t i;} convert;

    for (int i = 0; i < num_backgrounds; i++)
    {
        std::string name, filename;
        uint32_t mode;
        float x, y;
        int32_t ix, iy;

        file.read((char*)(&texture_size), sizeof(uint32_t));
        texture_size = ntohl(texture_size);
        temp = new char[texture_size];
        file.read(temp, sizeof(char) * texture_size);
        name = temp;
        delete[] temp;

        file.read((char*)(&texture_size), sizeof(uint32_t));
        texture_size = ntohl(texture_size);
        temp = new char[texture_size];
        file.read(temp, sizeof(char) * texture_size);
        filename = temp;
        delete[] temp;

        file.read((char*)(&mode), sizeof(uint32_t));
        mode = ntohl(mode);

        file.read((char*)(&ix), sizeof(int32_t));
        file.read((char*)(&iy), sizeof(int32_t));
        ix = ntohl(ix);
        iy = ntohl(iy);
        convert.i = ix;
        x = convert.f;
        convert.i = iy;
        y = convert.f;

        map.add(Background(name, filename, mode, x, y));
    }

    if (file.fail()) throw "Failed to read the BGDS chunk";

    return 0;
}

int BinaryMapHandler::readMTCL(std::ifstream& file, Map& map)
{
    uint32_t width = map.getWidth();
    uint32_t height = map.getHeight();
    int32_t* data = new int32_t[width * height];
    file.read((char*)(data), sizeof(int32_t) * width * height);
    for (uint32_t i = 0; i < width * height; i++) data[i] = ntohl(data[i]);
    TileBasedCollisionLayer* collisionLayer = new TileBasedCollisionLayer(width, height, data);
    map.setCollisionLayer(collisionLayer);

    delete[] data;
    if (file.fail()) throw "Failed to read the MTCL chunk";

    return 0;
}

int BinaryMapHandler::readMDCL(std::ifstream& file, Map& map)
{
    uint32_t width = map.getWidth();
    uint32_t height = map.getHeight();
    int32_t* data = new int32_t[width * height];
    file.read((char*)(data), sizeof(int32_t) * width * height);
    for (uint32_t i = 0; i < width * height; i++) data[i] = ntohl(data[i]);
    TileBasedCollisionLayer* collisionLayer = new TileBasedCollisionLayer(width, height, data);
    map.setCollisionLayer(collisionLayer);

    delete[] data;
    if (file.fail()) throw "Failed to read the MDCL chunk";

    return 0;
}

int BinaryMapHandler::readMPCL(std::ifstream& file, Map& map)
{
    int32_t numrects;
    file.read((char*)(&numrects), sizeof(int32_t));
    numrects = ntohl(numrects);
    std::vector<Rectangle> rectangles;
    rectangles.reserve(numrects);
    for (int32_t i = 0; i < numrects; i++)
    {
        int32_t x;
        int32_t y;
        uint32_t width;
        uint32_t height;
        file.read((char*)(&x), sizeof(int32_t));
        file.read((char*)(&y), sizeof(int32_t));
        file.read((char*)(&width), sizeof(uint32_t));
        file.read((char*)(&height), sizeof(uint32_t));
        x = ntohl(x);
        y = ntohl(y);
        width = ntohl(width);
        height = ntohl(height);
        rectangles.push_back(Rectangle(x, y, width, height));
    }
    map.setCollisionLayer(new PixelBasedCollisionLayer(rectangles));

    if (file.fail()) throw "Failed to read the MPCL chunk";

    return 0;
}

int BinaryMapHandler::readTTCI(std::ifstream& file, Map& map)
{
    throw "Failed to read the TTCI chunk";
    return -1;
}

int BinaryMapHandler::readTDCI(std::ifstream& file, Map& map)
{
    throw "Failed to read the TDCI chunk";
    return -1;
}

int BinaryMapHandler::readTPCI(std::ifstream& file, Map& map)
{
    throw "Failed to read the TPCI chunk";
    return -1;
}

int BinaryMapHandler::readANIM(std::ifstream& file, Map& map)
{
    uint32_t numanimations;
    file.read((char*)&numanimations, sizeof(uint32_t));
    numanimations = ntohl(numanimations);
    std::vector<AnimatedTile> tiles;
    tiles.reserve(numanimations);

    for (uint32_t i = 0; i < numanimations; i++)
    {
        int32_t delay, type, times;
        uint32_t frames;
        uint32_t texture_size;
        char* temp;
        std::string animname;

        file.read((char*)(&texture_size), sizeof(uint32_t));
        texture_size = ntohl(texture_size);
        temp = new char[texture_size];
        file.read(temp, sizeof(char) * texture_size);
        animname = temp;
        delete[] temp;

        file.read((char*)&delay, sizeof(int32_t));
        file.read((char*)&type, sizeof(int32_t));
        file.read((char*)&times, sizeof(int32_t));
        file.read((char*)&frames, sizeof(uint32_t));

        delay = ntohl(delay);
        type = ntohl(type);
        times = ntohl(times);
        frames = ntohl(frames);

        int32_t* data = new int32_t[frames];
        file.read((char*)data, sizeof(int32_t) * frames);
        for (uint32_t i = 0; i < frames; i++) data[i] = ntohl(data[i]);
        std::vector<int32_t> animframes(data, data+frames);
        delete[] data;

        tiles.push_back(AnimatedTile(animname, delay, (Animation::Type)type, times, animframes));
    }
    map.setAnimatedTiles(tiles);

    if (file.fail()) throw "Failed to read the ANIM chunk";

    return 0;
}

int BinaryMapHandler::writeHEAD(std::ofstream& file, Map& map)
{
    char major = MAJOR;
    char minor = MINOR;
    const std::string& chunk = "HEAD";
    // Header is always 16 bytes
    uint32_t size = htonl(16);

    file.write(chunk.c_str(), sizeof(char) * 4);
    file.write((char*) &size, sizeof(int32_t));
    file.write(&major, sizeof(char));
    file.write(&minor, sizeof(char));
    file.write(magic, sizeof(char) * 14);

    if (file.fail()) return -1;

    return 0;
}

int BinaryMapHandler::writeMAPP(std::ofstream& file, Map& map)
{
    // 8 ints + two strings.
    uint32_t size = htonl(8 * sizeof(uint32_t) + (map.getName().length() + 2 + map.getFilename().length()) * sizeof(char));
    const std::string& chunk = "MAPP";

    file.write(chunk.c_str(), sizeof(char) * 4);
    file.write((char*) &size, sizeof(uint32_t));

    uint32_t tile_width = htonl(map.getTileWidth());
    uint32_t tile_height = htonl(map.getTileHeight());
    uint32_t width = htonl(map.getWidth());
    uint32_t height = htonl(map.getHeight());
    uint32_t texture_size = map.getName().length() + 1;
    uint32_t texture_size_nl = htonl(texture_size);
    uint32_t numlayers = htonl(map.getNumLayers());
    uint32_t numbackgrounds = htonl(map.getNumBackgrounds());

    file.write((char*)(&width), sizeof(uint32_t));
    file.write((char*)(&height), sizeof(uint32_t));
    file.write((char*)(&tile_width), sizeof(uint32_t));
    file.write((char*)(&tile_height), sizeof(uint32_t));
    file.write((char*)(&numlayers), sizeof(uint32_t));
    file.write((char*)(&numbackgrounds), sizeof(uint32_t));

    file.write((char*)(&texture_size_nl), sizeof(uint32_t));
    file.write(map.getName().c_str(), sizeof(char) * texture_size);

    texture_size = map.getFilename().length() + 1;
    texture_size_nl = htonl(texture_size);
    file.write((char*)(&texture_size_nl), sizeof(uint32_t));
    file.write(map.getFilename().c_str(), sizeof(char) * texture_size);

    if (file.fail()) return -1;

    return 0;
}

int BinaryMapHandler::writeLYRS(std::ofstream& file, Map& map)
{
    const std::string& chunk = "LYRS";
    uint32_t size = 0;
    uint32_t width = map.getWidth();
    uint32_t height = map.getHeight();

    for (uint32_t i = 0; i < map.getNumLayers(); i++)
    {
        Layer& layer = map.getLayer(i);

        size += sizeof(uint32_t);
        size += (layer.getName().length() + 1) * sizeof(char);
        size += width * height * sizeof(uint32_t);
    }
    size = htonl(size);

    file.write(chunk.c_str(), sizeof(char) * 4);
    file.write((char*) &size, sizeof(uint32_t));

    int32_t texture_size, texture_size_nl;

    int32_t* buffer = new int32_t[width * height];

    for (unsigned int i = 0; i < map.getNumLayers(); i++)
    {
        Layer& layer = map.getLayer(i);
        texture_size = layer.getName().length() + 1;
        texture_size_nl = htonl(texture_size);
        file.write((char*)(&texture_size_nl), sizeof(int32_t));
        file.write(layer.getName().c_str(), sizeof(char) * texture_size);
        std::vector<int32_t>& data = layer.getData();
        for (uint32_t i = 0; i < width * height; i++) buffer[i] = htonl(data[i]);
        file.write((char*) buffer, sizeof(int32_t) * width * height);
    }

    delete[] buffer;

    if (file.fail()) return -1;

    return 0;
}

int BinaryMapHandler::writeBGDS(std::ofstream& file, Map& map)
{
    uint32_t size = 0;
    const std::string& chunk = "BGDS";

    for (uint32_t i = 0; i < map.getNumBackgrounds(); i++)
    {
        Background& back = map.getBackground(i);
        size += (back.getName().length() + 1) * sizeof(char);
        size += (back.getFilename().length() + 1) * sizeof(char);
        size += 5 * sizeof(int32_t);
    }
    size = htonl(size);

    file.write(chunk.c_str(), sizeof(char) * 4);
    file.write((char*) &size, sizeof(int32_t));

    uint32_t texture_size, texture_size_nl;
    union {float f; int32_t i;} convert;

    for (uint32_t i = 0; i < map.getNumBackgrounds(); i++)
    {
        Background back = map.getBackground(i);
        float x, y;
        int32_t ix, iy;
        uint32_t mode = htonl(back.getMode());
        back.getSpeed(x, y);

        convert.f = x;
        ix = htonl(convert.i);
        convert.f = y;
        iy = htonl(convert.i);

        texture_size = back.getName().length() + 1;
        texture_size_nl = htonl(texture_size);
        file.write((char*)(&texture_size_nl), sizeof(uint32_t));
        file.write(back.getName().c_str(), sizeof(char) * texture_size);

        texture_size = back.getFilename().length() + 1;
        texture_size_nl = htonl(texture_size);
        file.write((char*)(&texture_size_nl), sizeof(uint32_t));
        file.write(back.getFilename().c_str(), sizeof(char) * texture_size);

        file.write((char*) &mode, sizeof(uint32_t));
        file.write((char*) &ix, sizeof(int32_t));
        file.write((char*) &iy, sizeof(int32_t));
    }

    if (file.fail()) return -1;

    return 0;
}

int BinaryMapHandler::writeMTCL(std::ofstream& file, Map& map)
{
    uint32_t width = map.getWidth();
    uint32_t height = map.getHeight();

    uint32_t size = htonl(sizeof(int32_t) * width * height);
    const std::string& chunk = "MTCL";

    file.write(chunk.c_str(), sizeof(char) * 4);
    file.write((char*) &size, sizeof(uint32_t));

    TileBasedCollisionLayer* layer = dynamic_cast<TileBasedCollisionLayer*>(map.getCollisionLayer());
    int32_t* buffer = new int32_t[width * height];
    std::vector<int32_t>& data = layer->getData();

    for (uint32_t i = 0; i < width * height; i++) buffer[i] = htonl(data[i]);
    file.write((char*) buffer, sizeof(int32_t) * width * height);

    delete[] buffer;

    if (file.fail()) return -1;

    return 0;
}

int BinaryMapHandler::writeMDCL(std::ofstream& file, Map& map)
{
    uint32_t width = map.getWidth();
    uint32_t height = map.getHeight();

    uint32_t size = htonl(sizeof(int32_t) * width * height);
    const std::string& chunk = "MDCL";

    file.write(chunk.c_str(), sizeof(char) * 4);
    file.write((char*) &size, sizeof(uint32_t));

    TileBasedCollisionLayer* layer = dynamic_cast<TileBasedCollisionLayer*>(map.getCollisionLayer());
    int32_t* buffer = new int32_t[width * height];
    std::vector<int32_t>& data = layer->getData();

    for (uint32_t i = 0; i < width * height; i++) buffer[i] = htonl(data[i]);
    file.write((char*) buffer, sizeof(int32_t) * width * height);

    delete[] buffer;

    if (file.fail()) return -1;

    return 0;
}

int BinaryMapHandler::writeMPCL(std::ofstream& file, Map& map)
{
    PixelBasedCollisionLayer* layer = dynamic_cast<PixelBasedCollisionLayer*>(map.getCollisionLayer());
    const Region& region = layer->getData();
    const std::vector<Rectangle> rectangles = region.getData();
    uint32_t numrects = region.size();

    uint32_t size = htonl(sizeof(uint32_t) + sizeof(uint32_t) * 4 * numrects);
    const std::string& chunk = "MPCL";

    file.write(chunk.c_str(), sizeof(char) * 4);
    file.write((char*) &size, sizeof(uint32_t));

    uint32_t num = htonl(numrects);
    file.write((char*)&num, sizeof(uint32_t));

    for (uint32_t i = 0; i < numrects; i++)
    {
        const Rectangle& r = rectangles[i];
        int32_t x = htonl(r.x);
        int32_t y = htonl(r.y);
        uint32_t width = htonl(r.width);
        uint32_t height = htonl(r.height);

        file.write((char*) &x, sizeof(int32_t));
        file.write((char*) &y, sizeof(int32_t));
        file.write((char*) &width, sizeof(uint32_t));
        file.write((char*) &height, sizeof(uint32_t));
    }

    if (file.fail()) return -1;

    return 0;
}

int BinaryMapHandler::writeTTCI(std::ofstream& file, Map& map)
{
    return -1;
}

int BinaryMapHandler::writeTDCI(std::ofstream& file, Map& map)
{
    return -1;
}

int BinaryMapHandler::writeTPCI(std::ofstream& file, Map& map)
{
    return -1;
}

int BinaryMapHandler::writeANIM(std::ofstream& file, Map& map)
{
    uint32_t size = sizeof(int32_t);
    const std::string& chunk = "ANIM";
    for (uint32_t i = 0; i < map.getNumAnimatedTiles(); i++)
    {
        AnimatedTile& anim = map.getAnimatedTile(i);
        size += (anim.getName().length() + 1) * sizeof(char);
        size += (anim.getFrames().size() + 1) * sizeof(int32_t);
        size += 4 * sizeof(int32_t);
    }
    size = htonl(size);

    file.write(chunk.c_str(), sizeof(char) * 4);
    file.write((char*) &size, sizeof(uint32_t));

    uint32_t numanimations = map.getNumAnimatedTiles();
    numanimations = htonl(numanimations);
    file.write((char*)&numanimations, sizeof(uint32_t));

    for (uint32_t i = 0; i < map.getNumAnimatedTiles(); i++)
    {
        AnimatedTile& anim = map.getAnimatedTile(i);
        int32_t delay, type, times;
        uint32_t frames;
        uint32_t texture_size, texture_size_nl;

        texture_size = anim.getName().length() + 1;
        texture_size_nl = htonl(texture_size);
        file.write((char*)(&texture_size_nl), sizeof(uint32_t));
        file.write(anim.getName().c_str(), sizeof(char) * texture_size);

        delay = htonl(anim.getDelay());
        type = htonl(anim.getType());
        times = htonl(anim.getTimes());
        frames = htonl(anim.getNumFrames());

        file.write((char*)&delay, sizeof(int32_t));
        file.write((char*)&type, sizeof(int32_t));
        file.write((char*)&times, sizeof(int32_t));
        file.write((char*)&frames, sizeof(uint32_t));

        frames = anim.getNumFrames();
        int32_t* data = new int32_t[frames];
        copy(anim.getFrames().begin(), anim.getFrames().end(), data);
        for (uint32_t i = 0; i < frames; i++) data[i] = htonl(data[i]);
        file.write((char*)data, sizeof(int32_t) * frames);
        delete[] data;
    }

    if (file.fail()) return -1;

    return 0;
}
