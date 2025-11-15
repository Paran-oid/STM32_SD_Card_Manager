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

void die(etl::string<SSIZE> msg);

bool is_double_quoted(etl::string<SSIZE> s);
bool is_esc_seq(char c);

size_t find_outside_quotes(etl::string<SSIZE> s, char c, size_t start = 0,
                           size_t length = etl::string<SSIZE>::npos);

// from string ""foo""" to string "foo"
etl::string<SSIZE> format_str(const etl::string<SSIZE>& s);

// test\\n -> test\n
etl::string<SSIZE> unescape(etl::string<SSIZE> s);
