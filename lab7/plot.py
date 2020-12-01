import csv
import matplotlib.pyplot as plt

x = []
y1 = []
y2 = []
for i in range(0, 8000):
    x.append(i)

with open("hill_climb.tsv", 'rt') as file:
    plots = csv.reader(file, delimiter='\t')
    for row in plots:
        y1.append((row[5]))
with open("optymlsizacja_rand.tsv", 'rt') as file:
    plots = csv.reader(file, delimiter='\t')
    for row in plots:
        y2.append((row[5]))


fig, ax = plt.subplots(2)
ax[0].plot(x, y1, ':r', color='green', label="hill")
ax[1].plot(x, y2, ':r', color='red', label="random")
ax[0].invert_yaxis()
ax[1].invert_yaxis()
ax[0].legend()
ax[1].legend()
plt.show()