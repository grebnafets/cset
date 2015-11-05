#TODO: Someday when I feel like it...

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



for d in cdir:
	print("c: %s" % d)
	
print "\n"

for d in cppdir:
	print("cpp :%s" % d)
