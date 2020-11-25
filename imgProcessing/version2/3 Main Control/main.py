from scan import init, scanFor
# from serial_flow import readSerUntil
import serial, time

def readSer(ser):
    try:
        return ser.readline().decode('utf-8').strip()
    except Exception as identifier:
        return ''

def printSer(ser, string):
    ser.write((string + ':').encode())

def readSerUntil(ser, until):
    inst = readSer(ser)
    print(inst)
    while not inst in until:
        print(inst)
        inst = readSer(ser)

instruction_ser = serial.Serial('COM18', 115200)
readSerUntil(instruction_ser, ['INIT'])
print('pass')

state = init('COM3')
print(state)
# TODO: send state through serial
printSer(instruction_ser, f'Send {state["L"]}|{state["M"]}|{state["R"]}: ~')
time.sleep(4)
# TODO: input from Serial
inp = ''
while True:
    inp = readSer(instruction_ser)
    print(inp)
    if inp == 'INIT':
        state = init('COM3')
        printSer(instruction_ser, f'Send {state["L"]}|{state["M"]}|{state["R"]}: ~')
    if inp.startswith('$'):
        if inp == '$Q':
            break
        out = scanFor('COM3', inp[1:5])
        print(out)
        printSer(instruction_ser, f'Send {out}: ~')
