#pragma once

#include "etl/functional.h"
#include "etl/string.h"
#include "etl/unordered_map.h"
#include "etl/vector.h"
#include "utils.hpp"

<<<<<<< HEAD
constexpr uint16_t ARGS_CAPACITY  = 10;  // max number of items inside args buf inside the function
=======
constexpr uint16_t MAX_COMMAND_SIZE = 255;  // input buf that gets passed to handle_command
constexpr uint16_t ARGS_CAPACITY  = 10;  // max number of items inside args buf inside the function
constexpr uint16_t ARGS_ITEM_SIZE = MAX_COMMAND_SIZE / ARGS_CAPACITY;  // size of each string
>>>>>>> dc6478d (init)

const uint16_t COMMANDS_COUNT = 8;

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

<<<<<<< HEAD
using CmdExec = SD_RES (*)(const etl::vector<etl::string<SSIZE>, ARGS_CAPACITY>&);
=======
using CmdExec = SD_RES (*)(const etl::vector<etl::string<ARGS_ITEM_SIZE>, ARGS_CAPACITY>&);
>>>>>>> dc6478d (init)

// defined in command_handler.cpp
extern etl::unordered_map<CommandType, CmdExec, COMMANDS_COUNT> cmd_table;

<<<<<<< HEAD
void handle_command(const etl::string<SSIZE>& str);
=======
SD_RES handle_command(const etl::string<MAX_COMMAND_SIZE>& str);
>>>>>>> dc6478d (init)
