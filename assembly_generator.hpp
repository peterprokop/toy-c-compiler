#import "tac_node.hpp"

void generateAssembly(ProgramTACNode* program, std::ostream &output)
{
    auto function = program->function;
    
    // "_" is for macOS only
    output << "\t.globl _" << function->identifier << "\n";
    
    output << "_" << function->identifier << ":\n";
    
    for (const auto instruction : function->instructions) {
        output << "\t" << instruction->assembly() << "\n";
    }
    
    output << std::endl;
}
