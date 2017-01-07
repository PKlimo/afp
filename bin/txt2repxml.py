#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from __future__ import print_function
import sys
import re


def parseTLE(line):
    p = re.compile(r'''TLE \s*   ItemName=Kuvert \s* ItemValue=Group_
            (?P<HSS>.)
            Envelope\(
            (?P<Sparte>.*?)
            \s
            (?P<Envelope>.*?)
            \)Letter\(
            (?P<Letter>.*?)
            \).*?''', re.VERBOSE)
    m = p.search(line)
    if m:
        return (m.group('HSS'), m.group('Sparte'), m.group('Envelope'), m.group('Letter'))
    else:
        return None

def parseIMM(line):
    if line.startswith('IMM       '):
        return line[10:].strip()
    else:
        return None

def parseBPG(line):
    if line.startswith('BPG       '):
        return True
    else:
        return False

def parseBMM(line):
    if line.startswith('BMM       '):
        return line[10:].strip()
    else:
        return None

def parseMMC(line):
    for l in line.split(" "):
        if l.startswith("TrayNum="):
            return l[8:].strip()
    return None

def main():
    last_enveope = None
    last_page = None
    last_tray = None
    print('<File>')
    for l in sys.stdin.readlines():
        e = parseBMM(l)
        if e is not None:
            last_tray = e
        e = parseMMC(l)
        if e is not None:
            media2tray[last_tray] = e
        e = parseTLE(l)
        if e is not None:
            if e[2] != last_enveope:
                if last_enveope is not None:
                    print('</Letter>')
                    print('</Envelope>')
                print('<Envelope id="{}" hss="{}">'.format(e[2], e[0]))
                last_enveope = e[2]
            else:
                print('</Letter>')
            print('<Letter id="{}">'.format(e[3]))
        e = parseIMM(l)
        if e is not None:
            last_page = e
            last_tray = media2tray[last_page]
        if parseBPG(l):
            print('<page form="{}" tray="{}"/>'.format(last_page, last_tray))

    print('</Letter>')
    print('</Envelope>')
    print('</File>')

media2tray = dict()
if __name__ == "__main__":
    main()
