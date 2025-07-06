#pragma once

#include <string>

namespace Citadel
{
class FileUtils
{
public:
    static std::string ReadFileAsString(const std::string& filePath);
};
} // namespace Citadel
