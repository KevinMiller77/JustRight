#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include "kenum.h"

#include <unordered_map>
#include <string>

namespace JR::Options {
    K_ENUM(  
        OptionFlag, 
        HELP,
        VERSION,
        OUTPUT_FILE,
        TOKENIZER_CSV_OUTPUT_FILE,
        TOKENIZER_OUTPUT_TO_CONSOLE,
        INPUT_FILE,
    )

    /**
     * @brief Returns the help message for the given flag
     * 
     * @param flag         - The flag to describe
     * @return std::string - The help message
     */
    std::string describeOptionFlag(OptionFlag::Enum flag);

    /**
     * @brief Prints the help message with usage information
     * 
     * @param argv - The arguments
     * @param help - The help message
     */
    void printHelp(const char* filename);

    // Map of options describing the input from the user
    typedef struct {
        std::string filename;
        std::unordered_map<OptionFlag::Enum, std::string> flags;
    } Options;

    /**
     * @brief Parse the command line arguments into a map of options and store
     * 
     * @param argc - The number of arguments
     * @param argv - The arguments
     * @return Options - The map of options
     */
    bool registerOptions(int argc, char *argv[]);

    /**
     * @brief Check if the given option flag is present in the options map
     * 
     * @param options - The options map
     * @param flag    - The flag to check
     * @return true   - If the flag is present
     * @return false  - If the flag is not present
     */
    bool hasFlag(OptionFlag::Enum flag);

    /**
     * @brief Expect the given flag to be present in the options map
     * 
     * @param flag 
     * @return std::string - The value of the flag
     */
    std::string expectFlag(OptionFlag::Enum flag);

    /**
     * @brief Get the value of the given option flag from the options map
     * 
     * @param options - The options map
     * @param flag    - The flag to get the value for
     * @return std::string - The value of the flag
     */
    std::string getFlag(OptionFlag::Enum flag);
}
#endif // __OPTIONS_H__