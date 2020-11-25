import capture
import serial, time
import PROCESS
from get_data import extract_data

def scanFor(COM_PORT, dest):
    ser = serial.Serial(COM_PORT, 9600)
    time.sleep(4)
    ser.write(b'L')
    ser.read_until('L'.encode())
    capture.capture('COM2', 'l')
    sl = PROCESS.process('l.bmp')
    if sl == dest:
        ser.write(b'M')
        ser.read_until('M'.encode())
        ser.close()
        return extract_data('l.bmp')
    ser.write(b'M')
    ser.read_until('M'.encode())
    capture.capture('COM2', 'm')
    sm = PROCESS.process('m.bmp')
    if sm == dest:
        ser.write(b'M')
        ser.read_until('M'.encode())
        ser.close()
        return extract_data('m.bmp')
    ser.write(b'R')
    ser.read_until('R'.encode())
    capture.capture('COM2', 'r')
    sr = PROCESS.process('r.bmp')
    ser.write(b'M')
    ser.read_until('M'.encode())
    if sr == dest:
        ser.write(b'M')
        ser.read_until('M'.encode())
        ser.close()
        return extract_data('r.bmp')
    ser.close()
    return 'No pic!'

def init(COM_PORT):
    ser = serial.Serial(COM_PORT, 9600)
    time.sleep(4)
    ser.write(b'L')
    ser.read_until('L'.encode())
    capture.capture('COM2', 'l')
    sl = PROCESS.process('l.bmp')
    ser.write(b'M')
    ser.read_until('M'.encode())
    capture.capture('COM2', 'm')
    sm = PROCESS.process('m.bmp')
    ser.write(b'R')
    ser.read_until('R'.encode())
    capture.capture('COM2', 'r')
    sr = PROCESS.process('r.bmp')
    ser.write(b'M')
    ser.read_until('M'.encode())
    ser.close()


    # data = {
    #     'L': extract_data('l.bmp'),
    #     'M': extract_data('m.bmp'),
    #     'R': extract_data('r.bmp'),
    # }

    return { 'L': sl, 'M': sm, 'R': sr } # , 'data': data }

# print(init('COM3'))
