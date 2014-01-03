/******************************************************************************************************
 * Tile Map Editor
 * Copyright (C) 2009-2013 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
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

#include "AnimatedTile.hpp"

AnimatedTile::AnimatedTile(const std::string& _name, int32_t _delay, Animation::Type _type, int32_t _times) :
name(_name), delay(_delay), type(_type), times(_times)
{
}

AnimatedTile::AnimatedTile(const std::string& _name, int32_t _delay, Animation::Type _type, int32_t _times, const std::vector<int32_t>& _frames) :
name(_name), delay(_delay), type(_type), times(_times), frames(_frames)
{
}

AnimatedTile::~AnimatedTile()
{
}

AnimatedTile& AnimatedTile::operator=(const AnimatedTile& other)
{
    if (this != &other)
    {
        name = other.name;
        delay = other.delay;
        type = other.type;
        times = other.times;
        frames = other.frames;
    }

    return *this;
}

void AnimatedTile::add(int32_t frame)
{
    frames.push_back(frame);
}

int32_t AnimatedTile::getCurrentFrame(int clock) const
{
    if (clock % delay != 0) return cachedFrame;

    int timestep = clock / delay;
    unsigned int numFrames = frames.size();
    int frame;
    int constrained, decrease;

    switch(type)
    {
        case Animation::Normal:
            frame = timestep % numFrames;
            break;
        case Animation::Reverse:
            frame = numFrames - 1 - (timestep % numFrames);
            break;
        case Animation::Ping:
            constrained = timestep % (numFrames * 2 - 2);
            decrease = constrained / numFrames;
            frame = constrained % numFrames;
            if (decrease) frame = numFrames - 2 - frame;
            break;
        case Animation::PingReverse:
            constrained = timestep % (numFrames * 2 - 2);
            decrease = constrained / numFrames;
            frame = constrained % numFrames;
            if (decrease) frame = numFrames - 2 - frame;
            frame = numFrames - 1 - frame;
            break;
        case Animation::Random:
            frame = rand() % numFrames;
            break;
        default:
            frame = -1;
    }

    cachedFrame = frames[frame];

    return cachedFrame;
}
