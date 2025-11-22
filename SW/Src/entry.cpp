
#include <filesystem>
#include <iostream>

#include "command_handler.hpp"
#include "utils.hpp"

#define TESTING_ 0

namespace fs = std::filesystem;

stm_sd::string sinput;               // pass to handle_command
char           tbuf[stm_sd::SSIZE];  // to use for input

void setup()
{
#if TESTING_
    run_tests();  // to configure tests modify run_tests in tests.cpp inside Tests folder
#endif

    printf("=======STM32 MICRO SD CARD READER READY!=======\r\n");
    stm_sd::g_cwd.assign(fs::current_path().string().c_str());
}

void loop()
{
    std::cin.getline(tbuf, stm_sd::SSIZE);
    sinput.assign(tbuf);
    sinput = stm_sd::unescape(sinput.c_str());
    stm_sd::handle_command(sinput);
}
