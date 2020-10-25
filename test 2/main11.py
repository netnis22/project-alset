import numpy as np
import cv2

img1 = np.zeros((250,500, 3), np.uint8)
img1 = cv2.rectangle(img1,(200,0), (300,100), (255,255,255), -1)
img2 = cv2.imread("image_1.jpg")

img2 = cv2.resize(img2, (500,250))


#bitAnd = cv2.bitwise_and(img1, img2)
#bitor = cv2.bitwise_or(img1, img2)
#bitxor = cv2.bitwise_xor(img1, img2)
bitnot1 = cv2.bitwise_not(img1)
bitnot2 = cv2.bitwise_not(img2)

cv2.imshow('img1', img1)
cv2.imshow('img2', img2)
#cv2.imshow('bitAnd', bitAnd)
#cv2.imshow('bitAnd', bitor)
#cv2.imshow('bitAnd', bitxor)
cv2.imshow('bitAnd1', bitnot1)
cv2.imshow('bitAnd2', bitnot2)


cv2.waitKey(0)
cv2.destroyAllWindows()