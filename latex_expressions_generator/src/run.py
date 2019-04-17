import os
import sys

#Small script to divide inputs into parts
expressions_number = sys.argv[1]
batch_size = sys.argv[2]

if(int(expressions_number) % int(batch_size) != 0):
	print('Wrong input')
	exit()

for i in range(0,int(int(expressions_number)/int(batch_size))):
	print('Generating %d part...' % (i+1))
	os.system("./l1_1.native "+expressions_number+" > out/raw_tex"+str(i+1)+".in")
	

print('done')