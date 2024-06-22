#include <tokenizer.h>
#include <log.h>

#include <fstream>
#include <regex>

namespace JR::Tokenizer {
    std::string toRegex(std::string in) {
        in.erase(std::remove_if(in.begin(), in.end(), ::isspace), in.end());
        return "^(" + in + ")";
    }

    // Turn the above array into a regex pattern
    std::string keywords = R"(
        use|exposing|as|
        fun|let|const|
        class|private|protected|public|open|static|
        init|constructor|
        for|in|if|else|while|
        sizeof|type|
        in|nullptr|
        return
    )";

    std::string types = R"(
        void|bool|string|
        uchar|ushort|uint|ulong|
        char|short|int|long|
        float|double
    )";

    std::string operators = R"(
        (\.\.\.)|\.|\:\:|
        \+|\-|\*|\/|\%|
        \=|\+\=|\-\=|\*\=|\/\=|\%\=|\&\=|\|\=|\^\=|\<\<\=|\>\>\=|
        \+\+|\-\-|
        \=\=|\!\=|\<|\>|\<\=|\>\=|
        \&\&|\|\||\!|
        \&|\||\^|\~|\<\<|\>\>|
        \?|:|new|delete
    )";

    typedef std::pair<std::regex, TokenType> Rule;
    const std::vector<Rule> s_Rules = {
        { std::regex("^(\\/\\/.*)(?:\r?\n|\r|$)"),                  TokenType::COMMENT},
        { std::regex("^(\\/\\*[\\s\\S]*?\\*\\/)"),                  TokenType::COMMENT},
        { std::regex("^(\r?\n)"),                                   TokenType::NEWLINE},
        { std::regex("^([ \\t\\r\\f\\v]+)"),                        TokenType::WHITESPACE},
        { std::regex("^\"([^\"]*)\""),                              TokenType::STRING_LITERAL},
        { std::regex("^'([^']|\\\\')'"),                            TokenType::CHAR_LITERAL},
        { std::regex("^(0x[0-9a-fA-F]+)"),                          TokenType::INTEGER_LITERAL},
        { std::regex("^(0b[01]+)"),                                 TokenType::INTEGER_LITERAL},
        { std::regex("^([0-9]+)"),                                  TokenType::INTEGER_LITERAL},
        { std::regex("^(true|false)"),                              TokenType::BOOLEAN_LITERAL},
        { std::regex("^([0-9]+\\.[0-9]+f?)"),                      TokenType::FLOAT_LITERAL},
        { std::regex(toRegex(keywords)),                        TokenType::KEYWORD},
        { std::regex(toRegex(types)),                           TokenType::TYPE},
        { std::regex("^([a-zA-Z_][a-zA-Z0-9_\\-\\*]*)"),            TokenType::IDENTIFIER},
        { std::regex(toRegex(operators)),                        TokenType::OPERATOR},
        { std::regex("^(:)"),                                       TokenType::COLON},
        { std::regex("^(;)"),                                       TokenType::SEMICOLON},
        { std::regex("^(,)"),                                       TokenType::SEPERATOR},
        { std::regex("^(\\()"),                                     TokenType::OPEN_PARAM},
        { std::regex("^(\\))"),                                     TokenType::CLOSE_PARAM},
        { std::regex("^(\\{)"),                                     TokenType::OPEN_SCOPE},
        { std::regex("^(\\})"),                                     TokenType::CLOSE_SCOPE},
        { std::regex("^(\\[)"),                                     TokenType::OPEN_BRACKET},
        { std::regex("^(\\])"),                                     TokenType::CLOSE_BRACKET},
        { std::regex("^(\\<)"),                                     TokenType::OPEN_ANGLE},
        { std::regex("^(\\>)"),                                     TokenType::CLOSE_ANGLE}
    };

    /*
    *   ------------------------------
    *   Tokenizer constants
    *   ------------------------------
    */
    bool m_Initialized = false;
    
    std::string m_Filepath = "";
    std::string m_Content = "";

    size_t m_Line = 1;
    size_t m_Col = 1;
    size_t m_Index = 0;

    Ref<Token> m_CurrentToken = nullptr;

    /*
    *   ------------------------------
    *   Tokenizer internal functions
    *   ------------------------------
    */
    Ref<Token> _ReadToken() {
        if (m_Index >= m_Content.size()) {
            return nullptr;
        }

        Ref<Token> token = CreateRef<Token>();

        token->content = "";
        token->line = m_Line;
        token->column = m_Col;

        std::smatch match;
        std::string uneatenContent(m_Content.begin() + m_Index, m_Content.end());
        for (const auto& rule : s_Rules) {
            if (std::regex_search(uneatenContent, match, rule.first)) {
                token->content = match[1];
                token->type = rule.second;

                m_Index += match[0].length();
                m_Col += match[0].length();

                // Update line and column for newlines in multi-line comments
                if (token->type == TokenType::COMMENT) {
                    std::string fullCapture = std::string(match[0]);
                    size_t newlines = std::count(fullCapture.begin(), fullCapture.end(), '\n');
                    if (newlines > 0) {
                        m_Line += newlines;
                        m_Col = fullCapture.size() - fullCapture.find_last_of('\n');
                    }

                    return _ReadToken();
                }

                // Ignore non-newline whitespace
                if (token->type == TokenType::WHITESPACE) {
                    return _ReadToken();
                }

                // Update line and column for newlines
                if (token->type == TokenType::NEWLINE) {
                    token->content = ""; // Content is empty for newlines, since it's just a line break

                    m_Line++;
                    m_Col = 1;

                    // We only need to tokenize one newline in a row (i.e. skip multiple newlines)
                    // This is because newline may indicate the end of a statement in the parser
                    // but we don't care about multiple in a row. We also dont care about newlines 
                    // following a semicolon, since they are not significant.
                    if (m_CurrentToken == nullptr || 
                        (m_CurrentToken->type == TokenType::NEWLINE || m_CurrentToken->type == TokenType::SEMICOLON)
                    ) {
                        return _ReadToken();
                    }
                }

                // Convert hex and binary literals to base10
                if (token->type == TokenType::INTEGER_LITERAL) {
                    if (token->content.find("0x") != std::string::npos) {
                        token->content = std::to_string(std::stoul(token->content.substr(2), nullptr, 16));
                    } else if (token->content.find("0b") != std::string::npos) {
                        token->content = std::to_string(std::stoul(token->content.substr(2), nullptr, 2));
                    }
                }



                return token;
            }
        }

        if (uneatenContent[0] == '\"') {
            throw TokenizerException(m_Filepath, "Unterminated string literal", m_Line, m_Col);
        } else if (uneatenContent[0] == '\'') {
            throw TokenizerException(m_Filepath, "Invalid or unterminated char literal", m_Line, m_Col);
        }
        throw TokenizerException(m_Filepath, "Unknown symbol", m_Line, m_Col);
    }

    /*
    *   ------------------------------
    *   Tokenizer API functions
    *   ------------------------------
    */

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

        LOG_TRACE("Tokenized: " + token->ToString());
        return token;
    }

    std::string TokenTypeToString(TokenType type) {
        switch (type) {
            case TokenType::NONE                : return "NONE";
            case TokenType::COMMENT             : return "COMMENT";
            case TokenType::NEWLINE             : return "NEWLINE";
            case TokenType::WHITESPACE          : return "WHITESPACE";
            case TokenType::CHAR_LITERAL        : return "CHAR_LITERAL";
            case TokenType::STRING_LITERAL      : return "STRING_LITERAL";
            case TokenType::INTEGER_LITERAL     : return "INTEGER_LITERAL";
            case TokenType::BOOLEAN_LITERAL     : return "BOOLEAN_LITERAL";
            case TokenType::FLOAT_LITERAL       : return "FLOAT_LITERAL";
            case TokenType::KEYWORD             : return "KEYWORD";
            case TokenType::TYPE                : return "TYPE";
            case TokenType::IDENTIFIER          : return "IDENTIFIER";
            case TokenType::OPERATOR            : return "OPERATOR";
            case TokenType::COLON               : return "COLON";
            case TokenType::SEMICOLON           : return "SEMICOLON";
            case TokenType::SEPERATOR           : return "SEPERATOR";
            case TokenType::OPEN_PARAM          : return "OPEN_PARAM";
            case TokenType::CLOSE_PARAM         : return "CLOSE_PARAM";
            case TokenType::OPEN_SCOPE          : return "OPEN_SCOPE";
            case TokenType::CLOSE_SCOPE         : return "CLOSE_SCOPE";
            case TokenType::OPEN_BRACKET        : return "OPEN_BRACKET";
            case TokenType::CLOSE_BRACKET       : return "CLOSE_BRACKET";
            case TokenType::OPEN_ANGLE          : return "OPEN_ANGLE";
            case TokenType::CLOSE_ANGLE         : return "CLOSE_ANGLE";
        }
    }
}