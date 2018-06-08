#!/usr/bin/env python
import time
import threading
import random
import FleaLanes

debug = True
class Flea(threading.Thread):
  def __init__(self,name,interval):
    super(Flea,self).__init__()
    self.isDead = False
    self.name = "Flea"+name
    self.place = None
    self.interval = interval
    self.count=0
  def dead(self):
    return self.isDead
    
  def kill(self):
    if debug: print self.name,"killed"
    self.isDead = True
    
  def move(self):
    if FleaLanes.findPlace(self):
      self.kill()
    else:
      if debug: print self.name,"moving to:",str(self.place)
      
  def run(self):
    print("Starting Flea:"+self.name)
    while not self.dead():
      self.move()
      time.sleep(self.interval)
      
    print("Dead Flea:"+self.name)
        
      