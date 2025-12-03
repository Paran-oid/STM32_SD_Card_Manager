#include "tests.hpp"

#include "printf.h"
#include "utils.hpp"

void runTests()
{
    TestResult res;

    res = testReadWriteSD();
    if (!res.passed) stm_sd::die(res.msg);

    printf_("all tests ran with success!\r\n");
}
