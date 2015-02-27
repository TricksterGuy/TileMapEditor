#include <boost/test/auto_unit_test.hpp>
#include "MapHandlerManager.hpp"
#include "MapHandlerManager.hpp"
#include "BinaryMapHandler.hpp"
#include "TextMapHandler.hpp"
#include "ImageMapHandler.hpp"


BOOST_AUTO_TEST_CASE(TestFindHandler)
{
  BaseMapHandler* handler = MapHandlerManager().FindHandler("map");
  BOOST_CHECK_EQUAL(handler->GetName(), "Official Map Format V1.0");

  handler = MapHandlerManager().FindHandler("png");
  BOOST_CHECK_EQUAL(handler->GetName(), "Map Image Writer");

  handler = MapHandlerManager().FindHandler("bmp");
  BOOST_CHECK_EQUAL(handler->GetName(), "Map Image Writer");

  handler = MapHandlerManager().FindHandler("txt");
  BOOST_CHECK_EQUAL(handler->GetName(), "Text Format");

  handler = MapHandlerManager().FindHandler("xml");
  BOOST_CHECK_EQUAL(handler->GetName(), "Xml Format");

  handler = MapHandlerManager().FindHandler("");
  BOOST_CHECK_EQUAL(handler->GetName(), "Map Layers Image Writer");

  handler = MapHandlerManager().FindHandler("zzyzz");
  BOOST_CHECK(handler == NULL);
}

BOOST_AUTO_TEST_CASE(TestSavePass)
{
  int data[25];
  int data2[25];

  Map map;
  for (int i = 0; i < 25; i++) data[i] = rand() % 100;
  for (int i = 0; i < 25; i++) data2[i] = rand() % 100;
  map.Add(Layer());
  map.Add(Layer());
  map.Resize(5, 5);
  for (int i = 0; i < 25; i++) map.GetLayer(0)[i] = data[i];
  for (int i = 0; i < 25; i++) map.GetLayer(1)[i] = data2[i];

  MapHandlerManager().Save("testout2.MAP", map);

  Map map2;
  MapHandlerManager().Load("testout2.MaP", map2);

  const std::vector<int32_t>& layer1 = map.GetLayer(0).GetData();
  const std::vector<int32_t>& layer2 = map.GetLayer(1).GetData();
  BOOST_CHECK_EQUAL_COLLECTIONS(layer1.begin(), layer1.end(), data, data + 25);
  BOOST_CHECK_EQUAL_COLLECTIONS(layer2.begin(), layer2.end(), data2, data2 + 25);
}

BOOST_AUTO_TEST_CASE(TestSaveFail)
{
  Map map;

  try
  {
    MapHandlerManager().Save("testout3.ZZYNAS", map);
    BOOST_FAIL("");
  }
  catch (const char* s)
  {
  }
}

BOOST_AUTO_TEST_CASE(TestLoadFail)
{
  Map map;

  MapHandlerManager().Load("testout3.ZZYNAS", map);
  MapHandlerManager().Load("testout4.map", map);
}
