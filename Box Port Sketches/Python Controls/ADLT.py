# import the necessary packages
import numpy as np
import argparse
import cv2
import time
import math

def nothing(x):
	pass

cap = cv2.VideoCapture(0)
#create window
cv2.namedWindow('gray')
#create buttons first
cv2.createTrackbar('param1','gray',35,200,nothing)
cv2.createTrackbar('param2','gray',65,200,nothing)
cv2.createTrackbar('minR','gray',20,200,nothing)
cv2.createTrackbar('maxR','gray',60,200,nothing)
cv2.createTrackbar('treshold','gray',255,255,nothing)  
cv2.createTrackbar('posL','gray',41,200,nothing)
cv2.createTrackbar('posU','gray',81,200,nothing)
cv2.createTrackbar('block','gray',3,50,nothing)
cv2.createTrackbar('C','gray',11,100,nothing)
cv2.createTrackbar('minD','gray',200,200,nothing)

count = 0

while(True):
	# Capture frame-by-frame
	frame = cv2.imread("testdots.jpg")
	# grab the dimensions of the image and calculate the center
	# of the image
	(h, w) = frame.shape[:2]
	center = (w / 2, h / 2)

	# rotate the image by 180 degrees
	M = cv2.getRotationMatrix2D(center, count, 1.0)
	frame = cv2.warpAffine(frame, M, (w, h))

	p1 = cv2.getTrackbarPos('param1','gray')
	p2 = cv2.getTrackbarPos('param2','gray')
	minR = cv2.getTrackbarPos('minR','gray')
	maxR = cv2.getTrackbarPos('maxR','gray')
	treshold = cv2.getTrackbarPos('treshold','gray')
	posL = cv2.getTrackbarPos('posL','gray')
	posU = cv2.getTrackbarPos('posU','gray')
	block = 2*(cv2.getTrackbarPos('block','gray')) +1
	C = cv2.getTrackbarPos('C','gray')
	minD = cv2.getTrackbarPos('minD','gray')


	# load the image, clone it for output, and then convert it to grayscale
			
	output = frame.copy()
	gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
	blue,green,red = cv2.split(frame)

	r = np.sqrt(np.power(red,2)+np.power(green,2)+np.power(blue,2))
	phi = np.dot(np.arctan2(blue,np.sqrt(np.power(green,2)+np.power(red,2))),180/np.pi)
	theta = np.dot(np.arctan2(red,green),180/np.pi)


	r = r.astype("uint8")
	phi = phi.astype("uint8")
	theta = theta.astype("uint8")

	lower = np.array([posL])
	upper = np.array([posU])
	mask = cv2.inRange(blue, 200, 210)
	

	res = cv2.bitwise_and(frame,frame, mask= mask)
	cimg = cv2.cvtColor(res,cv2.COLOR_BGR2GRAY)
	
	# apply GuassianBlur to reduce noise. medianBlur is also added for smoothening, reducing noise.
	cimg = cv2.GaussianBlur(cimg,(5,5),0);
	cimg = cv2.medianBlur(cimg,5)
	
	# Adaptive Guassian Threshold is to detect sharp edges in the Image. For more information Google it.
	C = 2*C-1.50
	gray = cv2.adaptiveThreshold(cimg,treshold,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,block,C)
	
	#kernel = np.ones((2.6,2.7),np.uint8)
	#gray = cv2.erode(gray,kernel,iterations = 1)
	#gray = erosion
	
	#gray = cv2.dilate(gray,kernel,iterations = 1)
	#gray = dilation

	# get the size of the final image
	# img_size = gray.shape
	# print img_size

	# detect circles in the image
	#circles = cv2.HoughCircles(cimg,cv2.cv.CV_HOUGH_GRADIENT, 1, minD, p1, p2, minR, maxR)
	# print circles
	# Setup SimpleBlobDetector parameters.
	params = cv2.SimpleBlobDetector_Params()

	# Change thresholds
	params.minThreshold = minR
	params.maxThreshold = maxR


	# Filter by Area.
	params.filterByArea = True
	params.maxArea = 400

	#Filter by Color
	params.filterByColor = True
	params.blobColor = 0

	# Filter by Circularity
	params.filterByCircularity = False
	params.minCircularity = 0.1

	# Filter by Convexity
	params.filterByConvexity = False
	params.minConvexity = 0.87
	    
	# Filter by Inertia
	params.filterByInertia = False
	params.minInertiaRatio = 0.01

	# Create a detector with the parameters
	ver = (cv2.__version__).split('.')
	if int(ver[0]) < 3 :
		detector = cv2.SimpleBlobDetector(params)
	else : 
		detector = cv2.SimpleBlobDetector_create(params)


	# Detect blobs.
	heading=0
	KPF=[]
	keypoints = detector.detect(gray)
	im_with_keypoints=frame
	for kp in keypoints:
		#print(len(keypoints))
		if (len(keypoints)==4):
			#print(int(keypoints[0].pt[0]),int(keypoints[0].pt[1]),int(keypoints[1].pt[0]),int(keypoints[1].pt[1]),int(keypoints[2].pt[0]),int(keypoints[2].pt[1]),int(keypoints[3].pt[0]),int(keypoints[3].pt[1]))
			x1= int(keypoints[0].pt[0])
			y1= int(keypoints[0].pt[1])
			x2= int(keypoints[1].pt[0])
			y2= int(keypoints[1].pt[1])
			x3= int(keypoints[2].pt[0])
			y3= int(keypoints[2].pt[1])
			x4= int(keypoints[3].pt[0])
			y4= int(keypoints[3].pt[1])

			centerX=(x1+x2+x3+x4)/4
			centerY=(y1+y2+y3+y4)/4

			heading = math.degrees(math.atan2(y1-centerY,x1-centerX))
			print heading
		if count ==0:

			kpPrevious = keypoints	
			count+=1
			im_with_keypoints = cv2.drawKeypoints(frame, keypoints, np.array([]), (0,255,0),  cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
		else:
			for KPP in kpPrevious:
				distance=math.sqrt(float(math.pow(KPP.pt[0]-kp.pt[0],2))+float(math.pow(KPP.pt[1]-kp.pt[1],2)))
				
				if distance < 0.2:
					#print(distance)
					KPF.append(KPP)
		if (len(KPF))>=4:
			#print(KPF[0].pt,KPF[1].pt)
			pt1 = KPF[0].pt
			pt2 = KPF[1].pt

			#cv2.line(frame,pt1,pt2,(0,0,255))
		im_with_keypoints = cv2.drawKeypoints(frame, KPF, np.array([]), (0,255,0), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
			
	#KPFPrevious = KPF
	#im_with_keypoints = cv2.drawKeypoints(frame, KPFPrevious, np.array([]), (0,255,0), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
					

	# Draw detected blobs as red circles.
	# cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS ensures
	# the size of the circle corresponds to the size of blob
	
	
	#cv2.rectangle(frame, (keypoints - 5, y - 5), (x + 5, y + 5), (0, 128, 255), -1)
	# ensure at least some circles were found
	'''
	if circles is not None:
		# convert the (x, y) coordinates and radius of the circles to integers
		circles = np.round(circles[0, :]).astype("int")
		
		# loop over the (x, y) coordinates and radius of the circles
		for (x, y, r) in circles:
			# draw the circle in the output image, then draw a rectangle in the image
			# corresponding to the center of the circle
			cv2.circle(output, (x, y), r, (255, 0, 0), 4)
			cv2.rectangle(output, (x - 5, y - 5), (x + 5, y + 5), (0, 128, 255), -1)
	'''
	# Display the resulting frame
	cv2.imshow('blue',mask)
	#cv2.imshow("yellow",mask2)
    	#cv2.imshow('Filtered Frame',gray)
	cv2.imshow("Keypoints", im_with_keypoints)
 	if cv2.waitKey(1) & 0xFF == ord('q'):
		break
	count+=1
	if count>360:
		count=0
# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()	
