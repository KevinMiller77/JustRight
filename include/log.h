#ifndef __LOG_H__
#define __LOG_H__

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
#define LOG_FORMAT(level, fmt, ...) \
    do { \
        if (level <= DEBUG_LEVEL) { \
            if (level == DEBUG_LEVEL_ERROR) { \
                std::cerr << "\033[1;31m"; \
            } else if (level == DEBUG_LEVEL_WARN) { \
                std::cerr << "\033[1;33m"; \
            } else if (level == DEBUG_LEVEL_INFO) { \
                std::cerr << "\033[1;32m"; \
            } else if (level == DEBUG_LEVEL_DEBUG) { \
                std::cerr << "\033[1;34m"; \
            } else if (level == DEBUG_LEVEL_TRACE) { \
                std::cerr << "\033[1;37m"; \
            } \
            std::ostringstream os; \
            os << __FILE__ << ":" << __LINE__; \
            os << " " << fmt << format_log(__VA_ARGS__) << std::endl; \
            std::cerr << os.str(); \
            std::cerr << "\033[0m"; \
        } \
    } while(0)

// Fallback when no arguments are provided
#define LOG(level, fmt, ...) LOG_FORMAT(level, fmt, ##__VA_ARGS__)

// Function to format variable arguments
template<typename... Args>
std::string format_log(Args&&... args) {
    std::ostringstream oss;
    (oss << ... << std::forward<Args>(args));
    return oss.str();
}

// Define specific macros for each debug level with optional tag
#define LOG_ERROR(fmt, ...) LOG(DEBUG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) LOG(DEBUG_LEVEL_WARN, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) LOG(DEBUG_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) LOG(DEBUG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define LOG_TRACE(fmt, ...) LOG(DEBUG_LEVEL_TRACE, fmt, ##__VA_ARGS__)

#endif // __LOG_H__