#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <set>
#include <utility>


namespace clogged {

/**
 * @brief Log levels for logging
 *
 */
    enum class LogLevel {
        NA = 0, DEBUG = 1, INFO = 2, WARNING = 3, ERROR = 4
    };
//enum class LogLevel { DEBUG, INFO, WARNING, ERROR, NA };

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
        RESET = 0, BLUE = 34, GREEN = 32, RED = 31
    };

/// @brief Ensure that we only log when NDEBUG flg is not set
#ifndef NDEBUG

#define LOG_LINE "File: " << __FILE__ << "::->::Line(" << __LINE__

#define RELATIVE_PATH(file)                                                  \
  (std::string(file).find_last_of("/\\") != std::string::npos                \
       ? std::string(file).substr(std::string(file).find_last_of("/\\") + 1) \
       : std::string(file))

#define LOG_RELLINE \
  "File: " << RELATIVE_PATH(__FILE__) << "::->::Line(" << __LINE__ << ")"

#define LOG_FNC "Function: " << __func__ << " "

/// Not a fan of this practice
/// But would prefer not to use parenthesis


/**
 * @brief Logger class with colors and team names
 * @author @amantham20 @chatGPT
 *
 * @details I was about to name this clogged.
 * @details This is a custom logger class that can be used to log messages to
 * the console.
 */
    class Logger {
    public:
        /**
         * @brief Sets the Team name for the current log
         *
         * @param team name of the team
         * @return Logger&
         */
        Logger &operator<<(const Team &team) {
          currentTeam = team; // NOLINT(unused)
          metaPrinted = false; // NOLINT(unused)

          return *this;
        }

        /**
         * @brief sets the log level for the current log
         *
         * @param logLevel Level/Type of the log
         * @return Logger&
         */
        Logger &operator<<(LogLevel logLevel) {
          currentLogLevel = logLevel;
          metaPrinted = false; // NOLINT(unused)

          return *this;
        }

        /**
         * @brief colors of the log
         *
         * @param color
         * @return Logger&
         */
        Logger &operator<<(Color color) {
          currentColor = color;
          return *this;
        }


//#pragma clang diagnostic push
//#pragma ide diagnostic ignored "UnreachableCode" /// TODO: Check if this is valid
        /**
         * @brief Manipulator for endl so that we can reset the values when a team is
         * done logging
         *
         * @param manipulator
         * @return Logger&
         */
        Logger &operator<<(std::ostream &(*manipulator)(std::ostream &)) {
          typedef std::ostream &(*EndlManipulator)(std::ostream &);
          if (static_cast<int>(minLogLevel) <= static_cast<int>(currentLogLevel)) {

            // Compare the function pointers
            if (manipulator == static_cast<EndlManipulator>(std::endl) ||
                manipulator == endl) {
              // Handle std::endl here
              currentTeam = Team::NA;
              currentLogLevel = LogLevel::DEBUG;
              currentColor = Color::RESET;

              std::cout << std::endl;

              metaPrinted = false;
            }

          }

          return *this;
        }
//#pragma clang diagnostic pop

        /**
         * @brief Aye I used a template.
         * @brief Function to log the value
         *
         * @tparam T
         * @param value
         * @return Logger&
         *
         * @TODO: Might have to change this so that we only break a team log when a
         * new team is set. aka ensure that  logger << Team::TEAM_1 << "Hello" <<
         * "World" << endl; works in one line with one team print
         */
        template<typename T>
        Logger &operator<<(const T &value) {
          // TODO: Define when to log by loglevel comparison. Goal is to send it in as
          if (static_cast<int>(minLogLevel) <= static_cast<int>(currentLogLevel)) {

            // a flag in the CMakeLists.txt
            // added additional flag in case one wants to compile without colors (or)
            // if the terminal does not support colors
#ifndef D_ANSI_COLOR_CODES
            std::string colorStart =
                    "\033[" + std::to_string(static_cast<int>(currentColor)) + "m";
            std::string colorEnd = "\033[0m";
#else
            std::string colorStart = "";
            std::string colorEnd = "";
#endif
            std::ostringstream logMessage;
            logMessage << colorStart;
            if (!metaPrinted) {
              logMessage << teamToString(currentTeam) << logToString(currentLogLevel);
              metaPrinted = true;
            }

            logMessage << value << colorEnd;
            std::cout << logMessage.str();  // << std::endl;  //TODO: Might have to
            // make enable this so that we can have
            // same line logging when endl is not used
          }

          return *this;
        }


        /**
         * Only instance of the logger once
         * Changes requested from Dr.@ofria
         *
         * @authors @mercere99
         * @return
         */
        static Logger &Log() {
          static Logger instance; // Guaranteed to be initialized only once.

          return instance;
        }

        /**
         * Only instance of the logger once
         * Changes requested from Dr.@ofria
         *
         * @authors @mercere99
         * @return
         */
        template<typename T, typename... EXTRA_Ts>
        static Logger &Log(T &&arg1, EXTRA_Ts &&... extra_args) {
          Log() << std::forward<T>(arg1);  // Log the first argument.
          if constexpr (sizeof...(EXTRA_Ts) == 0) {  // No arguments left.
            return Log() << Logger::endl;  // Trigger a flush.
          } else {
            return Log(std::forward<EXTRA_Ts>(extra_args)...);  // Log remaining arguments.
          }
        }

        /**
         * @brief Returns the current time stamp
         *
         * @return std::string
         */
        [[maybe_unused]] static std::string time() {
          auto now = std::chrono::system_clock::now();
          std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
          std::string timestamp = std::ctime(&currentTime);
          timestamp.pop_back(); // Remove the trailing newline character

          return "[" + timestamp + "] ";
        }


        /**
         * @brief Custom endl to reset the values
         *
         * @param os
         * @return std::ostream&
         */
        static std::ostream &endl(std::ostream &os) {
          Log() << std::endl;  // Call the custom Logger::endl to reset values
          return os;
        }

        /**
         * @brief Sets the minimum log level for the logger
         * @param level
         */
        [[maybe_unused]] static void setMinimumLogLevel(LogLevel level) {
          minLogLevel = level;
        }

        /**
         * @brief Checks if the log level is greater than the minimum log level
         * @param level
         * @return bool
         */
        [[maybe_unused]] static bool checkLogLevel(LogLevel level) {
          return static_cast<int>(minLogLevel) <= static_cast<int>(level);
        }

        /**
         * @brief Sets the global teams for the logger
         * @param teams
         */
        [[maybe_unused]] static void setPrintTeams(std::set<Team> teams) {
          currentPrintTeams = std::move(teams); //TODO: Check if this works
        }

        /**
         * @brief Sets the global teams for the logger
         * @param teams
         */
        [[maybe_unused]] static void setPrintTeams(const std::vector<Team> &teams) {
          currentPrintTeams = std::set<Team>(teams.begin(), teams.end());
        }

        /**
         * @brief Sets the global teams for the logger
         * @param team
         */
        [[maybe_unused]] static void setPrintTeams(Team team) {
          currentPrintTeams.insert(team);
        }

        /**
         * @brief Sets the global teams for the logger
         * @param teams
         */
        [[maybe_unused]] static void setPrintTeams(std::initializer_list<Team> teams) {
          currentPrintTeams = std::set<Team>(teams.begin(), teams.end());
        }

        /**
         * @brief Checks if the team is in the global teams
         * @param team
         * @return bool
         */
        [[maybe_unused]] static bool checkTeam(Team team) {
          return currentPrintTeams.find(team) != currentPrintTeams.end();
        }



    private:
        /// @brief Current team for that is going to log
        Team currentTeam = Team::NA;

        /// @brief Current log level for the log
        LogLevel currentLogLevel = LogLevel::DEBUG;

        /// @brief Current color for the log
        Color currentColor = Color::RESET;

        static LogLevel minLogLevel;

        /// @brief flag that checks and ensures if the meta data is printed once
        bool metaPrinted = false;

        static std::set<Team> currentPrintTeams;

        /**
         * @brief Map to convert Team enum to string
         *
         */
        const std::map<Team, std::string> teamToStringMap = {
                {Team::TEAM_1,  "Team 1"},
                {Team::TEAM_2,  "Team 2"},
                {Team::TEAM_3,  "Team 3"},
                {Team::TEAM_4,  "Team 4"},
                {Team::TEAM_5,  "Team 5"},
                {Team::TEAM_6,  "Team 6"},
                {Team::TEAM_7,  "Team 7"},
                {Team::TEAM_8,  "Team 8"},
                {Team::TEAM_9,  "Team 9"},
                {Team::GENERAL, "General"}};

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
        static std::string logToString(LogLevel logLevel) {
          if (logLevel == LogLevel::DEBUG) {
            return "(DEBUG) ";
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


        [[maybe_unused]] static std::string getCurrentTimestamp() {
          auto now = std::chrono::system_clock::now();
          std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
          std::string timestamp = std::ctime(&currentTime);
          timestamp.pop_back(); // Remove the trailing newline character
          return timestamp;
        }
    };

    LogLevel Logger::minLogLevel = LogLevel::NA;
    std::set<Team> Logger::currentPrintTeams = {Team::TEAM_1, Team::TEAM_2, Team::TEAM_3, Team::TEAM_4, Team::TEAM_5,
                                                Team::TEAM_6, Team::TEAM_7, Team::TEAM_8, Team::TEAM_9, Team::GENERAL,
                                                Team::NA};


#else

#define LOGLINE ""
#define LOG_RELLINE ""
#define LOG_FNC ""

#define log Log()

    class Logger {
     public:
      template <typename T>
      Logger &operator<<(const T &value) {
        return *this;
      }

      Logger &operator<<(std::ostream &(*manipulator)(std::ostream &)) {
        return *this;
      }

      static std::ostream &endl(std::ostream &os) { return os; }

      static Logger& Log() {
        static Logger instance; // Guaranteed to be initialized only once.
        return instance;
      }

    };

    //Logger Logger::log;
#endif

}  // namespace clogged
