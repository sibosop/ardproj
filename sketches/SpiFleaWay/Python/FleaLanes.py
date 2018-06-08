#!/usr/bin/env python
import threading
import Flea

LaneMutex = threading.Lock()

MaxLanes = 4
MaxSlots = 5
LaneTable = []




class Place:
  def __init__(self,l,p):
    self.flea = None
    self.pos = {}
    self.pos['lane'] = l
    self.pos['slot'] = p
    
  def empty(self):
    return self.flea == None
  
  def lane(self):
    return self.pos['lane']
  
  def slot(self):
    return self.pos['slot']
    
  def clear(self):
    self.flea = None
    
  def show(self):
    if self.flea != None:
      return self.flea.name.ljust(8)
    return "-".ljust(8)
   
  def __str__(self):
    return str(self.lane())+":"+str(self.slot()) 
    
def setPlace(flea,place):
  if flea.place != None:
    flea.place.flea = None
  flea.place = place
  place.flea = flea
    
    
def findPlace(flea):
  if flea.place == None:
    for i in range(MaxLanes-1,-1,-1):
      if LaneTable[i][0].empty():
        setPlace(flea,LaneTable[i][0])
        return False
    return False
  
  lane = flea.place.lane()
  slot = flea.place.slot()
  nextSlot = slot + 1
  # look ahead straight
  if nextSlot == MaxSlots:
    LaneTable[lane][slot].clear()
    return True
    
  if LaneTable[lane][nextSlot].empty():
    setPlace(flea,LaneTable[lane][nextSlot])
    return False
          
  # then left
  if lane != 0:
    if LaneTable[lane-1][nextSlot].empty():
      setPlace(flea,LaneTable[lane-1][nextSlot])
      return False  
  # then right
  if (lane+1) != MaxLanes:
    if LaneTable[lane+1][nextSlot].empty():
      setPlace(flea,LaneTable[lane+1][nextSlot])
      return False
  return False
  
def dump():
  for lane in LaneTable:
    d = ""
    for place in lane:
      d += place.show() + " "
    print d    
    
for lane in range(MaxLanes):
  LaneTable.append([])
  for slot in range(MaxSlots):
    LaneTable[-1].append(Place(lane,slot))
  


    