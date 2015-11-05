from time import gmtime, strftime
import os

cdir = []
cppdir = []
tmp = ""

for root, dirnames, filenames in os.walk('cset'):
	for filename in filenames:
		tmp = os.path.join(root, filename);
		tmp = os.path.dirname(os.path.realpath(tmp))
		if filename.endswith(('.c')):
			cdir.append(tmp)
		elif filename.endswith(('.cpp')):
			cppdir.append(tmp)


os.system("echo '\n###Test C " + strftime("%Y-%m-%d %H:%M", gmtime()) + "###\n' > result.txt")
for d in cdir:
	if not "ts" in d and not "support" in d and not "test" in d and not "prof" in d:
		os.system(d + "/main 4 >> result.txt")

os.system("echo '\n###Test C++ " + strftime("%Y-%m-%d %H:%M", gmtime()) + "###\n' >> result.txt")
for d in cppdir:
	if not "ts" in d and not "support" in d and not "test" in d and not "prof" in d:
		os.system(d + "/main 4 >> result.txt")
