#include "sd.hpp"

// Mount/Unmount
MP_RES MicroSDReader::mount()
{
    return f_mount(&m_fs, "", 1) == FR_OK ? MP_RES::OK : MP_RES::ERR;
}

MP_RES MicroSDReader::is_mounted() const
{
    return m_fs.fs_type != 0 ? MP_RES::OK : MP_RES::ERR;
}

MP_RES MicroSDReader::unmount()
{
    FRESULT res = f_mount(NULL, "", 1);
    if (res != FR_OK) return MP_RES::ERR;
    return MP_RES::OK;
}

// File and directory management
FIL* MicroSDReader::open_file(etl::string_view path, uint8_t mode)
{
    for (auto& file : m_file_handles)
    {
        // TODO: change this in the future
        if (!file.obj.fs)
        {
            if (f_open(&file, path.data(), mode) == FR_OK) return &file;
        }
    }

    return nullptr;
}

MP_RES MicroSDReader::close_file(FIL* file)
{
    if (file)
    {
        return f_close(file) == FR_OK ? MP_RES::OK : MP_RES::ERR;
    }
    return MP_RES::ERR;
}

MP_RES SDFile::write(etl::string_view txt)
{
    return f_write(&m_fil, txt.data(), txt.size(), NULL) == FR_OK ? MP_RES::OK : MP_RES::ERR;
}

template <size_t N>
MP_RES SDFile::read(etl::string<N> txt)
{
    return f_read(&m_fil, txt, txt.capacity(), NULL) == FR_OK ? MP_RES::OK : MP_RES::ERR;
}

MP_RES SDFile::append(etl::string_view txt)
{
    f_lseek(&m_fil, this->size());  // TODO: make sure this is correct
    return f_write(&m_fil, txt.data(), txt.size(), NULL) == FR_OK ? MP_RES::OK : MP_RES::ERR;
}

MP_RES SDFile::seek(uint32_t offset)
{
    return f_lseek(&m_fil, offset) == FR_OK ? MP_RES::OK : MP_RES::ERR;
}

MP_RES SDFile::truncate(etl::string_view path)
{
}

uint32_t SDFile::size() const
{
    return f_size(&m_fil);
}

MP_RES SDFile::rename(etl::string_view old_path, etl::string_view new_path)
{
}

MP_RES SDFile::remove()
{
}

MP_RES MicroSDReader::exists(etl::string_view path)
{
}

MP_RES MicroSDReader::mkdir(etl::string_view path)
{
}

MP_RES MicroSDReader::list_files(etl::string_view dir)
{
}

uint64_t MicroSDReader::total_space() const
{
}

uint64_t MicroSDReader::free_space() const
{
}
