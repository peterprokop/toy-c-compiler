#import "tokens.hpp"
#import "astnode.hpp"

/*
EBNF:
<program> ::= <function>
<function> ::= "int" <identifier> "(" "void" ")" "{" <statement> "}"
<statement> ::= "return" <exp> ";"
<exp> ::= <int>
<identifier> ::= ? An identifier token ?
<int> ::= ? A constant token ?
*/

bool checkTokenType(Token *token, TokenType tokenType)
{
    if (token->tokenType() != tokenType) {
        std::cout << "error: unexpected token type:" << token->tokenType()
        << " expected:" << tokenType << std::endl;
        return false;
    }
    return true;
}

void tryParseExpression(std::vector<Token *>::const_iterator &begin,
                        std::vector<Token *>::const_iterator end)
{
    checkTokenType(*begin, TokenTypeConstantToken);
    begin++;
}

void tryParseStatement(std::vector<Token *>::const_iterator &begin,
                       std::vector<Token *>::const_iterator end)
{
    checkTokenType(*begin, TokenTypeReturnToken);
    begin++;
    
    tryParseExpression(begin, end);
    
    checkTokenType(*begin, TokenTypeSemicolonToken);
    begin++;
}

void tryParseFunction(std::vector<Token *>::const_iterator begin,
                      std::vector<Token *>::const_iterator end)
{
    if (begin == end) {
        std::cout << "error: no tokens" << std::endl;
    }
    
    checkTokenType(*begin, TokenTypeIntToken);
    begin++;
    
    checkTokenType(*begin, TokenTypeIdentifierToken);
    begin++;
    
    checkTokenType(*begin, TokenTypeOpenParenthesisToken);
    begin++;
    
    checkTokenType(*begin, TokenTypeVoidToken);
    begin++;
    
    checkTokenType(*begin, TokenTypeCloseParenthesisToken);
    begin++;
    
    checkTokenType(*begin, TokenTypeOpenBraceToken);
    begin++;
    
    tryParseStatement(begin, end);
    
    checkTokenType(*begin, TokenTypeCloseBraceToken);
    begin++;
}

void tryParseProgram(std::vector<Token *>::const_iterator begin,
                     std::vector<Token *>::const_iterator end)
{
    tryParseFunction(begin, end);
}
