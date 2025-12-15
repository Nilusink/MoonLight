import matplotlib.pyplot as plt


with open("arduino_data.csv", "r") as f:
    lines = f.readlines()

data = [line.strip().split(",") for line in lines[1:]]
times = [float(row[0]) for row in data]
values = [int(row[1]) for row in data]


plt.plot(times, values, 'b-o')
plt.ylim(-0.2, 1.2)
plt.xlabel('Time')
plt.ylabel('Value')
plt.title('ir values')
plt.show()
