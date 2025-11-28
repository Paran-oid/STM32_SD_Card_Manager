#pragma once

extern "C"
{
#include "ff.h"
#include "main.h"
}

#include <etl/string.h>

#include "defs.hpp"
#include "status.hpp"

namespace stm_sd
{

/***********************************************************
 * Forward Declarations
 ***********************************************************/

/***********************************************************
 * Public typedefs / structs
 ***********************************************************/
struct PathData
{
    string folder;
    string filename;
};

/***************************************************************
 * Function Declarations
 ***************************************************************/

void   die(const string&);
Status fail(const string&);

size_t findOutsideQuotes(
    const string&, char, size_t = 0,
    size_t length = string::npos);  // find function but works only outside quotes
string formatStr(const string&);    //  ""foo""" -> "foo"
string unescape(const string&);     // test\\n -> test\n

PathData extractPath(const string&);

inline Status      mapFRESULT(FRESULT);
inline const char* statusMessageMap(Status);

bool isFilename(const string&);

inline bool isDoubleQuoted(const string&);
inline bool isEscSeq(char);
inline bool isFlag(const string&);

};  // namespace stm_sd

/***********************************************************
 * Template/Inline Functions/Methods Declarations
 ***********************************************************/

inline bool stm_sd::isDoubleQuoted(const string& s)
{
    return s.size() >= 2 && s.front() == '\"' && s.back() == '\"';
}

inline bool stm_sd::isEscSeq(char c)
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

bool stm_sd::isFlag(const string& s)
{
    return s.size() >= 2 && s[0] == '-' && s[1] != '-';
}

inline stm_sd::Status stm_sd::mapFRESULT(FRESULT res)
{
    switch (res)
    {
        case FR_OK:
            return Status::OK;
        case FR_DISK_ERR:
            return Status::DISK_ERR;
        case FR_INT_ERR:
            return Status::INT_ERR;
        case FR_NOT_READY:
            return Status::NOT_READY;
        case FR_NO_FILE:
            return Status::NO_FILE;
        case FR_NO_PATH:
            return Status::NO_PATH;
        case FR_INVALID_NAME:
            return Status::INVALID_NAME;
        case FR_DENIED:
            return Status::DENIED;
        case FR_EXIST:
            return Status::EXIST;
        case FR_INVALID_OBJECT:
            return Status::INVALID_OBJECT;
        case FR_WRITE_PROTECTED:
            return Status::WRITE_PROTECTED;
        case FR_INVALID_DRIVE:
            return Status::INVALID_DRIVE;
        case FR_NOT_ENABLED:
            return Status::NOT_ENABLED;
        case FR_NO_FILESYSTEM:
            return Status::NO_FILESYSTEM;
        case FR_MKFS_ABORTED:
            return Status::MKFS_ABORTED;
        case FR_TIMEOUT:
            return Status::TIMEOUT;
        case FR_LOCKED:
            return Status::LOCKED;
        case FR_NOT_ENOUGH_CORE:
            return Status::NOT_ENOUGH_CORE;
        case FR_TOO_MANY_OPEN_FILES:
            return Status::TOO_MANY_OPEN_FILES;
        case FR_INVALID_PARAMETER:
            return Status::INVALID_PARAMETER;
        default:
            return Status::ERR;  // fallback for unknown
    }
}

inline const char* stm_sd::statusMessageMap(Status s)
{
    switch (s)
    {
        case Status::OK:
            return "";
        case Status::DISK_ERR:
            return "disk error";
        case Status::INT_ERR:
            return "internal error";
        case Status::NOT_READY:
            return "not ready";
        case Status::NO_FILE:
            return "file not found";
        case Status::NO_PATH:
            return "path not found";
        case Status::INVALID_NAME:
            return "invalid name";
        case Status::DENIED:
            return "access denied";
        case Status::EXIST:
            return "already exists";
        case Status::INVALID_OBJECT:
            return "invalid object";
        case Status::WRITE_PROTECTED:
            return "write protected";
        case Status::INVALID_DRIVE:
            return "invalid drive";
        case Status::NOT_ENABLED:
            return "volume not enabled";
        case Status::NO_FILESYSTEM:
            return "no filesystem";
        case Status::MKFS_ABORTED:
            return "format aborted";
        case Status::TIMEOUT:
            return "timeout";
        case Status::LOCKED:
            return "locked";
        case Status::NOT_ENOUGH_CORE:
            return "not enough memory";
        case Status::TOO_MANY_OPEN_FILES:
            return "too many open files";
        case Status::INVALID_PARAMETER:
            return "invalid parameter";
        default:
            return "";
    }
}
