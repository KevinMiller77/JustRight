#include <tokenizer.h>

#include <fstream>

namespace JR::Tokenizer {
    bool m_Initialized = false;
    
    std::string m_Filepath = "";
    std::string m_Content = "";

    size_t m_Line = 1;
    size_t m_Col = 1;
    size_t m_Index = 0;

    Ref<Token> m_CurrentToken = nullptr;

    Ref<Token> _ReadToken() {
        if (m_Index >= m_Content.size()) {
            return nullptr;
        }

        Ref<Token> token = CreateRef<Token>();

        token->content = "";
        token->line = m_Line;
        token->column = m_Col;

        if (m_Content[m_Index] == '"') {
            token->type = TokenType::STRING_LITERAL;

            m_Index++;
            while (m_Index < m_Content.size() && m_Content[m_Index] != '"') {
                token->content += m_Content[m_Index];

                m_Index++;
            }

            if (m_Index >= m_Content.size()) {
                throw TokenizerException(m_Filepath, "Unterminated string literal", m_Line, m_Col);
            }

            m_Index++;
            m_Col += token->content.size() + 2;
        } else if (m_Content[m_Index] >= '0' && m_Content[m_Index] <= '9') {
            token->type = TokenType::NUMERIC_LITERAL;

            while (m_Index < m_Content.size() && m_Content[m_Index] >= '0' && m_Content[m_Index] <= '9') {
                token->content += m_Content[m_Index];
                m_Index++;
            }

            m_Col += token->content.size();
        } else if (m_Content[m_Index] == '+' || m_Content[m_Index] == '-' || m_Content[m_Index] == '*' || m_Content[m_Index] == '/') {
            token->type = TokenType::OPERATOR;
            token->content = m_Content[m_Index];

            m_Col++;
            m_Index++;
        } else if (m_Content[m_Index] == '\n') {
            m_Line++;
            m_Col = 1;

            m_Index++;
            return _ReadToken();
        } else if (m_Content[m_Index] == ' ') {
            m_Col++;

            m_Index++;
            return _ReadToken();
        } else {
            throw UnknownSymbolException(m_Filepath, token->line, token->column);
        }

        return token;
    }

    void Init(std::string filepath) {
        auto file = std::ifstream(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open input file " + filepath);
        }

        m_Content = std::string(
            (std::istreambuf_iterator<char>(file)), 
            std::istreambuf_iterator<char>()
        );
        m_Filepath = filepath;

        m_CurrentToken = _ReadToken();
        m_Initialized = true;
    }

    Ref<Token> PeekToken() {
        if (!m_Initialized) {
            throw std::runtime_error("Tokenizer not initialized");
        }

        return m_CurrentToken;
    }

    Ref<Token> NextToken() {
        if (!m_Initialized) {
            throw std::runtime_error("Tokenizer not initialized");
        }

        if (m_CurrentToken == nullptr) {
            return nullptr;
        }
        Ref<Token> token = m_CurrentToken;
        m_CurrentToken = _ReadToken();

        return token;
    }
}