/**
 * @file        logger.h
 * @author      Sergio Guerrero Blanco <sergioguerreroblanco@hotmail.com>
 * @date        <2025-09-15>
 * @version     1.0.0
 *
 * @brief       Thread-safe logging utility with levels.
 *
 * @details
 * The Logger class provides a global, thread-safe mechanism to print
 * messages to the standard output stream. Messages are categorized by
 * severity levels (INFO, WARN, ERROR), and a configurable minimum level
 * filter determines which messages are displayed.
 *
 * Internally, the Logger uses a static mutex to synchronize concurrent
 * writes from multiple threads, ensuring that log messages are not
 * interleaved. It also attaches a timestamp and the severity label to
 * each printed message, providing clear context for debugging and
 * monitoring concurrent applications.
 */

/*****************************************************************************/

/* Include Guard */

#pragma once

/*****************************************************************************/

/* Libraries */

#include <mutex>
#include <string>

/*****************************************************************************/

/* Class Logger */

class Logger 
{
    /******************************************************************/

    /* Public Data Types */

    public:
        enum class Level 
        { 
            DBG = 0,
            INFO = 1, 
            WARN = 2, 
            ERROR = 3 
        };

    /******************************************************************/

    /* Public Methods */

    public:

        static void set_min_level(Level lvl);

        static void debug(const std::string& msg);
        static void info(const std::string& msg);
        static void warn(const std::string& msg);
        static void error(const std::string& msg);

        static void log(Level lvl, const std::string& msg);


    /******************************************************************/

    /* Private Methods */

    private:

        static std::string timestamp();           // "YYYY-MM-DD HH:MM:SS"
        static const char* levelToString(Level);  // "INFO"/"WARN"/"ERROR"

    /******************************************************************/

    /* Private Attributes */

    private:

        static std::mutex mtx;
        static Level minLevel;

    /******************************************************************/
};