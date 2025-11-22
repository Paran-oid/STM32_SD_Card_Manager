#pragma once

#include <cstdint>

#include "ff.hpp"

namespace stm_sd
{

enum class status : uint8_t
{

    ok                  = FR_OK,                   // 0
    disk_err            = FR_DISK_ERR,             // 1
    int_err             = FR_INT_ERR,              // 2
    not_ready           = FR_NOT_READY,            // 3
    no_file             = FR_NO_FILE,              // 4
    no_path             = FR_NO_PATH,              // 5
    invalid_name        = FR_INVALID_NAME,         // 6
    denied              = FR_DENIED,               // 7
    exist               = FR_EXIST,                // 8
    invalid_object      = FR_INVALID_OBJECT,       // 9
    write_protected     = FR_WRITE_PROTECTED,      // 10
    invalid_drive       = FR_INVALID_DRIVE,        // 11
    not_enabled         = FR_NOT_ENABLED,          // 12
    no_filesystem       = FR_NO_FILESYSTEM,        // 13
    mkfs_aborted        = FR_MKFS_ABORTED,         // 14
    timeout             = FR_TIMEOUT,              // 15
    locked              = FR_LOCKED,               // 16
    not_enough_core     = FR_NOT_ENOUGH_CORE,      // 17
    too_many_open_files = FR_TOO_MANY_OPEN_FILES,  // 18
    invalid_parameter   = FR_INVALID_PARAMETER,    // 19
    err                                            // 20 (custom)
};

}  // namespace stm_sd
