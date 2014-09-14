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
        map.Destroy();
    }
};

BOOST_FIXTURE_TEST_CASE(TestMapLoad, MapTest)
{
    BOOST_CHECK_EQUAL(map.GetTileWidth(), 32);
    BOOST_CHECK_EQUAL(map.GetTileHeight(), 32);
    BOOST_CHECK_EQUAL(map.GetWidth(), 5);
    BOOST_CHECK_EQUAL(map.GetHeight(), 5);
    BOOST_CHECK_EQUAL(map.GetName(), "My Map");
    BOOST_CHECK_EQUAL(map.GetFilename(), "011-PortTown01.png");
    BOOST_CHECK(map.GetNumLayers() == 1);
    BOOST_CHECK(map.GetNumBackgrounds() == 0);
}

BOOST_FIXTURE_TEST_CASE(TestMapSaveBinary, MapTest)
{
    Map map2;

    MapHandlerManager().Save("testout.map", map);
    MapHandlerManager().Load("testout.map", map2);

    BOOST_CHECK_EQUAL(map2.GetTileWidth(), 32);
    BOOST_CHECK_EQUAL(map2.GetTileHeight(), 32);
    BOOST_CHECK_EQUAL(map2.GetWidth(), 5);
    BOOST_CHECK_EQUAL(map2.GetHeight(), 5);
    BOOST_CHECK_EQUAL(map2.GetName(), "My Map");
    BOOST_CHECK_EQUAL(map2.GetFilename(), "011-PortTown01.png");
    BOOST_CHECK(map2.GetNumLayers() == 1);
    BOOST_CHECK(map2.GetNumBackgrounds() == 0);
}

// Daniel
BOOST_FIXTURE_TEST_CASE(TestMapSaveText, MapTest)
{
    Map map2;

    MapHandlerManager().Save("testout.txt", map);
    MapHandlerManager().Load("testout.txt", map2);

    BOOST_CHECK_EQUAL(map2.GetTileWidth(), 32);
    BOOST_CHECK_EQUAL(map2.GetTileHeight(), 32);
    BOOST_CHECK_EQUAL(map2.GetWidth(), 5);
    BOOST_CHECK_EQUAL(map2.GetHeight(), 5);
    BOOST_CHECK_EQUAL(map2.GetName(), "My Map");
    BOOST_CHECK_EQUAL(map2.GetFilename(), "011-PortTown01.png");
    BOOST_CHECK(map2.GetNumLayers() == 1);
    BOOST_CHECK(map2.GetNumBackgrounds() == 0);

    map2.Destroy();
    int layer[25] = {-1, -1, -1, 11, -1, -1, 6, -1, -1, -1, -1, -1, -1, -1, 7, 7, -1, -1, -1, 15, 15, -1, -1, 13, -1};
    int collision[25] = {-1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, -1, 0, -1};

    MapHandlerManager().Load("complete.txt", map2);
    BOOST_CHECK_EQUAL(map2.GetTileWidth(), 32);
    BOOST_CHECK_EQUAL(map2.GetTileHeight(), 32);
    BOOST_CHECK_EQUAL(map2.GetWidth(), 5);
    BOOST_CHECK_EQUAL(map2.GetHeight(), 5);
    BOOST_CHECK_EQUAL(map2.GetName(), "My Map");
    BOOST_CHECK_EQUAL(map2.GetFilename(), "011-PortTown01.png");

    BOOST_CHECK(map2.GetNumLayers() == 2);
    BOOST_CHECK_EQUAL(map2.GetLayer(1).GetName(), "Layer 1");
    const std::vector<int32_t>& layerdata = map2.GetLayer(1).GetData();
    BOOST_CHECK_EQUAL_COLLECTIONS(layerdata.begin(), layerdata.end(), layer, layer + 25);

    BOOST_CHECK(map2.GetNumBackgrounds() == 1);
    BOOST_CHECK_EQUAL(map2.GetBackground(0).GetName(), "Background 0");
    BOOST_CHECK_EQUAL(map2.GetBackground(0).GetFilename(), "003-StarlitSky01.png");
    BOOST_CHECK(map2.GetBackground(0).GetMode() == 6);

    BOOST_REQUIRE(map2.HasCollisionLayer());
    const std::vector<int32_t>& clayerdata = dynamic_cast<TileBasedCollisionLayer*>(map2.GetCollisionLayer())->GetData();
    BOOST_CHECK_EQUAL_COLLECTIONS(clayerdata.begin(), clayerdata.end(), collision, collision + 25);
}

