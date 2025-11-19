#pragma once

#include "etl/functional.h"
#include "etl/string.h"
#include "etl/unordered_map.h"
#include "etl/vector.h"
#include "utils.hpp"

namespace stm_sd
{

inline constexpr uint8_t ARGS_CAPACITY = 10;  // max number of items passed inside input buf

enum class CommandType
{
    CAT = 0,  // read to file
    ECHO,     // write to a file
    LS,       // list contents of a directory
    RM,       // delete a file/dir
    CP,       // move and/or rename file
    CD,       // enter a directory
    CLEAR,    // clears terminal
    PWD,      // prints current working directory
    MKDIR,    // creates a new directory(or more)
    RMDIR,    // deletes a directory(or more)
    TOUCH,    // create empty file(s)(have to make it update timestamp too at some point)
    NONE,     // undefined
    COMMANDS_COUNT
};

using CmdArgs = etl::vector<string, ARGS_CAPACITY>;
using CmdExec = StatusCode (*)(const CmdArgs&);
using CmdExecMap =
    etl::unordered_map<CommandType, CmdExec, static_cast<size_t>(CommandType::COMMANDS_COUNT)>;

// defined in command_handler.cpp
extern CmdExecMap cmd_table;

void handle_command(const string& str);

}  // namespace stm_sd
