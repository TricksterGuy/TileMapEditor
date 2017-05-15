#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <iostream>
#include <fstream>
#include "Logger.hpp"
#include "MapHandlerManager.hpp"
#include "Handlers.hpp"

struct Global
{
    Global() : test_log("test.log")
    {
        boost::unit_test::unit_test_log.set_stream(test_log);
        //logger->SetLogLevel(LogLevel::VERBOSE_LEVEL);

        MapHandlerManager().Add(new BinaryMapHandler());
        MapHandlerManager().Add(new TextMapHandler());
    }

    ~Global()
    {
        boost::unit_test::unit_test_log.set_stream(std::cout);
    }

    std::ofstream test_log;
};

BOOST_GLOBAL_FIXTURE(Global);
