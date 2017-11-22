# Project 2 Hangman Game

## Implementation

1. Encapsulate common Socket methods in our defined classes.
2. Only one packet will be sent at one time. It means the client or server need to wait for receiving message after sending one message.
3. The client will send empty message to respond the regular message packet (msg flag isn’t 0) from the server.
4. Using Python Thread module to build multiple connections at one time.

## Team member

Yang Wu (ywu613) and Yifei Wang (ywang3167).

For this project, both of us contribute the same. Yifei mainly works on client.py and Yang is responsible for server side.

## How to execute

The Python version is 3.6.0. First, use __python server.py 2017__ to set up the server. Then use __python client.py localhost 2017__ to set up client and start the game.