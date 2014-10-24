#!/usr/bin/python3

'''
This script removes the first dir and suffix of a list of files given to it
'''

###########
# imports #
###########
import sys # for argv
import os.path # splitext
import os # for sep

##############
# parameters #
##############

#############
# functions #
#############

########
# code #
########
result=[]
for f in sys.argv[1:]:
	r=os.path.splitext(os.sep.join(f.split(os.sep)[1:]))[0]
	result.append(r)
print(' '.join(result), end='')
