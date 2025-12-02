TESTING = False


def main():

    # if TESTING:
    # runTests()

    print("=======SOFTWARE STM32 MICRO SD CARD READER READY!=======")

    while (True):
        s = input("")
        if s:
            s = unescape(s)
            handleCommand(s)


if __name__ == "__main__":
    main()
