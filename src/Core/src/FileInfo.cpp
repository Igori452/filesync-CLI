#include "../include/FileInfo.hpp"

FileInfo::FileInfo(std::filesystem::path fp) 
    : path_(std::move(fp)) {}

bool FileInfo::update() noexcept
{
    std::error_code ec;

    if (!std::filesystem::exists(path_))
    {
        // Запись в логгер информации 
        // о том что файла по данному пути не существует
        return false;
    
    }
    
    size_ = std::filesystem::file_size(path_, ec);
    if (ec) 
    {
        // Запись в логгер
        return false;
    }
    edited_ = std::filesystem::last_write_time(path_, ec);
    if (ec) 
    {
        // Запись в логгер
        return false;
    }

    permission_ = std::filesystem::status(path_, ec).permissions();

    if (ec) 
    {
        // Запись в логгер
        return false;
    }

    snapshotTime = std::chrono::system_clock::now();

    return true;
}

const SnapshotTime& FileInfo::getSnapshotTime() const 
{
    return snapshotTime;
}

const std::filesystem::path& FileInfo::path() const 
{
    return path_;
}

uintmax_t FileInfo::size() const 
{
    return size_;
}

const FileTime& FileInfo::edited() const 
{
    return edited_;
}

const std::filesystem::perms& FileInfo::permission() const 
{
    return permission_;
}