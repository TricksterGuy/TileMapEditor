#define BOOST_TEST_DYN_LINK
#include <boost/test/auto_unit_test.hpp>
#include "Layer.hpp"

struct LayerTest
{
    Layer layer;

    LayerTest() : layer("", 5, 5)
    {
    }

    ~LayerTest()
    {
    }
};

BOOST_FIXTURE_TEST_CASE(TestLayerResize1, LayerTest)
{
    layer[24] = 78;  // Layer[4, 4]
    layer.Resize(10, 10);
    BOOST_CHECK_EQUAL(layer[4 * 10 + 4], 78);
    BOOST_CHECK_EQUAL(layer.GetWidth(), 10);
    BOOST_CHECK_EQUAL(layer.GetHeight(), 10);
}

BOOST_FIXTURE_TEST_CASE(TestLayerResizeClear, LayerTest)
{
    layer[24] = 78;  // Layer[4, 4]
    layer.Resize(2, 2, false);
    BOOST_CHECK_EQUAL(layer.GetWidth(), 2);
    BOOST_CHECK_EQUAL(layer.GetHeight(), 2);
    const std::vector<int32_t>& actual = layer.GetData();
    const std::vector<int32_t> expected = {-1, -1, -1, -1};
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actual.begin(), actual.end());
}

BOOST_FIXTURE_TEST_CASE(TestLayerResizeUnchanged, LayerTest)
{
    std::vector<int32_t> expected = {
        6, 3, 12, 33, 93,
        1, 1, 1, 1, 1,
        45, 39, 22, 11, 102,
        4, 23, 444, 28, 1824,
        22, 38, 32, 42, 555
    };
    layer.SetData(expected);
    layer.Resize(5, 5);
    BOOST_CHECK_EQUAL(layer.GetWidth(), 5);
    BOOST_CHECK_EQUAL(layer.GetHeight(), 5);
    const std::vector<int32_t>& actual = layer.GetData();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actual.begin(), actual.end());
}

BOOST_FIXTURE_TEST_CASE(TestLayerResize2, LayerTest)
{
    layer[15] = 34;  // Layer[0, 3]
    layer.Resize(1, 25);
    BOOST_CHECK_EQUAL(layer[3 * 1 + 0], 34);
    BOOST_CHECK_EQUAL(layer.GetWidth(), 1);
    BOOST_CHECK_EQUAL(layer.GetHeight(), 25);
}

BOOST_FIXTURE_TEST_CASE(TestLayerClear, LayerTest)
{
    std::vector<int32_t> start = {
        6, 3, 12, 33, 93,
        1, 1, 1, 1, 1,
        45, 39, 22, 11, 102,
        4, 23, 444, 28, 1824,
        22, 38, 32, 42, 555
    };
    std::vector<int32_t> expected = {
        -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1,
    };
    layer.SetData(start);
    layer.Clear();
    BOOST_CHECK_EQUAL(layer.GetWidth(), 5);
    BOOST_CHECK_EQUAL(layer.GetHeight(), 5);
    const std::vector<int32_t>& actual = layer.GetData();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actual.begin(), actual.end());
}

BOOST_FIXTURE_TEST_CASE(TestLayerShift, LayerTest)
{
    std::vector<int32_t> start = {
        6, 3, 12, 33, 93,
        1, 1, 1, 1, 1,
        45, 39, 22, 11, 102,
        4, 23, 444, 28, 1824,
        22, 38, 32, 42, 555
    };
    std::vector<int32_t> expected = {
        -1, -1, -1, -1, -1,
        -1, -1, 6, 3, 12,
        -1, -1, 1, 1, 1,
        -1, -1, 45, 39, 22,
        -1, -1, 4, 23, 444,
    };
    layer.SetData(start);
    layer.Shift(2, 1, false);
    BOOST_CHECK_EQUAL(layer.GetWidth(), 5);
    BOOST_CHECK_EQUAL(layer.GetHeight(), 5);
    const std::vector<int32_t>& actual = layer.GetData();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actual.begin(), actual.end());
}

BOOST_FIXTURE_TEST_CASE(TestLayerShiftNegative, LayerTest)
{
    std::vector<int32_t> start = {
        6, 3, 12, 33, 93,
        1, 1, 1, 1, 1,
        45, 39, 22, 11, 102,
        4, 23, 444, 28, 1824,
        22, 38, 32, 42, 555
    };
    std::vector<int32_t> expected = {
        23, 444, 28, 1824, -1,
        38, 32, 42, 555, -1,
        -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1,
    };
    layer.SetData(start);
    layer.Shift(-1, -3, false);
    BOOST_CHECK_EQUAL(layer.GetWidth(), 5);
    BOOST_CHECK_EQUAL(layer.GetHeight(), 5);
    const std::vector<int32_t>& actual = layer.GetData();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actual.begin(), actual.end());
}

BOOST_FIXTURE_TEST_CASE(TestLayerShiftWrap1, LayerTest)
{
    std::vector<int32_t> start = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    std::vector<int32_t> expected = {
        6, 4, 5,
        9, 7, 8,
        3, 1, 2
    };
    layer.Resize(3, 3, false);
    layer.SetData(start);
    layer.Shift(1, 2, true);
    BOOST_CHECK_EQUAL(layer.GetWidth(), 3);
    BOOST_CHECK_EQUAL(layer.GetHeight(), 3);
    const std::vector<int32_t>& actual = layer.GetData();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actual.begin(), actual.end());
}

BOOST_FIXTURE_TEST_CASE(TestLayerShiftWrap2, LayerTest)
{
    std::vector<int32_t> start = {
        6, 3, 12, 33, 93,
        1, 1, 1, 1, 1,
        45, 39, 22, 11, 102,
        4, 23, 444, 28, 1824,
        22, 38, 32, 42, 555
    };
    std::vector<int32_t> expected = {
        444, 28, 1824, 4, 23,
        32, 42, 555, 22, 38,
        12, 33, 93, 6, 3,
        1, 1, 1, 1, 1,
        22, 11, 102, 45, 39,
    };
    layer.SetData(start);
    layer.Shift(3, 2, true);
    BOOST_CHECK_EQUAL(layer.GetWidth(), 5);
    BOOST_CHECK_EQUAL(layer.GetHeight(), 5);
    const std::vector<int32_t>& actual = layer.GetData();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actual.begin(), actual.end());
}

BOOST_FIXTURE_TEST_CASE(TestLayerShiftWrapNegative, LayerTest)
{
    std::vector<int32_t> start = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    std::vector<int32_t> expected = {
        5, 6, 4,
        8, 9, 7,
        2, 3, 1,
    };
    layer.Resize(3, 3, false);
    layer.SetData(start);
    layer.Shift(2, -1, true);
    BOOST_CHECK_EQUAL(layer.GetWidth(), 3);
    BOOST_CHECK_EQUAL(layer.GetHeight(), 3);
    const std::vector<int32_t>& actual = layer.GetData();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actual.begin(), actual.end());
}
