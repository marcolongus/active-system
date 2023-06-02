import concurrent.futures
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np
import alphashape

from descartes import PolygonPatch

colores = ['blue', 'red', 'green']
archivo = "data/animacion.txt"

N = 15_000
L = 475
np_steps = 10_000

delta = 600

animation_step = np.loadtxt(archivo, usecols=2, max_rows=1)

def generate_data(start_frame, num_frames):
    for i in range(start_frame, num_frames, 100):
        num = i
        x = np.loadtxt(archivo, usecols=0, skiprows=num * N, max_rows=N)
        y = np.loadtxt(archivo, usecols=1, skiprows=num * N, max_rows=N)
        estado = np.loadtxt(archivo, usecols=3, skiprows=num * N, max_rows=N, dtype=int)

        yield num, x, y, estado

def convave_hull(num, x, y, estado):
    points = np.column_stack((x,y))
    mask = np.where(estado>0)
    concave_flag = False
    try:
        alpha = alphashape.optimizealpha(points[mask])
        hull = alphashape.alphashape(points[mask], alpha)
        hull_exterior_points = np.array(hull.exterior.coords)
        concave_flag = True
        with open('area.txt', 'a') as file:
            file.write(f"{round(hull.area, 1)} {(num + 1) * animation_step} {alpha} \n")
        print(f"(Area, alpha, time) = ({hull.area}, {alpha}, {num})")
    except Exception as error:
        print("Error: {error}")
        return None
    return hull, concave_flag


def update(num, x, y, estado):
    fig, ax = plt.subplots()
    print(num)
    
    # Calculate concave hull
    concave_flag = False
    #hull, concave_falg = convave_hull(num, x, y, estado)

    plt.xlabel("x coordinate")
    plt.ylabel("y coordinate")
    plt.axis('square')
    plt.grid()
    plt.xlim(-1, L+1)
    plt.ylim(-1, L+1)

    if concave_flag:
        ax.add_patch(PolygonPatch(hull, fill=False, color='black', linewidth=0.8))

    for j in range(N):
        circ = patches.Circle((x[j], y[j]), 1, alpha=0.7, fc=colores[estado[j]])
        ax.add_patch(circ)
        
    x_r = np.arange(0, L, 10)
    y_r = lambda x : (L/delta) * (x - L/5)
    y_perp = lambda x : (-delta/L) * (x - L/5) + 150

    plt.plot(x_r, y_r(x_r), color='black',linestyle="dashed")
    plt.plot(x_r, y_perp(x_r), color='black',linestyle="dashed")
    plt.plot(x_r, y_perp(x_r)+100, color='black',linestyle="dashed")
    plt.plot(x_r, y_perp(x_r)+200, color='black',linestyle="dashed")


    plt.plot(x_r, y_r(x_r-100), color='black',linestyle="dashed")
    plt.plot(x_r, y_r(x_r-200), color='black',linestyle="dashed")   

    plt.savefig(f"video/pic{num}.png", dpi=100)
    plt.close()


if __name__ == '__main__':

    print(np.arctan(L/30)*180/ np.pi)
    for num, x, y, estado in generate_data(0, np_steps):
        update(num, x, y, estado)


    # from multiprocessing import freeze_support
    # freeze_support()

    # with concurrent.futures.ProcessPoolExecutor() as executor:
    #     results = []

    #     for num, x, y, estado in generate_data(0, np_steps):
    #         results.append(executor.submit(update, num, x, y, estado))

    #     for result in concurrent.futures.as_completed(results):
    #         try:
    #             result.result()
    #         except Exception as e:
    #             print(f"Exception occurred: {e}")