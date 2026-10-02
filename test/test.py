import socket

port = input("Enter port:")
s = socket.socket()
s.connect(("127.0.0.1", 9122))
while True:
    data = input()
    s.sendall(data.encode())
    print(s.recv(4096).decode())   