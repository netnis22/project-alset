import numpy as np
import cv2


def click_event(event, x, y, flags, param):
    font = cv2.FONT_HERSHEY_COMPLEX_SMALL

    if event == cv2.EVENT_LBUTTONDOWN:
        print(x, ' ', y, )
        strXY = str(x) + ', ' + str(y)
        cv2.putText(img, strXY, (x, y), font, 0.5, (0, 0, 255), 2)
        cv2.imshow('image', img)
    if event == cv2.EVENT_RBUTTONDOWN:
        xys = loopDraw((0, 0), (1000, 1000))
        cv2.circle(img, (xys[0]), 20, (0, 0, 255), 0)
        cv2.circle(img, (xys[-1]), 20, (0, 0, 255), 0)
        cv2.imshow('image', img)
        xys = []



def loopDraw(xy1, xy2):
    xyS = []
    indexes = 0
    for x in range(xy1[0], xy2[0] + 1):
        for y in range(xy1[1], xy2[1] + 1):
            if IsYellow(img[y, x, 0], img[y, x, 1], img[y, x, 2]):
                xyS.insert(indexes, (x, y))
                indexes = indexes + 1
    return xyS


def IsYellow(blue, green, red):
    if blue in range(0, 100):
        if red in range(150, 255):
            if green in range(150, 255):
                return True


img = cv2.imread('road.jpg', 1)
cv2.imshow('image', img)
points = []

cv2.setMouseCallback('image', click_event)

cv2.waitKey(0)
cv2.destroyAllWindows()
