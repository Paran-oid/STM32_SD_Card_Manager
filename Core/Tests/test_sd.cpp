#include "hal_init.hpp"
#include "tests.hpp"

TestResult test_sd_read_write(bool write, bool read, bool display)
{
    if (sd_reader.mount()) return {false, "couldn't mount micro sd card"};

    etl::string_view path = "test.txt";

    SDFile* file = sd_reader.open_file(path, FA_READ | FA_WRITE);
    if (!file) return {false, "couldn't create file"};

    constexpr size_t  scap = 50;
    etl::string<scap> s;

    if (write)
    {
        if (file->write("hello world!")) return {false, "couldn't write to file..."};
        if (file->seek(0)) return {false, "couldn't return to start of file"};
    }

    if (read)
    {
        if (file->read(s)) return {false, "couldn't read file"};
    }

    if (display)
    {
        etl::array<etl::string_view, 5> msgs = {"content of ", path, "is:\r\n", s, "\r\n\r\n"};
        for (auto msg : msgs) log(msg);
    }

    if (sd_reader.close_file(file->fil())) return {false, "close file failed"};
    if (sd_reader.unmount()) return {false, "unmount failed"};

    return {true, ""};
}
