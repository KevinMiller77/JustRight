#ifndef __K_ENUM_H__
#define __K_ENUM_H__

#include <unordered_map>
#include <vector>
#include <string>

namespace K::Enum {
    inline std::vector<std::string> SplitEnumString(std::string s) { \
        std::vector<std::string> result; 
        size_t start = 0, end;
        while ((end = s.find(',', start)) != std::string::npos) {
            result.push_back(s.substr(start, end - start)); 
            start = end + 1;
        }
        result.push_back(s.substr(start));
        return result; 
    }
}

#define K_ENUM(EnumName, ...) \
    namespace EnumName { \
        enum Enum { __VA_ARGS__ }; \
        inline std::vector<std::string> Strings = K::Enum::SplitEnumString(#__VA_ARGS__); \
        inline std::vector<Enum> Values = { __VA_ARGS__ }; \
    } \

#endif // __K_ENUM_H__