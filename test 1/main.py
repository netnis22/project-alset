import cv2

if __name__ == '__main__':
    print(cv2.__version__)

    img = cv2.imread('lena.jpg', 0)
    cv2.imshow('image', img)
    print(img)

    k = cv2.waitKey(0)

    if k == 27:
        cv2.destroyWindow('image')

    elif k == ord('s'):
        cv2.imwrite('lena_copy.png', img)
        cv2.destroyWindow('image')

    cv2.destroyAllWindows()

