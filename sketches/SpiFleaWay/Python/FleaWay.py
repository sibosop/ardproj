#!/usr/bin/env python
import time
import Flea
import FleaLanes
import random

minFleaBirth = 0.08
maxFleaBirth = 0.1

minFleaInterval = 0.01
maxFleaInterval = .5

maxFleas = 25

FleaTable = {}

debug = False
FleaCount=0
def addFlea():
  global FleaCount
  global maxFleas
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
    if FleaCount % 25 == 0:
      maxFleas = random.randint(5,30)



if __name__ == '__main__':
  while True:
    addFlea()
    if debug: print "numFleas",len(FleaTable)
    wait = random.uniform(minFleaBirth,maxFleaBirth) 
    if debug: print "next flea:",wait
    time.sleep(wait)