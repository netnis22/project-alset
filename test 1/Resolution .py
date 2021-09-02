import cv2

cap = cv2.VideoCapture(0)


def make_1080p():
    cap.set(3, 1920)
    cap.set(4, 1080)

def make_720p():
    cap.set(3, 1280)
    cap.set(4, 720)

def make_480p():
    cap.set(3, 640)
    cap.set(4, 480)

def make_240p():
    cap.set(3, 320)
    cap.set(4, 240)

def change_res(width, height):
    cap.set(3, width)
    cap.set(4, height)

cv2.namedWindow('frame')
make_480p()

while cap.isOpened():
    ret, frame = cap.read()
    if ret:
        cv2.imshow('frame', frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    else:
        break

cap.release()
out.release()
cv2.destroyAllWindows()
