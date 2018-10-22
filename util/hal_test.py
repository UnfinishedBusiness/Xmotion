import hal
h = hal.component("indicators")
h.newpin("ind",hal.HAL_BIT, hal.HAL_OUT)
h.ready()
hal.connect("ind","motion.spindle-on")
while 1:
    print h['ind']
