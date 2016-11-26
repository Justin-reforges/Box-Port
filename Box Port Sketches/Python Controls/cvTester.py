import cv2


capture = cv2.VideoCapture(1)

while True:
	
	frame, ret = capture.read()

	cv2.imshow("frame",frame)

