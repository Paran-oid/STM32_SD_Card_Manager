#include "tests.hpp"

void run_tests()
{
    TestResult res;

    res = test_sd_read_write();
    if (!res.passed) die(res.msg);

    log("all tests ran with success!\n");
}
