#define BOOST_TEST_DYN_LINK
#include <boost/test/auto_unit_test.hpp>
#include "ChunkStream.hpp"

struct ChunkStreamTest
{
    ChunkStreamWriter cs;
    uint32_t header_size;

    ChunkStreamTest() : cs("TEST"), header_size(cs.Size()) {}
    ~ChunkStreamTest() {}
};

BOOST_FIXTURE_TEST_CASE(TestNoDataSize, ChunkStreamTest)
{
    BOOST_CHECK_EQUAL(cs.Size(), header_size);
}

BOOST_FIXTURE_TEST_CASE(TestAddChars, ChunkStreamTest)
{
    cs << 'H';
    BOOST_CHECK_EQUAL(cs.Size(), 1);
    cs << 'E';
    BOOST_CHECK_EQUAL(cs.Size(), 2);
    cs << 'L';
    BOOST_CHECK_EQUAL(cs.Size(), 3);
    cs << 'O';
    BOOST_CHECK_EQUAL(cs.Size(), 4);

    BOOST_CHECK_EQUAL(cs.Data(), "HELO");
}

BOOST_FIXTURE_TEST_CASE(TestAddString, ChunkStreamTest)
{
    cs << "HELLO WORLD";

    BOOST_CHECK_EQUAL(cs.Size(), 15);

    std::string str = cs.Data();
    std::string expected("\000\000\000\013HELLO WORLD", 15);
    BOOST_CHECK_EQUAL(str.size(), 15);
    BOOST_CHECK_EQUAL(str, expected);
}

BOOST_FIXTURE_TEST_CASE(TestAddEmptyString, ChunkStreamTest)
{
    cs << "";

    BOOST_CHECK_EQUAL(cs.Size(), 4);

    std::string str = cs.Data();
    std::string expected("\000\000\000\000", 4);
    BOOST_CHECK_EQUAL(str.size(), 4);
    BOOST_CHECK_EQUAL(str, expected);
}

BOOST_FIXTURE_TEST_CASE(TestAddStringPartial, ChunkStreamTest)
{
    cs << set_width(5) << "HELLO WORLD";

    BOOST_CHECK_EQUAL(cs.Size(), 9);

    std::string str = cs.Data();
    std::string expected("\000\000\000\005HELLO", 9);
    BOOST_CHECK_EQUAL(str.size(), 9);
    BOOST_CHECK_EQUAL(str, expected);
}

BOOST_FIXTURE_TEST_CASE(TestAddShort, ChunkStreamTest)
{
    cs << (short) 0x5621;

    BOOST_CHECK_EQUAL(cs.Size(), sizeof(short));

    std::string str = cs.Data();
    std::string expected("\126\041", 2);
    BOOST_CHECK_EQUAL(str.size(), 2);
    BOOST_CHECK_EQUAL(str, expected);
}


BOOST_FIXTURE_TEST_CASE(TestAddInt, ChunkStreamTest)
{
    cs << 0x83475621;

    BOOST_CHECK_EQUAL(cs.Size(), sizeof(int));

    std::string str = cs.Data();
    std::string expected("\203\107\126\041", 4);
    BOOST_CHECK_EQUAL(str.size(), 4);
    BOOST_CHECK_EQUAL(str, expected);
}


BOOST_FIXTURE_TEST_CASE(TestBool, ChunkStreamTest)
{
    cs << true;

    BOOST_CHECK_EQUAL(cs.Size(), sizeof(bool));

    std::string str = cs.Data();
    std::string expected("\001", 1);
    BOOST_CHECK_EQUAL(str.size(), 1);
    BOOST_CHECK_EQUAL(str, expected);
}

BOOST_FIXTURE_TEST_CASE(TestAddFloat, ChunkStreamTest)
{
    float x = 11.63671875f;
    cs << x;

    BOOST_CHECK_EQUAL(cs.Size(), sizeof(int));

    std::string str = cs.Data();
    // 0 1011.10100011
    // 0 01110100011 * 2**3
    // 0 10000010 01110100011000000000000
    // 01000001 00111010 00110000 00000000
    std::string expected("\101\072\060\000", 4);
    BOOST_CHECK_EQUAL(str.size(), 4);
    BOOST_CHECK_EQUAL(str, expected);
}

