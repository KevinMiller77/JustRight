#include <fstream>
#include <tokenizer.h>

#define DEBUG_LEVEL DEBUG_LEVEL_TRACE
#include <log.h>
#include <options.h>

#include <exception>
#include <iostream>

using namespace JR;

int main(int argc, char *argv[]) {
    if (!Options::registerOptions(argc, argv)) {
        std::cerr << "Invalid arguments\n" << std::endl;
        Options::printHelp(argv[0]);
    } 

    if (Options::hasFlag(Options::OptionFlag::HELP)) {
        Options::printHelp(argv[0]);
        return 0;
    }

    if (Options::hasFlag(Options::OptionFlag::VERSION)) {
        std::cout << "JR Version 0.0.1" << std::endl;
        return 0;
    }

    std::string filepath = Options::expectFlag(Options::OptionFlag::INPUT_FILE);

    try {
        LOG_TRACE("Initializing tokenizer\n");
        Tokenizer::Init(filepath);
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

    if (Options::hasFlag(Options::OptionFlag::TOKENIZER_CSV_OUTPUT_FILE)) {
        LOG_TRACE("Writing Tokenizer CSV file");
        std::string csvFile = Options::expectFlag(Options::OptionFlag::TOKENIZER_CSV_OUTPUT_FILE);
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

    if (Options::hasFlag(Options::OptionFlag::TOKENIZER_OUTPUT_TO_CONSOLE)) {
        LOG_TRACE("Printing tokens to console");
        for (auto &token : tokens) {
            std::cout << token->ToString() << std::endl;
        }
    }

    return 0;
}