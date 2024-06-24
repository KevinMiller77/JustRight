#include <iostream>

#include <klib/kflags.h>
#include <klib/klog.h>
#include <ostream>

#define ITERABLE_FIND(x, y) (std::find((x).begin(), (x).end(), (y)))
#define ITERABLE_CONTAINS(x, y) (ITERABLE_FIND((x), (y)) != (x).end())

#define MAP_FIND(x, y) ((x).find((y)))
#define MAP_CONTAINS(x, y) (MAP_FIND((x), (y)) != (x).end())

#define RESERVED_FLAG_HELP -1
#define RESERVED_FLAG_CNT 1

bool s_Initialized = false;
std::string s_FileName = "";
std::string s_Usage = "";

K::Flags::FlagDefinitionList s_FlagDefinitions = {
    { RESERVED_FLAG_HELP, { "-h", "--help" }, false, "Display this help message" }
};
K::Flags::FlagValueMap s_FlagValues = {};
std::vector<std::string> s_UnqualifiedFlags = {};

namespace K::Flags {

    bool init(
        int argc, 
        char *argv[],
        std::string usageMessage,
        FlagDefinitionList flags
    ) {
        if (argc == 0) {
            std::cout << "Filename is missing from argv[0]? How does this happen? Are you a wizard?" << std::endl;
            return false;
        }

        s_FileName = std::string(argv[0]);
        s_Usage = usageMessage;
        s_FlagDefinitions.insert(s_FlagDefinitions.end(), flags.begin(), flags.end());
        s_Initialized = true;

        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];

            bool flagFound = false;
            for (FlagDefinition flag: s_FlagDefinitions) {
                if (!ITERABLE_CONTAINS(flag.identifiers, arg)) {
                    continue;
                }

                if (MAP_CONTAINS(s_FlagValues, flag.identity)){
                    std::cerr << "Invalid argument, duplicate flag provided: " << arg << std::endl;
                    printHelp();
                    return false;
                }

                if (!flag.hasArgument) {
                    s_FlagValues[flag.identity] = "";
                    flagFound = true;
                    break;
                }

                if (++i >= argc) {
                    std::cerr << "Missing required value for flag " << arg << std::endl;
                    printHelp();
                    return false;
                }

                s_FlagValues[flag.identity] = argv[i];
                flagFound = true;
                break;
            }

            if (!flagFound) {
                s_UnqualifiedFlags.push_back(arg);
            }
        }

        // If map contains help
        if (MAP_CONTAINS(s_FlagValues, RESERVED_FLAG_HELP)) {
            printHelp();
            return false;
        }

        return true;
    }

    void printHelp() {
        if (!s_Initialized) {
            std::cerr << "Flags library not initialized, cannot print help message." << std::endl;
        }

        std::cout << "Usage: " << s_FileName << " " << s_Usage << std::endl;
        std::cout << "Flags:" << std::endl;
        for (FlagDefinition flag : s_FlagDefinitions) {
            if (flag.identifiers.empty()) {
                continue;
            }

            std::cout << "  ";
            for (std::string identifier : flag.identifiers) {
                std::cout << identifier << " ";
            }
            std::cout << "  " << flag.helpMessage << std::endl;
        }
    }

    FlagData getFlag(size_t flag) {
        if (!s_Initialized) {
            std::cerr << "Flags library not initialized, cannot get flag value." << std::endl;
            abort();
        }

        if (!MAP_CONTAINS(s_FlagValues, flag)) {
            return { false, "" };
        }

        return { true, s_FlagValues[flag] };
    }

    std::string expectFlag(size_t identity) {
        if (!s_Initialized) {
            std::cerr << "Flags library not initialized, cannot get flag value." << std::endl;
            abort();
        }

        if (!MAP_CONTAINS(s_FlagValues, identity)) {
            std::cerr << "" << std::endl;
            printHelp();
            abort();
        }

        return s_FlagValues[identity];
    }

    std::vector<std::string> getUnqualifiedFlags() {
        return s_UnqualifiedFlags;
    }
}