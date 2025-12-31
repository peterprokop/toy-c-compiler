enum TokenType {
    TokenTypeToken,
    TokenTypeIdentifierToken,
    TokenTypeConstantToken,
    TokenTypeIntToken,
    TokenTypeVoidToken,
    TokenTypeReturnToken,
    TokenTypeOpenParenthesisToken,
    TokenTypeCloseParenthesisToken,
    TokenTypeOpenBraceToken,
    TokenTypeCloseBraceToken,
    TokenTypeSemicolonToken,
};

// Abstract
class Token {
public:
    virtual TokenType tokenType() {
        return TokenTypeToken;
    }
};

//Identifier [a-zA-Z_]
class IdentifierToken: public Token {
public:
    std::string identifier;
    IdentifierToken(std::string identifier):identifier(identifier) {}
    
    TokenType tokenType() override {
        return TokenTypeIdentifierToken;
    }
};

//Constant [0-9]+
class ConstantToken: public Token {
public:
    std::string constant;
    ConstantToken(std::string constant):constant(constant) {}
    
    TokenType tokenType() override {
        return TokenTypeConstantToken;
    }
};
class IntToken: public Token {
public:
    TokenType tokenType() override {
        return TokenTypeIntToken;
    }
};
class VoidToken: public Token {
public:
    TokenType tokenType() override {
        return TokenTypeVoidToken;
    }
};
class ReturnToken: public Token {
public:
    TokenType tokenType() override {
        return TokenTypeReturnToken;
    }
};

class OpenParenthesisToken: public Token {
public:
    TokenType tokenType() override {
        return TokenTypeOpenParenthesisToken;
    }
};
class CloseParenthesisToken: public Token {
public:
    TokenType tokenType() override {
        return TokenTypeCloseParenthesisToken;
    }
};

class OpenBraceToken: public Token {
public:
    TokenType tokenType() override {
        return TokenTypeOpenBraceToken;
    }
};
class CloseBraceToken: public Token {
public:
    TokenType tokenType() override {
        return TokenTypeCloseBraceToken;
    }
};

class SemicolonToken: public Token {
public:
    TokenType tokenType() override {
        return TokenTypeSemicolonToken;
    }
};
