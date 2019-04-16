import sys
import sympy as sm

#script name | input | output
req_arguments = 3

if(len(sys.argv) != req_arguments):
    print('Wrong number of args!\n')
    exit()


print('==============KakaiNet====================\n'
      'Welcome to '+sys.argv[0][sys.argv[0].rfind('/')+1:]+' script.\nInput: '+sys.argv[1]+'\nOutput: '+sys.argv[2]+'\n')

itr = 0
with open(sys.argv[1], 'r') as input:
    for line in input.readlines():
        sm.preview('$$'+line+'$$',
                   dvioptions=["-T", "tight", "-z", "0", "--truecolor", "-D 600", "-bg", "Transparent"],
                   viewer='file', filename='output'+str(itr)+'.png', euler=True)
        itr = itr + 1


print('Generating finished...\n'+'Produced '+str(itr)+' images')


