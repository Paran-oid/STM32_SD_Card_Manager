#pragma once

#include <etl/functional.h>
#include <etl/string.h>
#include <etl/unordered_map.h>
#include <etl/vector.h>

#include "utils.hpp"

constexpr uint16_t INPUT_BUF_SIZE = 256;  // input buf that gets passed to handle_command
constexpr uint16_t ARGS_BUF_CAPACITY =
    10;  // max number of items inside args buf inside the function
constexpr uint16_t ARGS_BUF_ITEM_SIZE = INPUT_BUF_SIZE / ARGS_BUF_CAPACITY;  // size of each string

const uint16_t COMMANDS_COUNT = 6;

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
    NONE
};

using CmdExec = SD_RES (*)(const etl::vector<etl::string<ARGS_BUF_ITEM_SIZE>, ARGS_BUF_CAPACITY>&);

// defined in command_handler.cpp
extern etl::unordered_map<CommandType, CmdExec, COMMANDS_COUNT> cmd_table;

SD_RES handle_command(const etl::string<INPUT_BUF_SIZE>& str);
