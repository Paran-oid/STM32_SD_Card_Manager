#pragma once

extern "C"
{
#include "ff.h"
#include "main.h"
}

namespace stm_sd
{

enum class Status : uint8_t
{

    OK                  = FR_OK,                   // 0
    DISK_ERR            = FR_DISK_ERR,             // 1
    INT_ERR             = FR_INT_ERR,              // 2
    NOT_READY           = FR_NOT_READY,            // 3
    NO_FILE             = FR_NO_FILE,              // 4
    NO_PATH             = FR_NO_PATH,              // 5
    INVALID_NAME        = FR_INVALID_NAME,         // 6
    DENIED              = FR_DENIED,               // 7
    EXIST               = FR_EXIST,                // 8
    INVALID_OBJECT      = FR_INVALID_OBJECT,       // 9
    WRITE_PROTECTED     = FR_WRITE_PROTECTED,      // 10
    INVALID_DRIVE       = FR_INVALID_DRIVE,        // 11
    NOT_ENABLED         = FR_NOT_ENABLED,          // 12
    NO_FILESYSTEM       = FR_NO_FILESYSTEM,        // 13
    MKFS_ABORTED        = FR_MKFS_ABORTED,         // 14
    TIMEOUT             = FR_TIMEOUT,              // 15
    LOCKED              = FR_LOCKED,               // 16
    NOT_ENOUGH_CORE     = FR_NOT_ENOUGH_CORE,      // 17
    TOO_MANY_OPEN_FILES = FR_TOO_MANY_OPEN_FILES,  // 18
    INVALID_PARAMETER   = FR_INVALID_PARAMETER,    // 19
    ERR                                            // 20 (custom)
};

}  // namespace stm_sd
