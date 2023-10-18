
#include "./core/EasyLogging.hpp"


using namespace clogged;

void samplefunction() {
    Logger::Log() << LOG_FNC << "This Log() message includes file and line number." << std::endl;
}


int main() {
    Logger::Log() << Team::TEAM_1 << LogLevel::DEBUG << Color::BLUE << "This is a debug message from Team A." << "aye 2" << std::endl;
    Logger::Log() << Team::TEAM_2 << LogLevel::INFO << Color::GREEN << "This is an info message from Team B." << std::endl;
    Logger::Log() << Team::TEAM_3 << LogLevel::ERROR << Color::RED << " RED Error message from Team C." << std::endl;
    Logger::Log() << Team::TEAM_4 << LogLevel::ERROR << Color::BLUE << "Error message from Team C. no endl";
    Logger::Log() << "cont no endl Error message from Team";

    Logger::Log() << std::endl <<  "Warning message standard overload" << std::endl;

    samplefunction();


//    // Example 1: Logging a debug message for Team 1 in blue color
//    Logger::Log() << Team::TEAM_1 << LogLevel::DEBUG << Color::BLUE << "Debug message for Team 1" << Logger::endl;
//
//    // Example 2: Logging an error message for Team 5 in red color
//    Logger::Log() << Team::TEAM_5 << LogLevel::ERROR << Color::RED << "Error message for Team 5" << Logger::endl;
//
//    // Example 3: Logging an info message for General team in green color
//    Logger::Log() << Team::GENERAL << LogLevel::INFO << Color::GREEN << "Info message for General team" << Logger::endl;
//
//    // Example 4: Logging a warning message without specifying team and color
//    Logger::Log() << LogLevel::WARNING << "Generic warning message level Warning" << Logger::endl;
//
//    // Example 4: Logging a warning message without specifying team and color
//    Logger::Log() << LogLevel::NA  << "Generic warning message" << Logger::endl;




    return 0;
}
