#!/usr/bin/env python

import socket
import sys


TCP_IP = ''
TCP_PORT = int(sys.argv[1])
BUFFER_SIZE = 20  # Normally 1024, but we want fast response

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
while 1:
  s.bind((TCP_IP, TCP_PORT))
  s.listen(1)

  while 1:
    conn, addr = s.accept()
    print 'Connection address:', addr
    while 1:
      data = conn.recv(BUFFER_SIZE)
      if not data: break
      print "received data:", data
      conn.send(data)  # echo
    conn.close()
