#!/usr/bin/python

import sys # for sys.argv

old_lic=open("support/old_license.txt").read()
new_lic=open("support/license.txt").read()

#print old_lic
#print new_lic
#sys.exit(0)

for x in sys.argv[1:]:
	try:
		in_f=open(x)
		f=in_f.read()
		in_f.close()
		#print f, old_lic
		if f.startswith(old_lic):
			f=new_lic+f[len(old_lic):]
			#print f
			in_f=open(x,"wb")
			in_f.write(f)
			in_f.close()
			print "file",x,"was license replaced..."
		else:
			print "file",x,"does not start with the right license"
	except:
		pass
