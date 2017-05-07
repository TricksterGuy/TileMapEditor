#include "ChunkStream.hpp"
#include <cstring>

#ifdef LINUX
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#ifdef WINDOWS
#include <winsock2.h>
#endif
#endif

ChunkStreamReader::ChunkStreamReader(std::istream& _stream, uint32_t size_name, uint32_t _flags) : stream(_stream), size(0), consumed_size(0), flags(_flags), width(0)
{
    char chunk_name[size_name];
    stream.read(chunk_name, size_name);
    name.assign(chunk_name, size_name);
    stream.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));
    size = ntohl(size);
    VerboseLog("Read chunk %s size %zd", name.c_str(), size);
}

ChunkStreamReader& ChunkStreamReader::operator>>(bool& val)
{
    stream.read(reinterpret_cast<char*>(&val), sizeof(val));
    VerboseLog("Reading %d size: %zd bytes", val, sizeof(val));
    consumed_size += sizeof(val);
    if (consumed_size > size)
        DebugFatalLog("Read past end of chunk %s %zd bytes read out of %zd", name.c_str(), consumed_size, size);
    return *this;
}

ChunkStreamReader& ChunkStreamReader::operator>>(char& val)
{
    stream.read(&val, sizeof(val));
    VerboseLog("Reading %d size: %zd bytes", val, sizeof(val));
    consumed_size += sizeof(val);
    if (consumed_size > size)
        DebugFatalLog("Read past end of chunk %s %zd bytes read out of %zd", name.c_str(), consumed_size, size);
    return *this;
}

ChunkStreamReader& ChunkStreamReader::operator>>(unsigned char& val)
{
    stream.read(reinterpret_cast<char*>(&val), sizeof(val));
    VerboseLog("Reading %d size: %zd bytes", val, sizeof(val));
    consumed_size += sizeof(val);
    if (consumed_size > size)
        DebugFatalLog("Read past end of chunk %s %zd bytes read out of %zd", name.c_str(), consumed_size, size);
    return *this;
}

ChunkStreamReader& ChunkStreamReader::operator>>(short& val)
{
    stream.read(reinterpret_cast<char*>(&val), sizeof(val));
    val = ntohs(val);
    VerboseLog("Reading %d size: %zd bytes", val, sizeof(val));
    consumed_size += sizeof(val);
    if (consumed_size > size)
        DebugFatalLog("Read past end of chunk %s %zd bytes read out of %zd", name.c_str(), consumed_size, size);
    return *this;
}

ChunkStreamReader& ChunkStreamReader::operator>>(unsigned short& val)
{
    stream.read(reinterpret_cast<char*>(&val), sizeof(val));
    val = ntohs(val);
    VerboseLog("Reading %d size: %zd bytes", val, sizeof(val));
    consumed_size += sizeof(val);
    if (consumed_size > size)
        DebugFatalLog("Read past end of chunk %s %zd bytes read out of %zd", name.c_str(), consumed_size, size);
    return *this;
}

ChunkStreamReader& ChunkStreamReader::operator>>(int& val)
{
    stream.read(reinterpret_cast<char*>(&val), sizeof(val));
    val = ntohl(val);
    VerboseLog("Reading %d size: %zd bytes", val, sizeof(val));
    consumed_size += sizeof(val);
    if (consumed_size > size)
        DebugFatalLog("Read past end of chunk %s %zd bytes read out of %zd", name.c_str(), consumed_size, size);
    return *this;
}

ChunkStreamReader& ChunkStreamReader::operator>>(unsigned int& val)
{
    stream.read(reinterpret_cast<char*>(&val), sizeof(val));
    val = ntohl(val);
    VerboseLog("Reading %d size: %zd bytes", val, sizeof(val));
    consumed_size += sizeof(val);
    if (consumed_size > size)
        DebugFatalLog("Read past end of chunk %s %zd bytes read out of %zd", name.c_str(), consumed_size, size);
    return *this;
}

ChunkStreamReader& ChunkStreamReader::operator>>(float& val)
{
    union
    {
        float f;
        int32_t i;
    } convert;

    stream.read((char*)(&convert.i), sizeof(int32_t));
    convert.i = ntohl(convert.i);
    val = convert.f;
    VerboseLog("Reading %.8f size: %zd bytes", val, sizeof(val));
    consumed_size += sizeof(val);
    if (consumed_size > size)
        DebugFatalLog("Read past end of chunk %s %zd bytes read out of %zd", name.c_str(), consumed_size, size);
    return *this;
}

ChunkStreamReader& ChunkStreamReader::operator>>(std::string& val)
{
    uint32_t effSize = width;
    if (flags & ChunkStreamReader::READ_STRING_SIZES && width == 0)
    {
        (*this) >> effSize;
        VerboseLog("Reading size for string got: %zd", effSize);
    }

    char data[effSize];
    stream.read(data, effSize);
    val.assign(data, effSize);

    VerboseLog("Reading %s size: %zd bytes", val.c_str(), effSize);
    consumed_size += effSize;
    width = 0;
    if (consumed_size > size)
        DebugFatalLog("Read past end of chunk %s %zd bytes read out of %zd", name.c_str(), consumed_size, size);
    return *this;
}

