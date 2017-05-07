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
#ifndef CHUNK_STREAM_HPP
#define CHUNK_STREAM_HPP

#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Logger.hpp"

class set_flags;
class set_width;

class ChunkStreamReader
{
public:
    ChunkStreamReader(std::istream& _stream, uint32_t size_name, uint32_t flags = READ_SIZES);
    ChunkStreamReader& operator>>(bool& val);
    ChunkStreamReader& operator>>(char& val);
    ChunkStreamReader& operator>>(unsigned char& val);
    ChunkStreamReader& operator>>(short& val);
    ChunkStreamReader& operator>>(unsigned short& val);
    ChunkStreamReader& operator>>(int& val);
    ChunkStreamReader& operator>>(unsigned int& val);
    ChunkStreamReader& operator>>(float& val);
    ChunkStreamReader& operator>>(std::string& val);
    ChunkStreamReader& operator>>(ChunkStreamReader& (*pf)(ChunkStreamReader&));
    void SetFlags(uint32_t _flags) { flags = _flags; }
    void SetWidth(uint32_t _width) { width = _width; }
    const std::string& Name() const { return name; }
    uint32_t Size() const { return size; }
    uint32_t ConsumedSize() const { return consumed_size; }
    uint32_t Flags() const { return flags; }
    uint32_t Width() const { return width; }
    bool Ok() const { return !stream.fail() && consumed_size == size; }
    enum
    {
        NO_READ_STRING_SIZES = 0,
        NO_READ_VECTOR_SIZES = 0,
        NO_READ_SIZES = 0,
        READ_STRING_SIZES = 1,
        READ_VECTOR_SIZES = 2,
        READ_SIZES = 3,
    };

private:
    std::istream& stream;
    std::string name;
    uint32_t size;
    uint32_t consumed_size;
    uint32_t flags;
    uint32_t width;
};

/** Helper class to write chunked data easily.
  * This data has a header specified by the name parameter, followed by the size of the data afterward
  * This class assumes all sizes are 32 bit unsigned integers.
  */
class ChunkStreamWriter
{
public:
    ChunkStreamWriter(const std::string& _name, uint32_t _flags = WRITE_STRING_SIZES) : out(std::stringstream::out | std::stringstream::binary), name(_name), size(0), flags(_flags), width(0) {}
    ChunkStreamWriter& operator<<(bool val);
    ChunkStreamWriter& operator<<(char val);
    ChunkStreamWriter& operator<<(unsigned char val);
    ChunkStreamWriter& operator<<(short val);
    ChunkStreamWriter& operator<<(unsigned short val);
    ChunkStreamWriter& operator<<(int val);
    ChunkStreamWriter& operator<<(unsigned int val);
    ChunkStreamWriter& operator<<(float val);
    ChunkStreamWriter& operator<<(const char* val);
    ChunkStreamWriter& operator<<(const std::string& val);
    ChunkStreamWriter& operator<<(ChunkStreamWriter& (*pf)(ChunkStreamWriter&));
    void SetFlags(uint32_t _flags) { flags = _flags; }
    void SetWidth(uint32_t _width) { width = _width; }
    const std::string& Name() const { return name; }
    uint32_t Size() const { return size; }
    std::string Data() const { return out.str(); }
    uint32_t Flags() const { return flags; }
    uint32_t Width() const { return width; }
    enum
    {
        NO_WRITE_STRING_SIZES = 0,
        NO_WRITE_VECTOR_SIZES = 0,
        NO_WRITE_SIZES = 0,
        WRITE_STRING_SIZES = 1,
        WRITE_VECTOR_SIZES = 2,
        WRITE_SIZES = 3,
    };

private:
    std::stringstream out;
    std::string name;
    uint32_t size;
    uint32_t flags;
    uint32_t width;
};

template<typename VecType>
ChunkStreamReader& operator>>(ChunkStreamReader& cs, std::vector<VecType>& vec)
{
    uint32_t size;

    if (cs.Flags() & ChunkStreamReader::READ_VECTOR_SIZES)
    {
        cs >> size;
        VerboseLog("Reading number of elements for vector got: %zd", size);
    }
    else
        size = vec.size();

    vec.resize(size);
    VerboseLog("Reading %zd elements for vector", size);
    VecType el;
    for (uint32_t i = 0; i < size; i++)
    {
        cs >> el;
        vec[i] = el;
    }

    if (cs.ConsumedSize() > cs.Size())
        DebugFatalLog("Read past end of chunk %s %zd bytes read out of %zd", cs.Name().c_str(), cs.ConsumedSize(), cs.Size());

    return cs;
}


template<typename VecType>
ChunkStreamWriter& operator<<(ChunkStreamWriter& cs, const std::vector<VecType>& vec)
{
    if (cs.Flags() & ChunkStreamWriter::WRITE_VECTOR_SIZES)
        cs << (uint32_t) vec.size();
    for (const VecType& el : vec)
      cs << el;
    return cs;
}

class set_width
{
public:
    explicit set_width(uint32_t _width): width(_width) {}
    uint32_t get_width() const { return width; }
private:
    uint32_t width;
};

class set_flags
{
public:
    explicit set_flags(uint32_t _flags): flags(_flags) {}
    uint32_t get_flags() const { return flags; }
private:
    uint32_t flags;
};

std::ostream& operator<<(std::ostream& os, const ChunkStreamWriter& cs);
ChunkStreamReader& operator>>(ChunkStreamReader& cs, const set_width& width);
ChunkStreamWriter& operator<<(ChunkStreamWriter& cs, const set_width& width);
ChunkStreamReader& operator>>(ChunkStreamReader& cs, const set_flags& flags);
ChunkStreamWriter& operator<<(ChunkStreamWriter& cs, const set_flags& flags);

#endif
