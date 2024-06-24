#include <tokenizer.h>

#define DEBUG_LEVEL DEBUG_LEVEL_TRACE
#include <log.h>
#include <flags.h>
#include <klib/kflags.h>

#include <exception>
#include <iostream>
#include <fstream>

using namespace JR;

int main(int argc, char *argv[]) {
    if (!K::Flags::init(
        argc, argv,
        "<flags> <input.jr>",
        s_FlagDefinitions)
    ) {
        return 1;
    }

    if (K::Flags::getFlag(Flags::VERSION).present) {
        std::cout << "JR Version 0.0.1" << std::endl;
        return 0;
    }

    std::vector<std::string> inputFiles = K::Flags::getUnqualifiedFlags();
    if (inputFiles.size() == 0) {
        LOG_ERROR("No input file provided");
        K::Flags::printHelp();
        return 1;
    }

    try {
        LOG_TRACE("Initializing tokenizer\n");
        Tokenizer::Init(inputFiles[0]);
    } catch (std::exception &e) {
        LOG_ERROR(e.what());
        return 1;
    }

    std::vector <Ref<Tokenizer::Token>> tokens;
    try {
        LOG_TRACE("Tokenizing file...");
        while (Tokenizer::PeekToken() ) {
            Ref<Tokenizer::Token> token = Tokenizer::NextToken();
            tokens.push_back(token);
        }
        LOG_TRACE("File tokenized successfully\n");
    } catch (Tokenizer::TokenizerException& e) {
        LOG_ERROR(e.what());
        return 1;
    } catch (std::exception& e) {
        LOG_ERROR(e.what());
        return 1;
    }

    K::Flags::FlagData tokenizeToCsv = K::Flags::getFlag(Flags::TOKENIZER_CSV_OUTPUT_FILE);
    if (tokenizeToCsv.present) {
        LOG_TRACE("Writing Tokenizer CSV file");
        std::string csvFile = tokenizeToCsv.value;
        std::ofstream file(csvFile);
        if (!file.is_open()) {
            LOG_ERROR("Could not open Tokenizer CSV file: " + csvFile);
            return 1;
        } 
        file << "Type,Value,Line,Column" << std::endl;
        for (auto &token : tokens) {
            file << Tokenizer::TokenType::Strings[token->type] << ",\"" << token->content << "\"," << token->line << "," << token->column << std::endl;
        }
        file.close();
        LOG_TRACE("Tokenizer CSV file written successfully");
    }

    K::Flags::FlagData tokenizeToConsole = K::Flags::getFlag(Flags::TOKENIZER_OUTPUT_TO_CONSOLE);
    if (tokenizeToConsole.present) {
        LOG_TRACE("Printing tokens to console");
        for (auto &token : tokens) {
            std::cout << token->ToString() << std::endl;
        }
    }

    return 0;
}