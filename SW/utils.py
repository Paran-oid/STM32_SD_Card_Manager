from status import Status


def die(s):
    print(s)


def fail(s):
    print(s)
    return Status.ERR


def findOutsideQuotes(str, c, start, length):
    end = length
