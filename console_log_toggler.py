import os
import re

mode = int(input("Press 1 to remove the console log\nPress 2 to add the console log\n :"))

files = (os.listdir("src/"))

if mode == 1:
	print("Commedted out the log\n\n")
	for fi in files:
		print(fi)
		f = open("src/"+fi, 'r')
		data = f.read()
		f.close()
		data = re.sub("cout", "//cout", data)

		f = open("src/"+fi , 'w')
		f.write(data)
		f.close()
	os.system("make")
	os.system("make clean")
	os.system("make")

elif mode == 2:
	print("Uncommedted the log\n\n")
	for fi in files:
		print(fi)
		f = open("src/"+fi, 'r')
		data = f.read()
		f.close()

		data = re.sub("//cout", "cout", data)
		f = open("src/"+fi , 'w')
		f.write(data)
		f.close()
	os.system("make")
	os.system("make clean")
	os.system("make")

else:
	print("Wrong Input")