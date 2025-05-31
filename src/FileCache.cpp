#include "FileCache.h"

int FileCache::nextId = 0;

namespace global
{
    FileCache files;
}

FileId FileCache::openFile(const string &filename)
{
    FileId id = nextId++;
    ifstream *stream = new ifstream(filename);
    records[id] = {filename, stream};
    return id;
}

FileId FileCache::openFile(istream &stream, const string &filename)
{
    FileId id = nextId++;
    records[id] = {filename, &stream};
    if (filename.empty())
    {
        records[id].filename = "stream"; // Default name for unnamed streams
    }
    return id;
}

void FileCache::closeFile(FileId id)
{
    auto it = records.find(id);
    if (it != records.end())
    {
        if (it->second.stream && it->second.stream != &std::cin)
        {
            delete it->second.stream; // Only delete if it's not cin
        }
        records.erase(it);
    }
}

void FileCache::closeAllFiles()
{
    for (auto &pair : records)
    {
        if (pair.second.stream && pair.second.stream != &std::cin)
        {
            delete pair.second.stream; // Only delete if it's not cin
        }
    }
    records.clear();
}

FileRecord *FileCache::getFileRecord(FileId id)
{
    auto it = records.find(id);
    if (it != records.end())
    {
        return &it->second;
    }
    return nullptr;
}

FileRecord *FileCache::getFileRecord(const string &filename)
{
    for (auto &pair : records)
    {
        if (pair.second.filename == filename)
        {
            return &pair.second;
        }
    }
    return nullptr;
}
