# coding=utf-8

from socket import socket, AF_INET, SOCK_STREAM

server_port = 12000

server_socket = socket(AF_INET, SOCK_STREAM)
server_socket.bind(('', server_port))
server_socket.listen(1)

print("The server is ready to receive")

while True:
    conn_socket, addr = server_socket.accept()
    sentence = conn_socket.recv(1024)
    sentence = sentence.decode().upper()
    conn_socket.send(sentence.encode())
    conn_socket.close()
