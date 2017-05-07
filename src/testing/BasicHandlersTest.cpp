#include <boost/test/auto_unit_test.hpp>
#include "MapHandlerManager.hpp"
#include "MapHandlerManager.hpp"
#include "BinaryMapHandler.hpp"
#include "TextMapHandler.hpp"
#include "ImageMapHandler.hpp"


BOOST_AUTO_TEST_CASE(TestFindHandler)
{
    BaseMapHandler* handler = MapHandlerManager().FindHandlerByExtension("map");
    BOOST_REQUIRE(handler != nullptr);
    BOOST_CHECK_EQUAL(handler->GetName(), "Official Map Format");

    /*
    handler = MapHandlerManager().FindHandlerByExtension("png");
    BOOST_REQUIRE(handler != nullptr);
    BOOST_CHECK_EQUAL(handler->GetName(), "Map Image Writer");

    handler = MapHandlerManager().FindHandlerByExtension("bmp");
    BOOST_REQUIRE(handler != nullptr);
    BOOST_CHECK_EQUAL(handler->GetName(), "Map Image Writer");
    */

    handler = MapHandlerManager().FindHandlerByExtension("txt");
    BOOST_REQUIRE(handler != nullptr);
    BOOST_CHECK_EQUAL(handler->GetName(), "Text Format");

    /*
    handler = MapHandlerManager().FindHandlerByExtension("xml");
    BOOST_REQUIRE(handler != nullptr);
    BOOST_CHECK_EQUAL(handler->GetName(), "Xml Format");

    handler = MapHandlerManager().FindHandlerByExtension("");
    BOOST_REQUIRE(handler != nullptr);
    BOOST_CHECK_EQUAL(handler->GetName(), "Map Layers Image Writer");
    */

    handler = MapHandlerManager().FindHandlerByExtension("zzyzz");
    BOOST_REQUIRE(handler == nullptr);
}

BOOST_AUTO_TEST_CASE(TestSavePass)
{
    int data[25];
    int data2[25];
    for (int i = 0; i < 25; i++) data[i] = rand() % 100;
    for (int i = 0; i < 25; i++) data2[i] = rand() % 100;

    Map map, map2;
    map.Add(Layer("layer 1", 5, 5));
    map.Add(Layer("layer 2", 5, 5));

    for (int i = 0; i < 25; i++) map.GetLayer(0)[i] = data[i];
    for (int i = 0; i < 25; i++) map.GetLayer(1)[i] = data2[i];

    MapHandlerManager().Save("testout2.MAP", map);
    MapHandlerManager().Load("testout2.MaP", map2);

    const Layer& layer1 = map.GetLayer(0);
    const Layer& layer2 = map.GetLayer(1);
    const std::vector<int32_t>& layer1_data = layer1.GetData();
    const std::vector<int32_t>& layer2_data = layer2.GetData();

    BOOST_CHECK_EQUAL(layer1.GetName(), "layer 1");
    BOOST_CHECK_EQUAL(layer2.GetName(), "layer 2");
    BOOST_CHECK_EQUAL_COLLECTIONS(layer1_data.begin(), layer1_data.end(), data, data + 25);
    BOOST_CHECK_EQUAL_COLLECTIONS(layer2_data.begin(), layer2_data.end(), data2, data2 + 25);
}

BOOST_AUTO_TEST_CASE(TestSaveFail)
{
    Map map;

    BOOST_CHECK_THROW(MapHandlerManager().Save("testout3.ZZYNAS", map), const char*);
}

BOOST_AUTO_TEST_CASE(TestLoadFail)
{
    Map map;

    BOOST_CHECK_THROW(MapHandlerManager().Load("testout3.ZZYNAS", map), const char*);
}
