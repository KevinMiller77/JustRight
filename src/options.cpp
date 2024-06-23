#include <options.h>

#include <iostream>

JR::Options::Options s_Options = {};

namespace JR::Options {
    std::string describeOptionFlag(OptionFlag::Enum flag) {
        switch (flag) {
            case OptionFlag::HELP:
                return "-h --help: Display this help message";
            case OptionFlag::VERSION:
                return "-v --version: Display the version of the program";
            case OptionFlag::INPUT_FILE:
                return "<file.jr>: The input file to read from, should be the last argument";
            case OptionFlag::OUTPUT_FILE:
                return "-o --output <file.jr>: The output file to write to";
            case OptionFlag::TOKENIZER_CSV_OUTPUT_FILE:
                return "--tokenizer-csv-output <file.csv>: The output file to write the tokenizer CSV to";
            case OptionFlag::TOKENIZER_OUTPUT_TO_CONSOLE:
                return "--tdebug: Output the tokenizer debug to the console";
            default:
                return "";
        }
    }

    void printHelp(const char* filename) {  
        // Green
        std::cout << "\033[1;32mJR Compiler\033[0m" << std::endl;
        std::cout << "Usage: " << filename << " <options> <file.jr>" << std::endl;
        std::cout << "Options:" << std::endl;
        for (OptionFlag::Enum e: OptionFlag::Values) {
            std::string desc = describeOptionFlag(e);
            
            if (desc.empty()) 
                continue;

            std::cout << "  " << describeOptionFlag(e) << std::endl;
        }
    }

    bool registerOptions(int argc, char *argv[]) {
        s_Options.filename = argv[0];
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            if (arg == "-h" || arg == "--help") {
                s_Options.flags[OptionFlag::HELP] = "";
            } else if (arg == "-v" || arg == "--version") {
                s_Options.flags[OptionFlag::VERSION] = "";
            } else if (arg == "-o" || arg == "--output") {
                if (i + 1 >= argc) {
                    return false;
                } 

                s_Options.flags[OptionFlag::OUTPUT_FILE] = argv[i + 1];
                i++;
            } else if (arg == "--tokenizer-csv-output") {
                if (i + 1 >= argc) {
                    return false;
                }

                s_Options.flags[OptionFlag::TOKENIZER_CSV_OUTPUT_FILE] = argv[i + 1];
                i++;
            } else if (arg == "--tdebug") {
                s_Options.flags[OptionFlag::TOKENIZER_OUTPUT_TO_CONSOLE] = "";
            } else {
                s_Options.flags[OptionFlag::INPUT_FILE] = arg;
            }
        }

        return true;
    }

    bool hasFlag(OptionFlag::Enum flag) {
        return s_Options.flags.find(flag) != s_Options.flags.end();
    }

    std::string expectFlag(OptionFlag::Enum flag) {
        if (!hasFlag(flag)) {
            // Make the following text red
            std::cerr << "\033[1;31mError:\033[0m ";
            std::cerr << "Expected argument " << describeOptionFlag(flag) << std::endl << std::endl;
            printHelp(s_Options.filename.c_str());
            exit(1);
        }

        return s_Options.flags[flag];
    }

    std::string getFlag(OptionFlag::Enum flag) {
        return s_Options.flags[flag];
    }
}