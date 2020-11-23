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

inst = readSer(instruction_ser)
while inst != 'INIT' and inst != 'QUIT':
    print(inst)
    printSer(instruction_ser, 'Enter instruction code: ')
    inst = readSer(instruction_ser)
# # # time.sleep(2)
# readSerUntil(instruction_ser, 'sss')

instruction_ser.close()

