#import "tac_node.hpp"
#import "astnode.hpp"

std::vector<InstructionTACNode *> tryConvertInstructions(StatementASTNode *statement)
{
    auto instructions = std::vector<InstructionTACNode *>();
    auto constant = statement->expression->constant->constant;
    auto movInstruction =
        new MovInstructionTACNode(
                                  new ImmOperandTACNode(constant),
                                  new RegisterOperandTACNode()
                                  );
    instructions.push_back(movInstruction);
    instructions.push_back(new RetInstructionTACNode());
    return instructions;
}

ProgramTACNode *tryConvertToTAC(ProgramASTNode* program)
{
    auto astFunction = program->function;
    auto instructions = tryConvertInstructions(astFunction->statement);
    auto tacFunction = new FunctionTACNode(astFunction->identifier, instructions);
    return new ProgramTACNode(tacFunction);
}
