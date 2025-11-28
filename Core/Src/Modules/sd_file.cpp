#include "file.hpp"

namespace stm_sd
{

Status File::write(const char* s)
{
    return mapFRESULT(f_write(&m_fil, s, strlen(s), NULL));
}

Status File::seek(uint32_t offset)
{
    return mapFRESULT(f_lseek(&m_fil, offset));
}

Status File::truncate()
{
    return mapFRESULT(f_truncate(&m_fil));
}

uint32_t File::size() const
{
    return f_size(&m_fil);
}

Status File::rename(const string& oldPath, const string& newPath)
{
    return mapFRESULT(f_rename(oldPath.c_str(), newPath.c_str()));
}

bool File::is_open()
{
    return m_fil.obj.fs != nullptr;
}

}  // namespace stm_sd
