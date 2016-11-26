import serial
import time

ser=serial.Serial("/dev/ttyUSB0",57600)

ser.close()
ser.open()

while ser.isOpen():
	inp = raw_input()
        ser.write(inp)
	#ser.read()
	time.sleep(0.5)

