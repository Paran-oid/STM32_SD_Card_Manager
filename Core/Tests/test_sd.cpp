#include "hal_init.hpp"
#include "printf.h"
#include "tests.hpp"

TestResult test_sd_read_write(bool display)
{
    /*
    ?checks the following:
        *- sd card mounts and unmounts
        *- write, read, display (through params)
        *- file opens and closes
        *- seek works or not
    */

    if (sd_reader.mount()) return {false, "couldn't mount micro sd card"};

    etl::string<MAX_DIR_SIZE> path = "test.txt";

    SDFile* file = sd_reader.open_file(path, FA_READ | FA_WRITE);
    if (!file) return {false, "couldn't create file"};

    constexpr size_t  scap = 50;
    etl::string<scap> s;

    if (file->write("hello world!")) return {false, "couldn't write to file..."};
    if (file->seek(0)) return {false, "couldn't return to start of file"};
    if (file->read(s)) return {false, "couldn't read file"};

    if (display)
    {
        etl::array<etl::string_view, 5> msgs = {"content of ", path, "is:\r\n", s, "\r\n\r\n"};
        for (auto msg : msgs) printf("%s", msg.data());
    }

    if (sd_reader.close_file(file)) return {false, "close file failed"};
    if (sd_reader.unmount()) return {false, "unmount failed"};

    return {true, ""};
}
