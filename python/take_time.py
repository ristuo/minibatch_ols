import subprocess
import time

sum_times = 0
n = 100
for i in range(n):
    start_time = time.time()
    subprocess.run("./main", stdout = subprocess.PIPE)
    end_time = time.time()
    sum_times += end_time - start_time

print("Took %s seconds on average\n" % (sum_times/n))
