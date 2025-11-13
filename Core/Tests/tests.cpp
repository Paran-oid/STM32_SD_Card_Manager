#include "tests.hpp"

#include "printf.h"
#include "utils.hpp"

void run_tests()
{
    TestResult res;

    res = test_sd_read_write();
    if (!res.passed) die(res.msg);

    printf("all tests ran with success!\n");
}
