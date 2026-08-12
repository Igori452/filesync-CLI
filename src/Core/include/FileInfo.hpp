#pragma once

#include <filesystem>
#include <chrono>

using FileTime = std::filesystem::file_time_type;
using SnapshotTime = std::chrono::system_clock::time_point;

class FileInfo
{

    private:

        SnapshotTime snapshotTime;
        
        std::filesystem::path path_;
        uintmax_t size_;

        FileTime edited_;

        std::filesystem::perms permission_;

    public:

        FileInfo() = delete;
        explicit FileInfo(std::filesystem::path fp);
        
        bool update() noexcept;

        const SnapshotTime& getSnapshotTime() const;

        const std::filesystem::path& path() const;
        uintmax_t size() const;

        const FileTime& edited() const;

        const std::filesystem::perms& permission() const;
};