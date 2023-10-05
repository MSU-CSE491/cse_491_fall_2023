#include <iostream>
#include <sstream>
#include <map>


/**
 * @brief Log levels for logging
 * 
 */
enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    NA
};

/**
 * @brief Teams Names for logging
 * 
 */
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

/**
 * @brief Colors for logging
 * 
 */
enum class Color {
    RESET = 0,
    BLUE = 34,
    GREEN = 32,
    RED = 31
};

/**
 * @brief Level of logging
 * @TODO: Change this to be a flag in the CMakeLists.txt
 */
const LogLevel LOGLEVEL = LogLevel::DEBUG;


/**
 * @brief Logger class with colors and team names
 * @author @amantham20 @chatGPT
 * 
 * @details I was about to name this clogged.
 * @details This is a custom logger class that can be used to log messages to the console.
 */
class Logger {
public:

    /**
     * @brief Sets the Team name for the current log
     * 
     * @param team name of the team
     * @return Logger& 
     */
    Logger& operator<<(Team team) {
        currentTeam = team;
        // std::cout << endl; //TODO: Might have to enable this so that we can have same line logging when endl is not used
        return *this;
    }

    /**
     * @brief sets the log level for the current log
     * 
     * @param logLevel Level/Type of the log
     * @return Logger& 
     */
    Logger& operator<<(LogLevel logLevel) {
        currentLogLevel = logLevel;
        return *this;
    }

    /**
     * @brief colors of the log
     * 
     * @param color 
     * @return Logger& 
     */
    Logger& operator<<(Color color) {
        currentColor = color;
        return *this;
    }

    /**
     * @brief Manipulator for endl so that we can reset the values when a team is done logging
     * 
     * @param manipulator 
     * @return Logger& 
     */
    Logger& operator<<(std::ostream& (*manipulator)(std::ostream&)) {
        if (manipulator == endl) {

            currentTeam = Team::NA;
            currentLogLevel = LogLevel::DEBUG;
            currentColor = Color::RESET;
            // std::cout << std::endl; //TODO: Might have to enable this so that we can have same line logging when endl is not used
        }
        return *this;
    }

    /**
     * @brief Aye I used a template.
     * @brief Function to log the value
     * 
     * @tparam T 
     * @param value 
     * @return Logger& 
     * 
     * @TODO: Might have to change this so that we only break a team log when a new team is set. aka ensure that  logger << Team::TEAM_1 << "Hello" << "World" << endl; works in one line with one team print
     */
    template <typename T>
    Logger& operator<<(const T& value) {

        /// @brief Ensure that we only log when NDEBUG flg is not set
        #ifndef NDEBUG
        if (currentLogLevel >= LOGLEVEL)
        {
            std::ostringstream logMessage;
            logMessage << "\033[" << static_cast<int>(currentColor) << "m" << teamToString(currentTeam) << logToString(currentLogLevel)  << value << "\033[0m";
            std::cout << logMessage.str();
                            //            << std::endl;  //TODO: Might have to make enable this so that we can have same line logging when endl is not used
        }
        #endif
        return *this;
    }



    static Logger log; /// Global log instance //TODO: Check if poluting the global namespace is a good idea??

    /**
     * @brief Custom endl to reset the values
     * 
     * @param os 
     * @return std::ostream& 
     */
    static std::ostream& endl(std::ostream& os) {

        log << std::endl; // Call the custom Logger::endl to reset values
        return os;
    }

private:
    /// @brief Current team for that is going to log
    Team currentTeam = Team::NA;

    /// @brief Current log level for the log
    LogLevel currentLogLevel = LogLevel::DEBUG;

    /// @brief Current color for the log
    Color currentColor = Color::RESET;

    /**
     * @brief Map to convert Team enum to string
     * 
     */
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
            {Team::GENERAL, "General"}
    };

    /**
     * @brief Converts Team enum to string
     * 
     * @param team 
     * @return std::string 
     */
    std::string teamToString(Team team) {

        auto it = teamToStringMap.find(team);
        if (it != teamToStringMap.end()) {
            return "[" + it->second + "]";
        }

        return "";

    }

    /**
     * @brief Converts LogLevel enum to string
     * 
     * @param logLevel 
     * @return std::string 
     */
    std::string logToString(LogLevel logLevel) {
        if (logLevel == LogLevel::DEBUG) {
            return "(DEBUG) " ;
        } else if (logLevel == LogLevel::INFO) {
            return "(INFO) ";
        } else if (logLevel == LogLevel::WARNING) {
            return "(WARNING) ";
        } else if (logLevel == LogLevel::ERROR) {
            return "(ERROR) ";
        } else {
            return "";
        }
    }
};

/// @brief Global log instance
Logger Logger::log;