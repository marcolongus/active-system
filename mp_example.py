import multiprocessing as mp
import concurrent.futures
import time
import math

def do_somthing(seconds):
	print(f"Sleeping {seconds} second...")
	time.sleep(seconds)
	return f"Done Sleeping {seconds}"

if __name__ == '__main__':
	mp.freeze_support()

	with concurrent.futures.ProcessPoolExecutor() as executor:
		results = [executor.submit(do_somthing, 1) for _ in range(200)]

		for f in concurrent.futures.as_completed(results):
			print(f.result())
	

	#processes = []

	# for _ in range(6):
	# 	p = mp.Process(target=do_somthing, args=[2])
	# 	p.start()
	# 	processes.append(p)

	# for process in processes:
	# 	process.join()


