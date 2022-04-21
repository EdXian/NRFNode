import socket
import time
import struct
HOST = '127.0.0.1'
PORT = 6000

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

while True:
    indata = s.recv(1024)
    indata=indata[0:6]
    print(len(indata))
    accx, accy, accz = struct.unpack("<hhh", indata)
    print(str(accx)+ ' ' + str(accy) + ' ' +str(accz))
    
