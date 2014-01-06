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

#ifndef BINARYMAPHANDLER_HPP
#define BINARYMAPHANDLER_HPP

#include "BaseMapHandler.hpp"

/** Handler for .map files
  * My own map format that saves to a binary file
  */
class BinaryMapHandler : public BaseMapHandler
{
	public:
		BinaryMapHandler();
		~BinaryMapHandler();

        /** See BaseMapHandler::load */
        virtual void load(const std::string& filename, Map& map);
        /** See BaseMapHandler::save */
        virtual void save(const std::string& filename, Map& map);

	private:
        void readChunkName(std::ifstream& file, std::string& name, uint32_t& size);
        void readHEAD(std::ifstream& file, Map& map);
        void readMAPP(std::ifstream& file, Map& map, int32_t& num_layers, int32_t& num_backgrounds);
        void readLYRS(std::ifstream& file, Map& map, int32_t& num_layers);
        void readBGDS(std::ifstream& file, Map& map, int32_t& num_backgrounds);
        void readMTCL(std::ifstream& file, Map& map);
        void readMDCL(std::ifstream& file, Map& map);
        void readMPCL(std::ifstream& file, Map& map);
        void readTTCI(std::ifstream& file, Map& map);
        void readTDCI(std::ifstream& file, Map& map);
        void readTPCI(std::ifstream& file, Map& map);
        void readANIM(std::ifstream& file, Map& map);

        void writeHEAD(std::ofstream& file, Map& map);
        void writeMAPP(std::ofstream& file, Map& map);
        void writeLYRS(std::ofstream& file, Map& map);
        void writeBGDS(std::ofstream& file, Map& map);
        void writeMTCL(std::ofstream& file, Map& map);
        void writeMDCL(std::ofstream& file, Map& map);
        void writeMPCL(std::ofstream& file, Map& map);
        void writeTTCI(std::ofstream& file, Map& map);
        void writeTDCI(std::ofstream& file, Map& map);
        void writeTPCI(std::ofstream& file, Map& map);
        void writeANIM(std::ofstream& file, Map& map);
};

#endif
