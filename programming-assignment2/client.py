# -*- coding: utf-8 -*-
"""
@file:      client.py
@author:    Yang Wu, Yifei Wang
@project:   CS 3251 Project 2 - Hangman Game
"""

import socket
import sys

from threading import Thread

class Client:
    def __init__(self):
        pass

def main():
    num_args = len(sys.argv)
    if num_args < 3:
        # print("Please specify the server IP and port number!")
        sys.exit("Please specify the server IP and port number!")

    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        print("Socket successfully created")
    except socket.error as err:
        print("Socket creation failed with error {}".format(err))
        sys.exit()

    server_ip   = sys.argv[1]
    server_port = sys.argv[2]
    try:
        s.connect((server_ip, server_port))
    except socket.error as err:
        print("Socket connection failed with error {}".format(err))
        sys.exit()

    is_ready = input("$ Ready to start game? (y/n):")
    if is_ready == "n":
        s.close()
        sys.exit()

    # send an empty message to the server
    msg = ""
    raw = chr(len(msg)) + msg
    s.send(raw)

if __name__ == '__main__':
    main()