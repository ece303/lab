import streamlit as st
import pandas as pd
import serial
import time

# serial connect
arduino = serial.Serial('COM6', 9600, timeout=0.1)
time.sleep(2) 

# make title
st.title('Photocell Duty Cycle vs Voltage, Current, Resistance')

# make dutycycle number display
duty_cycle_number = st.empty()

# create dataframes for 3 charts
data_volt = pd.DataFrame({
    'Duty Cycle': [],
    'Voltage': []
})
data_current = pd.DataFrame({
    'Duty Cycle': [],
    'Current': []
})
data_resistance = pd.DataFrame({
    'Duty Cycle': [],
    'Resistance': []
})

# create 3 Streamlit charts
chart_volt = st.line_chart(data_volt)
chart_current = st.line_chart(data_current)
chart_resistance = st.line_chart(data_resistance)


if st.button('Start'): #if button clicked
    for i in range(0, 256):  
        # pass the value to arduino
        arduino.write(f'{i}\n'.encode())
        time.sleep(0.1)
        
        # get data from arduino
        if arduino.inWaiting() > 0:
            raw_data = arduino.readline().decode().strip()
            if raw_data:  
                
                sensor_value = int(raw_data)
                duty_cycle = (i / 255.0) * 100
                duty_cycle_number.write(f'Duty Cycle: {duty_cycle:.2f}%')

                # calculate voltage, current, resistance
                voltage = 5 - sensor_value * (5.0 / 1023.0)
                current = sensor_value * (5.0 / 1023.0) / 5000 * 1000.0
                resistance = voltage / current
                
                # make dataframe for adding new data
                new_data_volt = pd.DataFrame({'Duty Cycle': [duty_cycle], 'Voltage': [voltage]})
                new_data_current = pd.DataFrame({'Duty Cycle': [duty_cycle], 'Current': [current]})
                new_data_resistance = pd.DataFrame({'Duty Cycle': [duty_cycle], 'Resistance': [resistance]})
                
                chart_volt.add_rows(new_data_volt.set_index('Duty Cycle'))
                chart_current.add_rows(new_data_current.set_index('Duty Cycle'))
                chart_resistance.add_rows(new_data_resistance.set_index('Duty Cycle'))

# close arduino
arduino.close()
