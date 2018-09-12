#!/usr/bin/env python
# -*- coding: utf-8 -*-
import linuxcnc
c = linuxcnc.command()
c.mode(linuxcnc.MODE_MANUAL)
c.jog(linuxcnc.JOG_INCREMENT, 0, 100, 1)
