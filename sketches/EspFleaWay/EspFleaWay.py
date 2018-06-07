#!/usr/bin/env python

import socket
import sys
import random
import time
import errno


TCP_IP = ''
TCP_PORT = int(sys.argv[1])
BUFFER_SIZE = 20  # Normally 1024, but we want fast response

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
count = 0
while 1:
  s.bind((TCP_IP, TCP_PORT))
  s.listen(1)

  while 1:
    conn, addr = s.accept()
    print 'Connection address:', addr
    while 1:
      try:
        pos = random.randint(0,60)
        r = random.randint(0,256)
        g = random.randint(0,255)
        b = random.randint(0,255)
        msg = "%02X" % pos + "%02X" % r + "%02X" % g + "%02X" % b + "\n"
        conn.send(msg)  # echo
        time.sleep(1)
      except socket.error as e:
        if e.errno != errno.ECONNRESET:
          raise
        else:
          print "client disconnect, await return\n"
          conn.close()
          break;

