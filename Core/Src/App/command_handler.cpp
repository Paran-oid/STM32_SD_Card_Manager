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
extern cmd_exec send_exec;
extern cmd_exec receive_exec;

string g_cwd;  // exists just for SW

cmd_exec_map cmd_table = {{command_type::cat, cat_exec},     {command_type::echo, echo_exec},
                          {command_type::ls, ls_exec},       {command_type::rm, rm_exec},
                          {command_type::cp, cp_exec},       {command_type::cd, cd_exec},
                          {command_type::clear, clear_exec}, {command_type::pwd, pwd_exec},
                          {command_type::mkdir, mkdir_exec}, {command_type::rmdir, rmdir_exec},
                          {command_type::touch, touch_exec}, {command_type::mv, mv_exec},
                          {command_type::send, send_exec},   {command_type::receive, receive_exec}};

static command_type check_command_type(const string& item)
{
    static const std::unordered_map<std::string, command_type> command_map = {
        {"cat", command_type::cat},     {"echo", command_type::echo},
        {"ls", command_type::ls},       {"rm", command_type::rm},
        {"cp", command_type::cp},       {"cd", command_type::cd},
        {"clear", command_type::clear}, {"pwd", command_type::pwd},
        {"mkdir", command_type::mkdir}, {"rmdir", command_type::rmdir},
        {"touch", command_type::touch}, {"mv", command_type::mv},
        {"send", command_type::send},   {"receive", command_type::receive}};

    auto it = command_map.find(item.c_str());
    if (it != command_map.end())
    {
        return it->second;
    }
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
        printf_("command not found\r\n");
        return;
    }

    status      res = it->second(args);
    const char* msg = status_message(res);
    printf_("%s%s", msg, strcmp(msg, "") != 0 ? "\r\n" : "");  // add new line if msg is filled
}

}  // namespace stm_sd
