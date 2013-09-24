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

#ifndef LAYER_HPP
#define LAYER_HPP

#include <vector>
#include <string>
#include <cstdint>

/** This class stores one layer of tile information
  */
class Layer
{
	public:
        /** Creates a new layer with the specified name, width, height and data.
          * @param name name of the layer.
          * @param width non-zero width of the layer.
          * @param height non-zero height of the layer.
          * @param data data which should be width*height ints.
          */
		Layer(const std::string& name, uint32_t width, uint32_t height, const std::vector<int32_t>& data);
        /** Creates a new layer with the specified name, width, height and data.
          * @param name name of the layer.
          * @param width non-zero width of the layer.
          * @param height non-zero height of the layer.
          * @param data data which should be width*height ints.
          */
		Layer(const std::string& name, uint32_t width, uint32_t height, const int32_t* data);
        /** Creates a new layer with the specified name, width, and height.
          * @param name name of the layer.
          * @param width non-zero width of the layer.
          * @param height non-zero height of the layer.
          */
		Layer(const std::string& name = "", uint32_t width = 1, uint32_t height = 1);

		Layer(const Layer& layer);
		~Layer();

		Layer& operator=(const Layer& layer);
		bool operator==(const Layer& other);
		int32_t& operator[](const uint32_t index) {return data[index];}
		const int32_t& operator[](const uint32_t index) const {return data[index];}

        /** Clears the layer. */
		void clear();
        /** Shifts the entire layer in any direction a certain number of tiles.
          * @param horizontal amount to move left or right (negative = left).
          * @param vertical amount to move up or down (negative = up).
          * @param wrap if true any rows/columns will wrap around.
          */
		void shift(int horizontal, int vertical, bool wrap = false);
        /** Resizes the layer to fit the new dimensions.
          * @param newwidth the new width of the layer in tiles.
          * @param newheight the new height of the layer in tiles.
          * @param copy if true then don't destroy the layer in the process if false then clear out the layer.
          */
		void resize(uint32_t width, uint32_t height, bool copy = true);

		std::string getName() const {return name;}
		uint32_t getWidth() const {return width;}
		uint32_t getHeight() const {return height;}
		const std::vector<int32_t>& getData() const {return data;}
		std::vector<int32_t>& getData() {return data;}
		int32_t at(uint32_t x, uint32_t y) const {return at(y * width + x);}
		int32_t at(uint32_t index) const {return data[index];}

		void setName(const std::string& _name) {name = _name;}
		void set(uint32_t x, uint32_t y, int32_t value) {set(y * width + x, value);}
		void set(uint32_t index, int32_t value) {data[index] = value;}

	protected:
		std::string name;
		uint32_t width, height;
		std::vector<int32_t> data;
};

#endif
