#!/usr/bin/env python
import subprocess
import time

debug = False

def isMac():
  rval = False
  mach = subprocess.check_output(["uname"]).rstrip()
  if mach == "Darwin":
    rval = True
  return rval

if not isMac():
  if False:
    import spidev
    spi = spidev.SpiDev()
    spi.open(0,1)
    spi.max_speed_hz = 1000000
    print spi.max_speed_hz
    print spi.lsbfirst


buff = ""
def queueVal(v):
  global buff
  if debug: print "queueing",v
  buff += v
  
def flushIt():
  global buff
  buff += "\n";
  if isMac():
    if debug: print buff.rstrip()
  else:
    if False:
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
        spi.close()
  buff = ""
  
  
if __name__ == '__main__':
  print "isMac:",isMac()
  queueVal("82AFEF8F")
  queueVal("43")
  flushIt()
  queueVal("FFFFF")
  flushIt()
  
