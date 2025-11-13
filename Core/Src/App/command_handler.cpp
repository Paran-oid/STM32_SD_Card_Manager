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

static CommandType check_command_type(const etl::string<SSIZE>& item)
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

void handle_command(const etl::string<SSIZE>& str)
{
    etl::vector<etl::string<SSIZE>, ARGS_CAPACITY> args;
    etl::string<SSIZE>                             cmd;
    size_t                                         start = 0, end = 0;

    size_t pos_space = str.find(' ');
    if (pos_space != etl::string<SSIZE>::npos)
    {
        cmd   = str.substr(0, pos_space);
        start = end = pos_space + 1;
        while ((end = str.find(' ', start)) != etl::string<SSIZE>::npos)
        {
            if (args.size() == args.capacity()) die("too many arguments entered...");
            etl::string<SSIZE> item = str.substr(start, end - start);
            args.push_back(item);
            start = end + 1;
        }
        args.push_back(str.substr(start));
    }
    else
    {
        // if there are no spaces no need to go over the array
        cmd = str;
    }

    CommandType cmd_type = check_command_type(cmd);
    (void) cmd_type;
    auto it = cmd_table.find(cmd_type);
    if (it == cmd_table.end())
    {
        printf("command not found\r\n");
        return;
    }

    printf("\r\n");
    SD_RES res = it->second(args);
    if (res == SD_RES::OK) printf("\r\n");
    return;
}
