# SSIZE = 256  # usual string size, must not be less than 256
# BLOCK_SIZE = 512  # number of bytes to extract when reading a file

CMD_HANDLER_ARGS_CAPACITY = 10  # max number of items passed inside input buffer

MAX_FILE_HANDLES = 16  # if modified, FS_LOCK must be modified too
MAX_LABEL_SIZE = 32  # specified in fatfs itself
MAX_DIR_SIZE = 256  # just like previously
PAGE_SIZE = 16  # can be customized

BYTES_PER_GB = 1024.0 * 1024.0 * 1024.0

OVERWRITE = (1 << 1)  # custom created OVERWRITE
RECURSIVE = (1 << 2)  # custom created RECURSIVE

DEFAULT_TIMEOUT = 100  # timeout in ms
BACKSPACE = 0x7F  # some consoles read backspace as 0x7F instead of '\b'
