# -*- coding: utf-8 -*-
"""
@file:      client.py
@author:    Yang Wu, Yifei Wang
@project:   CS 3251 Project 2 - Hangman Game
"""

import socket
import sys

# Client socket with message encode and decode,
# which derived from Socket Programming HOWTO
# https://docs.python.org/3.6/howto/sockets.html
class ClientSocket:
    def __init__(self, sock=None):
        if sock == None:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        else:
            self.sock = sock

    def connect(self, host, port):
        self.sock.connect((host, port))

    def close(self):
        self.sock.close()

    def send(self, data):
        msg_len = chr(len(data))
        msg = msg_len + data
        self.sock.send(msg.encode('utf-8'))

    def recv(self):
        msg = self.sock.recv(2048)
        msg = msg.decode('utf-8')
        msg_flag = ord(msg[0])
        server_msg = {'msg_flag': msg_flag}
        if msg_flag == 0:
            server_msg['word_len'] = ord(msg[1])
            server_msg['num_incorrect'] = ord(msg[2])
            server_msg['data'] = msg[3:]
        else:
            server_msg['data'] = msg[1:]
        return server_msg

def display(msg):
    word_len = msg['word_len']
    data = msg['data']
    first_line = '$ ' + ' '.join(data[:word_len])
    second_line = '$ Incorrect Guesses: ' + ' '.join(data[word_len:])
    print(first_line + '\n' + second_line)
    return set(data) - {'_'}

def guess(letter_set):
    letter = input('$ \n$ Letter to guess: ')
    while len(letter) >= 2 or not letter.isalpha():
        letter = input('$ Error! Pleas guess one letter.\n$ Letter to guess: ')
    while letter.lower() in letter_set:
        letter = input('$ Error! Leeter {} has been guessend before, pleas guess another letter.\n$ Letter to guess: '.format(letter))
    return letter.lower()

def run():
    num_args = len(sys.argv)
    if num_args < 3:
        print("Please specify the server IP and port number!")
        return

    # build the client socket and connect to the server
    server_ip   = sys.argv[1]
    server_port = int(sys.argv[2])
    sock = ClientSocket()
    sock.connect(server_ip, server_port)

    # ask user if they are ready to start the game
    is_ready = input('$ Ready to start game? (y/n): ')
    if is_ready == 'n': # if no, close socket and exit
        sock.close()
        return

    # game starts
    sock.send('')
    # keep sending and receiving message until win or lose
    while True:
        server_msg = sock.recv()
        if server_msg['msg_flag'] == 0:
            letter_set = display(server_msg)
            letter = guess(letter_set)
            sock.send(letter)
        else:
            print('$ ' + server_msg['data'])
            if server_msg['data'] == 'Game Over!':
                break
            sock.send('') # empty message as ACKs
    sock.close()

if __name__ == '__main__':
    run()