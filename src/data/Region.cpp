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

#include <algorithm>
#include <set>
#include <list>
#include "Region.hpp"

Region::Region(const std::vector<Rectangle>& _rectangles) : rectangles(_rectangles)
{
}

Region::Region()
{
}

Region::~Region()
{
}

bool Region::contains(int32_t x, int32_t y) const
{
    for (const Rectangle& rectangle : rectangles)
    {
        if (rectangle.contains(x, y))
            return true;
    }
    return false;
}


bool Region::contains(const Rectangle& r) const
{
    printf("CONTAINS CALLED\n");
    std::set<Rectangle> intersectSet;
    for (const Rectangle& inr : rectangles)
    {
        Rectangle overlap;
        if (r.intersects(inr, overlap))
            intersectSet.insert(overlap);
    }

    // Some optimizations
    if (intersectSet.empty()) return false;

    std::vector<Rectangle> intersect(intersectSet.begin(), intersectSet.end());
    int64_t total = 0;
    for (const Rectangle& inr : intersect)
        total += inr.area();


    // If the of the inner rectangles is less than the rectangles area then we know it doesn't cover the rectangle
    if (total < r.area())
        return false;

    std::sort(intersect.begin(), intersect.end());
    std::set<int32_t> events;
    for (const Rectangle& inr : intersect)
    {
        events.insert(inr.x);
        events.insert(inr.x + inr.width);
    }

    int rptr = 0;
    int lastevent = 0;
    int64_t area = 0;
    std::list<Rectangle> activeSet;
    for (const int32_t& event : events)
    {
        int32_t cutlength = 0;
        // Get Area and add it.
        if (!activeSet.empty())
        {
            std::set<int> events;
            for (const Rectangle& inr : activeSet)
            {
                events.insert(inr.y);
                events.insert(inr.y + inr.height);
            }
            int prevEvent = 0;
            int overlap = 0;
            for (const int32_t& event2 : events)
            {
                if (overlap != 0)
                    cutlength += event2 - prevEvent;

                for (const Rectangle& inr : activeSet)
                {
                    // Enter event
                    if (inr.y == event2)
                        overlap++;
                    else if (inr.y + inr.height == (uint32_t)event2)
                        overlap--;
                }
                prevEvent = event2;
            }
        }

        area += cutlength * (event - lastevent);

        // Leave event? remove rectangles.
        activeSet.remove_if([&event](const Rectangle& inr) {return inr.x + inr.width == (uint32_t)event;});

        // Enter event add rectangles to active set.
        while (rptr < (int)intersect.size() && intersect[rptr].x == (int32_t)event)
        {
            activeSet.push_back(intersect[rptr]);
            rptr++;
        }
        lastevent = event;
    }

    return area == r.area();
}

bool Region::contains(const Region& r) const
{
    /// TODO Implement
    return false;
}

Rectangle Region::bounds() const
{
    /// TODO Implement
    return Rectangle(0, 0, 0, 0);
}

void Region::intersect(const Rectangle& r)
{
    std::set<Rectangle> intersect;
    for (const Rectangle& inr : rectangles)
    {
        Rectangle overlap;
        if (r.intersects(inr, overlap))
            intersect.insert(overlap);
    }
    rectangles.assign(intersect.begin(), intersect.end());
}

void Region::intersect(const Region& r)
{
    /// TODO Implement
}

void Region::subtract(const Rectangle& r)
{
    /// TODO Implement
}

void Region::subtract(const Region& r)
{
    /// TODO Implement
}

void Region::move(int32_t x, int32_t y)
{
    for (Rectangle& r : rectangles)
        r.move(x, y);
}

void Region::add(const Rectangle& r)
{
    if (!contains(r))
        doAdd(r);
}

void Region::add(const Region& r)
{
    for (const Rectangle& inr : r.getData())
        add(inr);
}

void Region::exclusiveOr(const Rectangle& r)
{
    /// TODO Implement
}

void Region::exclusiveOr(const Region& r)
{
    /// TODO Implement
}

int64_t Region::area() const
{
    sort(rectangles.begin(), rectangles.end());
    std::set<int32_t> events;
    for (const Rectangle& inr : rectangles)
    {
        events.insert(inr.x);
        events.insert(inr.x + inr.width);
    }

    int rptr = 0;
    int lastevent = 0;
    int64_t area = 0;
    std::list<Rectangle> activeSet;
    for (const int32_t& event : events)
    {
        int32_t cutlength = 0;
        // Get Area and add it.
        if (!activeSet.empty())
        {
            std::set<int32_t> events;
            for (const Rectangle& inr : activeSet)
            {
                events.insert(inr.y);
                events.insert(inr.y + inr.height);
            }
            int prevEvent = 0;
            int overlap = 0;
            for (const int32_t& event2 : events)
            {
                if (overlap != 0)
                    cutlength += event2 - prevEvent;

                for (const Rectangle& inr : activeSet)
                {
                    // Enter event
                    if (inr.y == event2)
                        overlap++;
                    else if (inr.y + inr.height == (uint32_t)event2)
                        overlap--;
                }

                prevEvent = event2;
            }
        }
        area += cutlength * (event - lastevent);

        // Leave event? remove rectangles.
        activeSet.remove_if([&event](const Rectangle& inr) {return inr.x + inr.width == (uint32_t)event;});

        // Enter event add rectangles to active set.
        while (rptr < (int)rectangles.size() && rectangles[rptr].x == event)
        {
            activeSet.push_back(rectangles[rptr]);
            rptr++;
        }
        lastevent = event;
    }

    return area;
}

int Region::size() const
{
    return rectangles.size();
}

void Region::clear()
{
    rectangles.clear();
}

void Region::doAdd(const Rectangle& r)
{
    std::vector<Rectangle>::iterator it = remove_if(rectangles.begin(), rectangles.end(),
                                                    [&r](Rectangle& inr) {return r.contains(inr);});
    rectangles.erase(it, rectangles.end());
    rectangles.push_back(r);
}
