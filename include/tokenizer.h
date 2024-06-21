#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include <string>

#include "ref.h"

namespace JR::Tokenizer {
    enum class TokenType {
        STRING_LITERAL,
        NUMERIC_LITERAL,
        OPERATOR,
    };

    struct Token {
        std::string content;
        TokenType type;

        size_t line;
        size_t column;

        std::string ToString() {
            std::string typeStr = "";
            switch (type) {
                case TokenType::STRING_LITERAL:
                    typeStr = "STRING_LITERAL";
                    break;
                case TokenType::NUMERIC_LITERAL:
                    typeStr = "NUMERIC_LITERAL";
                    break;
                case TokenType::OPERATOR:
                    typeStr = "OPERATOR";
                    break;
            }

            return "Token(\"" + content + "\", " + typeStr + ", " + std::to_string(line) + ", " + std::to_string(column) + ")";
        }
    };

    /**
     * @brief Initialize the Tokenizer with a file path
     * 
     * @param filepath - The path to the file to tokenize
     */
    void Init(std::string filepath);

    /**
     * @brief Peek at the next token from the file
     * 
    */
    Ref<Token> PeekToken();

    /**
     * @brief Retrieve the next token from the file
     * 
     * @return Token 
     */
    Ref<Token> NextToken();

    /**
     * @brief Generic Tokenizer exception
     * 
    */
    class TokenizerException : public std::exception {
    public:
        TokenizerException(std::string filepath, std::string description, size_t line, size_t column) {
            m_Filepath = filepath;
            m_Description = description;
            m_Line = std::to_string(line);
            m_Column =  std::to_string(column);
        }

        std::string what() {
            // What format do compilers give the filepath, line, and column?
            return m_Filepath + ":" + m_Line + ":" + m_Column + ": Tokenizer error => " + m_Description;
        }
    protected:
        std::string m_Filepath;
        std::string m_Description;
        std::string m_Line;
        std::string m_Column;
    };

    /**
     * @brief An unknown symbol was encountered by the Tokenizer
     * 
    */
    class UnknownSymbolException : public TokenizerException {
    public:
        UnknownSymbolException(std::string filepath, size_t line, size_t column)
            : TokenizerException(filepath, "Unknown symbol", line, column) {}
    };
}

#endif // __TOKENIZER_H__