ChunkStreamReader& ChunkStreamReader::operator>>(ChunkStreamReader& (*pf)(ChunkStreamReader&))
{
    return pf(*this);
}

ChunkStreamWriter& ChunkStreamWriter::operator<<(bool val)
{
    VerboseLog("Writing %d size: %zd bytes", val, sizeof(val));
    size += sizeof(val);
    out.write(reinterpret_cast<char*>(&val), sizeof(val));
    return *this;
}

ChunkStreamWriter& ChunkStreamWriter::operator<<(char val)
{
    VerboseLog("Writing %d size: %zd bytes", val, sizeof(val));
    size += sizeof(val);
    out.write(reinterpret_cast<char*>(&val), sizeof(val));
    return *this;
}

ChunkStreamWriter& ChunkStreamWriter::operator<<(unsigned char val)
{
    VerboseLog("Writing %d size: %zd bytes", val, sizeof(val));
    size += sizeof(val);
    out.write(reinterpret_cast<char*>(&val), sizeof(val));
    return *this;
}

ChunkStreamWriter& ChunkStreamWriter::operator<<(short val)
{
    VerboseLog("Writing %d size: %zd bytes", val, sizeof(val));
    size += sizeof(val);
    val = htons(val);
    out.write(reinterpret_cast<char*>(&val), sizeof(val));
    return *this;
}

ChunkStreamWriter& ChunkStreamWriter::operator<<(unsigned short val)
{
    VerboseLog("Writing %zd size: %zd bytes", val, sizeof(val));
    size += sizeof(val);
    val = htons(val);
    out.write(reinterpret_cast<char*>(&val), sizeof(val));
    return *this;
}

ChunkStreamWriter& ChunkStreamWriter::operator<<(int val)
{
    VerboseLog("Writing %d size: %zd bytes", val, sizeof(val));
    size += sizeof(val);
    val = htonl(val);
    out.write(reinterpret_cast<char*>(&val), sizeof(val));
    return *this;
}

ChunkStreamWriter& ChunkStreamWriter::operator<<(unsigned int val)
{
    VerboseLog("Writing %zd size: %zd bytes", val, sizeof(val));
    size += sizeof(val);
    val = htonl(val);
    out.write(reinterpret_cast<char*>(&val), sizeof(val));
    return *this;
}

ChunkStreamWriter& ChunkStreamWriter::operator<<(float val)
{
    VerboseLog("Writing %.8f size: %zd bytes", val, sizeof(val));
    union
    {
        float f;
        int i;
    } convert;
    convert.f = val;

    size += sizeof(convert.i);
    convert.i = htonl(convert.i);
    out.write(reinterpret_cast<char*>(&convert.i), sizeof(convert.i));
    return *this;
}

ChunkStreamWriter& ChunkStreamWriter::operator<<(const char* val)
{
    uint32_t strsize = width == 0 ? strlen(val) : width;
    VerboseLog("Writing %s size: %zd bytes", val, strsize);
    if (flags & ChunkStreamWriter::WRITE_STRING_SIZES)
        (*this) << strsize;
    size += strsize;
    out.write(val, strsize);
    width = 0;
    return *this;
}

ChunkStreamWriter& ChunkStreamWriter::operator<<(const std::string& val)
{
    uint32_t effWidth = width == 0 ? val.size() : width;
    VerboseLog("Writing %s size: %zd bytes", val.c_str(), effWidth);
    if (flags & ChunkStreamWriter::WRITE_STRING_SIZES)
        (*this) << effWidth;
    size += effWidth;
    out.write(val.c_str(), effWidth);
    width = 0;
    return *this;
}

ChunkStreamWriter& ChunkStreamWriter::operator<<(ChunkStreamWriter& (*pf)(ChunkStreamWriter&))
{
    return pf(*this);
}

std::ostream& operator<<(std::ostream& os, const ChunkStreamWriter& cs)
{
    const std::string& name = cs.Name();
    uint32_t size = htonl(cs.Size());
    std::string data = cs.Data();

    os.write(name.c_str(), name.size());
    os.write(reinterpret_cast<char*>(&size), sizeof(uint32_t));
    os.write(data.c_str(), cs.Size());

    return os;
}

ChunkStreamReader& operator>>(ChunkStreamReader& cs, const set_width& width)
{
    cs.SetWidth(width.get_width());
    return cs;
}

ChunkStreamWriter& operator<<(ChunkStreamWriter& cs, const set_width& width)
{
    cs.SetWidth(width.get_width());
    return cs;
}

ChunkStreamReader& operator>>(ChunkStreamReader& cs, const set_flags& flags)
{
    cs.SetFlags(flags.get_flags());
    return cs;
}

ChunkStreamWriter& operator<<(ChunkStreamWriter& cs, const set_flags& flags)
{
    cs.SetFlags(flags.get_flags());
    return cs;
}
