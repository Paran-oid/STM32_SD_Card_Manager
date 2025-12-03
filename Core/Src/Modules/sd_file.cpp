#include "sd_file.hpp"

namespace stm_sd
{

Status SDFile::write(const char* s)
{
    return mapFRESULT(f_write(&m_fil, s, strlen(s), NULL));
}

Status SDFile::seek(uint32_t offset)
{
    return mapFRESULT(f_lseek(&m_fil, offset));
}

Status SDFile::truncate()
{
    return mapFRESULT(f_truncate(&m_fil));
}

uint32_t SDFile::size() const
{
    return f_size(&m_fil);
}

Status SDFile::rename(const string& oldPath, const string& newPath)
{
    return mapFRESULT(f_rename(oldPath.c_str(), newPath.c_str()));
}

bool SDFile::is_open()
{
    return m_fil.obj.fs != nullptr;
}

}  // namespace stm_sd
