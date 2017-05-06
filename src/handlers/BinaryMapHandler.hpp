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
#ifndef BINARY_MAP_HANDLER_HPP
#define BINARY_MAP_HANDLER_HPP

#include "BaseMapHandler.hpp"
#include "ChunkStream.hpp"

/** Handler for .map files
  * My own map format that saves to a binary file
  */
class BinaryMapHandler : public BaseMapHandler {
public:
    BinaryMapHandler();

    /** See BaseMapHandler::Load */
    virtual void Load(const std::string& filename, Map& map);
    /** See BaseMapHandler::Load */
    virtual void Load(std::istream& file, Map& map);
    /** See BaseMapHandler::Save */
    virtual void Save(const std::string& filename, const Map& map);
    /** See BaseMapHandler::Save */
    virtual void Save(std::ostream& file, const Map& map);

private:
    void ReadHEAD(ChunkStreamReader& file, Map& map);
    void ReadMAPP(ChunkStreamReader& file, Map& map);
    void ReadLYRS(ChunkStreamReader& file, Map& map);
    void ReadBGDS(ChunkStreamReader& file, Map& map);
    void ReadMTCL(ChunkStreamReader& file, Map& map);
    void ReadMDCL(ChunkStreamReader& file, Map& map);
    void ReadMPCL(ChunkStreamReader& file, Map& map);
    void ReadTTCI(ChunkStreamReader& file, Map& map);
    void ReadTDCI(ChunkStreamReader& file, Map& map);
    void ReadTPCI(ChunkStreamReader& file, Map& map);
    void ReadANIM(ChunkStreamReader& file, Map& map);

    void WriteHEAD(std::ostream& file, const Map& map);
    void WriteMAPP(std::ostream& file, const Map& map);
    void WriteLYRS(std::ostream& file, const Map& map);
    void WriteBGDS(std::ostream& file, const Map& map);
    void WriteMTCL(std::ostream& file, const Map& map);
    void WriteMDCL(std::ostream& file, const Map& map);
    void WriteMPCL(std::ostream& file, const Map& map);
    void WriteTTCI(std::ostream& file, const Map& map);
    void WriteTDCI(std::ostream& file, const Map& map);
    void WriteTPCI(std::ostream& file, const Map& map);
    void WriteANIM(std::ostream& file, const Map& map);
};

#endif
