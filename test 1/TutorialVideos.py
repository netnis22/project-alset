import cv2


def infrocam(cap):
    print('infro from camara')
    print('the HEIGHT ', cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    print('the WIDTH ', cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    print("camara status:", cap.isOpened())


cap = cv2.VideoCapture(0)

infrocam(cap)

while cap.isOpened():
    ret, frame = cap.read()

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    cv2.imshow('frame', gray)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
