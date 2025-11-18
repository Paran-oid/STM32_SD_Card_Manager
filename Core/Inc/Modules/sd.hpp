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

    void write(uint8_t);

    template <size_t N>
    uint32_t read(etl::string<N>& str)
    {
        char buf[N - 1];  // must be char[] because f_read works that way
        UINT bytes_read = 0;

        FRESULT fres = f_read(&m_fil, buf, N - 1, &bytes_read);
        if (fres != FR_OK) return 0;

        str.assign(buf);
        str[bytes_read] = '\0';  // content of files don't null terminate
        return bytes_read;
    }

    StatusCode write(const string&);

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
