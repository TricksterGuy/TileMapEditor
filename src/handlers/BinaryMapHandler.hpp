/******************************************************************************************************
 * Tile Map Editor
 * Copyright (C) 2009-2014 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
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
        virtual void Load(const std::string& filename, Map& map);
        /** See BaseMapHandler::save */
        virtual void Save(const std::string& filename, Map& map);

	private:
        void ReadChunkName(std::ifstream& file, std::string& name, uint32_t& size);
        void ReadHEAD(std::ifstream& file, Map& map);
        void ReadMAPP(std::ifstream& file, Map& map, int32_t& num_layers, int32_t& num_backgrounds);
        void ReadLYRS(std::ifstream& file, Map& map, int32_t& num_layers);
        void ReadBGDS(std::ifstream& file, Map& map, int32_t& num_backgrounds);
        void ReadMTCL(std::ifstream& file, Map& map);
        void ReadMDCL(std::ifstream& file, Map& map);
        void ReadMPCL(std::ifstream& file, Map& map);
        void ReadTTCI(std::ifstream& file, Map& map);
        void ReadTDCI(std::ifstream& file, Map& map);
        void ReadTPCI(std::ifstream& file, Map& map);
        void ReadANIM(std::ifstream& file, Map& map);

        void WriteHEAD(std::ofstream& file, Map& map);
        void WriteMAPP(std::ofstream& file, Map& map);
        void WriteLYRS(std::ofstream& file, Map& map);
        void WriteBGDS(std::ofstream& file, Map& map);
        void WriteMTCL(std::ofstream& file, Map& map);
        void WriteMDCL(std::ofstream& file, Map& map);
        void WriteMPCL(std::ofstream& file, Map& map);
        void WriteTTCI(std::ofstream& file, Map& map);
        void WriteTDCI(std::ofstream& file, Map& map);
        void WriteTPCI(std::ofstream& file, Map& map);
        void WriteANIM(std::ofstream& file, Map& map);
};

#endif
