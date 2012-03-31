# this example shows how you can emit a dollar sign in a makefile rule...

all:
	echo $$ 

dont_work:
	echo $
