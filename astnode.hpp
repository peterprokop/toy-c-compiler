#import <iostream>
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

void printWithIndentation(int indentation, std::string value) {
    for (int i = 0; i < indentation; i++) {
        std::cout << " ";
    }
    std::cout << value << std::endl;
}

class IntASTNode: public ASTNode {
public:
    int constant;
    IntASTNode(int constant): constant(constant) {};
    
    void prettyPrint(int indentation) {
        printWithIndentation(indentation, std::to_string(constant));
    };
};

class ExpressionASTNode: public ASTNode {
public:
    IntASTNode *constant;
    ExpressionASTNode(IntASTNode *constant): constant(constant) {};
    
    void prettyPrint(int indentation) {
        // Return is only expression for now
        printWithIndentation(indentation, "Return(");
        constant->prettyPrint(indentation + 2);
        printWithIndentation(indentation, ")");
    }
};

class StatementASTNode: public ASTNode {
public:
    ExpressionASTNode *expression;
    StatementASTNode(ExpressionASTNode *expression): expression(expression) {};
    
    void prettyPrint(int indentation) {
        expression->prettyPrint(indentation + 2);
    }
};

class FunctionASTNode: public ASTNode {
public:
    std::string identifier;
    StatementASTNode *statement;
    FunctionASTNode(std::string identifier, StatementASTNode *statement)
        :identifier(identifier), statement(statement) {};
    
    void prettyPrint(int indentation) {
        printWithIndentation(indentation, "Function(");
        printWithIndentation(indentation + 2, "name=" + identifier);
        printWithIndentation(indentation + 2, "body=");
        statement->prettyPrint(indentation + 2);
        printWithIndentation(indentation, ")");
    }
};

class ProgramASTNode: public ASTNode {
public:
    FunctionASTNode *function;
    
    ProgramASTNode(FunctionASTNode *function):function(function) {}
    
    void prettyPrint() {
        printWithIndentation(0, "Program(");
        function->prettyPrint(2);
        printWithIndentation(0, ")");
    }
};
