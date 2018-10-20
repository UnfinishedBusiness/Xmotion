#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sys
import linuxcnc
try:
        s = linuxcnc.stat() # create a connection to the status channel
        s.poll() # get current values
except linuxcnc.error, detail:
        print "error", detail
        sys.exit(1)
print getattr(s,'position')[0] #X
print getattr(s,'position')[1] #Y
print getattr(s,'position')[2] #Z
