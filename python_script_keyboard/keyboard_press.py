from pynput.keyboard import Key, Listener, Controller
import pynput 
import time
import serial
import sys
import threading

port = '/dev/ttyUSB1'

class SerialThread(threading.Thread):
 def run(self):
  ser = serial.Serial(port)
  print (ser)
  if (ser.is_open == True):
      message_ser = "The serial port is open"
      print(message_ser)
  else:
      message_ser = "The serial port is closed"
      print(message_ser)
  # while(True):
    
class KeyboardThread(threading.Thread):
 def run(self):
  ser = serial.Serial(port)

  def serTog(key):
   if key.char == "1":
     ser.write(b'1')
   elif key.char == "2":
     ser.write(b'2')

  def on_press(key):
   try:
    # print('{0} pressed'.format(key))
    serTog(key)
    if key.char == "f":
     ser.write(b'f')
    elif key.char == "b":
     ser.write(b'b')
    elif key.char == "l":
     ser.write(b'l')
    elif key.char == "r":
     ser.write(b'r')
    else:
     ser.write(b'n')
     
    if key.char == "q":
     exit()
   except AttributeError:
    print('special key {0} pressed'.format(key))
    
  def on_release(key):
   ser.write(b'n')
   if key == Key.esc:
    print("closing serial and keylog")
    ser.close()
    return False
  

  with Listener( on_press = on_press, on_release = on_release ) as listener:
   listener.join()
  listener = keyboard.Listener(on_press=on_press, on_release=on_release)
  listener.start()

serial_thread = SerialThread()
keyboard_thread = KeyboardThread()

serial_thread.start()
keyboard_thread.start()
serial_thread.join()
keyboard_thread.join()


  
# For typing a character or a string

# keyboard = Controller()
# time.sleep(2)

# keyboard.press('i')
# keyboard.release('i')
# keyboard.type("This is a sentence written in Python")
