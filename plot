#!/usr/bin/python2

from matplotlib import pyplot as pp
pp.ion()

while True:
  # read 2d data point from stdin
  data = [float(x) for x in raw_input().split()]
  assert len(data) == 2, "can only plot 2d data!"
  x,y = data
  # plot the data
  print "plotting!"
  pp.plot([x],[y],'r.')
  pp.draw()
