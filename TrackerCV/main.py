from asyncio.windows_events import NULL
from itertools import count
from sre_constants import SUCCESS
import cvzone
from cvzone import ColorFinder
import cv2
import socket


cap = cv2.VideoCapture()
cap.open(0)
cap.set(3, 1080)
cap.set(4, 720)

succes, img = cap.read()
h, w, _ = img.shape

myColorFinder = ColorFinder(False)
hsvVals = {'hmin': 37, 'smin': 34, 'vmin': 0, 'hmax': 105, 'smax': 255, 'vmax': 255}

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(("127.0.0.1", 12000))

while True:
    success, img = cap.read()
    imgColor, mask = myColorFinder.update(img, hsvVals)
    imgContour, contour = cvzone.findContours(img, mask)

    if contour: 
        data = contour[0]['center'][0],\
               h-contour[0]['center'][1],\
               int(contour[0]['area'])
        print(data)
        if ( data[0] != NULL and data[1] != NULL ):
            sock.send((str(data[0]) + "\n").encode())
            sock.send((str(data[1]) + "\n").encode())

    imgStack = cvzone.stackImages([img, imgColor, mask, imgContour], 2, 0.5)
    cv2.imshow("Image", imgStack)
    cv2.waitKey(1)