#include "command_handler.hpp"

#include "defs.hpp"
#include "status.hpp"
#include "utils.hpp"

namespace stm_sd
{

// each of these is defined in their respective .cpp file (some may be shared inside
// common_cmds.cpp)
extern CmdExec catExec;
extern CmdExec echoExec;
extern CmdExec lsExec;
extern CmdExec rmExec;
extern CmdExec cpExec;
extern CmdExec cdExec;
extern CmdExec clearExec;
extern CmdExec pwdExec;
extern CmdExec mkdirExec;
extern CmdExec rmdirExec;
extern CmdExec touchExec;
extern CmdExec mvExec;
extern CmdExec sendExec;
extern CmdExec receiveExec;
CmdExecMap     cmdTable = {{CommandType::CAT, catExec},     {CommandType::ECHO, echoExec},
                           {CommandType::LS, lsExec},       {CommandType::RM, rmExec},
                           {CommandType::CP, cpExec},       {CommandType::CD, cdExec},
                           {CommandType::CLEAR, clearExec}, {CommandType::PWD, pwdExec},
                           {CommandType::MKDIR, mkdirExec}, {CommandType::RMDIR, rmdirExec},
                           {CommandType::TOUCH, touchExec}, {CommandType::MV, mvExec},
                           {CommandType::SEND, sendExec},   {CommandType::RECEIVE, receiveExec}};

static CommandType check_command_type(const string& item)
{
    static const std::unordered_map<std::string, CommandType> commandMap = {
        {"CAT", CommandType::CAT},     {"ECHO", CommandType::ECHO},
        {"LS", CommandType::LS},       {"RM", CommandType::RM},
        {"CP", CommandType::CP},       {"CD", CommandType::CD},
        {"CLEAR", CommandType::CLEAR}, {"PWD", CommandType::PWD},
        {"MKDIR", CommandType::MKDIR}, {"RMDIR", CommandType::RMDIR},
        {"TOUCH", CommandType::TOUCH}, {"MV", CommandType::MV},
        {"SEND", CommandType::SEND},   {"RECEIVE", CommandType::RECEIVE}};

    auto it = commandMap.find(item.c_str());
    if (it != commandMap.end())
    {
        return it->second;
    }
    return CommandType::NONE;
}

void handleCommand(const string& str)
{
    CmdArgs args  = {};
    string  cmd   = "";
    size_t  start = 0, end = 0;

    size_t posSpace = findOutsideQuotes(str, ' ');
    if (posSpace != string::npos)
    {
        cmd   = str.substr(0, posSpace);
        start = end = posSpace + 1;
        while ((end = findOutsideQuotes(str, ' ', start)) != string::npos)
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

    CommandType cmdType = check_command_type(cmd);
    auto        it      = cmdTable.find(cmdType);
    if (it == cmdTable.end())
    {
        printf_("command not found\r\n");
        return;
    }

    Status      res = it->second(args);
    const char* msg = statusMessageMap(res);
    printf_("%s%s", msg, strcmp(msg, "") != 0 ? "\r\n" : "");  // add new line if msg is filled
}

}  // namespace stm_sd
