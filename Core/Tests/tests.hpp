#pragma once

#include <etl/string.h>

#include "utils.hpp"

//! WARNING: HAL FOR C++ MUST BE INITIALIZED BEFORE ANY TEST GETS EXECUTED!

struct TestResult
{
    bool           passed;
    stm_sd::string msg;
};

void runTests();

TestResult testReadWriteSD(bool display = false);
