#! /usr/bin/env python3

import random, sys, argparse

class shuf:
    def __init__(self, lines):
        self.lines = lines

    def shuf(self, head_count=""):
        random.shuffle(self.lines)
        for i in range(len(self.lines)):
            print(self.lines[i])

    def repeat_shuf(self, head_count=""):
        if head_count:
            for _ in range(int(head_count)):
                print(random.choice(self.lines))
        else:
            while True:
                print(random.choice(self.lines))

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-e", "--echo", action="store_true", help="treat each ARG as an input line")
    parser.add_argument("-n", "--head-count", action="store", help="output at most COUNT lines")
    parser.add_argument("-r", "--repeat", action="store_true", help="output lines can be repeated")
    parser.add_argument("input", nargs="*")
    args = parser.parse_args()

    lines = []
    if args.input or args.echo:
        if args.echo:
            # treat each arg as an input line
            lines = args.input
        elif len(args.input) == 1:
            # open file
            with open(args.input[0]) as infile:
                for line in infile.readlines():
                    lines.append(line.rstrip('\n'))
        else:
            # input error
            sys.exit("Too many arguments")
    else:
        # empty input, read stdin
        for line in sys.stdin:
            lines.append(line.rstrip('\n'))

    s = shuf(lines)
    if args.repeat:
        s.repeat_shuf(args.head_count)
    else:
        s.shuf(args.head_count)

if __name__ == "__main__":
    main()
