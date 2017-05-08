//#define BOOST_TEST_MODULE Map
#include <boost/test/auto_unit_test.hpp>
#include "Map.hpp"
#include "MapHandlerManager.hpp"
#include "TileBasedCollisionLayer.hpp"

struct MapTest
{
    Map map;

    MapTest()
    {
        MapHandlerManager().Load("test.map", map);
    }

    ~MapTest()
    {
    }
};

BOOST_FIXTURE_TEST_CASE(TestMapLoad, MapTest)
{
    BOOST_CHECK_EQUAL(map.GetWidth(), 5);
    BOOST_CHECK_EQUAL(map.GetHeight(), 5);
    BOOST_CHECK_EQUAL(map.GetName(), "My Map");
    BOOST_CHECK_EQUAL(map.GetNumLayers(),  1);
    BOOST_CHECK_EQUAL(map.GetNumBackgrounds(), 0);


    const Tileset& tileset = map.GetTileset();
    uint32_t tile_width, tile_height;
    tileset.GetTileDimensions(tile_width, tile_height);
    BOOST_CHECK_EQUAL(tileset.GetFilename(), "011-PortTown01.png");
    BOOST_CHECK_EQUAL(tile_width, 32);
    BOOST_CHECK_EQUAL(tile_height, 32);
}

BOOST_FIXTURE_TEST_CASE(TestMapSaveBinary, MapTest)
{
    Map map2;
    const Tileset& tileset = map2.GetTileset();
    uint32_t tile_width, tile_height;
    tileset.GetTileDimensions(tile_width, tile_height);

    MapHandlerManager().Save("testout.map", map);
    MapHandlerManager().Load("testout.map", map2);

    BOOST_CHECK_EQUAL(map2.GetWidth(), 5);
    BOOST_CHECK_EQUAL(map2.GetHeight(), 5);
    BOOST_CHECK_EQUAL(map2.GetName(), "My Map");
    BOOST_CHECK_EQUAL(map2.GetNumLayers(), 1);
    BOOST_CHECK_EQUAL(map2.GetNumBackgrounds(), 0);
    BOOST_CHECK_EQUAL(tileset.GetFilename(), "011-PortTown01.png");
    BOOST_CHECK_EQUAL(tile_width, 32);
    BOOST_CHECK_EQUAL(tile_height, 32);
}


BOOST_FIXTURE_TEST_CASE(TestMapSaveText, MapTest)
{
    Map map2;

    MapHandlerManager().Save("testout.txt", map);
    MapHandlerManager().Load("testout.txt", map2);

    BOOST_CHECK_EQUAL(map2.GetWidth(), 5);
    BOOST_CHECK_EQUAL(map2.GetHeight(), 5);
    BOOST_CHECK_EQUAL(map2.GetName(), "My Map");
    BOOST_CHECK_EQUAL(map2.GetNumLayers(), 1);
    BOOST_CHECK_EQUAL(map2.GetNumBackgrounds(), 0);


    const Tileset& tileset = map2.GetTileset();
    uint32_t tile_width, tile_height;
    tileset.GetTileDimensions(tile_width, tile_height);
    BOOST_CHECK_EQUAL(tileset.GetFilename(), "011-PortTown01.png");
    BOOST_CHECK_EQUAL(tile_width, 32);
    BOOST_CHECK_EQUAL(tile_height, 32);
}

