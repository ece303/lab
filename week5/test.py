import serial
import time

status = [False, False, False]
arduino = serial.Serial('COM5', 9600, timeout=5) #macbook: ls /dev/tty.* -> format as /dev/tty.usbmodem or /dev/tty.usbserial
arduino.flush()
time.sleep(1)
inp = input('Write Command: ')

while inp != 'stop':
    arduino.reset_output_buffer()
    arduino.reset_input_buffer()
    arduino.write(inp.encode('utf-8'))
    time.sleep(2)
    a = arduino.read(3).decode()
    for i in a:
        if i=='g':
            status[0] = not(status[0])
        elif i=='w':
            status[1] = not(status[1])
        elif i== 'r':
            status[2] = not(status[2])
    print(f"Status: GREEN: {status[0]}, WHITE: {status[1]}, RED:{status[2]}")
    inp = input('Write Command: ')

print(f"Final Status: GREEN: {status[0]}, WHITE: {status[1]}, RED: {status[2]}")
arduino.close()