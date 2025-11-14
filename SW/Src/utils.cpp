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
