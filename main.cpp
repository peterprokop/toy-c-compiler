#import <iostream>
#import <fstream>
#import <sstream>

#import "tokens.hpp"
#import "parser.hpp"
#import "tac_converter.hpp"
#import "assembly_generator.hpp"
#import "shell_exec.hpp"

void logDebug(std::string logContent) {
     // std::cout << logContent << std::endl;
}

bool isWhiteSpace(const char *source) {
    return *source == ' '
        || *source == '\t'
        || *source == '\n';
}

bool isNumeric(const char *source) {
    return (*source >= '0') && (*source <= '9');
}

bool isAlpha(const char *source) {
    return ((*source >= 'a') && (*source <= 'z'))
        || ((*source >= 'A') && (*source <= 'Z'))
        || (*source == '_');
}

bool isWordBoundary(const char *source) {
    return isWhiteSpace(source)
    || (
        !isAlpha(source)
        && !isNumeric(source)
    );
}

bool tryLex(const char *substring, const char *source, bool shouldEndOnWordBoundary)
{
    if (strlen(source) < strlen(substring)) {
        return false;
    }
    
    const char *currentSubstringChar = substring;
    const char *currentSourceChar = source;
    while (*currentSubstringChar) {
        if (*currentSubstringChar != *currentSourceChar) {
            return false;
        }
        currentSubstringChar++;
        currentSourceChar++;
    }
    
    if (shouldEndOnWordBoundary) {
        return isWordBoundary(currentSourceChar);
    }
    return true;
}

bool tryLexToken(const char *token, const char *source)
{
    return tryLex(token, source, true);
}

bool tryLexConstant(const char *source, std::string *constant)
{
    *constant = "";
    bool gotAtLeastOneCharacter = false;
    const char *currentSourceChar = source;
    while (*currentSourceChar) {
        if (isNumeric(currentSourceChar)) {
            gotAtLeastOneCharacter = true;
            *constant += *currentSourceChar;
            currentSourceChar++;
        } else {
            break;
        }
    }
    
    return gotAtLeastOneCharacter && isWordBoundary(currentSourceChar);
}

bool tryLexIdentifier(const char *source, std::string *identifier)
{
    *identifier = "";
    bool gotAtLeastOneCharacter = false;
    const char *currentSourceChar = source;
    while (*currentSourceChar) {
        if (isAlpha(currentSourceChar)) {
            gotAtLeastOneCharacter = true;
            *identifier += *currentSourceChar;
            currentSourceChar++;
        } else {
            break;
        }
    }
    
    return gotAtLeastOneCharacter && isWordBoundary(currentSourceChar);
}

bool tryLexEndOfTaggedComment(const char **source)
{
    while(**source) {
        if (tryLex((char *)"*/", *source, false)) {
            *source += 2;
            return true;
        }
        (*source)++;
    }
    return false;
}

