import serial
from PIL import Image
import time
import sys

width=160
height=120

def read_image(serial, name):
  print("Reading Image...")
  data = serial.read(width * height)
  print("Received Image...")
  image = Image.frombytes('P', (width, height), data)
  image.save(f'./{name}.bmp')

def capture(COM_PORT, name):
  port = COM_PORT
  print("Using port {}".format(port))

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

  for i in range(3):
    print("Waiting for RDY from Arduino...")
    ser.read_until('*RDY*'.encode())
    read_image(ser, name)

  ser.close()
