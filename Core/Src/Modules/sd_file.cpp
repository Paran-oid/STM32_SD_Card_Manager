#include "file.hpp"

namespace stm_sd
{

status file::write(const char* s)
{
    return map_fresult(f_write(&m_fil, s, strlen(s), NULL));
}

status file::seek(uint32_t offset)
{
    return map_fresult(f_lseek(&m_fil, offset));
}

status file::truncate()
{
    return map_fresult(f_truncate(&m_fil));
}

uint32_t file::size() const
{
    return f_size(&m_fil);
}

status file::rename(const string& old_path, const string& new_path)
{
    return map_fresult(f_rename(old_path.c_str(), new_path.c_str()));
}

bool file::is_open()
{
    return m_fil.obj.fs != nullptr;
}

}  // namespace stm_sd
