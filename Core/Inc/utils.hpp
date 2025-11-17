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

using estring = etl::string<SSIZE>;

void die(estring msg);

bool is_double_quoted(estring s);
bool is_esc_seq(char c);

size_t find_outside_quotes(estring s, char c, size_t start = 0, size_t length = estring::npos);

// from string ""foo""" to string "foo"
estring format_str(const estring& s);

// test\\n -> test\n
estring unescape(estring s);
