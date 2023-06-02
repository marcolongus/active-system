import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np
from matplotlib.collections import PatchCollection
from matplotlib.patches import Polygon, Circle
from matplotlib.animation import FuncAnimation

colores = ['blue', 'red', 'green']
archivo = "main/data/animacion.txt"

N = 15
L = 20
np_steps = 10_000

animation_step = np.loadtxt(archivo, usecols=2, max_rows=1)

fig, ax = plt.subplots()
plt.xlabel("x coordinate")
plt.ylabel("y coordinate")
plt.axis('square')
plt.grid()
plt.xlim(-1, L+1)
plt.ylim(-1, L+1)

# Load data once
data = np.loadtxt(archivo, skiprows=0, max_rows=N*np_steps).reshape(np_steps, N, -1)
x_data = data[:, :, 0]
y_data = data[:, :, 1]
estado_data = data[:, :, 3].astype(int)

circles = [Circle((0, 0), 1, alpha=0.7, fc=colores[0]) for _ in range(N)]
circ_collection = PatchCollection(circles)


def update(num):
    x = x_data[num]
    y = y_data[num]
    estado = estado_data[num]

    for j, circ in enumerate(circles):
        circ.center = (x[j], y[j])
        circ.set_facecolor(colores[estado[j]])

    circ_collection.set_array(np.array(estado))

ani = FuncAnimation(fig, update, frames=np_steps, interval=100, blit=False)
ani.save('animation.mp4', dpi=100)

plt.show()