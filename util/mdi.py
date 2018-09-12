#!/usr/bin/env python
# -*- coding: utf-8 -*-
import linuxcnc
c = linuxcnc.command()
c.mode(linuxcnc.MODE_MDI)
c.mdi("G0 X10 Y10")
