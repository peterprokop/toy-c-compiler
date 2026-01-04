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

std::string changeExtension(std::string filePath, std::string newExtension)
{
    size_t lastDotIndex = filePath.find_last_of(".");
    std::string filePathNoExtension = filePath.substr(0, lastDotIndex);
    return newExtension.length()
        ? (filePathNoExtension + "." + newExtension)
        : filePathNoExtension;
}
