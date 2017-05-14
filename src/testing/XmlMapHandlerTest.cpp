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
#include <sstream>
#include <boost/test/auto_unit_test.hpp>
#include "Logger.hpp"
#include "Map.hpp"
#include "TestUtil.hpp"
#include "TileBasedCollisionLayer.hpp"
#include "XmlMapHandler.hpp"

const char* file_data =
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Map>\n"
    "   <Properties>\n"
    "       <Name>HELLO WORLD</Name>\n"
    "       <Tileset>011-PortTown01.png</Tileset>\n"
    "       <TileDimensions>32, 32</TileDimensions>\n"
    "   </Properties>\n"
    "   <Layer>\n"
    "       <Name>A</Name>\n"
    "       <Position>32, 24</Position>\n"
    "       <Origin>10, 12</Origin>\n"
    "       <Scale>3.000000, 5.000000</Scale>\n"
    "       <Rotation>92.000000</Rotation>\n"
    "       <Opacity>50.000000</Opacity>\n"
    "       <BlendMode>0</BlendMode>\n"
    "       <BlendColor>FEFDFCFA</BlendColor>\n"
    "       <Priority>0</Priority>\n"
    "       <Dimensions>2, 2</Dimensions>\n"
    "       <Data>\n"
    "           50, 70,\n"
    "           70, 60,\n"
    "       </Data>\n"
    "   </Layer>\n"
    "   <Background>\n"
    "       <Name>B</Name>\n"
    "       <Filename>003-StarlitSky01.png</Filename>\n"
    "       <Mode>6</Mode>\n"
    "       <Speed>2, 4</Speed>\n"
    "       <Position>1, 2</Position>\n"
    "       <Origin>3, 4</Origin>\n"
    "       <Scale>5.000000, 6.000000</Scale>\n"
    "       <Rotation>7.000000</Rotation>\n"
    "       <Opacity>8.000000</Opacity>\n"
    "       <BlendMode>9</BlendMode>\n"
    "       <BlendColor>A0B0C0D</BlendColor>\n"
    "       <Priority>1</Priority>\n"
    "   </Background>\n"
    "   <Animation>\n"
    "       <Name>Water</Name>\n"
    "       <Delay>3</Delay>\n"
    "       <Type>0</Type>\n"
    "       <Times>-1</Times>\n"
    "       <Frames>\n"
    "           0, 1, 2, 3, 4, 5,\n"
    "       </Frames>\n"
    "   </Animation>\n"
    "   <Animation>\n"
    "       <Name>Waterfall</Name>\n"
    "       <Delay>1</Delay>\n"
    "       <Type>1</Type>\n"
    "       <Times>-1</Times>\n"
    "       <Frames>\n"
    "           61, 62, 63, 64, 65,\n"
    "       </Frames>\n"
    "   </Animation>\n"
    "   <Collision>\n"
    "       <Type>0</Type>\n"
    "       <Dimensions>2, 2</Dimensions>\n"
    "       <Data>\n"
    "           1, 0,\n"
    "           1, 1,\n"
    "       </Data>\n"
    "   </Collision>\n"
    "</Map>\n";

BOOST_AUTO_TEST_CASE(XmlMapHandlerTestLoad)
{
    XmlMapHandler handler;
    Map map;

    std::stringstream file(file_data);
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
    BOOST_CHECK_EQUAL(map.GetNumLayers(), 1);
    BOOST_CHECK_EQUAL(map.GetNumBackgrounds(), 1);
    const Tileset& tileset = map.GetTileset();
    uint32_t tile_width, tile_height;
    tileset.GetTileDimensions(tile_width, tile_height);
    BOOST_CHECK_EQUAL(tileset.GetFilename(), "011-PortTown01.png");
    BOOST_CHECK_EQUAL(tile_width, 32);
    BOOST_CHECK_EQUAL(tile_height, 32);

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

    const std::vector<AnimatedTile>& animated_tiles = tileset.GetAnimatedTiles();
    BOOST_CHECK_EQUAL(animated_tiles.size(), 2);
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
    BOOST_CHECK_EQUAL_COLLECTIONS(actualFramesAt1.begin(), actualFramesAt1.end(), expectedFramesAt1.begin(), expectedFramesAt1.end());
    BOOST_CHECK_EQUAL(at2.GetName(), "Waterfall");
    BOOST_CHECK_EQUAL(at2.GetDelay(), 1);
    BOOST_CHECK_EQUAL(at2.GetType(), 1);
    BOOST_CHECK_EQUAL(at2.GetTimes(), -1);
    BOOST_CHECK_EQUAL_COLLECTIONS(actualFramesAt2.begin(), actualFramesAt2.end(), expectedFramesAt2.begin(), expectedFramesAt2.end());

    BOOST_REQUIRE(map.HasCollisionLayer());
    TileBasedCollisionLayer* clayer = dynamic_cast<TileBasedCollisionLayer*>(map.GetCollisionLayer());
    actualData = clayer->GetData();
    expectedData = {1, 0, 1, 1};
    BOOST_CHECK_EQUAL(clayer->GetWidth(), 2);
    BOOST_CHECK_EQUAL(clayer->GetHeight(), 2);
    BOOST_CHECK_EQUAL_COLLECTIONS(actualData.begin(), actualData.end(), expectedData.begin(), expectedData.end());
}

BOOST_AUTO_TEST_CASE(XmlMapHandlerTestSave)
{
    XmlMapHandler handler;
    Map map;

    std::stringstream file(file_data);
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

    std::stringstream expected(file_data);

    std::string expectedLine;
    std::string actualLine;
    while (expected && out)
    {
        std::getline(expected, expectedLine);
        std::getline(out, actualLine);
        trim(actualLine);
        trim(expectedLine);

        //BOOST_CHECK_EQUAL(expectedLine, actualLine);
    }
}
