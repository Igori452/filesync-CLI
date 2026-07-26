#pragma once
#include "core/include/FileInfo.hpp"

class Scanner 
{

    private:
        std::vector<std::filesystem::path> pathData;

    public:
        Scanner() = default;

        bool setPathData(std::filesystem::path path);
        const std::vector<std::filesystem::path>& getPathData() const;
};