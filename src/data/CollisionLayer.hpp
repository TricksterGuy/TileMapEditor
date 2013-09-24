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

#ifndef COLLISIONLAYER_HPP
#define COLLISIONLAYER_HPP

#include <cstdint>

namespace Collision
{
    /**
      * Defines a type of collision layer.
      */
    enum Type
    {
        /** Tile based collision layer
          * Defines collision per tile on the map as -1 for not passable
          * and 0 for passable.
          */
        TileBased = 0,
        /** Pixel based collision layer
          * Defines collision per pixel on the map. This is defined by a list
          * of rectangles specifying where the user can't move to.
          */
        PixelBased = 1,
        /** Direction based collision layer
          * Defines collision info per tile on the map; however unlike
          * TileBased collision layers each tile can specify which directions
          * you are allowed to move.  This data is stored as a bitvector per tile.
          */
        DirectionBased = 2
    };
}

/**
  * This class defines a special type of layer called a collision layer.
  * This class is used internally by the Map class.
  * @see Collision::Type for possible collision layer types.
  */
class CollisionLayer
{
	public:
	    /** Creates a new empty collision layer.
	      * @param type Type of collision layer to create.
          */
		CollisionLayer(Collision::Type type = Collision::TileBased);
		virtual ~CollisionLayer();

        /** Clears the collision layer. */
		virtual void clear() = 0;
        /** Shifts the entire collision layer in any direction a certain number of tiles.
          * @param horizontal amount to move left or right (negative = left).
          * @param vertical amount to move up or down (negative = up).
          * @param wrap if true any rows/columns will wrap around.
          */
		virtual void shift(int horizontal, int vertical, bool wrap = false) = 0;
        /** Resizes the collision layer to fit the new dimensions.
          * @param newwidth the new width of the collision layer in tiles.
          * @param newheight the new height of the collision layer in tiles.
          * @param copy if true then don't destroy the layer in the process if false then clear out the layer.
          */
		virtual void resize(uint32_t width, uint32_t height, bool copy = true) = 0;

		Collision::Type getType() const {return type;}

	protected:
		Collision::Type type;
};

#endif
