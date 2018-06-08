#!/usr/bin/env python
import spidev
import time

spi = spidev.SpiDev()
spi.open(0,1)
spi.max_speed_hz = 1000000
print spi.max_speed_hz
print spi.lsbfirst

val = 0

try:
  while True:
    buf = "%08X"%val+"\n";
    o = []
    for c in buf:
      o.append(ord(c))
    #print o
    resp = spi.writebytes(o)
    val += 1
    time.sleep(.0001)
except KeyboardInterrupt:
  spi.close
  
