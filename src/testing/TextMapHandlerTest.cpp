/******************************************************************************************************
 * TileMapEditor
 * Copyright (C) 2015 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
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
#define BOOST_TEST_DYN_LINK
#include <boost/test/auto_unit_test.hpp>
#include <sstream>
#include "Logger.hpp"
#include "Map.hpp"
#include "TestUtil.hpp"
#include "TextMapHandler.hpp"
#include "TileBasedCollisionLayer.hpp"

const char* map_data_txt_file =
    "Properties\n"
    "name: HELLO WORLD\n"
    "tileset: 011-PortTown01.png\n"
    "tile_dimensions: 32 32\n"
    "\n"
    "Layers\n"
    "name: A\n"
    "dimensions: 2 2\n"
    "position: 32 24\n"
    "origin: 10 12\n"
    "scale: 3 5\n"
    "rotation: 92\n"
    "opacity: 50\n"
    "blend_mode: 0\n"
    "blend_color: FEFDFCFA\n"
    "priority: 0\n"
    "data: 50 70\n"
    "data: 70 60\n"
    "\n"
    "\n"
    "Backgrounds\n"
    "name: B\n"
    "filename: 003-StarlitSky01.png\n"
    "mode: 6\n"
    "speed: 2 4\n"
    "position: 1 2\n"
    "origin: 3 4\n"
    "scale: 5 6\n"
    "rotation: 7\n"
    "opacity: 8\n"
    "blend_mode: 9\n"
    "blend_color: A0B0C0D\n"
    "priority: 1\n"
    "\n"
    "\n"
    "Animations\n"
    "name: Water\n"
    "delay: 3\n"
    "type: 0\n"
    "times: -1\n"
    "frames: 0 1 2 3 4 5\n"
    "\n"
    "name: Waterfall\n"
    "delay: 1\n"
    "type: 1\n"
    "times: -1\n"
    "frames: 61 62 63 64 65\n"
    "\n"
    "\n"
    "Collision\n"
    "type: 0\n"
    "dimensions: 2 2\n"
    "data: 1 0\n"
    "data: 1 1\n"
    "\n";

BOOST_AUTO_TEST_CASE(TextMapHandlerLoad)
{
    TextMapHandler handler;
    Map map;

    std::stringstream file(map_data_txt_file);
    try
    {
        handler.Load(file, map);
    }
    catch (const char* s)
    {
        BOOST_FAIL(s);
        return;
    }

    BOOST_CHECK_EQUAL(map.GetName(), "HELLO WORLD");
    const Tileset& tileset = map.GetTileset();
    uint32_t tile_width, tile_height;
    tileset.GetTileDimensions(tile_width, tile_height);
    const std::vector<AnimatedTile>& animated_tiles = tileset.GetAnimatedTiles();
    BOOST_CHECK_EQUAL(tileset.GetFilename(), "011-PortTown01.png");
    BOOST_CHECK_EQUAL(tile_width, 32);
    BOOST_CHECK_EQUAL(tile_height, 32);
    BOOST_CHECK_EQUAL(animated_tiles.size(), 2);

    BOOST_CHECK_EQUAL(map.GetNumLayers(), 1);
    BOOST_CHECK_EQUAL(map.GetNumBackgrounds(), 1);
    BOOST_REQUIRE(map.HasCollisionLayer());

    Layer& layer = map.GetLayer(0);
    DrawAttributes* attr = dynamic_cast<DrawAttributes*>(&layer);
    std::vector<int32_t> actualData = layer.GetData();
    std::vector<int32_t> expectedData = {50, 70, 70, 60};

    BOOST_CHECK_EQUAL(layer.GetName(), "A");
    BOOST_CHECK_EQUAL(layer.GetWidth(), 2);
    BOOST_CHECK_EQUAL(layer.GetHeight(), 2);
    BOOST_CHECK_EQUAL_COLLECTIONS(actualData.begin(), actualData.end(), expectedData.begin(), expectedData.end());

    int32_t x, y;
    int32_t ox, oy;
    float sx, sy;
    attr->GetPosition(x, y);
    attr->GetOrigin(ox, oy);
    attr->GetScale(sx, sy);
    BOOST_CHECK_EQUAL(x, 32);
    BOOST_CHECK_EQUAL(y, 24);
    BOOST_CHECK_EQUAL(ox, 10);
    BOOST_CHECK_EQUAL(oy, 12);
    BOOST_CHECK_EQUAL(sx, 3.0f);
    BOOST_CHECK_EQUAL(sy, 5.0f);
    BOOST_CHECK_EQUAL(attr->GetRotation(), 92.0f);
    BOOST_CHECK_EQUAL(attr->GetOpacity(), 50.0f);
    BOOST_CHECK_EQUAL(attr->GetBlendMode(), 0);
    BOOST_CHECK_EQUAL(attr->GetBlendColor(), (uint32_t)0xFEFDFCFA);
    BOOST_CHECK_EQUAL(attr->GetDepth(), 0);

    Background& background = map.GetBackground(0);
    attr = dynamic_cast<DrawAttributes*>(&background);
    BOOST_CHECK_EQUAL(background.GetName(), "B");
    BOOST_CHECK_EQUAL(background.GetFilename(), "003-StarlitSky01.png");
    BOOST_CHECK_EQUAL(background.GetMode(), 6);
    background.GetSpeed(sx, sy);
    BOOST_CHECK_EQUAL(sx, 2);
    BOOST_CHECK_EQUAL(sy, 4);
    attr->GetPosition(x, y);
    attr->GetOrigin(ox, oy);
    attr->GetScale(sx, sy);
    BOOST_CHECK_EQUAL(x, 1);
    BOOST_CHECK_EQUAL(y, 2);
    BOOST_CHECK_EQUAL(ox, 3);
    BOOST_CHECK_EQUAL(oy, 4);
    BOOST_CHECK_EQUAL(sx, 5.0f);
    BOOST_CHECK_EQUAL(sy, 6.0f);
    BOOST_CHECK_EQUAL(attr->GetRotation(), 7.0f);
    BOOST_CHECK_EQUAL(attr->GetOpacity(), 8.0f);
    BOOST_CHECK_EQUAL(attr->GetBlendMode(), 9);
    BOOST_CHECK_EQUAL(attr->GetBlendColor(), (uint32_t)0x0A0B0C0D);
    BOOST_CHECK_EQUAL(attr->GetDepth(), 1);

    const AnimatedTile& at1 = animated_tiles[0];
    const AnimatedTile& at2 = animated_tiles[1];
    const std::vector<int32_t>& actualFramesAt1 = at1.GetFrames();
    const std::vector<int32_t>& actualFramesAt2 = at2.GetFrames();
    std::vector<int32_t> expectedFramesAt1 = {0, 1, 2, 3, 4, 5};
    std::vector<int32_t> expectedFramesAt2 = {61, 62, 63, 64, 65};
    BOOST_CHECK_EQUAL(at1.GetName(), "Water");
    BOOST_CHECK_EQUAL(at1.GetDelay(), 3);
    BOOST_CHECK_EQUAL(at1.GetType(), 0);
    BOOST_CHECK_EQUAL(at1.GetTimes(), -1);
    BOOST_CHECK_EQUAL_COLLECTIONS(actualFramesAt1.begin(), actualFramesAt1.end(), expectedFramesAt1.begin(),
                                  expectedFramesAt1.end());
    BOOST_CHECK_EQUAL(at2.GetName(), "Waterfall");
    BOOST_CHECK_EQUAL(at2.GetDelay(), 1);
    BOOST_CHECK_EQUAL(at2.GetType(), 1);
    BOOST_CHECK_EQUAL(at2.GetTimes(), -1);
    BOOST_CHECK_EQUAL_COLLECTIONS(actualFramesAt2.begin(), actualFramesAt2.end(), expectedFramesAt2.begin(),
                                  expectedFramesAt2.end());

    TileBasedCollisionLayer* clayer = dynamic_cast<TileBasedCollisionLayer*>(map.GetCollisionLayer());
    actualData = clayer->GetData();
    expectedData = {1, 0, 1, 1};
    BOOST_CHECK_EQUAL(clayer->GetWidth(), 2);
    BOOST_CHECK_EQUAL(clayer->GetHeight(), 2);
    BOOST_CHECK_EQUAL_COLLECTIONS(actualData.begin(), actualData.end(), expectedData.begin(), expectedData.end());
}

BOOST_AUTO_TEST_CASE(TextMapHandlerSave)
{
    TextMapHandler handler;
    Map map;

    std::stringstream file(map_data_txt_file);
    std::stringstream out;
    try
    {
        handler.Load(file, map);
        handler.Save(out, map);
    }
    catch (const char* s)
    {
        BOOST_FAIL(s);
        return;
    }

    std::stringstream expected(map_data_txt_file);

    std::string expectedLine;
    std::string actualLine;
    while (expected && out)
    {
        std::getline(expected, expectedLine);
        std::getline(out, actualLine);
        trim(actualLine);
        trim(expectedLine);

        BOOST_CHECK_EQUAL(expectedLine, actualLine);
    }
}
