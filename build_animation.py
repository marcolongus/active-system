import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np

colores = ['blue', 'red', 'green']
archivo = "tumbling_test/data/animacion.txt"

N = 15
L = 30
np_steps = 10_000

animation_step = np.loadtxt(archivo, usecols=2, max_rows=1)

def generate_data(start_frame, num_frames):
    for i in range(start_frame, num_frames, 1):
        num = i
        x = np.loadtxt(archivo, usecols=0, skiprows=num * N, max_rows=N)
        y = np.loadtxt(archivo, usecols=1, skiprows=num * N, max_rows=N)
        estado = np.loadtxt(archivo, usecols=3, skiprows=num * N, max_rows=N, dtype=int)
        yield num, x, y, estado

def update(num, x, y, estado):
    fig, ax = plt.subplots()

    plt.xlabel("x coordinate")
    plt.ylabel("y coordinate")
    plt.axis('square')
    plt.grid()
    plt.xlim(-1, L+1)
    plt.ylim(-1, L+1)


    circ = [patches.Circle((x[j], y[j]), 1, alpha=0.7, fc=colores[estado[j]]) for j in range(N)]
    [ax.add_patch(part) for part in circ]

    plt.savefig(f"pic{num}.png", dpi=100)
    plt.close()


if __name__ == '__main__':
    for num, x, y, estado in generate_data(0, np_steps):
        update(num, x, y, estado)

