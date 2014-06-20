#ifndef TOKENIZER_H
#define TOKENIZER_H
#include "tokens.h"
#include <cstring>
#include <string>
#include <map>

SWIFT_NS_BEGIN

struct TokenizerError
{
    int line;
    int column;
    int error_code;
};
struct KeywordInfo
{
    Keyword::T keyword;
    KeywordType::T type;
};
class Tokenizer
{
    struct State
    {
        const wchar_t* cursor;
        bool hasSpace;
        bool inStringExpression;
    };
public:
    Tokenizer(const wchar_t* data);
    ~Tokenizer();
public:
    void set(const wchar_t* data);
    bool next(Token& token);
    bool peek(Token& token);
private:
    bool nextImpl(Token& token);
    void resetToken(Token& token);
    bool skipSpaces();
    bool get(wchar_t &ch);
    void unget();
    bool peek(wchar_t &ch);
    
    void tassert(bool cond);
    wchar_t must_get();
    void match(wchar_t ch);
    
    
    bool readSymbol(Token& token, TokenType::T type);
    bool readOperator(Token& token, int max = 0);
    bool readComment(Token& token);
    bool readMultilineComment(Token& token);
    bool readString(Token& token);
    bool readNumber(Token& token);
    bool readNumberLiteral(Token& token, int base);
    bool readIdentifier(Token& token);
private:
    wchar_t* data;
    const wchar_t* end;
    size_t size;
    State state;
    std::map<std::wstring, KeywordInfo> keywords;
};


SWIFT_NS_END
#endif//TOKENIZER_H