BOOST_FIXTURE_TEST_CASE(TestAddVectorInt, ChunkStreamTest)
{
    std::vector<unsigned int> x = {0x12345678, 3, 0x0, 0x80000000};
    cs << x;

    BOOST_CHECK_EQUAL(cs.Size(), 4 * sizeof(int));

    std::string str = cs.Data();
    std::string expected("\022\064\126\170\000\000\000\003\000\000\000\000\200\000\000\000", 16);
    BOOST_CHECK_EQUAL(str.size(), 4 * sizeof(int));
    BOOST_CHECK_EQUAL(str, expected);
}

BOOST_FIXTURE_TEST_CASE(TestAddVectorIntWithSize, ChunkStreamTest)
{
    cs << set_flags(ChunkStreamWriter::WRITE_VECTOR_SIZES);
    std::vector<uint32_t> x = {0x12345678, 3, 0x0, 0x80000000};
    cs << x;

    BOOST_CHECK_EQUAL(cs.Size(), 5 * sizeof(uint32_t));

    std::string str = cs.Data();
    std::string expected("\000\000\000\004\022\064\126\170\000\000\000\003\000\000\000\000\200\000\000\000", 20);
    BOOST_CHECK_EQUAL(str.size(), 5 * sizeof(int));
    BOOST_CHECK_EQUAL(str, expected);
}

BOOST_FIXTURE_TEST_CASE(TestAddMany, ChunkStreamTest)
{
    std::vector<unsigned int> x = {0x12345678, 3, 0x0, 0x80000000};
    cs << (unsigned int)x.size();
    cs << x;
    cs << 'c';
    cs << 'n';
    cs << 't';
    cs << 62;
    cs << (short)0xf052;

    BOOST_CHECK_EQUAL(cs.Size(), 29);

    std::string str = cs.Data();
    BOOST_CHECK_EQUAL(str.size(), 29);

    std::string expected("\000\000\000\004" "\022\064\126\170" "\000\000\000\003" "\000\000\000\000" "\200\000\000\000" "cnt" "\000\000\000\076" "\360\122", 29);
    BOOST_CHECK_EQUAL(str, expected);
}

BOOST_FIXTURE_TEST_CASE(TestWrite, ChunkStreamTest)
{
    std::vector<unsigned int> x = {0x12345678, 3, 0x0, 0x80000000};
    cs << (unsigned int)x.size();
    cs << x;
    cs << 'c';
    cs << 'n';
    cs << 't';
    cs << 62;
    cs << (short)0xf052;

    std::stringstream chunk(std::stringstream::out | std::stringstream::binary);
    chunk << cs;

    std::string str = chunk.str();
    BOOST_CHECK_EQUAL(str.size(), 37);

    std::string expected("TEST" "\000\000\000\035" "\000\000\000\004" "\022\064\126\170" "\000\000\000\003" "\000\000\000\000" "\200\000\000\000" "cnt" "\000\000\000\076" "\360\122", 37);
    BOOST_CHECK_EQUAL(str, expected);
}

BOOST_FIXTURE_TEST_CASE(TestReadNoDataSize, ChunkStreamTest)
{
    std::stringstream stream(std::string("TEST\000\000\000\000", 8));
    ChunkStreamReader csr(stream, 4);

    BOOST_CHECK_EQUAL(csr.Name(), "TEST");
    BOOST_CHECK_EQUAL(csr.Size(), 0);
    BOOST_CHECK(csr.Ok());
}

BOOST_FIXTURE_TEST_CASE(TestReadChars, ChunkStreamTest)
{
    std::stringstream stream(std::string("TEST\000\000\000\003" "cnt", 8 + 3));
    ChunkStreamReader csr(stream, 4);

    char c;
    csr >> c;
    BOOST_CHECK_EQUAL(c, 'c');
    csr >> c;
    BOOST_CHECK_EQUAL(c, 'n');
    csr >> c;
    BOOST_CHECK_EQUAL(c, 't');
    BOOST_CHECK(csr.Ok());
}

