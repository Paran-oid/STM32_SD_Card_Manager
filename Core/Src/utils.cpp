#include "utils.hpp"

#include "printf.h"
#include "sd_filesystem.hpp"

namespace stm_sd
{

// \r\n passed after message
void die(const string& msg)
{
    printf_("%s\r\n", msg.c_str());
    Error_Handler();
}

Status fail(const string& msg)
{
    printf_("%s\r\n", msg.c_str());
    return Status::ERR;
}

size_t findOutsideQuotes(const string& s, char c, size_t start, size_t length)
{
    size_t end = length == string::npos ? s.size() : length - start;

    bool isInQuotes = false;
    for (size_t i = start; i < end && i < s.size(); i++)
    {
        char cur = s[i];
        if (cur == '\"' && (i == 0 || s[i - 1] != '\\'))
        {
            isInQuotes = !isInQuotes;
        }
        else if (cur == c && !isInQuotes)
        {
            return i;
        }
    }

    return string::npos;
}

string formatStr(const string& s)
{
    if (!isDoubleQuoted(s)) return s;
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

PathData extractPath(const string& p)
{
    size_t posSlash = p.find_last_of("/\\");
    if (posSlash == string::npos) return {"", p};

    return {p.substr(0, posSlash), p.substr(posSlash + 1)};
}

bool isFilename(const string& path)
{
    PathData pd = extractPath(path);
    if (pd.filename.empty()) return false;

    const std::string invalidChars = "/\\:*?\"<>|";
    for (char c : pd.filename)
    {
        size_t idx = pd.filename.find(c);
        if (idx == string::npos)
        {
            return false;
        }
    }
    return true;
}

bool isValidPath(const string& path)
{
    const string invalidChars =
        "\0/\"# "
        "$&'()*+,-./"
        ":;<=>?@[\\]^`{|}~"
        "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16"
        "\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F";

    for (char c : path)
    {
        size_t idx = path.find(c);
        if (idx == string::npos)
        {
            return false;
        }
    }
    return true;
}

}  // namespace stm_sd
