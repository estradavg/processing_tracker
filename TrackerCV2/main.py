import socket
import random

HOST = '127.0.0.1'                     # Symbolic name meaning all available interfaces
PORT = 12000              # Arbitrary non-privileged port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

while True:
    #s.send("test\n".encode());
    data = random.random()        #simulated data
    data = (str(data) + "\n").encode()
    s.send(data)