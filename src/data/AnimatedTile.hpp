/******************************************************************************************************
 * Tile Map Editor
 * Copyright (C) 2009-2017 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
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
#ifndef ANIMATED_TILE_HPP
#define ANIMATED_TILE_HPP

#include <cstdint>
#include <string>
#include <vector>

/** This class represents a special type of tile that will be animated. */
class AnimatedTile
{
public:
    /** Represents how an animation will play through its frames. */
    enum Type
    {
        /** Play through the sequence.
          * Example 0,1,2,3,4 played through three times will be
          * 0,1,2,3,4,0,1,2,3,4,0,1,2,3,4
          */
        Normal = 0,
        /** Play through the sequence in reverse.
          * Example 0,1,2,3,4 played through three times will be
          * 4,3,2,1,0,4,3,2,1,0,4,3,2,1,0
          */
        Reverse = 1,
        /** Play through the sequence then play through in reverse.
          * Example 0,1,2,3,4 played through once will be
          * 0,1,2,3,4,3,2,1
          */
        Ping = 2,
        /** Play through the sequence in reverse then play through normally.
          * Example 0,1,2,3,4 played through once will be
          * 4,3,2,1,0,1,2,3,4
          */
        PingReverse = 3,
        /** Play through the frames randomly. */
        Random = 4
    };

    /** Creates an animated tile with an empty sequence of frames.
      * @param name Name of the animated tile.
      * @param delay Delay before the frame switches.
      * @param type Type of animation @see AnimatedTile::Type.
      * @param times Number of times the animation plays through its sequence -1 for infinite.
      */
    AnimatedTile(const std::string& name = "", int32_t delay = 1, Type type = Normal, int32_t times = -1);
    /** Creates an animated tile with predefined frames.
      * @param name Name of the animated tile.
      * @param delay Delay before the frame switches.
      * @param type Type of animation @see AnimatedTile::Type.
      * @param times Number of times the animation plays through its sequence -1 for infinite.
      * @param frames Sequence of frames (tile_ids) to play.
      */
    AnimatedTile(const std::string& name, int32_t delay, Type type, int32_t times, const std::vector<int32_t>& frames);

    int32_t& operator[](const int index) { return frames[index]; }
    const int32_t& operator[](const int index) const { return frames[index]; }

    /** Adds a new frame to this animated tile.
      * @param frame tile_id to add to the sequence in the animation.
      */
    void Add(int32_t frame);
    /** Gets the current frame (tile_id) in the animation.
      * @param global_clock the current timestep.
      */
    int32_t GetCurrentFrame(int global_clock) const;

    const std::string& GetName() const { return name; }
    int32_t GetDelay() const { return delay; }
    Type GetType() const { return type; }
    int32_t GetTimes() const { return times; }
    const std::vector<int32_t>& GetFrames() const { return frames; }
    uint32_t GetNumFrames() const { return frames.size(); }

    void SetName(const std::string& _name) { name = _name; }
    void SetDelay(int32_t _delay) { delay = _delay; }
    void SetType(Type _type) { type = _type; }
    void SetTimes(int32_t _times) { times = _times; }
    void SetFrames(const std::vector<int32_t>& _frames) { frames = _frames; }

private:
    /** The name of the AnimatedTile */
    std::string name;
    /** The delay in frames between tile ids in the AnimatedTile */
    int32_t delay;
    /** The type of animation the AnimatedTile uses */
    Type type;
    /** The number of times the AnimatedTile plays through its frames */
    int32_t times;
    /** Tile ids to play through for the AnimatedTile */
    std::vector<int32_t> frames;
};

#endif
