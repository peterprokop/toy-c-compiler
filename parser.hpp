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
        
        throw std::invalid_argument("unexpected token type");
        
        return false;
    }
    return true;
}

ExpressionASTNode *tryParseExpression(std::vector<Token *>::const_iterator &begin,
                                      std::vector<Token *>::const_iterator end)
{
    checkTokenType(*begin, TokenTypeConstantToken);
    const std::string stringValue = dynamic_cast<ConstantToken *>(*begin)->constant;
    IntASTNode *intASTNode = new IntASTNode(std::stoi(stringValue));
    begin++;
    return new ExpressionASTNode(intASTNode);
}

StatementASTNode *tryParseStatement(std::vector<Token *>::const_iterator &begin,
                                    std::vector<Token *>::const_iterator end)
{
    checkTokenType(*begin, TokenTypeReturnToken);
    begin++;
    
    ExpressionASTNode *expression = tryParseExpression(begin, end);
    
    checkTokenType(*begin, TokenTypeSemicolonToken);
    begin++;
    return new StatementASTNode(expression);
}

FunctionASTNode *tryParseFunction(std::vector<Token *>::const_iterator begin,
                                  std::vector<Token *>::const_iterator end)
{
    if (begin == end) {
        std::cout << "error: no tokens" << std::endl;
    }
    
    checkTokenType(*begin, TokenTypeIntToken);
    begin++;
    
    checkTokenType(*begin, TokenTypeIdentifierToken);
    const std::string functionName = dynamic_cast<IdentifierToken *>(*begin)->identifier;
    begin++;
    
    checkTokenType(*begin, TokenTypeOpenParenthesisToken);
    begin++;
    
    checkTokenType(*begin, TokenTypeVoidToken);
    begin++;
    
    checkTokenType(*begin, TokenTypeCloseParenthesisToken);
    begin++;
    
    checkTokenType(*begin, TokenTypeOpenBraceToken);
    begin++;
    
    StatementASTNode *statement = tryParseStatement(begin, end);
    
    checkTokenType(*begin, TokenTypeCloseBraceToken);
    begin++;
    
    return new FunctionASTNode(functionName, statement);
}

ProgramASTNode *tryParseProgram(std::vector<Token *>::const_iterator begin,
                                std::vector<Token *>::const_iterator end)
{
    FunctionASTNode *function = tryParseFunction(begin, end);
    return new ProgramASTNode(function);
}
