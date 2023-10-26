import serial
import matplotlib.pyplot as plt
import time

arduino = serial.Serial('COM6', 9600, timeout=5) #macbook: ls /dev/tty.* -> format as /dev/tty.usbmodem or /dev/tty.usbserial
time.sleep(1)
xdata=[]
voltage=[]
duty_cycle=[float(i)/255.0*100 for i in range(255)]

for k in range(255):
    arduino.reset_output_buffer()
    arduino.reset_input_buffer()
    arduino.write(b'1')
    time.sleep(0.5)
    
    # Read 5 values and calculate average
    readings = []
    read = 0
    while True:
        try:
            a = int(arduino.readline().decode("utf-8"))
            readings.append(a)
            read += 1
        except:
            time.sleep(0.5)
            read -= 1
        if read == 5: break        
    avg_reading = sum(readings) / len(readings)
    
    v = float(avg_reading)/1023.0*5.0
    voltage.append(v)
    print(k, avg_reading)

arduino.close()
plt.plot(duty_cycle, voltage, 'bo')
plt.xlabel('Duty Cycle (%) ')
plt.ylabel('Voltage (V)')
plt.grid(True)
plt.xlim(0,100)
plt.ylim(0,5)
plt.show()
