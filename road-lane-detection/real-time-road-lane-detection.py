# importing
import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt
import datetime


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


# show the images on the screen
def showImages(frame, mask, edges, roi, res):
    cv.imshow("video", frame)
    cv.imshow("mask", mask)
    cv.imshow('Edges', edges)
    cv.imshow("res", res)
    cv.imshow("roi", roi)


# Checks if esc is pressed and if esc is pressed returns False otherwise returns True
# The function exists to exit the loop
def breakLoop():
    key = cv.waitKey(1)
    if key == 27:
        return False
    return True


def region_of_interest(edges):
    height = edges.shape[0]
    polygons1 = np.array([[(0, height), (640, height), (0, 250)]])
    polygons2 = np.array([[(0, 250), (640, height), (640, 250)]])

    mask = np.zeros_like(edges)
    cv.fillPoly(mask, polygons1, 255)
    cv.fillPoly(mask, polygons2, 255)

    masked_image = cv.bitwise_and(edges, mask)
    return masked_image


#def create_coordinates(image, line_parameters):
    slope, intercept = line_parameters
    y1 = image.shape[0]
    y2 = int(y1 * (3 / 5))
    x1 = int((y1 - intercept) / slope)
    x2 = int((y2 - intercept) / slope)
    return np.array([x1, y1, x2, y2])


#def average_slope_intercept(image, lines):
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

    left_fit_average = np.average(left_fit, axis=0)
    right_fit_average = np.average(right_fit, axis=0)
    left_line = create_coordinates(image, left_fit_average)
    right_line = create_coordinates(image, right_fit_average)
    return np.array([left_line, right_line])


#def display_lines(image, lines):
    line_image = np.zeros_like(image)
    if lines is not None:
        for x1, y1, x2, y2 in lines:
            cv.line(line_image, (x1, y1), (x2, y2), (255, 0, 0), 10)
    return line_image


def main():
    flag = True
    cap = cv.VideoCapture(0)

    while flag & cap.isOpened():
        _, frame = cap.read()

        infroCam(frame, cap)  # Activates the function infrocam
        mask = makeMask(frame)  # Activates the function makemask
        edges = cv.Canny(mask, 0, 400)  # Prepares an image only at the edges of the mask
        roi = region_of_interest(edges)

        #lines = cv.HoughLinesP(roi, 2, np.pi / 180, 100, np.array([]), minLineLength=40, maxLineGap=5)
        #averaged_lines = average_slope_intercept(frame, lines)
        #line_image = display_lines(frame, averaged_lines)
        #res = cv.addWeighted(frame, 0.8, line_image, 1, 1)
        res =frame

        showImages(frame, mask, edges, roi, res)  # Activates the function showImages

        flag = breakLoop()  # Activates the function showImages breakLoop
    cap.release()
    cv.destroyAllWindows()


if __name__ == '__main__':
    main()
