//
// Created by Aman Dhruva Thamminana on 10/14/23.
//


#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>


#include "core/EasyLogging.hpp"


using namespace clogged;

TEST_CASE("Logger class tests", "[Logger]") {
  // Test case 1: Check if Logger can log messages with different log levels, teams, and colors

  SECTION("Logger message logging") {
    // Redirect standard output to a stringstream to capture the log messages
    std::stringstream output;
    auto old_cout = std::cout.rdbuf(output.rdbuf());

    // Log messages
    Logger::Log() << Team::TEAM_1 << LogLevel::DEBUG << Color::BLUE
                  << "This is a debug message from Team A." << "aye 2" << Logger::endl;
    Logger::Log() << Team::TEAM_2 << LogLevel::INFO << Color::GREEN
                  << "This is an info message from Team B." << Logger::endl;
    Logger::Log() << Team::TEAM_3 << LogLevel::ERROR << Color::RED
                  << "RED Error message from Team C." << Logger::endl;
    Logger::Log() << Team::TEAM_4 << LogLevel::ERROR << Color::BLUE
                  << "Error message from Team C. no endl";
    Logger::Log() << "cont no endl Error message from Team";
    Logger::Log() << std::endl << "Warning message standard overload" << std::endl;
    Logger::Log() << Team::TEAM_5 << LogLevel::WARNING << Color::BLUE
                  << "Warning message from Team D." << Logger::endl;

    // Reset standard output
    std::cout.rdbuf(old_cout);

    // Get the captured log messages from the stringstream
    std::string logOutput = output.str();
#ifndef NDEBUG
    // Check if the log messages match the expected output
    REQUIRE(logOutput == "\x1B[34m[Team 1](DEBUG) This is a debug message from Team A.\x1B[0m\x1B[34maye 2\x1B[0m\n"
                         "\x1B[32m[Team 2](INFO) This is an info message from Team B.\x1B[0m\n"
                         "\x1B[31m[Team 3](ERROR) RED Error message from Team C.\x1B[0m\n"
                         "\x1B[34m[Team 4](ERROR) Error message from Team C. no endl\x1B[0m"
                         "\x1B[34mcont no endl Error message from Team\x1B[0m\n"
                         "\x1B[0m(DEBUG) Warning message standard overload\x1B[0m\n"
                         "\x1B[34m[Team 5](WARNING) Warning message from Team D.\x1B[0m\n");
  

#else
    REQUIRE(logOutput == "");
#endif
  }

}

