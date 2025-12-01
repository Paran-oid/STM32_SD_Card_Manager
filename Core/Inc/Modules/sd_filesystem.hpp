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
#include "status.hpp"
#include "utils.hpp"

namespace stm_sd
{

/***************************************************************
 * Forward Declarations
 ***************************************************************/
class SDFile;

// SD file system
enum class SDFS : uint8_t
{
    fat   = FM_FAT,
    fat32 = FM_FAT32,
    exfat = FM_EXFAT,
    any   = FM_ANY,
    sfd   = FM_SFD
};

// SD type
enum class SDType : uint8_t
{
    sdsc,
    sdhc,
    sdxc
};

/***********************************************************
 * filesystem namespace for managing the filesystem itself
 ***********************************************************/
namespace sd_filesystem
{

/***********************************************************
 * Public Functions
 ***********************************************************/
void   init(SPI_HandleTypeDef&);
Status mount();
bool   isMounted();
Status unmount();

SDFile* open(const string&, uint8_t);  // returns a file handle
Status  close(SDFile*);                // close file
Status  copy(const string&, const string&, uint8_t);
Status  remove(const string&, bool = false);
Status  rename(const string&, const string&);

Status format(SDFS);

SDFile* open(const string&, uint8_t);
Status  close(SDFile*);

bool   exists(const string&);
Status mkdir(const string&);

int8_t list(const string&, uint8_t, etl::array<FILINFO, PAGE_SIZE>&);

uint64_t totalSpace();
uint64_t freeSpace();

bool isFile(const string&);
bool isDirectory(const string&);

/***********************************************************
 * Getters/Setters
 ***********************************************************/
string label();
Status setLabel(const string&);

string cwd();
Status chdir(const string&);

}  // namespace sd_filesystem

}  // namespace stm_sd
