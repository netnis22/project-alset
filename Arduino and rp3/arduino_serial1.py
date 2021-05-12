#!/usr/bin/env python3

import time
import serial

def main():
    ser = serial.Serial('/dev/ttyUSB0', 9600)
    # ser.flush()
    while True:
        buff = (input('> ')+ "_"  ).encode()
        ser.write(buff)
        time.sleep(0.1)
       # while ser.in_waiting > 0:דד
            #line = ser.read().decode()ד
           # print(line)
    time.sleep(0.1)

if __name__ == '__main__':
    main()