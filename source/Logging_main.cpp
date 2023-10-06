
#include "./core/EasyLogging.hpp"


using namespace clogged;

void samplefunction() {
    Logger::log << LOG_FNC << "This log message includes file and line number." << Logger::endl;
}


int main() {
    Logger::log << Team::TEAM_1 << LogLevel::DEBUG << Color::BLUE << "This is a debug message from Team A." << "aye 2" << Logger::endl;
    Logger::log << Team::TEAM_2 << LogLevel::INFO << Color::GREEN << "This is an info message from Team B." << Logger::endl;
    Logger::log << Team::TEAM_3 << LogLevel::ERROR << Color::RED << " RED Error message from Team C." << Logger::endl;
    Logger::log << Team::TEAM_4 << LogLevel::ERROR << Color::BLUE << "Error message from Team C. no endl";
    Logger::log << "cont no endl Error message from Team";

    Logger::log << std::endl <<  "Warning message standard overload" << Logger::endl;

    samplefunction();




//    // Example 1: Logging a debug message for Team 1 in blue color
//    Logger::log << Team::TEAM_1 << LogLevel::DEBUG << Color::BLUE << "Debug message for Team 1" << Logger::endl;
//
//    // Example 2: Logging an error message for Team 5 in red color
//    Logger::log << Team::TEAM_5 << LogLevel::ERROR << Color::RED << "Error message for Team 5" << Logger::endl;
//
//    // Example 3: Logging an info message for General team in green color
//    Logger::log << Team::GENERAL << LogLevel::INFO << Color::GREEN << "Info message for General team" << Logger::endl;
//
//    // Example 4: Logging a warning message without specifying team and color
//    Logger::log << LogLevel::WARNING << "Generic warning message level Warning" << Logger::endl;
//
//    // Example 4: Logging a warning message without specifying team and color
//    Logger::log << LogLevel::NA  << "Generic warning message" << Logger::endl;




    return 0;
}
