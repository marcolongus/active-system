import os
import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
from sklearn.metrics import r2_score, mean_squared_error


def get_vel(source):
	with open(source, 'r') as file:
		lines = file.readlines()
		for i, line in enumerate(lines):
			if i==3:
				vel = float(line.strip().split(" ")[-1])
	return vel

def get_sim_data(source, row=3):
	with open(source, 'r') as file:
		lines = file.readlines()
		for i, line in enumerate(lines):
			if i==row:
				vel = float(line.strip().split(" ")[-1])
	return vel


topdir = ".\data/density"

L = 475
area = L ** 2
plt.ylabel("Area")
plt.xlabel("Time")


wave_speed = []
agent_speed = []
density_system = []
tumbling_system = []

for dirpath, dirnames, filenames in os.walk(topdir, topdown=True):
	for dirname in dirnames:
		file_path = os.path.join(dirpath, dirname, "area.txt")
		label_path = os.path.join(dirpath, dirname, "simulation_data.txt")
		try:
			density = get_sim_data(label_path, 1)
			speed = get_sim_data(label_path, 3) 
			tumbling = get_sim_data(label_path, 5)
			print(tumbling)

			data = np.loadtxt(file_path)
			animation_step = np.loadtxt(os.path.join(dirpath, dirname, "animacion.txt"), usecols=2, max_rows=1)
			
			# PLOT FIGURE
			x = (data[:, 1] + 1) * animation_step
			y = data[:, 0] 

			plt.plot(x, y, label=float(tumbling))
			plt.scatter(x, y)

			# FITTING LINE
			x = x.reshape((-1, 1))
			model = LinearRegression()
			model.fit(x, y)

			print(50 * "-")
			print(file_path)
			print(50 * "-")
			print(f"Step : {animation_step}")
			print("Coefficients:", model.coef_)
			print("Intercept:", model.intercept_)
			
			# SAVING RESULTS
			agent_speed.append(float(speed))
			wave_speed.append(model.coef_[0] / (2 * L) )
			density_system.append(density)
			tumbling_system.append(1/tumbling)

			# Calculate the R-squared and MSE metrics
			y_pred = model.predict(x)
			r2 = r2_score(y, y_pred)
			mse = mean_squared_error(y, y_pred)

			print("R-squared:", r2)
			print("Mean squared error:", mse)
			print("\n\n")

		except:
			print(file_path)
			print("File not found")

plt.legend(title='tumbling - v=0.05')
plt.show()

#############################
# Density
#############################

plt.xlabel("N")
plt.ylabel("wave_speed")
plt.plot(density_system, wave_speed)
plt.scatter(density_system, wave_speed)

# plt.semilogx()

#############################
# Velocity
#############################

# agent_speed[-2], agent_speed[-1] = agent_speed[-1], agent_speed[-2]
# wave_speed[-2], wave_speed[-1] = wave_speed[-1], wave_speed[-2]

# plt.xlabel('agent_speed')
# plt.ylabel('wave_speed')
# plt.plot(agent_speed[:-2], wave_speed[:-2])
# plt.scatter(agent_speed[:-2], wave_speed[:-2])

# x = np.array(agent_speed[:-2])
# y = wave_speed[:-2]

# x = x.reshape((-1, 1))
# model = LinearRegression()
# model.fit(x, y)

# print("FITTING RESULTS:")
# print("Coefficients:", model.coef_)
# print("Intercept:", model.intercept_)

# y_pred = model.predict(x)
# r2 = r2_score(y, y_pred)
# mse = mean_squared_error(y, y_pred)

# print("R-squared:", r2)
# print("Mean squared error:", mse)

# plt.title(f'slope {round(model.coef_[0], 2)}, MSE {round(mse, 2)}')

#plt.semilogx()

############################
# Tumbling
############################

# plt.xlabel("tumbling")
# plt.ylabel("wave_speed")
# plt.plot(tumbling_system, wave_speed)
# plt.scatter(tumbling_system, wave_speed)
# plt.semilogx()

plt.show()