#include "utils.hpp"

#include "printf.h"

namespace stm_sd
{

// \r\n passed after message
void die(const string& msg)
{
    printf("%s\r\n", msg.c_str());
    Error_Handler();
}

size_t find_outside_quotes(const string& s, char c, size_t start, size_t length)
{
    size_t end = length == string::npos ? s.size() : length - start;

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

    return string::npos;
}

string format_str(const string& s)
{
    if (!is_double_quoted(s)) return "";
    return s.substr(1, s.size() - 2);  // get rid of double quotes for the string
}

string unescape(const string& s)
{
    string res;
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

}  // namespace stm_sd
