#import <iostream>
#import <fstream>
#import <sstream>

#import "tokens.hpp"

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

bool tryParse(const char *substring, const char *source, bool shouldEndOnWordBoundary)
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

bool tryParseToken(const char *token, const char *source)
{
    return tryParse(token, source, true);
}

bool tryParseConstant(const char *source, std::string *constant)
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

bool tryParseIdentifier(const char *source, std::string *identifier)
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

bool tryParseEndOfTaggedComment(const char **source)
{
    while(**source) {
        if (tryParse((char *)"*/", *source, false)) {
            *source += 2;
            return true;
        }
        (*source)++;
    }
    return false;
}

std::vector<Token> getTokens(const char *inputSource, int &returnValue)
{
    auto tokens = std::vector<Token>();
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
                tokens.push_back(OpenBraceToken());
                source++;
                continue;
            case '}':
                logDebug("}");
                tokens.push_back(CloseBraceToken());
                source++;
                continue;
            case '(':
                logDebug("(");
                tokens.push_back(OpenParenthesisToken());
                source++;
                continue;
            case ')':
                logDebug(")");
                tokens.push_back(CloseParenthesisToken());
                source++;
                continue;
            case ';':
                logDebug(";");
                tokens.push_back(SemicolonToken());
                source++;
                continue;
                
            default:
                break;
        }
        
        // One-line comments, starting with `//`
        if (tryParse((char *)"//", source, false)) {
            logDebug("//");
            // Don't create tokens for comments
            while(source && (*source != '\n')) {
                source++;
            }
            continue;
        }
        
        // Tagged comments, starting with `/*`
        if (tryParse((char *)"/*", source, false)) {
            logDebug("/*");
            // Don't create tokens for comments
            if (tryParseEndOfTaggedComment(&source)) {
                continue;
            } else {
                std::cout << "error: `/*` comment is not closed";
                returnValue = 5;
                return tokens;
            }
        }
        
        // int, void, return
        if (tryParseToken((char *)"int", source)) {
            logDebug("int");
            tokens.push_back(IntToken());
            source += strlen("int");
            continue;
        }
        if (tryParseToken((char *)"void", source)) {
            logDebug("void");
            tokens.push_back(VoidToken());
            source += strlen("void");
            continue;
        }
        if (tryParseToken((char *)"return", source)) {
            logDebug("return");
            tokens.push_back(ReturnToken());
            source += strlen("return");
            continue;
        }
        
        std::string constant = "";
        if (tryParseConstant(source, &constant)) {
            logDebug("constant");
            tokens.push_back(ConstantToken(constant));
            source += constant.size();
            continue;
        }
        
        std::string identifier = "";
        if (tryParseIdentifier(source, &identifier)) {
            logDebug("identifier");
            tokens.push_back(IdentifierToken(identifier));
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

/**
 Return values:
 0 - All good
 1 - No input files
 2 - Unexpected lexer error
 3 - Unexpected character during lexing
 4 - `/*` comment is not closed
 */
int main(int argc, const char * argv[]) {
    
    /*
     Possible options:
     --lex Run the lexer, but stop before parsing
     --parse Run the lexer and parser, but stop before assem-
     bly generation
     --codegen Perform lexing, parsing, and assembly genera-
     tion, but stop before code emission
     -S eEit an assembly file
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
    //std::cout << "info: tokens lexed: " << tokens.size() << std::endl;
    
    return returnValue;
}
