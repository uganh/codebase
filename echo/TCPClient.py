# coding=utf-8

import sys
from socket import socket, AF_INET, SOCK_STREAM

if len(sys.argv) != 2:
    print("usage: %s server" % sys.argv[0], file=sys.stderr)
    sys.exit(1)

server_name = sys.argv[1]
server_port = 12000

sentence = input("Input lowercase sentence: ")

client_socket = socket(AF_INET, SOCK_STREAM)
client_socket.connect((server_name, server_port))
client_socket.send(sentence.encode())
sentence = client_socket.recv(1024)
client_socket.close()

print(sentence.decode())
