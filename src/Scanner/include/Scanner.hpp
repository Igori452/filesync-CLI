#pragma once
#include "core/include/FileInfo.hpp"

class Scanner 
{

    private:
        std::vector<std::filesystem::path> allPaths;

    public:
        Scanner() = default;

        bool computeAllPaths(const std::string_view spth);
        const std::vector<std::filesystem::path>& getAllPaths() const;
};