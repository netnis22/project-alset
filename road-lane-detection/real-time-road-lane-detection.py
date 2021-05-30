# importing
#!/usr/bin/env python3

import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt
import datetime
import operator
import time
import serial


# פונקציות שאני לא מישתמש
def average_slope_intercept1(image, lines):
    left_fit = []
    right_fit = []
    for line in lines:
        x1, y1, x2, y2 = line.reshape(4)

        # It will fit the polynomial and the intercept and slope
        parameters = np.polyfit((x1, x2), (y1, y2), 1)
        slope = parameters[0]
        intercept = parameters[1]
        if slope < 0:
            left_fit.append((slope, intercept))
        else:
            right_fit.append((slope, intercept))

    if left_fit:
        left_fit_average = np.average(left_fit, axis=0)
        print(left_fit_average, 'left')
        left_line = create_coordinates(image, left_fit_average)
    if right_fit:
        right_fit_average = np.average(right_fit, axis=0)
        print(right_fit_average, 'right')
        right_line = create_coordinates(image, right_fit_average)


# Prints camera information
def infrocamPrint(cap):
    print('infro from camara:')
    print('the HEIGHT ', cap.get(cv.CAP_PROP_FRAME_HEIGHT))
    print('the WIDTH ', cap.get(cv.CAP_PROP_FRAME_WIDTH))
    print("fps ", cap.get(cv.CAP_PROP_FPS))


