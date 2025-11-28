#pragma once

/***************************************************************
 * Includes
 ***************************************************************/
extern "C"
{
#include "main.h"
}

/***************************************************************
 * Forward Declarations
 ***************************************************************/
namespace etl
{

template <size_t N>
class string;

}

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

inline constexpr uint8_t  MAX_FILE_HANDLES = 16U;  // if modified FS_LOCK must be modified too
inline constexpr uint8_t  MAX_LABEL_SIZE   = 32U;
inline constexpr uint16_t MAX_DIR_SIZE     = 256U;
inline constexpr uint8_t  PAGE_SIZE        = 16U;

inline constexpr uint64_t BYTES_PER_GB = 1024.0 * 1024.0 * 1024.0;

inline constexpr uint8_t OVERWRITE = (1 << 1);
inline constexpr uint8_t RECURSIVE = (1 << 2);

using string = etl::string<SSIZE>;

#define CLEAR_CMD "\033[2J\033[H\r\n"

}  // namespace stm_sd
