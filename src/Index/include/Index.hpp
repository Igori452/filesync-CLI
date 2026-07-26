#pragma once

#include "core/include/FileInfo.hpp"

/**
 * Использует сканнер для заполнения всего вектора.
 * сканнер удобен так как индекс ничего не знает про
 * файловую систему, и вообще не знает какой путь 
 * и что ему с ним делать.
 *  
 */

class Index 
{

    private:
        std::vector<FileInfo> files;
        

    public:
        Index() = delete;
        Index (const std::vector<std::filesystem::path>& pth);

        void setFiles (const std::vector<std::filesystem::path>& pth);
        bool operator==(const Index & ind) const;

};