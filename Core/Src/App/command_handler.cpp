#include "command_handler.hpp"

#include "utils.hpp"

/*
 * HOW TO ADD A FUNCTIONALITY:
 * - modify COMMANDS_COUNT (usually just increment as much as needed)
 * - add extern declaration for functions (and define it somewhere)
 * - add enum for it
 * - add it to the cmd_table unordered_map
 * - pass it in check_command_type function
 */

// each of these is defined in their respective .cpp file (some may be shared inside
// common_cmds.cpp)
// extern CmdExec cat_exec;
// extern CmdExec echo_exec;
// extern CmdExec ls_exec;
// extern CmdExec rm_exec;
// extern CmdExec cp_exec;
// extern CmdExec cd_exec;
// extern CmdExec clear_exec;
// extern CmdExec pwd_exec;

// etl::unordered_map<CommandType, CmdExec, COMMANDS_COUNT> cmd_table = {
//     {CommandType::CAT, cat_exec},     {CommandType::ECHO, echo_exec}, {CommandType::LS, ls_exec},
//     {CommandType::RM, rm_exec},       {CommandType::CP, cp_exec},     {CommandType::CD, cd_exec},
//     {CommandType::CLEAR, clear_exec}, {CommandType::PWD, pwd_exec}};

static CommandType check_command_type(const etl::string<ARGS_ITEM_SIZE>& item)
{
    if (item == "cat") return CommandType::CAT;
    if (item == "echo") return CommandType::ECHO;
    if (item == "ls") return CommandType::LS;
    if (item == "rm") return CommandType::RM;
    if (item == "cp") return CommandType::CP;
    if (item == "cd") return CommandType::CD;
    if (item == "clear") return CommandType::CLEAR;
    if (item == "pwd") return CommandType::PWD;
    return CommandType::NONE;
}

SD_RES handle_command(const etl::string<MAX_COMMAND_SIZE>& str)
{
    etl::vector<etl::string<ARGS_ITEM_SIZE>, ARGS_CAPACITY> args;
    etl::string<MAX_COMMAND_SIZE>                           cmd;
    size_t                                                  start = 0, end = 0;

    size_t pos_space = str.find(' ');
    if (pos_space != etl::string<MAX_COMMAND_SIZE>::npos)
    {
        cmd   = str.substr(0, pos_space);
        start = pos_space + 1;
        while ((end = str.find(' ', start)) != etl::string<MAX_COMMAND_SIZE>::npos)
        {
            if (args.size() == args.capacity()) die("too many arguments entered...");
            etl::string<ARGS_ITEM_SIZE> item = str.substr(start, end);
            args.push_back(item);
            start = end + 1;
        }
        args.push_back(str.substr(start));  // add last element
    }
    else
    {
        // if there are no spaces no need to go over the array
        cmd = str;
    }

    CommandType cmd_type = check_command_type(cmd);
    (void) cmd_type;
    // auto        it       = cmd_table.find(cmd_type);
    // if (it == cmd_table.end()) return SD_RES::ERR;

    // return it->second(args);
    return SD_RES::OK;
}
