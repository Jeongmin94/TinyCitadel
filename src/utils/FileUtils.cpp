#include "pch.h"

#include "FileUtils.h"

std::string Citadel::FileUtils::ReadFileAsString(const std::string& filePath)
{
    std::ifstream inputFileStream(filePath);

    if (inputFileStream.is_open() == false)
    {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return std::string();
    }

    std::stringstream stringStream;
    stringStream << inputFileStream.rdbuf();
    inputFileStream.close();

    return stringStream.str();
}
