#ifndef __FLAGS_H__
#define __FLAGS_H__

#include "ktypes.h"

#include <vector>
#include <string>
#include <unordered_map>

/**
    The KLib Flags library. This library provides a way to register input flags from the user.
    The flags are registered with a list of identifiers, the number of arguments the flag takes,
    and a help message to display when the user requests help.

    NOTE: The -h and --help flags are built-in and reserved for the help message, and the library will automatically
            display the help message when the user requests it.
 */
namespace K::Flags {
    struct FlagDefinition {
        i64 identity;                           // An identity set by the caller, usally an kenum value
        std::vector<std::string> identifiers;   // The list of accepted identifiers for the flag
        bool hasArgument;                       // Determines if the flag takes an argument
        std::string helpMessage;                // The help message to display when the user requests help
    };

    typedef std::vector<FlagDefinition> FlagDefinitionList;
    typedef std::unordered_map<i64, std::string> FlagValueMap;

    struct FlagData {
        bool present;
        std::string value;
    };

    /**
     * @brief Initialize the flags library with the usage message and the list of flags
     * 
     * @param   argc - The number of arguments
     * @param   argv - The arguments
     * @param   usageMessage - The usage message to display when the user requests help
     * @param   flags - The list of flags to register
     * @return  true - Processing was successful, move on
     *          false - Processing was unsuccessful or user requested help be printed. Exit the process.
     */
    bool init(
        int argc, 
        char *argv[],
        std::string usageMessage,
        FlagDefinitionList flags
    );

    /**
     * @brief Prints the help message with usage information
     * 
     * @param argv - The arguments
     * @param help - The help message
     */
    void printHelp();

    /**
     * @brief Get the value of a flag if present
     * 
     * @param options - The options map
     * @param flag    - The flag to get the value for
     * @return  [0] => bool - True if the flag was present
                [1] => std::string - The value of the flag if present
     */
    FlagData getFlag(size_t flag);

    /**
     * @brief Expect the given flag to be present in the options map, 
     *          if the flag is not present, help is printed and the program exits
     * 
     * @param identity - The identity of the flag
     * @return std::string - The value of the flag
     */
    std::string expectFlag(size_t identity);

    /**
     * @brief Get the Unqualified Flags object
     * 
     * @return std::vector<std::string> 
     */
    std::vector<std::string> getUnqualifiedFlags();
}
#endif // __FLAGS_H__