BOOST_FIXTURE_TEST_CASE(TestMapSaveTextComplete, MapTest)
{
    int layer[25] = {-1, -1, -1, 11, -1, -1, 6, -1, -1, -1, -1, -1, -1, -1, 7, 7, -1, -1, -1, 15, 15, -1, -1, 13, -1};
    int collision[25] = {-1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, -1, 0, -1};

    Map map2;


    MapHandlerManager().Load("complete.txt", map2);
    BOOST_CHECK_EQUAL(map2.GetWidth(), 5);
    BOOST_CHECK_EQUAL(map2.GetHeight(), 5);
    BOOST_CHECK_EQUAL(map2.GetName(), "My Map");

    const Tileset& tileset = map2.GetTileset();
    uint32_t tile_width, tile_height;
    tileset.GetTileDimensions(tile_width, tile_height);
    BOOST_CHECK_EQUAL(tileset.GetFilename(), "011-PortTown01.png");
    BOOST_CHECK_EQUAL(tile_width, 32);
    BOOST_CHECK_EQUAL(tile_height, 32);


    BOOST_REQUIRE_EQUAL(map2.GetNumLayers(), 2);
    BOOST_CHECK_EQUAL(map2.GetLayer(1).GetName(), "Layer 1");
    const std::vector<int32_t>& layerdata = map2.GetLayer(1).GetData();
    BOOST_CHECK_EQUAL_COLLECTIONS(layerdata.begin(), layerdata.end(), layer, layer + 25);

    BOOST_REQUIRE_EQUAL(map2.GetNumBackgrounds(), 1);
    BOOST_CHECK_EQUAL(map2.GetBackground(0).GetName(), "Background 0");
    BOOST_CHECK_EQUAL(map2.GetBackground(0).GetFilename(), "003-StarlitSky01.png");
    BOOST_CHECK_EQUAL(map2.GetBackground(0).GetMode(), 6);

    BOOST_REQUIRE(map2.HasCollisionLayer());
    const std::vector<int32_t>& clayerdata = dynamic_cast<TileBasedCollisionLayer*>(map2.GetCollisionLayer())->GetData();
    BOOST_CHECK_EQUAL_COLLECTIONS(clayerdata.begin(), clayerdata.end(), collision, collision + 25);
}

BOOST_FIXTURE_TEST_CASE(TestMapSaveXml, MapTest)
{
    Map map2;

    MapHandlerManager().Save("testout.xml", map);
    MapHandlerManager().Load("testout.xml", map2);

    BOOST_CHECK_EQUAL(map2.GetWidth(), 5);
    BOOST_CHECK_EQUAL(map2.GetHeight(), 5);
    BOOST_CHECK_EQUAL(map2.GetName(), "My Map");
    BOOST_CHECK_EQUAL(map2.GetNumLayers(), 1);
    BOOST_CHECK_EQUAL(map2.GetNumBackgrounds(), 0);

    const Tileset& tileset = map2.GetTileset();
    uint32_t tile_width, tile_height;
    tileset.GetTileDimensions(tile_width, tile_height);
    BOOST_CHECK_EQUAL(tileset.GetFilename(), "011-PortTown01.png");
    BOOST_CHECK_EQUAL(tile_width, 32);
    BOOST_CHECK_EQUAL(tile_height, 32);
}