// Patrick
BOOST_FIXTURE_TEST_CASE(TestMapSaveXml, MapTest)
{
    Map map2;

    MapHandlerManager().Save("testout.xml", map);
    MapHandlerManager().Load("testout.xml", map2);

    BOOST_CHECK_EQUAL(map2.GetTileWidth(), 32);
    BOOST_CHECK_EQUAL(map2.GetTileHeight(), 32);
    BOOST_CHECK_EQUAL(map2.GetWidth(), 5);
    BOOST_CHECK_EQUAL(map2.GetHeight(), 5);
    BOOST_CHECK_EQUAL(map2.GetName(), "My Map");
    BOOST_CHECK_EQUAL(map2.GetFilename(), "011-PortTown01.png");
    BOOST_CHECK(map2.GetNumLayers() == 1);
    BOOST_CHECK(map2.GetNumBackgrounds() == 0);

    map2.Destroy();
    int layer[25] = {-1, -1, -1, 11, -1, -1, 6, -1, -1, -1, -1, -1, -1, -1, 7, 7, -1, -1, -1, 15, 15, -1, -1, 13, -1};
    int collision[25] = {-1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, -1, 0, -1};

    MapHandlerManager().Load("complete.xml", map2);
    BOOST_CHECK_EQUAL(map2.GetTileWidth(), 32);
    BOOST_CHECK_EQUAL(map2.GetTileHeight(), 32);
    BOOST_CHECK_EQUAL(map2.GetWidth(), 5);
    BOOST_CHECK_EQUAL(map2.GetHeight(), 5);
    BOOST_CHECK_EQUAL(map2.GetName(), "My Map");
    BOOST_CHECK_EQUAL(map2.GetFilename(), "011-PortTown01.png");

    BOOST_CHECK(map2.GetNumLayers() == 2);
    BOOST_CHECK_EQUAL(map2.GetLayer(1).GetName(), "Layer 1");
    const std::vector<int32_t>& layerdata = map2.GetLayer(1).GetData();
    BOOST_CHECK_EQUAL_COLLECTIONS(layerdata.begin(), layerdata.end(), layer, layer + 25);

    BOOST_CHECK(map2.GetNumBackgrounds() == 1);
    BOOST_CHECK_EQUAL(map2.GetBackground(0).GetName(), "Background 0");
    BOOST_CHECK_EQUAL(map2.GetBackground(0).GetFilename(), "003-StarlitSky01.png");
    BOOST_CHECK(map2.GetBackground(0).GetMode() == 6);

    BOOST_REQUIRE(map2.HasCollisionLayer());
    const std::vector<int32_t>& clayerdata = dynamic_cast<TileBasedCollisionLayer*>(map2.GetCollisionLayer())->GetData();
    BOOST_CHECK_EQUAL_COLLECTIONS(clayerdata.begin(), clayerdata.end(), collision, collision + 25);
}

BOOST_FIXTURE_TEST_CASE(TestMapResizeCopy, MapTest)
{
    map.Resize(1, 1, true);
    BOOST_CHECK_EQUAL(map.GetWidth(), 1);
    BOOST_CHECK_EQUAL(map.GetHeight(), 1);
    BOOST_CHECK_EQUAL(map.GetLayer(0)[0], 0);
}

BOOST_FIXTURE_TEST_CASE(TestMapResizeClear, MapTest)
{
    map.Resize(10, 20, false);
    BOOST_CHECK_EQUAL(map.GetWidth(), 10);
    BOOST_CHECK_EQUAL(map.GetHeight(), 20);
    BOOST_CHECK_EQUAL(map.GetLayer(0)[55], -1);
}

BOOST_FIXTURE_TEST_CASE(TestMapSaveBackground, MapTest)
{
    Map map2 = map;
    Map map3;
    map2.Add(Background("The Sky", "003-StarlitSky01.png", (Background::ScrollMode::Autoscroll | Background::DrawMode::Repeating), 3, 20));

    MapHandlerManager().Save("testout3.map", map2);
    MapHandlerManager().Load("testout3.map", map3);

    BOOST_CHECK_EQUAL(map3.GetTileWidth(), 32);
    BOOST_CHECK_EQUAL(map3.GetTileHeight(), 32);
    BOOST_CHECK_EQUAL(map3.GetWidth(), 5);
    BOOST_CHECK_EQUAL(map3.GetHeight(), 5);
    BOOST_CHECK_EQUAL(map3.GetName(), "My Map");
    BOOST_CHECK_EQUAL(map3.GetFilename(), "011-PortTown01.png");
    BOOST_CHECK(map3.GetNumLayers() == 1);
    BOOST_REQUIRE(map3.GetNumBackgrounds() == 1);

    Background back2 = map3.GetBackground(0);
    BOOST_CHECK_EQUAL(back2.GetName(), "The Sky");
    BOOST_CHECK_EQUAL(back2.GetFilename(), "003-StarlitSky01.png");
    BOOST_CHECK(back2.GetMode() == (Background::ScrollMode::Autoscroll | Background::DrawMode::Repeating));

    BOOST_CHECK_EQUAL(back2.GetSpeedX(), 3);
    BOOST_CHECK_EQUAL(back2.GetSpeedY(), 20);
}
