# coding=utf-8

from socket import socket, AF_INET, SOCK_DGRAM

server_port = 12000

client_socket = socket(AF_INET, SOCK_DGRAM)
client_socket.bind(('', server_port))

print("The server is ready to receive")

while True:
    message, client_addr = client_socket.recvfrom(2048)
    message = message.decode().upper()
    client_socket.sendto(message.encode(), client_addr)
