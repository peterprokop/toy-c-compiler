/*
EBNF:
<program> ::= <function>
<function> ::= "int" <identifier> "(" "void" ")" "{" <statement> "}"
<statement> ::= "return" <exp> ";"
<exp> ::= <int>
<identifier> ::= ? An identifier token ?
<int> ::= ? A constant token ?
*/

// Abstract
class ASTNode {};

class ProgramASTNode;
class FunctionASTNode;
class StatementASTNode;
class ExpressionASTNode;
class IntASTNode;

class IntASTNode: public ASTNode {
public:
    int constant;
    IntASTNode(int constant): constant(constant) {};
};

class ExpressionASTNode: public ASTNode {
public:
    IntASTNode constant;
    ExpressionASTNode(IntASTNode constant): constant(constant) {};
};

class StatementASTNode: public ASTNode {
public:
    ExpressionASTNode expression;
    StatementASTNode(ExpressionASTNode expression): expression(expression) {};
};

class FunctionASTNode: public ASTNode {
public:
    std::string identifier;
    StatementASTNode statement;
    FunctionASTNode(std::string identifier, StatementASTNode statement)
        :identifier(identifier), statement(statement) {};
};

class ProgramASTNode: public ASTNode {
public:
    FunctionASTNode function;
    ProgramASTNode(FunctionASTNode function):function(function) {}
};