# Place the information on the camera screen
def infroCam(frame, cap):
    font = cv.FONT_HERSHEY_COMPLEX_SMALL
    datet = str(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
    text = 'width: ' + str(cap.get(3)) + ' Height: ' + str(cap.get(4)) + ' Frame: ' + str(cap.get(5))
    frame = cv.putText(frame, datet, (0, 20), font, 0.7, (0, 0, 255), 1, cv.LINE_AA)
    frame = cv.putText(frame, text, (0, 40), font, 0.7, (0, 0, 255), 1, cv.LINE_AA)


# make the mask image
def makeMask(frame):
    l_h = 82
    l_s = 48
    l_v = 43

    u_h = 121
    u_s = 134
    u_v = 146

    l_b = np.array([l_h, l_s, l_v])
    u_b = np.array([u_h, u_s, u_v])

    hsv = cv.cvtColor(frame, cv.COLOR_BGR2HSV)

    mask = cv.inRange(hsv, l_b, u_b)
    mask = cv.GaussianBlur(mask, (5, 5), 0)
    mask = cv.medianBlur(mask, 5)

    return mask


# show the images on the screen
def showImages(frame, mask, edges, roi, res, line_image):
    #cv.imshow("video", frame)
    #cv.imshow("mask", mask)
    cv.imshow('Edges', edges)
    cv.imshow("roi", roi)
    cv.imshow("res", res)
    # cv.imshow("line_image", line_image)


# Checks if esc is pressed and if esc is pressed returns False otherwise returns True
# The function exists to exit the loop
def breakLoop():
    key = cv.waitKey(1000)
    if key == 27:
        return False
    return True


def region_of_interest(edges):
    height = edges.shape[0]
    #polygons1 = np.array([[(0, height), (640, height), (320, 240)]])
    polygons1 = np.array([[(0, height), (640, height), (0, 250)]])
    polygons2 = np.array([[(0, 250), (640, height), (640, 250)]])

    mask = np.zeros_like(edges)
    cv.fillPoly(mask, polygons1, 255)
    cv.fillPoly(mask, polygons2, 255)

    masked_image = cv.bitwise_and(edges, mask)
    return masked_image


def create_coordinates(image, line_parameters):
    slope, intercept = line_parameters
    y1 = image.shape[0]
    y2 = int(y1 * (3 / 5))
    x1 = int((y1 - intercept) / slope)
    x2 = int((y2 - intercept) / slope)
    return np.array([x1, y1, x2, y2])


def midelOFaLine(lines_coordinates):
    xMID = (lines_coordinates[0] + lines_coordinates[2]) / 2
    yMID = (lines_coordinates[1] + lines_coordinates[3]) / 2
    return np.array([xMID, yMID])


def midelOfRode(lineL, lineR):
    lineMidL = midelOFaLine(lineL)
    lineMidR = midelOFaLine(lineR)
    midX = (lineL[0] + lineR[0]) / 2
    midY = (lineL[1] + lineR[1]) / 2
    return np.array([midX, midY])


d = {'V10T': 0, 'V0T': 0, 'V0T': 0, 'V0T': 0, 'V10T': 0, 'V10T': 0, 'error': 0}

turnError = 8
def errorTurn(left_fit, right_fit, image):
    global turnError

    if left_fit:
        left_fit_average = np.average(left_fit, axis=0)
        # print(left_fit_average, 'left')
        left = True
        left_line = create_coordinates(image, left_fit_average)
        # print(left_line, "left_line")
    else:
        left = False
    if right_fit:
        right_fit_average = np.average(right_fit, axis=0)
        # print(right_fit_average, 'right')
        right = True
        right_line = create_coordinates(image, right_fit_average)
        # print(right_line, "right_line")
    else:
        right = False

    if(left and right):
        turnError=((midelOfRode(left_line, right_line)[0])-320)/320
    elif(not(right) and left):
        turnError=1
    elif(not(left) and right):
        turnError=-1
    else:
        turnError=0


msg = ''
def checkAndPrint(left_fit, right_fit, image):
    global msg
    if left_fit:
        left_fit_average = np.average(left_fit, axis=0)
        # print(left_fit_average, 'left')
        left = True
        left_line = create_coordinates(image, left_fit_average)
        # print(left_line, "left_line")
    else:
        left = False
    if right_fit:
        right_fit_average = np.average(right_fit, axis=0)
        # print(right_fit_average, 'right')
        right = True
        right_line = create_coordinates(image, right_fit_average)
        # print(right_line, "right_line")
    else:
        right = False

    if (left and right and (310 < midelOfRode(left_line, right_line)[0] < 330)) and (
            left and right and midelOFaLine(left_line)[1] == midelOFaLine(right_line)[1]):
        msg = 'V10T' #MID
    elif left is False and right is False:
        msg = 'V0T' #STOP
    elif right and (left is False):
        msg = 'V0T' #LEFT
    elif (right is False) and left:
        msg = 'V0T' #RIGHT
    elif midelOfRode(left_line, right_line)[0] > 330:
        msg = 'V10T' #right + drive
    elif 310 > midelOfRode(left_line, right_line)[0]:
        msg = 'V10T' #left + drive
    else:
        msg = 'error'

    d[msg] = d[msg] + 1
    # print(msg)


def average_slope_intercept(image, lines):
    left = False
    right = False
    left_fit = []
    right_fit = []
    if lines is not None:
        for line in lines:
            x1, y1, x2, y2 = line.reshape(4)
            parameters = np.polyfit((x1, x2), (y1, y2), 1)
            slope = parameters[0]
            intercept = parameters[1]
            if slope < 0:
                left_fit.append((slope, intercept))
            else:
                right_fit.append((slope, intercept))
    errorTurn(left_fit, right_fit, image)
    checkAndPrint(left_fit, right_fit, image)


def display_lines(image, lines):
    line_image = np.zeros_like(image)
    if lines is not None:
        for x1, y1, x2, y2 in lines:
            cv.line(line_image, (x1, y1), (x2, y2), (255, 0, 0), 10)
    return line_image


def main():
    #communication
    ser = serial.Serial('/dev/ttyACM0', 9600)

    counter = 0
    flag = True
    cap = cv.VideoCapture(0)
    cap.set(3, 640)
    cap.set(4, 480)
    cap.set(5, 10)



    while flag & cap.isOpened():
        _, frame = cap.read()

        infroCam(frame, cap)  # Activates the function infrocam
        mask = makeMask(frame)  # Activates the function makemask
        edges = cv.Canny(mask, 0, 400)  # Prepares an image only at the edges of the mask
        roi = region_of_interest(edges)
        debug = region_of_interest(frame)
        #cv.imshow('debug', debug)



        msg = 'V0T0'+"_"
        lines = cv.HoughLinesP(roi, 2, np.pi / 180, 100, np.array([]), minLineLength=40, maxLineGap=5)
        averaged_lines = average_slope_intercept(edges, lines)
        counter += 1
        if counter > 2:
            msg = (max(d.items(), key=operator.itemgetter(1))[0])
            counter = 0
            for i in d.keys():
                d[i] = 0
            print((f'{msg}{turnError}')+'_')
            #communication
            buff = ((f'{msg}{turnError}')+'_').encode()
            ser.write(buff)

        line_image = display_lines(frame, averaged_lines)
        res = cv.addWeighted(frame, 0.8, line_image, 1, 1)
        showImages(frame, mask, edges, roi, res, line_image)  # Activates the function showImages



        flag = breakLoop()  # Activates the function showImages breakLoop
    cap.release()
    cv.destroyAllWindows()


if __name__ == '__main__':
    main()