from scan import init
import serial, time

state = init()
print(state)
# TODO: send state through serial
ser = serial.Serial('COM7', 9600)
time.sleep(4)
# TODO: input from Serial
inp = ''
while inp != 'Q':
    inp = input('Enter where you want me to rotate to: ')
    for key in state.keys():
        if state[key] == inp:
            ser.write(key.encode())
            ser.read_until(key.encode())
            break

ser.close()