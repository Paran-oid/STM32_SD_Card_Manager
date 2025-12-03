# STM32 SD Card Manager

![STM_SD](https://github.com/user-attachments/assets/13ecec31-3906-4e83-9bdc-bccfb3afa40e)


STM32 SD Card Manager is a program written in C++ for turning an STM32L476RG into a bridge for writing data from a PC to an SD Card through UART.

The commands are heavily inspired from the Linux CLI commands(cd, ls, echo, cat, ...)

Communication of the SD card and STM32 is established through SPI.

## Examples

Listing items inside directory path/dir1:

```bash
ls path/dir1
```

Changing into another directory:

```bash
cd path/dir1
```

## Usage

- cat
- cd
- clear
- pwd
- cp
- mkdir
- rmdir
- echo
- ls
- mv
- rm
- touch

## Installation

- Install the [STM32CubeIDE for VSCode](https://marketplace.visualstudio.com/items?itemName=stmicroelectronics.stm32-vscode-extension)
- Build the project
- Start debugging using STLink

## Contributing

Pull requests are welcome! Preferably the creation of an issue before-hand would be great too.

## License

[MIT](LICENSE)

## Acknowledgments

- [Embedded Template Library (ETL)](https://www.etlcpp.com/)
- [Lee Ji-hoon for ready to use SD Card Module code in FATFS/ folder](https://blog.naver.com/eziya76/221188701172)
- [eyalroz / printf](https://github.com/eyalroz/printf)
