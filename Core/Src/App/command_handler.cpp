#include "command_handler.hpp"

#include "defs.hpp"
#include "status.hpp"
#include "utils.hpp"

namespace stm_sd
{

// each of these is defined in their respective .cpp file (some may be shared inside
// common_cmds.cpp)
extern cmd_exec cat_exec;
extern cmd_exec echo_exec;
extern cmd_exec ls_exec;
extern cmd_exec rm_exec;
extern cmd_exec cp_exec;
extern cmd_exec cd_exec;
extern cmd_exec clear_exec;
extern cmd_exec pwd_exec;
extern cmd_exec mkdir_exec;
extern cmd_exec rmdir_exec;
extern cmd_exec touch_exec;
extern cmd_exec mv_exec;
extern cmd_exec free_space_exec;
extern cmd_exec total_space_exec;

cmd_exec_map cmd_table = {{command_type::cat, cat_exec},
                          {command_type::echo, echo_exec},
                          {command_type::ls, ls_exec},
                          {command_type::rm, rm_exec},
                          {command_type::cp, cp_exec},
                          {command_type::cd, cd_exec},
                          {command_type::clear, clear_exec},
                          {command_type::pwd, pwd_exec},
                          {command_type::mkdir, mkdir_exec},
                          {command_type::rmdir, rmdir_exec},
                          {command_type::touch, touch_exec},
                          {command_type::mv, mv_exec},
                          {command_type::free_space, free_space_exec},
                          {command_type::total_space, total_space_exec}};

static command_type check_command_type(const string& item)
{
    if (item == "cat") return command_type::cat;
    if (item == "echo") return command_type::echo;
    if (item == "ls") return command_type::ls;
    if (item == "rm") return command_type::rm;
    if (item == "cp") return command_type::cp;
    if (item == "cd") return command_type::cd;
    if (item == "clear") return command_type::clear;
    if (item == "pwd") return command_type::pwd;
    if (item == "mkdir") return command_type::mkdir;
    if (item == "rmdir") return command_type::rmdir;
    if (item == "touch") return command_type::touch;
    if (item == "mv") return command_type::mv;
    if (item == "free_space") return command_type::free_space;
    if (item == "total_space") return command_type::total_space;
    return command_type::none;
}

void handle_command(const string& str)
{
    cmd_args args  = {};
    string   cmd   = "";
    size_t   start = 0, end = 0;

    size_t pos_space = find_outside_quotes(str, ' ');
    if (pos_space != string::npos)
    {
        cmd   = str.substr(0, pos_space);
        start = end = pos_space + 1;
        while ((end = find_outside_quotes(str, ' ', start)) != string::npos)
        {
            if (args.size() == args.capacity()) die("too many arguments entered...");
            string item = str.substr(start, end - start);
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

    command_type cmd_type = check_command_type(cmd);
    auto         it       = cmd_table.find(cmd_type);
    if (it == cmd_table.end())
    {
        printf("command not found\r\n");
        return;
    }

    status      res = it->second(args);
    const char* msg = status_message(res);
    printf("%s%s", msg, strcmp(msg, "") != 0 ? "\r\n" : "");
}

}  // namespace stm_sd
