#!/usr/bin/env python
import time
import Flea
import FleaLanes
import random
import sys

minFleaBirth = 0.08
maxFleaBirth = 0.1

minFleaInterval = 0.01
maxFleaInterval = .2

maxFleas = 25

FleaTable = {}

debug = False
FleaCount=0
countMod = 25
def addFlea():
  global FleaCount
  global maxFleas
  global countMod
  deadFleas=[]
  for flea in FleaTable:
    if FleaTable[flea].dead():
      if debug: print flea,"is dead"
      deadFleas.append(flea)
  for d in deadFleas:
    del FleaTable[d]
      
  if len(FleaTable) < maxFleas:
    FleaCount += 1
    if FleaCount == 1000:
      FleaCount = 0
    flea = Flea.Flea(str(FleaCount),random.uniform(minFleaInterval,maxFleaInterval))
    flea.setDaemon(True)
    FleaTable[flea.name]=flea
    FleaTable[flea.name].start()
    if FleaCount % countMod == 0:
      maxFleas = random.randint(5,30)
      countMod = random.randint(5,45)
      



if __name__ == '__main__':
  if len(sys.argv) > 1:
    if sys.argv[1] == '-d':
      FleaLanes.dumpIt = True
  while True:
    addFlea()
    if debug: print "numFleas",len(FleaTable)
    wait = random.uniform(minFleaBirth,maxFleaBirth) 
    if debug: print "next flea:",wait
    time.sleep(wait)