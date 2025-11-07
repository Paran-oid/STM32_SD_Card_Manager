#pragma once

#include <etl/string_view.h>

#include "utils.hpp"

//! WARNING: HAL FOR C++ MUST BE INITIALIZED BEFORE ANY TEST GETS EXECUTED!

struct TestResult
{
    bool             passed;
    etl::string_view msg;
};

void run_tests();

TestResult test_sd_read_write(bool write = true, bool read = true, bool display = true);
