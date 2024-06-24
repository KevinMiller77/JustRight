#ifndef __K_LOG_H__
#define __K_LOG_H__

#include <iostream>
#include <sstream>
#include <string>
#include <utility>

// Define debug levels
#define DEBUG_LEVEL_NONE    0
#define DEBUG_LEVEL_ERROR   1
#define DEBUG_LEVEL_WARN    2
#define DEBUG_LEVEL_INFO    3
#define DEBUG_LEVEL_DEBUG   4
#define DEBUG_LEVEL_TRACE   5

// Set the current debug level
#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL DEBUG_LEVEL_DEBUG
#endif

// Helper macro to support variable arguments
#define LOG_FORMAT(level, ...) \
    do { \
        if (level <= DEBUG_LEVEL) { \
            std::ostringstream os; \
            if (level == DEBUG_LEVEL_ERROR) { \
                os << "\033[1;31m"; \
                os << " [ERROR]"; \
            } else if (level == DEBUG_LEVEL_WARN) { \
                os << "\033[1;33m"; \
                os << " [WARN]"; \
            } else if (level == DEBUG_LEVEL_INFO) { \
                os << "\033[1;32m"; \
                os << " [INFO]"; \
            } else if (level == DEBUG_LEVEL_DEBUG) { \
                os << "\033[1;34m"; \
                os << " [DEBUG]"; \
                os << __FILE__ << ":" << __LINE__; \
            } else if (level == DEBUG_LEVEL_TRACE) { \
                os << "\033[1;37m"; \
                os << __FILE__ << ":" << __LINE__; \
                os << " [TRACE]"; \
            } \
            os << " " << format_log(__VA_ARGS__) << std::endl; \
            os << "\033[0m"; \
            std::cout << os.str(); \
        } \
    } while(0)

// Fallback when no arguments are provided
#define LOG(level, ...) LOG_FORMAT(level, ##__VA_ARGS__)

// Function to format variable arguments
template<typename... Args>
std::string format_log(Args&&... args) {
    std::ostringstream oss;
    (oss << ... << std::forward<Args>(args));
    return oss.str();
}

// Define specific macros for each debug level with optional tag
#define LOG_ERROR(...) LOG(DEBUG_LEVEL_ERROR, ##__VA_ARGS__)
#define LOG_WARN(...) LOG(DEBUG_LEVEL_WARN, ##__VA_ARGS__)
#define LOG_INFO(...) LOG(DEBUG_LEVEL_INFO, ##__VA_ARGS__)
#define LOG_DEBUG(...) LOG(DEBUG_LEVEL_DEBUG, ##__VA_ARGS__)
#define LOG_TRACE(...) LOG(DEBUG_LEVEL_TRACE, ##__VA_ARGS__)

#endif // __K_LOG_H__