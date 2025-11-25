#include "tests.hpp"

#include "printf.h"
#include "utils.hpp"

void run_tests()
{
    TestResult res;

    res = test_sd_read_write();
    if (!res.passed) stm_sd::die(res.msg);

    printf_("all tests ran with success!\r\n");
}
