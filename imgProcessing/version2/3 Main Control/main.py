from scan import init, scanFor
# from serial_flow import readSerUntil
import serial, time

def readSer(ser):
    return ser.readline().decode().strip()

def printSer(ser, string):
    ser.write(string.encode())

def readSerUntil(ser, until):
    inst = readSer(ser)
    while not inst in until:
        print(inst)
        inst = readSer(ser)

instruction_ser = serial.Serial('COM3', 57600)
readSerUntil(instruction_ser, ['INIT'])
print('pass')

state = init('COM9')
print(state)
# TODO: send state through serial
printSer(instruction_ser, f'{state["L"]}|{state["M"]}|{state["R"]}')
time.sleep(4)
# TODO: input from Serial
inp = ''
while True:
    inp = readSer(instruction_ser)
    if inp == 'Q':
        break
    out = scanFor('COM9', inp)
    printSer(instruction_ser, out)
