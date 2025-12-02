/**
 * @file defs.hpp
 * @author Aziz Hmidi (azizhmidi125@gmail.com)
 * @brief Contains defines used throughout the code base
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
#include "main.h"
}
#include <etl/string.h>

/***************************************************************
 * Forward Declarations
 ***************************************************************/

namespace stm_sd
{

/***************************************************************
 * Public Typedefs / Structs
 ***************************************************************/

//? inline constexpr to make sure constexpr elements are declared only once

inline constexpr uint16_t SSIZE      = 256U;  // usual string size, must not be less than 256
inline constexpr uint16_t BLOCK_SIZE = 512U;  // number of bytes to extract when reading a file

inline constexpr uint8_t CMD_HANDLER_ARGS_CAPACITY =
    10;  // max number of items passed inside input buf

inline constexpr uint8_t  MAX_FILE_HANDLES = 16U;   // if modified FS_LOCK must be modified too
inline constexpr uint8_t  MAX_LABEL_SIZE   = 32U;   // specified in fatfs itself
inline constexpr uint16_t MAX_DIR_SIZE     = 256U;  // just like previously
inline constexpr uint8_t  PAGE_SIZE        = 16U;   // can be customized

inline constexpr uint64_t BYTES_PER_GB = 1024.0 * 1024.0 * 1024.0;

inline constexpr uint8_t OVERWRITE = (1 << 1);  // custom created OVERWRITE
inline constexpr uint8_t RECURSIVE = (1 << 2);  // custom created RECURSIVE

inline constexpr uint8_t DEFAULT_TIMEOUT = 100;  // timeout in ms
inline constexpr uint8_t BACKSPACE = 0x7F;  // some consoles read backspace as 0x7F instead of '\b'

using string = etl::string<SSIZE>;  // typedef for easier writing of string type

}  // namespace stm_sd
