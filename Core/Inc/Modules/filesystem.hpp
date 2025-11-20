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
#include "file.hpp"
#include "status.hpp"
#include "utils.hpp"

namespace stm_sd
{

// SD file system
enum class sdfs : uint8_t
{
    fat   = FM_FAT,
    fat32 = FM_FAT32,
    exfat = FM_EXFAT,
    any   = FM_ANY,
    sfd   = FM_SFD
};

// SD type
enum class sd_type : uint8_t
{
    sdsc,
    sdhc,
    sdxc
};

/***********************************************************
 * filesystem namespace for managing the filesystem itself
 ***********************************************************/
namespace filesystem
{

/***********************************************************
 * Public Functions
 ***********************************************************/
void   init(SPI_HandleTypeDef&);
status mount();
bool   is_mounted();
status unmount();

file*  open(const string&, uint8_t);  // returns a file handle
status close(file*);                  // close file
status remove(const string&, bool = false);
status rename(const string&, const string&);

status format(sdfs);

file*  open(const string&, uint8_t);
status close(file*);

bool   exists(const string&);
status mkdir(const string&);

int8_t list(const string&, uint8_t, etl::array<FILINFO, PAGE_SIZE>&);

uint64_t total_space();
uint64_t free_space();

bool is_file(const string&);
bool is_directory(const string&);

/***********************************************************
 * Getters/Setters
 ***********************************************************/
string label();
status set_label(const string&);

string cwd();
status chdir(const string&);

}  // namespace filesystem

}  // namespace stm_sd
