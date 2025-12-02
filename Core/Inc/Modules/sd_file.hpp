/**
 * @file sd_file.hpp
 * @author Aziz Hmidi (azizhmidi125@gmail.com)
 * @brief
        More or less a C++ wrapper for the fatfs API file related functionalities
 * @version 0.1
 * @date 2025-12-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

/***************************************************************
 * Includes
 ***************************************************************/
extern "C"
{
#include "fatfs.h"
#include "main.h"
}

#include <etl/array.h>
#include <etl/memory.h>
#include <etl/string.h>

#include "defs.hpp"
#include "sd_filesystem.hpp"
#include "status.hpp"
#include "utils.hpp"

namespace stm_sd
{

/***************************************************************
 * Public Typedefs / Structs
 ***************************************************************/

enum FileMode : uint8_t
{
    // Specifies read access to the file. Data can be read from the file.
    FREAD = FA_READ,

    // Specifies write access to the file. Data can be written to the file. Combine with FA_READ for
    // read-write access.
    FWRITE = FA_WRITE,

    // Opens the file. The function fails if the file does not exist. (Default behavior)
    FOPEN_EXISTING = FA_OPEN_EXISTING,

    // Creates a new file. If the file exists, it is truncated and overwritten.
    FCREATE_ALWAYS = FA_CREATE_ALWAYS,

    // Creates a new file. The function fails if the file already exists.
    FCREATE_NEW = FA_CREATE_NEW,

    // Opens the file. If it does not exist, a new file is created.
    FOPEN_ALWAYS = FA_OPEN_ALWAYS,

    // Same as OPEN_ALWAYS, except the read/write pointer is set at the end of the file (append
    // mode).
    FOPEN_APPEND = FA_OPEN_APPEND
};

/***************************************************************
 * SDFile Class for file-related Operations
 ***************************************************************/
class SDFile
{
    /***********************************************************
     * Private Members
     ***********************************************************/
    string m_path;
    FIL    m_fil;

   public:
    /***********************************************************
     * Constructors / Destructor
     ***********************************************************/
    SDFile() = delete;
    SDFile(const string& path) : m_path {path}
    {
    }
    SDFile(SDFile&& f) noexcept : m_path {etl::move(f.m_path)}, m_fil {f.m_fil}
    {
        f.m_fil.obj.fs = nullptr;
    };
    SDFile(const SDFile& f) = delete;
    SDFile& operator=(SDFile&& f) noexcept
    {
        if (this != &f)
        {
            if (m_fil.obj.fs) f_close(&m_fil);
            m_path         = etl::move(f.m_path);
            m_fil          = f.m_fil;
            f.m_fil.obj.fs = nullptr;
        }
        return *this;
    }
    SDFile& operator=(const SDFile& f) = delete;
    ~SDFile()
    {
        if (m_fil.obj.fs) f_close(&m_fil);
    }

    /***********************************************************
     * Public Methods
     ***********************************************************/
    template <size_t N>
    Status write(const etl::string<N>&);
    template <size_t N>
    Status write(const etl::array<uint8_t, N>&);
    Status write(const char*);

    template <size_t N>
    uint32_t read(etl::string<N>&);
    template <size_t N>
    uint32_t read(etl::array<uint8_t, N>&);

    Status seek(uint32_t);
    Status truncate();

    Status rename(const string&, const string&);

    bool is_open();

    /***********************************************************
     * Getters/Setters
     ***********************************************************/
    uint32_t size() const;

    FIL* fil()
    {
        return &m_fil;
    }
    const FIL* fil() const
    {
        return &m_fil;
    }

    const string& path() const
    {
        return m_path;
    }
};

/***********************************************************
 * Template/Inline Functions/Methods Declarations
 ***********************************************************/
template <size_t N>
Status SDFile::write(const etl::string<N>& s)
{
    unsigned int bytesWritten = 0;
    FRESULT      res          = f_write(&m_fil, s.data(), s.length(), &bytesWritten);
    if (res != FR_OK || bytesWritten != s.length()) return mapFRESULT(res);

    return Status::OK;
}

template <size_t N>
Status SDFile::write(const etl::array<uint8_t, N>& arr)
{
    return f_write(&m_fil, arr.data(), arr.size(), NULL) == FR_OK ? Status::OK : Status::ERR;
}

template <size_t N>
uint32_t SDFile::read(etl::array<uint8_t, N>& arr)
{
    char         buf[N];  // must be char[] because f_read works that way
    unsigned int bytesRead;

    FRESULT fres = f_read(&m_fil, buf, N - 1, &bytesRead);
    if (fres != FR_OK) return 0;

    for (uint32_t i = 0; i < bytesRead; i++)
    {
        arr[i] = static_cast<uint8_t>(buf[i]);
    }

    return bytesRead;
}

template <size_t N>
uint32_t SDFile::read(etl::string<N>& s)
{
    // for string overload we will null terminate
    char         buf[N - 1];  // must be char[] because f_read works that way
    unsigned int bytesRead = 0;

    FRESULT fres = f_read(&m_fil, buf, N - 1, &bytesRead);
    if (fres != FR_OK) return 0;

    s.assign(buf, bytesRead);
    s[bytesRead] = '\0';
    return bytesRead;
}

}  // namespace stm_sd
