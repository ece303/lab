#  #import serial
import serial
from serial import Serial as pySerial
import time
import tkinter as tk
import tkinter.font as tkFont
from tkinter import *
from tkinter import messagebox
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

plt.close('all')
root = tk.Tk()
root.geometry("1000x600")
root.title("Photoregsistor Evaluatior")
root.configure(bg='grey')

figure1 = plt.Figure(figsize=(12,6), dpi=100)
ax1 = figure1.add_subplot(221)
fig1 = FigureCanvasTkAgg(figure1, root)
fig1.get_tk_widget().pack(side=tk.LEFT, fill=tk.BOTH)
figure1.set_facecolor('lightgrey')
line1, = ax1.plot([], [], 'bo')
ax1.set_xlabel('Duty Cycle (%)', fontsize=10)
ax1.set_ylabel('Voltage (V)', fontsize=10)
ax1.set_title("Photocell Voltage")
ax1.grid(visible=True, which='major', color='k', linestyle='-')
#ax1.grid(b=True, which='major', color='k', linestyle='-') #visible=True,
ax1.grid(visible=True, which='minor', color='r', linestyle='-', alpha=0.2)
#ax1.grid(b=True, which='minor', color='r', linestyle='-', alpha=0.2)
ax1.minorticks_on()

ax2 = figure1.add_subplot(222)
line2, = ax2.plot([], [], 'bo')
ax2.set_xlabel('Duty Cycle (%)', fontsize=10)
ax2.set_ylabel('Current (mA)', fontsize=10)
ax2.set_title("Photocell Voltage")
ax2.grid(visible=True, which='major', color='k', linestyle='-')
ax2.grid(visible=True, which='minor', color='r', linestyle='-', alpha=0.2)
ax2.minorticks_on()

ax3 = figure1.add_subplot(224)
line3, = ax3.plot([], [], 'bo')
ax3.set_xlabel('Duty Cycle (%)', fontsize=10)
ax3.set_ylabel('REsistance (k\u03A9)', fontsize=10)
ax3.set_title("Photocell Voltage")
ax3.grid(visible=True, which='major', color='k', linestyle='-')
ax3.grid(visible=True, which='minor', color='r', linestyle='-', alpha=0.2)
ax3.minorticks_on()

figure1.subplots_adjust(hspace=0.5)
fontStyle = tkFont.Font(family="Lucida Grande", size=14)
var = StringVar()
label = Label(root, textvariable=var, font=fontStyle, bg="lightgrey")
label.place(x=100, y=400)


def update_graph(arduino, DC, V_res, V_pc, I, R):
    try:
        arduino.reset_input_buffer()
        arduino.reset_output_buffer()
        duty_cycle = len(DC) + 1
        DC.append(duty_cycle / 255 * 100)
        var.set("Duty Cycle: {:.2f} %".format(DC[-1]))
        arduino.write(b'1')
        arduino.flush()
        time.sleep(0.5)  
        data = arduino.read_until() 
        a = int(data.decode("ascii").strip())
        V_res.append(a / 1023 * 5)
        V_pc.append(5 - V_res[-1])
        I.append(V_res[-1] / 5000 * 1000)
        R.append(V_pc[-1] / I[-1] if I[-1] != 0 else 0)

        line1.set_data(DC, V_pc)
        ax1.set_xlim(0, max(DC) + 10)
        ax1.set_ylim(0, max(V_pc) + 1)

        line2.set_data(DC, I)
        ax2.set_xlim(0, max(DC) + 10)
        ax2.set_ylim(0, max(I) + 0.1)

        line3.set_data(DC, R)
        ax3.set_xlim(0, max(DC) + 10)
        ax3.set_ylim(0, max(R) + 10)

        fig1.draw()
    except ValueError as e:
        messagebox.showerror("Error", "Invalid data received")
    except serial.SerialException as e:
        messagebox.showerror("Serial Error", str(e))
    else:
        if len(DC) < 255:
            root.after(100, lambda: update_graph(arduino, DC, V_res, V_pc, I, R))  

def startCallBack():
    arduino = pySerial('COM6', 9600, timeout=5)
    time.sleep(2) 
    DC = []
    V_res = []
    V_pc = []
    I = []
    R = []
    update_graph(arduino, DC, V_res, V_pc, I, R)



# Create Start Button
start_button = Button(root, text = "Start", font=fontStyle, height=2,
                      width=5, command = startCallBack)
start_button.place(x=100, y=450)
root.mainloop()




