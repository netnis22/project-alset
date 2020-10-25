import numpy as np
import cv2
img = cv2.imread('lena.jpg',1)
img =np.zeros([600,600,3],np.uint8 )
font= cv2.QT_FONT_NORMAL

img = cv2.line(img, (0,0), (250,250), ( 147, 96, 44), 5)

img = cv2.arrowedLine(img, (0,250), (250,250), ( 147, 96, 44), 5)

img = cv2.rectangle(img, (384,0), (510,128), ( 147, 96, 44), 5)

img = cv2.circle(img,(447,64),64, ( 3, 3, 4) ,-1)

img = cv2.putText(img,'hello', (10,500) ,font,4, (0, 0, 204) , 5,cv2.LINE_AA)


cv2.imshow('image',img)

cv2.waitKey(0)
cv2.destroyAllWindows()
