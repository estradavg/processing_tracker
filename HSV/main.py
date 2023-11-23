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

myColorFinder = ColorFinder(True)
hsvVals = {'hmin': 2, 'smin': 155, 'vmin': 126, 'hmax': 9, 'smax': 255, 'vmax': 255}

while True:
    success, img = cap.read()
    
    #print(hsvVals)

    imgColor, mask = myColorFinder.update(img, hsvVals)

    imgStack = cvzone.stackImages([img, imgColor, mask], 2, 0.5)
    cv2.imshow("Image", imgStack)
    cv2.waitKey(1)