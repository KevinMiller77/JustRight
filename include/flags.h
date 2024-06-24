#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include "klib/kenum.h"
#include "klib/kflags.h"

namespace JR {
    K_ENUM( 
        Flags,
        VERSION,
        OUTPUT_FILE,
        TOKENIZER_CSV_OUTPUT_FILE,
        TOKENIZER_OUTPUT_TO_CONSOLE
    )

    inline K::Flags::FlagDefinitionList s_FlagDefinitions = {
        { 
            Flags::VERSION,
            { "-v", "--version" }, 
            false, 
            "Display the version of the program"
        },
        { 
            Flags::OUTPUT_FILE,
            { "-o", "--output" },
            true,
            "The output file to write to"
        },
        { 
            Flags::TOKENIZER_CSV_OUTPUT_FILE,
            { "--tokenizer-csv-output" },
            true,
            "The output file to write the tokenizer CSV to"
        },
        { 
            Flags::TOKENIZER_OUTPUT_TO_CONSOLE,
            { "--tdebug" },
            false,
            "Output the tokenizer debug to the console"
        },
    };
}
#endif // __OPTIONS_H__