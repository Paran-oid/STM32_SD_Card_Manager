#pragma once

/***************************************************************
 * Includes
 ***************************************************************/
#include <etl/string.h>
#include <etl/unordered_map.h>
#include <etl/vector.h>

#include "defs.hpp"
#include "printf.h"
namespace stm_sd
{

/***************************************************************
 * Forward Declarations
 ***************************************************************/
enum class Status : uint8_t;

/***************************************************************
 * Public Typedefs / Structs
 ***************************************************************/
enum class CommandType : uint8_t
{
    /*
     * HOW TO ADD A FUNCTIONALITY:
     * .hpp(this file):
     * - add enum for it
     *
     * .cpp(implementation of this .hpp)
     * - add extern declaration for functions (and define it somewhere)
     * - add it to the cmd_table unordered_map
     * - map it in check_command static function with it's string representation
     *
     */

    CAT = 0,      // read to file
    ECHO,         // write to a file
    LS,           // list contents of a directory
    RM,           // delete a file/dir
    CP,           // move and/or rename file
    CD,           // enter a directory
    CLEAR,        // clears terminal
    PWD,          // prints current working directory
    MKDIR,        // creates a new directory(or more)
    RMDIR,        // deletes a directory(or more)
    TOUCH,        // create empty file(s)(have to make it update timestamp too at some point)
    MV,           // move (and/or rename) a file
    FREE_SPACE,   // remaining free space of the sd card
    TOTAL_SPACE,  // total space inside the sd card
    NONE,         // undefined
    COUNT         // returns number of command types including none
};

/**
 *  @brief For the CmdArgs type we expect that the user will be entering max CMD_HANDLER_ARGS_CAPACITY arguments.
    That is we can't do something like this:
    
    case CMD_HANDLER_ARGS_CAPACITY = 4 
    cmd -r -f -c smth.c -o smth.o
    
    !!!IMPOSSIBLE!!! Either increase CMD_HANDLER_ARGS_CAPACITY in defs or the command will not be accepted
 * 
 */
using CmdArgs = etl::vector<string, CMD_HANDLER_ARGS_CAPACITY>;
using CmdExec = Status (*)(const CmdArgs&);
using CmdExecMap =
    etl::unordered_map<CommandType, CmdExec, static_cast<size_t>(CommandType::COUNT)>;

/***************************************************************
 * Function Declarations
 ***************************************************************/
void handleCommand(const string&);

}  // namespace stm_sd
