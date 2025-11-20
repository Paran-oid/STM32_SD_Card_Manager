#pragma once

/***************************************************************
 * Includes
 ***************************************************************/
extern "C"
{
#include "fatfs.h"
#include "main.h"
}

#include "defs.hpp"
#include "etl/array.h"
#include "etl/memory.h"
#include "etl/string.h"
#include "status.hpp"
#include "utils.hpp"

namespace stm_sd
{

/***************************************************************
 * Public Typedefs / Structs
 ***************************************************************/

enum file_mode : uint8_t
{
    read  = FA_READ,
    write = FA_WRITE,

    open_existing = FA_OPEN_EXISTING,
    create_new    = FA_CREATE_NEW,
    create_always = FA_CREATE_ALWAYS,
    open_always   = FA_OPEN_ALWAYS,
    open_append   = FA_OPEN_APPEND
};
/***************************************************************
 * File Class for file-related Operations
 ***************************************************************/
class file
{
   private:
    /***********************************************************
     * Private Members
     ***********************************************************/
    string m_path;
    FIL    m_fil;

   public:
    /***********************************************************
     * Constructors / Destructor
     ***********************************************************/
    file() = delete;

    file(const string&) : m_path {}
    {
    }

    ~file() = default;

    /***********************************************************
     * Public Methods
     ***********************************************************/
    template <size_t N>
    status write(const etl::string<N>&);
    template <size_t N>
    status write(const etl::array<uint8_t, N>&);
    status write(const char*);

    template <size_t N>
    uint32_t read(etl::string<N>&);
    template <size_t N>
    uint32_t read(etl::array<uint8_t, N>&);

    status seek(uint32_t);
    status truncate();

    status rename(const string&, const string&);

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
status file::write(const etl::string<N>& s)
{
    unsigned int bytes_written = 0;
    FRESULT      res           = f_write(&m_fil, s.data(), s.length(), &bytes_written);
    if (res != FR_OK || bytes_written != s.length()) return map_fresult(res);

    return status::ok;
}

template <size_t N>
status file::write(const etl::array<uint8_t, N>& arr)
{
    return f_write(&m_fil, arr.data(), arr.size(), NULL) == FR_OK ? status::ok : status::err;
}

template <size_t N>
uint32_t file::read(etl::array<uint8_t, N>& arr)
{
    char         buf[N];  // must be char[] because f_read works that way
    unsigned int bytes_read;

    FRESULT fres = f_read(&m_fil, buf, N - 1, &bytes_read);
    if (fres != FR_OK) return 0;

    for (uint32_t i = 0; i < bytes_read; i++)
    {
        arr[i] = static_cast<uint8_t>(buf[i]);
    }

    return bytes_read;
}

template <size_t N>
uint32_t file::read(etl::string<N>& s)
{
    // for string overload we will null terminate
    char         buf[N - 1];  // must be char[] because f_read works that way
    unsigned int bytes_read = 0;

    FRESULT fres = f_read(&m_fil, buf, N - 1, &bytes_read);
    if (fres != FR_OK) return 0;

    s.assign(buf, bytes_read);
    s[bytes_read] = '\0';
    return bytes_read;
}

}  // namespace stm_sd