std::vector<Token *> getTokens(const char *inputSource, int &returnValue)
{
    auto tokens = std::vector<Token *>();
    const char *source = inputSource;
    
    while (*source) {
        switch (*source) {
            // Skip whitespace
            case ' ':
            case '\t':
            case '\n':
                logDebug("whitespace");
                source++;
                continue;
            // Simple tokens
            case '{':
                logDebug("{");
                tokens.push_back(new OpenBraceToken());
                source++;
                continue;
            case '}':
                logDebug("}");
                tokens.push_back(new CloseBraceToken());
                source++;
                continue;
            case '(':
                logDebug("(");
                tokens.push_back(new OpenParenthesisToken());
                source++;
                continue;
            case ')':
                logDebug(")");
                tokens.push_back(new CloseParenthesisToken());
                source++;
                continue;
            case ';':
                logDebug(";");
                tokens.push_back(new SemicolonToken());
                source++;
                continue;
                
            default:
                break;
        }
        
        // One-line comments, starting with `//`
        if (tryLex((char *)"//", source, false)) {
            logDebug("//");
            // Don't create tokens for comments
            while(source && (*source != '\n')) {
                source++;
            }
            continue;
        }
        
        // Tagged comments, starting with `/*`
        if (tryLex((char *)"/*", source, false)) {
            logDebug("/*");
            // Don't create tokens for comments
            if (tryLexEndOfTaggedComment(&source)) {
                continue;
            } else {
                std::cout << "error: `/*` comment is not closed";
                returnValue = 5;
                return tokens;
            }
        }
        
        // int, void, return
        if (tryLexToken((char *)"int", source)) {
            logDebug("int");
            tokens.push_back(new IntToken());
            source += strlen("int");
            continue;
        }
        if (tryLexToken((char *)"void", source)) {
            logDebug("void");
            tokens.push_back(new VoidToken());
            source += strlen("void");
            continue;
        }
        if (tryLexToken((char *)"return", source)) {
            logDebug("return");
            tokens.push_back(new ReturnToken());
            source += strlen("return");
            continue;
        }
        
        std::string constant = "";
        if (tryLexConstant(source, &constant)) {
            logDebug("constant");
            tokens.push_back(new ConstantToken(constant));
            source += constant.size();
            continue;
        }
        
        std::string identifier = "";
        if (tryLexIdentifier(source, &identifier)) {
            logDebug("identifier");
            tokens.push_back(new IdentifierToken(identifier));
            source += identifier.size();
            continue;
        }
        
        // Error
        std::cout << "error: unexpected character: " << *source << std::endl;
        returnValue = 3;
        return tokens;
    }
    
    returnValue = 0;
    return tokens;
}

std::string changeExtension(std::string filePath, std::string newExtension)
{
    size_t lastDotIndex = filePath.find_last_of(".");
    std::string filePathNoExtension = filePath.substr(0, lastDotIndex);
    return newExtension.length()
        ? (filePathNoExtension + "." + newExtension)
        : filePathNoExtension;
}

/**
 Return values:
 0 - All good
 1 - No input files
 2 - Unexpected lexer error
 3 - Unexpected character during lexing
 4 - `/*` comment is not closed
 5 - Parse error
 */
int main(int argc, const char * argv[]) {
    
    /*
     Possible options:
     --lex Run the lexer, but stop before parsing
     --parse Run the lexer and parser, but stop before assem-
     bly generation
     --codegen Perform lexing, parsing, and assembly genera-
     tion, but stop before code emission
     -S Emit an assembly file
     */
    // Debug command-line flags
    /*
    std::cout << "argc: " << argc << std::endl;
    
    for (int i = 0; i < argc; ++i) {
        std::cout << "i:" << i << " argv[i]: " << argv[i] << std::endl;
    }
     */
    
    if (argc < 2) {
        std::cout << "error: no input files" << std::endl;
        return 1;
    }
        
    std::string sourcePath = argv[1];
    //std::cout << "info: opening file: " << sourcePath << std::endl;
    std::ifstream sourceInStream(sourcePath);
    
    if (!sourceInStream.is_open()) {
        std::cout << "error: failed to open: " << sourcePath << std::endl;
    }
    
    // Read all to buffer, right now we don't care about size
    std::stringstream sourceBuffer;
    sourceBuffer << sourceInStream.rdbuf();
    const std::string sourceString = sourceBuffer.str();
    
    const char* sourceCString = sourceString.c_str();
    
    int returnValue = 2;
    auto tokens = getTokens(sourceCString, returnValue);
    
    ProgramASTNode *program;
    try {
        program = tryParseProgram(tokens.cbegin(), tokens.cend());
        program->prettyPrint();
    } catch (...) {
        std::cout << "Terminating due to parse error" << std::endl;
        return 5;
    }
    
    auto tacProgram = tryConvertToTAC(program);
    
    // Generate assebly and save to `.s` file
    std::string assemblyFilePath = changeExtension(sourcePath, "s");
    std::ofstream assemblyFile(assemblyFilePath);
        
    generateAssembly(tacProgram, assemblyFile);
    assemblyFile.close();
    
    // Run GCC to create executable file
    std::string executableFilePath = changeExtension(assemblyFilePath, "");
    // On ARM Mac you need to do `arch -x86_64` (or `arch -x86_64 zsh` from shell) first to work with x64 AT&T assembly
    shellExec("arch -x86_64 gcc " + assemblyFilePath + " -o " + executableFilePath);
    
    return returnValue;
}
