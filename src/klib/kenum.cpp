#include <klib/kenum.h>

namespace K::Enum {
    std::vector<std::string> SplitEnumString(std::string s) {
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