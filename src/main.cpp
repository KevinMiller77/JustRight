#include <exception>

#include <iostream>
#include <tokenizer.h>

using namespace JR;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << "<file.jr>\n" << std::endl;
        return 1;
    }

    std::string filepath = argv[1];
    try {
        Tokenizer::Init(filepath);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }


    std::vector<Ref<Tokenizer::Token>> tokens;
    try {
        while (Tokenizer::PeekToken() ) {
            Ref<Tokenizer::Token> token = Tokenizer::NextToken();
            tokens.push_back(token);
        }
    } catch (Tokenizer::TokenizerException& e) {
        std::cerr << e.what() << std::endl;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    for (auto& token : tokens) {
        std::cout << token->ToString() << std::endl;
    }

    return 0;
}