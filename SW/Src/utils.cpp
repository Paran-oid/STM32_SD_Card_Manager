#include "utils.hpp"

void die(etl::string_view msg)
{
    printf("%s\r\n", msg.data());
    for (;;);
}

bool isstring(etl::string_view s)
{
    if (s.size() >= 2 && s.front() == '\"' && s.back() == '\"') return true;
    return false;
}

bool isescseq(char c)
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

size_t find_outside_quotes(etl::string_view s, char c, size_t start, size_t length)
{
    size_t end = length == etl::string_view::npos ? s.size() : length - start;

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

    return etl::string_view::npos;
}

etl::string<SSIZE> format_str(const etl::string<SSIZE>& s)
{
    if (!isstring(s)) return "";
    return s.substr(1, s.size() - 2);  // get rid of double quotes for the string
}

etl::string<SSIZE> unescape(etl::string_view s)
{
    etl::string<SSIZE> res;
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
