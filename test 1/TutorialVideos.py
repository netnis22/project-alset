import cv2
import datetime


def nothing(x):
    print(x)

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
font = cv2.FONT_HERSHEY_COMPLEX_SMALL

cv2.namedWindow('frame')

cv2.createTrackbar('HEIGHT', 'frame', 480, 1980, nothing)
cv2.createTrackbar('WIDTH', 'frame', 640, 1060, nothing)
switch = 'color/gray'
cv2.createTrackbar(switch, 'frame', 0, 1, nothing)



# print indromaisah about the camara
infrocam(cap)

while cap.isOpened():
    ret, frame = cap.read()

    if ret:
        out.write(frame)

        datet = str(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        text = 'width: ' + str(cap.get(3)) + ' Height: ' + str(cap.get(4)) + ' Frame: ' + str(cap.get(5))

        frame = cv2.putText(frame, datet, (0, 20), font, 0.7, (0, 0, 255), 1, cv2.LINE_AA)
        frame = cv2.putText(frame, text, (0, 40), font, 0.7, (0, 0, 255), 1, cv2.LINE_AA)

        height = cv2.getTrackbarPos('HEIGHT', 'frame')
        width = cv2.getTrackbarPos('WIDTH', 'frame')
        s = cv2.getTrackbarPos(switch, 'frame')

        #cap.set(cv2.CAP_PROP_FRAME_HEIGHT, height)
        #cap.set(cv2.CAP_PROP_FRAME_WIDTH, width)

        if s == 0:
            pass
        else:
            frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)




        cv2.imshow('frame', frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    else:
        break

cap.release()
out.release()
cv2.destroyAllWindows()
