#!/usr/bin/env python3

import time
import serial


def main1():
    #ser = serial.Serial('/dev/ttyACM0', 9600)
    ser = serial.Serial('/dev/ttyUSB0', 9600)
    # ser.flush()
    # ser.reset_input_buffer()
    # ser.reset_output_buffer()

    while True:
        # buff = input('> ').encode()
        # ser.write(buff)

        # print(f'in_waiting = {ser.in_waiting}')
        while ser.in_waiting > 0:
            print(f'in_waiting = {ser.in_waiting}')
            rd = ser.read().decode()
            print(rd)

        time.sleep(0.1)


def main():
    ser = serial.Serial('/dev/ttyUSB0', 9600)
    # ser.flush()
    while True:
        buff = (input('> ')+ "_"  ).encode()
        ser.write(buff)
        time.sleep(0.1)
        while ser.in_waiting > 0:
            line = ser.read().decode()
            print(line)
    time.sleep(0.1)

if __name__ == '__main__':
    main()