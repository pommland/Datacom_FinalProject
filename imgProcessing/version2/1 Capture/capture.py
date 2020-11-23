import serial
from PIL import Image
import time
import sys

if len(sys.argv) > 1:
  port = sys.argv[1]
else:
  port=input("Which serial port to communicate on?")

print("Using port {}".format(port))
width=160
height=120

def read_image(serial):
  print("Reading Image...")
  data = serial.read(width * height)
  print("Received Image...")
  image = Image.frombytes('P', (width, height), data)
  image.save('./image.bmp')

# def isImageStart(index):
#   if index < len(COMMAND):
#     if COMMAND[index] == ser.read():
#       return isImageStart(index + 1)
#     else:
#       return False
#   return True

ser = serial.Serial(
  port=port,
  baudrate=1000000,
  parity=serial.PARITY_NONE,
  stopbits=serial.STOPBITS_ONE,
  bytesize=serial.EIGHTBITS)

print('Wait for Arduino to set itself up...')

if ser.isOpen() == False:
  ser.open()
  print('Serial opened')
else:
  print('Serial already open')


while True:
  print("Waiting for RDY from Arduino...")
  ser.read_until('*RDY*'.encode())
  read_image(ser)

ser.close()
print('Serial closed')