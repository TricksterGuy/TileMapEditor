/******************************************************************************************************
 * Tile Map Editor
 * Copyright (C) 2009-2015 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
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
#include "Logger.hpp"
#include "PixelBasedCollisionLayer.hpp"
#include "TileBasedCollisionLayer.hpp"

#define MAJOR 2
#define MINOR 0

const char magic[14] = {0x54, 0x52, 0x49, 0x43, 0x4b, 0x53, 0x54, 0x45, 0x52, 0x47, 0x55, 0x59, 0x38, 0x37};

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

  int32_t num_layers = -1;
  int32_t num_backgrounds = -1;

  while (!file.eof())
  {
    std::string chunkname;
    uint32_t size;

    ReadChunkName(file, chunkname, size);
    VerboseLog("Read chunk name %s size %zd", chunkname.c_str(), size);

    unsigned int start = file.tellg();

    if (chunkname == "HEAD")
      ReadHEAD(file, map);
    else if (chunkname == "MAPP")
      ReadMAPP(file, map, num_layers, num_backgrounds);
    else if (chunkname == "LYRS")
      ReadLYRS(file, map, num_layers);
    else if (chunkname == "BGDS")
      ReadBGDS(file, map, num_backgrounds);
    else if (chunkname == "MTCL")
      ReadMTCL(file, map);
    else if (chunkname == "MDCL")
      ReadMDCL(file, map);
    else if (chunkname == "MPCL")
      ReadMPCL(file, map);
    else if (chunkname == "TTCI")
      ReadTTCI(file, map);
    else if (chunkname == "TDCI")
      ReadTDCI(file, map);
    else if (chunkname == "TPCI")
      ReadTPCI(file, map);
    else if (chunkname == "ANIM")
      ReadANIM(file, map);
    else if (chunkname == "EOM")
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
      case Collision::TileBased:
        WriteMTCL(file, map);
        break;
      case Collision::DirectionBased:
        WriteMDCL(file, map);
        break;
      case Collision::PixelBased:
        WriteMPCL(file, map);
        break;
      default:
        fprintf(stderr, "Unknown Collision Type %d ignoring\n", layer->GetType());
        break;
    }
  }
  // The folowing chunk's features are not implemented at this time
  // if (writeTTCI(file, map)) return -1;
  // if (writeTDCI(file, map)) return -1;
  // if (writeTPCI(file, map)) return -1;
  if (map.GetNumAnimatedTiles() > 0)
    WriteANIM(file, map);

  // Write EOM chunk
  char eom[4] = "EOM";
  int32_t size = htonl(0);

  file.write(eom, sizeof(char) * 4);
  file.write((char*)&size, sizeof(int32_t));
}

void BinaryMapHandler::ReadChunkName(std::istream& file, std::string& name, uint32_t& size)
{
  EventLog l(__func__);
  char chunk[5];

  file.read(chunk, sizeof(char) * 4);
  chunk[4] = 0;

  file.read((char*)&size, sizeof(uint32_t));
  size = ntohl(size);

  name = chunk;

  if (file.fail())
    throw "Failed to read chunk name";
}

void BinaryMapHandler::ReadHEAD(std::istream& file, Map& map)
{
  EventLog l(__func__);
  char major;
  char minor;
  char filemagic[14];

  file.read(&major, sizeof(char));
  file.read(&minor, sizeof(char));
  file.read(filemagic, sizeof(char) * 14);

  // We don't support newer formatted files than MAJOR.MINOR
  if (major > MAJOR)
    throw "Incorrect major version";
  if (minor > MINOR && major == MAJOR)
    throw "Incorrect minor version";

  // Check if magic numbers are equal.
  if (memcmp(magic, filemagic, sizeof(char) * 14) != 0)
    throw "Not a .map file";

  if (file.fail())
    throw "Failed to read HEAD chunk";
}

void BinaryMapHandler::ReadMAPP(std::istream& file, Map& map, int32_t& num_layers, int32_t& num_backgrounds)
{
  EventLog l(__func__);
  uint32_t tile_width;
  uint32_t tile_height;
  std::string name;
  std::string filename;
  uint32_t texture_size;

  file.read((char*)(&tile_width), sizeof(uint32_t));
  file.read((char*)(&tile_height), sizeof(uint32_t));
  file.read((char*)(&num_layers), sizeof(uint32_t));
  file.read((char*)(&num_backgrounds), sizeof(uint32_t));

  tile_width = ntohl(tile_width);
  tile_height = ntohl(tile_height);
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

  if (file.fail())
    throw "Failed to read the MAPP chunk";
  if (num_layers <= -1 || num_backgrounds <= -1)
    throw "Invalid number of layers or backgrounds";

  map.SetTileDimensions(tile_width, tile_height);
  map.SetName(name);
  map.SetFilename(filename);
}

void BinaryMapHandler::ReadLYRS(std::istream& file, Map& map, int32_t& num_layers)
{
  EventLog l(__func__);
  uint32_t texture_size;
  uint32_t width = map.GetWidth();
  uint32_t height = map.GetHeight();
  int32_t* data = new int32_t[width * height];

  for (int i = 0; i < num_layers; i++)
  {
    file.read((char*)(&texture_size), sizeof(uint32_t));
    texture_size = ntohl(texture_size);
    char* temp = new char[texture_size];
    file.read(temp, sizeof(char) * texture_size);


    file.read((char*)(data), sizeof(uint32_t) * width * height);
    for (uint32_t j = 0; j < width * height; j++) data[j] = ntohl(data[j]);
    Layer layer(temp, width, height, data);
    map.Add(layer);

    delete[] temp;
  }

  delete[] data;

  if (file.fail())
    throw "Failed to read the LYRS chunk";
}

void BinaryMapHandler::ReadBGDS(std::istream& file, Map& map, int32_t& num_backgrounds)
{
  EventLog l(__func__);
  uint32_t texture_size;
  char* temp;

  union
  {
    float f;
    int32_t i;
  } convert;

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

    map.Add(Background(name, filename, mode, x, y));
  }

  if (file.fail())
    throw "Failed to read the BGDS chunk";
}

void BinaryMapHandler::ReadMTCL(std::istream& file, Map& map)
{
  EventLog l(__func__);
  uint32_t width = map.GetWidth();
  uint32_t height = map.GetHeight();
  int32_t* data = new int32_t[width * height];
  file.read((char*)(data), sizeof(int32_t) * width * height);
  for (uint32_t i = 0; i < width * height; i++) data[i] = ntohl(data[i]);
  TileBasedCollisionLayer* collisionLayer = new TileBasedCollisionLayer(width, height, data);
  map.SetCollisionLayer(collisionLayer);
  delete[] data;

  if (file.fail())
    throw "Failed to read the MTCL chunk";
}

void BinaryMapHandler::ReadMDCL(std::istream& file, Map& map)
{
  EventLog l(__func__);
  uint32_t width = map.GetWidth();
  uint32_t height = map.GetHeight();
  int32_t* data = new int32_t[width * height];
  file.read((char*)(data), sizeof(int32_t) * width * height);
  for (uint32_t i = 0; i < width * height; i++) data[i] = ntohl(data[i]);
  TileBasedCollisionLayer* collisionLayer = new TileBasedCollisionLayer(width, height, data);
  map.SetCollisionLayer(collisionLayer);
  delete[] data;

  if (file.fail())
    throw "Failed to read the MDCL chunk";
}

void BinaryMapHandler::ReadMPCL(std::istream& file, Map& map)
{
  EventLog l(__func__);
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
  map.SetCollisionLayer(new PixelBasedCollisionLayer(rectangles));

  if (file.fail())
    throw "Failed to read the MPCL chunk";
}

void BinaryMapHandler::ReadTTCI(std::istream& file, Map& map)
{
  EventLog l(__func__);
  throw "Failed to read the TTCI chunk";
}

void BinaryMapHandler::ReadTDCI(std::istream& file, Map& map)
{
  EventLog l(__func__);
  throw "Failed to read the TDCI chunk";
}

void BinaryMapHandler::ReadTPCI(std::istream& file, Map& map)
{
  EventLog l(__func__);
  throw "Failed to read the TPCI chunk";
}

void BinaryMapHandler::ReadANIM(std::istream& file, Map& map)
{
  EventLog l(__func__);
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
    std::vector<int32_t> animframes(data, data + frames);
    delete[] data;

    tiles.push_back(AnimatedTile(animname, delay, (Animation::Type)type, times, animframes));
  }
  map.SetAnimatedTiles(tiles);

  if (file.fail())
    throw "Failed to read the ANIM chunk";
}

void BinaryMapHandler::WriteHEAD(std::ostream& file, const Map& map)
{
  char major = MAJOR;
  char minor = MINOR;
  const std::string& chunk = "HEAD";
  // Header is always 16 bytes
  uint32_t size = htonl(16);

  file.write(chunk.c_str(), sizeof(char) * 4);
  file.write((char*)&size, sizeof(int32_t));
  file.write(&major, sizeof(char));
  file.write(&minor, sizeof(char));
  file.write(magic, sizeof(char) * 14);

  if (file.fail())
    throw "Failed to write the HEAD chunk";
}

void BinaryMapHandler::WriteMAPP(std::ostream& file, const Map& map)
{
  // 8 ints + two strings.
  uint32_t size =
      htonl(8 * sizeof(uint32_t) + (map.GetName().length() + 2 + map.GetFilename().length()) * sizeof(char));
  const std::string& chunk = "MAPP";

  file.write(chunk.c_str(), sizeof(char) * 4);
  file.write((char*)&size, sizeof(uint32_t));

  uint32_t tile_width = htonl(map.GetTileWidth());
  uint32_t tile_height = htonl(map.GetTileHeight());
  uint32_t width = htonl(map.GetWidth());
  uint32_t height = htonl(map.GetHeight());
  uint32_t texture_size = map.GetName().length() + 1;
  uint32_t texture_size_nl = htonl(texture_size);
  uint32_t numlayers = htonl(map.GetNumLayers());
  uint32_t numbackgrounds = htonl(map.GetNumBackgrounds());

  file.write((char*)(&width), sizeof(uint32_t));
  file.write((char*)(&height), sizeof(uint32_t));
  file.write((char*)(&tile_width), sizeof(uint32_t));
  file.write((char*)(&tile_height), sizeof(uint32_t));
  file.write((char*)(&numlayers), sizeof(uint32_t));
  file.write((char*)(&numbackgrounds), sizeof(uint32_t));

  file.write((char*)(&texture_size_nl), sizeof(uint32_t));
  file.write(map.GetName().c_str(), sizeof(char) * texture_size);

  texture_size = map.GetFilename().length() + 1;
  texture_size_nl = htonl(texture_size);
  file.write((char*)(&texture_size_nl), sizeof(uint32_t));
  file.write(map.GetFilename().c_str(), sizeof(char) * texture_size);

  if (file.fail())
    throw "Failed to write the MAPP chunk";
}

void BinaryMapHandler::WriteLYRS(std::ostream& file, const Map& map)
{
  const std::string& chunk = "LYRS";
  uint32_t size = 0;
  uint32_t width = map.GetWidth();
  uint32_t height = map.GetHeight();

  for (uint32_t i = 0; i < map.GetNumLayers(); i++)
  {
    const Layer& layer = map.GetLayer(i);

    size += sizeof(uint32_t);
    size += (layer.GetName().length() + 1) * sizeof(char);
    size += width * height * sizeof(uint32_t);
  }
  size = htonl(size);

  file.write(chunk.c_str(), sizeof(char) * 4);
  file.write((char*)&size, sizeof(uint32_t));

  int32_t texture_size, texture_size_nl;

  int32_t* buffer = new int32_t[width * height];

  for (unsigned int i = 0; i < map.GetNumLayers(); i++)
  {
    const Layer& layer = map.GetLayer(i);
    texture_size = layer.GetName().length() + 1;
    texture_size_nl = htonl(texture_size);
    file.write((char*)(&texture_size_nl), sizeof(int32_t));
    file.write(layer.GetName().c_str(), sizeof(char) * texture_size);
    const std::vector<int32_t>& data = layer.GetData();
    for (uint32_t i = 0; i < width * height; i++) buffer[i] = htonl(data[i]);
    file.write((char*)buffer, sizeof(int32_t) * width * height);
  }

  delete[] buffer;

  if (file.fail())
    throw "Failed to write the LYRS chunk";
}

void BinaryMapHandler::WriteBGDS(std::ostream& file, const Map& map)
{
  uint32_t size = 0;
  const std::string& chunk = "BGDS";

  for (uint32_t i = 0; i < map.GetNumBackgrounds(); i++)
  {
    const Background& back = map.GetBackground(i);
    size += (back.GetName().length() + 1) * sizeof(char);
    size += (back.GetFilename().length() + 1) * sizeof(char);
    size += 5 * sizeof(int32_t);
  }
  size = htonl(size);

  file.write(chunk.c_str(), sizeof(char) * 4);
  file.write((char*)&size, sizeof(int32_t));

  uint32_t texture_size, texture_size_nl;

  for (uint32_t i = 0; i < map.GetNumBackgrounds(); i++)
  {
    const Background& back = map.GetBackground(i);
    int32_t ix, iy;
    uint32_t mode = htonl(back.GetMode());
    back.GetSpeed(ix, iy);

    ix = htonl(ix);
    iy = htonl(iy);

    texture_size = back.GetName().length() + 1;
    texture_size_nl = htonl(texture_size);
    file.write((char*)(&texture_size_nl), sizeof(uint32_t));
    file.write(back.GetName().c_str(), sizeof(char) * texture_size);

    texture_size = back.GetFilename().length() + 1;
    texture_size_nl = htonl(texture_size);
    file.write((char*)(&texture_size_nl), sizeof(uint32_t));
    file.write(back.GetFilename().c_str(), sizeof(char) * texture_size);

    file.write((char*)&mode, sizeof(uint32_t));
    file.write((char*)&ix, sizeof(int32_t));
    file.write((char*)&iy, sizeof(int32_t));
  }

  if (file.fail())
    throw "Failed to write the BGDS chunk";
}

void BinaryMapHandler::WriteMTCL(std::ostream& file, const Map& map)
{
  uint32_t width = map.GetWidth();
  uint32_t height = map.GetHeight();

  uint32_t size = htonl(sizeof(int32_t) * width * height);
  const std::string& chunk = "MTCL";

  file.write(chunk.c_str(), sizeof(char) * 4);
  file.write((char*)&size, sizeof(uint32_t));

  TileBasedCollisionLayer* layer = dynamic_cast<TileBasedCollisionLayer*>(map.GetCollisionLayer());
  int32_t* buffer = new int32_t[width * height];
  std::vector<int32_t>& data = layer->GetData();

  for (uint32_t i = 0; i < width * height; i++) buffer[i] = htonl(data[i]);
  file.write((char*)buffer, sizeof(int32_t) * width * height);

  delete[] buffer;

  if (file.fail())
    throw "Failed to write the MTCL chunk";
}

void BinaryMapHandler::WriteMDCL(std::ostream& file, const Map& map)
{
  uint32_t width = map.GetWidth();
  uint32_t height = map.GetHeight();

  uint32_t size = htonl(sizeof(int32_t) * width * height);
  const std::string& chunk = "MDCL";

  file.write(chunk.c_str(), sizeof(char) * 4);
  file.write((char*)&size, sizeof(uint32_t));

  TileBasedCollisionLayer* layer = dynamic_cast<TileBasedCollisionLayer*>(map.GetCollisionLayer());
  int32_t* buffer = new int32_t[width * height];
  std::vector<int32_t>& data = layer->GetData();

  for (uint32_t i = 0; i < width * height; i++) buffer[i] = htonl(data[i]);
  file.write((char*)buffer, sizeof(int32_t) * width * height);

  delete[] buffer;

  if (file.fail())
    throw "Failed to write the MDCL chunk";
}

void BinaryMapHandler::WriteMPCL(std::ostream& file, const Map& map)
{
  PixelBasedCollisionLayer* layer = dynamic_cast<PixelBasedCollisionLayer*>(map.GetCollisionLayer());
  const Region& region = layer->GetData();
  const std::vector<Rectangle> rectangles = region.GetData();
  uint32_t numrects = region.Size();

  uint32_t size = htonl(sizeof(uint32_t) + sizeof(uint32_t) * 4 * numrects);
  const std::string& chunk = "MPCL";

  file.write(chunk.c_str(), sizeof(char) * 4);
  file.write((char*)&size, sizeof(uint32_t));

  uint32_t num = htonl(numrects);
  file.write((char*)&num, sizeof(uint32_t));

  for (uint32_t i = 0; i < numrects; i++)
  {
    const Rectangle& r = rectangles[i];
    int32_t x = htonl(r.x);
    int32_t y = htonl(r.y);
    uint32_t width = htonl(r.width);
    uint32_t height = htonl(r.height);

    file.write((char*)&x, sizeof(int32_t));
    file.write((char*)&y, sizeof(int32_t));
    file.write((char*)&width, sizeof(uint32_t));
    file.write((char*)&height, sizeof(uint32_t));
  }

  if (file.fail())
    throw "Failed to write the MPCL chunk";
}

void BinaryMapHandler::WriteTTCI(std::ostream& file, const Map& map)
{
  throw "Failed to write the TTCI chunk";
}

void BinaryMapHandler::WriteTDCI(std::ostream& file, const Map& map)
{
  throw "Failed to write the TDCI chunk";
}

void BinaryMapHandler::WriteTPCI(std::ostream& file, const Map& map)
{
  throw "Failed to write the TPCI chunk";
}

void BinaryMapHandler::WriteANIM(std::ostream& file, const Map& map)
{
  uint32_t size = sizeof(int32_t);
  const std::string& chunk = "ANIM";
  for (uint32_t i = 0; i < map.GetNumAnimatedTiles(); i++)
  {
    const AnimatedTile& anim = map.GetAnimatedTile(i);
    size += (anim.GetName().length() + 1) * sizeof(char);
    size += (anim.GetFrames().size() + 1) * sizeof(int32_t);
    size += 4 * sizeof(int32_t);
  }
  size = htonl(size);

  file.write(chunk.c_str(), sizeof(char) * 4);
  file.write((char*)&size, sizeof(uint32_t));

  uint32_t numanimations = map.GetNumAnimatedTiles();
  numanimations = htonl(numanimations);
  file.write((char*)&numanimations, sizeof(uint32_t));

  for (uint32_t i = 0; i < map.GetNumAnimatedTiles(); i++)
  {
    const AnimatedTile& anim = map.GetAnimatedTile(i);
    int32_t delay, type, times;
    uint32_t frames;
    uint32_t texture_size, texture_size_nl;

    texture_size = anim.GetName().length() + 1;
    texture_size_nl = htonl(texture_size);
    file.write((char*)(&texture_size_nl), sizeof(uint32_t));
    file.write(anim.GetName().c_str(), sizeof(char) * texture_size);

    delay = htonl(anim.GetDelay());
    type = htonl(anim.GetType());
    times = htonl(anim.GetTimes());
    frames = htonl(anim.GetNumFrames());

    file.write((char*)&delay, sizeof(int32_t));
    file.write((char*)&type, sizeof(int32_t));
    file.write((char*)&times, sizeof(int32_t));
    file.write((char*)&frames, sizeof(uint32_t));

    frames = anim.GetNumFrames();
    int32_t* data = new int32_t[frames];
    copy(anim.GetFrames().begin(), anim.GetFrames().end(), data);
    for (uint32_t i = 0; i < frames; i++) data[i] = htonl(data[i]);
    file.write((char*)data, sizeof(int32_t) * frames);
    delete[] data;
  }

  if (file.fail())
    throw "Failed to write the ANIM chunk";
}
