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
MP_RES MicroSDReader::open_file(etl::string_view path, uint8_t mode)
{
    return f_open(&m_fil, path.data(), mode) == FR_OK ? MP_RES::OK : MP_RES::ERR;
}

MP_RES MicroSDReader::close_file()
{
    return f_close(&m_fil) == FR_OK ? MP_RES::OK : MP_RES::ERR;
}

MP_RES MicroSDReader::write_file(etl::string_view path, etl::string_view data)
{
    m_fil.
    // TODO: implement
}

template <size_t N>
MP_RES MicroSDReader::read_file(etl::string_view path, etl::string<N> data)
{
    f_puts("Hello from Aziz", &file);
}

MP_RES MicroSDReader::append_file(etl::string_view path, etl::string_view data)
{
    // TODO: implement
}

MP_RES MicroSDReader::seek(uint32_t offset)
{
    // TODO: implement
}

MP_RES MicroSDReader::truncate(etl::string_view path)
{
    // TODO: implement
}

uint32_t MicroSDReader::file_size(etl::string_view path) const
{
    // TODO: implement
}

MP_RES MicroSDReader::rename_file(etl::string_view old_path, etl::string_view new_path)
{
    // TODO: implement
}

MP_RES MicroSDReader::delete_file(etl::string_view path)
{
    // TODO: implement
}

MP_RES MicroSDReader::exists(etl::string_view path)
{
    // TODO: implement
}

MP_RES MicroSDReader::mkdir(etl::string_view path)
{
    // TODO: implement
}

MP_RES MicroSDReader::list_files(etl::string_view dir)
{
    // TODO: implement
}

uint64_t MicroSDReader::total_space() const
{
    // TODO: implement
}

uint64_t MicroSDReader::free_space() const
{
    // TODO: implement
}
