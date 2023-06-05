import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np

import concurrent.futures


colores = ["blue", "red", "green"]
archivo = "data/animacion.txt"

N = 1
L = 150
np_steps = 30_000
delta = 20

print(f"Angle: {np.arctan(L/delta) * 180 / np.pi}")

animation_step = np.loadtxt(archivo, usecols=2, max_rows=1)

x_r = np.arange(0, L, 10)

y_r = lambda x: (L / delta) * (x - L / 4)
y_perp = lambda x: (-delta / L) * (x - L / 4) + 150


def generate_data(start_frame, num_frames):
    for i in range(start_frame, num_frames, 1):
        num = i
        x = np.loadtxt(archivo, usecols=0, skiprows=num * N, max_rows=N)
        y = np.loadtxt(archivo, usecols=1, skiprows=num * N, max_rows=N)
        estado = np.loadtxt(archivo, usecols=3, skiprows=num * N, max_rows=N, dtype=int)
        yield num, x, y, estado


def update(num, x, y, estado, fig, ax):

    # ax.cla()
    ax.set_xlabel("x coordinate")
    ax.set_ylabel("y coordinate")
    ax.axis("square")
    ax.grid()
    ax.set_xlim(-1, L + 1)
    ax.set_ylim(-1, L + 1)

    # Refraction wall
    ax.plot(x_r, y_r(x_r), color="black", linestyle="dashed")
    ax.plot(x_r, y_r(x_r - 20), color="black", linestyle="dashed")
    ax.plot(x_r, y_r(x_r - 10), color="black", linestyle="dashed")

    # Perpendiccular to refraction wall
    ax.plot(x_r, y_perp(x_r), color="black", linestyle="dashed")
    ax.plot(x_r, y_perp(x_r) + 20, color="black", linestyle="dashed")
    ax.plot(x_r, y_perp(x_r) + 10, color="black", linestyle="dashed")

    for j in range(N):
        # disk = patches.Circle((x[j], y[j]), 1, alpha=0.7, fc=colores[estado[j]])
        disk = patches.Circle((x, y), 1, alpha=0.7, fc=colores[estado])
        ax.add_patch(disk)

    fig.savefig(f"pic{num}.png", dpi=100)


if __name__ == "__main__":

    parallel = False

    if parallel:
        from multiprocessing import freeze_support

        freeze_support()

        fig, ax = plt.subplots()
        with concurrent.futures.ProcessPoolExecutor() as executor:
            for num, x, y, estado in generate_data(0, np_steps):
                results.append(executor.submit(update, num, x, y, estado, fig, ax))

            for result in concurrent.futures.as_completed(results):
                try:
                    print(result.result())
                except Exception as e:
                    print(f"Exception occurred: {e}")

    else:
        fig, ax = plt.subplots()
        for num, x, y, estado in generate_data(0, np_steps):
            print(num)
            update(num, x, y, estado, fig, ax)

        plt.close(fig)
