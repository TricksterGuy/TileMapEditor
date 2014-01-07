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

#ifndef TILEBASEDCOLLISIONLAYER_HPP
#define TILEBASEDCOLLISIONLAYER_HPP

#include <cstdint>
#include <vector>
#include "CollisionLayer.hpp"

/** A collision layer that is tile based.
  * Collision information is defined per tile as either -1 for impassable
  * or 0 for passable.
  */
class TileBasedCollisionLayer : public CollisionLayer
{
	public:
	    /** Creates a collision layer with specified width, height and data.
	      * @param width Nonzero Width of the collision layer.
	      * @param height Nonzero Height of the collision layer.
	      * @param data collision info Must be width * height ints
	      */
		TileBasedCollisionLayer(int width, int height, const std::vector<int32_t>& data);
	    /** Creates a collision layer with specified width, height and data.
	      * @param width Nonzero Width of the collision layer.
	      * @param height Nonzero Height of the collision layer.
	      * @param data collision info Must be width * height ints
	      */
		TileBasedCollisionLayer(int width, int height, const int32_t* data);
	    /** Creates a collision layer with specified width, height and data.
	      * @param width Nonzero Width of the collision layer.
	      * @param height Nonzero Height of the collision layer.
	      */
		TileBasedCollisionLayer(int width = 1, int height = 1);
		TileBasedCollisionLayer(const TileBasedCollisionLayer& layer);
		~TileBasedCollisionLayer();
		TileBasedCollisionLayer& operator=(const TileBasedCollisionLayer& layer);
		bool operator==(const TileBasedCollisionLayer& other);
		const int32_t& operator[](const uint32_t index) const {return data[index];}
		int32_t& operator[](const uint32_t index) {return data[index];}

        /** @see CollisionLayer::clear */
		virtual void Clear();
		/** @see CollisionLayer::shift */
		virtual void Shift(int horizontal, int vertical, bool wrap = false);
		/** @see CollisionLayer::resize */
		virtual void Resize(uint32_t width, uint32_t height, bool copy = true);

		uint32_t GetWidth() const {return width;}
		uint32_t GetHeight() const {return height;}
		const std::vector<int32_t>& GetData() const {return data;}
		std::vector<int32_t>& GetData() {return data;}
		int32_t At(uint32_t index) const {return data[index];}
		int32_t At(uint32_t x, uint32_t y) const {return At(y * width + x);}

		void Set(uint32_t x, uint32_t y, int32_t value) {Set(y * width + x, value);}
		void Set(uint32_t index, int32_t value) {data[index] = value;}

	protected:
        uint32_t width, height;
        std::vector<int32_t> data;
};

#endif
