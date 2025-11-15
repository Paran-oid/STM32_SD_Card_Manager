#pragma once

extern "C"
{
#include "main.h"
}

#include "etl/string.h"

constexpr uint8_t  SSIZE      = 255U;  // usual string size
constexpr uint16_t BLOCK_SIZE = 512U;

enum SD_RES
{
    OK  = 0,
    ERR = 1
};

void die(etl::string_view msg);

bool is_double_quoted(etl::string_view s);
bool is_esc_seq(char c);

size_t find_outside_quotes(etl::string_view s, char c, size_t start = 0,
                           size_t length = etl::string_view::npos);

// from string ""foo""" to string "foo"
etl::string<SSIZE> format_str(const etl::string<SSIZE>& s);

// test\\n -> test\n
etl::string<SSIZE> unescape(etl::string_view s);
