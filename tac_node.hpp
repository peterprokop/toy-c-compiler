/*
Grammar:
program = Program(function_definition)
function_definition = Function(identifier name, instruction* instructions)
instruction = Mov(operand src, operand dst) | Ret
operand = Imm(int) | Register
*/

// Abstract
class TACNode {};
class InstructionTACNode: public TACNode {};
class OperandTACNode: public TACNode {};

class ImmOperandTACNode: public TACNode {};
class RegisterOperandTACNode: public TACNode {};

class MovInstructionTACNode: public TACNode {
public:
    OperandTACNode *src;
    OperandTACNode *dst;
    
    MovInstructionTACNode(OperandTACNode * src, OperandTACNode * dst)
        :src(src), dst(dst) {};
};

class RetInstructionTACNode: public TACNode {};

class FunctionTACNode: public TACNode {
public:
    std::string identifier;
    std::vector<InstructionTACNode *> instructions;

    FunctionTACNode(std::string identifier, std::vector<InstructionTACNode *> instructions)
        :identifier(identifier), instructions(instructions) {};
};


class ProgramTACNode: public TACNode {
public:
    FunctionTACNode *function;

    ProgramTACNode(FunctionTACNode *function):function(function) {}
};
