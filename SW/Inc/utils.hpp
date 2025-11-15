#pragma once

#include <cstdint>

#include "etl/string.h"
#include "etl/string_view.h"

constexpr uint8_t SSIZE = 255U;  // usual string size

enum SD_RES
{
    OK  = 0,
    ERR = 1,
};

/* File access mode and open method flags (3rd argument of f_open) */
#define FA_READ          0x01
#define FA_WRITE         0x02
#define FA_OPEN_EXISTING 0x00
#define FA_CREATE_NEW    0x04
#define FA_CREATE_ALWAYS 0x08
#define FA_OPEN_ALWAYS   0x10
#define FA_OPEN_APPEND   0x30

void die(etl::string_view msg);

bool is_double_quoted(etl::string_view s);
bool isescseq(char c);

size_t find_outside_quotes(etl::string_view s, char c, size_t start = 0,
                           size_t length = etl::string_view::npos);

// from string ""foo"" to string "foo"
etl::string<SSIZE> format_str(const etl::string<SSIZE>& s);

etl::string<SSIZE> unescape(etl::string_view s);
