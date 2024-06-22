#include <tokenizer.h>
#include <log.h>

#include <exception>
#include <iostream>
#include <fstream>

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

    // Print token information to a csv file
    std::string csv_file = filepath + ".csv";
    std::ofstream ofs(csv_file);
    ofs << "Type,Value,Line,Column\n";
    for (auto &token : tokens) {
        ofs << Tokenizer::TokenTypeToString(token->type) << "," << token->content << "," << token->line << "," << token->column << "\n";
    }

    return 0;
}