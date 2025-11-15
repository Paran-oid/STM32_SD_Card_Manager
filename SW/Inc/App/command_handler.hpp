#pragma once

#include "etl/string.h"
#include "etl/unordered_map.h"
#include "etl/vector.h"
#include "utils.hpp"

constexpr uint16_t ARGS_CAPACITY = 10;  // max number of items inside args buf inside the function

const uint16_t     COMMANDS_COUNT = 10;
extern etl::string<SSIZE> g_cwd;

enum class CommandType
{
    CAT,    // read to file
    ECHO,   // write to a file
    LS,     // list contents of a directory
    RM,     // delete a file/dir
    CP,     // move and/or rename file
    CD,     // enter a directory
    CLEAR,  // clears terminal
    PWD,    // prints current working directory
    MKDIR,  // creates a new directory(or more)
    RMDIR,  // deletes a directory(or more)
    NONE
};

using CmdExec = SD_RES (*)(const etl::vector<etl::string<SSIZE>, ARGS_CAPACITY>&);

// defined in command_handler.cpp
extern etl::unordered_map<CommandType, CmdExec, COMMANDS_COUNT> cmd_table;

void handle_command(const etl::string<SSIZE>& str);
