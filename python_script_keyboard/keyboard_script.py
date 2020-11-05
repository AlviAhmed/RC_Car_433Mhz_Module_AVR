import serial
import time
import keyboard


ser = serial.Serial('/dev/ttyUSB2')
time.sleep(1)
print (ser)
if (ser.is_open == True):
        message_ser = "The serial port is open"
        print(message_ser)
else:
        message_ser = "The serial port is closed"
        print(message_ser)

for x in range (100):
        ser.write(b'f')
        ser.write(b'b')
        ser.write(b'l')
        ser.write(b'r')

ser.write(b'n')
ser.close()

print("closed")