BOOST_FIXTURE_TEST_CASE(TestReadEmptyString, ChunkStreamTest)
{
    std::stringstream stream(std::string("TEST\000\000\000\004" "\000\000\000\000", 8 + 4));
    ChunkStreamReader csr(stream, 4);

    std::string hello = "X";
    csr >> hello;
    BOOST_CHECK_EQUAL(hello, "");
    BOOST_CHECK(csr.Ok());
}

BOOST_FIXTURE_TEST_CASE(TestReadString, ChunkStreamTest)
{
    std::stringstream stream(std::string("TEST\000\000\000\011" "\000\000\000\004" "HELLO", 8 + 9));
    ChunkStreamReader csr(stream, 4);

    std::string hell;
    char c;
    csr >> hell;
    BOOST_CHECK_EQUAL(hell, "HELL");
    csr >> c;
    BOOST_CHECK_EQUAL(c, 'O');
    BOOST_CHECK(csr.Ok());
}

BOOST_FIXTURE_TEST_CASE(TestReadStringWithSize, ChunkStreamTest)
{
    std::stringstream stream(std::string("TEST\000\000\000\005" "HELLO", 8 + 5));
    ChunkStreamReader csr(stream, 4);

    std::string hello;
    csr >> set_width(5) >> hello;
    BOOST_CHECK_EQUAL(hello, "HELLO");
    BOOST_CHECK(csr.Ok());
}

BOOST_FIXTURE_TEST_CASE(TestReadMany, ChunkStreamTest)
{
    std::stringstream stream(std::string("TEST\000\000\000\012" "\377\377\377\377" "\101\072\060\000" "\126\041", 8 + 10));
    ChunkStreamReader csr(stream, 4);


    uint32_t x;
    float y;
    short z;

    csr >> x;
    BOOST_CHECK_EQUAL(x, 0xFFFFFFFFU);
    csr >> y;
    BOOST_CHECK_EQUAL(y, 11.63671875f);
    csr >> z;
    BOOST_CHECK_EQUAL(z, 0x5621);
    BOOST_CHECK(csr.Ok());
}

BOOST_FIXTURE_TEST_CASE(TestReadVector, ChunkStreamTest)
{
    std::stringstream stream(std::string("TEST\000\000\000\011" "\000\000\000\005" "\377A\000B ", 8 + 9));
    ChunkStreamReader csr(stream, 4);

    std::vector<char> arr;
    csr >> arr;

    char expected[5] = {-1, 'A', 0, 'B', ' '};
    BOOST_CHECK_EQUAL_COLLECTIONS(arr.begin(), arr.end(), expected, expected + 5);

    BOOST_CHECK(csr.Ok());
}

BOOST_FIXTURE_TEST_CASE(TestReadVectorInt, ChunkStreamTest)
{
    std::stringstream stream(std::string("TEST\000\000\000\024" "\000\000\000\004" "\022\064\126\170" "\000\000\000\003" "\000\000\000\000" "\200\000\000\000", 8 + 20));
    ChunkStreamReader csr(stream, 4);

    std::vector<int> arr;
    csr >> arr;

    std::vector<unsigned int> expected = {0x12345678, 3, 0x0, 0x80000000};
    BOOST_CHECK_EQUAL_COLLECTIONS(arr.begin(), arr.end(), expected.begin(), expected.end());

    BOOST_CHECK(csr.Ok());
}

BOOST_FIXTURE_TEST_CASE(TestReadVectorIntWithVecSize, ChunkStreamTest)
{
    std::stringstream stream(std::string("TEST\000\000\000\020" "\022\064\126\170" "\000\000\000\003" "\000\000\000\000" "\200\000\000\000", 8 + 16));
    ChunkStreamReader csr(stream, 4, ChunkStreamReader::NO_READ_SIZES);

    std::vector<int> arr(3);
    int last;
    csr >> arr;
    csr >> last;

    std::vector<unsigned int> expected = {0x12345678, 3, 0x0};
    BOOST_CHECK_EQUAL_COLLECTIONS(arr.begin(), arr.end(), expected.begin(), expected.end());
    BOOST_CHECK_EQUAL(last, 0x80000000);

    BOOST_CHECK(csr.Ok());
}
