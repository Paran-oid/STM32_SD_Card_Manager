#include "command_handler.hpp"

#include "utils.hpp"

/*
 * HOW TO ADD A FUNCTIONALITY:
 * - add extern declaration (and define it somewhere)
 * - add enum for it
 * - add it to the cmd_table unordered_map
 * - pass it in check_command_type function
 */

// each of these is defined in their respective .cpp file (some may be shared inside
// common_cmds.cpp)
extern CmdExec cat_exec;
extern CmdExec echo_exec;
extern CmdExec ls_exec;
extern CmdExec rm_exec;
extern CmdExec cp_exec;
extern CmdExec cd_exec;
extern CmdExec clear_exec;
extern CmdExec pwd_exec;

etl::unordered_map<CommandType, CmdExec, COMMANDS_COUNT> cmd_table = {
    {CommandType::CAT, cat_exec},     {CommandType::ECHO, echo_exec}, {CommandType::LS, ls_exec},
    {CommandType::RM, rm_exec},       {CommandType::CP, cp_exec},     {CommandType::CD, cd_exec},
    {CommandType::CLEAR, clear_exec}, {CommandType::PWD, pwd_exec}};

static CommandType check_command_type(const etl::string<ARGS_BUF_ITEM_SIZE>& item)
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

SD_RES handle_command(const etl::string<INPUT_BUF_SIZE>& str)
{
    etl::vector<etl::string<ARGS_BUF_ITEM_SIZE>, ARGS_BUF_CAPACITY> args;
    size_t                                                          start = 0, end = 0;

    // TODO: get cmd first instead of putting them all inside the vector and then getting rid of the
    // ;first arg

    // TODO: make sure you don't have more elements then capacity
    // store arguments inside args vector
    while ((end = str.find(' ', start)) != etl::string<INPUT_BUF_SIZE>::npos)
    {
        etl::string<ARGS_BUF_ITEM_SIZE> item = str.substr(start, end);
        args.push_back(item);
        start = end + 1;
    }
    args.push_back(str.substr(start));  // add last element

    CommandType cmd_type = check_command_type(args[0]);
    auto        it       = cmd_table.find(cmd_type);
    if (it == cmd_table.end()) return SD_RES::ERR;

    args.erase(args.begin());  // no need for cmd anymore
    return it->second(args);
}
