#! /usr/bin/env python3

import random, sys
import argparse

def shuf(text):
    lines = text.readlines()
    random.shuffle(lines)
    return lines

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-e", "--echo", action="store_true", help="treat each ARG as an input line")
    parser.add_argument("-n", "--head-count", action="store", help="output at most COUNT lines")
    parser.add_argument("-r", "--repeat", action="store_true", help="output lines can be repeated")
    parser.add_argument("infile", nargs="?", type=argparse.FileType("r"), default=sys.stdin)
    args = parser.parse_args()

    lines = shuf(args.infile)
    print(len(lines))
    
    count = len(lines)
    if (args.head-count and count > head-count):
        count = head-count
    for i in range(count):
        print(lines[i].rstrip('\n'))

if __name__ == "__main__":
    main()
