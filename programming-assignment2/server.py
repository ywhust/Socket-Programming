# -*- coding: utf-8 -*-
"""
@file:      server.py
@author:    Yang Wu, Yifei Wang
@project:   CS 3251 Project 2 - Hangman Game
"""

import socket
import thread
import sys

class Server:

    thread_count = 0

    def __init__(self):
        pass

def main():
    num_args = len(sys.argv)
    if num_args < 2:
        print("Please specify the port number!")
        exit(1)

    port = int(sys.argv[1])
    filename = "" # default word list
    if num_args == 3:
        filename = sys.argv[2]

if __name__ == '__main__':
    main()