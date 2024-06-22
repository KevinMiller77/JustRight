#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include <string>

#include "ref.h"

namespace JR::Tokenizer {
    struct Token;
    
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

    enum class TokenType {
        NONE,
        COMMENT, NEWLINE, WHITESPACE,
        CHAR_LITERAL, STRING_LITERAL, INTEGER_LITERAL, BOOLEAN_LITERAL, FLOAT_LITERAL,
        KEYWORD, TYPE, IDENTIFIER, OPERATOR,
        COLON, SEMICOLON, SEPERATOR,
        OPEN_PARAM, CLOSE_PARAM,
        OPEN_SCOPE, CLOSE_SCOPE,
        OPEN_BRACKET, CLOSE_BRACKET,
        OPEN_ANGLE, CLOSE_ANGLE,
    };
    std::string TokenTypeToString(TokenType type);
    
    struct Token {
        std::string content;
        TokenType type;

        size_t line;
        size_t column;

        std::string ToString() {
            return "Token(\"" + content + "\", " + TokenTypeToString(type) + ", " + std::to_string(line) + ", " + std::to_string(column) + ")";
        }
    };
}

#endif // __TOKENIZER_H__