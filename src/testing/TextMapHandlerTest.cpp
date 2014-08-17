/******************************************************************************************************
 * Morphan
 * Copyright (C) 2014 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
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

#if !defined( WIN32 )
    #define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MAIN
#include <sstream>
#include <boost/test/auto_unit_test.hpp>
#include <wx/log.h>
#include "Map.hpp"
#include "TextMapHandler.hpp"
#include "TileBasedCollisionLayer.hpp"

const char* file_data =
"Properties\n"
"name: HELLO\n"
"tileset: 011-PortTown01.png\n"
"tile_dimensions: 32 32\n"
"\n"
"Layers\n"
"name: A\n"
"position: 32 24\n"
"origin: 10 12\n"
"scale: 3 5\n"
"rotation: 92\n"
"opacity: 50\n"
"blend_mode: 0\n"
"blend_color: FEFDFCFA\n"
"dimensions: 2 2\n"
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

static inline void trim(std::string& str)
{
    std::string::size_type pos = str.find_last_not_of(' ');
    if(pos != std::string::npos) {
        str.erase(pos + 1);
        pos = str.find_first_not_of(' ');
        if(pos != std::string::npos) str.erase(0, pos);
    }
    else
        str.erase(str.begin(), str.end());
}

struct TextMapHandlerTest
{
    TextMapHandlerTest() {wxLog::SetVerbose(true);}
    ~TextMapHandlerTest() {}
    TextMapHandler handler;
    Map map;
};

BOOST_FIXTURE_TEST_CASE(TestLoad, TextMapHandlerTest)
{
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

    BOOST_REQUIRE_EQUAL(map.GetName(), "HELLO");
    BOOST_REQUIRE_EQUAL(map.GetFilename(), "011-PortTown01.png");
    BOOST_REQUIRE_EQUAL(map.GetTileWidth(), 32);
    BOOST_REQUIRE_EQUAL(map.GetTileHeight(), 32);
    BOOST_REQUIRE_EQUAL(map.GetNumLayers(), 1);
    BOOST_REQUIRE_EQUAL(map.GetNumAnimatedTiles(), 2);
    BOOST_REQUIRE_EQUAL(map.GetNumBackgrounds(), 1);
    BOOST_REQUIRE(map.HasCollisionLayer());

    Layer& layer = map.GetLayer(0);
    DrawAttributes* attr = dynamic_cast<DrawAttributes*>(&layer);
    std::vector<int32_t> actualData = layer.GetData();
    std::vector<int32_t> expectedData = {50, 70, 70, 60};

    BOOST_REQUIRE_EQUAL(layer.GetName(), "A");
    BOOST_REQUIRE_EQUAL(layer.GetWidth(), 2);
    BOOST_REQUIRE_EQUAL(layer.GetHeight(), 2);
    BOOST_REQUIRE_EQUAL_COLLECTIONS(actualData.begin(), actualData.end(), expectedData.begin(), expectedData.end());
    BOOST_REQUIRE_EQUAL(attr->GetX(), 32);
    BOOST_REQUIRE_EQUAL(attr->GetY(), 24);
    BOOST_REQUIRE_EQUAL(attr->GetOriginX(), 10);
    BOOST_REQUIRE_EQUAL(attr->GetOriginY(), 12);
    BOOST_REQUIRE_EQUAL(attr->GetScaleX(), 3.0f);
    BOOST_REQUIRE_EQUAL(attr->GetScaleY(), 5.0f);
    BOOST_REQUIRE_EQUAL(attr->GetRotation(), 92.0f);
    BOOST_REQUIRE_EQUAL(attr->GetOpacity(), 50.0f);
    BOOST_REQUIRE_EQUAL(attr->GetBlendMode(), 0);
    BOOST_REQUIRE_EQUAL(attr->GetBlendColor(), (uint32_t)0xFEFDFCFA);

    Background& background = map.GetBackground(0);
    attr = dynamic_cast<DrawAttributes*>(&background);
    BOOST_REQUIRE_EQUAL(background.GetName(), "B");
    BOOST_REQUIRE_EQUAL(background.GetFilename(), "003-StarlitSky01.png");
    BOOST_REQUIRE_EQUAL(background.GetMode(), 6);
    BOOST_REQUIRE_EQUAL(background.GetSpeedX(), 2);
    BOOST_REQUIRE_EQUAL(background.GetSpeedY(), 4);
    BOOST_REQUIRE_EQUAL(attr->GetX(), 1);
    BOOST_REQUIRE_EQUAL(attr->GetY(), 2);
    BOOST_REQUIRE_EQUAL(attr->GetOriginX(), 3);
    BOOST_REQUIRE_EQUAL(attr->GetOriginY(), 4);
    BOOST_REQUIRE_EQUAL(attr->GetScaleX(), 5.0f);
    BOOST_REQUIRE_EQUAL(attr->GetScaleY(), 6.0f);
    BOOST_REQUIRE_EQUAL(attr->GetRotation(), 7.0f);
    BOOST_REQUIRE_EQUAL(attr->GetOpacity(), 8.0f);
    BOOST_REQUIRE_EQUAL(attr->GetBlendMode(), 9);
    BOOST_REQUIRE_EQUAL(attr->GetBlendColor(), (uint32_t)0x0A0B0C0D);

    AnimatedTile& at1 = map.GetAnimatedTile(0);
    AnimatedTile& at2 = map.GetAnimatedTile(1);
    const std::vector<int32_t>& actualFramesAt1 = at1.GetFrames();
    const std::vector<int32_t>& actualFramesAt2 = at2.GetFrames();
    std::vector<int32_t> expectedFramesAt1 = {0, 1, 2, 3, 4, 5};
    std::vector<int32_t> expectedFramesAt2 = {61, 62, 63, 64, 65};
    BOOST_REQUIRE_EQUAL(at1.GetName(), "Water");
    BOOST_REQUIRE_EQUAL(at1.GetDelay(), 3);
    BOOST_REQUIRE_EQUAL(at1.GetType(), 0);
    BOOST_REQUIRE_EQUAL(at1.GetTimes(), -1);
    BOOST_REQUIRE_EQUAL_COLLECTIONS(actualFramesAt1.begin(), actualFramesAt1.end(), expectedFramesAt1.begin(), expectedFramesAt1.end());
    BOOST_REQUIRE_EQUAL(at2.GetName(), "Waterfall");
    BOOST_REQUIRE_EQUAL(at2.GetDelay(), 1);
    BOOST_REQUIRE_EQUAL(at2.GetType(), 1);
    BOOST_REQUIRE_EQUAL(at2.GetTimes(), -1);
    BOOST_REQUIRE_EQUAL_COLLECTIONS(actualFramesAt2.begin(), actualFramesAt2.end(), expectedFramesAt2.begin(), expectedFramesAt2.end());

    TileBasedCollisionLayer* clayer = dynamic_cast<TileBasedCollisionLayer*>(map.GetCollisionLayer());
    actualData = clayer->GetData();
    expectedData = {1, 0, 1, 1};
    BOOST_REQUIRE_EQUAL(clayer->GetWidth(), 2);
    BOOST_REQUIRE_EQUAL(clayer->GetHeight(), 2);
    BOOST_REQUIRE_EQUAL_COLLECTIONS(actualData.begin(), actualData.end(), expectedData.begin(), expectedData.end());
}

BOOST_FIXTURE_TEST_CASE(TestSave, TextMapHandlerTest)
{
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

        BOOST_REQUIRE_EQUAL(expectedLine, actualLine);
    }
}
