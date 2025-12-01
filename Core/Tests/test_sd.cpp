#include "filesystem.hpp"
#include "hal_init.hpp"
#include "printf.h"
#include "tests.hpp"

using namespace stm_sd;

namespace fs = stm_sd::filesystem;

TestResult testReadWriteSD(bool display)
{
    /*
    ?checks the following:
        *- sd card mounts and unmounts
        *- write, read, display (through params)
        *- file opens and closes
        *- seek works or not
    */

    if (fs::mount() != Status::OK) return {false, "couldn't mount micro sd card"};

    string path = "test.txt";

    File* file = fs::open(path, FREAD | FWRITE);
    if (!file) return {false, "couldn't create file"};

    string s = {};

    if (file->write("hello world!") != Status::OK) return {false, "couldn't write to file..."};
    if (file->seek(0) != Status::OK) return {false, "couldn't return to start of file"};
    if (file->read(s) <= 0) return {false, "couldn't read file"};

    if (display)
    {
        etl::array<string, 5> msgs = {"content of ", path, "is:\r\n", s, "\r\n\r\n"};
        for (auto msg : msgs) printf_("%s", msg.c_str());
    }

    if (fs::close(file) != Status::OK) return {false, "close file failed"};
    if (fs::unmount() != Status::OK) return {false, "unmount failed"};

    return {true, ""};
}
