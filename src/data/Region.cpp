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
#include "Region.hpp"

#include <algorithm>
#include <list>
#include <set>

#include "Logger.hpp"

Region::Region(const std::vector<Rectangle>& _rectangles) : rectangles(_rectangles)
{
}

bool Region::operator==(const Region& r) const
{
    return rectangles == r.rectangles;
}

bool Region::Contains(int32_t x, int32_t y) const
{
    for (const Rectangle& rectangle : rectangles)
    {
        if (rectangle.Contains(x, y))
            return true;
    }
    return false;
}

bool Region::Contains(const Rectangle& r) const
{
    std::set<Rectangle> intersectSet;
    for (const auto& inr : rectangles)
    {
        Rectangle overlap;
        if (r.Intersects(inr, overlap))
            intersectSet.insert(overlap);
    }

    // Some optimizations
    if (intersectSet.empty())
        return false;

    std::vector<Rectangle> intersect(intersectSet.begin(), intersectSet.end());
    int64_t total = 0;
    for (const Rectangle& inr : intersect)
        total += inr.Area();

    // If the of the inner rectangles is less than the rectangles area then we know it doesn't cover the rectangle
    if (total < r.Area())
        return false;

    std::sort(intersect.begin(), intersect.end());
    std::set<int32_t> events;
    for (const auto& inr : intersect)
    {
        events.insert(inr.x);
        events.insert(inr.x + inr.width);
    }

    unsigned int rptr = 0;
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
            for (const auto& inr : activeSet)
            {
                events.insert(inr.y);
                events.insert(inr.y + inr.height);
            }
            int prevEvent = 0;
            int overlap = 0;
            for (const auto& event2 : events)
            {
                if (overlap != 0)
                    cutlength += event2 - prevEvent;

                for (const auto& inr : activeSet)
                {
                    // Enter event
                    if (inr.y == event2)
                        overlap++;
                    else if (inr.y + inr.height == event2)
                        overlap--;
                }
                prevEvent = event2;
            }
        }

        area += cutlength * (event - lastevent);

        // Leave event? remove rectangles.
        activeSet.remove_if([&event](const Rectangle& inr)
        {
            return inr.x + inr.width == event;
        });

        // Enter event add rectangles to active set.
        while (rptr < intersect.size() && intersect[rptr].x == event)
        {
            activeSet.push_back(intersect[rptr]);
            rptr++;
        }
        lastevent = event;
    }

    return area == r.Area();
}

Rectangle Region::Bounds() const
{
    if (rectangles.empty())
        return Rectangle();

    int32_t minx, miny, maxx, maxy;
    rectangles[0].GetCoords(minx, miny, maxx, maxy);

    for (const auto& r : rectangles)
    {
        int32_t cix, ciy, cfx, cfy;
        r.GetCoords(cix, ciy, cfx, cfy);

        if (minx > cix)
            minx = cix;
        if (miny > ciy)
            miny = ciy;
        if (maxx > cfx)
            maxx = cfx;
        if (maxy > cfy)
            maxy = cfy;
    }

    return Rectangle(minx, miny, maxx - minx, maxy - miny);
}

void Region::Intersect(const Rectangle& r)
{
    std::set<Rectangle> intersect;
    for (const auto& inr : rectangles)
    {
        Rectangle overlap;
        if (r.Intersects(inr, overlap))
            intersect.insert(overlap);
    }

    rectangles.assign(intersect.begin(), intersect.end());
}

bool Region::Intersects(const Rectangle& r)
{
    for (const auto& inr : rectangles)
    {
        Rectangle overlap;
        if (r.Intersects(inr, overlap))
            return true;
    }

    return false;
}


void Region::Subtract(const Rectangle& remove)
{
    std::vector<Rectangle> newRectangles = rectangles;
    rectangles.clear();
    for (const auto& rectangle : newRectangles)
    {
        Rectangle overlap;
        // If it doesn't intersect we are ok, but if it does
        // try to create 4 rectangles.
        if (rectangle.Intersects(remove, overlap))
        {
            int32_t x1i, y1i, x1f, y1f;
            int32_t x2i, y2i, x2f, y2f;

            rectangle.GetCoords(x1i, y1i, x1f, y1f);
            overlap.GetCoords(x2i, y2i, x2f, y2f);

            Rectangle top(std::min(x1i, x2i), std::min(y1i, y2i), rectangle.width, y2i - y1i);
            Rectangle left(std::min(x1i, x2i), std::min(y1i, y2i), x2i - x1i, rectangle.height);
            Rectangle right(std::min(x1f, x2f), std::min(y1i, y2i), x1f - x2f, rectangle.height);
            Rectangle bottom(std::min(x1i, x2i), std::min(y1f, y2f), rectangle.width, y1f - y2f);

            if (top.IsValid())
                Add(top);
            if (left.IsValid())
                Add(left);
            if (right.IsValid())
                Add(right);
            if (bottom.IsValid())
                Add(bottom);
        }
        else
            rectangles.push_back(rectangle);
    }
}

