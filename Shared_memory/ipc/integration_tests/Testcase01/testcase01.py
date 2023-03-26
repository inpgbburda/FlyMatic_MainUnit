import subprocess
import os
from multiprocessing import shared_memory as shm
import time
import threading

os.chdir('build')

cmake_command = ['cmake', '..']

subprocess.run(cmake_command)

build_command = ['make', '-j4']

subprocess.run(build_command)

shme = shm.SharedMemory(name = 'shm', create=True, size=1024)

subprocess.run(['pwd'])

print("start")

try:
    p1 = subprocess.run(["./bin/Testcase01", "/shm"])
except  Exception as e:
    print(f"Error {e}")


# shme.unlink()
# shme.close()