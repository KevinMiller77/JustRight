#include <tokenizer.h>

#define DEBUG_LEVEL DEBUG_LEVEL_TRACE
#include <log.h>

#include <exception>
#include <iostream>

using namespace JR;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file.jr>\n" << std::endl;
        return 1;
    }

    std::string filepath = argv[1];

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

    for (auto &token : tokens) {
        LOG_TRACE("Found tokens: " + token->ToString());
    }

    return 0;
}