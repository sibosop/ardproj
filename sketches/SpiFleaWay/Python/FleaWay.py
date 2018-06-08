#!/usr/bin/env python
import time
import Flea
import FleaLanes
import random

minFleaBirth = 1.0
maxFleaBirth = 5.0

minFleaInterval = 1.0
maxFleaInterval = 5.0

maxFleas = 5

FleaTable = {}

debug = True
FleaCount=0
def addFlea():
  global FleaCount
  deadFleas=[]
  for flea in FleaTable:
    if FleaTable[flea].dead():
      if debug: print flea,"is dead"
      deadFleas.append(flea)
  for d in deadFleas:
    del FleaTable[d]
      
  if len(FleaTable) < maxFleas:
    FleaCount += 1
    flea = Flea.Flea(str(FleaCount),random.uniform(minFleaInterval,maxFleaInterval))
    flea.setDaemon(True)
    FleaTable[flea.name]=flea
    FleaTable[flea.name].start()



if __name__ == '__main__':
  while True:
    addFlea()
    if debug: print "numFleas",len(FleaTable)
    if debug: FleaLanes.dump()
    wait = random.uniform(minFleaBirth,maxFleaBirth) 
    if debug: print "next flea:",wait
    time.sleep(wait)