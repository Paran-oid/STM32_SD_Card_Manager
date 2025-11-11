#include "command_handler.hpp"

#include "utils.hpp"

// each of these is defined in their respective .cpp file (some may be shared inside
// common_cmds.cpp)
extern CmdExec cat_exec;
extern CmdExec echo_exec;
extern CmdExec ls_exec;
extern CmdExec rm_exec;
extern CmdExec cp_exec;
extern CmdExec cd_exec;

etl::unordered_map<CommandType, CmdExec, COMMANDS_COUNT> cmd_table = {
    {CommandType::CAT, cat_exec}, {CommandType::ECHO, echo_exec}, {CommandType::LS, ls_exec},
    {CommandType::RM, rm_exec},   {CommandType::CP, cp_exec},     {CommandType::CD, cd_exec}};

static CommandType check_command_type(const etl::string<ARGS_BUF_ITEM_SIZE>& item)
{
    if (item == "cat") return CommandType::CAT;
    if (item == "echo") return CommandType::ECHO;
    if (item == "ls") return CommandType::LS;
    if (item == "rm") return CommandType::RM;
    if (item == "cp") return CommandType::CP;
    if (item == "cd") return CommandType::CD;
    return CommandType::NONE;
}

SDR_RES handle_command(const etl::string<INPUT_BUF_SIZE>& input)
{
    etl::vector<etl::string<ARGS_BUF_ITEM_SIZE>, ARGS_BUF_CAPACITY> args;
    size_t                                                          start = 0, end = 0;

    // store arguments inside args vector
    while ((end = input.find(' ', start)) != etl::string<INPUT_BUF_SIZE>::npos)
    {
        etl::string<ARGS_BUF_ITEM_SIZE> item = input.substr(start, end);
        args.push_back(item);
        start = end + 1;
    }
    args.push_back(input.substr(start));  // add last element

    CommandType cmd_type = check_command_type(args[0]);
    auto        it       = cmd_table.find(cmd_type);
    if (it == cmd_table.end()) return SDR_RES::ERR;

    return it->second(args);
}
