#!/usr/bin/python

"""
comm -- compare two sorted files line by line
Implementation in python
"""

import random, sys, locale
from optparse import OptionParser

class line:
    def __init__(self, filename):
        if filename == '-':
            f = sys.stdin
        else:
            f = open (filename, 'r')
        self.lines = f.readlines()
        self.length = len(self.lines)
        f.close()
    

def main():
    locale.setlocale(locale.LC_ALL, 'C')
    version_msg = "%prog 1.0"
    usage_msg = """%prog [OPTION]... FILE1 FILE2

Compare sorted files FILE1 and FILE2 line by line."""

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)

    parser.add_option("-1",
                  action="store_false", dest="colOne", default=True,
                  help="Suppress printing of column 1.")

    parser.add_option("-2",
                  action="store_false", dest="colTwo", default=True,
                  help="Suppress printing of column 2.")

    parser.add_option("-3",
                  action="store_false", dest="colThree", default=True,
                  help="Suppress printing of column 3.")
    
    parser.add_option("-u",
                 action="store_false", dest="colU", default=True,
                 help="Sort unsorted files.")

    options, args = parser.parse_args()

    if len(args) != 2:
        parser.error("wrong number of operands")

    tabOne = ''
    tabTwo = ''

    if options.colOne:
        tabOne = '\t'
    if options.colTwo:
        tabTwo = '\t'
        
    input_file_one = args[0]
    input_file_two = args[1]

    file_one = line(input_file_one)
    file_two = line(input_file_two)

    content_one = []
    content_two = []
    
    for item in file_one.lines:
	    content_one.append(item)
    for item in file_two.lines:
            content_two.append(item)

    countOne = 0
    countTwo = 0

    common = []

    while (countOne != len(content_one) or countTwo != len(content_two)) and options.colU:
         fileOne = True
         fileTwo = True
         if countOne != len(content_one) and countTwo != len(content_two):
             if content_one[countOne] > content_two[countTwo]:
                 fileOne = False
             else:
                 fileTwo = False
         if countOne != len(content_one) and fileOne:
             noMatch = True
             for twoLine in content_two:
                  if content_one[countOne] == twoLine:
                      noMatch = False
                      content_two.remove(twoLine)
             if noMatch and options.colOne and content_one[countOne] != '\n':
                 sys.stdout.write(content_one[countOne])
             if not noMatch and options.colThree and content_one[countOne] != '\n':
                sys.stdout.write(tabOne + tabTwo + content_one[countOne])
             countOne += 1
         if countTwo != len(content_two) and fileTwo:
             noMatch = True
             for oneLine in content_one:
                 if content_two[countTwo] == oneLine:
                     noMatch = False
                     content_one.remove(oneLine)
             if noMatch and options.colTwo and content_two[countTwo] != '\n':
                 sys.stdout.write(tabOne + content_two[countTwo])
             if not noMatch and options.colThree and content_two[countTwo] != '\n':
                sys.stdout.write(tabOne + tabTwo + content_two[countTwo])
             countTwo += 1
    while countOne != len(content_one) and not options.colU:
        noMatch = True
        for twoLine in content_two:
            if content_one[countOne] == twoLine:
                noMatch = False
                content_two.remove(twoLine)
        if noMatch and options.colOne and content_one[countOne] != '\n':
            sys.stdout.write(content_one[countOne])
        if not noMatch and options.colThreee and content_one[countOne] != '\n':
            sys.stdout.write(tabOne + tabTwo + content_one[countOne])
        countOne += 1
    while countTwo != len(content_two) and not options.colU and content_two[countTwo] != '\n':
        sys.stdout.write(tabOne + content_two[countTwo])
        countTwo += 1
    
    

if __name__ == "__main__":
    main()
