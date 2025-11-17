#include "sd.hpp"

SD_RES SDFile::write(estring txt)
{
    return f_write(&m_fil, txt.data(), txt.length(), NULL) == FR_OK ? SD_RES::OK : SD_RES::ERR;
}

SD_RES SDFile::seek(uint32_t offset)
{
    return f_lseek(&m_fil, offset) == FR_OK ? SD_RES::OK : SD_RES::ERR;
}

SD_RES SDFile::truncate()
{
    return f_truncate(&m_fil) == FR_OK ? SD_RES::OK : SD_RES::ERR;
}

uint32_t SDFile::size() const
{
    return f_size(&m_fil);
}

SD_RES SDFile::rename(estring old_path, estring new_path)
{
    return f_rename(old_path.c_str(), new_path.c_str()) == FR_OK ? SD_RES::OK : SD_RES::ERR;
}
