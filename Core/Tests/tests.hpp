#pragma once

#include "etl/string.h"
#include "utils.hpp"

//! WARNING: HAL FOR C++ MUST BE INITIALIZED BEFORE ANY TEST GETS EXECUTED!

struct TestResult
{
    bool    passed;
    estring msg;
};

void run_tests();

TestResult test_sd_read_write(bool display = false);
