#pragma once

extern "C"
{
#include "ff.h"
#include "main.h"
}

#include "etl/string.h"

namespace stm_sd
{

// inline constexpr to make sure constexpr elements are declared only once

inline constexpr uint16_t SSIZE      = 256U;
inline constexpr uint16_t BLOCK_SIZE = 512U;

using string = etl::string<SSIZE>;

enum class StatusCode
{
    OK  = 0,
    ERR = 1
};

void die(const string& msg);

size_t find_outside_quotes(
    const string& s, char c, size_t start = 0,
    size_t length = string::npos);   // find function but works only outside quotes
string format_str(const string& s);  //  ""foo""" -> "foo"
string unescape(const string& s);    // test\\n -> test\n

struct PathData
{
    string folder;
    string filename;
};
PathData extract_path(const string& p);

inline bool is_double_quoted(const string& s)
{
    return s.size() >= 2 && s.front() == '\"' && s.back() == '\"';
}

inline bool is_esc_seq(char c)
{
    switch (c)
    {
        case '\n':
        case '\r':
        case '\t':
        case '\v':
        case '\f':
        case '\b':
        case '\a':
        case '\\':
        case '\'':
        case '\"':
        case '\?':
        case '\0':
            return true;
        default:
            return false;
    }

    return false;
}

inline bool is_flag(const string& s)
{
    return s.size() >= 2 && s[0] == '-' && s[1] != '-';
}

};  // namespace stm_sd
