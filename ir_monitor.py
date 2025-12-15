import serial
# import matplotlib.pyplot as plt
# import matplotlib.animation as animation
import time
import csv

# -------------------- Configuration --------------------
SERIAL_PORT = 'COM8'  # Replace with your port (e.g., '/dev/ttyUSB0' on Linux)
BAUD_RATE = 112500
CSV_FILE = 'arduino_data.csv'
MAX_POINTS = 1000  # number of points to display on the live plot

# -------------------- Initialize --------------------
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

timestamps = []
values = []

# Open CSV file in append mode
csv_file = open(CSV_FILE, 'w', newline='')
csv_writer = csv.writer(csv_file)
csv_writer.writerow(['Timestamp', 'Value'])  # header

# -------------------- Matplotlib Setup --------------------
# fig, ax = plt.subplots()
# line, = ax.plot([], [], 'b-o')
# ax.set_ylim(-0.2, 1.2)
# ax.set_xlabel('Time')
# ax.set_ylabel('Value')
# ax.set_title('Live Arduino Data')

# -------------------- Animation function --------------------
def update(frame):
    global timestamps, values
    try:
        if ser.in_waiting > 1:
            line_raw = ser.readline().decode('utf-8').strip()
            # print(line_raw)
            if line_raw in ['0', '1']:
                value = int(line_raw)
                now = time.time()

                # Save to CSV
                csv_writer.writerow([now, value])
                csv_file.flush()

                # Append to lists
                timestamps.append(now)
                values.append(value)

                # Keep last MAX_POINTS for plotting
                timestamps = timestamps[-MAX_POINTS:]
                values = values[-MAX_POINTS:]

                # # Update plot
                # line.set_data(range(len(values)), values)
                # ax.set_xlim(0, len(values) if len(values) > 10 else 10)

    except Exception as e:
        print("Error:", e)

    # return line,

# -------------------- Run animation --------------------
# ani = animation.FuncAnimation(fig, update, interval=1)  # update every 100 ms

# plt.show()

try:
    while True:
        update(None)

except KeyboardInterrupt:
    pass

# -------------------- Cleanup --------------------
ser.close()
csv_file.close()
