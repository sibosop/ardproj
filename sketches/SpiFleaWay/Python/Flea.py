#!/usr/bin/env python
import time
import threading
import random
import FleaLanes

debug = False
class Flea(threading.Thread):
  def __init__(self,name,interval):
    super(Flea,self).__init__()
    self.isDead = False
    self.number = name
    self.name = "Flea"+name
    self.place = None
    self.interval = interval
    self.count=0
    self.color = {'r' : random.randint(0,255), 'g' : random.randint(0,255), 'b' : random.randint(0,255)}
    
  def __str__(self):
    return "%02X" % self.color['r'] \
      + "%02X" % self.color['g'] \
      + "%02X" % self.color['b']
    
  def dead(self):
    return self.isDead
    
  def kill(self):
    if debug: print self.name,"killed"
    self.isDead = True
    
  def move(self):
    if debug: print "-".rjust(8,"-")
    if FleaLanes.findPlace(self):
      self.kill()
    else:
      if debug: print self.name,"moved to:",str(self.place)
    if debug: print "-".rjust(8,"-")
    
  def run(self):
    if debug: print("Starting Flea:"+self.name)
    while not self.dead():
      self.move()
      time.sleep(self.interval)
      
    if debug: print("Dead Flea:"+self.name)
        
      