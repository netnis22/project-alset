import cv2


# this def print indromaisah about the camara
def infrocam(cap):
    print('infro from camara:')
    print('the HEIGHT ', cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    print('the WIDTH ', cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    print("fps ", cap.get(cv2.CAP_PROP_FPS))
    print("camara open? ", cap.isOpened())


cap = cv2.VideoCapture(0)
fourcc = cv2.VideoWriter_fourcc(*'XVID')
out = cv2.VideoWriter('output.avi', fourcc, 30.0, (640, 480))

# print indromaisah about the camara
infrocam(cap)

while cap.isOpened():
    ret, frame = cap.read()

    if ret:

        out.write(frame)

        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        cv2.imshow('frame', gray)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    else:
        break

cap.release()
out.release()
cv2.destroyAllWindows()