void Region::Move(int32_t x, int32_t y)
{
    for (Rectangle& r : rectangles)
        r.Move(x, y);
}

void Region::Add(const Rectangle& r)
{
    if (!Contains(r))
        DoAdd(r);
}

void Region::Xor(const Rectangle& remove)
{
    std::vector<Rectangle> newRectangles = rectangles;
    rectangles.clear();
    for (const auto& rectangle : newRectangles)
    {
        Rectangle overlap;
        // If it doesn't intersect we are ok, but if it does
        // try to create 4 rectangles.
        if (rectangle.Intersects(remove, overlap))
        {
            int32_t x1i, y1i, x1f, y1f;
            int32_t x2i, y2i, x2f, y2f;
            int32_t x3i, y3i, x3f, y3f;

            rectangle.GetCoords(x1i, y1i, x1f, y1f);
            overlap.GetCoords(x2i, y2i, x2f, y2f);
            remove.GetCoords(x3i, y3i, x3f, y3f);

            Rectangle top(std::min(x1i, x2i), std::min(y1i, y2i), rectangle.width, y2i - y1i);
            Rectangle left(std::min(x1i, x2i), std::min(y1i, y2i), x2i - x1i, rectangle.height);
            Rectangle right(std::min(x1f, x2f), std::min(y1i, y2i), x1f - x2f, rectangle.height);
            Rectangle bottom(std::min(x1i, x2i), std::min(y1f, y2f), rectangle.width, y1f - y2f);

            Rectangle top2(std::min(x3i, x2i), std::min(y3i, y2i), remove.width, y2i - y3i);
            Rectangle left2(std::min(x3i, x2i), std::min(y3i, y2i), x2i - x3i, remove.height);
            Rectangle right2(std::min(x3f, x2f), std::min(y3i, y2i), x3f - x2f, remove.height);
            Rectangle bottom2(std::min(x3i, x2i), std::min(y3f, y2f), remove.width, y3f - y2f);

            if (top.IsValid())
                Add(top);
            if (left.IsValid())
                Add(left);
            if (right.IsValid())
                Add(right);
            if (bottom.IsValid())
                Add(bottom);

            if (top2.IsValid())
                Add(top2);
            if (left2.IsValid())
                Add(left2);
            if (right2.IsValid())
                Add(right2);
            if (bottom2.IsValid())
                Add(bottom2);
        }
        else
            rectangles.push_back(rectangle);
    }
}

int64_t Region::Area() const
{
    sort(rectangles.begin(), rectangles.end());
    std::set<int32_t> events;
    for (const auto& inr : rectangles)
    {
        events.insert(inr.x);
        events.insert(inr.x + inr.width);
    }

    unsigned int rptr = 0;
    int lastevent = 0;
    int64_t area = 0;
    std::list<Rectangle> activeSet;
    for (const auto& event : events)
    {
        int32_t cutlength = 0;
        // Get Area and add it.
        if (!activeSet.empty())
        {
            std::set<int32_t> events;
            for (const auto& inr : activeSet)
            {
                events.insert(inr.y);
                events.insert(inr.y + inr.height);
            }
            int prevEvent = 0;
            int overlap = 0;
            for (const auto& event2 : events)
            {
                if (overlap != 0)
                    cutlength += event2 - prevEvent;

                for (const auto& inr : activeSet)
                {
                    // Enter event
                    if (inr.y == event2)
                        overlap++;
                    else if (inr.y + inr.height == event2)
                        overlap--;
                }

                prevEvent = event2;
            }
        }
        area += cutlength * (event - lastevent);

        // Leave event? remove rectangles.
        activeSet.remove_if([&event](const Rectangle& inr)
        {
            return inr.x + inr.width == event;
        });

        // Enter event add rectangles to active set.
        while (rptr < rectangles.size() && rectangles[rptr].x == event)
        {
            activeSet.push_back(rectangles[rptr]);
            rptr++;
        }
        lastevent = event;
    }

    return area;
}

int Region::Size() const
{
    return rectangles.size();
}

void Region::Clear()
{
    rectangles.clear();
}

void Region::DoAdd(const Rectangle& r)
{
    VerboseLog("Adding rectangle(%d %d %d %d)", r.x, r.y, r.width, r.height);
    std::vector<Rectangle>::iterator it = remove_if(rectangles.begin(), rectangles.end(),
                                          [&r](Rectangle& inr)
    {
        return r.Contains(inr);
    });
    rectangles.erase(it, rectangles.end());
    rectangles.push_back(r);
}
