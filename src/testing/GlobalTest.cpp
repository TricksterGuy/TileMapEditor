// *** ADDED BY HEADER FIXUP ***
#include "istream"
// *** END ***
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <iostream>
#include <fstream>
#include "Logger.hpp"

struct Global
{
  Global() : test_log("test.log")
  {
    boost::unit_test::unit_test_log.set_stream(test_log);
    logger->SetLogLevel(LogLevel::VERBOSE);
  }

  ~Global()
  {
    boost::unit_test::unit_test_log.set_stream(std::cout);
  }

  std::ofstream test_log;
};

BOOST_GLOBAL_FIXTURE(Global);
