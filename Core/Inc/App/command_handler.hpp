#pragma once

/***************************************************************
 * Includes
 ***************************************************************/
#include "defs.hpp"
#include "etl/functional.h"
#include "etl/string.h"
#include "etl/unordered_map.h"
#include "etl/vector.h"
#include "status.hpp"
#include "utils.hpp"

namespace stm_sd
{

/***************************************************************
 * Public Typedefs / Structs
 ***************************************************************/
enum class command_type : uint8_t
{
    /*
     * HOW TO ADD A FUNCTIONALITY:
     * - add enum for it
     * - add extern declaration for functions (and define it somewhere)
     * - add it to the cmd_table unordered_map
     * - pass it in check_command_type function
     */

    cat = 0,    // read to file
    echo,       // write to a file
    ls,         // list contents of a directory
    rm,         // delete a file/dir
    cp,         // move and/or rename file
    cd,         // enter a directory
    clear,      // clears terminal
    pwd,        // prints current working directory
    mkdir,      // creates a new directory(or more)
    rmdir,      // deletes a directory(or more)
    touch,      // create empty file(s)(have to make it update timestamp too at some point)
    mv,         // move (and/or rename) a file
    none,       // undefined
    total_size  // returns number of command types including none
};

using cmd_args = etl::vector<string, CMD_HANDLER_ARGS_CAPACITY>;
using cmd_exec = status (*)(const cmd_args&);
using cmd_exec_map =
    etl::unordered_map<command_type, cmd_exec, static_cast<size_t>(command_type::total_size)>;

/***************************************************************
 * Exported Objects
 ***************************************************************/
extern cmd_exec_map cmd_table;  // defined in command_handler.cpp

/***************************************************************
 * Function Declarations
 ***************************************************************/
void handle_command(const string&);

}  // namespace stm_sd
