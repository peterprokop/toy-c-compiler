// Abstract
class Token {};

//Identifier [a-zA-Z_]
class IdentifierToken: public Token {
public:
    std::string identifier;
    IdentifierToken(std::string identifier):identifier(identifier) {}
};

//Constant [0-9]+
class ConstantToken: public Token {
public:
    std::string constant;
    ConstantToken(std::string constant):constant(constant) {}
};
class IntToken: public Token {};
class VoidToken: public Token {};
class ReturnToken: public Token {};

class OpenParenthesisToken: public Token {};
class CloseParenthesisToken: public Token {};

class OpenBraceToken: public Token {};
class CloseBraceToken: public Token {};

class SemicolonToken: public Token {};
