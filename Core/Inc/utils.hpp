#pragma once

extern "C"
{
#include "ff.h"
#include "main.h"
}

#include "defs.hpp"
#include "etl/string.h"
#include "status.hpp"

namespace stm_sd
{

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
status fail(const string&);

size_t find_outside_quotes(
    const string&, char, size_t = 0,
    size_t length = string::npos);  // find function but works only outside quotes
string format_str(const string&);   //  ""foo""" -> "foo"
string unescape(const string&);     // test\\n -> test\n

PathData extract_path(const string&);

inline status      map_fresult(FRESULT);
inline const char* status_message(status);

inline bool is_double_quoted(const string&);
inline bool is_esc_seq(char);
inline bool is_flag(const string&);

};  // namespace stm_sd

/***********************************************************
 * Template/Inline Functions/Methods Declarations
 ***********************************************************/

inline bool stm_sd::is_double_quoted(const string& s)
{
    return s.size() >= 2 && s.front() == '\"' && s.back() == '\"';
}

inline bool stm_sd::is_esc_seq(char c)
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

bool stm_sd::is_flag(const string& s)
{
    return s.size() >= 2 && s[0] == '-' && s[1] != '-';
}

inline stm_sd::status stm_sd::map_fresult(FRESULT res)
{
    switch (res)
    {
        case FR_OK:
            return status::ok;
        case FR_DISK_ERR:
            return status::disk_err;
        case FR_INT_ERR:
            return status::int_err;
        case FR_NOT_READY:
            return status::not_ready;
        case FR_NO_FILE:
            return status::no_file;
        case FR_NO_PATH:
            return status::no_path;
        case FR_INVALID_NAME:
            return status::invalid_name;
        case FR_DENIED:
            return status::denied;
        case FR_EXIST:
            return status::exist;
        case FR_INVALID_OBJECT:
            return status::invalid_object;
        case FR_WRITE_PROTECTED:
            return status::write_protected;
        case FR_INVALID_DRIVE:
            return status::invalid_drive;
        case FR_NOT_ENABLED:
            return status::not_enabled;
        case FR_NO_FILESYSTEM:
            return status::no_filesystem;
        case FR_MKFS_ABORTED:
            return status::mkfs_aborted;
        case FR_TIMEOUT:
            return status::timeout;
        case FR_LOCKED:
            return status::locked;
        case FR_NOT_ENOUGH_CORE:
            return status::not_enough_core;
        case FR_TOO_MANY_OPEN_FILES:
            return status::too_many_open_files;
        case FR_INVALID_PARAMETER:
            return status::invalid_parameter;
        default:
            return status::err;  // fallback for unknown
    }
}

inline const char* stm_sd::status_message(status s)
{
    switch (s)
    {
        case status::ok:
            return "";
        case status::disk_err:
            return "disk error";
        case status::int_err:
            return "internal error";
        case status::not_ready:
            return "not ready";
        case status::no_file:
            return "file not found";
        case status::no_path:
            return "path not found";
        case status::invalid_name:
            return "invalid name";
        case status::denied:
            return "access denied";
        case status::exist:
            return "already exists";
        case status::invalid_object:
            return "invalid object";
        case status::write_protected:
            return "write protected";
        case status::invalid_drive:
            return "invalid drive";
        case status::not_enabled:
            return "volume not enabled";
        case status::no_filesystem:
            return "no filesystem";
        case status::mkfs_aborted:
            return "format aborted";
        case status::timeout:
            return "timeout";
        case status::locked:
            return "locked";
        case status::not_enough_core:
            return "not enough memory";
        case status::too_many_open_files:
            return "too many open files";
        case status::invalid_parameter:
            return "invalid parameter";
        default:
            return "unknown error";
    }
}
