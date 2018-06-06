#!/usr/bin/env python

import socket
import sys


TCP_IP = sys.argv[1]
TCP_PORT = int(sys.argv[2])
print "addr:",TCP_IP,"port:",TCP_PORT
BUFFER_SIZE = 1024
MESSAGE = "Hello, World!"

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
s.send(MESSAGE)
data = s.recv(BUFFER_SIZE)
s.close()

print "received data:", data
