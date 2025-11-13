#pragma once

extern "C"
{
#include "main.h"
}

#include "etl/string.h"

enum SD_RES
{
    OK  = 0,
    ERR = 1
};

void die(etl::string_view msg);
