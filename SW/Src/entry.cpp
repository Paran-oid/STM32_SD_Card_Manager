
#include <iostream>

#include "command_handler.hpp"
#include "etl/string.h"
#include "utils.hpp"

#define TESTING_ 0

etl::string<SSIZE> sinput;       // pass to handle_command
char               tbuf[SSIZE];  // to use for input

void setup()
{
#if TESTING_
    run_tests();  // to configure tests modify run_tests in tests.cpp inside Tests folder
#endif

    printf("=======STM32 MICRO SD CARD READER READY!=======\r\n");
}

void loop()
{
    std::cin.getline(tbuf, SSIZE);
    sinput.assign(tbuf);
    sinput = unescape(sinput);
    handle_command(sinput);
}
