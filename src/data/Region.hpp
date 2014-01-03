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

#ifndef REGION_HPP
#define REGION_HPP

#include <vector>
#include "Rectangle.hpp"

/** Defines a region a set of rectangles.*/
class Region
{
    public:
        /** Creates a region from the rectangles given.
          * @param rectangles List of rectangles.
          */
        Region(const std::vector<Rectangle>& rectangles);
        /** Creates an empty region.*/
        Region();
        ~Region();

        const std::vector<Rectangle>& getData() const {return rectangles;}

        /** Test if the point (x,y) is contained in the region.
          * @param x X coordinate.
          * @param y Y coordinate.
          * @return true if the point is contained false otherwise.
          */
        bool contains(int32_t x, int32_t y) const;
        /** Test if the rectangle is fully contained in the region.
          * @param r Rectangle to test.
          * @return true if the rectangle is contained in this region false otherwise.
          */
        bool contains(const Rectangle& r) const;
        /** Test if the region is fully contained within this one.
          * @param r Region to test.
          * @return true if the region is contained in this one false otherwise.
          */
        bool contains(const Region& r) const;
        /** Gets the bounds of this region.
          * @return The smallest rectangle that fully contains this region.
          */
        Rectangle bounds() const;
        /** Intersects the rectangle given with this region.
          * @param r Rectangle to intersect this region with.
          */
        void intersect(const Rectangle& r);
        /** Intersects the region given with this region.
          * @param r Region to intersect this region with.
          */
        void intersect(const Region& r);
        /** Subtracts the rectangle given with this region.
          * @param r Rectangle to intersect this region with.
          */
        void subtract(const Rectangle& r);
        /** Subtracts the region given with this region.
          * @param r Region to intersect this region with.
          */
        void subtract(const Region& r);
        /** Moves this region.
          * @param x Horizontal Movement.
          * @param y Vertical Movement.
          */
        void move(int32_t x, int32_t y);
        /** Adds a rectangle to this region.
          * @param r Rectangle to add.
          */
        void add(const Rectangle& r);
        /** Adds a region to this region.
          * @param r Region to add.
          */
        void add(const Region& r);
        /** Xors this region with the rectangle given.
          * @param r Rectangle to xor with.
          */
        void exclusiveOr(const Rectangle& r);
        /** Xors this region with the region given.
          * @param r Region to xor with.
          */
        void exclusiveOr(const Region& r);
        /** Calculates the area of this region.
          * @return the area of the region.
          */
        int64_t area() const;
        /** Gets the size of the region (number of rectangles)
          * @return the number of rectangles that make up this region.
          */
        int size() const;
        /** Clears the region.*/
        void clear();

    private:
        mutable std::vector<Rectangle> rectangles;
        /** Adds a rectangle and minimizes rectangles */
        void doAdd(const Rectangle& r);
};

#endif
