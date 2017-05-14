#define BOOST_TEST_DYN_LINK
#include <boost/test/auto_unit_test.hpp>
#include "AnimatedTile.hpp"

namespace
{
    std::vector<int32_t> ComputeAnimationFrames(const AnimatedTile& tile, int num_steps)
    {
        std::vector<int32_t> frames;
        for (int32_t i = 0; i < num_steps; i++)
            frames.push_back(tile.GetCurrentFrame(i));
        return frames;
    }
}

BOOST_AUTO_TEST_CASE(AnimatedTileBasic)
{
    AnimatedTile tile;
    tile.Add(4);
    tile.Add(2);
    tile.Add(3);
    tile.Add(99);
    tile.Add(7);

    BOOST_REQUIRE_EQUAL(tile.GetNumFrames(), 5);
    BOOST_CHECK_EQUAL(tile[0], 4);
    BOOST_CHECK_EQUAL(tile[1], 2);
    BOOST_CHECK_EQUAL(tile[2], 3);
    BOOST_CHECK_EQUAL(tile[3], 99);
    BOOST_CHECK_EQUAL(tile[4], 7);
}

BOOST_AUTO_TEST_CASE(AnimatedTileBasic2)
{
    const std::vector<int32_t> frames = {4, 2, 6, 3, 2, 2, 2, 7};
    AnimatedTile tile("", 1, Animation::Normal, -1, frames);
    BOOST_REQUIRE_EQUAL(tile.GetNumFrames(), 8);
    BOOST_CHECK_EQUAL(tile[0], 4);
    BOOST_CHECK_EQUAL(tile[1], 2);
    BOOST_CHECK_EQUAL(tile[2], 6);
    BOOST_CHECK_EQUAL(tile[3], 3);
    BOOST_CHECK_EQUAL(tile[4], 2);
    BOOST_CHECK_EQUAL(tile[5], 2);
    BOOST_CHECK_EQUAL(tile[6], 2);
    BOOST_CHECK_EQUAL(tile[7], 7);
}

BOOST_AUTO_TEST_CASE(AnimatedTileNormal)
{
    AnimatedTile tile("", 1, Animation::Normal, -1, {7, 2, 8, 11, 19});
    std::vector<int32_t> actual = ComputeAnimationFrames(tile, 14);
    std::vector<int32_t> expected {7, 2, 8, 11, 19, 7, 2, 8, 11, 19, 7, 2, 8, 11};

    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actual.begin(), actual.end());
}

BOOST_AUTO_TEST_CASE(AnimatedTilePing)
{
    AnimatedTile tile("", 1, Animation::Ping, -1, {7, 2, 8, 11, 19});
    std::vector<int32_t> actual = ComputeAnimationFrames(tile, 14);
    std::vector<int32_t> expected {7, 2, 8, 11, 19, 11, 8, 2, 7, 2, 8, 11, 19, 11};

    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actual.begin(), actual.end());
}

BOOST_AUTO_TEST_CASE(AnimatedTileReverse)
{
    AnimatedTile tile("", 1, Animation::Reverse, -1, {7, 2, 8, 11, 19});
    std::vector<int32_t> actual = ComputeAnimationFrames(tile, 14);
    std::vector<int32_t> expected {19, 11, 8, 2, 7, 19, 11, 8, 2, 7, 19, 11, 8, 2};

    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actual.begin(), actual.end());
}

BOOST_AUTO_TEST_CASE(AnimatedTilePingReverse)
{
    AnimatedTile tile("", 1, Animation::PingReverse, -1, {7, 2, 8, 11, 19});
    std::vector<int32_t> actual = ComputeAnimationFrames(tile, 14);
    std::vector<int32_t> expected {19, 11, 8, 2, 7, 2, 8, 11, 19, 11, 8, 2, 7, 2};

    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actual.begin(), actual.end());
}

// Not implemented
/*BOOST_AUTO_TEST_CASE(AnimatedTileNormalTimes)
{
    AnimatedTile tile("", 1, Animation::Normal, 2, {7, 2, 8, 11, 19});
    std::vector<int32_t> actual = ComputeAnimationFrames(tile, 14);
    std::vector<int32_t> expected {7, 2, 8, 11, 19, 7, 2, 8, 11, 19, 7, 7, 7, 7};

    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actual.begin(), actual.end());
}*/

BOOST_AUTO_TEST_CASE(AnimatedTileNormalDelay)
{
    AnimatedTile tile("", 2, Animation::Normal, -1, {7, 2, 8, 11, 19});
    std::vector<int32_t> actual = ComputeAnimationFrames(tile, 14);
    std::vector<int32_t> expected {7, 7, 2, 2, 8, 8, 11, 11, 19, 19, 7, 7, 2, 2};

    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actual.begin(), actual.end());
}
