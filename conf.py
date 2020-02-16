import os

os.system("cat /proc/cpuinfo | grep 'model name' > tmp.txt")

f = open("tmp.txt", 'r')
data = f.readline()
f.close()
os.system("rm tmp.txt")

arc = data.split(":")[1].split()[0].strip()

print("Processor Type: " + data.split(":")[1].strip())

if arc[:3].lower() == "arm":
	SYS_ARC = "arm-linux-gnueabihf"
	print("Configured for ARM Architecture")
elif arc[:5].lower() == "intel":
	SYS_ARC = "x86_64-linux-gnu"
	print("Configured for Intel Architecture")

f = open("Makefile_h", "w")
f.write("SYS_ARC = " + SYS_ARC)
f.close()