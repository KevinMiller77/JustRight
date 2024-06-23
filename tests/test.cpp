#include "common.test.h"

#include <tokenizer.h>

// #define DEBUG_LEVEL DEBUG_LEVEL_TRACE
#include <log.h>

#include <fstream>

#define STR(X) #X
#define XSTR(X) STR(X)
#ifndef TESTS_ROOT_DIR
#define TESTS_ROOT_DIR .
#endif

int test_TokenizerSingleTokenType(std::string typeName, JR::Tokenizer::TokenType::Enum type) {
    std::string directory = XSTR(TESTS_ROOT_DIR);

    std::string inputFilepath = directory + "/artifacts/" + typeName + ".jr";
    std::string randomizedFilepath = directory + "/artifacts/" + typeName + "_randomized.jr";
    std::string csvFilepath = directory + "/artifacts/" + typeName + "_output.csv";
    
    // Randomize the file
    if (randomizeFileLinesOutputtingNew(inputFilepath, randomizedFilepath)) {
        LOG_ERROR("Failed to randomize file");
        return 1;
    }
    
    JR::Tokenizer::Reset();
    try {
        LOG_TRACE("Initializing tokenizer");
        JR::Tokenizer::Init(randomizedFilepath);
    } catch (std::exception &e) {
        LOG_ERROR(e.what());
        return 1;
    }

    std::vector <Ref<JR::Tokenizer::Token>> tokens;
    try {
        LOG_TRACE("Tokenizing file...");
        while (JR::Tokenizer::PeekToken() ) {
            Ref<JR::Tokenizer::Token> token = JR::Tokenizer::NextToken();
            tokens.push_back(token);
        }
        LOG_TRACE("File tokenized successfully");
    } catch (JR::Tokenizer::TokenizerException& e) {
        LOG_ERROR(e.what());
        return 1;
    } catch (std::exception& e) {
        LOG_ERROR(e.what());
        return 1;
    }

    for (auto &token : tokens) {
        LOG_TRACE("Found tokens: " + token->ToString());
    }

    // Output a csv of the output for validation
    std::ofstream ofs = std::ofstream(csvFilepath);
    ofs << "Type,Value,Line,Column";
    for (auto &token : tokens) {
        ofs << JR::Tokenizer::TokenType::Strings[token->type] << ",";
        ofs << ((token->type == JR::Tokenizer::TokenType::SEPERATOR) ? std::string("\",\"") : token->content);
        ofs << "," << token->line << "," << token->column << "";
    }
    ofs.close();


    std::vector<std::string> lines = {};
    if(getFileLinesWithoutCommentsOrWhitespace(randomizedFilepath, lines)) {
        LOG_ERROR("Failed to get file lines");
        return 1;
    }

    size_t lineNo = 0;
    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i]->type != type) {
            LOG_TRACE("Skipping token of type " + JR::Tokenizer::TokenType::Strings[tokens[i]->type] + " with value " + tokens[i]->content);
            continue;
        }

        if (lineNo >= lines.size()) {
            LOG_ERROR("Tokenization error => Expected more tokens but reached end of validation file. Remaining tokens");
            for (size_t j = i; j < tokens.size(); j++) {
                LOG_ERROR(tokens[j]->ToString());
            }

            break;
        }

        // Remove all whitespace from the line[lineNo]
        lines[lineNo].erase(std::remove_if(lines[lineNo].begin(), lines[lineNo].end(), isspace), lines[lineNo].end());
        if (tokens[i]->content != lines[lineNo]) {
            // Print an error with the token line information using filepath as filepath
            // It should be in file:line:col format
            std::string err = randomizedFilepath + ":" + std::to_string(tokens[i]->line) + ":" + std::to_string(tokens[i]->column);
            err += ": Expected \"" + lines[lineNo] + "\" but got \"" + tokens[i]->content + "\" ";
            err += "is " + lines[lineNo] + " a valid " + JR::Tokenizer::TokenType::Strings[type] + " token?";
            LOG_ERROR(err.c_str());
            return 1;
        }
        LOG_TRACE("Tokenization validated: " + tokens[i]->ToString());
        lineNo++;
    }

    if (lineNo < lines.size()) {
        LOG_ERROR("Tokenization error => Expected more tokens but reached end of tokens. Remaining lines");
        for (size_t i = lineNo; i < lines.size(); i++) {
            LOG_ERROR(lines[i]);
        }
        return 1;
    }
    LOG_TRACE("Tokenization validated successfully");

    // Cleanup files generated
    remove(randomizedFilepath.c_str());
    remove(csvFilepath.c_str());

    return 0;
}

int main() {
    std::vector<std::string> failedTests = {};

    LOG_INFO("------------------------------");
    LOG_INFO("Running Tokenizer Operator test...");
    LOG_INFO("------------------------------");
    if(test_TokenizerSingleTokenType("operators", JR::Tokenizer::TokenType::OPERATOR)) {
        LOG_ERROR("Test Failed: TokenizerOperator");
        failedTests.push_back("Tokenizer Operator");
    }
    LOG_INFO("Test Passed: TokenizerOperator");
    LOG_INFO("");

    LOG_INFO("------------------------------");
    LOG_INFO("Running Tokenizer Types test...");
    LOG_INFO("------------------------------");
    if(test_TokenizerSingleTokenType("types", JR::Tokenizer::TokenType::TYPE)) {
        LOG_ERROR("Test Failed: TokenizerType");
        failedTests.push_back("Tokenizer Type");
    }
    LOG_INFO("Test Passed: TokenizerType");
    LOG_INFO("");

    LOG_INFO("------------------------------");
    LOG_INFO("Running Tokenizer Keywords test...");
    LOG_INFO("------------------------------");
    if(test_TokenizerSingleTokenType("keywords", JR::Tokenizer::TokenType::KEYWORD)) {
        LOG_ERROR("Test Failed: TokenizerKeywords");
        failedTests.push_back("Tokenizer Keywords");
    }
    LOG_INFO("Test Passed: TokenizerKeywords");
    LOG_INFO("");

    LOG_INFO("------------------------------");
    LOG_INFO("Running Tokenizer Identifiers test...");
    LOG_INFO("------------------------------");
    if(test_TokenizerSingleTokenType("identifiers", JR::Tokenizer::TokenType::IDENTIFIER)) {
        LOG_ERROR("Test Failed: TokenizerIdentifiers");
        failedTests.push_back("Tokenizer Identifiers");
    }
    LOG_INFO("Test Passed: TokenizerIdentifiers");

    for (auto &test : failedTests) {
        LOG_ERROR("Failed test: " + test);
    }
    return failedTests.size() > 0 ? 1 : 0;
}