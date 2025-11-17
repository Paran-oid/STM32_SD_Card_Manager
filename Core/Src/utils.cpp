#include "utils.hpp"

#include "printf.h"

void die(estring msg)
{
    etl::string<255> sdsad;
    printf("%s\r\n", msg.c_str());
    Error_Handler();
}

bool is_double_quoted(estring s)
{
    if (s.size() >= 2 && s.front() == '\"' && s.back() == '\"') return true;
    return false;
}

bool is_esc_seq(char c)
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

size_t find_outside_quotes(estring s, char c, size_t start, size_t length)
{
    size_t end = length == estring::npos ? s.size() : length - start;

    bool in_quotes = false;
    for (size_t i = start; i < end && i < s.size(); i++)
    {
        char cur = s[i];
        if (cur == '\"' && (i == 0 || s[i - 1] != '\\'))
        {
            in_quotes = !in_quotes;
        }
        else if (cur == c && !in_quotes)
        {
            return i;
        }
    }

    return estring::npos;
}

estring format_str(const estring& s)
{
    if (!is_double_quoted(s)) return "";
    return s.substr(1, s.size() - 2);  // get rid of double quotes for the string
}

estring unescape(estring s)
{
    estring res;
    for (uint8_t i = 0; i < s.size(); i++)
    {
        char c = s[i];
        if (s[i] == '\\' && (i + 1) < static_cast<int>(s.size()))
        {
            switch (s[i + 1])
            {
                case 'n':
                    c = '\n';
                    break;
                case 'r':
                    c = '\r';
                    break;
                case 't':
                    c = '\t';
                    break;
                case 'v':
                    c = '\v';
                    break;
                case 'f':
                    c = '\f';
                    break;
                case 'b':
                    c = '\b';
                    break;
                case 'a':
                    c = '\a';
                    break;
                case '\\':
                    c = '\\';
                    break;
                case '\'':
                    c = '\'';
                    break;
                case '\"':
                    c = '\"';
                    break;
                case '?':
                    c = '\?';
                    break;
                case '0':
                    c = '\0';
                    break;
                default:
                    c = '\\';
                    break;
            }
            i++;
        }
        res += c;
    }
    return res;
}
