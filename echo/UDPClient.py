#! coding=utf-8

import sys
from socket import socket, AF_INET, SOCK_DGRAM

if len(sys.argv) != 2:
    print("usage: %s server" % sys.argv[0], file=sys.stderr)
    sys.exit(1)

server_name = sys.argv[1]
server_port = 12000

message = input("Input lowercase sentence: ")

client_socket = socket(AF_INET, SOCK_DGRAM)
client_socket.sendto(message.encode(), (server_name, server_port))
message, server_addr = client_socket.recvfrom(2048)
client_socket.close()

print(message.decode())
