import serial
import time

ser=serial.Serial("/dev/ttyUSB2",57600)

ser.close()
ser.open()
def openIris():
	ser.write("10,0")

def closeIris():
	ser.write("10,1")

def liftPad():
	ser.write("10,2")
def lowerPad():
	ser.write("10,3")
def stopAll():
	ser.write("0,0")
def manualOveride():
	ser.write("0,0")
def deploy():
	ser.write("30,0")
def retrieve():
	ser.write("30,1")
def chargeOn():
	ser.write("40,0")
def chargeOff():
	ser.write("40,1")
def takeoff():
	ser.write("50,1")
def landing():
	ser.write("50,2")
def cycle():
	for i in range(0,10):
		deploy()
		time.sleep(15)
		retrieve()
		time.sleep(30)

while ser.isOpen():
	inp = raw_input()
        if inp=="w":
		openIris()
	if inp=="s":
		closeIris()
	if inp=="a":
		liftPad()
	if inp=="d":
		lowerPad()
	if inp=="q":
		stopAll()
	if inp=="manual":
		manualOveride()
	if inp=="deploy":
		deploy()

	if inp=="retrieve":
		retrieve()
	if inp=="chargeON":
		print("charger On")
		chargeOn()
	if inp=="chargeOff":
		chargeOff()
	if inp=="cycle":
		cycle()
