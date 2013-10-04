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
        virtual int load(const std::string& filename, Map& map);
        /** See BaseMapHandler::save */
        virtual int save(const std::string& filename, Map& map);

	private:
        int readChunkName(std::ifstream& file, std::string& name, uint32_t& size);
        int readHEAD(std::ifstream& file, Map& map);
        int readMAPP(std::ifstream& file, Map& map, int32_t& num_layers, int32_t& num_backgrounds);
        int readLYRS(std::ifstream& file, Map& map, int32_t& num_layers);
        int readBGDS(std::ifstream& file, Map& map, int32_t& num_backgrounds);
        int readMTCL(std::ifstream& file, Map& map);
        int readMDCL(std::ifstream& file, Map& map);
        int readMPCL(std::ifstream& file, Map& map);
        int readTTCI(std::ifstream& file, Map& map);
        int readTDCI(std::ifstream& file, Map& map);
        int readTPCI(std::ifstream& file, Map& map);
        int readANIM(std::ifstream& file, Map& map);

        int writeHEAD(std::ofstream& file, Map& map);
        int writeMAPP(std::ofstream& file, Map& map);
        int writeLYRS(std::ofstream& file, Map& map);
        int writeBGDS(std::ofstream& file, Map& map);
        int writeMTCL(std::ofstream& file, Map& map);
        int writeMDCL(std::ofstream& file, Map& map);
        int writeMPCL(std::ofstream& file, Map& map);
        int writeTTCI(std::ofstream& file, Map& map);
        int writeTDCI(std::ofstream& file, Map& map);
        int writeTPCI(std::ofstream& file, Map& map);
        int writeANIM(std::ofstream& file, Map& map);
};

#endif