BOOST_FIXTURE_TEST_CASE(TestMapSaveXmlComplete, MapTest)
{
    int expected_layer[25] = {-1, -1, -1, 11, -1, -1, 6, -1, -1, -1, -1, -1, -1, -1, 7, 7, -1, -1, -1, 15, 15, -1, -1, 13, -1};
    int collision[25] = {-1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, -1, 0, -1};

    Map map2;
    MapHandlerManager().Load("complete.xml", map2);

    BOOST_CHECK_EQUAL(map2.GetWidth(), 5);
    BOOST_CHECK_EQUAL(map2.GetHeight(), 5);
    BOOST_CHECK_EQUAL(map2.GetName(), "My Map");
    BOOST_CHECK_EQUAL(map2.GetNumLayers(), 1);
    BOOST_CHECK_EQUAL(map2.GetNumBackgrounds(), 0);

    const Tileset& tileset = map2.GetTileset();
    uint32_t tile_width, tile_height;
    tileset.GetTileDimensions(tile_width, tile_height);
    BOOST_CHECK_EQUAL(tileset.GetFilename(), "011-PortTown01.png");
    BOOST_CHECK_EQUAL(tile_width, 32);
    BOOST_CHECK_EQUAL(tile_height, 32);

    BOOST_CHECK_EQUAL(map2.GetNumLayers(), 2);
    const Layer& layer = map2.GetLayer(1);
    BOOST_CHECK_EQUAL(layer.GetName(), "Layer 1");
    const std::vector<int32_t>& layerdata = layer.GetData();
    BOOST_CHECK_EQUAL_COLLECTIONS(layerdata.begin(), layerdata.end(), expected_layer, expected_layer + 25);

    const Background& background = map2.GetBackground(0);
    BOOST_CHECK_EQUAL(map2.GetNumBackgrounds(), 1);
    BOOST_CHECK_EQUAL(background.GetName(), "Background 0");
    BOOST_CHECK_EQUAL(background.GetFilename(), "003-StarlitSky01.png");
    BOOST_CHECK_EQUAL(background.GetMode(), 6);

    BOOST_REQUIRE(map2.HasCollisionLayer());
    const std::vector<int32_t>& clayerdata = dynamic_cast<TileBasedCollisionLayer*>(map2.GetCollisionLayer())->GetData();
    BOOST_CHECK_EQUAL_COLLECTIONS(clayerdata.begin(), clayerdata.end(), collision, collision + 25);
}

BOOST_FIXTURE_TEST_CASE(TestMapResizeCopy, MapTest)
{
    map.Resize(1, 1, true);
    const Layer& layer1 = map.GetLayer(0);
    BOOST_CHECK_EQUAL(layer1.GetWidth(), 1);
    BOOST_CHECK_EQUAL(layer1.GetHeight(), 1);
    BOOST_CHECK_EQUAL(layer1[0], 0);
}

BOOST_FIXTURE_TEST_CASE(TestMapResizeClear, MapTest)
{
    map.Resize(10, 20, false);
    const Layer& layer1 = map.GetLayer(0);
    BOOST_CHECK_EQUAL(layer1.GetWidth(), 10);
    BOOST_CHECK_EQUAL(layer1.GetHeight(), 20);
    BOOST_CHECK_EQUAL(layer1[55], -1);
}

BOOST_FIXTURE_TEST_CASE(TestMapSaveBackground, MapTest)
{

    Map map2;
    map.Add(Background("The Sky", "003-StarlitSky01.png",
                       (Background::ScrollMode::Autoscroll | Background::DrawMode::Repeating), 3, 20));

    MapHandlerManager().Save("testout3.map", map);
    MapHandlerManager().Load("testout3.map", map2);

    BOOST_CHECK_EQUAL(map2.GetWidth(), 5);
    BOOST_CHECK_EQUAL(map2.GetHeight(), 5);
    BOOST_CHECK_EQUAL(map2.GetName(), "My Map");

    const Tileset& tileset = map2.GetTileset();
    uint32_t tile_width, tile_height;
    tileset.GetTileDimensions(tile_width, tile_height);
    BOOST_CHECK_EQUAL(tileset.GetFilename(), "011-PortTown01.png");
    BOOST_CHECK_EQUAL(tile_width, 32);
    BOOST_CHECK_EQUAL(tile_height, 32);

    BOOST_CHECK_EQUAL(map2.GetNumLayers(), 1);

    BOOST_REQUIRE_EQUAL(map2.GetNumBackgrounds(), 1);
    Background back = map2.GetBackground(0);
    BOOST_CHECK_EQUAL(back.GetName(), "The Sky");
    BOOST_CHECK_EQUAL(back.GetFilename(), "003-StarlitSky01.png");
    BOOST_CHECK_EQUAL(back.GetMode(), (Background::ScrollMode::Autoscroll | Background::DrawMode::Repeating));

    float sx, sy;
    back.GetSpeed(sx, sy);
    BOOST_CHECK_EQUAL(sx, 3);
    BOOST_CHECK_EQUAL(sy, 20);
}
