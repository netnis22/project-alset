# importing
import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt
import datetime


def infrocamPrint(cap):
    print('infro from camara:')
    print('the HEIGHT ', cap.get(cv.CAP_PROP_FRAME_HEIGHT))
    print('the WIDTH ', cap.get(cv.CAP_PROP_FRAME_WIDTH))
    print("fps ", cap.get(cv.CAP_PROP_FPS))


def infrocam(frame, cap):
    font = cv.FONT_HERSHEY_COMPLEX_SMALL
    datet = str(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
    text = 'width: ' + str(cap.get(3)) + ' Height: ' + str(cap.get(4)) + ' Frame: ' + str(cap.get(5))
    frame = cv.putText(frame, datet, (0, 20), font, 0.7, (0, 0, 255), 1, cv.LINE_AA)
    frame = cv.putText(frame, text, (0, 40), font, 0.7, (0, 0, 255), 1, cv.LINE_AA)


def makemask(frame):
    l_h = 98
    l_s = 58
    l_v = 67

    u_h = 107
    u_s = 145
    u_v = 152

    l_b = np.array([l_h, l_s, l_v])
    u_b = np.array([u_h, u_s, u_v])

    hsv = cv.cvtColor(frame, cv.COLOR_BGR2HSV)

    mask = cv.inRange(hsv, l_b, u_b)
    mask = cv.GaussianBlur(mask, (5, 5), 0)
    mask = cv.medianBlur(mask, 5)

    return mask


def showimages(frame, mask, edges, res):
    cv.imshow("video", frame)
    cv.imshow("mask", mask)
    cv.imshow('Edges', edges)
    cv.imshow("res", res)


def main():
    cap = cv.VideoCapture(0)
    plt.title('image')

    while True:
        _, frame = cap.read()

        infrocam(frame, cap)

        mask = makemask(frame)
        edges = cv.Canny(mask, 0, 400)
        res = cv.bitwise_and(frame, frame)

        showimages(frame, mask, edges, res)

        key = cv.waitKey(1)
        if key == 27:
            break
    cap.release()
    cv.destroyAllWindows()


if __name__ == '__main__':
    main()
