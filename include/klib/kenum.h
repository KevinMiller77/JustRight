#ifndef __K_ENUM_H__
#define __K_ENUM_H__

#include <vector>
#include <string>

#include "ktypes.h"

namespace K::Enum {
    std::vector<std::string> SplitEnumString(std::string s);
}

/**
 * @brief A macro to define a custom enum wrapped in a namespace of EnumName, 
 *      containing the enum itself as `Enum` and helper members,
 *       `Strings` -> a vector of strings of the enum values, and
 *       `Values`  -> a vector of the enum values
 */
#define K_ENUM(EnumName, ...) \
    namespace EnumName { \
        enum Enum: u32 { NONE = 0, __VA_ARGS__ }; \
        inline std::vector<std::string> Strings = K::Enum::SplitEnumString(#__VA_ARGS__); \
        inline std::vector<Enum> Values = { __VA_ARGS__ }; \
    } \

#endif // __K_ENUM_H__