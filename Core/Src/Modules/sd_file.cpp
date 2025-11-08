#include "sd.hpp"

MP_RES SDFile::write(etl::string_view txt)
{
    return f_write(&m_fil, txt.data(), txt.length(), NULL) == FR_OK ? MP_RES::OK : MP_RES::ERR;
}

MP_RES SDFile::seek(uint32_t offset)
{
    return f_lseek(&m_fil, offset) == FR_OK ? MP_RES::OK : MP_RES::ERR;
}

MP_RES SDFile::truncate()
{
    return f_truncate(&m_fil) == FR_OK ? MP_RES::OK : MP_RES::ERR;
}

uint32_t SDFile::size() const
{
    return f_size(&m_fil);
}

MP_RES SDFile::rename(etl::string_view old_path, etl::string_view new_path)
{
    return f_rename(old_path.data(), new_path.data()) == FR_OK ? MP_RES::OK : MP_RES::ERR;
}
