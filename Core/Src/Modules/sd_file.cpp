#include "sd.hpp"

namespace stm_sd
{

StatusCode File::write(const string& str)
{
    return f_write(&m_fil, str.data(), str.length(), NULL) == FR_OK ? StatusCode::OK
                                                                    : StatusCode::ERR;
}

StatusCode File::seek(uint32_t offset)
{
    return f_lseek(&m_fil, offset) == FR_OK ? StatusCode::OK : StatusCode::ERR;
}

StatusCode File::truncate()
{
    return f_truncate(&m_fil) == FR_OK ? StatusCode::OK : StatusCode::ERR;
}

uint32_t File::size() const
{
    return f_size(&m_fil);
}

StatusCode File::rename(const string& old_path, const string& new_path)
{
    return f_rename(old_path.c_str(), new_path.c_str()) == FR_OK ? StatusCode::OK : StatusCode::ERR;
}

}  // namespace stm_sd
