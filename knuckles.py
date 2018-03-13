# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import serial
ser = serial.Serial('/dev/tty/usberial', 115200)
while true:
    print ser.readline()