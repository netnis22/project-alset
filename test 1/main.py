import cv2
import matplotlib
import matplotlib.pyplot as plt

if __name__ == '__main__':
    print(cv2.__version__)
    img = cv2.imread('road.jpg', 1)
    cv2.imshow('img', img)

    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    #b, g, r = cv2.split(cv2.imread('road.jpg', 1))  # get b,g,r
    #img = cv2.merge([r, g, b])  # switch it to rgb

    plt.imshow(img)
    plt.title('image')
    plt.show()

    k = cv2.waitKey(0)


    if k == 27:
        cv2.destroyWindow('image')

    elif k == ord('s'):
        cv2.imwrite('road_copy.png', img)
        cv2.destroyWindow('image')

    cv2.destroyAllWindows()

