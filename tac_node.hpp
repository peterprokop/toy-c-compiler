/*
Grammar:
program = Program(function_definition)
function_definition = Function(identifier name, instruction* instructions)
instruction = Mov(operand src, operand dst) | Ret
operand = Imm(int) | Register
*/

// Abstract
class TACNode {};
class InstructionTACNode: public TACNode {
public:
    virtual std::string assembly() {
        return "";
    };
};
class OperandTACNode: public TACNode {
public:
    virtual std::string assembly() {
        return "";
    };
};

class ImmOperandTACNode: public OperandTACNode {
public:
    int constant;
    ImmOperandTACNode(int constant): constant(constant) {};
    
    std::string assembly() override {
        return "$" + std::to_string(constant);
    };
};
class RegisterOperandTACNode: public OperandTACNode {
public:
    // Only EAX for now
    std::string assembly() override {
        return "%eax";
    };
};

class MovInstructionTACNode: public InstructionTACNode {
public:
    OperandTACNode *src;
    OperandTACNode *dst;
    
    MovInstructionTACNode(OperandTACNode * src, OperandTACNode * dst)
        :src(src), dst(dst) {};
    
    std::string assembly() override {
        return "movl " + src->assembly() + ", " + dst->assembly();
    };
};

class RetInstructionTACNode: public InstructionTACNode {
    std::string assembly() override {
        return "ret";
    };
};

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
