
#include "./core/EasyLogging.hpp"


int main() {
    Logger::log << Team::TEAM_1 << LogLevel::DEBUG << Color::BLUE << "This is a debug message from Team A." << "aye 2" << Logger::endl;
    Logger::log << Team::TEAM_2 << LogLevel::INFO << Color::GREEN << "This is an info message from Team B." << Logger::endl;
    Logger::log << Team::TEAM_3 << LogLevel::ERROR << Color::RED << " RED Error message from Team C." << Logger::endl;
    Logger::log << Team::TEAM_4 << LogLevel::ERROR << Color::BLUE << "Error message from Team C. no endl";
    Logger::log << "cont no endl Error message from Team";
    Logger::log << Team::TEAM_6 << LogLevel::ERROR << Color::BLUE << "Error message from Team C. no endl";
    Logger::log << "This is wi." << Logger::endl;
    Logger::log << "This is wi." << Logger::endl;
    Logger::log << Team::TEAM_5 << LogLevel::ERROR << Color::RED << "Error message from Team C." << Logger::endl;
    Logger::log << Team::TEAM_5 << LogLevel::ERROR << "Error message from Team C." << Logger::endl;

    return 0;
}
