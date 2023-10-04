#include <iostream>
#include <sstream>
#include <map>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

enum class Team {
    TEAM_1,
    TEAM_2,
    TEAM_3,
    TEAM_4,
    TEAM_5,
    TEAM_6,
    TEAM_7,
    TEAM_8,
    TEAM_9,
    GENERAL,
    NA
};

enum class Color {
    RESET = 0,
    BLUE = 34,
    GREEN = 32,
    RED = 31
};

const LogLevel LOGLEVEL = LogLevel::DEBUG;

class Logger {
public:
    Logger& operator<<(Team team) {
        currentTeam = team;
        return *this;
    }

    Logger& operator<<(LogLevel logLevel) {
        currentLogLevel = logLevel;
        return *this;
    }

    Logger& operator<<(Color color) {
        currentColor = color;
        return *this;
    }

    Logger& operator<<(std::ostream& (*manipulator)(std::ostream&)) {
        if (manipulator == endl) {

            currentTeam = Team::NA;
            currentLogLevel = LogLevel::DEBUG;
            currentColor = Color::RESET;
        }
        return *this;
    }

    template <typename T>
    Logger& operator<<(const T& value) {
        #ifndef NDEBUG
        if (currentLogLevel >= LOGLEVEL)
        {
            std::ostringstream logMessage;
            logMessage << "\033[" << static_cast<int>(currentColor) << "m[" << teamToString(currentTeam) << "]" << logToString(currentLogLevel) << " " << value << "\033[0m";
            std::cout << logMessage.str() << std::endl;
        }
        #endif
        return *this;
    }


    static Logger log; // Global log instance
    static std::ostream& endl(std::ostream& os) {

        log << std::endl; // Call the custom Logger::endl to reset values
        return os;
    }

private:
    Team currentTeam = Team::NA;
    LogLevel currentLogLevel = LogLevel::DEBUG;
    Color currentColor = Color::RESET;


    std::map<Team, std::string> teamToStringMap = {
            {Team::TEAM_1, "Team 1"},
            {Team::TEAM_2, "Team 2"},
            {Team::TEAM_3, "Team 3"},
            {Team::TEAM_4, "Team 4"},
            {Team::TEAM_5, "Team 5"},
            {Team::TEAM_6, "Team 6"},
            {Team::TEAM_7, "Team 7"},
            {Team::TEAM_8, "Team 8"},
            {Team::TEAM_9, "Team 9"},
            {Team::NA, "NA"},
            {Team::GENERAL, "General"}
    };

    std::string teamToString(Team team) {

        auto it = teamToStringMap.find(team);
        if (it != teamToStringMap.end()) {
            return it->second;
        }

        return "Unknown Team";

    }

    std::string logToString(LogLevel logLevel) {
        if (logLevel == LogLevel::DEBUG) {
            return "(DEBUG)";
        } else if (logLevel == LogLevel::INFO) {
            return "(INFO)";
        } else if (logLevel == LogLevel::WARNING) {
            return "(WARNING)";
        } else if (logLevel == LogLevel::ERROR) {
            return "(ERROR)";
        } else {
            return "";
        }
    }
};

Logger Logger::log;