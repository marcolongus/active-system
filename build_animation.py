import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np

colores = ['blue', 'red', 'green']
archivo = "data/animacion.txt"

N = 100
L = 50
np_steps = 30_000

animation_step = np.loadtxt(archivo, usecols=2, max_rows=1)

def generate_data(start_frame, num_frames):
    for i in range(start_frame, num_frames, 1):
        num = i
        x = np.loadtxt(archivo, usecols=0, skiprows=num * N, max_rows=N)
        y = np.loadtxt(archivo, usecols=1, skiprows=num * N, max_rows=N)
        estado = np.loadtxt(archivo, usecols=3, skiprows=num * N, max_rows=N, dtype=int)
        yield num, x, y, estado

def update(num, x, y, estado, fig, ax):
    ax.cla()

    ax.set_xlabel("x coordinate")
    ax.set_ylabel("y coordinate")
    ax.axis('square')
    ax.grid()
    ax.set_xlim(-1, L+1)
    ax.set_ylim(-1, L+1)


    for j in range(N):
        disk = patches.Circle((x[j], y[j]), 1, alpha=0.7, fc=colores[estado[j]])
        ax.add_patch(disk) 

    fig.savefig(f"pic{num}.png", dpi=100)


if __name__ == '__main__':
    fig, ax = plt.subplots()
    
    for num, x, y, estado in generate_data(0, np_steps):
        print(num)
        update(num, x, y, estado, fig, ax)

    plt.close(fig)