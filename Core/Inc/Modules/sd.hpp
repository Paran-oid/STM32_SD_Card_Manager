#pragma once

extern "C"
{
#include "fatfs.h"
#include "main.h"
}

#include "etl/array.h"
#include "etl/memory.h"
#include "etl/string.h"
#include "utils.hpp"

namespace stm_sd
{

inline constexpr uint8_t  MAX_FILE_HANDLES = 4;
inline constexpr uint8_t  MAX_LABEL_SIZE   = 32;
inline constexpr uint16_t MAX_DIR_SIZE     = 256;
inline constexpr uint8_t  PAGE_SIZE        = 16;

using path = etl::string<MAX_DIR_SIZE>;

enum class SDFileSystem
{
    FAT   = FM_FAT,
    FAT32 = FM_FAT32,
    EXFAT = FM_EXFAT,
    ANY   = FM_ANY,
    SFD   = FM_SFD
};

enum class SDType
{
    SDSC,
    SDHC,
    SDXC
};

class File
{
   private:
    string m_path;
    FIL    m_fil;

   public:
    File() = delete;

    File(const string&) : m_path {}
    {
    }

    ~File() = default;

    StatusCode write(const string& str);

    template <size_t N>
    StatusCode write(const etl::string<N>& str)
    {
        // TODO: make a check for written bytes to verify that all bytes were written
        return f_write(&m_fil, str.data(), str.length(), NULL) == FR_OK ? StatusCode::OK
                                                                        : StatusCode::ERR;
    }

    template <size_t N>
    StatusCode write(const etl::array<uint8_t, N>& arr)
    {
        return f_write(&m_fil, arr.data(), arr.size(), NULL) == FR_OK ? StatusCode::OK
                                                                      : StatusCode::ERR;
    }

    template <size_t N>
    uint32_t read(etl::string<N>& s)
    {
        // for string overload we will null terminate
        char buf[N - 1];  // must be char[] because f_read works that way
        UINT bytes_read = 0;

        FRESULT fres = f_read(&m_fil, buf, N - 1, &bytes_read);
        if (fres != FR_OK) return 0;

        s.assign(buf, bytes_read);
        s[bytes_read] = '\0';
        return bytes_read;
    }

    template <size_t N>
    uint32_t read(etl::array<uint8_t, N>& arr)
    {
        char buf[N];  // must be char[] because f_read works that way
        UINT bytes_read = 0;

        FRESULT fres = f_read(&m_fil, buf, N - 1, &bytes_read);
        if (fres != FR_OK) return 0;

        for (UINT i = 0; i < bytes_read; i++)
        {
            arr[i] = static_cast<uint8_t>(buf[i]);
        }
        return bytes_read;
    }

    StatusCode seek(uint32_t);
    StatusCode truncate();

    StatusCode rename(const string&, const string&);

    // getters and setters

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

// singleton namespace (like std::filesystem)
namespace filesystem
{

void       init(SPI_HandleTypeDef& hspi);
StatusCode mount();
bool       is_mounted();
StatusCode unmount();

File*      open(const string& path, uint8_t mode);  // open file
StatusCode close(File* file);                       // close file
StatusCode remove(const string& path, bool recursive = false);

StatusCode format(SDFileSystem fmt);

File*      open(const path& p, uint8_t mode);
StatusCode close(File* file);

bool       exists(const string& path);
StatusCode mkdir(const path& p);

int8_t     list(const path& dir_p, uint8_t page, etl::array<FILINFO, PAGE_SIZE>& out);
StatusCode delete_(const path& p, bool recursive);

uint64_t total_space();
uint64_t free_space();

bool is_file(const path& p);
bool is_directory(const path& p);

etl::string<MAX_LABEL_SIZE> label();
StatusCode                  set_label(const etl::string<MAX_LABEL_SIZE>&);

string     cwd();
StatusCode chdir(const string& p);

}  // namespace filesystem

}  // namespace stm_sd
