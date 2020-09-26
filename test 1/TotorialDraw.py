import cv2
import numpy as np

#img = cv2.imread('lena.jpg', 1)
img = np.zeros([512, 512, 3], np.uint8)
font = cv2.FONT_HERSHEY_PLAIN

img = cv2.line(img, (0, 0), (255, 255), (255, 255, 255), 3)
img = cv2.arrowedLine(img, (0, 255), (255, 255), (255, 255, 255), 3)
img = cv2.rectangle(img, (384, 0), (510, 128), (0, 60, 23), 3)
img = cv2.circle(img, (447, 63), 63, (0, 59, 69), -1)
img = cv2.putText(img, 'opencv', (10, 500), font, 4, (255, 255, 255), 10, cv2.LINE_AA)

cv2.imshow('image', img)

cv2.waitKey(0)
cv2.destroyAllWindows()
