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

void tryParseFunction() {
    
}

void tryParseProgram(std::vector<Token> tokens) {
    tryParseFunction();
}
