# -*- coding: utf-8 -*-
"""
@file:      server.py
@author:    Yang Wu, Yifei Wang
@project:   CS 3251 Project 2 - Hangman Game
"""

import socket
import sys

from threading import Thread

class ClientThread(Thread):
    def __init__(self, sock, nums):
        Thread.__init__(self)
        self.sock = sock
        self.nums = nums
        print('[+] New thread for this connection.')

    def run(self):
        start(self.sock, self.nums)

# Client socket with message encode and decode on the server side
class ClientSocket:

    message = {'win': 'You Win!', 'lose': 'You Lose :(', 'over': 'Game Over!', 'overload': 'server-overloaded'}
    count = 0;

    def __init__(self, sock, word):
        self.sock = sock
        self.word = list(word)
        self.data = ['_'] * len(word)
        self.word_len = len(word)
        self.num_incorrect = 0
        ClientSocket.count += 1

    def send(self, msg):
        self.sock.send(msg.encode('utf-8'))

    def recv(self):
        msg = self.sock.recv(2048)
        msg = msg.decode('utf-8')
        msg_len = ord(msg[0])
        return '' if msg_len == 0 else msg[1:1+msg_len]

    # generate game control packet
    def control_pckt(self, letter, is_correct=True):
        if not is_correct:
            self.data.append(letter)
            self.num_incorrect += 1
        data = ''.join(self.data)
        msg = chr(0) + chr(self.word_len) + chr(self.num_incorrect) + data
        return msg

    # if correct, unmask the letter and return true
    # else return false
    def match(self, letter):
        is_match = False
        for i in range(self.word_len):
            if self.word[i] == letter:
                self.data[i] = letter
                is_match = True
        return is_match

    # generate message packet
    def message_pckt(self, key):
        msg = ClientSocket.message[key]
        return chr(len(msg)) + msg

    # return true if guess the correct word
    def check(self):
        return self.word == self.data[:self.word_len]

    def close(self):
        self.sock.close()
        ClientSocket.count -= 1

# start hangman game for specific client
def start(sock, nums):
    # wait for start request
    letter = sock.recv()

    # check if this is the fourth client
    if ClientSocket.count > 3:
        overload_msg = sock.message_pckt('overload')
        sock.send(overload_msg)
        sock.close()
        return

    # check the answer and ask for next letter
    while sock.num_incorrect < nums:
        is_correct = sock.match(letter)
        if sock.check():
            win_msg = sock.message_pckt('win')
            sock.send(win_msg)
            break
        control_msg = sock.control_pckt(letter, is_correct)
        sock.send(control_msg)
        letter = sock.recv()

    # game over
    if sock.num_incorrect == nums:
        lose_msg = sock.message_pckt('lose')
        sock.send(lose_msg)
    letter = sock.recv()
    over_msg = sock.message_pckt('over')
    sock.send(over_msg)
    sock.close()

def main():
    num_args = len(sys.argv)
    if num_args < 2:
        print("Please specify the port number!")
        exit(1)

    server_port = int(sys.argv[1])
    filename = "" # default word list
    if num_args == 3:
        filename = sys.argv[2]

    server_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_sock.bind(('localhost', server_port))
    server_sock.listen()
    print('Waiting for connection...')

    while True:
        conn, addr = server_sock.accept()
        print('Got connection from {}'.format(addr))
        client_socket = ClientSocket(conn, 'word')
        thread = ClientThread(client_socket, 6)
        thread.start()


if __name__ == '__main__':
    main()