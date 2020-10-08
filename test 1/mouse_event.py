import numpy as np
import cv2

# print all the EVENTS
events = [i for i in dir(cv2) if 'EVENT' in i]
print(events)


def click_event1(event, x, y, flags, param):
    font = cv2.FONT_HERSHEY_COMPLEX_SMALL

    if event == cv2.EVENT_LBUTTONDOWN:
        print(x, ' ', y, )
        strXY = str(x) + ', ' + str(y)
        cv2.putText(img, strXY, (x, y), font, 0.5, (0, 0, 255), 2)
        cv2.imshow('image', img)
    if event == cv2.EVENT_RBUTTONDOWN:
        blue = img[y, x, 0]
        green = img[y, x, 1]
        red = img[y, x, 2]
        print(blue, ' ', green, ' ', red)
        strBGR = str(blue) + ', ' + str(green) + ', ' + str(red)
        cv2.putText(img, strBGR, (x, y), font, 0.5, (0, 0, 255), 2)
        cv2.imshow('image', img)


def click_event2(event, x, y, flags, param):
    font = cv2.FONT_HERSHEY_COMPLEX_SMALL

    if event == cv2.EVENT_RBUTTONDOWN:
        cv2.circle(img, (x, y), 20, (0, 0, 255), -1)
        points.append((x, y))
        if len(points) >= 2:
            cv2.line(img, points[-1], points[-2], (0, 0, 0), 2)
        cv2.imshow('image', img)
    if event == cv2.EVENT_LBUTTONDOWN:
        blue = img[y, x, 0]
        green = img[y, x, 1]
        red = img[y, x, 2]
        cv2.circle(img, (x, y), 3, (0, 0, 255), -1)
        myColorImage = np.zeros((512, 512, 3), np.uint8)
        myColorImage[:] = [blue, green, red]
        cv2.imshow('color', myColorImage)


# img = np.zeros((512, 512, 3), np.uint8)
img = cv2.imread('road2.jpg', 1)
cv2.imshow('image', img)
points = []

cv2.setMouseCallback('image', click_event1)

cv2.waitKey(0)
cv2.destroyAllWindows()
