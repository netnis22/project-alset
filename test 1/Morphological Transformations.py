import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt

img = cv.imread('smarties.png', cv.IMREAD_GRAYSCALE)

titles =['imade']
images = [img]

for i in range(1):
    plt.subplot(1, 1, i+1), plt.imshow(images[i], 'gray')
    plt.title(titles[i])
    plt.xticks([]), plt.yticks([])

plt.show()

